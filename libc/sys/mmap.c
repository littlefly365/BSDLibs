/*    $nlibc: mmap.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $    */

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
__RCSID("$nlibc: mmap.c,v 1.0 2026/10/11 11:47:10 littlefly365 Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#ifdef __weak_alias
__weak_alias(mmap, _mmap);
#endif

void *
_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	long ret;
	unsigned int align;
	size_t alignment;
	size_t extra;
	void *base;
	uintptr_t b, aligned;

	align = ((unsigned int)flags >> MAP_ALIGNMENT_SHIFT) & 0xff;
	flags &= ~MAP_ALIGNMENT_MASK;

	if (align == 0)
		return (void*)syscall(SYS_mmap, addr, len, prot, flags, fd, offset);

	alignment = (size_t)1 << align;
	extra = alignment - getpagesize();

	if (len > SIZE_MAX - extra) {
		errno = ENOMEM;
		return MAP_FAILED;
	}

	base = (void*)syscall(SYS_mmap, NULL, len + extra, prot, flags, fd, offset);
	if (base == MAP_FAILED)
		return MAP_FAILED;

	b = (uintptr_t)base;
	aligned = (b + alignment - 1) & ~(alignment - 1);

	if (aligned != b) {
		if (munmap((void*)b, aligned - b) == -1) {
			munmap(base, len + extra);
			return MAP_FAILED;
		}
	}

	if (aligned + len < b + len + extra) {
		if (munmap((void*)(aligned + len), (b + len + extra) - (aligned + len)) == -1) {
			munmap((void*)aligned, len);
			return MAP_FAILED;
		}
	}

	return (void*)aligned;
}
