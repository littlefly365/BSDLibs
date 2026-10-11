/*    $nlibc: lwp.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: lwp.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syscall.h>
#include <unistd.h>
#include <lwp.h>

#define ARCH_SET_FS		0x1002
#define ARCH_GET_FS		0x1003

#define FUTEX_WAIT		0
#define FUTEX_WAKE		1

int
_lwp_exit(void)
{
	(void)syscall(SYS_exit, 0);
	__builtin_unreachable();
}

lwpid_t
_lwp_self(void)
{
	return syscall(SYS_gettid);
}

int
_lwp_kill(lwpid_t lwp, int sig)
{
	return syscall(SYS_tgkill, getpid(), lwp, sig);
}

void
_lwp_setprivate(void *ptr)
{
	(void)syscall(SYS_arch_prctl, ARCH_SET_FS, ptr);
}

void
*_lwp_getprivate(void)
{
	unsigned long fs;
	if (syscall(SYS_arch_prctl, ARCH_GET_FS, &fs) == -1)
		return NULL;
	return (void*)fs;
}

/* FIXME: _lwp_park & _lwp_unpark */
int
_lwp_park(clockid_t clockid __unused, int flags __unused, struct timespec *ts __unused,
	lwpid_t unpark __unused, const void *hint __unused, const void *unparkhint __unused)
{
	return 0;
}

int
_lwp_unpark(lwpid_t lwp __unused, const void *hint __unused)
{
	return 0;
}
