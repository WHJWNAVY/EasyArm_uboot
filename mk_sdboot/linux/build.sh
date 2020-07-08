#!/bin/sh

echo "Please input sdcard device name, (e.g. <sdc>)"
read sdcard

if [[ -z "$sdcard" ]]; then
    echo "Invalid device name!"
    exit 1
fi

fdisk /dev/${sdcard} < fdisk.part
mkdir /tmp/fat32
mount /dev/${sdcard}2 /tmp/fat32
cp uImage /tmp/fat32/
cp zImage /tmp/fat32/
cp imx28-evk.dtb /tmp/fat32/
dd if=u-boot.sd of=/dev/${sdcard}1
dd if=rootfs.full.img of=/dev/${sdcard}3

