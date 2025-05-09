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

**Function specifications:**
- `int io_madvise_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a madvise operation from an io_uring submission queue entry.

- `int io_madvise(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a madvise operation for the given request.

- `int io_fadvise_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a fadvise operation from an io_uring submission queue entry.

- `int io_fadvise(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a fadvise operation for the given request.

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

**Function specifications:**
- `int io_epoll_ctl_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an epoll_ctl operation from a submission queue entry.
- `int io_epoll_ctl(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an epoll_ctl operation.
- `int io_epoll_wait_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an epoll_wait operation from a submission queue entry.
- `int io_epoll_wait(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an epoll_wait operation.
- `void io_epoll_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with an epoll operation.

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

**Function specifications:**
- `struct io_ring_ctx *io_ring_ctx_alloc(unsigned int entries, unsigned int flags);`
  Allocates and initializes an io_uring context with the specified number of entries and flags.
- `void io_ring_ctx_free(struct io_ring_ctx *ctx);`
  Frees all resources associated with an io_uring context.
- `int io_submit_sqes(struct io_ring_ctx *ctx, unsigned int nr, struct io_kiocb **reqs);`
  Submits an array of submission queue entries to the io_uring context.
- `void io_cqring_fill_event(struct io_ring_ctx *ctx, u64 user_data, s32 res, u32 flags);`
  Posts a completion event to the completion queue with the specified user data, result, and flags.
- `void io_commit_cqring(struct io_ring_ctx *ctx);`
  Commits all pending completion queue events to user space.
- `void io_put_task(struct task_struct *task);`
  Releases a reference to a task structure used by io_uring.
- `void io_req_complete(struct io_kiocb *req, long res, unsigned int issue_flags);`
  Completes an io_uring request with the specified result and flags.
- `void io_queue_async_work(struct io_kiocb *req);`
  Queues an asynchronous work item for execution.
- `void io_run_task_work(struct io_ring_ctx *ctx);`
  Runs pending task work for the given io_uring context.
- `void io_uring_task_cancel(struct io_uring_task *tctx, struct io_ring_ctx *ctx, bool cancel_all);`
  Cancels pending requests for a given task and context.
- `void io_uring_add_personality(struct io_ring_ctx *ctx, kuid_t kuid);`
  Adds a personality (user identity) to the io_uring context.
- `void io_uring_remove_personality(struct io_ring_ctx *ctx, unsigned int id);`
  Removes a personality from the io_uring context by ID.
- `void io_uring_show_fdinfo(struct seq_file *m, struct file *f);`
  Outputs io_uring-related information for the given file descriptor to the provided seq_file.


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

### kbuf.h
Declares data structures and functions for managing kernel and user-provided buffers in io_uring. Handles buffer selection, recycling, and registration, supporting both classic and ring-mapped buffer models. Provides mechanisms for buffer allocation, deallocation, and tracking, enabling efficient zero-copy I/O and buffer lifecycle management for asynchronous operations.

**Function specifications:**
- `int io_register_buffers(struct io_ring_ctx *ctx, void __user *arg, unsigned int nr_args);`
  Registers user-provided buffers with the io_uring context.
- `int io_unregister_buffers(struct io_ring_ctx *ctx);`
  Unregisters all buffers from the io_uring context.
- `void io_kbuf_recycle(struct io_ring_ctx *ctx, struct io_buffer *buf);`
  Recycles a buffer for reuse in the io_uring context.
- `struct io_buffer *io_kbuf_select(struct io_ring_ctx *ctx, size_t len, unsigned bgid);`
  Selects a buffer from the registered buffer group for the given length and group ID.
- `void io_kbuf_free(struct io_ring_ctx *ctx);`
  Frees all kernel buffers associated with the io_uring context.

### memmap.h
Defines interfaces for memory mapping and region management in io_uring. Provides functions for pinning user pages, handling unmapped area requests, and managing mapped regions for buffer sharing between user and kernel space. Supports both MMU and NOMMU systems, facilitating efficient memory operations and safe region cleanup.

**Function specifications:**
- `int io_pin_user_pages(unsigned long start, unsigned long nr_pages, int write, struct page **pages);`
  Pins user pages in memory for I/O operations.
- `void io_unpin_user_pages(struct page **pages, unsigned long nr_pages);`
  Unpins previously pinned user pages.
- `unsigned long io_get_unmapped_area(struct file *file, unsigned long addr, unsigned long len, unsigned long pgoff, unsigned long flags);`
  Finds an unmapped area suitable for memory mapping.
- `int io_mem_region_add(struct io_ring_ctx *ctx, unsigned long addr, unsigned long len);`
  Adds a memory region to the io_uring context.
- `void io_mem_region_remove(struct io_ring_ctx *ctx, unsigned long addr, unsigned long len);`
  Removes a memory region from the io_uring context.

### msg_ring.h
Declares functions for message ring operations within io_uring. Provides interfaces for preparing, executing, and cleaning up message ring requests, enabling inter-ring communication and synchronization between different io_uring instances or contexts.

**Function specifications:**
- `int io_msg_ring_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a message ring operation from a submission queue entry.
- `int io_msg_ring(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a message ring operation.
- `void io_msg_ring_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a message ring request.

### napi.h
Declares interfaces for NAPI (New API) integration with io_uring, enabling busy-polling and efficient network receive operations. Provides functions for registering/unregistering NAPI IDs, managing busy-poll lists, and integrating with kernel network stack for low-latency I/O, with conditional compilation for systems with or without busy-poll support.

**Function specifications:**
- `int io_napi_add_id(struct io_ring_ctx *ctx, unsigned int napi_id);`
  Registers a NAPI ID with the io_uring context.
- `void io_napi_del_id(struct io_ring_ctx *ctx, unsigned int napi_id);`
  Unregisters a NAPI ID from the io_uring context.
- `void io_napi_busy_poll(struct io_ring_ctx *ctx);`
  Performs busy-polling for registered NAPI IDs.
- `void io_napi_cleanup(struct io_ring_ctx *ctx);`
  Cleans up all NAPI-related resources in the io_uring context.

### net.h
Declares data structures and functions for asynchronous network operations in io_uring. Provides preparation and execution routines for socket, send, receive, shutdown, bind, listen, and accept operations, as well as zero-copy send support. Integrates with kernel networking APIs and manages message headers, vectors, and cleanup for efficient asynchronous networking.

**Function specifications:**
- `int io_socket_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a socket operation from a submission queue entry.
- `int io_socket(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a socket operation.
- `int io_sendmsg_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a sendmsg operation.
- `int io_sendmsg(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a sendmsg operation.
- `int io_recvmsg_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a recvmsg operation.
- `int io_recvmsg(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a recvmsg operation.
- `int io_shutdown_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a shutdown operation.
- `int io_shutdown(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a shutdown operation.
- `int io_bind_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a bind operation.
- `int io_bind(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a bind operation.
- `int io_listen_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a listen operation.
- `int io_listen(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a listen operation.
- `int io_accept_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an accept operation.
- `int io_accept(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an accept operation.
- `void io_netmsg_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a network message request.

### nop.h
Declares preparation and execution functions for the no-operation (NOP) request in io_uring. Used for testing, benchmarking, or as a placeholder operation, ensuring minimal overhead and integration with the io_uring submission and completion model.

**Function specifications:**
- `int io_nop_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a NOP operation from a submission queue entry.
- `int io_nop(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a NOP operation.

### notif.h
Declares data structures and functions for notification handling in io_uring, particularly for zero-copy and asynchronous completion notifications. Manages notification objects, memory accounting, and completion signaling, enabling efficient user-space notification of I/O completion events.

**Function specifications:**
- `int io_notif_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a notification request.
- `int io_notif(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a notification request.
- `void io_notif_complete(struct io_kiocb *req, long res);`
  Completes a notification request with the specified result.
- `void io_notif_account_mem(struct io_ring_ctx *ctx, ssize_t size);`
  Accounts for memory usage by notification objects.

### openclose.h
Declares preparation, execution, and cleanup functions for open, openat2, and close operations in io_uring. Provides interfaces for managing file descriptors, including support for fixed file tables and installation of fixed file descriptors, enabling efficient asynchronous file open/close operations.

**Function specifications:**
- `int io_openat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an openat operation.
- `int io_openat(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an openat operation.
- `void io_openat_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with an openat operation.
- `int io_openat2_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an openat2 operation.
- `int io_openat2(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an openat2 operation.
- `void io_openat2_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with an openat2 operation.
- `int io_close_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a close operation.
- `int io_close(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a close operation.

### poll.h
Declares data structures and functions for poll-based event notification in io_uring. Provides preparation and execution routines for poll add and remove operations, as well as cancellation and multishot support. Integrates with kernel poll mechanisms to enable efficient asynchronous event monitoring.

**Function specifications:**
- `int io_poll_add_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a poll add operation.
- `int io_poll_add(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a poll add operation.
- `int io_poll_remove_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a poll remove operation.
- `int io_poll_remove(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a poll remove operation.
- `void io_poll_multishot(struct io_kiocb *req);`
  Handles multishot poll events.
- `int io_poll_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd, unsigned int issue_flags);`
  Cancels a poll operation matching the given cancel data.

### refs.h
Declares inline helpers and macros for managing reference counting of io_uring request objects. Provides atomic operations for incrementing, decrementing, and testing reference counts, ensuring safe lifecycle management and preventing premature deallocation of in-flight requests.

**Function specifications:**
- `void io_req_ref_get(struct io_kiocb *req);`
  Increments the reference count of a request.
- `bool io_req_ref_put(struct io_kiocb *req);`
  Decrements the reference count and returns true if it reached zero.
- `bool io_req_ref_test(struct io_kiocb *req);`
  Tests if the reference count is non-zero.

### register.h
Declares functions for unregistering eventfd and personality objects in io_uring, as well as retrieving registered file descriptors. Provides interfaces for resource cleanup and management of registered resources within the io_uring context.

**Function specifications:**
- `int io_unregister_eventfd(struct io_ring_ctx *ctx);`
  Unregisters the eventfd from the io_uring context.
- `int io_unregister_personality(struct io_ring_ctx *ctx, unsigned int id);`
  Unregisters a personality object by ID.
- `int io_get_registered_fd(struct io_ring_ctx *ctx, unsigned int index, struct file **file);`
  Retrieves a registered file descriptor by index.

### rsrc.h
Declares data structures and functions for managing io_uring resources, including files and buffers. Provides interfaces for resource node allocation, lookup, reference counting, and registration/unregistration of files and buffers. Supports efficient resource tracking, memory accounting, and integration with user-space registration APIs.

**Function specifications:**
- `struct io_rsrc_node *io_rsrc_node_alloc(unsigned int nr, gfp_t gfp);`
  Allocates a resource node for managing files or buffers.
- `void io_rsrc_node_free(struct io_rsrc_node *node);`
  Frees a resource node.
- `int io_register_files(struct io_ring_ctx *ctx, void __user *arg, unsigned int nr_args);`
  Registers files with the io_uring context.
- `int io_unregister_files(struct io_ring_ctx *ctx);`
  Unregisters all files from the io_uring context.
- `int io_register_buffers(struct io_ring_ctx *ctx, void __user *arg, unsigned int nr_args);`
  Registers buffers with the io_uring context.
- `int io_unregister_buffers(struct io_ring_ctx *ctx);`
  Unregisters all buffers from the io_uring context.

### rw.h
Declares data structures and functions for asynchronous read and write operations in io_uring. Provides preparation and execution routines for various read/write variants (including fixed and vectored I/O), as well as cleanup and failure handling. Manages I/O vectors, state, and completion for high-performance asynchronous file operations.

**Function specifications:**
- `int io_read_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a read operation.
- `int io_read(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a read operation.
- `int io_write_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a write operation.
- `int io_write(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a write operation.
- `void io_rw_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a read/write operation.
- `void io_rw_fail(struct io_kiocb *req, int ret);`
  Handles failure for a read/write operation.

### slist.h
Declares inline functions and macros for managing singly-linked work lists used in io_uring's internal work queue implementation. Provides efficient list manipulation primitives for adding, removing, splicing, and iterating over work nodes, supporting scalable and lockless work queue management.

**Function specifications:**
- `void io_slist_add(struct io_slist_head *head, struct io_slist_node *node);`
  Adds a node to the singly-linked list.
- `struct io_slist_node *io_slist_del_head(struct io_slist_head *head);`
  Removes and returns the head node from the list.
- `void io_slist_splice(struct io_slist_head *from, struct io_slist_head *to);`
  Splices all nodes from one list to another.
- `bool io_slist_empty(const struct io_slist_head *head);`
  Checks if the list is empty.

### sqpoll.h
Declares data structures and functions for submission queue polling (SQPOLL) in io_uring. Manages the SQPOLL thread, context lists, synchronization, and CPU affinity. Provides interfaces for thread creation, parking, un-parking, and cleanup, enabling efficient offloaded submission queue processing.

**Function specifications:**
- `int io_sqpoll_start(struct io_ring_ctx *ctx);`
  Starts the SQPOLL thread for the io_uring context.
- `void io_sqpoll_stop(struct io_ring_ctx *ctx);`
  Stops the SQPOLL thread.
- `void io_sqpoll_park(struct io_ring_ctx *ctx);`
  Parks the SQPOLL thread.
- `void io_sqpoll_unpark(struct io_ring_ctx *ctx);`
  Unparks the SQPOLL thread.
- `void io_sqpoll_cleanup(struct io_ring_ctx *ctx);`
  Cleans up SQPOLL resources.

### statx.h
Declares preparation, execution, and cleanup functions for statx operations in io_uring. Provides interfaces for asynchronous file status queries, integrating with kernel statx APIs to retrieve extended file metadata efficiently.

**Function specifications:**
- `int io_statx_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a statx operation.
- `int io_statx(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a statx operation.
- `void io_statx_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a statx operation.

### sync.h
Declares preparation and execution functions for file synchronization operations in io_uring, including fsync, sync_file_range, and fallocate. Provides interfaces for preparing and executing these operations asynchronously, ensuring data integrity and efficient file space management.

**Function specifications:**
- `int io_fsync_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an fsync operation.
- `int io_fsync(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an fsync operation.
- `int io_sync_file_range_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a sync_file_range operation.
- `int io_sync_file_range(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a sync_file_range operation.
- `int io_fallocate_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a fallocate operation.
- `int io_fallocate(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a fallocate operation.

### splice.h
Declares preparation, execution, and cleanup functions for splice and tee operations in io_uring. Provides interfaces for asynchronous data transfer between file descriptors using kernel splice/tee mechanisms, enabling zero-copy data movement and efficient pipeline construction.

**Function specifications:**
- `int io_splice_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a splice operation.
- `int io_splice(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a splice operation.
- `void io_splice_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a splice operation.
- `int io_tee_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a tee operation.
- `int io_tee(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a tee operation.

### tctx.h
Declares data structures and functions for managing per-task io_uring context nodes. Provides interfaces for allocating, adding, and cleaning up task context nodes, as well as registering/unregistering ring file descriptors. Supports efficient tracking and cancellation of io_uring operations per task.

**Function specifications:**
- `struct io_uring_task *io_uring_get_task_ctx(struct task_struct *task);`
  Retrieves the io_uring task context for a given task.
- `void io_uring_put_task_ctx(struct io_uring_task *tctx);`
  Releases a reference to the io_uring task context.
- `int io_uring_add_task_ctx(struct io_ring_ctx *ctx, struct io_uring_task *tctx);`
  Adds a task context node to the io_uring context.
- `void io_uring_remove_task_ctx(struct io_ring_ctx *ctx, struct io_uring_task *tctx);`
  Removes a task context node from the io_uring context.
- `int io_uring_register_ring_fd(struct io_ring_ctx *ctx, int fd);`
  Registers a ring file descriptor with the io_uring context.
- `void io_uring_unregister_ring_fd(struct io_ring_ctx *ctx, int fd);`
  Unregisters a ring file descriptor from the io_uring context.

### timeout.h
Declares data structures and functions for managing timeouts in io_uring. Provides interfaces for preparing, executing, and cancelling timeout requests, as well as handling linked timeouts and flushing outstanding timeouts. Integrates with kernel hrtimer APIs for precise asynchronous timeout management.

**Function specifications:**
- `int io_timeout_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a timeout operation.
- `int io_timeout(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a timeout operation.
- `int io_timeout_remove_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a timeout removal operation.
- `int io_timeout_remove(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a timeout removal operation.
- `void io_flush_timeouts(struct io_ring_ctx *ctx);`
  Flushes all outstanding timeouts in the context.

### truncate.h
Declares preparation and execution functions for ftruncate operations in io_uring. Provides interfaces for asynchronously truncating files to a specified length, integrating with kernel file truncation mechanisms.

**Function specifications:**
- `int io_ftruncate_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares an ftruncate operation.
- `int io_ftruncate(struct io_kiocb *req, unsigned int issue_flags);`
  Executes an ftruncate operation.

### uring_cmd.h
Declares data structures and functions for handling custom uring commands (uring_cmd) in io_uring. Provides interfaces for preparing, executing, and cleaning up uring_cmd requests, as well as importing fixed user vectors and supporting cancellation of in-flight commands. Enables integration of device-specific or custom kernel commands with the io_uring asynchronous model.

**Function specifications:**
- `int io_uring_cmd(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a uring_cmd operation for the given request.
- `int io_uring_cmd_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a uring_cmd operation from a submission queue entry.
- `void io_uring_cmd_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with a uring_cmd request.
- `bool io_uring_try_cancel_uring_cmd(struct io_ring_ctx *ctx, struct io_uring_task *tctx, bool cancel_all);`
  Attempts to cancel uring_cmd requests in the given context and task.
- `void io_cmd_cache_free(const void *entry);`
  Frees a cached uring_cmd entry.
- `int io_uring_cmd_import_fixed_vec(struct io_uring_cmd *ioucmd, const struct iovec __user *uvec, size_t uvec_segs, int ddir, struct iov_iter *iter, unsigned issue_flags);`
  Imports a fixed user vector for use in a uring_cmd operation.

---

### waitid.h
Declares data structures and functions for supporting asynchronous waitid operations in io_uring. Provides interfaces for preparing, executing, and cancelling waitid requests, as well as removing all matching waitid operations from a context. Integrates with kernel process exit and wait mechanisms to enable non-blocking process state change notifications.

**Function specifications:**
- `int io_waitid_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a waitid operation from a submission queue entry.
- `int io_waitid(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a waitid operation for the given request.
- `int io_waitid_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd, unsigned int issue_flags);`
  Cancels a waitid operation matching the given cancel data.
- `bool io_waitid_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx, bool cancel_all);`
  Removes all matching waitid operations from the context.

---

### xattr.h
Declares function prototypes for extended attribute (xattr) operations in io_uring. Provides interfaces for preparing, executing, and cleaning up setxattr and getxattr requests, supporting both file-descriptor-based and path-based variants. Enables asynchronous manipulation and retrieval of extended file attributes.

**Function specifications:**
- `void io_xattr_cleanup(struct io_kiocb *req);`
  Cleans up resources associated with an xattr request.
- `int io_fsetxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a file-descriptor-based setxattr operation.
- `int io_fsetxattr(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a file-descriptor-based setxattr operation.
- `int io_setxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a path-based setxattr operation.
- `int io_setxattr(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a path-based setxattr operation.
- `int io_fgetxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a file-descriptor-based getxattr operation.
- `int io_fgetxattr(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a file-descriptor-based getxattr operation.
- `int io_getxattr_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a path-based getxattr operation.
- `int io_getxattr(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a path-based getxattr operation.

---

### zcrx.h
Declares data structures and functions for zero-copy receive (ZCRX) support in io_uring. Provides interfaces for registering and unregistering ZCRX interface queues, handling zero-copy receive operations, and managing ZCRX areas and interface queues. Integrates with kernel networking and memory management to enable high-performance, zero-copy network data reception.

**Function specifications:**
- `int io_register_zcrx_ifq(struct io_ring_ctx *ctx, struct io_uring_zcrx_ifq_reg __user *arg);`
  Registers a ZCRX interface queue for the given context.
- `void io_unregister_zcrx_ifqs(struct io_ring_ctx *ctx);`
  Unregisters all ZCRX interface queues for the context.
- `void io_shutdown_zcrx_ifqs(struct io_ring_ctx *ctx);`
  Shuts down all ZCRX interface queues for the context.
- `int io_zcrx_recv(struct io_kiocb *req, struct io_zcrx_ifq *ifq, struct socket *sock, unsigned int flags, unsigned issue_flags, unsigned int *len);`
  Performs a zero-copy receive operation using the specified interface queue and socket.
- `int io_recvzc(struct io_kiocb *req, unsigned int issue_flags);`
  Executes a zero-copy receive operation.
- `int io_recvzc_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);`
  Prepares a zero-copy receive operation from a submission queue entry.