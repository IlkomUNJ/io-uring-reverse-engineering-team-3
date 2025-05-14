// SPDX-License-Identifier: GPL-2.0

struct io_sq_data {
	refcount_t		refs;
	atomic_t		park_pending;
	struct mutex		lock;

	/* ctx's that are using this sqd */
	struct list_head	ctx_list;

	struct task_struct	*thread;
	struct wait_queue_head	wait;

	unsigned		sq_thread_idle;
	int			sq_cpu;
	pid_t			task_pid;
	pid_t			task_tgid;

	u64			work_time;
	unsigned long		state;
	struct completion	exited;
};

/**
 * Creates the SQPOLL thread and initializes the SQPOLL data structure.
 * Returns 0 on success or an error code on failure.
 */
int io_sq_offload_create(struct io_ring_ctx *ctx, struct io_uring_params *p);

/**
 * Finalizes the SQPOLL thread for a given context by parking it, removing the context,
 * updating the idle timeout, and un-parking the thread.
 */
void io_sq_thread_finish(struct io_ring_ctx *ctx);

/**
 * Stops the SQPOLL thread by setting the stop state and waking it up.
 * Waits for the thread to exit before returning.
 */
void io_sq_thread_stop(struct io_sq_data *sqd);

/**
 * Parks the SQPOLL thread by setting the park state and locking the SQPOLL data structure.
 * Ensures the thread is woken up if it is already parked.
 */
void io_sq_thread_park(struct io_sq_data *sqd);

/**
 * Unparks the SQPOLL thread by clearing the park state and waking it up.
 * Releases the lock on the SQPOLL data structure.
 */
void io_sq_thread_unpark(struct io_sq_data *sqd);

/**
 * Decrements the reference count of the SQPOLL data structure.
 * Stops the SQPOLL thread and frees the data structure if the reference count reaches zero.
 */
void io_put_sq_data(struct io_sq_data *sqd);

/**
 * Waits for space in the submission queue to become available.
 */
void io_sqpoll_wait_sq(struct io_ring_ctx *ctx);

/**
 * Sets the CPU affinity for the SQPOLL thread.
 * Returns 0 on success or an error code on failure.
 */
int io_sqpoll_wq_cpu_affinity(struct io_ring_ctx *ctx, cpumask_var_t mask);