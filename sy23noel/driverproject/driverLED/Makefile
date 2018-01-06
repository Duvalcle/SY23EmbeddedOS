# Makefile



KDIR=/opt/linux-source-2.6.32

obj-m := driverLED.o 

MODULE= $(obj-m:.o=.ko)

all:
	cp ../driversimfox/Module.symvers .
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
