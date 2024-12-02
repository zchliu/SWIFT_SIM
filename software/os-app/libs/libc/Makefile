NAME = libc
CFLAGS = -DNO_FLOATING_POINT -DHAVE_INITFINI_ARRAY
CFLAGS += -U_FORTIFY_SOURCE  # fix compile error on ubuntu
SRCS = $(shell find src/ -name "*.c" -o -name "*.S" -o -name "*.cpp")
include $(OS_APP_PATH)/Makefile
