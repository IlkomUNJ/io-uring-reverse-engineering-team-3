// SPDX-License-Identifier: GPL-2.0

#include <linux/io_uring/cmd.h>
#include <linux/io_uring_types.h>

struct io_async_cmd {
	struct io_uring_cmd_data	data;
	struct iou_vec			vec;
	struct io_uring_sqe		sqes[2];
};

// Executes a uring command by calling the file operation's uring_cmd handler.
int io_uring_cmd(struct io_kiocb *req, unsigned int issue_flags);

// Prepares a uring command by validating input and setting up the request.
int io_uring_cmd_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Cleans up resources for a uring command request.
void io_uring_cmd_cleanup(struct io_kiocb *req);

// Tries to cancel a uring command for a given context and task.
bool io_uring_try_cancel_uring_cmd(struct io_ring_ctx *ctx,
                   struct io_uring_task *tctx, bool cancel_all);

// Frees the memory associated with an asynchronous command.
void io_cmd_cache_free(const void *entry);

// Imports a fixed buffer vector for a uring command.
int io_uring_cmd_import_fixed_vec(struct io_uring_cmd *ioucmd,
                  const struct iovec __user *uvec,
                  size_t uvec_segs,
                  int ddir, struct iov_iter *iter,
                  unsigned issue_flags);