MODULE_NAME = virt_net_if
obj-m = $(MODULE_NAME).o
PWD = $(shell pwd)
all:
	make -C /lib/modules/$(shell uname -r)/build M="$(PWD)" modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M="$(PWD)" clean

start:
	sudo dmesg -C
	sudo insmod ${MODULE_NAME}.ko
	sudo dmesg

test:
	ping 192.168.0.178

exit:
	sudo rmmod $(MODULE_NAME)

