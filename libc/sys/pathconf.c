/*    $nlibc: pathconf.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: pathconf.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syslimits.h>
#include <sys/statvfs.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

long
#ifdef _FPATHCONF
fpathconf(int fd, int name)
#else
pathconf(const char *path, int name)
#endif
{
	int ret;
	struct statvfs st;
#ifdef _FPATHCONF
	if ((ret = fstatvfs(fd, &st)) != 0 && errno == EBADFD)
#else
	if (path == NULL) {
		errno = EFAULT;
		return -1;
	}

	if (strlen(path) > PATH_MAX) {
		errno = ENAMETOOLONG;
		return -1;
	}

	if ((ret = statvfs(path, &st)) != 0 && errno == ENOENT)
#endif
		return -1;

	switch (name) {
	case _PC_LINK_MAX:
		return LINK_MAX;
	case _PC_MAX_CANON:
		return MAX_CANON;
	case _PC_MAX_INPUT:
		return MAX_INPUT;
	case _PC_NAME_MAX:
		if (ret != 0)
			return NAME_MAX;
		return st.f_namemax;
	case _PC_PATH_MAX:
		return PATH_MAX;
	case _PC_PIPE_BUF:
		return PIPE_BUF;
	case _PC_CHOWN_RESTRICTED:
		return _POSIX_CHOWN_RESTRICTED;
	case _PC_NO_TRUNC:
		return _POSIX_NO_TRUNC;
	case _PC_VDISABLE:
		return _POSIX_VDISABLE;
	case _PC_SYNC_IO:
		return 1;
	case _PC_FILESIZEBITS:
		return 64; 	/* 64? */
	case _PC_SYMLINK_MAX:
		return -1;	/* Glibc says there's not any system with this limit */
	case _PC_2_SYMLINKS:
		return 1;
	case _PC_ACL_EXTENDED:
		return 0;	/* fd do not support acl */
	case _PC_MIN_HOLE_SIZE:
		return 4096;
	case _PC_ACL_PATH_MAX:
		return PATH_MAX; /* is it the same? */
	case _PC_ACL_NFS4:
		return 0;	/* fd do not support NFSv4 acl */
	default:
		break;
	}

	errno = EINVAL;
	return -1;
}
