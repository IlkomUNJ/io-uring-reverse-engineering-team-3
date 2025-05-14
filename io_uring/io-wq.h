#ifndef INTERNAL_IO_WQ_H
#define INTERNAL_IO_WQ_H

#include <linux/refcount.h>
#include <linux/io_uring_types.h>

struct io_wq;

enum {
	IO_WQ_WORK_CANCEL	= 1,
	IO_WQ_WORK_HASHED	= 2,
	IO_WQ_WORK_UNBOUND	= 4,
	IO_WQ_WORK_CONCURRENT	= 16,

	IO_WQ_HASH_SHIFT	= 24,	/* upper 8 bits are used for hash key */
};

enum io_wq_cancel {
	IO_WQ_CANCEL_OK,	/* cancelled before started */
	IO_WQ_CANCEL_RUNNING,	/* found, running, and attempted cancelled */
	IO_WQ_CANCEL_NOTFOUND,	/* work not found */
};

typedef struct io_wq_work *(free_work_fn)(struct io_wq_work *);
typedef void (io_wq_work_fn)(struct io_wq_work *);

struct io_wq_hash {
	refcount_t refs;
	unsigned long map;
	struct wait_queue_head wait;
};

/**
 * Decrements the reference count for the specified io_wq_hash.
 * Frees the hash if the reference count reaches zero.
 */
static inline void io_wq_put_hash(struct io_wq_hash *hash)
{
	if (refcount_dec_and_test(&hash->refs))
		kfree(hash);
}

struct io_wq_data {
	struct io_wq_hash *hash;
	struct task_struct *task;
	io_wq_work_fn *do_work;
	free_work_fn *free_work;
};

/**
 * Creates a new io_wq with the specified bounded and unbounded worker limits.
 * Initializes the io_wq with the provided data and ensures it is ready for use.
 */
struct io_wq *io_wq_create(unsigned bounded, struct io_wq_data *data);

/**
 * Starts the exit process for the specified io_wq.
 * Ensures that the io_wq is properly cleaned up and ready for destruction.
 */
void io_wq_exit_start(struct io_wq *wq);

/**
 * Exits and destroys the specified io_wq.
 * Ensures that all resources associated with the io_wq are properly released.
 */
void io_wq_put_and_exit(struct io_wq *wq);

/**
 * Enqueues a work item into the specified io_wq.
 * Ensures that the work item is properly added to the workqueue and ready for processing.
 */
void io_wq_enqueue(struct io_wq *wq, struct io_wq_work *work);

/**
 * Hashes a work item for the specified value.
 * Ensures that work items with the same hash value are not processed in parallel.
 */
void io_wq_hash_work(struct io_wq_work *work, void *val);

/**
 * Sets the CPU affinity for the specified io_uring_task.
 * Configures the task to run on the specified CPUs.
 */
int io_wq_cpu_affinity(struct io_uring_task *tctx, cpumask_var_t mask);

/**
 * Sets the maximum number of unbounded workers for the specified io_wq.
 * Returns the previous maximum number of unbounded workers.
 */
int io_wq_max_workers(struct io_wq *wq, int *new_count);

/**
 * Checks if the current worker has been stopped.
 * Returns true if the worker is no longer active, false otherwise.
 */
bool io_wq_worker_stopped(void);

/**
 * Checks if the specified work flags indicate that the work is hashed.
 * Returns true if the work is hashed, false otherwise.
 */
static inline bool __io_wq_is_hashed(unsigned int work_flags)
{
	return work_flags & IO_WQ_WORK_HASHED;
}

/**
 * Checks if the specified work item is hashed.
 * Returns true if the work is hashed, false otherwise.
 */
static inline bool io_wq_is_hashed(struct io_wq_work *work)
{
	return __io_wq_is_hashed(atomic_read(&work->flags));
}

typedef bool (work_cancel_fn)(struct io_wq_work *, void *);

/**
 * Cancels work items in the specified io_wq based on the provided
 * cancellation function and data. Returns the cancellation status.
 */
enum io_wq_cancel io_wq_cancel_cb(struct io_wq *wq, work_cancel_fn *cancel,
					void *data, bool cancel_all);

/**
 * Marks the specified worker as sleeping.
 * Updates the worker's state to indicate it is not actively processing work.
 */
#if defined(CONFIG_IO_WQ)
extern void io_wq_worker_sleeping(struct task_struct *);
extern void io_wq_worker_running(struct task_struct *);
#else
static inline void io_wq_worker_sleeping(struct task_struct *tsk)
{
}
#endif

/**
 * Marks the specified worker as running.
 * Updates the worker's state to indicate it is actively processing work.
 */
#if defined(CONFIG_IO_WQ)
extern void io_wq_worker_running(struct task_struct *);
#else
static inline void io_wq_worker_running(struct task_struct *tsk)
{
}
#endif

/**
 * Checks if the current task is an io_wq worker.
 * Returns true if the current task is an io_wq worker, false otherwise.
 */
static inline bool io_wq_current_is_worker(void)
{
	return in_task() && (current->flags & PF_IO_WORKER) &&
		current->worker_private;
}
#endif
