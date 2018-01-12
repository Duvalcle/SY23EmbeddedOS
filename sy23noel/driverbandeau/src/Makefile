# Makefile


KDIR=/opt/linux-source-2.6.32

obj-m := driverbandeau.o

MODULE= $(obj-m:.o=.ko)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules


clean:
	rm -f *~
	$(MAKE) -C $(KDIR) M=$(PWD) clean
