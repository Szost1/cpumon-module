obj-m := cpuusagemon.o
KERNELDIR = /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
