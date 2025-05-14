#ifndef IOU_CORE_H
#define IOU_CORE_H

#include <linux/errno.h>
#include <linux/lockdep.h>
#include <linux/resume_user_mode.h>
#include <linux/kasan.h>
#include <linux/poll.h>
#include <linux/io_uring_types.h>
#include <uapi/linux/eventpoll.h>
#include "alloc_cache.h"
#include "io-wq.h"
#include "slist.h"
#include "filetable.h"
#include "opdef.h"

#ifndef CREATE_TRACE_POINTS
#include <trace/events/io_uring.h>
#endif

enum {
	IOU_OK			= 0, /* deprecated, use IOU_COMPLETE */
	IOU_COMPLETE		= 0,

	IOU_ISSUE_SKIP_COMPLETE	= -EIOCBQUEUED,

	/*
	 * The request has more work to do and should be retried. io_uring will
	 * attempt to wait on the file for eligible opcodes, but otherwise
	 * it'll be handed to iowq for blocking execution. It works for normal
	 * requests as well as for the multi shot mode.
	 */
	IOU_RETRY		= -EAGAIN,

	/*
	 * Requeue the task_work to restart operations on this request. The
	 * actual value isn't important, should just be not an otherwise
	 * valid error code, yet less than -MAX_ERRNO and valid internally.
	 */
	IOU_REQUEUE		= -3072,
};

struct io_wait_queue {
	struct wait_queue_entry wq;
	struct io_ring_ctx *ctx;
	unsigned cq_tail;
	unsigned cq_min_tail;
	unsigned nr_timeouts;
	int hit_timeout;
	ktime_t min_timeout;
	ktime_t timeout;
	struct hrtimer t;

#ifdef CONFIG_NET_RX_BUSY_POLL
	ktime_t napi_busy_poll_dt;
	bool napi_prefer_busy_poll;
#endif
};

/**
 * Checks if the completion queue (CQ) should wake up based on the
 * number of events or timeouts. Returns true if the CQ should wake up,
 * false otherwise.
 */
static inline bool io_should_wake(struct io_wait_queue *iowq)
{
	struct io_ring_ctx *ctx = iowq->ctx;
	int dist = READ_ONCE(ctx->rings->cq.tail) - (int) iowq->cq_tail;

	/*
	 * Wake up if we have enough events, or if a timeout occurred since we
	 * started waiting. For timeouts, we always want to return to userspace,
	 * regardless of event count.
	 */
	return dist >= 0 || atomic_read(&ctx->cq_timeouts) != iowq->nr_timeouts;
}

#define IORING_MAX_ENTRIES	32768
#define IORING_MAX_CQ_ENTRIES	(2 * IORING_MAX_ENTRIES)

/**
 * Calculates the total size of the submission and completion queue (SQ and CQ)
 * rings based on the provided flags, SQ entries, and CQ entries. This function
 * also determines the offset for the SQ array within the ring buffer.
 */
unsigned long rings_size(unsigned int flags, unsigned int sq_entries,
			 unsigned int cq_entries, size_t *sq_offset);

/**
 * Initializes the io_uring_params structure with the provided SQ and CQ
 * entries, and sets the appropriate flags. This function also calculates the
 * size of the rings based on the provided parameters.
 */
int io_uring_fill_params(unsigned entries, struct io_uring_params *p);

/**
 * Refills the completion queue entry (CQE) cache for the io_uring context.
 * This function ensures that the CQE cache has enough entries to handle
 * pending completions. If the `overflow` flag is set, it allows refilling
 * even when the CQ is in an overflow state.
 */
bool io_cqe_cache_refill(struct io_ring_ctx *ctx, bool overflow);

/**
 * Runs task work for the io_uring context and handles signals.
 * This function processes local work and task work, ensuring that
 * all pending operations are handled.
 */
int io_run_task_work_sig(struct io_ring_ctx *ctx);

/**
 * Handles a deferred request failure. Marks the request as failed, sets the
 * result code, and performs any necessary cleanup. If the operation has a
 * specific failure handler, it is invoked. Finally, the request is completed
 * as a deferred operation.
 */
void io_req_defer_failed(struct io_kiocb *req, s32 res);

/**
 * Posts an auxiliary completion queue entry (CQE) to the io_uring context.
 * This function adds a CQE with the specified user data, result, and flags
 * to the completion queue. It is used for auxiliary completions that are
 * not directly tied to a specific request.
 */
