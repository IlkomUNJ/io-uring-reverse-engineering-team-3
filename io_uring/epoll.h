// SPDX-License-Identifier: GPL-2.0

#if defined(CONFIG_EPOLL)
/**
 * Prepares the epoll control operation by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_epoll_ctl_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the epoll control operation, starting the eventpoll in non-blocking
 * mode if specified.
 */
int io_epoll_ctl(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares the epoll wait operation by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_epoll_wait_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the epoll wait operation, sending events to the user space buffer.
 */
int io_epoll_wait(struct io_kiocb *req, unsigned int issue_flags);
#endif
