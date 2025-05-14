// SPDX-License-Identifier: GPL-2.0
#ifndef IOU_RSRC_H
#define IOU_RSRC_H

#include <linux/io_uring_types.h>
#include <linux/lockdep.h>

#define IO_VEC_CACHE_SOFT_CAP		256

enum {
	IORING_RSRC_FILE		= 0,
	IORING_RSRC_BUFFER		= 1,
};

struct io_rsrc_node {
	unsigned char			type;
	int				refs;

	u64 tag;
	union {
		unsigned long file_ptr;
		struct io_mapped_ubuf *buf;
	};
};

enum {
	IO_IMU_DEST	= 1 << ITER_DEST,
	IO_IMU_SOURCE	= 1 << ITER_SOURCE,
};

struct io_mapped_ubuf {
	u64		ubuf;
	unsigned int	len;
	unsigned int	nr_bvecs;
	unsigned int    folio_shift;
	refcount_t	refs;
	unsigned long	acct_pages;
	void		(*release)(void *);
	void		*priv;
	bool		is_kbuf;
	u8		dir;
	struct bio_vec	bvec[] __counted_by(nr_bvecs);
};

struct io_imu_folio_data {
	/* Head folio can be partially included in the fixed buf */
	unsigned int	nr_pages_head;
	/* For non-head/tail folios, has to be fully included */
	unsigned int	nr_pages_mid;
	unsigned int	folio_shift;
	unsigned int	nr_folios;
};

/**
 * Initializes the resource cache for the io_uring context.
 */
bool io_rsrc_cache_init(struct io_ring_ctx *ctx);

/**
 * Frees the resource cache for the io_uring context.
 */
void io_rsrc_cache_free(struct io_ring_ctx *ctx);

/**
 * Allocates a resource node for the io_uring context.
 */
struct io_rsrc_node *io_rsrc_node_alloc(struct io_ring_ctx *ctx, int type);

/**
 * Frees a resource node in the io_uring context.
 */
void io_free_rsrc_node(struct io_ring_ctx *ctx, struct io_rsrc_node *node);

/**
 * Frees resource data for the io_uring context.
 */
void io_rsrc_data_free(struct io_ring_ctx *ctx, struct io_rsrc_data *data);

/**
 * Allocates resource data for the io_uring context.
 */
int io_rsrc_data_alloc(struct io_rsrc_data *data, unsigned nr);

/**
 * Finds a buffer node for a request in the io_uring context.
 */
struct io_rsrc_node *io_find_buf_node(struct io_kiocb *req,
				      unsigned issue_flags);

/**
 * Imports a registered buffer for use in an I/O operation.
 */
int io_import_reg_buf(struct io_kiocb *req, struct iov_iter *iter,
			u64 buf_addr, size_t len, int ddir,
			unsigned issue_flags);

/**
 * Imports a registered vector for use in an I/O operation.
 */
int io_import_reg_vec(int ddir, struct iov_iter *iter,
			struct io_kiocb *req, struct iou_vec *vec,
			unsigned nr_iovs, unsigned issue_flags);

/**
 * Prepares an I/O vector for a registered buffer.
 */
int io_prep_reg_iovec(struct io_kiocb *req, struct iou_vec *iv,
			const struct iovec __user *uvec, size_t uvec_segs);

/**
 * Clones registered buffers from one io_uring context to another.
 */
int io_register_clone_buffers(struct io_ring_ctx *ctx, void __user *arg);

/**
 * Unregisters buffers from the io_uring context.
 */
int io_sqe_buffers_unregister(struct io_ring_ctx *ctx);

/**
 * Registers buffers in the io_uring context.
 */
int io_sqe_buffers_register(struct io_ring_ctx *ctx, void __user *arg,
			    unsigned int nr_args, u64 __user *tags);

/**
 * Unregisters files from the io_uring context.
 */
int io_sqe_files_unregister(struct io_ring_ctx *ctx);

/**
 * Registers files in the io_uring context.
 */
int io_sqe_files_register(struct io_ring_ctx *ctx, void __user *arg,
			  unsigned nr_args, u64 __user *tags);

/**
 * Updates registered files in the io_uring context.
 */
int io_register_files_update(struct io_ring_ctx *ctx, void __user *arg,
			     unsigned nr_args);

/**
 * Updates registered resources in the io_uring context.
 */
int io_register_rsrc_update(struct io_ring_ctx *ctx, void __user *arg,
			    unsigned size, unsigned type);

/**
 * Registers resources in the io_uring context.
 */
int io_register_rsrc(struct io_ring_ctx *ctx, void __user *arg,
			unsigned int size, unsigned int type);

/**
 * Validates a buffer for registration in the io_uring context.
 */
int io_buffer_validate(struct iovec *iov);

/**
 * Checks if a buffer can be coalesced based on its pages.
 */
bool io_check_coalesce_buffer(struct page **page_array, int nr_pages,
			      struct io_imu_folio_data *data);

static inline struct io_rsrc_node *io_rsrc_node_lookup(struct io_rsrc_data *data,
						       int index)
{
	if (index < data->nr)
		return data->nodes[array_index_nospec(index, data->nr)];
	return NULL;
}

static inline void io_put_rsrc_node(struct io_ring_ctx *ctx, struct io_rsrc_node *node)
{
	lockdep_assert_held(&ctx->uring_lock);
	if (!--node->refs)
		io_free_rsrc_node(ctx, node);
}

static inline bool io_reset_rsrc_node(struct io_ring_ctx *ctx,
				      struct io_rsrc_data *data, int index)
{
	struct io_rsrc_node *node = data->nodes[index];

	if (!node)
		return false;
	io_put_rsrc_node(ctx, node);
	data->nodes[index] = NULL;
	return true;
}

static inline void io_req_put_rsrc_nodes(struct io_kiocb *req)
{
	if (req->file_node) {
		io_put_rsrc_node(req->ctx, req->file_node);
		req->file_node = NULL;
	}
	if (req->flags & REQ_F_BUF_NODE) {
		io_put_rsrc_node(req->ctx, req->buf_node);
		req->buf_node = NULL;
	}
}

static inline void io_req_assign_rsrc_node(struct io_rsrc_node **dst_node,
					   struct io_rsrc_node *node)
{
	node->refs++;
	*dst_node = node;
}

static inline void io_req_assign_buf_node(struct io_kiocb *req,
					  struct io_rsrc_node *node)
{
	io_req_assign_rsrc_node(&req->buf_node, node);
	req->flags |= REQ_F_BUF_NODE;
}

/**
 * Updates files for a request in the io_uring context.
 */
int io_files_update(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a file update request from an io_uring submission queue entry (SQE).
 */
int io_files_update_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Accounts memory usage for a user in the io_uring context.
 */
int __io_account_mem(struct user_struct *user, unsigned long nr_pages);

static inline void __io_unaccount_mem(struct user_struct *user,
				      unsigned long nr_pages)
{
	atomic_long_sub(nr_pages, &user->locked_vm);
}

/**
 * Frees an I/O vector in the io_uring context.
 */
void io_vec_free(struct iou_vec *iv);

/**
 * Reallocates an I/O vector in the io_uring context.
 */
int io_vec_realloc(struct iou_vec *iv, unsigned nr_entries);

static inline void io_vec_reset_iovec(struct iou_vec *iv,
				      struct iovec *iovec, unsigned nr)
{
	io_vec_free(iv);
	iv->iovec = iovec;
	iv->nr = nr;
}

static inline void io_alloc_cache_vec_kasan(struct iou_vec *iv)
{
	if (IS_ENABLED(CONFIG_KASAN))
		io_vec_free(iv);
}

#endif