bool io_post_aux_cqe(struct io_ring_ctx *ctx, u64 user_data, s32 res, u32 cflags);

/**
 * Adds an auxiliary completion queue entry (CQE) to the io_uring context.
 * This function directly inserts a CQE with the specified user data, result,
 * and flags into the completion queue.
 */
void io_add_aux_cqe(struct io_ring_ctx *ctx, u64 user_data, s32 res, u32 cflags);

/**
 * Posts a completion queue entry (CQE) for the specified request. This
 * function marks the request as completed, sets the result code, and
 * adds the CQE to the completion queue. It also handles any necessary
 * cleanup and signaling.
 */
bool io_req_post_cqe(struct io_kiocb *req, s32 res, u32 cflags);

/**
 * Flushes the completion queue (CQ) ring, ensuring that all pending
 * operations are processed. This function handles deferred completions,
 * timeouts, and eventfd signaling as necessary.
 */
void __io_commit_cqring_flush(struct io_ring_ctx *ctx);

/**
 * Retrieves a file from the normal file descriptor table for the given request.
 * This function is used when the request does not use fixed files.
 */
struct file *io_file_get_normal(struct io_kiocb *req, int fd);

/**
 * Retrieves a file from the fixed file table for the given request.
 * This function is used when the request uses fixed files.
 */
struct file *io_file_get_fixed(struct io_kiocb *req, int fd,
			       unsigned issue_flags);

/**
 * Adds task work to the io_uring request. This function queues the task work
 * for execution with the specified flags.
 */
void __io_req_task_work_add(struct io_kiocb *req, unsigned flags);

/**
 * Adds task work to the io_uring request remotely. This function queues the
 * task work for execution in a remote context with the specified flags.
 */
void io_req_task_work_add_remote(struct io_kiocb *req, unsigned flags);

/**
 * Queues a request for task work execution. This function ensures that the
 * request is properly queued for processing in the task's context.
 */
void io_req_task_queue(struct io_kiocb *req);

/**
 * Completes a request as part of task work execution. This function finalizes
 * the request and performs any necessary cleanup.
 */
void io_req_task_complete(struct io_kiocb *req, io_tw_token_t tw);

/**
 * Queues a failed request for task work execution. This function handles
 * requests that encountered an error and ensures they are properly processed.
 */
void io_req_task_queue_fail(struct io_kiocb *req, int ret);

/**
 * Submits a request for task work execution. This function prepares the
 * request for execution and queues it for processing.
 */
void io_req_task_submit(struct io_kiocb *req, io_tw_token_t tw);

/**
 * Handles a list of task work items. This function processes the task work
 * items in the specified list and returns any remaining items.
 */
struct llist_node *io_handle_tw_list(struct llist_node *node, unsigned int *count, unsigned int max_entries);

/**
 * Runs task work for the specified io_uring task context. This function
 * processes the task work items in the context and returns any remaining
 * items.
 */
struct llist_node *tctx_task_work_run(struct io_uring_task *tctx, unsigned int max_entries, unsigned int *count);

/**
 * Executes task work for the io_uring task context. This function processes
 * pending task work items associated with the callback.
 */
void tctx_task_work(struct callback_head *cb);

/**
 * Cancels all pending requests in the io_uring context. This function ensures
 * proper cleanup of all pending operations and optionally cancels all requests
 * or specific ones based on the provided parameters.
 */
__cold void io_uring_cancel_generic(bool cancel_all, struct io_sq_data *sqd);

/**
 * Allocates a task context for the io_uring instance. This function initializes
 * the task context and associates it with the specified io_uring context.
 */
int io_uring_alloc_task_context(struct task_struct *task,
				struct io_ring_ctx *ctx);

/**
 * Adds a registered file to the io_uring task context. This function associates
 * a file with the task context and registers it within the specified range.
 */
int io_ring_add_registered_file(struct io_uring_task *tctx, struct file *file,
				     int start, int end);

/**
 * Queues a request to the io-wq (workqueue) for asynchronous execution.
 */
void io_req_queue_iowq(struct io_kiocb *req);

/**
 * Issues a poll operation for the specified request. This function handles
 * polling for events on the associated file descriptor.
 */
int io_poll_issue(struct io_kiocb *req, io_tw_token_t tw);

/**
 * Submits submission queue entries (SQEs) for processing. This function handles
 * the submission of multiple SQEs to the io_uring context.
 */
