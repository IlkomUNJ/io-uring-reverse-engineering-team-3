# Task 1: Information about io_uring source
List in this section source and headers of io_uring. For each of the C source/header, you must put description what's the prime responsibily of the source. Take notes, description of the source should be slightly technical like the example given.

## Source
### advise.c
Store io_madvice & io_fadvice structures, both have the same exact attributes. Which make them basically the same thing. Except function body treat them as separate. Codes which make use of io_madvice are guarded by compilation macro, which make its relevant functions only active if the build flag is set. But functions that make use of io_fadvice are active all the time. The exact difference between io_madvice & io_fadvice will only known after exploring do_madvise function for io_madvice & vfs_fadvise function for io_fadvice.

### alloc_cache.c
Implements the memory allocation cache logic for io_uring. Provides functions to initialize, allocate, and free objects from a per-context cache, reducing allocation overhead by reusing memory blocks. Handles dynamic allocation, zeroing of memory if required, and proper cleanup of cached objects, with support for integration with KASAN and kernel memory management routines.

### cancel.c
Implements the logic for cancelling in-flight io_uring requests. Provides functions to prepare, match, and execute both asynchronous and synchronous cancellation operations, supporting a variety of matching criteria such as user data, file descriptor, and opcode. Handles cancellation across multiple worker threads and contexts, manages synchronization and locking, and ensures proper cleanup and result reporting for cancelled requests.

## another source

## Headers
### advise.h
Declares the function prototypes for preparing and executing madvise and fadvise operations in io_uring. This header provides the interface for the corresponding implementation in advise.c, allowing other parts of the kernel to invoke memory and file advice operations through io_uring requests.

### alloc_cache.h
Defines the interface and inline helpers for managing a simple memory allocation cache used by io_uring. This header provides functions to initialize, allocate, free, and manage cached memory objects, optimizing frequent allocations and deallocations by reusing objects from a preallocated pool. It also includes KASAN (Kernel Address Sanitizer) support for memory safety when enabled.

**Function specifications:**
- `void io_alloc_cache_free(struct io_alloc_cache *cache, void (*free)(const void *));`
  Frees all cached objects in the allocation cache and releases the cache memory itself.

- `bool io_alloc_cache_init(struct io_alloc_cache *cache, unsigned max_nr, unsigned int size, unsigned int init_bytes);`
  Initializes the allocation cache with a maximum number of objects, object size, and the number of bytes to clear on allocation.

- `void *io_cache_alloc_new(struct io_alloc_cache *cache, gfp_t gfp);`
  Allocates a new object of the specified size using the given GFP (Get Free Page) flags.

- `bool io_alloc_cache_put(struct io_alloc_cache *cache, void *entry);`
  (Inline) Attempts to return an object to the cache. Returns true if successful, false if the cache is full.

- `void *io_alloc_cache_get(struct io_alloc_cache *cache);`
  (Inline) Retrieves an object from the cache if available, handling KASAN unpoisoning and zeroing if required.

- `void *io_cache_alloc(struct io_alloc_cache *cache, gfp_t gfp);`
  (Inline) Allocates an object from the cache if available, otherwise falls back to allocating a new object.

- `void io_cache_free(struct io_alloc_cache *cache, void *obj);`
  (Inline) Returns an object to the cache or frees it if the cache is full.

  ### cancel.h
Declares the data structures and function prototypes for request cancellation operations in io_uring. This header provides interfaces for preparing, executing, and matching asynchronous and synchronous cancellation requests, as well as utilities for removing and matching requests by sequence or criteria. It enables fine-grained control over cancelling in-flight io_uring operations from user or kernel context.

**Function specifications:**
- `int io_async_cancel_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an asynchronous cancel request based on the submission queue entry.

- `int io_async_cancel(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an asynchronous cancel operation for the given request.

- `int io_try_cancel(struct io_uring_task *tctx, struct io_cancel_data *cd, unsigned int issue_flags);`
  Attempts to cancel a request matching the provided cancel data in the given task context.

- `int io_sync_cancel(struct io_ring_ctx *ctx, void __user *arg);`
  Performs a synchronous cancel operation for the specified context and user argument.

- `bool io_cancel_req_match(struct io_kiocb *req, struct io_cancel_data *cd);`
  Checks if a request matches the given cancel data.

- `bool io_cancel_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx, struct hlist_head *list, bool cancel_all, bool (*cancel)(struct io_kiocb *));`
  Removes all matching requests from the list, optionally cancelling all, using a custom cancel function.

- `int io_cancel_remove(struct io_ring_ctx *ctx, struct io_cancel_data *cd, unsigned int issue_flags, struct hlist_head *list, bool (*cancel)(struct io_kiocb *));`
  Removes and cancels a specific request matching the cancel data from the list.

- `bool io_cancel_match_sequence(struct io_kiocb *req, int sequence);`
  (Inline) Matches and sets the cancel sequence for a request.