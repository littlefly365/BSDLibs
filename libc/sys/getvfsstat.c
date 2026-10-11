/*    $nlibc: getvfsstat.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: getvfsstat.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/statvfs.h>
#include <unistd.h>
#include <string.h>
#include <mntent.h>
#include <stdio.h>

int
__getvfsstat90(struct statvfs *buf, size_t bufsize, int flags __unused)
{
	FILE *f;
	int size = 0, i = 0;
	char linebuf[128];
	size_t nbuf = bufsize / sizeof(struct statvfs);
	struct statvfs vfs;
	struct mntent ent;

	if ((f = setmntent("/proc/self/mounts", "r")) == NULL)
		return -1;

	while (getmntent_r(f, &ent, linebuf, sizeof(linebuf)) != NULL)
	{
		if (buf != NULL && i < nbuf)
		{
			if (statvfs(ent.mnt_dir, &vfs) != 0)
				return -1;

			buf[i].f_flag = vfs.f_flag;
			buf[i].f_bsize = vfs.f_bsize;
			buf[i].f_frsize = vfs.f_frsize;
			buf[i].f_iosize = vfs.f_iosize;

			buf[i].f_blocks = vfs.f_blocks;
			buf[i].f_bfree = vfs.f_bfree;
			buf[i].f_bavail = vfs.f_bavail;
			buf[i].f_bresvd = vfs.f_bresvd;

			buf[i].f_files = vfs.f_files;
			buf[i].f_ffree = vfs.f_ffree;
			buf[i].f_favail = vfs.f_favail;
			buf[i].f_fresvd = vfs.f_fresvd;

			buf[i].f_syncreads = vfs.f_syncreads;
			buf[i].f_syncwrites = vfs.f_syncwrites;

			buf[i].f_asyncreads = vfs.f_asyncreads;
			buf[i].f_asyncwrites = vfs.f_asyncwrites;

			buf[i].f_fsidx = vfs.f_fsidx;
			buf[i].f_fsid = vfs.f_fsid;
			buf[i].f_namemax = vfs.f_namemax;
			buf[i].f_owner = vfs.f_owner;

			buf[i].f_spare[0] = vfs.f_spare[0];
			buf[i].f_spare[1] = vfs.f_spare[1];
			buf[i].f_spare[2] = vfs.f_spare[2];
			buf[i].f_spare[3] = vfs.f_spare[3];

			strlcpy(buf[i].f_fstypename, vfs.f_fstypename, sizeof(buf[i].f_fstypename));
			strlcpy(buf[i].f_mntonname, vfs.f_mntonname, sizeof(buf[i].f_mntonname));
			strlcpy(buf[i].f_mntfromname, vfs.f_mntfromname, sizeof(buf[i].f_mntfromname));
			strlcpy(buf[i].f_mntfromlabel, vfs.f_mntfromlabel, sizeof(buf[i].f_mntfromlabel));
			i++;
		}

		size++;
	}

	endmntent(f);
	return size;
}
