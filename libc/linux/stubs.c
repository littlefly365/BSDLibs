#warning "These functions are stubs and this is dangerous. You must implement them the fastest you can"

#include <stdlib.h>
#include <stdio.h>

void _exit(int ec);

void
printerror(const char *fc)
{
	fprintf(stderr, "%s: %s: is not implemented yet\n", getprogname(), fc);
	_exit(127);
}

void recvmsg() { printerror("recvmsg"); }
void profil() { printerror("profil"); }
void wait6() { printerror("wait6"); }
void timer_delete() { printerror("timer_delete"); }
void timer_create() { printerror("timer_create"); }
void __timer_gettime50() { printerror("__timer_gettime50"); }
void __timer_settime50() { printerror("__timer_settime50"); }
void __msgctl50() { printerror("__msgctl50"); }
void __semctl50() { printerror("__semctl50"); }
void ____semctl50() { printerror("____semctl50"); }
void __shmctl50() { printerror("__shmctl50"); }
void __adjtime50() { printerror("__adjtime50"); }
void __mq_timedsend50() { printerror("__mq_timedsend50"); }
void __mq_timedreceive50() { printerror("__mq_timedreceive50"); }
void __lfs_segwait50() { printerror("__lfs_segwait50"); }
void __aio_suspend50() { printerror("__aio_suspend50"); }
void __settimeofday50() { printerror("__settimeofday50"); }
void ____sigtimedwait50() { printerror("____sigtimedwait50"); }
void __ntp_gettime50() { printerror("__ntp_gettime50"); }
void __sigtramp_siginfo_2() { printerror("__sigtramp_siginfo_2"); }
