/*    $nlibc: syscalls.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

/*
 * Copyright (c) 2026, littlefly365
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$nlibc: syscalls.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <machine/vmparam.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/sem.h>
#include <sys/uio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include "reentrant.h"

#ifdef __weak_alias
__weak_alias(read, _read);
__weak_alias(lseek, _lseek);
__weak_alias(brk, _brk);
__weak_alias(sigprocmask, __sigprocmask14);
__weak_alias(pread, _pread);
__weak_alias(pwrite, _pwrite);
__weak_alias(pipe, _pipe);
__weak_alias(vfork, __vfork14);
__weak_alias(kill, _kill);
__weak_alias(ftruncate, _ftruncate);
__weak_alias(readlink, _readlink);
#endif

/*
 * The functions below are wrappers for the linux syscalls,
 * they're in numeric order (see sys/syscall.h), if the 
 * wrapper is complex, you'd use a separate file. 
 * (Make sure the symbol is the expected by NetBSD)
*/

/*
 * FIXME: setuid, setgid, setgroups (threads)
*/

ssize_t
_read(int fd, void *buf, size_t len)
{
	return syscall(SYS_read, fd, buf, len);
}

ssize_t
write(int fd, const void *buf, size_t len)
{
	return syscall(SYS_write, fd, buf, len);
}

int
open(const char *path, int flags, ...)
{
	va_list ap;
	mode_t mode;

	if (flags & O_CREAT)
	{
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);

		return syscall(SYS_open, path, flags, mode);
	}

	return syscall(SYS_open, path, flags);
}

int
close(int fd)
{
	return syscall(SYS_close, fd);
}

/*
 * syscall: 4
 * SYS_stat (won't be implemented)
*/

/*
 * syscall: 5
 * SYS_fstat (won't be implemented)
*/

/*
 * syscall: 6
 * SYS_lstat (won't be implemented)
*/

int
poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
	return syscall(SYS_poll, fds, nfds, timeout);
}

off_t
_lseek(int fd, off_t offset, int whence)
{
	return syscall(SYS_lseek, fd, offset, whence);
}

/*
 * syscall: 9
 * SYS_mmap (mmap.c)
*/

int
mprotect(void *addr, size_t len, int prot)
{
	size_t start, end;

	start = (size_t)addr & -PAGE_SIZE;
	end = (size_t)((char *)addr + len + PAGE_SIZE-1) & -PAGE_SIZE;

	return syscall(SYS_mprotect, start, end - start, prot);
}

int
munmap(void *start, size_t len)
{
	return syscall(SYS_munmap, start, len);
}

extern char _end;

char *__minbrk = &_end;

int
_brk(void *addr)
{
	if ((char*)addr < __minbrk)
		addr = __minbrk;

	if ((char*)syscall(SYS_brk, addr) < (char*)addr) {
		errno = ENOMEM;
		return -1;
	}

	return 0;
}

/*
 * syscall: 13
 * SYS_rt_sigaction (__sigaction_siginfo.c)
*/

int
__sigprocmask14(int how, const sigset_t *set, sigset_t *oset)
{
	if (set && how - SIG_BLOCK > 2) {
		errno = EINVAL;
		return -1;
	}

	if (syscall(SYS_rt_sigprocmask, how, set, oset, 8) != 0)
		return -1;

	if (oset != NULL)
		oset->__bits[0] &= ~0x380000000ULL;

	return 0;
}

/*
 * syscall: 16
 * SYS_ioctl (ioctl.c)
*/

ssize_t
_pread(int fd, void *buf, size_t count, off_t offs)
{
	return syscall(SYS_pread64, fd, buf, count, offs);
}

ssize_t
_pwrite(int fd, const void *buf, size_t count, off_t offs)
{
	return syscall(SYS_pwrite64, fd, buf, count, offs);
}

ssize_t
readv(int fd, const struct iovec *buf, int count)
{
	return syscall(SYS_readv, fd, buf, count);
}

ssize_t
writev(int fd, const struct iovec *buf, int count)
{
	return syscall(SYS_writev, fd, buf, count);
}

int
access(const char *path, int mode)
{
	return syscall(SYS_access, path, mode);
}

int
_pipe(int fd[2])
{
	return syscall(SYS_pipe, fd);
}

int
__select50(int n, fd_set *restrict rfds, fd_set *restrict wfds, fd_set *restrict efds, 
	struct timeval *restrict tv)
{
	return syscall(SYS_select, n, rfds, wfds, efds, (tv) ? ((long[]){tv->tv_sec, tv->tv_usec}) : 0);
}

int
_sys_sched_yield(void)
{
	return syscall(SYS_sched_yield);
}

