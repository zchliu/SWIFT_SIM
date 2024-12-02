#include <debug.h>
#include <proc.h>
#define ECALL_FROM_M 0xb
#define SYSCALL_YIELD 0xffffffff

void syscall_handle(Context *c);
static Context* __event_handle(Event e, Context* c);

Context* __irq_handle(Context *c) {
  Event ev = {0};
  switch (c->mcause) {
    case ECALL_FROM_M: {
      switch(c->gpr[17]){
        case SYSCALL_YIELD: ev.event = EVENT_YIELD; break;
        default: ev.event = EVENT_SYSCALL; break;
      } break;
    }

    default: ev.event = EVENT_ERROR; break;
  }
  c = __event_handle(ev, c);
  assert(c != NULL);
  return c;    
}


static Context* __event_handle(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:
      c->mepc += 4;
      break;
    case EVENT_SYSCALL:
      syscall_handle(c);
      c->mepc += 4;
      break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

extern void __trap_vector(void);

void yield() {
  asm volatile("li a7, -1; ecall");
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  asm volatile("csrw mtvec, %0" : : "r"(__trap_vector));
}
