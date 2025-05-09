# Task 1: Information about io_uring source
List in this section source and headers of io_uring. For each of the C source/header, you must put description what's the prime responsibily of the source. Take notes, description of the source should be slightly technical like the example given.

## Source
### advise.c
Store io_madvice & io_fadvice structures, both have the same exact attributes. Which make them basically the same thing. Except function body treat them as separate. Codes which make use of io_madvice are guarded by compilation macro, which make its relevant functions only active if the build flag is set. But functions that make use of io_fadvice are active all the time. The exact difference between io_madvice & io_fadvice will only known after exploring do_madvise function for io_madvice & vfs_fadvise function for io_fadvice.

### alloc_cache.c
Implements the memory allocation cache logic for io_uring. Provides functions to initialize, allocate, and free objects from a per-context cache, reducing allocation overhead by reusing memory blocks. Handles dynamic allocation, zeroing of memory if required, and proper cleanup of cached objects, with support for integration with KASAN and kernel memory management routines.

### cancel.c
Implements the logic for cancelling in-flight io_uring requests. Provides functions to prepare, match, and execute both asynchronous and synchronous cancellation operations, supporting a variety of matching criteria such as user data, file descriptor, and opcode. Handles cancellation across multiple worker threads and contexts, manages synchronization and locking, and ensures proper cleanup and result reporting for cancelled requests.

### epoll.c
Implements the integration of epoll control and wait operations with io_uring. Provides functions to prepare, submit, and complete epoll_ctl and epoll_wait requests using io_uring's asynchronous interface. Handles translation of io_uring submission queue entries to epoll operations, manages user-space event structures, and ensures proper error handling and result reporting for epoll-based event notification within io_uring.

### eventfd.c
Implements eventfd integration for io_uring. Provides functions to register and unregister eventfd file descriptors with an io_uring context, enabling asynchronous notification of completion events. Manages reference counting, RCU-safe access, and signaling logic for eventfd objects, including support for both synchronous and asynchronous notification modes. Handles safe signaling, flushing, and cleanup of eventfd resources, ensuring correct notification semantics and resource management in concurrent environments.

### fdinfo.c
Implements the logic for displaying detailed io_uring state and statistics in the /proc/<pid>/fdinfo interface. Provides functions to output submission and completion queue information, buffer and file table details, personalities, poll lists, and overflow lists for a given io_uring file descriptor. Integrates with the kernel's seq_file API to support efficient, structured output for user-space inspection and debugging. Handles locking and race conditions to ensure consistent reporting even when io_uring is active.

### filetable.c
Implements the management of fixed file tables for io_uring. Provides functions to allocate and free file tables, install and remove files in fixed slots, and manage file table bitmaps for efficient slot allocation and deallocation. Handles user-space requests to register file allocation ranges and ensures proper synchronization and error handling when manipulating file descriptors in the io_uring context. Integrates with resource management and locking mechanisms to maintain consistency and safety during concurrent operations.

### fs.c
Implements file system operation support for io_uring, including preparation, execution, and cleanup routines for rename, unlink, mkdir, symlink, and link requests. Provides translation of io_uring submission queue entries into corresponding VFS operations, manages user-space path arguments, and ensures proper resource allocation and cleanup. Handles error checking, asynchronous execution, and integration with kernel name resolution and permission mechanisms to enable efficient, non-blocking file system changes through io_uring.

### futex.c
Implements futex (fast userspace mutex) operation support for io_uring, including preparation, execution, and completion routines for futex wait, wake, and vectorized wait operations. Provides integration with the kernel's futex infrastructure to enable asynchronous futex operations via io_uring requests. Manages futex request state, queueing, and completion, including support for cancellation, removal, and cache management of futex-related data structures. Handles proper synchronization, error handling, and efficient resource cleanup for both single and multiple futex operations within the io_uring context.

### io_uring.c
Implements the main logic for the io_uring subsystem, including initialization, submission, and completion of asynchronous I/O requests via shared submission and completion rings. Manages the lifecycle of io_uring contexts, request allocation, task work, and resource cleanup. Handles system calls for io_uring setup and operation, integrates with kernel memory management, file and event notification systems, and supports advanced features such as SQPOLL, IOPOLL, and multishot requests. Provides mechanisms for efficient batching, synchronization, cancellation, and error handling, enabling high-performance, low-latency I/O for user space applications.

### io-wq.c
Implements the basic worker thread pool for io_uring (io-wq). Manages the creation, scheduling, and destruction of worker threads for asynchronous I/O execution. Handles work queueing, worker activation, concurrency control via work hashing, and dynamic scaling of worker pools. Provides mechanisms for work cancellation, CPU affinity management, and integration with kernel task and resource management. Ensures efficient, scalable, and safe execution of asynchronous I/O operations in both bounded and unbounded worker pool configurations.

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

