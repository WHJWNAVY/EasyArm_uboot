#!/bin/sh

echo "U-Boot build menu, please select your choice:"
echo "  1   make distclean"
echo "  2   config for mx28"
echo "  3   build u-boot"
echo "  4   make u-boot.sb"
echo "  q   exit"

source ./setenv.sh
NUM_CPUS=$(cat /proc/cpuinfo | grep -c processor)
TFTPBOOT=/home/work/tftpboot

make_distclean() {
    echo "make distclean"
    make distclean
}

config_mx28() {
    make mx28evk_defconfig
}

build_uboot() {
    #make -j${NUM_CPUS} u-boot.sb
    make -j${NUM_CPUS} spl/u-boot-spl
    make -j${NUM_CPUS} u-boot
}

make_uboot_sb() {
    elftosb -f imx28 -c arch/arm/cpu/arm926ejs/mxs/u-boot-imx28.bd -o u-boot.sb
}

read sel
echo "select $sel"

case $sel in
    [1]) make_distclean;;
    [2]) config_mx28;;
    [3]) build_uboot;;
    [4]) make_uboot_sb;;
    [q]) exit 0;;
    *)   echo "ONLY accept 1/2/3/4/q/Q"
esac    