int io_submit_sqes(struct io_ring_ctx *ctx, unsigned int nr);

/**
 * Performs I/O polling for the io_uring context. This function handles polling
 * for events on the submission queue and completion queue.
 */
int io_do_iopoll(struct io_ring_ctx *ctx, bool force_nonspin);

/**
 * Flushes pending completions in the io_uring context. This function ensures
 * that all pending completions are processed and added to the completion queue.
 */
void __io_submit_flush_completions(struct io_ring_ctx *ctx);

/**
 * Frees a work item in the io-wq (workqueue). This function releases resources
 * associated with the specified work item.
 */
struct io_wq_work *io_wq_free_work(struct io_wq_work *work);

/**
 * Submits a work item to the io-wq (workqueue). This function queues the work
 * item for execution in the workqueue.
 */
void io_wq_submit_work(struct io_wq_work *work);

/**
 * Frees a request in the io_uring context. This function releases resources
 * associated with the specified request.
 */
void io_free_req(struct io_kiocb *req);

/**
 * Queues the next request in the io_uring context. This function handles
 * queuing of linked requests for execution.
 */
void io_queue_next(struct io_kiocb *req);

/**
 * Refills the task references for the io_uring task context. This function
 * ensures that the task context has enough references to handle pending
 * operations.
 */
void io_task_refs_refill(struct io_uring_task *tctx);

/**
 * Allocates and refills request structures for the io_uring context. This
 * function ensures that the context has enough request structures to handle
 * pending submissions.
 */
bool __io_alloc_req_refill(struct io_ring_ctx *ctx);

/**
 * Matches a request with the specified task context. This function checks if
 * the request belongs to the specified task context and optionally cancels all
 * requests.
 */
bool io_match_task_safe(struct io_kiocb *head, struct io_uring_task *tctx,
			bool cancel_all);

/**
 * Activates the poll workqueue for the io_uring context. This function enables
 * polling and wakes up any tasks waiting on the poll workqueue.
 */
void io_activate_pollwq(struct io_ring_ctx *ctx);

/**
 * Asserts that the completion queue (CQ) lock is held in the io_uring context.
 * This function is used for debugging purposes to ensure proper locking
 * behavior during CQ operations.
 */
static inline void io_lockdep_assert_cq_locked(struct io_ring_ctx *ctx)
{
#if defined(CONFIG_PROVE_LOCKING)
	lockdep_assert(in_task());

	if (ctx->flags & IORING_SETUP_DEFER_TASKRUN)
		lockdep_assert_held(&ctx->uring_lock);

	if (ctx->flags & IORING_SETUP_IOPOLL) {
		lockdep_assert_held(&ctx->uring_lock);
	} else if (!ctx->task_complete) {
		lockdep_assert_held(&ctx->completion_lock);
	} else if (ctx->submitter_task) {
		/*
		 * ->submitter_task may be NULL and we can still post a CQE,
		 * if the ring has been setup with IORING_SETUP_R_DISABLED.
		 * Not from an SQE, as those cannot be submitted, but via
		 * updating tagged resources.
		 */
		if (!percpu_ref_is_dying(&ctx->refs))
			lockdep_assert(current == ctx->submitter_task);
	}
#endif
}

/**
 * Checks if the io_uring context is running in compatibility mode.
 * This function determines whether the context is using a compatibility
 * layer for older architectures or configurations.
 */
static inline bool io_is_compat(struct io_ring_ctx *ctx)
{
	return IS_ENABLED(CONFIG_COMPAT) && unlikely(ctx->compat);
}

/**
 * Adds task work to the io_uring request. This function queues the task work
 * for execution with default flags.
 */
static inline void io_req_task_work_add(struct io_kiocb *req)
{
	__io_req_task_work_add(req, 0);
}

/**
 * Flushes pending completions in the io_uring context. This function ensures
 * that all pending completions are processed and added to the completion queue.
 */
static inline void io_submit_flush_completions(struct io_ring_ctx *ctx)
{
	if (!wq_list_empty(&ctx->submit_state.compl_reqs) ||
	    ctx->submit_state.cq_flush)
		__io_submit_flush_completions(ctx);
}

#define io_for_each_link(pos, head) \
	for (pos = (head); pos; pos = pos->link)

/**
 * Retrieves a completion queue entry (CQE) from the io_uring context.
 * If the CQE cache is empty, it attempts to refill the cache. This function
 * also handles overflow scenarios if the `overflow` flag is set.
 */
