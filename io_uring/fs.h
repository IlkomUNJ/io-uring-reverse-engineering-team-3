// SPDX-License-Identifier: GPL-2.0

/**
 * Prepares the rename operation by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_renameat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the rename operation using the prepared parameters.
 */
int io_renameat(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources allocated during the rename operation.
 */
void io_renameat_cleanup(struct io_kiocb *req);

/**
 * Prepares the unlink operation by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_unlinkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the unlink operation using the prepared parameters.
 */
int io_unlinkat(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources allocated during the unlink operation.
 */
void io_unlinkat_cleanup(struct io_kiocb *req);

/**
 * Prepares the mkdir operation by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_mkdirat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the mkdir operation using the prepared parameters.
 */
int io_mkdirat(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources allocated during the mkdir operation.
 */
void io_mkdirat_cleanup(struct io_kiocb *req);

/**
 * Prepares the symlink operation by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_symlinkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the symlink operation using the prepared parameters.
 */
int io_symlinkat(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares the link operation by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_linkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes the link operation using the prepared parameters.
 */
int io_linkat(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources allocated during the link operation.
 */
void io_link_cleanup(struct io_kiocb *req);
