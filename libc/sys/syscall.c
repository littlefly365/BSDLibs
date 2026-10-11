/*    $nlibc: syscall.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: syscall.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syscall.h>
#include <stdarg.h>
#include <errno.h>
#include "namespace.h"

#ifdef __weak_alias
__weak_alias(syscall, _syscall);
__weak_alias(__syscall, syscall);
#endif

long
syscall(long n, ...)
{
	int r;
	va_list ap;

	va_start(ap, n);
	long a1 = va_arg(ap, long);
	long a2 = va_arg(ap, long);
	long a3 = va_arg(ap, long);
	long a4 = va_arg(ap, long);
	long a5 = va_arg(ap, long);
	long a6 = va_arg(ap, long);
	va_end(ap);

	if (n < 0 || n >= SYS_MAXSYSCALL) {
		errno = ENOSYS;
		return -1;
	}

#if defined(__x86_64__) || defined(__amd64__)
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;

	__asm__ __volatile__ (	"syscall" : "=a" (r) : "a"(n), "D"(a1),
				"S"(a2), "d"(a3), "r"(r10), "r"(r8), 
				"r"(r9) : "rcx", "r11", "memory");
#else
#error "nlibc is only for x86_64"
#endif

	if (r <= -1 || r >= 4095) {
		errno = -r;
		return -1;
	}

	return r;
}
