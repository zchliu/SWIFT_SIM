#include <debug.h>
#include "syscall.h"
#include <sys/time.h>
#include <mem.h>
#include <fs.h>
#include <proc.h>

void syscall_handle(Context *c) {
  uintptr_t a[4];
  a[0] = c->SYS_NUM;
  a[1] = c->SYS_ARG1;
  a[2] = c->SYS_ARG2;
  a[3] = c->SYS_ARG3;
  switch (a[0]) {
    case SYS_exit: {
      halt(0);
      break;
    }
    case SYS_yield: {
      yield();
      break;
    }
    case SYS_write: {
      c->SYS_RET = fs_write(a[1], (const void *)a[2], a[3]);
      break;
    }
    case SYS_open: {
      c->SYS_RET = fs_open((const char *)a[1], a[2], a[3]);
      break;
    }
    case SYS_read: {
      c->SYS_RET = fs_read(a[1], (void *)a[2], a[3]);
      break;
    }
    case SYS_lseek: {
      c->SYS_RET = fs_lseek(a[1], a[2], a[3]);
      break;
    }
    case SYS_close: {
      c->SYS_RET = fs_close(a[1]);
      break;
    }
    case SYS_gettimeofday: {
      size_t time = io_read(DEV_TIMER_UPTIME).us;
      ((struct timeval *)a[1])->tv_sec = time / 1000000;
      ((struct timeval *)a[1])->tv_usec = time % 1000000;
      c->SYS_RET = 0;
      break;
    }
    case SYS_execve: {
      user_naive_load((const char *) a[1]);
      c->SYS_RET = 0;
      break;
    }
    case SYS_brk: {
      c->SYS_RET = mm_brk(a[1]); 
      break;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