static inline bool io_get_cqe_overflow(struct io_ring_ctx *ctx,
					struct io_uring_cqe **ret,
					bool overflow)
{
	io_lockdep_assert_cq_locked(ctx);

	if (unlikely(ctx->cqe_cached >= ctx->cqe_sentinel)) {
		if (unlikely(!io_cqe_cache_refill(ctx, overflow)))
			return false;
	}
	*ret = ctx->cqe_cached;
	ctx->cached_cq_tail++;
	ctx->cqe_cached++;
	if (ctx->flags & IORING_SETUP_CQE32)
		ctx->cqe_cached++;
	return true;
}

/**
 * Retrieves a completion queue entry (CQE) from the io_uring context.
 * This function fetches a CQE from the cache if available. It does not
 * handle overflow scenarios.
 */
static inline bool io_get_cqe(struct io_ring_ctx *ctx, struct io_uring_cqe **ret)
{
	return io_get_cqe_overflow(ctx, ret, false);
}

/**
 * Retrieves an uncommitted completion queue entry (CQE) from the io_uring context.
 * This function increments the extra CQ count and marks the CQ for flushing.
 * It ensures that the retrieved CQE is not yet committed to the completion queue.
 */
static inline bool io_defer_get_uncommited_cqe(struct io_ring_ctx *ctx,
					       struct io_uring_cqe **cqe_ret)
{
	io_lockdep_assert_cq_locked(ctx);

	ctx->cq_extra++;
	ctx->submit_state.cq_flush = true;
	return io_get_cqe(ctx, cqe_ret);
}

/**
 * Fills a completion queue entry (CQE) with the specified request data.
 * This function copies the request data into the CQE and handles any
 * necessary cleanup. It returns true if the CQE was successfully filled,
 * false otherwise.
 */
static __always_inline bool io_fill_cqe_req(struct io_ring_ctx *ctx,
					    struct io_kiocb *req)
{
	struct io_uring_cqe *cqe;

	/*
	 * If we can't get a cq entry, userspace overflowed the
	 * submission (by quite a lot). Increment the overflow count in
	 * the ring.
	 */
	if (unlikely(!io_get_cqe(ctx, &cqe)))
		return false;


	memcpy(cqe, &req->cqe, sizeof(*cqe));
	if (ctx->flags & IORING_SETUP_CQE32) {
		memcpy(cqe->big_cqe, &req->big_cqe, sizeof(*cqe));
		memset(&req->big_cqe, 0, sizeof(req->big_cqe));
	}

	if (trace_io_uring_complete_enabled())
		trace_io_uring_complete(req->ctx, req, cqe);
	return true;
}

/**
 * Marks a request as failed. This function sets the appropriate flags
 * and handles any necessary cleanup. It also ensures that the request
 * is marked for skipping completion.
 */
static inline void req_set_fail(struct io_kiocb *req)
{
	req->flags |= REQ_F_FAIL;
	if (req->flags & REQ_F_CQE_SKIP) {
		req->flags &= ~REQ_F_CQE_SKIP;
		req->flags |= REQ_F_SKIP_LINK_CQES;
	}
}

/**
 * Sets the result and flags for a given io_kiocb request.
 * This function updates the completion queue entry (CQE) of the request
 * with the specified result and flags.
 */
static inline void io_req_set_res(struct io_kiocb *req, s32 res, u32 cflags)
{
	req->cqe.res = res;
	req->cqe.flags = cflags;
}

/**
 * Allocates memory for asynchronous data associated with the request.
 * This function uses a cache if available, otherwise it allocates
 * memory directly. It also sets the appropriate flags for the request.
 */
static inline void *io_uring_alloc_async_data(struct io_alloc_cache *cache,
					      struct io_kiocb *req)
{
	if (cache) {
		req->async_data = io_cache_alloc(cache, GFP_KERNEL);
	} else {
		const struct io_issue_def *def = &io_issue_defs[req->opcode];

		WARN_ON_ONCE(!def->async_size);
		req->async_data = kmalloc(def->async_size, GFP_KERNEL);
	}
	if (req->async_data)
		req->flags |= REQ_F_ASYNC_DATA;
	return req->async_data;
}

/**
 * Checks if the given request has associated asynchronous data.
 * This function verifies whether the request has the REQ_F_ASYNC_DATA
 * flag set, indicating that it has allocated asynchronous data.
 */
