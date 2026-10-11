/*    $nlibc: utimes.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: utimes.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

static void timeval_to_timespec(const struct timeval tv[2], struct timespec tm[2]);

int
__utimes50(const char *path, const struct timeval tv[2])
{
	struct timespec tm[2];
	if (!tv)
		return utimensat(AT_FDCWD, path, 0, 0);

	timeval_to_timespec(tv, tm);
	return utimensat(AT_FDCWD, path, tm, 0);
}

int
__futimes50(int fd, const struct timeval tv[2])
{
	struct timespec tm[2];
	if (!tv)
		return futimens(fd, 0);

	timeval_to_timespec(tv, tm);
	return futimens(fd, tm);
}

int
__lutimes50(const char *path, const struct timeval tv[2])
{
	struct timespec tm[2];
	if (!tv)
		return utimensat(AT_FDCWD, path, 0, AT_SYMLINK_NOFOLLOW);

	timeval_to_timespec(tv, tm);
	return utimensat(AT_FDCWD, path, tm, AT_SYMLINK_NOFOLLOW);
}

static void
timeval_to_timespec(const struct timeval tv[2], struct timespec tm[2])
{
	tm[0].tv_sec = tv[0].tv_sec;
	tm[0].tv_nsec = tv[0].tv_usec * 1000;
	tm[1].tv_sec = tv[1].tv_sec;
	tm[1].tv_nsec = tv[1].tv_usec * 1000;
}
