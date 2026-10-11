/*    $nlibc: sigaction.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: sigaction.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(sigaction, __sigaction_siginfo)
__weak_alias(__sigaction_sigtramp, __sigaction_siginfo);
__weak_alias(__sigtramp_siginfo_2, __sigaction_siginfo);
#endif

extern void __attribute__((noreturn)) linux_sigreturn(void);

struct linux_sigaction {
	void (*handler)(int);
	unsigned long flags;
	void (*restorer)(void);
	unsigned mask[2];
};

int
__sigaction_siginfo(int nsig, const struct sigaction *restrict sa, struct sigaction *restrict old)
{
	int flags;
	struct linux_sigaction sai, oldi;

	if (sa != NULL)
	{
		sai.handler = sa->sa_handler;
		flags = sa->sa_flags;

		flags |= 0x04000000;

		sai.flags = flags;
		sai.restorer = linux_sigreturn;

		sai.mask[0] = sa->sa_mask.__bits[0];
		sai.mask[1] = sa->sa_mask.__bits[1];
	}


	int ret = syscall(SYS_rt_sigaction, nsig, sa != NULL ? &sai : NULL, old != NULL ? &oldi : NULL, 8);
	if (ret == -1)
		return -1;

	if (old)
	{
		old->sa_handler = oldi.handler;
		old->sa_flags = oldi.flags & ~0x04000000;
		old->sa_mask.__bits[0] = oldi.mask[0];
		old->sa_mask.__bits[1] = oldi.mask[1];
	}

	return 0;
}
