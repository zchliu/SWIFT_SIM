#ifndef __MEM_H__
#define __MEM_H__

#include <common.h>

#ifndef PGSIZE
#define PGSIZE 4096
#endif

#define PG_ALIGN __attribute((aligned(PGSIZE)))

int mm_brk(uintptr_t brk);
void *new_page(size_t nr_page);
void init_mm();

#endif
