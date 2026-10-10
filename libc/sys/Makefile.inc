.PATH: ${ARCHDIR}/sys ${.CURDIR}/sys

SRCS+=	eventfd_read.c eventfd_write.c fh.c fpathconf.c futimens.c getdents.c \
	getvfsstat.c ioctl.c issetugid.c kqueue.c login.c mmap.c posix_madvise.c \
	pathconf.c lwp.c minherit.c sbrk.c setexid.c __sigaction_siginfo.c \
	stat.c statvfs1.c statvfs.c syscall.c syscalls.c __sysctl.c utimes.c \
	utrace.c uuidgen.c vadvise.c mkfifo.c

.include "${ARCHDIR}/sys/Makefile.inc"

MAN+=	accept.2 access.2 acct.2 adjtime.2 bind.2 brk.2 chdir.2 \
	chflags.2 chmod.2 chown.2 chroot.2 clock_getcpuclockid2.2 \
	clock_settime.2 clone.2 close.2 \
	connect.2 dup.2 eventfd.2 execve.2 _exit.2 extattr_get_file.2 \
	extattrctl.2 \
	fcntl.2 fdatasync.2 fdiscard.2 fhopen.2 \
	flock.2 fork.2 fsync.2 getcontext.2 getdents.2 \
	getfh.2 getvfsstat.2 getgid.2 getgroups.2 \
	getitimer.2 getlogin.2 getpeername.2 getpgrp.2 getpid.2 \
	getpriority.2 getrandom.2 getrlimit.2 getrusage.2 getsid.2 \
	getsockname.2 getsockopt.2 gettimeofday.2 getuid.2 \
	intro.2 ioctl.2 issetugid.2 \
	kill.2 kqueue.2 ktrace.2 _ksem.2 \
	lfs_bmapv.2 lfs_markv.2 lfs_segclean.2 lfs_segwait.2 \
	link.2 listen.2 lseek.2 \
	_lwp_create.2 _lwp_ctl.2 _lwp_detach.2 _lwp_exit.2 \
	_lwp_self.2 _lwp_setname.2 \
	_lwp_suspend.2 _lwp_wakeup.2 _lwp_wait.2 _lwp_kill.2 \
	_lwp_getname.2 _lwp_getprivate.2 \
	_lwp_park.2 _lwp_unpark.2 _lwp_unpark_all.2 \
	memfd_create.2 mkdir.2 mkfifo.2 mknod.2 \
	madvise.2 mincore.2 minherit.2 mlock.2 mlockall.2 mmap.2 modctl.2 \
	mount.2 \
	mprotect.2 mremap.2 msgctl.2 msgget.2 msgrcv.2 msgsnd.2 msync.2 \
	munmap.2 nanosleep.2 nfssvc.2 ntp_adjtime.2 open.2 pathconf.2 pipe.2 \
	poll.2 posix_fadvise.2 \
	profil.2 ptrace.2 __quotactl.2 \
	rasctl.2 read.2 readlink.2 \
	reboot.2 recv.2 rename.2 revoke.2 rmdir.2 \
	select.2 semctl.2 \
	semget.2 semop.2 send.2 setgroups.2 setpgid.2 setregid.2 \
	setreuid.2 setsid.2 setuid.2 shmat.2 shmctl.2 shmget.2 \
	shutdown.2 sigaction.2 sigaltstack.2 sigpending.2 \
	sigprocmask.2 sigqueue.2 sigstack.2 sigsuspend.2 sigtimedwait.2 \
	socket.2 \
	socketpair.2 stat.2 statvfs.2 swapctl.2 swapon.3 symlink.2 \
	sync.2 sysarch.2 syscall.2 timer_create.2 timer_delete.2 \
	timer_settime.2 timerfd.2 truncate.2 umask.2 undelete.2 \
	unlink.2 utimes.2 utrace.2 uuidgen.2 vfork.2 wait.2 write.2

MLINKS+=_exit.2 _Exit.2
MLINKS+=brk.2 sbrk.2
MLINKS+=clone.2 __clone.2
MLINKS+=dup.2 dup2.2
MLINKS+=dup.2 dup3.2
MLINKS+=chdir.2 fchdir.2
MLINKS+=chflags.2 fchflags.2 chflags.2 lchflags.2
MLINKS+=chmod.2 fchmod.2 chmod.2 lchmod.2
MLINKS+=chown.2 fchown.2 chown.2 lchown.2
MLINKS+=chroot.2 fchroot.2
MLINKS+=clock_settime.2 clock_gettime.2
MLINKS+=clock_settime.2 clock_getres.2
MLINKS+=eventfd.2 eventfd_read.2 \
	eventfd.2 eventfd_write.2
MLINKS+=extattr_get_file.2 extattr_set_file.2 \
	extattr_get_file.2 extattr_delete_file.2 \
	extattr_get_file.2 extattr_list_file.2 \
	extattr_get_file.2 extattr_get_fd.2 \
	extattr_get_file.2 extattr_set_fd.2 \
	extattr_get_file.2 extattr_delete_fd.2 \
	extattr_get_file.2 extattr_list_fd.2 \
	extattr_get_file.2 extattr_get_link.2 \
	extattr_get_file.2 extattr_set_link.2 \
	extattr_get_file.2 extattr_delete_link.2 \
	extattr_get_file.2 extattr_list_link.2