void
*mremap(void *oldp, size_t oldsize, void *newp, size_t newsize, int flags)
{
	if (newsize >= PTRDIFF_MAX) {
		errno = ENOMEM;
		return (void*)-1;
	}

	return (void*)syscall(SYS_mremap, oldp, oldsize, newsize, flags, newp);
}

int
__msync13(void *start, size_t len, int flags)
{
	return syscall(SYS_msync, start, len, flags);
}

int
mincore(void *addr, size_t len, char *vec)
{
	return syscall(SYS_mincore, addr, len, vec);
}

int
madvise(void *addr, size_t len, int advice)
{
	return syscall(SYS_madvise, addr, len, advice);
}

/*
 * syscall: 29
 * SYS_shmget (not implemented)
*/

/*
 * syscall: 30
 * SYS_shmat (not implemented)
*/

/*
 * syscall: 31
 * SYS_shmctl (not implemented)
*/

int
dup(int fd)
{
	return syscall(SYS_dup, fd);
}

int
dup2(int oldfd, int newfd)
{
	int ret;
	while ((ret = syscall(SYS_dup2, oldfd, newfd)) == -1 && errno == EBUSY);
	return ret;
}

/*
 * syscall: 34
 * SYS_pause (NetBSD has a replacement using sigsuspend)
*/

int
__nanosleep50(const struct timespec *req, struct timespec *rem)
{
	return syscall(SYS_nanosleep, req, rem);
}

int
__getitimer50(int which, struct itimerval *old)
{
	return syscall(SYS_getitimer, which, old);
}

/*
 * syscall: 37
 * SYS_alarm (NetBSD has a replacement using setitimer)
*/

int
__setitimer50(int which, const struct itimerval *restrict new, struct itimerval *restrict old)
{
	return syscall(SYS_setitimer, which, new, old);
}

pid_t
getpid(void)
{
	return syscall(SYS_getpid);
}

/*
 * syscall: 40
 * SYS_sendfile (not implemented)
*/

int
__socket30(int domain, int type, int prot)
{
	return syscall(SYS_socket, domain, type, prot);
}

int
connect(int socket, const struct sockaddr *addr, socklen_t len)
{
	return syscall(SYS_connect, socket, addr, len);
}

int
accept(int socket, struct sockaddr *restrict addr, socklen_t *restrict len)
{
	return syscall(SYS_connect, socket, addr, len);
}

ssize_t
sendto(int socket, const void *msg, size_t len, int flags, const struct sockaddr *daddr, socklen_t dlen)
{
	return syscall(SYS_sendto, socket, msg, len, flags, daddr, dlen);
}

ssize_t
recvfrom(int s, void *restrict buf, size_t len, int flags, struct sockaddr *restrict from, 
	socklen_t *restrict fromlen)
{
	return syscall(SYS_recvfrom, s, buf, len, flags, from, fromlen);
}

ssize_t
sendmsg(int socket, const struct msghdr *msg, int flags)
{
	return syscall(SYS_sendmsg, socket, msg, flags);
}

/*
 * syscall: 47
 * SYS_recvmsg (not implemented)
*/

int
shutdown(int sockfd, int how)
{
	return syscall(SYS_shutdown, sockfd, how);
}

int
bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	return syscall(SYS_bind, sockfd, addr, addrlen);
}

int
listen(int sockfd, int backlog)
{
	return syscall(SYS_listen, sockfd, backlog);
}

int
getsockname(int fd, struct sockaddr *restrict addr, socklen_t *restrict len)
{
	return syscall(SYS_getsockname, fd, addr, len);
}

int
getpeername(int fd, struct sockaddr *restrict addr, socklen_t *restrict len)
{
	return syscall(SYS_getpeername, fd, addr, len);
}

int
socketpair(int domain, int type, int prot, int sv[2])
{
	return syscall(SYS_socketpair, domain, type, prot, sv);
}

int
setsockopt(int fd, int level, int optname, const void *optval, socklen_t len)
{
	return syscall(SYS_getsockopt, fd, level, optname, optval, len);
}

int
getsockopt(int fd, int level, int optname, void *optval, socklen_t *len)
{
	return syscall(SYS_getsockopt, fd, level, optname, optval, len);
}

/*
 * syscall: 56
 * (not implemented)
*/

#ifdef __strong_alias
__strong_alias(__vfork14, __fork);
#endif
pid_t
__fork(void)
{
	/* 
	 * It's easier and more secure to use fork
	 * instead of vfork
	*/

	return syscall(SYS_fork);
}

int
execve(const char *path, char *const argv[], char *const envp[])
{
	return syscall(SYS_execve, path, argv, envp);
}

#ifdef __strong_alias
__strong_alias(_Exit, _exit);
#endif
void __dead
_exit(int code)
{
	(void)syscall(SYS_exit, code);
	__builtin_unreachable();
}

