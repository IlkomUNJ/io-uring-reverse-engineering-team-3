// SPDX-License-Identifier: GPL-2.0

#include "cancel.h"

/**
 * Prepares a futex request by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_futex_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Prepares a futexv request by extracting and validating parameters
 * from the submission queue entry (SQE).
 */
int io_futexv_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Waits on a futex request, setting up the waiters and handling wakeups.
 */
int io_futex_wait(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Waits on a futexv request, setting up the waiters and handling wakeups.
 */
int io_futexv_wait(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Wakes up a futex request, ensuring proper handling of strict flags.
 */
int io_futex_wake(struct io_kiocb *req, unsigned int issue_flags);

#if defined(CONFIG_FUTEX)
/**
 * Cancels a futex request from the io_ring_ctx.
 */
int io_futex_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd,
            unsigned int issue_flags);

/**
 * Removes all futex requests from the io_ring_ctx, optionally canceling all.
 */
bool io_futex_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
             bool cancel_all);

/**
 * Initializes the futex cache for the io_ring_ctx.
 * Returns true on success or false on failure.
 */
bool io_futex_cache_init(struct io_ring_ctx *ctx);

/**
 * Frees the futex cache associated with the io_ring_ctx.
 */
void io_futex_cache_free(struct io_ring_ctx *ctx);
#else
/**
 * Stub function for io_futex_cancel when CONFIG_FUTEX is not defined.
 * Always returns 0.
 */
static inline int io_futex_cancel(struct io_ring_ctx *ctx,
                  struct io_cancel_data *cd,
                  unsigned int issue_flags)
{
    return 0;
}
/**
 * Stub function for io_futex_remove_all when CONFIG_FUTEX is not defined.
 * Always returns false.
 */
static inline bool io_futex_remove_all(struct io_ring_ctx *ctx,
                       struct io_uring_task *tctx, bool cancel_all)
{
    return false;
}
/**
 * Stub function for io_futex_cache_init when CONFIG_FUTEX is not defined.
 * Always returns false.
 */
static inline bool io_futex_cache_init(struct io_ring_ctx *ctx)
{
    return false;
}
/**
 * Stub function for io_futex_cache_free when CONFIG_FUTEX is not defined.
 * Does nothing.
 */
static inline void io_futex_cache_free(struct io_ring_ctx *ctx)
{
}
#endif
