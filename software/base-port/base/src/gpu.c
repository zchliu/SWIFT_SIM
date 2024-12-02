#include <dev-mmio.h>
#include <base.h>
#include <tool.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __gpu_init() {;

}

void __gpu_config(DEV_GPU_CONFIG_T *cfg) {
  uint32_t size = inl(VGACTL_ADDR);
  uint32_t w = (size >> 16) & 0x0ffff;
  uint32_t h = size & 0x0ffff;
  *cfg = (DEV_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __gpu_fbdraw(DEV_GPU_FBDRAW_T *ctl) {
  bool sync = ctl->sync;
  int x = ctl->x; // x*w
  int y = ctl->y; // y*h
  int w = ctl->w;
  int h = ctl->h;
  uint32_t *pixels = ctl->pixels;
  uint32_t size = inl(VGACTL_ADDR);
  uint32_t W = (size >> 16) & 0x0ffff;
  uint32_t H = size & 0x0ffff;
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *ffb = (uint32_t *)(uintptr_t)FFB_ADDR;
  ffb[0] = (uint32_t)x;
  ffb[1] = (uint32_t)y;
  ffb[2] = (uint32_t)w;
  ffb[3] = (uint32_t)h;
  ffb[4] = (uint32_t)W;
  ffb[5] = (uint32_t)H;
  ffb[6] = (uint32_t)&pixels[0];

  ffb[7] = 0;
  
  // for(int i = 0; i < h; i++){
  //   for(int j = 0; j < w; j++){
  //     if(y + i >= H || x + j >= W) continue;
  //     fb[(y + i) * W + x + j] = pixels[i * w + j];
  //   }
  // }
  if(sync){
    outl(SYNC_ADDR, 1);
    return;
  }
}

void __gpu_status(DEV_GPU_STATUS_T *status) {
  status->ready = true;
}