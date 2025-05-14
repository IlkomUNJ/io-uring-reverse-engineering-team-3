// SPDX-License-Identifier: GPL-2.0

struct io_timeout_data {
	struct io_kiocb			*req;
	struct hrtimer			timer;
	struct timespec64		ts;
	enum hrtimer_mode		mode;
	u32				flags;
};

// Disarms a linked timeout operation for a given request.
struct io_kiocb *__io_disarm_linked_timeout(struct io_kiocb *req,
					    struct io_kiocb *link);
// Disarms a linked timeout operation if the request has a linked timeout.
static inline struct io_kiocb *io_disarm_linked_timeout(struct io_kiocb *req)
{
	struct io_kiocb *link = req->link;

	if (link && link->opcode == IORING_OP_LINK_TIMEOUT)
		return __io_disarm_linked_timeout(req, link);

	return NULL;
}

// Flushes all timeouts in the timeout list.
__cold void io_flush_timeouts(struct io_ring_ctx *ctx);
struct io_cancel_data;

// Line 23: Cancels a timeout operation for a given context and cancel data.
int io_timeout_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd);

// Line 25: Kills all timeouts for a given context and task.
__cold bool io_kill_timeouts(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
                 bool cancel_all);

// Line 27: Queues a linked timeout operation.
void io_queue_linked_timeout(struct io_kiocb *req);

// Line 28: Disarms the next linked timeout operation for a given request.
void io_disarm_next(struct io_kiocb *req);

// Line 30: Prepares a timeout operation by validating input and setting up the request.
int io_timeout_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Line 31: Prepares a linked timeout operation by validating input and setting up the request.
int io_link_timeout_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Line 32: Executes a timeout operation.
int io_timeout(struct io_kiocb *req, unsigned int issue_flags);

// Line 33: Prepares a timeout removal operation by validating input and setting up the request.
int io_timeout_remove_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Line 34: Executes a timeout removal operation.
int io_timeout_remove(struct io_kiocb *req, unsigned int issue_flags);