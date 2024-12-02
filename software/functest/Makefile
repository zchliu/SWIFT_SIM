# NAMES = add-longlong
# SRCS = src/$(NAMES)
# BASE_PORT = ../base-port
# SIM_PATH = ## This is for running the simulator, you can ignore for Lab 1

# -include $(BASE_PORT)/Makefile

# NAMES ?= $(notdir $(basename $(wildcard src/*.c))) # 可以使用 wildcard 搭配其他函数来获取所有的程序名，使用方法请查阅 Makefile 教程

# .PHONY: all
# all: $(NAMES)

# %: src/%.c
# 	@echo "NAMES = $@\nSRCS = src/$@\nBASE_PORT = ../base-port\n-include "$$"(BASE_PORT)/Makefile"  > $@.mk  # 你需要将 <content> 替换为往每个 Makefile 中添加的内容
# 	@make -s -f $@.mk
# 	@rm -rf $@.mk

# clean:
# 	@rm -rf ./build

# clean-all:
# 	@rm -rf ../base-port/base/build

NAMES ?= $(notdir $(basename $(wildcard src/*.c))) # 可以使用 wildcard 搭配其他函数来获取所有的程序名，使用方法请查阅 Makefile 教程
ARGS ?= -b

.PHONY: compile test clean clean-all # 这里的 all 是伪目标，是一个标签不会生成文件
compile: $(NAMES)

%: src/%.c
	@NAMES=$@ SRCS=src/$@ BASE_PORT=../base-port make -s -f ../base-port/Makefile # 你需要将尖括号包裹的参数替换为合适的值

run: compile $(NAMES)
	@for name in $(NAMES); do \
		NAMES=$$name BASE_PORT=../base-port SIM_PATH=../../simulator ARGS=$(ARGS) make run -s -f ../base-port/Makefile; \
		if [ $$? -ne 0 ]; then \
			echo "Test $$name failed!"; \
			exit 1; \
		fi; \
	done
	@echo "Test PASS!"

clean:
	rm -rf ./build

clean-all:
	rm -rf ./build
	rm -rf ../base-port/base/build



