# Task 3: Data Structure Investigation
The objective of this task is to document all internal data structures defined in io_uring. 

Structure name | Defined in | Attributes | Caller Functions Source | source caller | usage
---------------|------------|------------|-------------------------|---------------|-------------------
io_fadvise | io_uring/advise.c | file, u64 offset, u64 len, u32 advice | io_fadvise_force_async | io_uring/advise.c | function parameter
| | | | io_fadvise_prep | io_uring/advise.c | local variable
| | | | io_fadvise | io_uring/advise.c | local variable
io_madvise | io_uring/advise.c | file, u64 addr, u64 len, u32 advice | io_madvise_prep | io_uring/advise.c | local variable
| | | | io_madvise | io_uring/advise.c | local variable
io_cancel | io_uring/cancel.c | file, u64 addr, u32 flags, s32 f, u8 opcode | io_async_cancel_prep | io_uring/cancel.c | local variable
| | | | io_async_cancel | io_uring/cancel.c | local variable
io_cancel_data | io_uring/cancel.h | io_ring_ctx, u64 data, file, u8 opcode, u32 flags, int | io_try_cancel | io_uring/cancel.h | function parameter
| | | | io_cancel_req_match | io_uring/cancel.h | function parameter
| | | | io_cancel_remove | io_uring/cancel.h | function parameter
| | | | io_cancel_req_match | io_uring/cancel.c | function parameter
| | | | io_cancel_cb | io_uring/cancel.c | local variable
| | | | io_async_cancel_one | io_uring/cancel.c | function parameter
| | | | io_try_cancel | io_uring/cancel.c | function parameter
| | | | __io_async_cancel | io_uring/cancel.c | function parameter
| | | | io_async_cancel | io_uring/cancel.c | local variable
| | | | __io_sync_cancel | io_uring/cancel.c | function parameter
| | | | io_sync_cancel | io_uring/cancel.c | local variable
| | | | io_cancel_remove | io_uring/cancel.c | function parameter
| | | | io_futex_cancel | io_uring/futex.c | function parameter
| | | | io_futex_cancel | io_uring/futex.h | function parameter
| | | | io_poll_find | io_uring/poll.c | function parameter
| | | | io_poll_file_find | io_uring/poll.c | function parameter
| | | | __io_poll_cancel | io_uring/poll.c | function parameter
| | | | io_poll_cancel | io_uring/poll.c | function parameter
| | | | io_poll_remove | io_uring/poll.c | local variable
| | | | io_poll_cancel | io_uring/poll.h | function parameter
| | | | io_timeout_extract | io_uring/timeout.c | function parameter
| | | | io_timeout_cancel | io_uring/timeout.c | function parameter
| | | | io_req_task_link_timeout | io_uring/timeout.c | local variable
| | | | io_link_timeout_fn | io_uring/timeout.c | local variable
| | | | io_linked_timeout_update | io_uring/timeout.c | local variable 
| | | | io_timeout_update | io_uring/timeout.c | local variable
| | | | __io_timeout_prep | io_uring/timeout.c | local variable
| | | | io_timeout | io_uring/timeout.c | local variable
| | | | io_queue_linked_timeout | io_uring/timeout.c | local variable
| | | | io_kill_timeouts | io_uring/timeout.c | local variable
| | | | io_timeout_cancel | io_uring/timeout.h | function parameter
| | | | io_waitid_cancel | io_uring/waitid.c | function parameter
| | | | io_waitid_cancel | io_uring/waitid.h | function parameter
io_epoll | io_uring/epoll.c | file, int, int, int, epoll_event | io_epoll_ctl_prep | io_uring/epoll.c | local variable
| | | | io_epoll_ctl | io_uring/epoll.c | local variable
io_epoll_wait | io_uring/epoll.c | file, int, epoll_event | io_epoll_wait_prep | io_uring/epoll.c | local variable
| | | | io_epoll_wait | io_uring/epoll.c | local variable
io_ev_fd | io_uring/eventfd.c | eventfd_ctx, uint, uint, refcount_t, atomic_t, rcu_head | io_eventfd_free | io_uring/eventfd.c | local variable
| | | | io_eventfd_put | io_uring/eventfd.c | function parameter
| | | | io_eventfd_do_signal | io_uring/eventfd.c | local variable, function parameter
| | | | __io_eventfd_signal | io_uring/eventfd.c | function parameter
| | | | io_eventfd_grab | io_uring/eventfd.c | return value, local variable
| | | | io_eventfd_signal | io_uring/eventfd.c | local variable 
| | | | io_eventfd_flush_signal | io_uring/eventfd.c | local variable
| | | | io_eventfd_register | io_uring/eventfd.c | local variable
| | | | io_eventfd_unregister | io_uring/eventfd.c | function parameter
| | | | io_eventfd_release | io_uring/eventfd.c | function parameter
| | | | io_eventfd_trigger | io_uring/eventfd.c | function parameter
io_rename | io_uring/fs.c | file, int, int, filename, filename, int | io_renameat_prep | io_uring/fs.c | local variable
| | | | io_renameat | io_uring/fs.c | local variable
| | | | io_renameat_cleanup | io_uring/fs.c | local variable
io_unlink | io_uring/fs.c | file, int, int, filename | io_unlinkat_prep | io_uring/fs.c | local variable
| | | | io_unlinkat | io_uring/fs.c | local variable
| | | | io_unlinkat_cleanup | io_uring/fs.c | local variable
io_mkdir | io_uring/fs.c | file, int, umode_t, filename | io_mkdirat_prep | io_uring/fs.c | local variable
| | | | io_mkdirat | io_uring/fs.c | local variable
| | | | io_mkdirat_cleanup | io_uring/fs.c | local variable
io_link | io_uring/fs.c | file, int, int, filename, filename, int | io_symlinkat_prep | io_uring/fs.c | local variable
| | | | io_symlinkat | io_uring/fs.c | local variable
| | | | io_linkat_prep | io_uring/fs.c | local variable
| | | | io_linkat | io_uring/fs.c | local variable
| | | | io_link_cleanup | io_uring/fs.c | local variable
io_futex | io_uring/futex.c | file, u32, futex_waitv, ulong, ulong, ulong, u32, uint, bool | io_futexv_complete | io_uring/futex.c | local variable
| | | | io_futexv_claim | io_uring/futex.c | function parameter
| | | | __io_futex_cancel | io_uring/futex.c | local variable
| | | | io_futex_prep | io_uring/futex.c | local variable
| | | | io_futex_wakev_fn | io_uring/futex.c | local variable
| | | | io_futexv_prep | io_uring/futex.c | local variable
| | | | io_futexv_wait | io_uring/futex.c | local variable
| | | | io_futex_wait | io_uring/futex.c | local variable
| | | | io_futex_wake | io_uring/futex.c | local variable
io_futex_data | io_uring/futex.c | futex_q, io_kiocb | __io_futex_cancel | io_uring/futex.c | local variable
| | | | io_futex_wake_fn | io_uring/futex.c | local variable
| | | | io_futex_wait | io_uring/futex.c | local variable
| | | | io_alloc_cache_init | io_uring/futex.c | function parameter
io_defer_entry | io_uring/io_uring.c | list_head, io_kiocb, u32 | io_queue_deferred | io_uring/io_uring.c | local variable
| | | | io_drain_req | io_uring/io_uring.c | local variable
| | | | io_cancel_defer_files | io_uring/io_uring.c | local variable
ext_arg | io_uring/io_uring.c | size_t, timespec64, sigset_t, ktime_t, bool, bool | __io_cqring_wait_schedule | io_uring/io_uring.c | function parameter
| | | | io_cqring_wait_schedule | io_uring/io_uring.c | function parameter
| | | | io_cqring_wait | io_uring/io_uring.c | function parameter
| | | | io_get_ext_arg | io_uring/io_uring.c | function parameter
| | | | io_uring_enter | io_uring/io_uring.c | local variable
io_tctx_exit | io_uring/io_uring.c | callback_head, completion, io_ring_ctx | io_tctx_exit_cb | io_uring/io_uring.c | local variable
| | | | io_ring_exit_work | io_uring/io_uring.c | local variable
io_task_cancel | io_uring/io_uring.c | io_uring_task, bool | io_cancel_task_cb | io_uring/io_uring.c | local variable
| | | | io_uring_try_cancel_requests | io_uring/io_uring.c | local variable
io_wait_queue | io_uring/io_uring.h | wait_queue_entry, io_ring_ctx, uint, uint, uint, int, ktime_t, ktime_t, hrtimer, ktime_t, bool | io_should_wake | io_uring/io_uring.h | function parameter
| | | | io_wake_function | io_uring/io_uring.c | local variable
| | | | io_cqring_timer_wakeup | io_uring/io_uring.c | local variable
| | | | io_cqring_min_timer_wakeup | io_uring/io_uring.c | local variable
| | | | io_cqring_schedule_timeout | io_uring/io_uring.c | function parameter
| | | | __io_cqring_wait_schedule | io_uring/io_uring.c | function parameter
| | | | io_cqring_wait_schedule | io_uring/io_uring.c | function parameter
| | | | io_cqring_wait | io_uring/io_uring.c | local variable
| | | | io_napi_busy_loop_should_end | io_uring/napi.c | local variable
| | | | io_napi_blocking_busy_loop | io_uring/napi.c | function parameter
| | | | __io_napi_busy_loop | io_uring/napi.c | function parameter
io_worker | io_uring/io-wq.c | refcount_t, ulong, hlist_nulls_node, list_head, task_struct, io_wq, io_wq_acct, io_wq_work, raw_spinlock_t, completion, ulong, callback_head, int, rcu_head, delayed_work | io_wq_dec_running | io_uring/io-wq.c | function parameter
| | | | io_worker_get | io_uring/io-wq.c | function parameter
| | | | io_worker_release | io_uring/io-wq.c | function parameter
| | | | io_wq_get_acct | io_uring/io-wq.c  | function parameter
| | | | io_wq_worker_stopped | io_uring/io-wq.c  | local variable
| | | | io_worker_cancel_cb | io_uring/io-wq.c  | function parameter
| | | | io_task_worker_match | io_uring/io-wq.c  | local variable
| | | | io_worker_exit | io_uring/io-wq.c  | function parameter
| | | | io_acct_activate_free_worker | io_uring/io-wq.c  | local variable
| | | | io_wq_inc_running | io_uring/io-wq.c  | function parameter
| | | | create_worker_cb | io_uring/io-wq.c  | local variable
| | | | io_queue_worker_create | io_uring/io-wq.c  | function parameter
| | | | io_wq_dec_running | io_uring/io-wq.c  | function parameter
| | | | __io_worker_busy | io_uring/io-wq.c  | function parameter
| | | | __io_worker_idle | io_uring/io-wq.c  | function parameter
| | | | io_assign_current_work | io_uring/io-wq.c  | function parameter
| | | | io_worker_handle_work | io_uring/io-wq.c  | function parameter
| | | | io_wq_worker | io_uring/io-wq.c | local variable
| | | | io_wq_worker_running | io_uring/io-wq.c | local variable
| | | | io_wq_worker_sleeping | io_uring/io-wq.c | local variable
| | | | io_init_new_worker | io_uring/io-wq.c | function parameter
| | | | io_should_retry_thread | io_uring/io-wq.c | function parameter
| | | | queue_create_worker_retry | io_uring/io-wq.c | function parameter
| | | | create_worker_cont | io_uring/io-wq.c | local variable
| | | | io_workqueue_create | io_uring/io-wq.c | local variable
| | | | create_io_worker | io_uring/io-wq.c | local variable
| | | | io_acct_for_each_worker | io_uring/io-wq.c | function parameter, local variable
| | | | io_wq_for_each_worker | io_uring/io-wq.c | function parameter
| | | | io_wq_worker_wake | io_uring/io-wq.c | function parameter
| | | | __io_wq_worker_cancel | io_uring/io-wq.c | function parameter
| | | | io_wq_worker_cancel | io_uring/io-wq.c | function parameter
| | | | io_task_work_match | io_uring/io-wq.c | local variable
| | | | io_wq_cancel_tw_create | io_uring/io-wq.c | local variable
| | | | io_wq_worker_affinity | io_uring/io-wq.c | function parameter
io_wq_acct | io_uring/io-wq.c | raw_spinlock_t, uint, uint, atomic_t, hlist_nulls_head, list_head, raw_spinlock_t, io_wq_work_list, ulong | create_io_worker | io_uring/io-wq.c | function parameter
| | | | io_acct_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | io_worker_cancel_cb | io_uring/io-wq.c | local variable
| | | | io_worker_exit | io_uring/io-wq.c | local variable
| | | | __io_acct_run_queue | io_uring/io-wq.c | function parameter
| | | | io_acct_run_queue | io_uring/io-wq.c | function parameter
| | | | io_acct_activate_free_worker | io_uring/io-wq.c | function parameter
| | | | io_wq_create_worker | io_uring/io-wq.c | function parameter
| | | | io_wq_inc_running | io_uring/io-wq.c | local variable
| | | | create_worker_cb | io_uring/io-wq.c | local variable
| | | | io_queue_worker_create | io_uring/io-wq.c | function parameter
| | | | io_wq_dec_running | io_uring/io-wq.c | local variable
| | | | __io_worker_busy | io_uring/io-wq.c | function parameter
| | | | __io_worker_idle | io_uring/io-wq.c | function parameter
| | | | io_get_next_work | io_uring/io-wq.c | function parameter
| | | | io_worker_handle_work | io_uring/io-wq.c | function parameter
| | | | io_wq_worker | io_uring/io-wq.c | local variable
| | | | io_init_new_worker | io_uring/io-wq.c | function parameter
| | | | create_worker_cont | io_uring/io-wq.c | local variable
| | | | io_workqueue_create | io_uring/io-wq.c | local variable
| | | | create_io_worker | io_uring/io-wq.c | function parameter
| | | | io_acct_for_each_worker | io_uring/io-wq.c | function parameter
| | | | io_wq_insert_work | io_uring/io-wq.c | function parameter
| | | | io_wq_enqueue | io_uring/io-wq.c | function parameter
| | | | io_wq_remove_pending | io_uring/io-wq.c | function parameter
| | | | io_acct_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_pending_work | io_uring/io-wq.c | local variable
| | | | io_acct_cancel_running_work | io_uring/io-wq.c | function parameter
| | | | io_wq_hash_wake | io_uring/io-wq.c | local variable
| | | | io_wq_create | io_uring/io-wq.c | local variable
| | | | io_wq_max_workers | io_uring/io-wq.c | local variable
io_wq | io_uring/io-wq.c | ulong, free_work_fn, io_wq_work_fn, io_wq_hash, atomic_t, completion, hlist_node, task_struct, io_wq_acct, wait_queue_entry, io_wq_work, cpumask_var_t | create_io_worker | io_uring/io-wq.c | function parameter
| | | | io_acct_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_tw_create | io_uring/io-wq.c | function parameter
| | | | io_get_acct | io_uring/io-wq.c | function parameter
| | | | io_work_get_acct | io_uring/io-wq.c | function parameter
| | | | io_worker_ref_put | io_uring/io-wq.c | function parameter
| | | | io_worker_cancel_cb | io_uring/io-wq.c | local variable
| | | | io_worker_exit | io_uring/io-wq.c | local variable
| | | | io_wq_create_worker | io_uring/io-wq.c | function parameter
| | | | create_worker_cb | io_uring/io-wq.c | local variable
| | | | io_queue_worker_create | io_uring/io-wq.c | local variable
| | | | io_wq_dec_running | io_uring/io-wq.c | local variable
| | | | io_wait_on_hash | io_uring/io-wq.c | function parameter
| | | | io_get_next_work | io_uring/io-wq.c | funtion parameter
| | | | io_worker_handle_work | io_uring/io-wq.c | local variable
| | | | io_wq_worker | io_uring/io-wq.c | local variable
| | | | io_init_new_worker | io_uring/io-wq.c | function parameter
| | | | create_worker_cont | io_uring/io-wq.c | local variable
| | | | create_io_worker | io_uring/io-wq.c | function parameter
| | | | io_wq_for_each_worker | io_uring/io-wq.c | function parameter
| | | | io_run_cancel | io_uring/io-wq.c | function parameter
| | | | io_wq_insert_work | io_uring/io-wq.c | function parameter
| | | | io_wq_enqueue | io_uring/io-wq.c | function parameter
| | | | io_wq_remove_pending | io_uring/io-wq.c | function parameter
| | | | io_acct_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_running_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_cb | io_uring/io-wq.c | function parameter
| | | | io_wq_hash_wake | io_uring/io-wq.c | local variable
| | | | io_wq_create | io_uring/io-wq.c | local variable
| | | | io_wq_exit_start | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_tw_create | io_uring/io-wq.c | function parameter
| | | | io_wq_exit_workers | io_uring/io-wq.c | function parameter
| | | | io_wq_destroy | io_uring/io-wq.c | function parameter
| | | | io_wq_put_and_exit | io_uring/io-wq.c | function parameter
| | | | __io_wq_cpu_online | io_uring/io-wq.c | function parameter
| | | | io_wq_cpu_online | io_uring/io-wq.c | local variable
| | | | io_wq_cpu_offline | io_uring/io-wq.c | local variable
| | | | io_wq_max_workers | io_uring/io-wq.c | function parameter
| | | | io_wq_exit_start | io_uring/io-wq.h | function parameter
| | | | io_wq_put_and_exit | io_uring/io-wq.h | function parameter
| | | | io_wq_enqueue | io_uring/io-wq.h | function parameter
| | | | io_wq_max_workers | io_uring/io-wq.h | function parameter
| | | | io_wq_cancel_cb | io_uring/io-wq.h | function parameter
| | | | io_uring_clean_tctx | io_uring/tctx.c | local variable
io_cb_cancel_data | io_uring/io-wq.c | work_cancel_fn, void, int, int, bool | io_acct_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | create_worker_cont | io_uring/io-wq.c | local variable
| | | | io_wq_enqueue | io_uring/io-wq.c | local variable
| | | | __io_wq_worker_cancel | io_uring/io-wq.c | function parameter
| | | | io_wq_worker_cancel | io_uring/io-wq.c | local variable
| | | | io_acct_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_pending_work | io_uring/io-wq.c | function parameter
| | | | io_acct_cancel_running_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_running_work | io_uring/io-wq.c | function parameter
| | | | io_wq_cancel_cb | io_uring/io-wq.c | local variable
| | | | io_wq_destroy | io_uring/io-wq.c | local variable
online_data | io_uring/io-wq.c | uint, bool | io_wq_worker_affinity | io_uring/io-wq.c | local variable
| | | | __io_wq_cpu_online | io_uring/io-wq.c | local variable
io_wq_hash | io_uring/io-wq.h | refcount_t, ulong, wait_queue_head | io_wq_put_hash | io_uring/io-wq.h | function parameter
| | | | io_init_wq_offload | io_uring/tctx.c | local variable
io_wq_data | io_uring/io-wq.h | io_wq_hash, task_struct, io_wq_work_fn, free_work_fn | io_wq_create | io_uring/io-wq.h | function parameter
| | | | io_wq_create | io_uring/io-wq.c | function parameter
| | | | io_init_wq_offload | io_uring/tctx.c | local variable
io_provide_buf | io_uring/kbuf.c | file, __64, __u32, __u32, __u32, __16 | io_remove_buffers_prep | io_uring/kbuf.c | local variable
| | | | io_remove_buffers | io_uring/kbuf.c | local variable
| | | | io_provide_buffers_prep | io_uring/kbuf.c | local variable
| | | | io_add_buffers | io_uring/kbuf.c | function parameter
| | | | io_provide_buffers | io_uring/kbuf.c | local variable
io_buffer_list | io_uring/kbuf.h | list_head, io_uring_buf_ring, __u16, __u16, __u16, __u16, __u16, __u16, io_mapped_region | io_kbuf_commit | io_uring/kbuf.h | function parameter
| | | | io_kbuf_inc_commit | io_uring/kbuf.c | function parameter
| | | | io_kbuf_commit | io_uring/kbuf.c | function parameter
| | | | io_buffer_add_list | io_uring/kbuf.c | function parameter
| | | | io_kbuf_recycle_legacy | io_uring/kbuf.c | local variable
| | | | io_provided_buffer_select | io_uring/kbuf.c | function parameter
| | | | io_provided_buffers_select | io_uring/kbuf.c | function parameter
| | | | io_ring_buffer_select | io_uring/kbuf.c | function parameter
| | | | io_buffer_select | io_uring/kbuf.c | local variable
| | | | io_ring_buffers_peek | io_uring/kbuf.c | function parameter
| | | | io_buffers_select | io_uring/kbuf.c | local variable
| | | | io_buffers_peek | io_uring/kbuf.c | local variable
| | | | __io_put_kbuf_ring | io_uring/kbuf.c | local variable
| | | | __io_remove_buffers | io_uring/kbuf.c | function parameter
| | | | io_put_bl | io_uring/kbuf.c | function parameter
| | | | io_destroy_buffers | io_uring/kbuf.c | local variable
| | | | io_destroy_bl | io_uring/kbuf.c | function parameter
| | | | io_remove_buffers | io_uring/kbuf.c | local variable
| | | | io_add_buffers | io_uring/kbuf.c | function parameter
| | | | io_provide_buffers | io_uring/kbuf.c | local variable
| | | | io_register_pbuf_ring | io_uring/kbuf.c | local variable
| | | | io_unregister_pbuf_ring | io_uring/kbuf.c | local variable
| | | | io_register_pbuf_status | io_uring/kbuf.c | local variable
| | | | io_pbuf_get_region | io_uring/kbuf.c | local variable
io_buffer | io_uring/kbuf.h | list_head, __u64, __u32, __u16, __u16 | io_kbuf_recycle_legacy | io_uring/kbuf.c | local variable
| | | | io_provided_buffer_select | io_uring/kbuf.c | local variable
| | | | __io_remove_buffers | io_uring/kbuf.c | local variable
| | | | io_add_buffers | io_uring/kbuf.c | local variable
buf_sel_arg | io_uring/kbuf.h | iovec, size_t, size_t, ushort, ushort | io_buffers_select | io_uring/kbuf.h | function parameter
| | | | io_buffers_peek | io_uring/kbuf.h | function parameter
| | | | io_ring_buffers_peek | io_uring/kbuf.c | function parameter
| | | | io_buffers_select | io_uring/kbuf.c | function parameter
| | | | io_buffers_peek | io_uring/kbuf.c | function parameter
| | | | io_send_select_buffer | io_uring/net.c | local variable
| | | | io_recv_buf_select | io_uring/net.c | local variable
io_msg | io_uring/msg_ring.c | file, callback_head, u64, u32 | io_msg_ring_cleanup | io_uring/msg_ring.c | local variable
| | | io_msg_data_remote | io_uring/msg_ring.c | function parameter 
| | | __io_msg_ring_data | io_uring/msg_ring.c | local variable
| | | io_msg_ring_data | io_uring/msg_ring.c | function parameter
| | | io_msg_grab_file | io_uring/msg_ring.c  | local variable
| | | io_msg_install_complete | io_uring/msg_ring.c | local variable
| | | io_msg_tw_fd_complete | io_uring/msg_ring.c | local variable
| | | io_msg_fd_remote | io_uring/msg_ring.c | local variable
| | | io_msg_send_fd | io_uring/msg_ring.c  | local variable
| | | __io_msg_ring_prep | io_uring/msg_ring.c  | function parameter
| | | io_msg_ring_prep | io_uring/msg_ring.c | function parameter
| | | io_msg_ring | io_uring/msg_ring.c | local variable
| | | io_uring_sync_msg_ring | io_uring/msg_ring.c | local variable
io_napi_entry | io_uring/napi.c | uint, list_head, ulong, rcu_head | io_napi_hash_find | io_uring/napi.c | function parameter
| | | __io_napi_add_id | io_uring/napi.c | local variable
| | | __io_napi_del_id | io_uring/napi.c | local variable
| | | __io_napi_remove_stale | io_uring/napi.c | local variable
| | | static_tracking_do_busy_loop | io_uring/napi.c | local variable
| | | dynamic_tracking_do_busy_loop| io_uring/napi.c | local variable
| | | io_napi_free | io_uring/napi.c | local variable
io_shutdown | io_uring/net.c | file, int | io_shutdown_prep | io_uring/net.c | local variable
| | | io_shutdown | io_uring/net.c | local variable
io_accept | io_uring/net.c | file, sockaddr, _user, int, u32, ulong | io_accept_prep | io_uring/net.c | local variable
| | | io_accept | io_uring/net.c | local variable
io_socket | io_uring/net.c  | file, int, u32, ulong | io_socket_prep | io_uring/net.c | local variable
| | | io_socket | io_uring/net.c | local variable
io_connect | io_uring/net.c | file, sockaddr, int, bool | io_connect_prep | io_uring/net.c  | local variable
| | | io_connect | io_uring/net.c | local variable
io_bind | io_uring/net.c | file, int | io_bind_prep | io_uring/net.c | local variable
| | | io_bind_prep | io_uring/net.c | local variable
io_listen | io_uring/net.c | file, int | io_listen_prep | io_uring/net.c | local variable
| | | io_listen | io_uring/net.c | local variable
io_sr_msg | io_uring/net.c | file, union, int, unsigned, u16, bool, void __user, io_kiocb | io_mshot_prep_retry | io_uring/net.c | local variable
| | | io_compat_msg_copy_hdr | io_uring/net.c | local variable
| | | io_msg_copy_hdr | io_uring/net.c | local variable
| | | io_send_setup | io_uring/net.c | local variable
| | | io_sendmsg_setup | io_uring/net.c | local variable
| | | io_sendmsg_prep | io_uring/net.c | local variable
| | | io_send_finish | io_uring/net.c | local variable
| | | io_sendmsg | io_uring/net.c | local variable
| | | io_send_select_buffer | io_uring/net.c | local variable
| | | io_send | io_uring/net.c | local variable
| | | io_recvmsg_prep_setup | io_uring/net.c | local variable
| | | io_recvmsg_prep | io_uring/net.c | local variable
| | | io_recv_finish| io_uring/net.c | local variable
| | | io_recvmsg_prep_multishot | io_uring/net.c | function parameter
| | | io_recvmsg | io_uring/net.c | local variable
| | | io_recv_buf_select | io_uring/net.c | local variable
| | | io_recv | io_uring/net.c | local variable
| | | io_send_zc_cleanup | io_uring/net.c | local variable
| | | io_send_zc_prep | io_uring/net.c | local variable
| | | io_send_zc_import | io_uring/net.c | local variable
| | | io_send_zc | io_uring/net.c | local variable
| | | io_sendmsg_zc | io_uring/net.c | local variable
| | | io_sendrecv_fail | io_uring/net.c | local variable
io_recvzc | io_uring/net.c | file, unsigned, u16, u32, io_zcrx_ifq | io_recvzc_prep | io_uring/net.c | local variable
| | | io_recvzc | io_uring/net.c | local variable
io_recvmsg_multishot_hdr |  io_uring/net.c | msg, addr | io_recvmsg_multisho | io_uring/net.c | local variable
io_async_msghdr | io_uring/net.h | iou_vec, group, int, __kernel_size_t, sockaddr, msghdr, sockaddr_storage | io_netmsg_iovec_free | io_uring/net.c | function parameter 
| | | io_netmsg_recycle | io_uring/net.c | local variable
| | | io_msg_alloc_async | io_uring/net.c | local variable
| | | io_mshot_prep_retry | io_uring/net.c | function parameter
| | | io_net_import_vec | io_uring/net.c | function parameter
| | | io_compat_msg_copy_hdr | io_uring/net.c | function parameter
| | | io_msg_copy_hdr | io_uring/net.c | function parameter
| | | io_sendmsg_recvmsg_cleanup | io_uring/net.c | local variable
| | | io_send_setup | io_uring/net.c | local variable
| | | io_sendmsg_setup | io_uring/net.c | local variable
| | | io_bundle_nbufs | io_uring/net.c | function parameter
| | | io_send_finish | io_uring/net.c | function parameter
| | | io_sendmsg | io_uring/net.c | local variable
| | | io_send_select_buffer | io_uring/net.c | function parameter
| | | io_send | io_uring/net.c | local variable
| | | io_recvmsg_mshot_prep | io_uring/net.c | function parameter
| | | io_recvmsg_copy_hdr | io_uring/net.c | function parameter
| | | io_recvmsg_prep_setup | io_uring/net.c | local variable
| | | io_recv_finish | io_uring/net.c | function parameter
| | | io_recvmsg_prep_multishot | io_uring/net.c | function parameter
| | | io_recvmsg_multishot | io_uring/net.c | function parameter
| | | io_recvmsg | io_uring/net.c | local variable
| | | io_recv_buf_select | io_uring/net.c | function parameter
| | | io_recv | io_uring/net.c | local variable
| | | io_send_zc_cleanup | io_uring/net.c | local variable
| | | io_send_zc_prep | io_uring/net.c | local variable
| | | io_send_zc_import | io_uring/net.c | local variable
| | | io_send_zc | io_uring/net.c | local variable
| | | io_sendmsg_zc | io_uring/net.c | local variable
| | | io_connect_prep | io_uring/net.c | local variable
| | | io_connect | io_uring/net.c | local variable
| | | io_bind_prep | io_uring/net.c | local variable
| | | io_bind | io_uring/net.c | local variable
| | | io_netmsg_cache_free | io_uring/net.c | local variable
io_nop | io_uring/nop.c | file, int, uint | io_nop_prep | io_uring/nop.c | local variable
| | | io_nop | io_uring/nop.c | local variable
io_notif_data | io_uring/notif.h | file, ubuf_info, io_notif_data, unsigned, bool | io_notif_to_data | io_uring/notif.h | local variable
| | | io_notif_flush | io_uring/notif.h | local variable 
| | | io_notif_account_mem | io_uring/notif.h | local variable 
io_open | io_uring/openclose.c | file, int, u32, filename, open_how, ulong | io_openat_force_async | io_uring/openclose.c | function parameter
| | | __io_openat_prep | io_uring/openclose.c | local variable
| | | io_openat_prep | io_uring/openclose.c | local variable
| | | io_openat2_prep | io_uring/openclose.c | local variable
| | | io_openat2 | io_uring/openclose.c | local variable
| | | io_open_cleanup | io_uring/openclose.c | local variable
io_close | io_uring/openclose.c | file, int, u32 | io_close_fixed | io_uring/openclose.c | local variable
| | | io_close_prep | io_uring/openclose.c | local variable
| | | io_close | io_uring/openclose.c | local variable
io_fixed_install | io_uring/openclose.c | file, int | io_install_fixed_fd_prep | io_uring/openclose.c | local variable
| | | io_install_fixed_fd | io_uring/openclose.c | local variable
io_poll_update | io_uring/poll.c | file, u64, __poll_t, bool | io_poll_remove_prep | io_uring/poll.c | local variable
| | | io_poll_remove | io_uring/poll.c | local variable
io_poll_table | io_uring/poll.c | poll_table_struct, io_kiocb, int, bool, _poll_t | __io_queue_proc | io_uring/poll.c | function parameter
| | | io_poll_queue_proc | io_uring/poll.c | local variable 
| | | io_poll_can_finish_inline | io_uring/poll.c | function parameter
| | | __io_arm_poll_handler | io_uring/poll.c | function parameter
| | | io_async_queue_proc | io_uring/poll.c | local variable 
| | | io_arm_poll_handler | io_uring/poll.c | local variable 
| | | io_poll_add | io_uring/poll.c | local variable 
io_poll | io_uring/poll.h | file, wait_queue_head, __poll_t, int, wait_queue_entry | io_poll_get_double | io_uring/poll.c | local variable
| | | io_poll_get_single | io_uring/poll.c | local variable
| | | io_init_poll_iocb | io_uring/poll.c | function parameter
| | | io_poll_remove_entry | io_uring/poll.c | function parameter
| | | io_pollfree_wake | io_uring/poll.c | function parameter
| | | io_poll_wake | io_uring/poll.c | local variable
| | | io_poll_double_prepare | io_uring/poll.c | local variable
| | | __io_queue_proc | io_uring/poll.c | function parameter
| | | io_poll_queue_proc | io_uring/poll.c | local variable
| | | __io_arm_poll_handler | io_uring/poll.c | function parameter
| | | io_poll_add_prep | io_uring/poll.c | local variable
| | | io_poll_add | io_uring/poll.c | local variable
async_poll | io_uring/poll.h | io_poll | io_async_queue_proc | io_uring/poll.c | local variable
| | | io_req_alloc_apoll | io_uring/poll.c | local variable
| | | io_arm_poll_handler| io_uring/poll.c | local variable
io_ring_ctx_rings | io_uring/register.c | io_rings, io_uring_sqe, io_mapped_region | io_register_free_rings | io_uring/register.c | function parameter
| | | io_register_resize_rings | io_uring/register.c | local variable
io_rsrc_update | io_uring/rsrc.c | file, u64, u32 | io_files_update_prep | io_uring/rsrc.c | local variable 
| | | io_files_update_with_index_alloc | io_uring/rsrc.c | local variable 
| | | io_files_update | io_uring/rsrc.c | local variable 
io_rsrc_node | io_uring/rsrc.h | uchar, int, u63, union | io_rsrc_node_alloc | io_uring/rsrc.h | local variable
| | | io_free_rsrc_node | io_uring/rsrc.h | function parameter 
| | | io_find_buf_node | io_uring/rsrc.h | local variable
| | | io_rsrc_node_lookup | io_uring/rsrc.h | local variable
| | | io_put_rsrc_node | io_uring/rsrc.h | function parameter 
| | | io_reset_rsrc_node | io_uring/rsrc.h | local variable
| | | io_req_assign_rsrc_node| io_uring/rsrc.h | function parameter 
| | | io_req_assign_buf_node | io_uring/rsrc.h | function parameter 
| | | io_sqe_buffer_register | io_uring/rsrc.h | local variable
| | | io_rsrc_node_alloc | io_uring/rsrc.h | local variable
| | | io_rsrc_cache_init | io_uring/rsrc.h | function parameter 
| | | io_rsrc_data_alloc | io_uring/rsrc.h | function parameter 
| | | io_free_rsrc_node | io_uring/rsrc.h | function parameter 
| | | io_sqe_files_register | io_uring/rsrc.h | local variable
| | | headpage_already_acct | io_uring/rsrc.h | local variable
| | | io_sqe_buffer_register | io_uring/rsrc.h | local variable
| | | io_sqe_buffer_register | io_uring/rsrc.h | local variable
| | | io_buffer_register_bvec | io_uring/rsrc.h | local variable
| | | io_buffer_unregister_bvec | io_uring/rsrc.h | local variable
| | | io_find_buf_node | io_uring/rsrc.h | local variable
| | | io_import_reg_buf | io_uring/rsrc.h | local variable
| | | io_import_reg_vec | io_uring/rsrc.h | local variable
io_mapped_ubuf | io_uring/rsrc.h | u64, uint, refcount_t, ulong, void, bool, u8, bio_vec | io_release_ubuf | io_uring/rsrc.c | local variable
| | | io_alloc_imu | io_uring/rsrc.c | local variable
| | | io_free_imu | io_uring/rsrc.c | function parameter
| | | io_buffer_unmap | io_uring/rsrc.c | function parameter
| | | io_rsrc_cache_init | io_uring/rsrc.c | function parameter
| | | headpage_already_acct | io_uring/rsrc.c | function parameter
| | | io_buffer_account_pin | io_uring/rsrc.c | function parameter
| | | io_sqe_buffer_register | io_uring/rsrc.c | local variable
| | | io_buffer_register_bvec | io_uring/rsrc.c | local variable
| | | validate_fixed_range | io_uring/rsrc.c | function parameter
| | | io_import_fixed | io_uring/rsrc.c | function parameter
| | | io_vec_fill_bvec | io_uring/rsrc.c | function parameter
| | | io_estimate_bvec_size | io_uring/rsrc.c | function parameter
| | | io_vec_fill_kern_bvec | io_uring/rsrc.c | function parameter
| | | iov_kern_bvec_size | io_uring/rsrc.c | function parameter
| | | io_kern_bvec_size | io_uring/rsrc.c | function parameter
| | | io_import_reg_vec | io_uring/rsrc.c | local variable
io_imu_folio_data | io_uring/rsrc.h | uint | io_check_coalesce_buffer | io_uring/rsrc.h | function parameter
| | | io_coalesce_buffer | io_uring/rsrc.c | function parameter
| | | io_check_coalesce_buffer | io_uring/rsrc.c | function parameter
| | | o_sqe_buffer_register | io_uring/rsrc.c | local variable
io_rw | io_uring/rw.c | kiocb, u64, u32, rwf_t | io_iov_compat_buffer_select_prep | io_uring/rw.c | function parameter
| | | io_iov_buffer_select_prep | io_uring/rw.c | local variable
| | | __io_import_rw_buffer | io_uring/rw.c | local variable
| | | io_prep_rw_pi | io_uring/rw.c | function parameter
| | | __io_prep_rw | io_uring/rw.c | local variable
| | | io_init_rw_fixed | io_uring/rw.c | local variable
| | | io_rw_import_reg_vec | io_uring/rw.c | local variable
| | | io_rw_prep_reg_vec | io_uring/rw.c | local variable
| | | io_read_mshot_prep | io_uring/rw.c | local variable
| | | io_kiocb_update_pos | io_uring/rw.c | local variable
| | | io_rw_should_reissue | io_uring/rw.c | local variable
| | | io_req_end_write | io_uring/rw.c | local variable
| | | io_req_io_end | io_uring/rw.c | local variable
| | | io_req_rw_complete | io_uring/rw.c | local variable
| | | io_complete_rw | io_uring/rw.c | local variable
| | | io_complete_rw_iopoll | io_uring/rw.c | local variable
| | | io_rw_done | io_uring/rw.c | local variable
| | | kiocb_done | io_uring/rw.c | local variable
| | | loop_rw_iter | io_uring/rw.c | function parameter
| | | io_async_buf_func | io_uring/rw.c | local variable
| | | io_rw_should_retry | io_uring/rw.c | local variable
| | | io_iter_do_read | io_uring/rw.c | function parameter
| | | io_rw_init_file | io_uring/rw.c | local variable
| | | __io_read | io_uring/rw.c | local variable
| | | io_read_mshot | io_uring/rw.c | local variable
| | | io_write | io_uring/rw.c | local variable
| | | io_uring_classic_poll| io_uring/rw.c | local variable
io_meta_state | io_uring/rw.h | u32, iov_iter_state | io_uring_classic_poll | io_uring/rw.c | local variable
io_async_rw | io_uring/rw.h | u32, iov_iter_state | io_uring_classic_poll | io_uring/rw.c | local variable
io_splice | io_uring/splice.c | file_out, off_out, off_in, len, splice_fd_in, flags, rsrc_node | __io_splice_prep | io_uring/splice.c | local variable
| | | | io_splice_cleanup | io_uring/splice.c | local variable
| | | | io_splice_get_file | io_uring/splice.c | local variable
| | | |  io_tee | io_uring/splice.c | local variable
| | | | io_splice_prep | io_uring/splice.c | local variable
| | | | io_splice | io_uring/splice.c | local variable
io_sq_data | io_uring/sqpoll.h | refs, park_pending, lock, ctx_list, thread, wait, sq_thread_idle, sq_cpu, task_pid, task_tgid, work_time, state, exited | io_sq_thread_stop | io_uring/sqpoll.h | function parameter
| | | | io_sq_thread_park | io_uring/sqpoll.h | function parameter
| | | | io_sq_thread_unpark | io_uring/sqpoll.h | function parameter
| | | | io_put_sq_data | io_uring/sqpoll.h | function parameter
io_statx | io_uring/statx.c | file, dfd, mask, flags, filename, buffer | io_statx_prep | io_uring/statx.c | local variable
| | | | io_statx | io_uring/statx.c | local variable
| | | | io_statx_cleanup | io_uring/statx.c | local variable
io_sync | io_uring/sync.c | file, len, off, flags, mode | io_sfr_prep | io_uring/sync.c | local variable
| | | | io_sync_file_range | io_uring/sync.c | local variable
| | | | io_fsync_prep | io_uring/sync.c | local variable
| | | | io_fsync | io_uring/sync.c | local variable
| | | | io_fallocate_prep | io_uring/sync.c | local variable
| | | | io_fallocate | io_uring/sync.c | local variable
io_tctx_node | io_uring/tctx.h | ctx_node, task, ctx | 
io_timeout | io_uring/timeout.c | file, off, target_seq, repeats, list, head, prev | io_is_timeout_noseq | io_uring/timeout.c | local variable
| | | | io_timeout_finish | io_uring/timeout.c | function parameter
| | | | io_timeout_complete | io_uring/timeout.c | local variable
| | | | io_flush_killed_timeouts | io_uring/timeout.c | local variable
| | | | io_kill_timeout | io_uring/timeout.c | local variable
| | | | io_flush_timeouts | io_uring/timeout.c | local variable
| | | | __io_disarm_linked_timeout | io_uring/timeout.c | local variable
| | | | io_timeout_fn | io_uring/timeout.c | local variable
| | | | io_timeout_extract | io_uring/timeout.c | local variable
io_timeout_rem | io_uring/timeout.c | file, addr, ts, flags, ltimeout | io_timeout_remove_prep | io_uring/timeout.c | local variable
| | | | io_timeout_remove | io_uring/timeout.c | local variable
io_timeout_data | io_uring/timeout.h | req, timer, ts, mode, flags | io_is_timeout_noseq | io_uring/timeout.c | local variable
| | | | io_timeout_finish | io_uring/timeout.c | local variable
| | | | io_timeout_complete | io_uring/timeout.c | local variable
| | | | io_kill_timeout | io_uring/timeout.c | local variable
| | | | __io_disarm_linked_timeout | io_uring/timeout.c | local variable
| | | | io_timeout_fn | io_uring/timeout.c | local variable
| | | | io_timeout_extract | io_uring/timeout.c | local variable 
| | | | io_link_timeout_fn | io_uring/timeout.c | local variable
| | | | io_timeout_get_clock | io_uring/timeout.c | function parameter 
| | | | io_linked_timeout_update | io_uring/timeout.c | local variable
| | | | io_timeout_update | io_uring/timeout.c | local variable
| | | | __io_timeout_prep | io_uring/timeout.c | local variable
| | | | io_timeout | io_uring/timeout.c | local variable
| | | | io_queue_linked_timeout | io_uring/timeout.c | local variable
io_ftrunc | io_uring/truncate.c | file, len | io_ftruncate_prep | io_uring/truncate.c | local variable
| | | | io_ftruncate | io_uring/truncate.c | local variable
io_async_cmd | io_uring/uring_cmd.h | data, vec, sqes | 
io_waitid | io_uring/waitid.c | file, which, upid, options, refs, head, infop, info | io_waitid_compat_copy_si | io_uring/waitid.c | function parameter
| | | | io_waitid_copy_si | io_uring/waitid.c | local variable
| | | | io_waitid_complete | io_uring/waitid.c | local variable
| | | | __io_waitid_cancel | io_uring/waitid.c | local variable
| | | | io_waitid_drop_issue_ref | io_uring/waitid.c | local variable
| | | | io_waitid_cb | io_uring/waitid.c | local variable
| | | | io_waitid_wait | io_uring/waitid.c | local variable
| | | | io_waitid_prep | io_uring/waitid.c | local variable
| | | | io_waitid | io_uring/waitid.c | local variable
io_waitid_async | io_uring/waitid.h | req. wo | io_waitid_free | io_uring/waitid.c | local variable
| | | | __io_waitid_cancel | io_uring/waitid.c | local variable
| | | | io_waitid_drop_issue_ref | io_uring/waitid.c | local variable
| | | | io_waitid_cb | io_uring/waitid.c | local variable
| | | | io_waitid_wait | io_uring/waitid.c | local variable
| | | | io_waitid_prep | io_uring/waitid.c | local varibale
| | | |io_waitid | io_uring/waitid.c | local variable
io_xattr | io_uring/xattr.c | file, ctx, filename | io_xattr_cleanup | io_uring/xattr.c | local variable
| | | | __io_getxattr_prep | io_uring/xattr.c | local variable
| | | | io_getxattr_prep | io_uring/xattr.c | local variable
| | | | io_fgetxattr | io_uring/xattr.c | local variable
| | | | io_getxattr | io_uring/xattr.c | local variable
| | | | __io_setxattr_prep | io_uring/xattr.c | local variable
| | | | io_setxattr_prep | io_uring/xattr.c | local variable
| | | | io_fsetxattr | io_uring/xattr.c | local variable
| | | | io_setxattr | io_uring/xattr.c | local variable
io_zcrx_args | io_uring/zcrx.c | req, ifq, sock, nr_skbs | io_zcrx_recv_skb | io_uring/zcrx.c | local variable
| | | | io_zcrx_tcp_recvmsg | io_uring/zcrx.c | local variable
io_zcrx_area | io_uring/zcrx.h | nia, ifq, user_refs, is_mapped, area_id, pages, nr_folios, freelist_lock, free_count, freelist | __io_zcrx_unmap_area | io_uring/zcrx.c | local variable
| | | | io_zcrx_unmap_area | io_uring/zcrx.c | function parameter
| | | | io_zcrx_map_area | io_uring/zcrx.c | function parameter
| | | | io_zcrx_iov_to_area | io_uring/zcrx.c | return type
| | | | io_get_user_counter | io_uring/zcrx.c | local variable
| | | | io_zcrx_iov_page | io_uring/zcrx.c | local variable
| | | | io_zcrx_free_area | io_uring/zcrx.c | function parameter
| | | | io_zcrx_create_area | io_uring/zcrx.c | local variable
| | | | __io_zcrx_get_free_niov | io_uring/zcrx.c | function parameter
| | | | io_zcrx_return_niov_freelist | io_uring/zcrx.c | local variable
| | | | io_zcrx_scrub | io_uring/zcrx.c | local variable
| | | | io_zcrx_ring_refill | io_uring/zcrx.c | local variable
| | | | io_zcrx_refill_slow | io_uring/zcrx.c | local variable
| | | | io_pp_zc_destroy | io_uring/zcrx.c | local variable
| | | | io_zcrx_queue_cqe | io_uring/zcrx.c | local variable
| | | | io_zcrx_alloc_fallback | io_uring/zcrx.c | function parameter
| | | | io_zcrx_copy_chunk | io_uring/zcrx.c | local variable

If the following row value in a column is missing, assume the value is the same with the previous row in the same column. 
Continue until all data structures documented properly.
