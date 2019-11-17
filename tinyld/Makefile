obj-m := tinyld.o
KERNEL_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
all:
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean:
	rm *.o *.ko *.mod.c *.order *.symvers *.o.ur-safe

.PHONY:clean
