$ cp build.sh ./linux-5.5.5
$ cp setenv.sh ./linux-5.5.5
$ patch -p1 < 0001-port-for-easyarm-imx280a.patch # 可以用打补丁的形式,或者直接使用我修改好的文件,如下
$ cp imx28-evk.dts /arch/arm/boot/dts/imx28-evk.dts
> linux 的移植相比u-boot的来说简单多了,linux已经把kernel和dts分开了
> 不需要对kernel做任何改动, 只需要修改设备树文件(`/arch/arm/boot/dts/imx28-evk.dts`)就可以了. 
> 从面的patch补丁中就可以看出我只是修改`imx28-evk.dts`,并没有对kernel做任何改动.
> 后面如果对设备树进行了改动,也只需要编译设备树就可以了,不需要重新编译kernel.
## [windows](../mk_sdboot/win/mk_sdboot.bat)

copy imx28-evk.dtb h: /y
```

## [linux *(new)](../mk_sdboot/linux/README.md)

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