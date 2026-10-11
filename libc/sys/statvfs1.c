/*    $nlibc: statvfs1.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: statvfs1.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syscall.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mntent.h>
#include <limits.h>
#include <fcntl.h>

struct statvfs_linux {
	unsigned long f_bsize;
	unsigned long f_frsize;
	fsblkcnt_t f_blocks;
	fsblkcnt_t f_bfree;
	fsblkcnt_t f_bavail;
	fsfilcnt_t f_files;
	fsfilcnt_t f_ffree;
	fsfilcnt_t f_favail;
	unsigned long f_fsid;
	int __f_unused;
	unsigned long f_flag;
	unsigned long f_namemax;
	unsigned int f_type;
	int __reserved[5];
};

static int __vfs(const char *path, struct statvfs *restrict buf, struct statvfs_linux *restrict vfs_linux);

int
__statvfs190(const char *restrict path, struct statvfs *restrict buf, int flags __unused)
{
	char new_path[PATH_MAX];
	struct statvfs_linux vfs_linux = {0};

	if (realpath(path, new_path) == NULL)
		return -1;

	if (syscall(SYS_statfs, new_path, &vfs_linux) < 0)
		return -1;

	return __vfs(new_path, buf, &vfs_linux);
}

int
__fstatvfs190(int fd, struct statvfs *restrict buf, int flags __unused)
{
	char path[PATH_MAX], tmp[PATH_MAX];
	struct statvfs_linux vfs_linux = {0};

	snprintf(tmp, sizeof(tmp), "/proc/self/%d", fd);
	if (realpath(tmp, path) == NULL)
		return -1;

	if (syscall(SYS_fstatfs, fd, &vfs_linux) < 0)
		return -1;

	return __vfs(path, buf, &vfs_linux);

}

static int
__vfs(const char *path, struct statvfs *restrict buf, struct statvfs_linux *restrict vfs_linux)
{
	FILE *fp;
	size_t blen = 0;
	char linebuf[128];
	struct mntent ent;
	char mnt_dir[PATH_MAX];
	char mnt_type[PATH_MAX];
	char mnt_fsname[PATH_MAX];

	buf->f_flag = vfs_linux->f_flag;
	buf->f_bsize = vfs_linux->f_bsize;
	buf->f_frsize = vfs_linux->f_frsize;
	buf->f_iosize = 0;

	buf->f_blocks = vfs_linux->f_blocks;
	buf->f_bfree = vfs_linux->f_bfree;
	buf->f_bavail = vfs_linux->f_bavail;
	buf->f_bresvd = 0;

	buf->f_files = vfs_linux->f_files;
	buf->f_ffree = vfs_linux->f_ffree;
	buf->f_favail = vfs_linux->f_favail;
	buf->f_fresvd = 0;

	/* We can't get this information in Linux */
	buf->f_syncreads = 0;
	buf->f_syncwrites = 0;
	buf->f_asyncreads = 0;
	buf->f_asyncwrites = 0;

	buf->f_fsidx = (fsid_t){ .__fsid_val = 0 };
	buf->f_fsid = vfs_linux->f_fsid;
	buf->f_namemax = vfs_linux->f_namemax;
	buf->f_owner = 0;

	if ((fp = setmntent("/proc/self/mounts", "r")) != NULL)
	{
		while (getmntent_r(fp, &ent, linebuf, sizeof(linebuf)) != NULL)
		{
			size_t len = strlen(ent.mnt_dir);
			if (strncmp(path, ent.mnt_dir, len))
				continue;
			if (len > 1 && path[len] != '\0' && path[len] != '/')
				continue;
			if (len > blen)
			{
				blen = len;
				strlcpy(mnt_type, ent.mnt_type, sizeof(mnt_type));
				strlcpy(mnt_dir, ent.mnt_dir, sizeof(mnt_dir));
				strlcpy(mnt_fsname, ent.mnt_fsname, sizeof(mnt_fsname));
			}
		}

		if (blen != 0)
		{
			strlcpy(buf->f_fstypename, mnt_type, sizeof(buf->f_fstypename));
			strlcpy(buf->f_mntonname, mnt_dir, sizeof(buf->f_mntonname));
			strlcpy(buf->f_mntfromname, mnt_fsname, sizeof(buf->f_mntfromname));
			strlcpy(buf->f_mntfromlabel, "unknown", sizeof(buf->f_mntfromlabel));
			endmntent(fp);
			return 0;
		}

		endmntent(fp);
	}

	strlcpy(buf->f_fstypename, "unknown", sizeof(buf->f_fstypename));
	strlcpy(buf->f_mntonname, "/", sizeof(buf->f_mntonname));
	strlcpy(buf->f_mntfromname, "unknown", sizeof(buf->f_mntfromname));
	strlcpy(buf->f_mntfromlabel, "unknown", sizeof(buf->f_mntfromlabel));
	return 0;
}
