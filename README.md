# EasyArm SDboot

## build-env

* [install_build_package.sh](toolchain/install_build_package.sh)
* [crosstool-ng-armv5te-arm926ejs-build](toolchain/README.md)



## u-boot

* [README - v2020.07-rc4-port-easyarm280](u-boot-2020.07-rc4/README.md)
* [Source - v2020.07-rc4-port-easyarm280](https://github.com/WHJWNAVY/u-boot/tree/v2020.07-rc4-port-easyarm280)



## linux

* [README - linux-5.5.5](linux-5.5.5/README.md)



## ~~imx-bootlets-src~~

* [imx-bootlets-src-10.12.01.tar.gz](http://repository.timesys.com/buildsources/i/imx-bootlets/imx-bootlets-10.12.01/imx-bootlets-src-10.12.01.tar.gz)
* [imx-bootlets-src-10.12.01.tar.gz](http://download.ossystems.com.br/bsp/freescale/source/imx-bootlets-src-10.12.01.tar.gz)

* [ref-git.yoctoproject.org](http://git.yoctoproject.org/cgit/cgit.cgi/meta-freescale/tree/recipes-bsp/imx-bootlets/imx-bootlets_10.12.01.bb?h=pyro)



## documents

* [再谈SD卡分区与引导(推荐) * new](mk_sdboot/linux/README.md)

* [EasyArm_IMX280A_从SD卡启动系统](mk_sdboot/EasyArm_IMX280A_从SD卡启动系统.pdf)
* [EasyArm_IMX280A_SD分区详解](mk_sdboot/EasyArm_IMX280A_SD分区详解.pdf)
* [EasyArm_IMX280A_SD卡启动引导](mk_sdboot/EasyArm_IMX280A_SD卡启动引导.pdf)
* [EasyArm_IMX280A_u-boot的FAT32支持](mk_sdboot/EasyArm_IMX280A_u-boot的FAT32支持.pdf)
* [SD_TF卡启动系统Easy-28xx](mk_sdboot/SD_TF卡启动系统Easy-28xx.pdf)



## make sdcard boot

### [linux](mk_sdboot/linux/README.md)

```bash
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
```

### ~~[windows](mk_sdboot/win/mk_sdboot.bat)~~

```bat
@echo off
echo. 
echo EasyARM-iMX28x 制作SD卡启动程序
echo. 
echo 请输入SD卡盘符：
set /p diskpath=
set cmdpath=%~dp0
echo 注意:
echo 文件会被烧写在 %diskpath% 盘
echo. 
rem ---------------------------------------------------------------------------------------------
%cmdpath%cfimager.exe -a -f %cmdpath%u-boot.sb -e %cmdpath%rootfs.full.img -d %diskpath%
rem ---------------------------------------------------------------------------------------------
rem 用copy把uImage拷贝到FAT32分区中,避免直接烧写造成FAT32分区信息被破坏.
rem 由于uboot支持对fat32文件系统的读写,所以可以直接使用uboot的fatload命令把
rem uImage加载到内存中并启动. 命令为:"fatload mmc 0:1 $(loadaddr) uImage;bootm"
copy %cmdpath%zImage %diskpath%: /y
copy %cmdpath%imx28-evk.dtb %diskpath%: /y
echo 烧写完毕，按键退出
echo.
pause>nul
```

> cfimager.exe
```bash
cfimager.exe -h
Usage: cfimager.exe [options]
Options:
  -h, -?, -help                 Show this help
  -v, -version                  Display the version
  -f, -file <path>              Input firmware file
  -d, -drive <char>             Drive letter (no colon, case does not matter)
  -a, -always-format            Always reformat entire drive
  -x, -extra <int>              Extra kilobytes in firmware region
  -e, -extra-image <path>       Extra (3rd) partition image file
  -n, -no-format                Do not format the FAT32 partition
  -i, -info                     Show info about the drive, do not format
  -TA3                          Format drive for TA3 ROM only
  -TA4                          Format drive for TA4+ ROM only
  -imx51                        Format driver for i.MX51 ROM only
  -imx53                        Format driver for i.MX53 ROM only
  -img, -bin                    Preformatted image is provided, dump it on the device starting at block 0
  -redundant_boot, -dual_boot,
  -secondary_boot               Format the drive for secondary boot with primary and secondary image and config block
  -bcb, -BCB                    Format the drive for BCB boot
  -raw                          Write Image to physical location
  -offset                                               physical location offset, must be used with -raw
  -skip                         skip how many byte of firmware image, must be used with -raw
                                typical usage for linux uboot.bin is
                                   cfimager -raw -offset 0x400 -skip 0x400 -f uboot.bin -d G

The -f and -d options are required. All other options are optional. By default,
the tool will not reformat the entire drive. If it cannot place the firmware
without reformatting, it will display a message. You can then run the tool
again using the -always-format option.


The -e option used created 3rd partition with extra image file such ext3 for linuxboot image create
By default, the drive will be formatted so that it can work on both TA3 and
TA4 (and above) devices. Using the -TA3 or -TA4 switch will cause the drive
to be formatted so that it works with only that ROM version.

Based on the -imx51 option and the name of the nb0 file, the address to flash
the i.MX51 file on the card is determined automatically by cfimager

The -no-format option causes the tool to skip writing the FAT partition, but
still writes the corresponding partition entry in the MBR. This lets you use
the operating system's native formatter.
```



## bootargs

> BOOT FROM SD CARD

```bash
baudrate=115200
boot_fdt=try
bootargs=console=ttyAMA0,115200 root=/dev/nfs ip=dhcp nfsroot=:,v3,tcp
bootcmd=mmc dev ${mmcdev}; if mmc rescan; then if run loadbootscript; then run bootscript; else if run loadimage; then run mmcboot; else run netboot; fi; fi; else run netboot; fi
bootdelay=3
bootfile=uImage
bootscript=echo Running bootscript from mmc ...; source
console_fsl=ttyAM0
console_mainline=ttyAMA0
ethact=FEC0
ethaddr=00:04:29:2a:ae:02
ethprime=FEC0
fdt_addr=0x41000000
fdt_file=imx28-evk.dtb
get_cmd=dhcp
image=zImage
ip_dyn=yes
loadaddr=0x42000000
loadbootscript=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};
loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}
loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}
mmcargs=setenv bootargs console=${console_mainline},${baudrate} root=${mmcroot}
mmcboot=echo Booting from mmc ...; run mmcargs; if test ${boot_fdt} = yes || test ${boot_fdt} = try; then if run loadfdt; then bootz ${loadaddr} - ${fdt_addr}; else if test ${boot_fdt} = try; then bootz; else echo WARN: Cannot load the DT; fi; fi; else bootz; fi;
mmcdev=0
mmcpart=2
mmcroot=/dev/mmcblk0p3 rw rootwait
script=boot.scr
update_sd_firmware=if mmc rescan ; then if tftp ${update_sd_firmware_filename} ; then setexpr fw_sz ${filesize} / 0x200 ; setexpr fw_sz ${fw_sz} + 1 ; mmc write ${loadaddr} 0x800 ${fw_sz} ; fi ; fi
update_sd_firmware_filename=u-boot.sd
```

> bootcmd

```bash
mmc dev ${mmcdev};
if mmc rescan; then
    if run loadbootscript; then
        run bootscript;
    else
        if run loadimage; then
            run mmcboot;
        else
            run netboot;
        fi;
    fi;
else
    run netboot;
fi
```

> mmcboot

```bash
echo Booting from mmc ...;
run mmcargs;
if test ${boot_fdt} = yes || test ${boot_fdt} = try; then
    if run loadfdt; then
        bootz ${loadaddr} - ${fdt_addr};
    else
        if test ${boot_fdt} = try; then
            bootz;
        else
            echo WARN: Cannot load the DT;
        fi;
    fi;
else
    bootz;
fi;
```