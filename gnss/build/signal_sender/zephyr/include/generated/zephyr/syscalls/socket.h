/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_SOCKET_H
#define Z_INCLUDE_SYSCALLS_SOCKET_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <zephyr/syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern void * z_impl_zsock_get_context_object(int sock);

__pinned_func
static inline void * zsock_get_context_object(int sock)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		return (void *) arch_syscall_invoke1(parm0.x, K_SYSCALL_ZSOCK_GET_CONTEXT_OBJECT);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_get_context_object(sock);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_get_context_object(sock) ({ 	void * syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_GET_CONTEXT_OBJECT, zsock_get_context_object, sock); 	syscall__retval = zsock_get_context_object(sock); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_GET_CONTEXT_OBJECT, zsock_get_context_object, sock, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_socket(int family, int type, int proto);

__pinned_func
static inline int zsock_socket(int family, int type, int proto)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = family };
		union { uintptr_t x; int val; } parm1 = { .val = type };
		union { uintptr_t x; int val; } parm2 = { .val = proto };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_SOCKET);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_socket(family, type, proto);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_socket(family, type, proto) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_SOCKET, zsock_socket, family, type, proto); 	syscall__retval = zsock_socket(family, type, proto); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_SOCKET, zsock_socket, family, type, proto, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_socketpair(int family, int type, int proto, int * sv);

__pinned_func
static inline int zsock_socketpair(int family, int type, int proto, int * sv)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = family };
		union { uintptr_t x; int val; } parm1 = { .val = type };
		union { uintptr_t x; int val; } parm2 = { .val = proto };
		union { uintptr_t x; int * val; } parm3 = { .val = sv };
		return (int) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_ZSOCK_SOCKETPAIR);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_socketpair(family, type, proto, sv);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_socketpair(family, type, proto, sv) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_SOCKETPAIR, zsock_socketpair, family, type, proto, sv); 	syscall__retval = zsock_socketpair(family, type, proto, sv); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_SOCKETPAIR, zsock_socketpair, family, type, proto, sv, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_close(int sock);

__pinned_func
static inline int zsock_close(int sock)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		return (int) arch_syscall_invoke1(parm0.x, K_SYSCALL_ZSOCK_CLOSE);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_close(sock);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_close(sock) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_CLOSE, zsock_close, sock); 	syscall__retval = zsock_close(sock); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_CLOSE, zsock_close, sock, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_shutdown(int sock, int how);

__pinned_func
static inline int zsock_shutdown(int sock, int how)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; int val; } parm1 = { .val = how };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_ZSOCK_SHUTDOWN);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_shutdown(sock, how);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_shutdown(sock, how) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_SHUTDOWN, zsock_shutdown, sock, how); 	syscall__retval = zsock_shutdown(sock, how); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_SHUTDOWN, zsock_shutdown, sock, how, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_bind(int sock, const struct sockaddr * addr, socklen_t addrlen);

__pinned_func
static inline int zsock_bind(int sock, const struct sockaddr * addr, socklen_t addrlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; const struct sockaddr * val; } parm1 = { .val = addr };
		union { uintptr_t x; socklen_t val; } parm2 = { .val = addrlen };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_BIND);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_bind(sock, addr, addrlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_bind(sock, addr, addrlen) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_BIND, zsock_bind, sock, addr, addrlen); 	syscall__retval = zsock_bind(sock, addr, addrlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_BIND, zsock_bind, sock, addr, addrlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_connect(int sock, const struct sockaddr * addr, socklen_t addrlen);

__pinned_func
static inline int zsock_connect(int sock, const struct sockaddr * addr, socklen_t addrlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; const struct sockaddr * val; } parm1 = { .val = addr };
		union { uintptr_t x; socklen_t val; } parm2 = { .val = addrlen };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_CONNECT);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_connect(sock, addr, addrlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_connect(sock, addr, addrlen) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_CONNECT, zsock_connect, sock, addr, addrlen); 	syscall__retval = zsock_connect(sock, addr, addrlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_CONNECT, zsock_connect, sock, addr, addrlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_listen(int sock, int backlog);

__pinned_func
static inline int zsock_listen(int sock, int backlog)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; int val; } parm1 = { .val = backlog };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_ZSOCK_LISTEN);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_listen(sock, backlog);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_listen(sock, backlog) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_LISTEN, zsock_listen, sock, backlog); 	syscall__retval = zsock_listen(sock, backlog); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_LISTEN, zsock_listen, sock, backlog, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_accept(int sock, struct sockaddr * addr, socklen_t * addrlen);