pid_t
__wait450(pid_t pid, int *wstatus, int options, struct rusage *rusage)
{
	return syscall(SYS_wait4, pid, wstatus, options, rusage);
}

int
_kill(pid_t pid, int nsig)
{
	return syscall(SYS_kill, pid, nsig);
}

/*
 * syscall: 63
 * SYS_uname (NetBSD has a replacement using sysctl)
*/

int
_semget(key_t key, int nsems, int semflg)
{
	return syscall(SYS_semget, key, nsems, semflg);
}

int
_semop(int id, struct sembuf *buf, size_t n)
{
	return syscall(SYS_semop, id, buf, n);
}

/*
 * syscall: 66
 * SYS_semctl (not implemented)
*/

/*
 * syscall: 67
 * SYS_shmdt (not implemented)
*/

/*
 * syscall: 68
 * SYS_msgget (not implemented)
*/

/*
 * syscall: 69
 * SYS_msgsnd (not implemented)
*/

/*
 * syscall: 70
 * SYS_msgrcv (not implemented)
*/

/*
 * syscall: 71
 * SYS_msgctl (not implemented)
*/

int
fcntl(int fd, int cmd, ...)
{
	u_long arg;
	va_list ap;

	va_start(ap, cmd);
	arg = va_arg(ap, unsigned long);
	va_end(ap);

	return syscall(SYS_fcntl, fd, cmd, (void *)arg);
}

int
flock(int fd, int op)
{
	return syscall(SYS_flock, fd, op);
}

int
fsync(int fd)
{
	return syscall(SYS_fsync, fd);
}

int
fdatasync(int fd)
{
	return syscall(SYS_fdatasync, fd);
}

int
truncate(const char *path, off_t offset)
{
	return syscall(SYS_truncate, path, offset);
}

int
_ftruncate(int fd, off_t offset)
{
	return syscall(SYS_ftruncate, fd, offset);
}

/*
 * syscall: 78
 * SYS_getdents (getdents.c)
*/

int
__getcwd(char *buf, size_t len)
{
	return syscall(SYS_getcwd, buf, len);
}

int
chdir(const char *path)
{
	return syscall(SYS_chdir, path);
}

int
fchdir(int fd)
{
	return syscall(SYS_fchdir, fd);
}

int
rename(const char *old, const char *new)
{
	return syscall(SYS_rename, old, new);
}

int
mkdir(const char *path, mode_t mode)
{
	return syscall(SYS_mkdir, path, mode);
}

int
rmdir(const char *path)
{
	return syscall(SYS_rmdir, path);
}

/*
 * syscall: 85
 * SYS_creat (NetBSD has a replacement using open)
*/

int
link(const char *old, const char *new)
{
	return syscall(SYS_link, old, new);
}

int
unlink(const char *path)
{
	return syscall(SYS_unlink, path);
}

int
symlink(const char *old, const char *new)
{
	return syscall(SYS_symlink, old, new);
}

ssize_t
_readlink(const char *path, char *buf, size_t len)
{
	return syscall(SYS_readlink, path, buf, len);
}

int
chmod(const char *path, mode_t mode)
{
	return syscall(SYS_chmod, path, mode);
}

int
fchmod(int fd, mode_t mode)
{
	return syscall(SYS_fchmod, fd, mode);
}

int
chown(const char *path, uid_t owner, gid_t group)
{
	return syscall(SYS_chown, path, owner, group);
}

int
fchown(int fd, uid_t owner, gid_t group)
{
	return syscall(SYS_chown, fd, owner, group);
}

int
lchown(const char *path, uid_t owner, gid_t group)
{
	return syscall(SYS_lchown, path, owner, group);
}

mode_t
umask(mode_t umask)
{
	return syscall(SYS_umask, umask);
}

int
__gettimeofday50(struct timeval *restrict tv, void *restrict tz)
{
	return syscall(SYS_gettimeofday, tv, tz);
}

int
getrlimit(int resource, struct rlimit *rlim)
{
	return syscall(SYS_prlimit64, 0, resource, 0, rlim);
}

int
__getrusage50(int who, struct rusage *ru)
{
	return syscall(SYS_getrusage, who, ru);
}

/*
 * syscalls: 99-101
 * (not implemented)
*/

uid_t
getuid(void)
{
	return syscall(SYS_getuid);
}

/*
 * syscall: 103
 * (not implemented)
*/

gid_t
getgid(void)
{
	return syscall(SYS_getgid);
}

int
setuid(uid_t uid)
{
	if (!__isthreaded)
		return syscall(SYS_setuid, uid);
	return syscall(SYS_setuid, uid);
}

