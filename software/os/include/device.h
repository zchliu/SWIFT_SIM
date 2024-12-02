#ifndef __DEVICE__H__
#define __DEVICE__H__
#include <common.h>
extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t keyboard_read(void *buf, size_t offset, size_t len);
extern size_t fb_write(const void *buf, size_t offset, size_t len);
extern size_t dispinfo_read(void *buf, size_t offset, size_t len);
extern size_t invalid_read(void *buf, size_t offset, size_t len);
extern size_t invalid_write(const void *buf, size_t offset, size_t len);
#endif