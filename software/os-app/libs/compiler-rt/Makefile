REPO_PATH = repo
ifeq ($(wildcard repo/LICENSE.TXT),)
  $(shell git clone --depth=1 https://github.com/USTC-System-Courses/CECS-compiler-rt.git $(REPO_PATH))
endif

NAME = compiler-rt
SRCS = $(shell find repo/ -name "*.c")
include $(OS_APP_PATH)/Makefile