static inline bool req_has_async_data(struct io_kiocb *req)
{
	return req->flags & REQ_F_ASYNC_DATA;
}

/**
 * Releases the file associated with the given io_kiocb request.
 * This function ensures that the file reference is properly released
 * if the request does not use fixed files.
 */
static inline void io_put_file(struct io_kiocb *req)
{
	if (!(req->flags & REQ_F_FIXED_FILE) && req->file)
		fput(req->file);
}

/**
 * Unlocks the io_uring submission context. This function releases the
 * uring_lock if the IO_URING_F_UNLOCKED flag is set in the issue_flags.
 */
static inline void io_ring_submit_unlock(struct io_ring_ctx *ctx,
					 unsigned issue_flags)
{
	lockdep_assert_held(&ctx->uring_lock);
	if (unlikely(issue_flags & IO_URING_F_UNLOCKED))
		mutex_unlock(&ctx->uring_lock);
}

/**
 * Locks the io_uring submission context. This function acquires the
 * uring_lock if the IO_URING_F_UNLOCKED flag is set in the issue_flags.
 */
static inline void io_ring_submit_lock(struct io_ring_ctx *ctx,
				       unsigned issue_flags)
{
	/*
	 * "Normal" inline submissions always hold the uring_lock, since we
	 * grab it from the system call. Same is true for the SQPOLL offload.
	 * The only exception is when we've detached the request and issue it
	 * from an async worker thread, grab the lock for that case.
	 */
	if (unlikely(issue_flags & IO_URING_F_UNLOCKED))
		mutex_lock(&ctx->uring_lock);
	lockdep_assert_held(&ctx->uring_lock);
}

/**
 * Commits the completion queue ring by updating the tail pointer.
 * This function ensures that the completion queue entries are properly
 * committed and visible to other threads.
 */
static inline void io_commit_cqring(struct io_ring_ctx *ctx)
{
	/* order cqe stores with ring update */
	smp_store_release(&ctx->rings->cq.tail, ctx->cached_cq_tail);
}

/**
 * Wakes up the poll workqueue if there are any waiters. This function
 * checks if there are any tasks waiting on the poll workqueue and wakes
 * them up if necessary.
 */
static inline void io_poll_wq_wake(struct io_ring_ctx *ctx)
{
	if (wq_has_sleeper(&ctx->poll_wq))
		__wake_up(&ctx->poll_wq, TASK_NORMAL, 0,
				poll_to_key(EPOLL_URING_WAKE | EPOLLIN));
}

/**
 * Wakes up the completion queue ring if there are any waiters. This function
 * checks if there are any tasks waiting on the completion queue and wakes
 * them up if necessary.
 */
static inline void io_cqring_wake(struct io_ring_ctx *ctx)
{
	/*
	 * Trigger waitqueue handler on all waiters on our waitqueue. This
	 * won't necessarily wake up all the tasks, io_should_wake() will make
	 * that decision.
	 *
	 * Pass in EPOLLIN|EPOLL_URING_WAKE as the poll wakeup key. The latter
	 * set in the mask so that if we recurse back into our own poll
	 * waitqueue handlers, we know we have a dependency between eventfd or
	 * epoll and should terminate multishot poll at that point.
	 */
	if (wq_has_sleeper(&ctx->cq_wait))
		__wake_up(&ctx->cq_wait, TASK_NORMAL, 0,
				poll_to_key(EPOLL_URING_WAKE | EPOLLIN));
}

/**
 * Checks if the submission queue ring is full. This function verifies
 * whether the submission queue is full based on the tail and head pointers.
 */
static inline bool io_sqring_full(struct io_ring_ctx *ctx)
{
	struct io_rings *r = ctx->rings;

	/*
	 * SQPOLL must use the actual sqring head, as using the cached_sq_head
	 * is race prone if the SQPOLL thread has grabbed entries but not yet
	 * committed them to the ring. For !SQPOLL, this doesn't matter, but
	 * since this helper is just used for SQPOLL sqring waits (or POLLOUT),
	 * just read the actual sqring head unconditionally.
	 */
	return READ_ONCE(r->sq.tail) - READ_ONCE(r->sq.head) == ctx->sq_entries;
}

/**
 * Retrieves the number of available entries in the submission queue (SQ) ring.
 * This function calculates the difference between the SQ tail and the cached
 * SQ head, ensuring that the number of entries does not exceed the maximum
 * allowed SQ entries.
 */
