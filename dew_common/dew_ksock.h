#ifndef __DEW_KSOCK_H_
#define __DEW_KSOCK_H_

#include <linux/kernel.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/inet.h>

struct socket *dew_ksock_create(int domain, int type, int protocol);
int dew_ksock_connect(struct socket *sk, struct sockaddr *addr, int len);
ssize_t dew_ksock_send(struct socket *sk, const void *buff, size_t len, int flags);
ssize_t dew_ksock_recv(struct socket *sk, void *buff, size_t len, int flags);
int dew_ksock_shutdown(struct socket *sk, int how);
void dew_ksock_close(struct socket *sk);

#endif // __DEW_KSOCK_H_
