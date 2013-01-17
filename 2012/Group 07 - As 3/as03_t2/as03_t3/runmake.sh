#!/bin/sh
make && make install && (umount /media/ || true) && mount -t testfs none /media/