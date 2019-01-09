KERN_DIR = /home/iTop4412/kernel/iTop4412_Kernel_3.0/

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= lcos.o

