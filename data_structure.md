# Task 3: Data Structure Investigation
The objective of this task is to document all internal data structures defined in io_uring.

Structure name | Defined in | Attributes | Caller Functions Source | source caller | usage
---------------|------------|------------|-------------------------|---------------|-------------------
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
| | | | io_req_task_link_timeout | io_uring/timeout.c | local variable
| | | | io_link_timeout_fn | io_uring/timeout.c | local variable
| | | | io_linked_timeout_update | io_uring/timeout.c | local variable
| | | | io_timeout_update | io_uring/timeout.c | local variable
| | | | __io_timeout_prep | io_uring/timeout.c | local variable
| | | | io_timeout | io_uring/timeout.c | local variable
| | | | io_queue_linked_timeout | io_uring/timeout.c | local variable
| | | | io_kill_timeouts | io_uring/timeout.c | local variable
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
