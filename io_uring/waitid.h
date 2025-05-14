// SPDX-License-Identifier: GPL-2.0

#include "../kernel/exit.h"

struct io_waitid_async {
	struct io_kiocb *req;
	struct wait_opts wo;
};

// Prepares a waitid request by validating input and setting up the request.
int io_waitid_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes a waitid request by waiting for a process to change state.
int io_waitid(struct io_kiocb *req, unsigned int issue_flags);

// Cancels a waitid request for a given context.
int io_waitid_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd,
		     unsigned int issue_flags);

// Removes all waitid requests for a given context and task.
bool io_waitid_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
			  bool cancel_all);