### epoll.h
Declares the function prototypes for preparing and executing epoll control and wait operations in io_uring, enabled when CONFIG_EPOLL is set. This header provides the interface for integrating epoll-based event notification with io_uring, allowing submission and completion of epoll_ctl and epoll_wait requests through the io_uring interface.

### eventfd.h
Declares the function prototypes for eventfd integration in io_uring. This header provides interfaces for registering and unregistering eventfd file descriptors with an io_uring context, as well as signaling and flushing eventfd notifications. These functions enable asynchronous notification mechanisms between user space and io_uring operations.

**Function specifications:**
- `int io_eventfd_register(struct io_ring_ctx *ctx, void __user *arg, unsigned int eventfd_async);`
  Registers an eventfd file descriptor with the given io_uring context, optionally enabling asynchronous notification.

- `int io_eventfd_unregister(struct io_ring_ctx *ctx);`
  Unregisters the eventfd file descriptor from the specified io_uring context.

- `void io_eventfd_flush_signal(struct io_ring_ctx *ctx);`
  Flushes any pending eventfd signals for the given io_uring context.

- `void io_eventfd_signal(struct io_ring_ctx *ctx);`
  Signals the eventfd associated with the specified io_uring context.

### fdinfo.h
Declares the function prototype for displaying io_uring-specific file descriptor information in the /proc filesystem. Provides an interface for dumping detailed state and statistics about an io_uring instance to a seq_file, enabling integration with the kernel's fdinfo infrastructure for user-space inspection and debugging.

**Function specifications:**
- `void io_uring_show_fdinfo(struct seq_file *m, struct file *f);`
  Outputs io_uring-related information for the given file descriptor to the provided seq_file, typically used in /proc/<pid>/fdinfo.

### filetable.h
Declares the data structures and function prototypes for managing fixed file tables in io_uring. This header provides interfaces for allocating, freeing, installing, and removing files in the fixed file table, as well as utilities for bitmap management and file slot operations. It enables efficient file descriptor management for io_uring operations that use fixed file sets, supporting both user and kernel space interactions.

**Function specifications:**
- `bool io_alloc_file_tables(struct io_ring_ctx *ctx, struct io_file_table *table, unsigned nr_files);`
  Allocates file tables for the given io_uring context and table structure.

- `void io_free_file_tables(struct io_ring_ctx *ctx, struct io_file_table *table);`
  Frees the file tables associated with the given io_uring context.

- `int io_fixed_fd_install(struct io_kiocb *req, unsigned int issue_flags, struct file *file, unsigned int file_slot);`
  Installs a file into the fixed file table at the specified slot for a given request.

- `int __io_fixed_fd_install(struct io_ring_ctx *ctx, struct file *file, unsigned int file_slot);`
  Installs a file into the fixed file table at the specified slot for a given context.

- `int io_fixed_fd_remove(struct io_ring_ctx *ctx, unsigned int offset);`
  Removes a file from the fixed file table at the specified offset.

- `int io_register_file_alloc_range(struct io_ring_ctx *ctx, struct io_uring_file_index_range __user *arg);`
  Registers a file allocation range for the given io_uring context.

- `io_req_flags_t io_file_get_flags(struct file *file);`
  Retrieves io_uring-specific flags for the given file.

- `void io_file_bitmap_clear(struct io_file_table *table, int bit);`
  (Inline) Clears the specified bit in the file table bitmap and updates the allocation hint.

- `void io_file_bitmap_set(struct io_file_table *table, int bit);`
  (Inline) Sets the specified bit in the file table bitmap and updates the allocation hint.

- `unsigned int io_slot_flags(struct io_rsrc_node *node);`
  (Inline) Returns the slot flags for the given resource node.

- `struct file *io_slot_file(struct io_rsrc_node *node);`
  (Inline) Returns the file pointer for the given resource node.

- `void io_fixed_file_set(struct io_rsrc_node *node, struct file *file);`
  (Inline) Sets the file pointer and flags for the given resource node.

- `void io_file_table_set_alloc_range(struct io_ring_ctx *ctx, unsigned off, unsigned len);`
  (Inline) Sets the allocation range for the file table in the given context.

### fs.h
Declares the function prototypes for file system operations in io_uring, including preparation, execution, and cleanup routines for rename, unlink, mkdir, symlink, and link operations. This header provides the interface for integrating common file system manipulations with io_uring's asynchronous request model, enabling efficient and non-blocking file system changes through io_uring submissions.

**Function specifications:**
- `int io_renameat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a renameat operation from an io_uring submission queue entry.

- `int io_renameat(struct io_kiocb *req, unsigned int issue_flags);`
  Executes the renameat operation.

- `void io_renameat_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a renameat operation.

- `int io_unlinkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an unlinkat operation from an io_uring submission queue entry.

- `int io_unlinkat(struct io_kiocb *req, unsigned int issue_flags);`
  Executes the unlinkat operation.

- `void io_unlinkat_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with an unlinkat operation.

