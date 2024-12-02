#include <fs.h>
#include <debug.h>
#include <device.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *  name;
  size_t  size;
  size_t  disk_offset;
  bool    is_open;
  ReadFn  read;
  WriteFn write;
  size_t  open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENTS, FD_FB, FD_DISPINFO};

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]    = {"stdin",           0, 0, 0, invalid_read,  invalid_write},
  [FD_STDOUT]   = {"stdout",          0, 0, 0, invalid_read,  serial_write},
  [FD_STDERR]   = {"stderr",          0, 0, 0, invalid_read,  serial_write},
  [FD_EVENTS]   = {"/dev/events",     0, 0, 0, keyboard_read,  invalid_write},
  [FD_FB]       = {"/dev/fb",         0, 0, 0, invalid_read,  fb_write},
  [FD_DISPINFO] = {"/proc/dispinfo",  0, 0, 0, dispinfo_read,  invalid_write},
  #include "files.h"
};

const size_t FILE_NUM = sizeof(file_table) / sizeof(file_table[0]);

/* Init the file system. */
void init_fs() {
  Log("Initializing file system...");
  DEV_GPU_CONFIG_T gpu_config = io_read(DEV_GPU_CONFIG);
  file_table[FD_FB].size = gpu_config.height * gpu_config.width * 4;
}

/* Open a file and return the size of file. */
int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < FILE_NUM; i++) {
    if (strcmp(file_table[i].name, pathname) == 0) {
      file_table[i].is_open = true;
      file_table[i].open_offset = 0;
      return i;
    }
  }
  return -1;
}

/* Read `len` bytes of data from the file with `fd` into `buf` */
size_t fs_read(int fd, void *buf, size_t len) {
  if (fd < 0 || fd >= FILE_NUM || !file_table[fd].is_open) {
    return -1;
  }

  size_t read_len = len;
  if (file_table[fd].open_offset + len > file_table[fd].size) {
    read_len = file_table[fd].size - file_table[fd].open_offset;
  }

  if (file_table[fd].read) {
    read_len = file_table[fd].read(buf, file_table[fd].open_offset, read_len);
  } else {
    ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, read_len);
  }
  file_table[fd].open_offset += read_len;

  return read_len;
}


/* Write `len` bytes of data from `buf` into the file with `fd` */
size_t fs_write(int fd, const void *buf, size_t len) {
  if (fd < 0 || fd >= FILE_NUM || !file_table[fd].is_open) {
    return -1;
  }

  size_t write_len = len;
  if (file_table[fd].open_offset + len > file_table[fd].size) {
    write_len = file_table[fd].size - file_table[fd].open_offset;
  }

  if (file_table[fd].write) {
    write_len = file_table[fd].write(buf, file_table[fd].open_offset, write_len);
  } else {
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, write_len);
  }
  file_table[fd].open_offset += write_len;

  return write_len;
}

/* Seek the file with `fd` from the `offset` based on `whence` */
size_t fs_lseek(int fd, size_t offset, int whence) {
  if (fd < 0 || fd >= FILE_NUM || !file_table[fd].is_open) {
    return -1;
  }

  size_t new_offset;
  switch (whence) {
    case SEEK_SET:
      new_offset = offset;
      break;
    case SEEK_CUR:
      new_offset = file_table[fd].open_offset + offset;
      break;
    case SEEK_END:
      new_offset = file_table[fd].size + offset;
      break;
    default:
      return -1;
  }

  if (new_offset > file_table[fd].size) {
    return -1;
  }

  file_table[fd].open_offset = new_offset;
  return new_offset;
}

/* Close the file with `fd` */
int fs_close(int fd){
  if (fd < 0 || fd >= FILE_NUM || !file_table[fd].is_open) {
    return -1;
  }

  file_table[fd].is_open = false;
  file_table[fd].open_offset = 0;
  return 0;
}
