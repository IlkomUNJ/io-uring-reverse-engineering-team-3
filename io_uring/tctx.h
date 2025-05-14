// SPDX-License-Identifier: GPL-2.0

struct io_tctx_node {
	struct list_head	ctx_node;
	struct task_struct	*task;
	struct io_ring_ctx	*ctx;
};

// Allocates and initializes the io_uring task context for a task.
int io_uring_alloc_task_context(struct task_struct *task,
                struct io_ring_ctx *ctx);

// Removes a task context node from the io_uring context.
void io_uring_del_tctx_node(unsigned long index);

// Adds a task context node to the io_uring context.
int __io_uring_add_tctx_node(struct io_ring_ctx *ctx);

// Adds a task context node during submission.
int __io_uring_add_tctx_node_from_submit(struct io_ring_ctx *ctx);

// Cleans up all task context nodes and workqueues for the io_uring task.
void io_uring_clean_tctx(struct io_uring_task *tctx);

// Unregisters all ring file descriptors for io_uring.
void io_uring_unreg_ringfd(void);

// Registers ring file descriptors for io_uring.
int io_ringfd_register(struct io_ring_ctx *ctx, void __user *__arg,
               unsigned nr_args);

// Unregisters ring file descriptors for io_uring.
int io_ringfd_unregister(struct io_ring_ctx *ctx, void __user *__arg,
             unsigned nr_args);

/*
 * Note that this task has used io_uring. We use it for cancelation purposes.
 */
static inline int io_uring_add_tctx_node(struct io_ring_ctx *ctx)
{
	struct io_uring_task *tctx = current->io_uring;

	if (likely(tctx && tctx->last == ctx))
		return 0;

	return __io_uring_add_tctx_node_from_submit(ctx);
}
