#include <common.h>

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
    Context *p = (Context *)(kstack.end - sizeof(Context));
    p->mepc = (uintptr_t)entry;
    return p;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
    Context *p = (Context *)(kstack.end - sizeof(Context));
    p->mepc = (uintptr_t)entry;
    p->gpr[10] = (uintptr_t)arg;
    return p;
}
