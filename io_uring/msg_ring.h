// SPDX-License-Identifier: GPL-2.0

/**
 * Synchronously sends a message to a target io_uring context.
 */
int io_uring_sync_msg_ring(struct io_uring_sqe *sqe);

/**
 * Prepares an io_msg request from an io_uring submission queue entry (SQE).
 */
int io_msg_ring_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Processes an io_msg request, handling different message types.
 */
int io_msg_ring(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources associated with an io_msg request.
 */
void io_msg_ring_cleanup(struct io_kiocb *req);
