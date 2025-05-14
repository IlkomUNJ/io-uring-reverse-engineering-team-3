// SPDX-License-Identifier: GPL-2.0

// Cleans up resources used by an xattr operation.
void io_xattr_cleanup(struct io_kiocb *req);

// Prepares a file-specific setxattr operation.
int io_fsetxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes a file-specific setxattr operation.
int io_fsetxattr(struct io_kiocb *req, unsigned int issue_flags);

// Prepares a setxattr operation for a given path.
int io_setxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes a setxattr operation for a given path.
int io_setxattr(struct io_kiocb *req, unsigned int issue_flags);

// Prepares a file-specific getxattr operation.
int io_fgetxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes a file-specific getxattr operation.
int io_fgetxattr(struct io_kiocb *req, unsigned int issue_flags);

// Prepares a getxattr operation for a given path.
int io_getxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes a getxattr operation for a given path.
int io_getxattr(struct io_kiocb *req, unsigned int issue_flags);
