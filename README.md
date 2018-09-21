[![Build Status](https://travis-ci.org/rpodgorny/unionfs-fuse.svg?branch=master)](https://travis-ci.org/rpodgorny/unionfs-fuse)
[![Gratipay](http://img.shields.io/gratipay/rpodgorny.svg)](https://gratipay.com/rpodgorny/)

multipath-fuse made of unionfs-fuse
============

Effectively union.
Write operations go to RW branch, Read operations go from RO branch.
First branch must be RW second RO. 

usage
============

unionfs -o cow -o dirs=/home/test/t01=RW:/home/test/testbr/t01=RO /home/test/t01merge

in /etc/fstab:
unionfs#/home/test/t01=rw:/home/test/testbr/t01=ro /home/test/t01merge fuse allow_other,cow,direct_io,_netdev 0 0

if you need to wait for underlying network file-system to mount first 
unionfs#/home/test/t01=rw:/home/test/testbr/t01=ro /home/test/t01merge fuse x-systemd.requires-mounts-for=/home/test/t01,allow_other,cow,direct_io,_netdev 0 0

install
============

cd ~
git clone --single-branch -b stable https://github.com/lavdnone/unionfs-fuse.git
cd unionfs-fuse
make
sudo make install
