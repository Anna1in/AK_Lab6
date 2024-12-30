
ifneq ($(KERNELRELEASE),)
include Kbuild
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$$PWD EXTRA_CFLAGS="-DDEBUG"
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif

