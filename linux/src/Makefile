ifneq ($(KERNELRELEASE),)
	obj-m += telegraph.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD  := $(shell pwd)

default:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

install: telegraph.ko
	install -d /lib/modules/$(shell uname -r)/kernel/misc/
	install -m 644 telegraph.ko /lib/modules/$(shell uname -r)/kernel/misc/
	depmod
endif
