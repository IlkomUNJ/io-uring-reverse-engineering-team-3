// SPDX-License-Identifier: GPL-2.0

#include <linux/io_uring_types.h>
#include <linux/pagemap.h>

struct io_meta_state {
	u32			seed;
	struct iov_iter_state	iter_meta;
};

struct io_async_rw {
	struct iou_vec			vec;
	size_t				bytes_done;

	struct_group(clear,
		struct iov_iter			iter;
		struct iov_iter_state		iter_state;
		struct iovec			fast_iov;
		/*
		 * wpq is for buffered io, while meta fields are used with
		 * direct io
		 */
		union {
			struct wait_page_queue		wpq;
			struct {
				struct uio_meta			meta;
				struct io_meta_state		meta_state;
			};
		};
	);
};

/**
 * Prepares a fixed read request from an io_uring submission queue entry (SQE).
 */
int io_prep_read_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a fixed write request from an io_uring submission queue entry (SQE).
 */
int io_prep_write_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a fixed vectored read request from an io_uring submission queue entry (SQE).
 */
int io_prep_readv_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a fixed vectored write request from an io_uring submission queue entry (SQE).
 */
int io_prep_writev_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a vectored read request from an io_uring submission queue entry (SQE).
 */
int io_prep_readv(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a vectored write request from an io_uring submission queue entry (SQE).
 */
int io_prep_writev(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a read request from an io_uring submission queue entry (SQE).
 */
int io_prep_read(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a write request from an io_uring submission queue entry (SQE).
 */
int io_prep_write(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Handles a read request.
 */
int io_read(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Handles a write request.
 */
int io_write(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Handles a fixed read request.
 */
int io_read_fixed(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Handles a fixed write request.
 */
int io_write_fixed(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources associated with a vectored read/write request.
 */
void io_readv_writev_cleanup(struct io_kiocb *req);

/**
 * Handles a failed read/write request.
 */
void io_rw_fail(struct io_kiocb *req);

/**
 * Completes a read/write request.
 */
void io_req_rw_complete(struct io_kiocb *req, io_tw_token_t tw);

/**
 * Prepares a multishot read request from an io_uring submission queue entry (SQE).
 */
int io_read_mshot_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Handles a multishot read request.
 */
int io_read_mshot(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Frees resources associated with an asynchronous read/write structure.
 */
void io_rw_cache_free(const void *entry);
