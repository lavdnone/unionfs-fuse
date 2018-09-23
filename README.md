[![Build Status](https://travis-ci.org/rpodgorny/unionfs-fuse.svg?branch=master)](https://travis-ci.org/rpodgorny/unionfs-fuse)
[![Gratipay](http://img.shields.io/gratipay/rpodgorny.svg)](https://gratipay.com/rpodgorny/)

multipathIO-fuse made of unionfs-fuse
============

Effectively union.

Write operations go to RW branch, Read operations go from RO branch.

First branch must be RW second RO. 

Lets say you have GlusterFS through number of sites as replicated volume for redundancy. To the moment of this writing there is no way you can get fast response for small file loads. As an example maildir dovecot mail storage format. And you can't switch from one-file one-email format as you want multiple nodes to access same emails. In most cases you can access local (for each site) copy of underlying GlusterFS brick, which has all the files. Using this unionfs build result mount point will pull reads locally with great small file speeds, but will do writes to the cluster to keep replicated volume in tact. 

If you app has enormous number of reads, compared to writes, using this can allow to separate reads to faster uplinked mount (10Gb subnet) and have rear writes go through lower grade network (100Mb). Or if you want to have writes go through another layer of some converting file system.

This can also hide changes made on system files on a separate path, effectively disregarding them but keeping record of what intruder was trying to change. Whoever will be making changes might not even figure that changes are not really applied and file system is effectively read only.

Not using `cow` (copy on write) option will make existing files unchangeable but all new files will go to writable path. This can be aceived with MergeFS or aufs as well.

This was tested on terabytes of emails for purpose of stable release, initially having perfectly stable unionfs-fuse.

usage
============
```bash
unionfs -o cow -o dirs=/home/test/t01=RW:/home/test/testbr/t01=RO /home/test/t01merge
```

in /etc/fstab:
```bash
unionfs#/home/test/t01=rw:/home/test/testbr/t01=ro /home/test/t01merge fuse allow_other,cow,direct_io,_netdev 0 0
```

if you need to wait for underlying network file-system to mount first like NFS or GlusterFS
```bash
unionfs#/home/test/t01=rw:/home/test/testbr/t01=ro /home/test/t01merge fuse x-systemd.requires-mounts-for=/home/test/t01,allow_other,cow,direct_io,_netdev 0 0
```

install
============

```bash
sudo yum install fuse fuse-devel

cd ~
git clone --single-branch -b stable https://github.com/lavdnone/unionfs-fuse.git
cd unionfs-fuse
make
sudo make install
```
