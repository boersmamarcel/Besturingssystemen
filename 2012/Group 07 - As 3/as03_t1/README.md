
###Commands;
Eerst map aanmaken waarop het filesystem gemount kan worden:
		mkdir /media  enter
		
ga naar map waar code in staat

compilen, mounten & testen:
		make && make install && umount /media/ && mount -t testfs none /media/
		ls /media/
		
Je hebt natuurlijk al je /etc/system.conf aangepast door 
service testfs {
};

toe te voegen