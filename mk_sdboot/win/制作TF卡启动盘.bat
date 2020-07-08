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