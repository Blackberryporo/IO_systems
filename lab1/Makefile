MODULE_NAME = calculator
DEVICE_NAME = /dev/var2
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
	sudo echo "5+6" >> $(DEVICE_NAME)
	sudo echo "1-12" >> $(DEVICE_NAME)
	sudo echo "10/2" >> $(DEVICE_NAME)
	sudo echo "20*90" >> $(DEVICE_NAME)
	cat /dev/var2
	cat /proc/var2
	sudo dmesg

exit:
	sudo rmmod $(MODULE_NAME)

