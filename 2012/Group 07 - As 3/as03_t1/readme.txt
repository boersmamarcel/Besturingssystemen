How to install the testfs filesystem

Step 1:
Run make and make install

	$> cd as03_t1
	$> make
	$> make install

Step 2:
Edit system.conf

	$> nano /etc/system.conf
add the following line above "service rs":
	service testfs {};

Step 3:
Reboot

Step 4:
Mount the filesystem:

	$> mkdir /media
	$> mount -t testfs none /media

Step 5:
Experience the filesystem

	$> cd /media
	$> ls

All in one (Don't forget to edit the system.conf):
	$> cd as03_t1
	$> make && make install && umount /media/ && mount -t testfs none /media/
	$> ls /media/