- `int io_mkdirat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a mkdirat operation from an io_uring submission queue entry.

- `int io_mkdirat(struct io_kiocb *req, unsigned int issue_flags);`
  Executes the mkdirat operation.

- `void io_mkdirat_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a mkdirat operation.

- `int io_symlinkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a symlinkat operation from an io_uring submission queue entry.

- `int io_symlinkat(struct io_kiocb *req, unsigned int issue_flags);`
  Executes the symlinkat operation.

- `int io_linkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a linkat operation from an io_uring submission queue entry.

- `int io_linkat(struct io_kiocb *req, unsigned int issue_flags);`
  Executes the linkat operation.

- `void io_link_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a linkat operation.

### futex.h
Declares the function prototypes for futex (fast userspace mutex) operations in io_uring, including preparation, wait, wake, and vectorized variants. Provides interfaces for integrating futex-based synchronization primitives with io_uring's asynchronous request model. Also declares functions for futex cancellation, removal, and cache management, with conditional compilation for systems with or without futex support.

**Function specifications:**
- `int io_futex_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a futex wait operation from an io_uring submission queue entry.

- `int io_futexv_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a vectorized futex wait operation from an io_uring submission queue entry.

- `int io_futex_wait(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a futex wait operation.

- `int io_futexv_wait(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a vectorized futex wait operation.

- `int io_futex_wake(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a futex wake operation.

- `int io_futex_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd, unsigned int issue_flags);`
  Cancels a futex operation matching the given cancel data (only if CONFIG_FUTEX is enabled).

- `bool io_futex_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx, bool cancel_all);`
  Removes all matching futex operations from the context (only if CONFIG_FUTEX is enabled).

- `bool io_futex_cache_init(struct io_ring_ctx *ctx);`
  Initializes the futex cache for the given io_uring context (only if CONFIG_FUTEX is enabled).

- `void io_futex_cache_free(struct io_ring_ctx *ctx);`
  Frees the futex cache for the given io_uring context (only if CONFIG_FUTEX is enabled).

### io_uring.h
Declares the core data structures, constants, and function prototypes for the io_uring subsystem. Provides interfaces for submission and completion queue management, request allocation and lifecycle, task work handling, file and resource management, and integration with kernel synchronization primitives. Defines helpers and inlines for efficient request processing, completion event posting, and context management, supporting both synchronous and asynchronous I/O operations. This header is central to the coordination of all io_uring operations within the kernel.

### io-wq.h
Declares the core data structures, constants, and function prototypes for the internal io_uring worker thread pool (io-wq) subsystem. Provides interfaces for creating and destroying worker pools, enqueuing work, managing work hashing and concurrency, setting CPU affinity, and controlling the number of worker threads. Defines types and helpers for work cancellation, worker state management, and integration with the io_uring task context. Supports both bounded and unbounded worker pools, efficient work distribution, and cancellation semantics for asynchronous I/O operations.

**Function specifications:**
- `struct io_wq *io_wq_create(unsigned bounded, struct io_wq_data *data);`
  Creates a new io-wq worker pool with the specified number of bounded workers and work handling callbacks.

- `void io_wq_exit_start(struct io_wq *wq);`
  Initiates the shutdown sequence for the given io-wq worker pool.

- `void io_wq_put_and_exit(struct io_wq *wq);`
  Finalizes shutdown and frees resources for the given io-wq worker pool.

- `void io_wq_enqueue(struct io_wq *wq, struct io_wq_work *work);`
  Enqueues a work item into the io-wq for asynchronous execution.

- `void io_wq_hash_work(struct io_wq_work *work, void *val);`
  Assigns a hash value to a work item to control concurrency (e.g., for serialized inode operations).

- `int io_wq_cpu_affinity(struct io_uring_task *tctx, cpumask_var_t mask);`
  Sets the CPU affinity mask for the io-wq worker pool associated with the given io_uring task context.

- `int io_wq_max_workers(struct io_wq *wq, int *new_count);`
  Sets or retrieves the maximum number of workers for bounded and unbounded pools.

- `bool io_wq_worker_stopped(void);`
  Returns true if the current task is an io-wq worker and is in the process of stopping.

- `enum io_wq_cancel io_wq_cancel_cb(struct io_wq *wq, work_cancel_fn *cancel, void *data, bool cancel_all);`
  Attempts to cancel work items in the io-wq matching the provided callback and data.

- `void io_wq_worker_sleeping(struct task_struct *tsk);`
  Notifies the io-wq subsystem that a worker is going to sleep (only if CONFIG_IO_WQ is enabled).

- `void io_wq_worker_running(struct task_struct *tsk);`
  Notifies the io-wq subsystem that a worker is running (only if CONFIG_IO_WQ is enabled).

- `bool io_wq_current_is_worker(void);`
  Returns true if the current task is an io-wq worker.