#ifndef __IRQ_H__
#define __IRQ_H__

#include <common.h>
Context* __irq_handle(Context *c);
bool cte_init(Context*(*handler)(Event, Context*));
void yield();

#endif