MLINKS+=access.2 faccessat.2
MLINKS+=chmod.2 fchmodat.2
MLINKS+=chown.2 fchownat.2
MLINKS+=execve.2 fexecve.2
MLINKS+=fdiscard.2 posix_fallocate.2 
MLINKS+=fhopen.2 fhstat.2 fhopen.2 fhstatvfs.2 fhopen.2 fhstatvfs1.2
MLINKS+=stat.2 fstatat.2
MLINKS+=fsync.2 fsync_range.2
MLINKS+=getcontext.2 setcontext.2
MLINKS+=getgid.2 getegid.2
MLINKS+=getitimer.2 setitimer.2
MLINKS+=getlogin.2 setlogin.2
MLINKS+=getlogin.2 getlogin_r.2
MLINKS+=getpgrp.2 getpgid.2
MLINKS+=getpid.2 getppid.2
MLINKS+=getpriority.2 setpriority.2
MLINKS+=getrlimit.2 setrlimit.2
MLINKS+=getsockopt.2 getsockopt2.2
MLINKS+=getsockopt.2 setsockopt.2
MLINKS+=gettimeofday.2 settimeofday.2
MLINKS+=getuid.2 geteuid.2
MLINKS+=intro.2 errno.2
MLINKS+=kqueue.2 kevent.2
MLINKS+=kqueue.2 kqueue1.2
MLINKS+=kqueue.2 EV_SET.2
MLINKS+=_ksem.2 _ksem_open.2
MLINKS+=_ksem.2 _ksem_close.2
MLINKS+=_ksem.2 _ksem_init.2
MLINKS+=_ksem.2 _ksem_destroy.2
MLINKS+=_ksem.2 _ksem_getvalue.2
MLINKS+=_ksem.2 _ksem_post.2
MLINKS+=_ksem.2 _ksem_wait.2
MLINKS+=_ksem.2 _ksem_timedwait.2
MLINKS+=_ksem.2 _ksem_trywait.2
MLINKS+=_ksem.2 _ksem_unlink.2
MLINKS+=ktrace.2 fktrace.2
MLINKS+=lseek.2 seek.2
MLINKS+=link.2 linkat.2
MLINKS+=_lwp_suspend.2 _lwp_continue.2
MLINKS+=_lwp_getprivate.2 _lwp_setprivate.2
MLINKS+=madvise.2 posix_madvise.2
MLINKS+=mlock.2 munlock.2
MLINKS+=mlockall.2 munlockall.2
MLINKS+=mkdir.2 mkdirat.2
MLINKS+=mkfifo.2 mkfifoat.2
MLINKS+=mknod.2 mknodat.2
MLINKS+=mount.2 unmount.2
MLINKS+=open.2 openat.2
MLINKS+=ntp_adjtime.2 ntp_gettime.2
MLINKS+=pathconf.2 fpathconf.2
MLINKS+=poll.2 pollts.2 poll.2 ppoll.2
MLINKS+=read.2 readv.2 read.2 pread.2 read.2 preadv.2
MLINKS+=readlink.2 readlinkat.2
MLINKS+=recv.2 recvfrom.2 recv.2 recvmsg.2 recv.2 recvmmsg.2
MLINKS+=rename.2 renameat.2
MLINKS+=select.2 pselect.2
MLINKS+=send.2 sendmsg.2 send.2 sendto.2 send.2 sendmmsg.2
MLINKS+=setpgid.2 setpgrp.2
MLINKS+=setuid.2 setegid.2 setuid.2 seteuid.2 setuid.2 setgid.2
MLINKS+=shmat.2 shmdt.2
MLINKS+=symlink.2 symlinkat.2
MLINKS+=timer_settime.2 timer_gettime.2 timer_settime.2 timer_getoverrun.2
MLINKS+=semop.2 semtimedop.2
MLINKS+=sigqueue.2 sigqueueinfo.2
MLINKS+=sigtimedwait.2 sigwaitinfo.2
MLINKS+=sigtimedwait.2 sigwait.2
MLINKS+=stat.2 fstat.2 stat.2 lstat.2
MLINKS+=statvfs.2 fstatvfs.2
MLINKS+=statvfs.2 statvfs1.2
MLINKS+=statvfs.2 fstatvfs1.2
MLINKS+=syscall.2 __syscall.2
MLINKS+=timerfd.2 timerfd_create.2 \
	timerfd.2 timerfd_gettime.2 \
	timerfd.2 timerfd_settime.2
MLINKS+=truncate.2 ftruncate.2
MLINKS+=unlink.2 unlinkat.2
MLINKS+=utimes.2 futimes.2 utimes.2 lutimes.2
MLINKS+=utimes.2 utimens.2 utimes.2 futimens.2 utimes.2 lutimens.2
MLINKS+=utimes.2 utimensat.2
MLINKS+=wait.2 wait3.2 wait.2 wait4.2 wait.2 waitpid.2
MLINKS+=wait.2 wait6.2 wait.2 waitid.2
MLINKS+=write.2 writev.2 write.2 pwrite.2 write.2 pwritev.2
MLINKS+=pipe.2 pipe2.2
MLINKS+=accept.2 paccept.2 accept.2 accept4.2
MLINKS+=nanosleep.2 clock_nanosleep.2
MLINKS+=clock_getcpuclockid2.2 clock_getcpuclockid.2
