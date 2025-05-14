// SPDX-License-Identifier: GPL-2.0
#ifndef IORING_CANCEL_H
#define IORING_CANCEL_H

#include <linux/io_uring_types.h>

struct io_cancel_data {
	struct io_ring_ctx *ctx;
	union {
		u64 data;
		struct file *file;
	};
	u8 opcode;
	u32 flags;
	int seq;
};

/**
 * Prepares the asynchronous cancel operation by extracting and validating
 * parameters from the submission queue entry (SQE).
 */
int io_async_cancel_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the asynchronous cancel operation using the prepared parameters.
 */
int io_async_cancel(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Attempts to cancel a request using various mechanisms, including poll,
 * waitid, futex, and timeout.
 */
int io_try_cancel(struct io_uring_task *tctx, struct io_cancel_data *cd,
		  unsigned int issue_flags);

/**
 * Executes a synchronous cancel operation, optionally waiting for a timeout
 * if specified.
 */
int io_sync_cancel(struct io_ring_ctx *ctx, void __user *arg);

/**
 * Returns true if the request matches the criteria outlined by the cancel data.
 */
bool io_cancel_req_match(struct io_kiocb *req, struct io_cancel_data *cd);

/**
 * Removes all requests from the specified list that match the cancel criteria.
 * Returns true if any requests were canceled.
 */
bool io_cancel_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
			  struct hlist_head *list, bool cancel_all,
			  bool (*cancel)(struct io_kiocb *));

/**
 * Removes requests from the specified list that match the cancel criteria.
 * Returns the number of requests canceled or -ENOENT if no matching requests
 * were found.
 */
int io_cancel_remove(struct io_ring_ctx *ctx, struct io_cancel_data *cd,
		     unsigned int issue_flags, struct hlist_head *list,
		     bool (*cancel)(struct io_kiocb *));

/**
 * Matches the sequence of a request to ensure proper cancellation handling.
 * Returns true if the sequence matches, false otherwise.
 */
static inline bool io_cancel_match_sequence(struct io_kiocb *req, int sequence)
{
	if (req->cancel_seq_set && sequence == req->work.cancel_seq)
		return true;

	req->cancel_seq_set = true;
	req->work.cancel_seq = sequence;
	return false;
}

#endif
