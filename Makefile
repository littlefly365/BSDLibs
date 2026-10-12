WLIBCOMPAT?=no

SUBDIR=libc .WAIT libm libcrypt libutil ld.elf_so csu include

.if ${WLIBCOMPAT} != "no"
SUBDIR+=libcompat
.endif

.include <bsd.subdir.mk>