__pinned_func
static inline int zsock_accept(int sock, struct sockaddr * addr, socklen_t * addrlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; struct sockaddr * val; } parm1 = { .val = addr };
		union { uintptr_t x; socklen_t * val; } parm2 = { .val = addrlen };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_ACCEPT);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_accept(sock, addr, addrlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_accept(sock, addr, addrlen) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_ACCEPT, zsock_accept, sock, addr, addrlen); 	syscall__retval = zsock_accept(sock, addr, addrlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_ACCEPT, zsock_accept, sock, addr, addrlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern ssize_t z_impl_zsock_sendto(int sock, const void * buf, size_t len, int flags, const struct sockaddr * dest_addr, socklen_t addrlen);

__pinned_func
static inline ssize_t zsock_sendto(int sock, const void * buf, size_t len, int flags, const struct sockaddr * dest_addr, socklen_t addrlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; const void * val; } parm1 = { .val = buf };
		union { uintptr_t x; size_t val; } parm2 = { .val = len };
		union { uintptr_t x; int val; } parm3 = { .val = flags };
		union { uintptr_t x; const struct sockaddr * val; } parm4 = { .val = dest_addr };
		union { uintptr_t x; socklen_t val; } parm5 = { .val = addrlen };
		return (ssize_t) arch_syscall_invoke6(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, parm5.x, K_SYSCALL_ZSOCK_SENDTO);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_sendto(sock, buf, len, flags, dest_addr, addrlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_sendto(sock, buf, len, flags, dest_addr, addrlen) ({ 	ssize_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_SENDTO, zsock_sendto, sock, buf, len, flags, dest_addr, addrlen); 	syscall__retval = zsock_sendto(sock, buf, len, flags, dest_addr, addrlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_SENDTO, zsock_sendto, sock, buf, len, flags, dest_addr, addrlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern ssize_t z_impl_zsock_sendmsg(int sock, const struct msghdr * msg, int flags);

__pinned_func
static inline ssize_t zsock_sendmsg(int sock, const struct msghdr * msg, int flags)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; const struct msghdr * val; } parm1 = { .val = msg };
		union { uintptr_t x; int val; } parm2 = { .val = flags };
		return (ssize_t) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_SENDMSG);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_sendmsg(sock, msg, flags);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_sendmsg(sock, msg, flags) ({ 	ssize_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_SENDMSG, zsock_sendmsg, sock, msg, flags); 	syscall__retval = zsock_sendmsg(sock, msg, flags); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_SENDMSG, zsock_sendmsg, sock, msg, flags, syscall__retval); 	syscall__retval; })
#endif
#endif


extern ssize_t z_impl_zsock_recvfrom(int sock, void * buf, size_t max_len, int flags, struct sockaddr * src_addr, socklen_t * addrlen);

__pinned_func
static inline ssize_t zsock_recvfrom(int sock, void * buf, size_t max_len, int flags, struct sockaddr * src_addr, socklen_t * addrlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; void * val; } parm1 = { .val = buf };
		union { uintptr_t x; size_t val; } parm2 = { .val = max_len };
		union { uintptr_t x; int val; } parm3 = { .val = flags };
		union { uintptr_t x; struct sockaddr * val; } parm4 = { .val = src_addr };
		union { uintptr_t x; socklen_t * val; } parm5 = { .val = addrlen };
		return (ssize_t) arch_syscall_invoke6(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, parm5.x, K_SYSCALL_ZSOCK_RECVFROM);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_recvfrom(sock, buf, max_len, flags, src_addr, addrlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_recvfrom(sock, buf, max_len, flags, src_addr, addrlen) ({ 	ssize_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_RECVFROM, zsock_recvfrom, sock, buf, max_len, flags, src_addr, addrlen); 	syscall__retval = zsock_recvfrom(sock, buf, max_len, flags, src_addr, addrlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_RECVFROM, zsock_recvfrom, sock, buf, max_len, flags, src_addr, addrlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern ssize_t z_impl_zsock_recvmsg(int sock, struct msghdr * msg, int flags);

__pinned_func
static inline ssize_t zsock_recvmsg(int sock, struct msghdr * msg, int flags)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; struct msghdr * val; } parm1 = { .val = msg };
		union { uintptr_t x; int val; } parm2 = { .val = flags };
		return (ssize_t) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_RECVMSG);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_recvmsg(sock, msg, flags);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_recvmsg(sock, msg, flags) ({ 	ssize_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_RECVMSG, zsock_recvmsg, sock, msg, flags); 	syscall__retval = zsock_recvmsg(sock, msg, flags); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_RECVMSG, zsock_recvmsg, sock, msg, flags, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_fcntl_impl(int sock, int cmd, int flags);

__pinned_func
static inline int zsock_fcntl_impl(int sock, int cmd, int flags)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; int val; } parm1 = { .val = cmd };
		union { uintptr_t x; int val; } parm2 = { .val = flags };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_FCNTL_IMPL);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_fcntl_impl(sock, cmd, flags);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_fcntl_impl(sock, cmd, flags) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_FCNTL_IMPL, zsock_fcntl_impl, sock, cmd, flags); 	syscall__retval = zsock_fcntl_impl(sock, cmd, flags); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_FCNTL_IMPL, zsock_fcntl_impl, sock, cmd, flags, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_ioctl_impl(int sock, unsigned long request, va_list ap);

