// SPDX-License-Identifier: GPL-2.0

/**
 * Displays detailed information about the io_uring file descriptor in the
 * seq_file, including SQ/CQ states, user files, user buffers, and poll lists.
 */
void io_uring_show_fdinfo(struct seq_file *m, struct file *f);
