// SPDX-License-Identifier: GPL-2.0

#include <linux/net.h>
#include <linux/uio.h>
#include <linux/io_uring_types.h>

struct io_async_msghdr {
#if defined(CONFIG_NET)
	struct iou_vec				vec;

	struct_group(clear,
		int				namelen;
		struct iovec			fast_iov;
		__kernel_size_t			controllen;
		__kernel_size_t			payloadlen;
		struct sockaddr __user		*uaddr;
		struct msghdr			msg;
		struct sockaddr_storage		addr;
	);
#else
	struct_group(clear);
#endif
};

#if defined(CONFIG_NET)

/**
 * Prepares a shutdown request from an io_uring submission queue entry (SQE).
 */
int io_shutdown_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a shutdown operation on a socket.
 */
int io_shutdown(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Cleans up resources associated with a sendmsg or recvmsg request.
 */
void io_sendmsg_recvmsg_cleanup(struct io_kiocb *req);

/**
 * Prepares a sendmsg request from an io_uring submission queue entry (SQE).
 */
int io_sendmsg_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a sendmsg operation.
 */
int io_sendmsg(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Executes a send operation.
 */
int io_send(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a recvmsg request from an io_uring submission queue entry (SQE).
 */
int io_recvmsg_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a recvmsg operation.
 */
int io_recvmsg(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Executes a recv operation.
 */
int io_recv(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Handles failure for send and recv operations.
 */
void io_sendrecv_fail(struct io_kiocb *req);

/**
 * Prepares an accept request from an io_uring submission queue entry (SQE).
 */
int io_accept_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes an accept operation.
 */
int io_accept(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a socket creation request from an io_uring submission queue entry (SQE).
 */
int io_socket_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a socket creation operation.
 */
int io_socket(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a connect request from an io_uring submission queue entry (SQE).
 */
int io_connect_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a connect operation.
 */
int io_connect(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Executes a send_zc (zero-copy send) operation.
 */
int io_send_zc(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Executes a sendmsg_zc (zero-copy sendmsg) operation.
 */
int io_sendmsg_zc(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a send_zc (zero-copy send) request from an io_uring submission queue entry (SQE).
 */
int io_send_zc_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Cleans up resources associated with a send_zc (zero-copy send) request.
 */
void io_send_zc_cleanup(struct io_kiocb *req);

/**
 * Prepares a bind request from an io_uring submission queue entry (SQE).
 */
int io_bind_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a bind operation.
 */
int io_bind(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Prepares a listen request from an io_uring submission queue entry (SQE).
 */
int io_listen_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * Executes a listen operation.
 */
int io_listen(struct io_kiocb *req, unsigned int issue_flags);

/**
 * Frees resources associated with a cached network message.
 */
void io_netmsg_cache_free(const void *entry);
#else
static inline void io_netmsg_cache_free(const void *entry)
{
}
#endif
