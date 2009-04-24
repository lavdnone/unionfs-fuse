#!/bin/sh

# Copyright: Bernd Schubert <bernd.schubert@fastmail.fm>
# BSD license, see LICENSE file for details

FUSE_OPT="-o default_permissions,allow_other,use_ino,suid"
CHROOT_PATH="/tmp/unionfs"
UNION_OPT="-ocow,chroot=$CHROOT_PATH"

UBIN=/usr/bin/unionfs-fuse

mount -t proc proc /proc
mount -t tmpfs tmpfs /tmp

mkdir -p $CHROOT_PATH/root
mkdir -p $CHROOT_PATH/rw
mkdir -p /tmp/union

ulimit -n 32768

mount --bind / $CHROOT_PATH/root

$UBIN $FUSE_OPT $UNION_OPT /rw=RW:/root=RO /tmp/union

mount --bind /dev /tmp/union/dev
mount -t proc proc /tmp/union/proc

cd /tmp/union
mkdir oldroot
pivot_root . oldroot

init q
