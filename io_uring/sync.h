// SPDX-License-Identifier: GPL-2.0

// Prepares an io_sync_file_range operation by validating input and setting up the request.
int io_sfr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes an io_sync_file_range operation to synchronize file data.
int io_sync_file_range(struct io_kiocb *req, unsigned int issue_flags);

// Prepares an io_fsync operation by validating input and setting up the request.
int io_fsync_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes an io_fsync operation to synchronize file metadata and data.
int io_fsync(struct io_kiocb *req, unsigned int issue_flags);

// Prepares an io_fallocate operation by validating input and setting up the request.
int io_fallocate_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes an io_fallocate operation to allocate space for a file.
int io_fallocate(struct io_kiocb *req, unsigned int issue_flags);