/*    $nlibc: __sysctl.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: __sysctl.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

/*
 * It's not my plan to implement all sysctl under the Linux Kernel,
 * my plan is to implement the most case of use in the netbsd userland
 * and the third party software.
*/

#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>
#include <sys/random.h>
#include <sys/param.h> /* MACHINE and MACHINE_ARCH */
#include <sys/types.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <machine/vmparam.h>

static struct {
	char sysname[SYS_NMLN];	/* unused */
	char nodename[SYS_NMLN];
	char release[SYS_NMLN];
	char version[SYS_NMLN];
	char machine[SYS_NMLN];	/* unused */
	char domainname[SYS_NMLN];
} uts;

#define _PATH_HOSTID	"/etc/hostid"

static int get_int_from_file(const char *path);
static int get_from_file(const char *path, char *buf, size_t len);
static int copy(const char *src, char *dst, size_t srclen, size_t *dstlen);

int
__sysctl(const int *name, u_int miblen, void *oldp, size_t *oldlenp, const void *newp, size_t newlenp)
{
	if (miblen == 0 || miblen > CTL_MAXNAME) {
		errno = EINVAL;
		return -1;
	}

	/* CTL_KERN set and get information */
	if (name[0] == CTL_KERN) {
			/* Set information */
			if (newp != NULL && newlenp > 0) {
				switch (name[1]) {
				case KERN_HOSTNAME:
					return syscall(SYS_sethostname, newp, newlenp);
				case KERN_DOMAINNAME:
					return syscall(SYS_setdomainname, newp, newlenp);
				case KERN_HOSTID:
					int fd;
					if ((fd = open(_PATH_HOSTID, O_WRONLY | O_CREAT | O_TRUNC)) < 0)
						return -1;
					if (write(fd, newp, newlenp) < 0)
						return -1;
					close(fd);
					return 0;
				default:
					errno = EPERM;
					return -1; /* Cannot modify information like ostype, so EPERM */
				}
			}

			if (name[1] == KERN_OSRELEASE || name[1] == KERN_VERSION || \
				name[1] == KERN_HOSTNAME || name[1] == KERN_DOMAINNAME) {
				if (uts.release[0] == '\0') {
					if (syscall(SYS_uname, &uts) != 0)
						return -1;
				}
			}

			/* Get information */
			switch (name[1]) {
			case KERN_OSTYPE:
				return copy("Linux", oldp, 5, oldlenp);
			case KERN_OSRELEASE:
				return copy(uts.release, oldp, strlen(uts.sysname), oldlenp);
			case KERN_OSREV:
				*(int*)oldp = __NetBSD_Version__;
				break;
			case KERN_VERSION:
				return copy(uts.version, oldp, strlen(uts.version), oldlenp);
			case KERN_MAXVNODES:
				errno = ENOTSUP;
				return -1;
			case KERN_MAXPROC:
					if ((*(int*)oldp = get_int_from_file("/proc/sys/kernel/pid_max")) <= 0)
					return -1;
				break;
			case KERN_MAXFILES:
				if ((*(int*)oldp = get_int_from_file("/proc/sys/fs/file-max")) <= 0)
					return -1;
				break;
			case KERN_ARGMAX:
				*(int*)oldp = ARG_MAX;
				break;
			case KERN_SECURELVL:
				*(int*)oldp = 0;
				break;
			case KERN_HOSTNAME:
				return copy(uts.nodename, oldp, strlen(uts.nodename), oldlenp);
			case KERN_HOSTID:
				int fd;
				if ((fd = open(_PATH_HOSTID, O_RDONLY)) != 0)
					return -1;
				if (read(fd, oldp, *oldlenp) < 0)
					return -1;
				close(fd);
				break;
			case KERN_CLOCKRATE:
			case KERN_VNODE:
			case KERN_PROC:
			case KERN_FILE:
			case KERN_PROF:
				errno = ENOTSUP;
				return -1;
			case KERN_POSIX1:
				*(int*)oldp = _POSIX_VERSION;
				break;
			case KERN_NGROUPS:
				*(int*)oldp = 65536;
				break;
			case KERN_JOB_CONTROL:
			case KERN_SAVED_IDS:
			case KERN_OBOOTTIME:
				errno = ENOTSUP;
				return -1;
			case KERN_DOMAINNAME:
				return copy(uts.domainname, oldp, strlen(uts.domainname), oldlenp);
			case KERN_MAXPARTITIONS:
			case KERN_RAWPARTITION:
			case KERN_NTPTIME:
			case KERN_TIMEX:
			case KERN_AUTONICETIME:
			case KERN_RTC_OFFSET:
			case KERN_ROOT_DEVICE:
			case KERN_MSGBUFSIZE:
			case KERN_FSYNC:
			case KERN_OLDSYSVMSG:
			case KERN_OLDSYSVSHM:
			case KERN_OLDSHORTCORENAME:
				errno = ENOTSUP;
				return -1;
			case KERN_SYNCHRONIZED_IO:
				*(int*)oldp = 1;
				break;
			case KERN_IOV_MAX:
			case KERN_MBUF:
			case KERN_MAPPED_FILES:
			case KERN_MEMLOCK:
			case KERN_MEMLOCK_RANGE:
			case KERN_MEMORY_PROTECTION:
				errno = ENOTSUP;
				return -1;
			case KERN_LOGIN_NAME_MAX:
				*(int*)oldp = LOGIN_NAME_MAX;
				break;
			case KERN_DEFCORENAME:
			case KERN_LOGSIGEXIT:
			case KERN_PROC2:
			case KERN_PROC_ARGS:
			case KERN_FSCALE:
			case KERN_CCPU:
			case KERN_CP_TIME:
			case KERN_OLDSYSVIPC_INFO:
			case KERN_MSGBUF:
			case KERN_CONSDEV:
			case KERN_MAXPTYS:
			case KERN_PIPE:
			case KERN_MAXPHYS:
			case KERN_SBMAX:
			case KERN_TKSTAT:
			case KERN_MONOTONIC_CLOCK:
			case KERN_URND:
			case KERN_LABELSECTOR:
			case KERN_LABELOFFSET:
			case KERN_LWP:
			case KERN_FORKFSLEEP:
			case KERN_POSIX_THREADS:
			case KERN_POSIX_SEMAPHORES:
			case KERN_POSIX_BARRIERS:
			case KERN_POSIX_TIMERS:
			case KERN_POSIX_SPIN_LOCKS:
			case KERN_POSIX_READER_WRITER_LOCKS:
			case KERN_DUMP_ON_PANIC:
			case KERN_SOMAXKVA:
			case KERN_ROOT_PARTITION:
			case KERN_DRIVERS:
			case KERN_BUF:
			case KERN_FILE2:
			case KERN_VERIEXEC:
			case KERN_CP_ID:
			case KERN_HARDCLOCK_TICKS:
				errno = ENOTSUP;
				return -1;
			case KERN_ARND:
				if (getrandom(oldp, *oldlenp, 0) < 0)
					return -1;
				break;
			case KERN_SYSVIPC:
			case KERN_BOOTTIME:
			case KERN_EVCNT:
			case KERN_SOFIXEDBUF:
			case KERN_ENTROPY:
				errno = ENOTSUP;
				return -1;
			default:
				errno = EINVAL;
				return -1;
			}

	}

	if (name[0] == CTL_HW) {
		if (newp != NULL && newlenp > 0) {
			errno = EPERM;
			return -1;
		}

		switch (name[1]) {
		case HW_MACHINE:
			return copy(MACHINE, oldp, strlen(MACHINE), oldlenp);
		case HW_MODEL:
			return get_from_file("/sys/devices/virtual/dmi/id/product_name", oldp, *oldlenp);
		case HW_NCPU:
			if ((*(int*)oldp = get_int_from_file("/sys/devices/system/cpu/possible")) <= 0)
				return -1;
			break;
		case HW_BYTEORDER:
			*(int*)oldp = 1234; /* BSDLibC only for x86_64 for now */
			break;
		case HW_PHYSMEM:
		case HW_USERMEM:
			errno = ENOTSUP;
			return -1;
		case HW_PAGESIZE:
			*(int*)oldp = PAGE_SIZE;
			break;
		case HW_DISKNAMES:
		case HW_IOSTATS:
			errno = ENOTSUP;
			return -1;
		case HW_MACHINE_ARCH:
			return copy(MACHINE_ARCH, oldp, strlen(MACHINE_ARCH), oldlenp);
		case HW_ALIGNBYTES:
			*(int*)oldp = ALIGNBYTES;
			break;
		case HW_CNMAGIC:
		case HW_PHYSMEM64:
		case HW_USERMEM64:
		case HW_IOSTATNAMES:
			errno = ENOTSUP;
			return -1;
		case HW_NCPUONLINE:
			if ((*(int*)oldp = get_int_from_file("/sys/devices/system/cpu/online")) <= 0)
				return -1;
			break;
		default:
			errno = EINVAL;
			return -1;
		}
	}

	return 0;
}

