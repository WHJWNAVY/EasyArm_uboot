#!/bin/sh

echo "U-Boot build menu, please select your choice:"
echo "  1   make distclean"
echo "  2   config for mx28"
echo "  3   build linux kernel"
echo "  4   build linux kernel dtb"
echo "  q   exit"

source ./setenv.sh
NUM_CPUS=$(cat /proc/cpuinfo | grep -c processor)
SDBOOT=/home/work/easyarm280/mk_sdboot/

make_distclean() {
    echo "make distclean"
    make distclean
}

config_mx28() {
    make mxs_defconfig
}

build_kernel() {
    make -j${NUM_CPUS} LOADADDR=0x42000000 uImage
    cp ./arch/arm/boot/uImage ${SDBOOT}
    make -j${NUM_CPUS} zImage
    cp ./arch/arm/boot/zImage ${SDBOOT}
}

make_kernel_dtb() {
    make -j${NUM_CPUS} imx28-evk.dtb
    cp ./arch/arm/boot/dts/imx28-evk.dtb ${SDBOOT}
}

read sel
echo "select $sel"

case $sel in
    [1]) make_distclean;;
    [2]) config_mx28;;
    [3]) build_kernel;;
    [4]) make_kernel_dtb;;
    [q]) exit 0;;
    *)   echo "ONLY accept 1/2/3/4/q/Q"
esac    

