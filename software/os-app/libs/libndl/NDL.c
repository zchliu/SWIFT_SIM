#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  // printf("sec = %d, usec = %d\n", tv.tv_sec, tv.tv_usec);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  FILE *fd = fopen("/dev/events", "r");

  int ans = fread(buf, 1, len, fd);
  // printf("read %d\n", ans);
  fclose(fd);
  return ans;
}

static int canvas_w, canvas_h;

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  FILE* fd = fopen("/proc/dispinfo", "r");
  fscanf(fd, "WIDTH: %d\nHEIGHT: %d\n", &screen_w, &screen_h);
  if (*w == 0 && *h == 0) {
    *w = screen_w;
    *h = screen_h;
    canvas_w = screen_w;
    canvas_h = screen_h;
  } else {
    canvas_w = *w;
    canvas_h = *h;
  }
  // printf("screen_w = %d, screen_h = %d, w = %d, h = %d\n", screen_w, screen_h, *w, *h);
  fclose(fd);
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fp = open("/dev/fb", 0, 0);
  x += (screen_w - canvas_w) / 2;
  y += (screen_h - canvas_h) / 2;
  for (int i = 0; i < h; i++){
    lseek(fp, ((y + i) * screen_w + x) * 4, SEEK_SET);
    write(fp, pixels + i * w, w * 4);
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
