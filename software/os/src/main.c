#include <debug.h>

void init_irq(void);
void init_fs(void);
void init_proc(void);
void init_mm(void);

int main() {
  extern const char logo[];
  printf("%s", logo);
  Log("Build time: %s, %s", __TIME__, __DATE__);
  Log("Start initialization...");

  init_mm();
  init_irq();
  init_fs();
  init_proc();

  Log("Finish initialization");
  yield();

  panic("Should not reach here");
}
