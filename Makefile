all:
	cd socket && $(MAKE) && sudo insmod ksocket.ko
	cd master_device && $(MAKE) && sudo insmod master_device.ko
	cd slave_device && $(MAKE) && sudo insmod slave_device.ko
	gcc master.c -o master
	gcc slave.c -o slave
clean:
	cd master_device && $(MAKE) clean && sudo rmmod master_device.ko
	cd slave_device && $(MAKE) clean && sudo rmmod slave_device.ko
	cd socket && $(MAKE) clean && sudo rmmod ksocket.ko
	rm slave master
