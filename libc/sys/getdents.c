/*    $nlibc: getdents.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: getdents.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syscall.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

struct linux_dirent {
	ino_t		d_ino;
	off_t		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char		d_name[];
};

int
__getdents30(int fd, struct dirent *buf, size_t len)
{
	char kbuf[4096];
	ssize_t nread;
	size_t pos;
	size_t out;

	nread = syscall(SYS_getdents64, fd, kbuf, sizeof(kbuf));
	if (nread < 0)
		return -1;

	pos = 0;
	out = 0;

	while (pos < (size_t)nread) {
		struct linux_dirent *src;
		struct dirent *dst;
		size_t namlen;
		size_t reclen;
		size_t maxnamlen;

		src = (struct linux_dirent *)(void *)(kbuf + pos);

		/*
		 * d_reclen must include everything from the beginning of
		 * the record through the terminating NUL of d_name.
		 */
		if (src->d_reclen < offsetof(struct linux_dirent, d_name) ||
		    src->d_reclen > (size_t)nread - pos) {
			errno = EIO;
			return -1;
		}

		maxnamlen = src->d_reclen -
		    offsetof(struct linux_dirent, d_name);

		namlen = strnlen(src->d_name, maxnamlen);

		/*
		 * A valid Linux directory entry must contain a NUL-terminated
		 * name inside the record.
		 */
		if (namlen == maxnamlen) {
			errno = EIO;
			return -1;
		}

		reclen = offsetof(struct dirent, d_name) +
		    namlen + 1;

		/*
		 * struct dirent entries are aligned to 8 bytes.
		 */
		reclen = (reclen + 7) & ~(size_t)7;

		if (reclen > len - out)
			break;

		dst = (struct dirent *)((char *)buf + out);

		dst->d_fileno = src->d_ino;
		dst->d_namlen = namlen;
		dst->d_type = src->d_type;
		dst->d_reclen = reclen;

		memcpy(dst->d_name, src->d_name, namlen);
		dst->d_name[namlen] = '\0';

		pos += src->d_reclen;
		out += reclen;
	}

	return (int)out;
}