static inline unsigned int io_sqring_entries(struct io_ring_ctx *ctx)
{
	struct io_rings *rings = ctx->rings;
	unsigned int entries;

	/* make sure SQ entry isn't read before tail */
	entries = smp_load_acquire(&rings->sq.tail) - ctx->cached_sq_head;
	return min(entries, ctx->sq_entries);
}

/**
 * Executes task work for the current task. This function processes
 * pending task work, including io_uring-specific work and general
 * task work. It ensures that all necessary operations are completed
 * before returning.
 */
static inline int io_run_task_work(void)
{
	bool ret = false;

	/*
	 * Always check-and-clear the task_work notification signal. With how
	 * signaling works for task_work, we can find it set with nothing to
	 * run. We need to clear it for that case, like get_signal() does.
	 */
	if (test_thread_flag(TIF_NOTIFY_SIGNAL))
		clear_notify_signal();
	/*
	 * PF_IO_WORKER never returns to userspace, so check here if we have
	 * notify work that needs processing.
	 */
	if (current->flags & PF_IO_WORKER) {
		if (test_thread_flag(TIF_NOTIFY_RESUME)) {
			__set_current_state(TASK_RUNNING);
			resume_user_mode_work(NULL);
		}
		if (current->io_uring) {
			unsigned int count = 0;

			__set_current_state(TASK_RUNNING);
			tctx_task_work_run(current->io_uring, UINT_MAX, &count);
			if (count)
				ret = true;
		}
	}
	if (task_work_pending(current)) {
		__set_current_state(TASK_RUNNING);
		task_work_run();
		ret = true;
	}

	return ret;
}

/**
 * Checks if there is any local work pending in the io_uring context.
 * This function verifies if there are any items in the work list or
 * retry list that need to be processed.
 */
static inline bool io_local_work_pending(struct io_ring_ctx *ctx)
{
	return !llist_empty(&ctx->work_llist) || !llist_empty(&ctx->retry_llist);
}

/**
 * Checks if there is any task work pending for the current task.
 * This function verifies if there are any task work items or local
 * work items that need to be processed.
 */
static inline bool io_task_work_pending(struct io_ring_ctx *ctx)
{
	return task_work_pending(current) || io_local_work_pending(ctx);
}

/**
 * Locks the io_uring context for task work. This function ensures that
 * the uring_lock is held when accessing the io_uring context.
 */
static inline void io_tw_lock(struct io_ring_ctx *ctx, io_tw_token_t tw)
{
	lockdep_assert_held(&ctx->uring_lock);
}

/*
 * Don't complete immediately but use deferred completion infrastructure.
 * Protected by ->uring_lock and can only be used either with
 * IO_URING_F_COMPLETE_DEFER or inside a tw handler holding the mutex.
 */
static inline void io_req_complete_defer(struct io_kiocb *req)
	__must_hold(&req->ctx->uring_lock)
{
	struct io_submit_state *state = &req->ctx->submit_state;

	lockdep_assert_held(&req->ctx->uring_lock);

	wq_list_add_tail(&req->comp_list, &state->compl_reqs);
}

/**
 * Flushes the completion queue (CQ) ring if specific conditions are met.
 * This function ensures that deferred completions, timeouts, eventfd signaling,
 * or poll activation are handled appropriately.
 */
static inline void io_commit_cqring_flush(struct io_ring_ctx *ctx)
{
	if (unlikely(ctx->off_timeout_used || ctx->drain_active ||
		     ctx->has_evfd || ctx->poll_activated))
		__io_commit_cqring_flush(ctx);
}

/**
 * Adjusts the task reference count for the current io_uring task.
 * This function decreases the cached reference count by the specified
 * number and refills the task references if the count becomes negative.
 */
static inline void io_get_task_refs(int nr)
{
	struct io_uring_task *tctx = current->io_uring;

	tctx->cached_refs -= nr;
	if (unlikely(tctx->cached_refs < 0))
		io_task_refs_refill(tctx);
}

/**
 * Checks if the request cache is empty. This function verifies if
 * there are any free requests available in the cache.
 */
static inline bool io_req_cache_empty(struct io_ring_ctx *ctx)
{
	return !ctx->submit_state.free_list.next;
}

extern struct kmem_cache *req_cachep;

