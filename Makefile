SRC := $(shell find ./src -type f -name "*.c")


all:
	gcc -I./include $(SRC) -o sistema
