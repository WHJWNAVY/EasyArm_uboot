# Install crosstool-ng

```bash
$ git clone https://github.com/crosstool-ng/crosstool-ng
$ cd crosstool-ng
$ ./bootstrap
$ make
$ make install # maybe need sudo

# check whether install success
$ ct-ng -v
GNU Make 4.1
Built for x86_64-pc-linux-gnu
Copyright (C) 1988-2014 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

$ ct-ng list-samples
```



# Build Configure

```bash
$ mkdir armv5te-arm926ejs-build
$ ct-ng arm-unknown-linux-gnueabi
$ ct-ng menuconfig
```

## Paths and misc options

![Paths and misc options](https://raw.githubusercontent.com/WHJWNAVY/myImage/master/PicGo/20200703155028.png)

## Target options

![Target options](https://raw.githubusercontent.com/WHJWNAVY/myImage/master/PicGo/20200703155029.png)

## Toolchain options

![Toolchain options](https://raw.githubusercontent.com/WHJWNAVY/myImage/master/PicGo/20200703155030.png)



# Config Patch

```diff
--- arm-unknown-linux-gnueabi.config	2020-07-03 15:18:34.168786597 +0800
+++ .config	2020-07-03 15:22:54.876696100 +0800
@@ -44,13 +44,13 @@
 #
 # Paths
 #
-CT_LOCAL_TARBALLS_DIR="${HOME}/src"
+CT_LOCAL_TARBALLS_DIR="${HOME}/work/toolchain/src"
 CT_SAVE_TARBALLS=y
 # CT_TARBALLS_BUILDROOT_LAYOUT is not set
 CT_WORK_DIR="${CT_TOP_DIR}/.build"
 CT_BUILD_TOP_DIR="${CT_WORK_DIR:-${CT_TOP_DIR}/.build}/${CT_HOST:+HOST-${CT_HOST}/}${CT_TARGET}"
 CT_BUILD_DIR="${CT_BUILD_TOP_DIR}/build"
-CT_PREFIX_DIR="${CT_PREFIX:-${HOME}/x-tools}/${CT_HOST:+HOST-${CT_HOST}/}${CT_TARGET}"
+CT_PREFIX_DIR="${HOME}/work/toolchain/${CT_TARGET}"
 CT_RM_RF_PREFIX_DIR=y
 # CT_REMOVE_DOCS is not set
 # CT_BUILD_MANUALS is not set
@@ -138,8 +138,7 @@
 # CT_ARCH_XTENSA is not set
 CT_ARCH="arm"
 CT_ARCH_CHOICE_KSYM="ARM"
-CT_ARCH_CPU=""
-CT_ARCH_TUNE=""
+CT_ARCH_CPU="arm926ej-s"
 CT_ARCH_ARM_SHOW=y
 
 #
@@ -187,7 +186,6 @@
 CT_ARCH_SUPPORTS_WITH_FPU=y
 CT_ARCH_SUPPORTS_SOFTFP=y
 CT_ARCH_EXCLUSIVE_WITH_CPU=y
-CT_ARCH_ARCH=""
 CT_ARCH_FPU=""
 # CT_ARCH_FLOAT_AUTO is not set
 # CT_ARCH_FLOAT_HW is not set
@@ -210,7 +208,7 @@
 CT_SYSROOT_DIR_PREFIX=""
 CT_WANTS_STATIC_LINK=y
 CT_WANTS_STATIC_LINK_CXX=y
-# CT_STATIC_TOOLCHAIN is not set
+CT_STATIC_TOOLCHAIN=y
 CT_SHOW_CT_VERSION=y
 CT_TOOLCHAIN_PKGVERSION=""
 CT_TOOLCHAIN_BUGURL=""
@@ -218,9 +216,9 @@
 #
 # Tuple completion and aliasing
 #
-CT_TARGET_VENDOR="unknown"
+CT_TARGET_VENDOR="926ejs"
 CT_TARGET_ALIAS_SED_EXPR=""
-CT_TARGET_ALIAS=""
+CT_TARGET_ALIAS="arm-fslc-linux-gnueabi"
 
 #
 # Toolchain type
@@ -364,17 +362,10 @@
 CT_BINUTILS_HAS_PLUGINS=y
 CT_BINUTILS_HAS_PKGVERSION_BUGURL=y
 CT_BINUTILS_GOLD_SUPPORTS_ARCH=y
-CT_BINUTILS_GOLD_SUPPORT=y
 CT_BINUTILS_FORCE_LD_BFD_DEFAULT=y
-# CT_BINUTILS_LINKER_LD is not set
-CT_BINUTILS_LINKER_LD_GOLD=y
-CT_BINUTILS_GOLD_INSTALLED=y
-CT_BINUTILS_GOLD_THREADS=y
-CT_BINUTILS_LINKER_BOTH=y
-CT_BINUTILS_LINKERS_LIST="ld,gold"
-CT_BINUTILS_LD_WRAPPER=y
+CT_BINUTILS_LINKER_LD=y
+CT_BINUTILS_LINKERS_LIST="ld"
 CT_BINUTILS_LINKER_DEFAULT="bfd"
-CT_BINUTILS_PLUGINS=y
 CT_BINUTILS_RELRO=m
 CT_BINUTILS_DETERMINISTIC_ARCHIVES=y
 CT_BINUTILS_EXTRA_CONFIG_ARRAY=""
@@ -533,8 +524,6 @@
 CT_GCC_REQUIRE_4_9_or_later=y
 CT_GCC_later_than_4_8=y
 CT_GCC_4_8_or_later=y
-CT_CC_GCC_ENABLE_PLUGINS=y
-CT_CC_GCC_GOLD=y
 CT_CC_GCC_HAS_LIBMPX=y
 CT_CC_GCC_ENABLE_CXX_FLAGS=""
 CT_CC_GCC_CORE_EXTRA_CONFIG_ARRAY=""
@@ -547,7 +536,6 @@
 # Optimisation features
 #
 CT_CC_GCC_USE_GRAPHITE=y
-CT_CC_GCC_USE_LTO=y
 
 #
 # Settings for libraries running on target
@@ -636,9 +624,8 @@
 CT_GDB_later_than_7_0=y
 CT_GDB_7_0_or_later=y
 CT_GDB_CROSS=y
-# CT_GDB_CROSS_STATIC is not set
+CT_GDB_CROSS_STATIC=y
 # CT_GDB_CROSS_SIM is not set
-# CT_GDB_CROSS_PYTHON is not set
 CT_GDB_CROSS_EXTRA_CONFIG_ARRAY=""
 CT_GDB_NATIVE=y
 CT_GDB_GDBSERVER=y
```



# Build Toolchain

```bash
ct-ng build
```



# Test Toolchain

```bash
$ ./arm-926ejs-linux-gnueabi/bin/arm-926ejs-linux-gnueabi-gcc -v

Using built-in specs.
COLLECT_GCC=./arm-926ejs-linux-gnueabi/bin/arm-926ejs-linux-gnueabi-gcc
COLLECT_LTO_WRAPPER=/home/xxxxxx/work/toolchain/arm-926ejs-linux-gnueabi/libexec/gcc/arm-926ejs-linux-gnueabi/9.2.0/lto-wrapper
Target: arm-926ejs-linux-gnueabi
Configured with: /home/xxxxxx/work/toolchain/arm-unknown-linux-gnueabi/.build/arm-926ejs-linux-gnueabi/src/gcc/configure --build=x86_64-build_pc-linux-gnu --host=x86_64-build_pc-linux-gnu --target=arm-926ejs-linux-gnueabi --prefix=/home/xxxxxx/work/toolchain/arm-926ejs-linux-gnueabi --with-sysroot=/home/xxxxxx/work/toolchain/arm-926ejs-linux-gnueabi/arm-926ejs-linux-gnueabi/sysroot --enable-languages=c,c++ --with-cpu=arm926ej-s --with-float=soft --with-pkgversion='crosstool-NG 1.24.0.105_5659366' --disable-sjlj-exceptions --enable-__cxa_atexit --disable-libmudflap --disable-libgomp --disable-libssp --disable-libquadmath --disable-libquadmath-support --disable-libsanitizer --disable-libmpx --with-gmp=/home/xxxxxx/work/toolchain/arm-unknown-linux-gnueabi/.build/arm-926ejs-linux-gnueabi/buildtools --with-mpfr=/home/xxxxxx/work/toolchain/arm-unknown-linux-gnueabi/.build/arm-926ejs-linux-gnueabi/buildtools --with-mpc=/home/xxxxxx/work/toolchain/arm-unknown-linux-gnueabi/.build/arm-926ejs-linux-gnueabi/buildtools --with-isl=/home/xxxxxx/work/toolchain/arm-unknown-linux-gnueabi/.build/arm-926ejs-linux-gnueabi/buildtools --disable-lto --with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++ -lm' --enable-threads=posix --enable-target-optspace --disable-plugin --disable-nls --disable-multilib --with-local-prefix=/home/xxxxxx/work/toolchain/arm-926ejs-linux-gnueabi/arm-926ejs-linux-gnueabi/sysroot --enable-long-long
Thread model: posix
gcc version 9.2.0 (crosstool-NG 1.24.0.105_5659366)
```

