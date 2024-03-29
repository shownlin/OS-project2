#include <linux/module.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include "ksocket.h"

#define KSOCKET_NAME	"ksocket"
#define KSOCKET_VERSION	"0.0.2"
#define KSOCKET_DESCPT	"BSD-style socket APIs for kernel 2.6 developers"
#define KSOCKET_AUTHOR	"msn : song.xian-guang@hotmail.com\n"\
						"blog: http://sxg.cublog.cn"
#define KSOCKET_DATE	"2008-05-15"

MODULE_AUTHOR(KSOCKET_AUTHOR);
MODULE_DESCRIPTION(KSOCKET_NAME"-"KSOCKET_VERSION"\n"KSOCKET_DESCPT);
MODULE_LICENSE("Dual BSD/GPL");

ssize_t receive(_t socket, void *buffer, size_t length, int flags)
{
	struct socket *sk;
	struct msghdr msg;
	struct iovec iov;
	int ret;
#ifndef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	memset(&msg,0,sizeof(msg));
	sk = (struct socket *)socket;

	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

	//type
	msg.msg_iter.type = READ;
	//address
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	//msg_iter
	msg.msg_iter.iov = &iov;
	msg.msg_iter.iov_offset = 0;
	msg.msg_iter.count = iov.iov_len;
	msg.msg_iter.nr_segs = 1;
	//control
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

#ifndef KSOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif
	//hardik
	ret = sock_recvmsg(sk, &msg, flags);
#ifndef KSOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	if (ret < 0)
		goto out_receive;
	
out_receive:
	return ret;

}

ssize_t transmit(_t socket, const void *buffer, size_t length, int flags)
{
	struct socket *sk;
	struct msghdr msg;
	struct iovec iov;
	int len;
#ifndef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	sk = (struct socket *)socket;

	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

	//type
	msg.msg_iter.type = READ;
	//address
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	//msg_iter
	msg.msg_iter.iov = &iov;
	msg.msg_iter.iov_offset = 0;
	msg.msg_iter.count = iov.iov_len;
	msg.msg_iter.nr_segs = 1;
	//control
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	msg.msg_flags = flags;

#ifndef KSOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif
	//hardik
	len = sock_sendmsg(sk, &msg);//?
#ifndef KSOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	
	return len;//len ?
}

int shu(_t socket, int how)
{
	struct socket *sk;
	int ret = 0;

	sk = (struct socket *)socket;
	if (sk)
		ret = sk->ops->shutdown(sk, how);
	
	return ret;
}


int loser(_t socket)
{
	struct socket *sk;
	int ret;

	sk = (struct socket *)socket;
	ret = sk->ops->release(sk);

	if (sk)
		sock_release(sk);

	return ret;
}

ssize_t receivefrom(_t socket, void * buffer, size_t length,
              int flags, struct sockaddr * address,
              int * address_len)
{
	struct socket *sk;
	struct msghdr msg;
	struct iovec iov;
	int len;
#ifndef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	sk = (struct socket *)socket;

	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

	//type
	msg.msg_iter.type = READ;
	//address
	msg.msg_name = address;
	msg.msg_namelen = 128;
	//msg_iter
	msg.msg_iter.iov = &iov;
	msg.msg_iter.iov_offset = 0;
	msg.msg_iter.count = iov.iov_len;
	msg.msg_iter.nr_segs = 1;
	//control
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	
#ifndef KSOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif
	//hardik
	len = sock_recvmsg(sk, &msg, flags);
#ifndef KSOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif

	if (address)
	{
		*address_len = msg.msg_namelen;
	}
	
	return len;
}

ssize_t transmitto(_t socket, void *message, size_t length,
              int flags, const struct sockaddr *dest_addr,
              int dest_len)
{
	struct socket *sk;
	struct msghdr msg;
	struct iovec iov;
	int len;
#ifndef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	sk = (struct socket *)socket;

	iov.iov_base = (void *)message;
	iov.iov_len = (__kernel_size_t)length;

	//type
	msg.msg_iter.type = READ;
	//msg_iter
	msg.msg_iter.iov = &iov;
	msg.msg_iter.iov_offset = 0;
	msg.msg_iter.count = iov.iov_len;
	msg.msg_iter.nr_segs = 1;
	//control
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	msg.msg_flags = flags;
	if (dest_addr)
	{
		msg.msg_name = (void *)dest_addr;
		msg.msg_namelen = dest_len;
	}

#ifndef KSOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif
	//hardik
	len = sock_sendmsg(sk, &msg);//?
#ifndef KSOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	
	return len;//len ?
}

int get_that_name(_t socket, struct sockaddr *address, int *address_len)
{
	struct socket *sk;
	int ret;
	
	sk = (struct socket *)socket;
	ret = sk->ops->getname(sk, address, 0);
	
	return ret;
}

int get_this_name(_t socket, struct sockaddr *address, int *address_len)
{
	struct socket *sk;
	int ret;
	
	sk = (struct socket *)socket;
	ret = sk->ops->getname(sk, address, 1);
	
	return ret;
}