/**
 * Extracts a request from the free request cache in the io_uring context.
 * This function retrieves a request from the free list and removes it
 * from the cache for further processing.
 */
static inline struct io_kiocb *io_extract_req(struct io_ring_ctx *ctx)
{
	struct io_kiocb *req;

	req = container_of(ctx->submit_state.free_list.next, struct io_kiocb, comp_list);
	wq_stack_extract(&ctx->submit_state.free_list);
	return req;
}

/**
 * Allocates a request from the io_uring context. This function checks if
 * there are any free requests available in the cache. If not, it attempts
 * to refill the cache. If successful, it extracts a request from the cache
 * for further processing.
 */
static inline bool io_alloc_req(struct io_ring_ctx *ctx, struct io_kiocb **req)
{
	if (unlikely(io_req_cache_empty(ctx))) {
		if (!__io_alloc_req_refill(ctx))
			return false;
	}
	*req = io_extract_req(ctx);
	return true;
}

/**
 * Checks if the io_uring context is allowed to defer task work execution.
 * This function verifies if the current task is the submitter task and
 * if the context is set up for deferred task running.
 */
static inline bool io_allowed_defer_tw_run(struct io_ring_ctx *ctx)
{
	return likely(ctx->submitter_task == current);
}

/**
 * Checks if the io_uring context is allowed to run task work in a
 * thread worker. This function verifies if the context is set up for
 * deferred task running and if the current task is the submitter task.
 */
static inline bool io_allowed_run_tw(struct io_ring_ctx *ctx)
{
	return likely(!(ctx->flags & IORING_SETUP_DEFER_TASKRUN) ||
		      ctx->submitter_task == current);
}

/*
 * Terminate the request if either of these conditions are true:
 *
 * 1) It's being executed by the original task, but that task is marked
 *    with PF_EXITING as it's exiting.
 * 2) PF_KTHREAD is set, in which case the invoker of the task_work is
 *    our fallback task_work.
 */
static inline bool io_should_terminate_tw(void)
{
	return current->flags & (PF_KTHREAD | PF_EXITING);
}

/*
 * This is a special case for the tw_complete() function. It is used
 * when we want to complete the request immediately, but we don't want
 * to run the task work in the current context. Instead, we queue it
 * for later execution
 */
static inline void io_req_queue_tw_complete(struct io_kiocb *req, s32 res)
{
	io_req_set_res(req, res, 0);
	req->io_task_work.func = io_req_task_complete;
	io_req_task_work_add(req);
}

/*
 * IORING_SETUP_SQE128 contexts allocate twice the normal SQE size for each
 * slot.
 * Determines the size of a submission queue entry (SQE) for the io_uring context.
 * This function checks if the context is configured with IORING_SETUP_SQE128,
 * which doubles the size of each SQE slot.
 */
static inline size_t uring_sqe_size(struct io_ring_ctx *ctx)
{
	if (ctx->flags & IORING_SETUP_SQE128)
		return 2 * sizeof(struct io_uring_sqe);
	return sizeof(struct io_uring_sqe);
}

/**
 * Checks if the file associated with the request can be polled.
 * This function verifies if the request has the REQ_F_CAN_POLL flag set
 * or if the file itself supports polling.
 */
static inline bool io_file_can_poll(struct io_kiocb *req)
{
	if (req->flags & REQ_F_CAN_POLL)
		return true;
	if (req->file && file_can_poll(req->file)) {
		req->flags |= REQ_F_CAN_POLL;
		return true;
	}
	return false;
}

/**
 * Retrieves the current time based on the io_uring context's clock configuration.
 * This function returns the current time using either the monotonic clock or
 * a clock with an offset, depending on the context's clockid.
 */
static inline ktime_t io_get_time(struct io_ring_ctx *ctx)
{
	if (ctx->clockid == CLOCK_MONOTONIC)
		return ktime_get();

	return ktime_get_with_offset(ctx->clock_offset);
}

enum {
	IO_CHECK_CQ_OVERFLOW_BIT,
	IO_CHECK_CQ_DROPPED_BIT,
};

/**
 * Checks if there is any pending work in the io_uring context.
 * This function verifies if there are overflow conditions in the
 * completion queue or if there is local work pending in the context.
 */
static inline bool io_has_work(struct io_ring_ctx *ctx)
{
	return test_bit(IO_CHECK_CQ_OVERFLOW_BIT, &ctx->check_cq) ||
	       io_local_work_pending(ctx);
}
#endif