int
setgid(gid_t gid)
{
	if (!__isthreaded)
		return syscall(SYS_setgid, gid);
	return syscall(SYS_setgid, gid);
}

uid_t
geteuid(void)
{
	return syscall(SYS_geteuid);
}

gid_t
getegid(void)
{
	return syscall(SYS_getegid);
}

int
setpgid(pid_t pid, pid_t pgid)
{
	return syscall(SYS_setpgid, pid, pgid);
}

pid_t
getppid(void)
{
	return syscall(SYS_getppid);
}

pid_t
_getpgrp(void)
{
	return syscall(SYS_getpgrp);
}

pid_t
setsid(void)
{
	return syscall(SYS_setsid);
}

int
setreuid(uid_t ruid, uid_t euid)
{
	return syscall(SYS_setreuid, ruid, euid);
}

int
setregid(gid_t rgid, gid_t egid)
{
	return syscall(SYS_setregid, rgid, egid);
}

int
getgroups(int gidsetlen, gid_t *gidset)
{
	return syscall(SYS_getgroups, gidsetlen, gidset);
}

int
setgroups(int n, const gid_t *groups)
{
	if (!__isthreaded)
		return syscall(SYS_setgroups, n, groups);
	return syscall(SYS_setgroups, n, groups);
}

/*
 * syscall: 117-129
 * (not implemented)
*/

int
__sigsuspend14(const sigset_t *mask)
{
	return syscall(SYS_rt_sigsuspend, mask, 8);
}

int
__sigaltstack14(const stack_t *restrict ss, stack_t *restrict old)
{
	return syscall(SYS_sigaltstack, ss, old);
}

/*
 * syscalls: 132
 * (not implemented)
*/

int
__mknod50(const char *path, mode_t mode, dev_t dev)
{
	return syscall(SYS_mknod, path, mode, dev);
}

/*
 * syscalls: 134-139
 * (not implemented)
*/

int
getpriority(int which, id_t who)
{
	int ret;
	if ((ret = syscall(SYS_getpriority, which, who)) < 0)
		return ret;
	return 20-ret;
}

int
setpriority(int which, id_t who, int prio)
{
	return syscall(SYS_setpriority, which, who, prio);
}

/*
 * syscalls: 142-159
 * (not implemented)
*/

int
setrlimit(int resource, const struct rlimit *rlim)
{
        return syscall(SYS_prlimit64, 0, resource, rlim, 0);
}

int
chroot(const char *path)
{
	return syscall(SYS_chroot, path);
}

/*
 * syscalls: 162-163
 * (not implemented)
*/

/*
 * syscalls: 165-226
 * (not implemented)
*/

int
__settimeofday(const struct timeval *restrict tv, const void *restrict tz)
{
	return syscall(SYS_settimeofday, tv, tz);
}

int
__clock_settime50(clockid_t clk, const struct timespec *ts)
{
	return syscall(SYS_clock_settime, clk, ts);
}

int
__clock_gettime50(clockid_t clk, struct timespec *ts)
{
	return syscall(SYS_clock_gettime, clk, ts);
}

int
__clock_getres50(clockid_t clk, struct timespec *ts)
{
	return syscall(SYS_clock_getres, clk, ts);
}

/*
 * syscalls: 230-249
 * (not implemented)
*/

int
utimensat(int fd, const char *path, const struct timespec times[2], int flags)
{
	return syscall(SYS_utimensat, fd, path, times, flags);
}

#ifdef __strong_alias
__strong_alias(__pollts50, ppoll);
#endif
int
ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout, const sigset_t *sigmask)
{
	return syscall(SYS_ppoll, fds, nfds, timeout, sigmask);
}

int
__pselect50(int n, fd_set *restrict rfds, fd_set *restrict wfds, fd_set *restrict efds, const struct timespec *restrict ts, const sigset_t *restrict mask)
{
	long data[2] = { (uintptr_t)mask, 8 };
	return syscall(SYS_select, n, rfds, wfds, efds, (ts) ? ((long[]){ts->tv_sec, ts->tv_nsec}) : 0, data);
}

/*
 * syscalls: 281-291
 * (not implemented)
*/

int
__dup3100(int oldfd, int newfd, int flags)
{
	int ret;
	while ((ret = syscall(SYS_dup3, oldfd, newfd, flags)) == -1 && errno == EBUSY);
	return ret;
}

int
pipe2(int fd[2], int flags)
{
	return syscall(SYS_pipe2, fd, flags);
}

/*
 * syscalls: 294-317
 * (not implemented)
*/

ssize_t
getrandom(void *buf, size_t len, unsigned flags)
{
	return syscall(SYS_getrandom, buf, len, flags);
}

/*
 * syscalls: 319-470
 * (not implemented)
*/
