#!/bin/bash
PREFIX=`pwd`/../../obj/glib-2.40.0
CURPATH=`pwd`
tar -xvf  glib-2.40.0.tar.gz
cd glib-2.40.0

./autogen.sh

if [ $? -ne 0 ];then
	echo "autoconf failed!"
	exit
fi
./configure --prefix $PREFIX

if [ $? -ne 0 ];then
	echo "configure failed!"
	exit
fi

make 
make install
make clean
cd $CURPATH
rm -rf glib-2.40.0
