#ifndef NETLINK_H_INCLUDED
#define NETLINK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <linux/netlink.h>

#include "../common/mem_ops.h"
#include "../common/structures.h"
#include "../common/deserialize.h"

#define MAX_PAYLOAD 1024

int get_netlink_id(void);
int net_init(struct nlmsghdr **nlh, struct iovec *iov);
syscall_info *read_from_socket(int sock_fd, struct nlmsghdr *nlh);

#endif