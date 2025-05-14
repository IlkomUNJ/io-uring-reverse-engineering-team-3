// SPDX-License-Identifier: GPL-2.0
#ifndef IOU_KBUF_H
#define IOU_KBUF_H

#include <uapi/linux/io_uring.h>
#include <linux/io_uring_types.h>

enum {
	/* ring mapped provided buffers */
	IOBL_BUF_RING	= 1,
	/* buffers are consumed incrementally rather than always fully */
	IOBL_INC	= 2,
};

struct io_buffer_list {
	/*
	 * If ->buf_nr_pages is set, then buf_pages/buf_ring are used. If not,
	 * then these are classic provided buffers and ->buf_list is used.
	 */
	union {
		struct list_head buf_list;
		struct io_uring_buf_ring *buf_ring;
	};
	__u16 bgid;

	/* below is for ring provided buffers */
	__u16 buf_nr_pages;
	__u16 nr_entries;
	__u16 head;
	__u16 mask;

	__u16 flags;

	struct io_mapped_region region;
};

struct io_buffer {
	struct list_head list;
	__u64 addr;
	__u32 len;
	__u16 bid;
	__u16 bgid;
};

enum {
	/* can alloc a bigger vec */
	KBUF_MODE_EXPAND	= 1,
	/* if bigger vec allocated, free old one */
	KBUF_MODE_FREE		= 2,
};

struct buf_sel_arg {
	struct iovec *iovs;
	size_t out_len;
	size_t max_len;
	unsigned short nr_iovs;
	unsigned short mode;
};

/**
 * Selects a buffer for the given request. This function determines the
 * appropriate buffer to use based on the request and issue flags.
 */
void __user *io_buffer_select(struct io_kiocb *req, size_t *len,
			      unsigned int issue_flags);

/**
 * Selects buffers for the given request. This function processes the
 * provided arguments and selects buffers for the request based on the
 * issue flags.
 */
int io_buffers_select(struct io_kiocb *req, struct buf_sel_arg *arg,
		      unsigned int issue_flags);

/**
 * Peeks at the buffers for the given request. This function retrieves
 * information about the buffers without modifying their state.
 */
int io_buffers_peek(struct io_kiocb *req, struct buf_sel_arg *arg);

/**
 * Destroys all buffers associated with the given io_uring context.
 * This function ensures that all resources allocated for the buffers
 * are properly released.
 */
void io_destroy_buffers(struct io_ring_ctx *ctx);

/**
 * Prepares to remove buffers for the given request. This function
 * processes the submission queue entry (SQE) and sets up the request
 * for buffer removal.
 */
int io_remove_buffers_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Removes buffers for the given request. This function processes the
 * request and removes the associated buffers based on the issue flags.
 */
int io_remove_buffers(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares to provide buffers for the given request. This function
 * processes the submission queue entry (SQE) and sets up the request
 * for buffer provisioning.
 */
int io_provide_buffers_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Provides buffers for the given request. This function processes the
 * request and adds the associated buffers based on the issue flags.
 */
int io_provide_buffers(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Registers a provided buffer ring with the given io_uring context.
 * This function maps the buffer ring into the context's address space.
 */
int io_register_pbuf_ring(struct io_ring_ctx *ctx, void __user *arg);

/**
 * Unregisters a provided buffer ring from the given io_uring context.
 * This function removes the mapping of the buffer ring from the context's
 * address space.
 */
int io_unregister_pbuf_ring(struct io_ring_ctx *ctx, void __user *arg);

/**
 * Registers the status of a provided buffer ring with the given io_uring
 * context. This function updates the status of the buffer ring in the
 * context.
 */
int io_register_pbuf_status(struct io_ring_ctx *ctx, void __user *arg);

/**
 * Recycles a legacy buffer for the given request. This function handles
 * the recycling of buffers based on the issue flags.
 */
bool io_kbuf_recycle_legacy(struct io_kiocb *req, unsigned issue_flags);

/**
 * Drops a legacy buffer for the given request. This function ensures
 * that the buffer is properly released and removed from the request.
 */
void io_kbuf_drop_legacy(struct io_kiocb *req);

/**
 * Releases kernel buffers for the given request. This function processes
 * the request and releases the specified number of buffers.
 */
unsigned int __io_put_kbufs(struct io_kiocb *req, int len, int nbufs);

/**
 * Commits kernel buffers for the given request. This function processes
 * the buffer list and commits the specified number of buffers.
 */
bool io_kbuf_commit(struct io_kiocb *req,
		    struct io_buffer_list *bl, int len, int nr);

/**
 * Retrieves the mapped region for a provided buffer group ID (BGID) in the
 * given io_uring context. This function returns the region associated with
 * the specified BGID.
 */
struct io_mapped_region *io_pbuf_get_region(struct io_ring_ctx *ctx,
					    unsigned int bgid);

/**
 * Recycles a buffer ring for the given request. This function handles
 * the recycling of buffers in the ring and updates the request state.
 */
static inline bool io_kbuf_recycle_ring(struct io_kiocb *req)
{
	/*
	 * We don't need to recycle for REQ_F_BUFFER_RING, we can just clear
	 * the flag and hence ensure that bl->head doesn't get incremented.
	 * If the tail has already been incremented, hang on to it.
	 * The exception is partial io, that case we should increment bl->head
	 * to monopolize the buffer.
	 */
	if (req->buf_list) {
		req->buf_index = req->buf_list->bgid;
		req->flags &= ~(REQ_F_BUFFER_RING|REQ_F_BUFFERS_COMMIT);
		return true;
	}
	return false;
}

/**
 * Determines if a buffer should be selected for the given request.
 * This function checks the request flags and returns true if a buffer
 * should be selected, false otherwise.
 */
static inline bool io_do_buffer_select(struct io_kiocb *req)
{
	if (!(req->flags & REQ_F_BUFFER_SELECT))
		return false;
	return !(req->flags & (REQ_F_BUFFER_SELECTED|REQ_F_BUFFER_RING));
}

/**
 * Recycles a buffer for the given request. This function handles the
 * recycling of buffers based on the request flags and issue flags.
 */
static inline bool io_kbuf_recycle(struct io_kiocb *req, unsigned issue_flags)
{
	if (req->flags & REQ_F_BL_NO_RECYCLE)
		return false;
	if (req->flags & REQ_F_BUFFER_SELECTED)
		return io_kbuf_recycle_legacy(req, issue_flags);
	if (req->flags & REQ_F_BUFFER_RING)
		return io_kbuf_recycle_ring(req);
	return false;
}

/**
 * Releases a single kernel buffer for the given request. This function
 * processes the request and releases the specified buffer based on the
 * issue flags.
 */
static inline unsigned int io_put_kbuf(struct io_kiocb *req, int len,
				       unsigned issue_flags)
{
	if (!(req->flags & (REQ_F_BUFFER_RING | REQ_F_BUFFER_SELECTED)))
		return 0;
	return __io_put_kbufs(req, len, 1);
}

/**
 * Releases multiple kernel buffers for the given request. This function
 * processes the request and releases the specified number of buffers
 * based on the issue flags.
 */
static inline unsigned int io_put_kbufs(struct io_kiocb *req, int len,
					int nbufs, unsigned issue_flags)
{
	if (!(req->flags & (REQ_F_BUFFER_RING | REQ_F_BUFFER_SELECTED)))
		return 0;
	return __io_put_kbufs(req, len, nbufs);
}
#endif
