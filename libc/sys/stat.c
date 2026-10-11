/*    $nlibc: stat.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: stat.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

struct statx_timespec {
	int64_t tv_sec;
	uint32_t tv_nsec;
	int32_t pad;
};

struct statx {
	uint32_t stx_mask;
	uint32_t stx_blksize;
	uint64_t stx_attributes;
	uint32_t stx_nlink;
	uint32_t stx_uid;
	uint32_t stx_gid;
	uint16_t stx_mode;
	uint16_t pad1;
	uint64_t stx_ino;
	uint64_t stx_size;
	uint64_t stx_blocks;
	uint64_t stx_attributes_mask;
	struct statx_timespec stx_atime;
	struct statx_timespec stx_btime;
	struct statx_timespec stx_ctime;
	struct statx_timespec stx_mtime;
	uint32_t stx_rdev_major;
	uint32_t stx_rdev_minor;
	uint32_t stx_dev_major;
	uint32_t stx_dev_minor;
	uint64_t spare[14];
};

int
fstatat(int fd,  const char *path, struct stat *restrict stat, int flag)
{
	struct statx stx = {0};

	if (syscall(SYS_statx, fd, path, flag, 0xfff, &stx) != 0)
		return -1;

	stat->st_dev = makedev(stx.stx_dev_major, stx.stx_dev_minor);
	stat->st_ino = stx.stx_ino;
	stat->st_mode = stx.stx_mode;
	stat->st_nlink = stx.stx_nlink;
	stat->st_uid = stx.stx_uid;
	stat->st_gid = stx.stx_gid;
	stat->st_rdev = makedev(stx.stx_rdev_major, stx.stx_rdev_minor);
	stat->st_size = stx.stx_size;
	stat->st_blksize = stx.stx_blksize;
	stat->st_blocks = stx.stx_blocks;

	stat->st_atim.tv_sec = stx.stx_atime.tv_sec;
	stat->st_atim.tv_nsec = stx.stx_atime.tv_nsec;

	stat->st_mtim.tv_sec = stx.stx_mtime.tv_sec;
	stat->st_mtim.tv_nsec = stx.stx_mtime.tv_nsec;

	stat->st_ctim.tv_sec = stx.stx_ctime.tv_sec;
	stat->st_ctim.tv_nsec = stx.stx_ctime.tv_nsec;

	if (stx.stx_mask & 0x800U) {
		stat->st_birthtim.tv_sec = stx.stx_btime.tv_sec;
		stat->st_birthtim.tv_nsec = stx.stx_btime.tv_nsec;
	} else {
		/* Fill with 0 when btime is not available in the fs */
		stat->st_birthtim.tv_sec = 0;
		stat->st_birthtim.tv_nsec = 0;
	}

	stat->st_flags = 0; /* Linux does not have st_sflags */
	stat->st_gen = 0; /* Linux does not have st_gen */
	return 0;
}

int
stat(const char *path, struct stat *restrict stat)
{
	return fstatat(AT_FDCWD, path, stat, 0);
}

int
fstat(int fd, struct stat *restrict stat)
{
	return fstatat(fd, "", stat, 0x1000);
}

int
lstat(const char *path, struct stat *restrict stat)
{
	return fstatat(AT_FDCWD, path, stat, AT_SYMLINK_NOFOLLOW);
}
