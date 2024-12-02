#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include "NDL.h"
int main() {
  struct timeval tv;
  int count = 0;
  NDL_Init(0);
  while(1) {
    // gettimeofday(&tv, NULL);
    uint32_t time = NDL_GetTicks();
    if (time / 500 >= count){
      count++;
      printf("hello for the %dth time!\n", count);
    }
  }
  
  return 0;
}