__pinned_func
static inline int zsock_ioctl_impl(int sock, unsigned long request, va_list ap)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; unsigned long val; } parm1 = { .val = request };
		union { uintptr_t x; va_list val; } parm2;
		va_copy(parm2.val, ap);
		int invoke__retval = arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_IOCTL_IMPL);
		va_end(parm2.val);
		return invoke__retval;
	}
#endif
	compiler_barrier();
	return z_impl_zsock_ioctl_impl(sock, request, ap);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_ioctl_impl(sock, request, ap) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_IOCTL_IMPL, zsock_ioctl_impl, sock, request, ap); 	syscall__retval = zsock_ioctl_impl(sock, request, ap); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_IOCTL_IMPL, zsock_ioctl_impl, sock, request, ap, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_poll(struct zsock_pollfd * fds, int nfds, int timeout);

__pinned_func
static inline int zsock_poll(struct zsock_pollfd * fds, int nfds, int timeout)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; struct zsock_pollfd * val; } parm0 = { .val = fds };
		union { uintptr_t x; int val; } parm1 = { .val = nfds };
		union { uintptr_t x; int val; } parm2 = { .val = timeout };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_POLL);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_poll(fds, nfds, timeout);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_poll(fds, nfds, timeout) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_POLL, zsock_poll, fds, nfds, timeout); 	syscall__retval = zsock_poll(fds, nfds, timeout); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_POLL, zsock_poll, fds, nfds, timeout, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_getsockopt(int sock, int level, int optname, void * optval, socklen_t * optlen);

__pinned_func
static inline int zsock_getsockopt(int sock, int level, int optname, void * optval, socklen_t * optlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; int val; } parm1 = { .val = level };
		union { uintptr_t x; int val; } parm2 = { .val = optname };
		union { uintptr_t x; void * val; } parm3 = { .val = optval };
		union { uintptr_t x; socklen_t * val; } parm4 = { .val = optlen };
		return (int) arch_syscall_invoke5(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, K_SYSCALL_ZSOCK_GETSOCKOPT);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_getsockopt(sock, level, optname, optval, optlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_getsockopt(sock, level, optname, optval, optlen) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_GETSOCKOPT, zsock_getsockopt, sock, level, optname, optval, optlen); 	syscall__retval = zsock_getsockopt(sock, level, optname, optval, optlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_GETSOCKOPT, zsock_getsockopt, sock, level, optname, optval, optlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_setsockopt(int sock, int level, int optname, const void * optval, socklen_t optlen);

__pinned_func
static inline int zsock_setsockopt(int sock, int level, int optname, const void * optval, socklen_t optlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; int val; } parm1 = { .val = level };
		union { uintptr_t x; int val; } parm2 = { .val = optname };
		union { uintptr_t x; const void * val; } parm3 = { .val = optval };
		union { uintptr_t x; socklen_t val; } parm4 = { .val = optlen };
		return (int) arch_syscall_invoke5(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, K_SYSCALL_ZSOCK_SETSOCKOPT);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_setsockopt(sock, level, optname, optval, optlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_setsockopt(sock, level, optname, optval, optlen) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_SETSOCKOPT, zsock_setsockopt, sock, level, optname, optval, optlen); 	syscall__retval = zsock_setsockopt(sock, level, optname, optval, optlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_SETSOCKOPT, zsock_setsockopt, sock, level, optname, optval, optlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_getpeername(int sock, struct sockaddr * addr, socklen_t * addrlen);

__pinned_func
static inline int zsock_getpeername(int sock, struct sockaddr * addr, socklen_t * addrlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; struct sockaddr * val; } parm1 = { .val = addr };
		union { uintptr_t x; socklen_t * val; } parm2 = { .val = addrlen };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_GETPEERNAME);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_getpeername(sock, addr, addrlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_getpeername(sock, addr, addrlen) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_GETPEERNAME, zsock_getpeername, sock, addr, addrlen); 	syscall__retval = zsock_getpeername(sock, addr, addrlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_GETPEERNAME, zsock_getpeername, sock, addr, addrlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_getsockname(int sock, struct sockaddr * addr, socklen_t * addrlen);

__pinned_func
static inline int zsock_getsockname(int sock, struct sockaddr * addr, socklen_t * addrlen)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = sock };
		union { uintptr_t x; struct sockaddr * val; } parm1 = { .val = addr };
		union { uintptr_t x; socklen_t * val; } parm2 = { .val = addrlen };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_GETSOCKNAME);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_getsockname(sock, addr, addrlen);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_getsockname(sock, addr, addrlen) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_GETSOCKNAME, zsock_getsockname, sock, addr, addrlen); 	syscall__retval = zsock_getsockname(sock, addr, addrlen); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_GETSOCKNAME, zsock_getsockname, sock, addr, addrlen, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_gethostname(char * buf, size_t len);

