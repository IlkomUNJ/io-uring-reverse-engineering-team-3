// SPDX-License-Identifier: GPL-2.0

// Prepares an io_ftruncate operation by validating input and setting up the request.
int io_ftruncate_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes an io_ftruncate operation to truncate or extend a file.
int io_ftruncate(struct io_kiocb *req, unsigned int issue_flags);
