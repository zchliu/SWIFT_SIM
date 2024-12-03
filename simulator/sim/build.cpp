#include <stdint.h>
#include <iostream>
#include <assert.h>
#include "include/debug.h"

extern uint8_t pmem[];

// load a binary file into your cpu
uint64_t load_img(char *img_file){

    FILE *fp = fopen(img_file, "rb");
    if (fp == NULL) {
        printf("Error: can't open %s\n", img_file);
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    uint64_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fread(pmem, size, 1, fp);
    fclose(fp);
    return size;
}