static int
copy(const char *src, char *dst, size_t srclen, size_t *dstlen)
{
	if (dstlen == NULL)
	{
		*dstlen = srclen;
		return 0;
	}

	if (*dstlen < srclen)
	{
		*dstlen = srclen;
		errno = ENOMEM;
		return -1;
	}

	strlcpy(dst, src, *dstlen);
	return 0;
}

int
get_int_from_file(const char *path)
{
	int e;
	char buf[128];
	char *str;
	const char *errstr = NULL;

	int fd = open(path, O_RDONLY);
	if (fd == -1)
		return -1;

	ssize_t len = read(fd, buf, sizeof(buf));
	buf[len] = '\0';

	close(fd);

	if ((str = strrchr(buf, '\n')) != NULL)
		*str = '\0';

	if ((str = strchr(buf, '-')) != NULL)
	{
		*str = '\0';
		char *endp = str + 1;

		int n1 = strtoi(buf, NULL, 0, 0, INT32_MAX, &e);
		if (e)
			return -1;

		int n2 = strtoi(endp, NULL, 0, 0, INT32_MAX, &e);
		if (e)
			return -1;

		return n2 - n1 + 1;
	}

	/* ',' is not supported */
	if ((str = strchr(buf, ',')) != NULL)
	{
		return 1; /* fallback */
	}

	int n = strtoi(buf, NULL, 0, 0, INT32_MAX, &e);
	if (e)
		return -1;

	return n;
}

static int
get_from_file(const char *path, char *buf, size_t len)
{
	int fd, nl;
	char *str;
	if ((fd = open(path, O_RDONLY)) < 0)
		return -1;
	if ((nl = read(fd, buf, len)) <= 0)
		return -1;

	close(fd);
	buf[nl] = '\0';

	if ((str = strrchr(buf, '\n')) != NULL)
		*str = '\0';
	return 0;
}
