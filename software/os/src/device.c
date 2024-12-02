#include <debug.h>

#define NAME(key) \
  [KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [KEY_NONE] = "NONE",
  KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char *p = (char *)buf;
  for (int i = 0; i < len; i++) {
    putch(p[i]);
  }
  return len;
}

size_t keyboard_read(void *buf, size_t offset, size_t len) {
  DEV_INPUT_KEYBRD_T kbd = io_read(DEV_INPUT_KEYBRD);
  size_t read_len = 0;
  if (kbd.keydown) {
    read_len += sprintf(buf, "kd %s\n", keyname[kbd.keycode]);
  } else if (kbd.keycode != KEY_NONE) {
    read_len += sprintf(buf, "ku %s\n", keyname[kbd.keycode]);
  }
  return read_len;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  DEV_GPU_CONFIG_T gpu_config = io_read(DEV_GPU_CONFIG);
  return sprintf(buf, "WIDTH: %d\nHEIGHT: %d", gpu_config.width, gpu_config.height);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  DEV_GPU_CONFIG_T gpu_config = io_read(DEV_GPU_CONFIG);
  offset /= 4;
  len /= 4;
  int x = offset % gpu_config.width;
  int y = offset / gpu_config.width;
  io_write(DEV_GPU_FBDRAW, x, y, (void*)buf, len, 1, true);
  return len;
}


size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}


