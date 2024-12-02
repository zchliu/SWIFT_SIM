#ifndef __PROC_H__
#define __PROC_H__

#include <common.h>
#include <mem.h>

#define STACK_SIZE (8 * PGSIZE)

typedef union {
  uint8_t stack[STACK_SIZE] PG_ALIGN;
  struct {
    Context *cp;
  };
} PCB;

extern PCB *current;
extern void user_naive_load(const char *filename);
extern void kernel_ctx_load(PCB *pcb, void (*entry)(void *), void *arg);
extern void user_ctx_load(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);

extern int execve(const char *filename, char *const argv[], char *const envp[]);
extern Context* schedule(Context *prev);


#endif