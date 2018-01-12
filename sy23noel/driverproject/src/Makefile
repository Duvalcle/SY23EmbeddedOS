# Makefile



KDIR=/opt/linux-source-2.6.32

obj-m := drivermodele.o driverfox.o

MODULE= $(obj-m:.o=.ko)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
