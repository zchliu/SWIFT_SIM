BASE_PORT = $(abspath ../base-port)
SIM_PATH = $(abspath ../../simulator)
NAMES = nanos-lite
SRCS = $(shell find -L ./src/ -name "*.c" -o -name "*.cpp" -o -name "*.S")
ARGS = "-b $(abspath ../../simulator/sim/device/ramdisk/ramdisk.img)"
include $(BASE_PORT)/Makefile

OS_APP_PATH = $(abspath ../os-app)
update:
	@make -s -C $(OS_APP_PATH) ISA=riscv32 ramdisk
