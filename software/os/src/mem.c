#include <mem.h>
#include <common.h>

static void *pf = NULL;
extern char _end;
static void *pbrk = NULL;
void* new_page(size_t nr_page) {
    void* p = pf;
    pf = (void*)((char*)pf + nr_page * PGSIZE);
    return p;
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
    pbrk = (void*)brk;
    return 0;
}

void init_mm() {
    pf = (void *)ROUNDUP(heap.start, PGSIZE);
    Log("Physical pages starting from %p", pf);
}
