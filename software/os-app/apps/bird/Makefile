REPO_PATH = repo
ifeq ($(wildcard repo/include/BirdGame.h),)
  $(shell git clone --depth=1 https://github.com/USTC-System-Courses/CECS-sdlbird-game.git $(REPO_PATH))
endif

NAME = bird
SRCS = $(shell find $(REPO_PATH)/src/ -name "*.cpp")
INC_PATH = $(REPO_PATH)/include
LIBS += libSDL libSDL_image libfixedptc
include $(OS_APP_PATH)/Makefile

install: install-file
init:;

install-file:
	mkdir -p $(OS_APP_PATH)/fsimg/share/games
	ln -sf -T $(abspath $(REPO_PATH)/res) $(OS_APP_PATH)/fsimg/share/games/bird

.PHONY: install-file init
