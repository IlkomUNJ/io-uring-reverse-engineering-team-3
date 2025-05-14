// SPDX-License-Identifier: GPL-2.0
#ifndef IORING_REGISTER_H
#define IORING_REGISTER_H

/**
 * Unregisters an eventfd from the io_uring context.
 */
int io_eventfd_unregister(struct io_ring_ctx *ctx);

/**
 * Unregisters a personality from the io_uring context.
 */
int io_unregister_personality(struct io_ring_ctx *ctx, unsigned id);

/**
 * Retrieves a file associated with an io_uring ring or a registered file descriptor.
 */
struct file *io_uring_register_get_file(unsigned int fd, bool registered);

#endif
