// SPDX-License-Identifier: GPL-2.0

#include <linux/io_uring_types.h>

#define IO_POLL_ALLOC_CACHE_MAX 32

enum {
	IO_APOLL_OK,
	IO_APOLL_ABORTED,
	IO_APOLL_READY
};

struct io_poll {
	struct file			*file;
	struct wait_queue_head		*head;
	__poll_t			events;
	int				retries;
	struct wait_queue_entry		wait;
};

struct async_poll {
	struct io_poll		poll;
	struct io_poll		*double_poll;
};

/*
 * Must only be called inside issue_flags & IO_URING_F_MULTISHOT, or
 * potentially other cases where we already "own" this poll request.
 */
static inline void io_poll_multishot_retry(struct io_kiocb *req)
{
	atomic_inc(&req->poll_refs);
}

/**
 * Prepares a poll add request from an io_uring submission queue entry (SQE).
 */
int io_poll_add_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a poll add request.
 */
int io_poll_add(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a poll remove request from an io_uring submission queue entry (SQE).
 */
int io_poll_remove_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a poll remove request.
 */
int io_poll_remove(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cancels a poll request based on the provided cancellation data.
 */
int io_poll_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd,
           unsigned issue_flags);

/**
 * Arms a poll handler for a request.
 */
int io_arm_poll_handler(struct io_kiocb *req, unsigned issue_flags);

/**
 * Removes all poll requests from the cancellation hash table.
 */
bool io_poll_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
            bool cancel_all);

/**
 * Handles task work for a poll request.
 */
void io_poll_task_func(struct io_kiocb *req, io_tw_token_t tw);
