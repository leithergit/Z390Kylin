#! /bin/sh
rm ./a.out
rm /data_fs/lib/libmwReader.so*
tftp -gr a.out $1
tftp -b 20000 -gr libmwReader.so 192.168
mv libmwReader.so /data_fs/lib/libmwReader.so
ln /data_fs/lib/libmwReader.so /data_fs/lib/libmwReader.so.1
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data_fs/lib
chmod a+x ./a.out
./a.out