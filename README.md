# EasyArm SDboot
> EasyArm-u-boot-2017.11-Linux2.6.35-SDboot

## u-boot-v2020.07-rc4
* [README - v2020.07-rc4-port-easyarm280](u-boot-2020.07-rc4/README.md)
* [Source - v2020.07-rc4-port-easyarm280](https://github.com/WHJWNAVY/u-boot/tree/v2020.07-rc4-port-easyarm280)

## imx-bootlets-src-10.12.01.tar.gz

[imx-bootlets-src-10.12.01.tar.gz](http://repository.timesys.com/buildsources/i/imx-bootlets/imx-bootlets-10.12.01/imx-bootlets-src-10.12.01.tar.gz)
[imx-bootlets-src-10.12.01.tar.gz](http://download.ossystems.com.br/bsp/freescale/source/imx-bootlets-src-10.12.01.tar.gz)

[ref-git.yoctoproject.org](http://git.yoctoproject.org/cgit/cgit.cgi/meta-freescale/tree/recipes-bsp/imx-bootlets/imx-bootlets_10.12.01.bb?h=pyro)

## README

* [EasyArm_IMX280A_从SD卡启动系统](easyarm_sdboot/EasyArm_IMX280A_从SD卡启动系统.pdf)
* [EasyArm_IMX280A_SD分区详解](easyarm_sdboot/EasyArm_IMX280A_SD分区详解.pdf)
* [EasyArm_IMX280A_SD卡启动引导](easyarm_sdboot/EasyArm_IMX280A_SD卡启动引导.pdf)
* [EasyArm_IMX280A_u-boot的FAT32支持](easyarm_sdboot/EasyArm_IMX280A_u-boot的FAT32支持.pdf)
* [SD_TF卡启动系统Easy-28xx](easyarm_sdboot/SD_TF卡启动系统Easy-28xx.pdf)



## 制作TF卡启动盘

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
rem %cmdpath%cfimager.exe -a -f %cmdpath%imx28_ivt_uboot.sb -d %diskpath%
%cmdpath%cfimager.exe -a -f %cmdpath%imx28_ivt_uboot.sb -e %cmdpath%rootfs.ext2.img -d %diskpath%
rem ---------------------------------------------------------------------------------------------
rem 从第2048(0x800)个扇区开始存放uImage,即从第1048576(0x100000)字节开始存放uImage
rem 总共2048个扇区,每个扇区512个字节,所以是2048*512=1048576
rem %cmdpath%cfimager.exe -raw -offset 0x100000 -f %cmdpath%uImage -d %diskpath%
rem ---------------------------------------------------------------------------------------------
rem 用copy把uImage拷贝到FAT32分区中,避免直接烧写造成FAT32分区信息被破坏.
rem 由于uboot支持对fat32文件系统的读写,所以可以直接使用uboot的fatload命令把
rem uImage加载到内存中并启动. 命令为:"fatload mmc 0:1 $(loadaddr) uImage;bootm"
copy %cmdpath%uImage %diskpath%: /y
rem ---------------------------------------------------------------------------------------------
rem 用全0文件填充uboot环境变量存放区域
rem uboot的环境变量默认存放在从第1024(0x400)字节处开始的127Kb空间中
%cmdpath%cfimager.exe -raw -offset 0x400 -f %cmdpath%zero.raw -d %diskpath%
rem ---------------------------------------------------------------------------------------------
echo 烧写完毕，按键退出
echo.
pause>nul
```



## bootargs

* BOOT FROM NAND FLASH
```bash
bootargs=gpmi=g console=ttyAM0,115200n8 ubi.mtd=1 root=ubi0:rootfs rootfstype=ubifs fec_mac= ethact mem=64M
```

* BOOT FROM SD CARD
```bash
setenv bootargs 'gpmi=g console=ttyAM0,115200n8 root=/dev/mmcblk0p3 rw rootwait rootfstype=ext2 init=/sbin/init fec_mac= ethact mem=64M'
# uImage放在SD卡的fat32分区中,uboot支持fat32分区,可以用fatload直接从fat32分区中加载uImage
setenv sdcard_boot 'fatload mmc 0:1 $(loadaddr) uImage;bootm'
setenv bootcmd 'run sdcard_boot'
```

## cfimager.exe
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