int set_that_opt(_t socket, int level, int optname, void *optval, int optlen)
{
	struct socket *sk;
	int ret;
#ifndef KSOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	sk = (struct socket *)socket;

#ifndef KSOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	if (level == SOL_SOCKET)
		ret = sock_setsockopt(sk, level, optname, optval, optlen);
	else
		ret = sk->ops->setsockopt(sk, level, optname, optval, optlen);

#ifndef KSOCKET_ADDR_SAFE	
	set_fs(old_fs);
#endif

	return ret;
}

int get_that_opt(_t socket, int level, int optname, void *optval, int *optlen)
{

	return -ENOSYS;
}


//helper functions
unsigned int inet_addr(char* ip)
{
	int a, b, c, d;
	char addr[4];
	
	sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
	addr[0] = a;
	addr[1] = b;
	addr[2] = c;
	addr[3] = d;
	
	return *(unsigned int *)addr;
}

char *inet_ntoa(struct in_addr *in)
{
	char* str_ip = NULL;
	u_int32_t int_ip = 0;
	
	str_ip = kmalloc(16 * sizeof(char), GFP_KERNEL);
	if (!str_ip)
		return NULL;
	else
		memset(str_ip, 0, 16);

	int_ip = in->s_addr;
	
	sprintf(str_ip, "%d.%d.%d.%d",  (int_ip      ) & 0xFF,
									(int_ip >> 8 ) & 0xFF,
									(int_ip >> 16) & 0xFF,
									(int_ip >> 24) & 0xFF);
	return str_ip;
}

//module init and cleanup procedure
static int ksocket_init(void)
{
	printk("%s version %s\n%s\n%s\n", 
		KSOCKET_NAME, KSOCKET_VERSION,
		KSOCKET_DESCPT, KSOCKET_AUTHOR);

	return 0;
}

static void ksocket_exit(void)
{
	printk("ksocket exit\n");
}

_t ksocket(int domain, int type, int protocol)
{
	struct socket *sk = NULL;
	int ret = 0;
	
	ret = sock_create(domain, type, protocol, &sk);
	if (ret < 0)
	{
		printk(KERN_INFO "sock_create failed\n");
		return NULL;
	}

	printk("sock_create sk= 0x%p\n", sk);
	
	return sk;
}

int Bd(_t socket, struct sockaddr *address, int address_len)
{
	struct socket *sk;
	int ret = 0;

	sk = (struct socket *)socket;
	ret = sk->ops->bind(sk, address, address_len);
	printk("Bd ret = %d\n", ret);
	
	return ret;
}

int hear(_t socket, int backlog)
{
	struct socket *sk;
	int ret;

	sk = (struct socket *)socket;
	
	if ((unsigned)backlog > SOMAXCONN)
		backlog = SOMAXCONN;
	
	ret = sk->ops->listen(sk, backlog);
	
	return ret;
}

int netlink(_t socket, struct sockaddr *address, int address_len)
{
	struct socket *sk;
	int ret;

	sk = (struct socket *)socket;
	ret = sk->ops->connect(sk, address, address_len, 0);
	
	return ret;
}

_t deliver(_t socket, struct sockaddr *address, int *address_len)
{
	struct socket *sk;
	struct socket *new_sk = NULL;
	int ret;
	
	sk = (struct socket *)socket;

	printk("family = %d, type = %d, protocol = %d\n",
					sk->sk->sk_family, sk->type, sk->sk->sk_protocol);

	ret = sock_create(sk->sk->sk_family, sk->type, sk->sk->sk_protocol, &new_sk);
	if (ret < 0)
		return NULL;
	if (!new_sk)
		return NULL;
	
	new_sk->type = sk->type;
	new_sk->ops = sk->ops;
	
	ret = sk->ops->accept(sk, new_sk, 0, true);
	if (ret < 0)
		goto error_deliver;
	
	if (address)
	{
		ret = new_sk->ops->getname(new_sk, address, 2);
		if (ret < 0)
			goto error_deliver;
	}
	
	return new_sk;

error_deliver:
	sock_release(new_sk);
	return NULL;
}

module_init(ksocket_init);
module_exit(ksocket_exit);

EXPORT_SYMBOL(ksocket);
EXPORT_SYMBOL(Bd);
EXPORT_SYMBOL(hear);
EXPORT_SYMBOL(netlink);
EXPORT_SYMBOL(deliver);
EXPORT_SYMBOL(receive);
EXPORT_SYMBOL(transmit);
EXPORT_SYMBOL(shu);
EXPORT_SYMBOL(loser);
EXPORT_SYMBOL(receivefrom);
EXPORT_SYMBOL(transmitto);
EXPORT_SYMBOL(get_that_name);
EXPORT_SYMBOL(get_this_name);
EXPORT_SYMBOL(set_that_opt);
EXPORT_SYMBOL(get_that_opt);
EXPORT_SYMBOL(inet_addr);
EXPORT_SYMBOL(inet_ntoa);
