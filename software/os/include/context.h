#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include <common.h>

Context *ucontext(AddrSpace *as, Area kstack, void *entry);
Context *kcontext(Area kstack, void (*entry)(void *), void *arg);

#endif