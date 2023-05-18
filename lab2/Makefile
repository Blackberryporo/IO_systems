MODULE_NAME = lab2
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
	sudo mkfs.vfat /dev/vramdisk1
	sudo mkfs.vfat /dev/vramdisk2
	sudo mkfs.vfat /dev/vramdisk5

	sudo mkdir /mnt/vramdisk1
	sudo mkdir /mnt/vramdisk2
	sudo mkdir /mnt/vramdisk5

	sudo mount -t vfat /dev/vramdisk1 /mnt/vramdisk1
	sudo mount -t vfat /dev/vramdisk2 /mnt/vramdisk2
	sudo mount -t vfat /dev/vramdisk5 /mnt/vramdisk5

create:
	sudo dd if=/dev/urandom of=/mnt/vramdisk1/file bs=1M count=5
	sudo dd if=/dev/urandom of=/mnt/vramdisk2/file bs=1M count=5
	sudo dd if=/dev/urandom of=/mnt/vramdisk5/file bs=1M count=5

remove:
	sudo rm /mnt/vramdisk1/file
	sudo rm /mnt/vramdisk2/file
	sudo rm /mnt/vramdisk5/file

copy: 
	sudo echo "Copying files within virtual disk"
	time sudo cp /mnt/vramdisk1/file /mnt/vramdisk2
	time sudo cp /mnt/vramdisk2/file /mnt/vramdisk5
	time sudo cp /mnt/vramdisk5/file /mnt/vramdisk1

copyext: 
	sudo echo "Copying files from virtual file to real disk"
	time sudo cp /mnt/vramdisk1/file ~
	time sudo cp /mnt/vramdisk2/file ~
	time sudo cp /mnt/vramdisk5/file ~
	
exit:
	sudo rm -rf /mnt/vramdisk1/file
	sudo rm -rf /mnt/vramdisk2/file
	sudo rm -rf /mnt/vramdisk5/file

	sudo rm -rf /mnt/vramdisk1
	sudo rm -rf /mnt/vramdisk2
	sudo rm -rf /mnt/vramdisk5

	sudo umount /dev/vramdisk1
	sudo umount /dev/vramdisk2
	sudo umount /dev/vramdisk5

	sudo rmmod $(MODULE_NAME)

