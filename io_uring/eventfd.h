struct io_ring_ctx;

/**
 * Registers an eventfd with the io_ring_ctx for completion queue notifications.
 * Returns 0 on success or a negative error code on failure.
 */
int io_eventfd_register(struct io_ring_ctx *ctx, void __user *arg,
            unsigned int eventfd_async);

/**
 * Unregisters the eventfd from the io_ring_ctx, releasing its resources.
 * Returns 0 on success or -ENXIO if no eventfd was registered.
 */
int io_eventfd_unregister(struct io_ring_ctx *ctx);

/**
 * Signals the eventfd only if new events have been posted to the completion
 * queue.
 */
void io_eventfd_flush_signal(struct io_ring_ctx *ctx);

/**
 * Signals the eventfd associated with the io_ring_ctx.
 */
void io_eventfd_signal(struct io_ring_ctx *ctx);
