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