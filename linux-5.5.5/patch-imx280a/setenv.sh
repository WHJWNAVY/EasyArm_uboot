#!/bin/bash


export WORK_BASE=/home/work/easyarm280
export ARCH=arm
export CROSS_COMPILE=arm-926ejs-linux-gnueabi-
#export CROSS_COMPILE=arm-fsl-linux-gnueabi-
export CROSS_COMPILE_DIR=${WORK_BASE}/toolchain/arm-926ejs-linux-gnueabi/bin
#export CROSS_COMPILE_DIR=${WORK_BASE}/toolchain/arm-fsl-linux-gnueabi/bin
export PATH=${CROSS_COMPILE_DIR}:$PATH

