# Task 3: Data Structure Investigation
The objective of this task is to document all internal data structures defined in io_uring. 

Structure name | Defined in | Attributes | Caller Functions Source | source caller | usage
---------------|------------|------------|-------------------------|---------------|-------------------
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
| | | | io_timeout_update | io_uring/timeout.c | local variable
| | | | io_timeout_remove | io_uring/timeout.c | local variable
| | | | io_timeout_cancel | io_uring/timeout.h | function parameter
| | | | io_waitid_cancel | io_uring/waitid.c | function parameter
| | | | io_waitid_cancel | io_uring/waitid.h | function parameter
io_epoll | io_uring/epoll.c | file, int, int, int, epoll_event | io_epoll_ctl_prep | io_uring/epoll.c | local variable
| | | | io_epoll_ctl | io_uring/epoll.c | local variable
io_epoll_wait | io_uring/epoll.c | file, int, epoll_event | io_epoll_wait_prep | io_uring/epoll.c | local variable
| | | | io_epoll_wait | io_uring/epoll.c | local variable
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


If the following row value in a column is missing, assume the value is the same with the previous row in the same column. 
Continue until all data structures documented properly.
