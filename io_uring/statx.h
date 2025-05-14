// SPDX-License-Identifier: GPL-2.0

// Prepares an io_statx operation by validating input and setting up the request.
int io_statx_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes an io_statx operation to retrieve file attributes.
int io_statx(struct io_kiocb *req, unsigned int issue_flags);

// Cleans up resources used by an io_statx operation.
void io_statx_cleanup(struct io_kiocb *req);
