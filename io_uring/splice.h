// SPDX-License-Identifier: GPL-2.0

// Prepares an io_tee operation by validating offsets and initializing fields.
int io_tee_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes an io_tee operation to duplicate data between files.
int io_tee(struct io_kiocb *req, unsigned int issue_flags);

// Cleans up resources used by an io_splice request.
void io_splice_cleanup(struct io_kiocb *req);

// Prepares an io_splice operation by setting offsets and validating fields.
int io_splice_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes an io_splice operation to transfer data between files.
int io_splice(struct io_kiocb *req, unsigned int issue_flags);