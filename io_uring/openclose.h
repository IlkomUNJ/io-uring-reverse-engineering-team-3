// SPDX-License-Identifier: GPL-2.0

/**
 * Closes a fixed file descriptor in the io_uring context.
 */
int __io_close_fixed(struct io_ring_ctx *ctx, unsigned int issue_flags,
             unsigned int offset);

/**
 * Prepares an openat request from an io_uring submission queue entry (SQE).
 */
int io_openat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes an openat operation, delegating to openat2.
 */
int io_openat(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources associated with an open request.
 */
void io_open_cleanup(struct io_kiocb *req);

/**
 * Prepares an openat2 request with extended flags from an io_uring submission queue entry (SQE).
 */
int io_openat2_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes an openat2 operation, opening a file with the specified parameters.
 */
int io_openat2(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a close request from an io_uring submission queue entry (SQE).
 */
int io_close_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a close operation, closing a file descriptor or fixed file.
 */
int io_close(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a request to install a fixed file descriptor from an io_uring submission queue entry (SQE).
 */
int io_install_fixed_fd_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Installs a fixed file descriptor for a request.
 */
int io_install_fixed_fd(struct io_kiocb *req, unsigned int issue_flags);
