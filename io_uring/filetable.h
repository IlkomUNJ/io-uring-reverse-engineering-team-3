// SPDX-License-Identifier: GPL-2.0
#ifndef IOU_FILE_TABLE_H
#define IOU_FILE_TABLE_H

#include <linux/file.h>
#include <linux/io_uring_types.h>
#include "rsrc.h"

/**
 * Allocates memory for file tables and initializes the bitmap for tracking
 * file slots. Returns true on success or false on failure.
 */
bool io_alloc_file_tables(struct io_ring_ctx *ctx, struct io_file_table *table, unsigned nr_files);

/**
 * Frees the memory associated with the file tables and clears the bitmap.
 */
void io_free_file_tables(struct io_ring_ctx *ctx, struct io_file_table *table);

/**
 * Installs a fixed file descriptor into the file table, ensuring proper
 * locking and cleanup on failure. Returns the slot index on success or a
 * negative error code on failure.
 */
int io_fixed_fd_install(struct io_kiocb *req, unsigned int issue_flags,
            struct file *file, unsigned int file_slot);

/**
 * Installs a fixed file descriptor into the file table, allocating a slot
 * if necessary. Returns the slot index on success or a negative error code
 * on failure.
 */
int __io_fixed_fd_install(struct io_ring_ctx *ctx, struct file *file,
                unsigned int file_slot);

/**
 * Removes a fixed file descriptor from the file table at the specified offset.
 * Returns 0 on success or a negative error code on failure.
 */
int io_fixed_fd_remove(struct io_ring_ctx *ctx, unsigned int offset);

/**
 * Registers a range of file slots for allocation in the file table.
 * Returns 0 on success or a negative error code on failure.
 */
int io_register_file_alloc_range(struct io_ring_ctx *ctx,
                 struct io_uring_file_index_range __user *arg);

/**
 * Retrieves the flags associated with a file, such as support for non-blocking
 * operations. Returns the flags as an io_req_flags_t value.
 */
io_req_flags_t io_file_get_flags(struct file *file);

/**
 * Clears a specific bit in the file table bitmap, marking the slot as
 * available for allocation.
 */
static inline void io_file_bitmap_clear(struct io_file_table *table, int bit)
{
    WARN_ON_ONCE(!test_bit(bit, table->bitmap));
    __clear_bit(bit, table->bitmap);
    table->alloc_hint = bit;
}

/**
 * Sets a specific bit in the file table bitmap, marking the slot as allocated.
 */
static inline void io_file_bitmap_set(struct io_file_table *table, int bit)
{
    WARN_ON_ONCE(test_bit(bit, table->bitmap));
    __set_bit(bit, table->bitmap);
    table->alloc_hint = bit + 1;
}

#define FFS_NOWAIT        0x1UL
#define FFS_ISREG        0x2UL
#define FFS_MASK        ~(FFS_NOWAIT|FFS_ISREG)

/**
 * Retrieves the slot flags for a resource node, such as support for non-blocking
 * operations.
 */
static inline unsigned int io_slot_flags(struct io_rsrc_node *node)
{
    return (node->file_ptr & ~FFS_MASK) << REQ_F_SUPPORT_NOWAIT_BIT;
}

/**
 * Retrieves the file pointer associated with a resource node.
 */
static inline struct file *io_slot_file(struct io_rsrc_node *node)
{
    return (struct file *)(node->file_ptr & FFS_MASK);
}

/**
 * Sets the file pointer and associated flags for a resource node.
 */
static inline void io_fixed_file_set(struct io_rsrc_node *node,
                     struct file *file)
{
    node->file_ptr = (unsigned long)file |
        (io_file_get_flags(file) >> REQ_F_SUPPORT_NOWAIT_BIT);
}

/**
 * Sets the allocation range for the file table, defining the start and end
 * indices for slot allocation.
 */
static inline void io_file_table_set_alloc_range(struct io_ring_ctx *ctx,
                         unsigned off, unsigned len)
{
    ctx->file_alloc_start = off;
    ctx->file_alloc_end = off + len;
    ctx->file_table.alloc_hint = ctx->file_alloc_start;
}

#endif