__pinned_func
static inline int zsock_gethostname(char * buf, size_t len)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; char * val; } parm0 = { .val = buf };
		union { uintptr_t x; size_t val; } parm1 = { .val = len };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_ZSOCK_GETHOSTNAME);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_gethostname(buf, len);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_gethostname(buf, len) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_GETHOSTNAME, zsock_gethostname, buf, len); 	syscall__retval = zsock_gethostname(buf, len); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_GETHOSTNAME, zsock_gethostname, buf, len, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_zsock_inet_pton(sa_family_t family, const char * src, void * dst);

__pinned_func
static inline int zsock_inet_pton(sa_family_t family, const char * src, void * dst)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; sa_family_t val; } parm0 = { .val = family };
		union { uintptr_t x; const char * val; } parm1 = { .val = src };
		union { uintptr_t x; void * val; } parm2 = { .val = dst };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ZSOCK_INET_PTON);
	}
#endif
	compiler_barrier();
	return z_impl_zsock_inet_pton(family, src, dst);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define zsock_inet_pton(family, src, dst) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ZSOCK_INET_PTON, zsock_inet_pton, family, src, dst); 	syscall__retval = zsock_inet_pton(family, src, dst); 	sys_port_trace_syscall_exit(K_SYSCALL_ZSOCK_INET_PTON, zsock_inet_pton, family, src, dst, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_z_zsock_getaddrinfo_internal(const char * host, const char * service, const struct zsock_addrinfo * hints, struct zsock_addrinfo * res);

__pinned_func
static inline int z_zsock_getaddrinfo_internal(const char * host, const char * service, const struct zsock_addrinfo * hints, struct zsock_addrinfo * res)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const char * val; } parm0 = { .val = host };
		union { uintptr_t x; const char * val; } parm1 = { .val = service };
		union { uintptr_t x; const struct zsock_addrinfo * val; } parm2 = { .val = hints };
		union { uintptr_t x; struct zsock_addrinfo * val; } parm3 = { .val = res };
		return (int) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_Z_ZSOCK_GETADDRINFO_INTERNAL);
	}
#endif
	compiler_barrier();
	return z_impl_z_zsock_getaddrinfo_internal(host, service, hints, res);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define z_zsock_getaddrinfo_internal(host, service, hints, res) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_Z_ZSOCK_GETADDRINFO_INTERNAL, z_zsock_getaddrinfo_internal, host, service, hints, res); 	syscall__retval = z_zsock_getaddrinfo_internal(host, service, hints, res); 	sys_port_trace_syscall_exit(K_SYSCALL_Z_ZSOCK_GETADDRINFO_INTERNAL, z_zsock_getaddrinfo_internal, host, service, hints, res, syscall__retval); 	syscall__retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
