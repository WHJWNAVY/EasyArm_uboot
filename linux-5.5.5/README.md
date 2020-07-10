# 准备开发环境

> 详见[u-boot-2020.07-rc4](https://github.com/WHJWNAVY/EasyArm_uboot/tree/master/u-boot-2020.07-rc4)



# 升级arm-gcc工具链

> 详见[u-boot-2020.07-rc4](https://github.com/WHJWNAVY/EasyArm_uboot/tree/master/u-boot-2020.07-rc4)



# 下载和编译

```bash
$ wget https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.5.5.tar.gz
$ tar -xf linux-5.5.5.tar.gz
$ cp 0001-port-for-easyarm-imx280a.patch ./linux-5.5.5
$ cp build.sh ./linux-5.5.5
$ cp setenv.sh ./linux-5.5.5
$ cd linux-5.5.5
$ patch -p1 < 0001-port-for-easyarm-imx280a.patch # 可以用打补丁的形式,或者直接使用我修改好的文件,如下
$ cp imx28-evk.dts /arch/arm/boot/dts/imx28-evk.dts
$ ./build.sh
```

> linux 的移植相比u-boot的来说简单多了,linux已经把kernel和dts分开了
> 不需要对kernel做任何改动, 只需要修改设备树文件(`/arch/arm/boot/dts/imx28-evk.dts`)就可以了. 
> 从面的patch补丁中就可以看出我只是修改`imx28-evk.dts`,并没有对kernel做任何改动.
> 后面如果对设备树进行了改动,也只需要编译设备树就可以了,不需要重新编译kernel.

## [0001-port-for-easyarm-imx280a.patch](./patch-imx280a/0001-port-for-easyarm-imx280a.patch)

```diff
--- a/arch/arm/boot/dts/imx28-evk.dts
+++ b/arch/arm/boot/dts/imx28-evk.dts
@@ -11,7 +11,7 @@
 
 	memory@40000000 {
 		device_type = "memory";
-		reg = <0x40000000 0x08000000>;
+		reg = <0x40000000 0x04000000>;
 	};
 
 
@@ -107,9 +107,9 @@
 			ssp0: spi@80010000 {
 				compatible = "fsl,imx28-mmc";
 				pinctrl-names = "default";
-				pinctrl-0 = <&mmc0_8bit_pins_a
+				pinctrl-0 = <&mmc0_4bit_pins_a
 					&mmc0_cd_cfg &mmc0_sck_cfg>;
-				bus-width = <8>;
+				bus-width = <4>;
 				wp-gpios = <&gpio2 12 0>;
 				vmmc-supply = <&reg_vddio_sd0>;
 				status = "okay";
@@ -159,10 +159,11 @@
 					fsl,pull-up = <MXS_PULL_DISABLE>;
 				};
 
-				led_pin_gpio3_5: led_gpio3_5@0 {
+				led_pin_gpios: led_gpios@0 {
 					reg = <0>;
 					fsl,pinmux-ids = <
-						MX28_PAD_AUART1_TX__GPIO_3_5
+						MX28_PAD_SSP0_DATA7__GPIO_2_7
+						MX28_PAD_SAIF0_MCLK__GPIO_3_20
 					>;
 					fsl,drive-strength = <MXS_DRIVE_4mA>;
 					fsl,voltage = <MXS_VOLTAGE_HIGH>;
@@ -198,7 +199,7 @@
 				pinctrl-names = "default";
 				pinctrl-0 = <&lcdif_24bit_pins_a
 					     &lcdif_pins_evk>;
-				status = "okay";
+				status = "disabled";
 
 				port {
 					display_out: endpoint {
@@ -237,7 +238,7 @@
 			};
 
 			lradc@80050000 {
-				status = "okay";
+				status = "disabled";
 				fsl,lradc-touchscreen-wires = <4>;
 				fsl,ave-ctrl = <4>;
 				fsl,ave-delay = <2>;
@@ -267,26 +268,26 @@
 
 			pwm: pwm@80064000 {
 				pinctrl-names = "default";
-				pinctrl-0 = <&pwm2_pins_a>;
+				pinctrl-0 = <&pwm4_pins_a>;
 				status = "okay";
 			};
 
 			duart: serial@80074000 {
 				pinctrl-names = "default";
-				pinctrl-0 = <&duart_pins_a>;
+				pinctrl-0 = <&duart_pins_b>;
 				status = "okay";
 			};
 
 			auart0: serial@8006a000 {
 				pinctrl-names = "default";
-				pinctrl-0 = <&auart0_pins_a>;
+				pinctrl-0 = <&auart0_2pins_a>;
 				uart-has-rtscts;
 				status = "okay";
 			};
 
-			auart3: serial@80070000 {
+			auart1: serial@8006c000 {
 				pinctrl-names = "default";
-				pinctrl-0 = <&auart3_pins_a>;
+				pinctrl-0 = <&auart1_2pins_a>;
 				status = "okay";
 			};
 
@@ -303,7 +304,7 @@
 	ahb@80080000 {
 		usb0: usb@80080000 {
 			pinctrl-names = "default";
-			pinctrl-0 = <&usb0_id_pins_a>;
+			pinctrl-0 = <&usb0_id_pins_b>;
 			vbus-supply = <&reg_usb0_vbus>;
 			status = "okay";
 		};
@@ -318,7 +319,7 @@
 			pinctrl-names = "default";
 			pinctrl-0 = <&mac0_pins_a>;
 			phy-supply = <&reg_fec_3v3>;
-			phy-reset-gpios = <&gpio4 13 GPIO_ACTIVE_LOW>;
+			phy-reset-gpios = <&gpio1 26 GPIO_ACTIVE_LOW>;
 			phy-reset-duration = <100>;
 			status = "okay";
 		};
@@ -327,13 +328,13 @@
 			phy-mode = "rmii";
 			pinctrl-names = "default";
 			pinctrl-0 = <&mac1_pins_a>;
-			status = "okay";
+			status = "disabled";
 		};
 	};
 
 	sound {
 		compatible = "fsl,imx28-evk-sgtl5000",
-			     "fsl,mxs-audio-sgtl5000";
+				"fsl,mxs-audio-sgtl5000";
 		model = "imx28-evk-sgtl5000";
 		saif-controllers = <&saif0 &saif1>;
 		audio-codec = <&sgtl5000>;
@@ -342,13 +343,19 @@
 	leds {
 		compatible = "gpio-leds";
 		pinctrl-names = "default";
-		pinctrl-0 = <&led_pin_gpio3_5>;
+		pinctrl-0 = <&led_pin_gpios>;
 
-		user {
-			label = "Heartbeat";
-			gpios = <&gpio3 5 0>;
+		run {
+			label = "led-run";
+			gpios = <&gpio2 7 0>;
 			linux,default-trigger = "heartbeat";
 		};
+
+		err {
+			label = "led-err";
+			gpios = <&gpio3 20 0>;
+			linux,default-trigger = "default-on";
+		};
 	};
 
 	backlight_display: backlight {
```



# 烧写和启动

> 烧写镜像到sd卡并启动
## [windows](../mk_sdboot/win/mk_sdboot.bat)

```bash
.\cfimager.exe -a -f .\u-boot.sb -e rootfs.full.img -d h
copy zImage h: /y
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
```

> bootlog

```
U-Boot 2020.07-rc4 (Jul 04 2020 - 00:14:52 +0800)

CPU:   Freescale i.MX28 rev1.2 at 454 MHz
BOOT:  SSP SD/MMC #0, 3V3
DRAM:  64 MiB
NAND:  use legacy bch geometry
128 MiB
MMC:   MXS MMC: 0
Loading Environment from MMC... *** Warning - bad CRC, using default environment

In:    serial
Out:   serial
Err:   serial
Net:   FEC0 [PRIME]
Hit any key to stop autoboot:  0 
switch to partitions #0, OK
mmc0 is current device
5300920 bytes read in 1600 ms (3.2 MiB/s)
Booting from mmc ...
22122 bytes read in 41 ms (526.4 KiB/s)
Kernel image @ 0x42000000 [ 0x000000 - 0x50e2b8 ]
## Flattened Device Tree blob at 41000000
   Booting using the fdt blob at 0x41000000
   Loading Device Tree to 43b39000, end 43b41669 ... OK

Starting kernel ...

[    0.000000] Booting Linux on physical CPU 0x0
[    0.000000] Linux version 5.5.5-g7070cea-dirty (wnavy@wnavy-vm) (gcc version 9.2.0 (crosstool-NG 1.24.0.105_5659366)) #1 Tue Jul 7 00:38:11 CST 2020
[    0.000000] CPU: ARM926EJ-S [41069265] revision 5 (ARMv5TEJ), cr=0005317f
[    0.000000] CPU: VIVT data cache, VIVT instruction cache
[    0.000000] OF: fdt: Machine model: Freescale i.MX28 Evaluation Kit
[    0.000000] Memory policy: Data cache writeback
[    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 16256
[    0.000000] Kernel command line: console=ttyAMA0,115200 root=/dev/mmcblk0p3 rw rootwait
[    0.000000] Dentry cache hash table entries: 8192 (order: 3, 32768 bytes, linear)
[    0.000000] Inode-cache hash table entries: 4096 (order: 2, 16384 bytes, linear)
[    0.000000] mem auto-init: stack:off, heap alloc:off, heap free:off
[    0.000000] Memory: 47544K/65536K available (7092K kernel code, 635K rwdata, 2648K rodata, 324K init, 6368K bss, 17992K reserved, 0K cma-reserved)
[    0.000000] SLUB: HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
[    0.000000] Running RCU self tests
[    0.000000] NR_IRQS: 16, nr_irqs: 16, preallocated irqs: 16
[    0.000000] random: get_random_bytes called from start_kernel+0x2b4/0x4e0 with crng_init=0
[    0.000000] clocksource: mxs_timer: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 79635851949 ns
[    0.000038] sched_clock: 32 bits at 24MHz, resolution 41ns, wraps every 89478484971ns
[    0.002257] Console: colour dummy device 80x30
[    0.002429] Lock dependency validator: Copyright (c) 2006 Red Hat, Inc., Ingo Molnar
[    0.002491] ... MAX_LOCKDEP_SUBCLASSES:  8
[    0.002544] ... MAX_LOCK_DEPTH:          48
[    0.002592] ... MAX_LOCKDEP_KEYS:        8192
[    0.002638] ... CLASSHASH_SIZE:          4096
[    0.002685] ... MAX_LOCKDEP_ENTRIES:     32768
[    0.002733] ... MAX_LOCKDEP_CHAINS:      65536
[    0.002780] ... CHAINHASH_SIZE:          32768
[    0.002828]  memory used by lock dependency info: 3997 kB
[    0.002877]  memory used for stack traces: 2112 kB
[    0.002925]  per task-struct memory footprint: 1536 bytes
[    0.003267] Calibrating delay loop... 226.09 BogoMIPS (lpj=1130496)
[    0.071546] pid_max: default: 32768 minimum: 301
[    0.072633] Mount-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.072758] Mountpoint-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.083465] CPU: Testing write buffer coherency: ok
[    0.094288] Setting up static identity map for 0x40008400 - 0x40008458
[    0.103232] devtmpfs: initialized
[    0.178418] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns
[    0.178695] futex hash table entries: 256 (order: 1, 11264 bytes, linear)
[    0.180643] pinctrl core: initialized pinctrl subsystem
[    0.196613] NET: Registered protocol family 16
[    0.199970] DMA: preallocated 256 KiB pool for atomic coherent allocations
[    0.428485] Serial: AMBA PL011 UART driver
[    0.432586] 80074000.serial: ttyAMA0 at MMIO 0x80074000 (irq = 222, base_baud = 0) is a PL011 rev2
[    0.713981] printk: console [ttyAMA0] enabled
[    0.889110] mxs-dma 80004000.dma-apbh: initialized
[    0.908854] mxs-dma 80024000.dma-apbx: initialized
[    0.946217] SCSI subsystem initialized
[    0.953954] usbcore: registered new interface driver usbfs
[    0.960612] usbcore: registered new interface driver hub
[    0.966849] usbcore: registered new device driver usb
[    0.984452] pps_core: LinuxPPS API ver. 1 registered
[    0.989523] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giometti <giometti@linux.it>
[    0.999158] PTP clock support registered
[    1.006525] Advanced Linux Sound Architecture Driver Initialized.
[    1.024400] clocksource: Switched to clocksource mxs_timer
[    2.655197] random: fast init done
[    3.630761] NET: Registered protocol family 2
[    3.641319] tcp_listen_portaddr_hash hash table entries: 128 (order: 0, 5120 bytes, linear)
[    3.650374] TCP established hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    3.658496] TCP bind hash table entries: 1024 (order: 3, 36864 bytes, linear)
[    3.666357] TCP: Hash tables configured (established 1024 bind 1024)
[    3.674116] UDP hash table entries: 256 (order: 2, 20480 bytes, linear)
[    3.681431] UDP-Lite hash table entries: 256 (order: 2, 20480 bytes, linear)
[    3.690246] NET: Registered protocol family 1
[    3.702827] RPC: Registered named UNIX socket transport module.
[    3.709385] RPC: Registered udp transport module.
[    3.714183] RPC: Registered tcp transport module.
[    3.719191] RPC: Registered tcp NFSv4.1 backchannel transport module.
[    3.742843] workingset: timestamp_bits=30 max_order=14 bucket_order=0
[    3.852491] NFS: Registering the id_resolver key type
[    3.860031] Key type id_resolver registered
[    3.864847] Key type id_legacy registered
[    3.869574] jffs2: version 2.2. (NAND) © 2001-2006 Red Hat, Inc.
[    3.881311] Block layer SCSI generic (bsg) driver version 0.4 loaded (major 248)
[    3.889453] io scheduler mq-deadline registered
[    3.894102] io scheduler kyber registered
[    3.906141] pwm-backlight backlight: backlight supply power not found, using dummy regulator
[    3.927051] 8006a000.serial: ttyAPP0 at MMIO 0x8006a000 (irq = 220, base_baud = 1500000) is a 8006a000.serial
[    3.939733] mxs-auart 8006a000.serial: Found APPUART 3.1.0
[    3.947635] 8006c000.serial: ttyAPP1 at MMIO 0x8006c000 (irq = 221, base_baud = 1500000) is a 8006c000.serial
[    3.960154] mxs-auart 8006c000.serial: Found APPUART 3.1.0
[    8.032732] nand: device found, Manufacturer ID: 0xc2, Chip ID: 0xf1
[    8.039446] nand: Macronix NAND 128MiB 3,3V 8-bit
[    8.044253] nand: 128 MiB, SLC, erase size: 128 KiB, page size: 2048, OOB size: 64
[    8.052280] Scanning device for bad blocks
[    8.612323] Bad eraseblock 850 at 0x000006a40000
[    8.759365] gpmi-nand 8000c000.gpmi-nand: driver registered.
[    8.797974] spi-nor spi1.0: unrecognized JEDEC id bytes: ff ff ff ff ff ff
[    8.811453] libphy: Fixed MDIO Bus: probed
[    8.967174] libphy: fec_enet_mii_bus: probed
[    8.978260] usbcore: registered new interface driver asix
[    8.984151] usbcore: registered new interface driver ax88179_178a
[    8.991075] usbcore: registered new interface driver cdc_ether
[    8.997805] usbcore: registered new interface driver smsc95xx
[    9.004026] usbcore: registered new interface driver net1080
[    9.010418] usbcore: registered new interface driver cdc_subset
[    9.017062] usbcore: registered new interface driver zaurus
[    9.023305] usbcore: registered new interface driver cdc_ncm
[    9.029310] ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
[    9.036939] usbcore: registered new interface driver usb-storage
[    9.068170] ci_hdrc ci_hdrc.0: EHCI Host Controller
[    9.074195] ci_hdrc ci_hdrc.0: new USB bus registered, assigned bus number 1
[    9.104682] ci_hdrc ci_hdrc.0: USB 2.0 started, EHCI 1.00
[    9.128549] hub 1-0:1.0: USB hub found
[    9.133663] hub 1-0:1.0: 1 port detected
[    9.158503] ci_hdrc ci_hdrc.1: EHCI Host Controller
[    9.163730] ci_hdrc ci_hdrc.1: new USB bus registered, assigned bus number 2
[    9.194731] ci_hdrc ci_hdrc.1: USB 2.0 started, EHCI 1.00
[    9.208329] hub 2-0:1.0: USB hub found
[    9.212604] hub 2-0:1.0: 1 port detected
[    9.232317] stmp3xxx-rtc 80056000.rtc: registered as rtc0
[    9.240114] i2c /dev entries driver
[    9.263847] stmp3xxx_rtc_wdt stmp3xxx_rtc_wdt: initialized watchdog with heartbeat 19s
[    9.280220] mxs-mmc 80010000.spi: Got WP GPIO
[    9.316784] mxs-mmc 80010000.spi: initialized
[    9.375456] mmc0: new high speed SDHC card at address 0007
[    9.386819] usbcore: registered new interface driver usbhid
[    9.392495] usbhid: USB HID core driver
[    9.403055] mmcblk0: mmc0:0007 SD16G 14.5 GiB 
[    9.452132]  mmcblk0: p1 p2 p3
[    9.463335] imx28-pinctrl 80018000.pinctrl: pin SAIF0_MCLK already requested by leds; cannot claim for 80042000.saif
[    9.474274] imx28-pinctrl 80018000.pinctrl: pin-116 (80042000.saif) status -22
[    9.481850] imx28-pinctrl 80018000.pinctrl: could not request pin 116 (SAIF0_MCLK) from group saif0.0  on device 80018000.pinctrl
[    9.493821] mxs-saif 80042000.saif: Error applying setting, reverse things back
[    9.523013] mxs-saif: probe of 80042000.saif failed with error -22
[    9.534272] mxs-sgtl5000 sound: failed to get mclk
[    9.539583] mxs-sgtl5000: probe of sound failed with error -22
[    9.547234] NET: Registered protocol family 17
[    9.552400] Key type dns_resolver registered
[    9.560733] registered taskstats version 1
[    9.712756] stmp3xxx-rtc 80056000.rtc: setting system clock to 1970-01-01T13:08:19 UTC (47299)
[    9.723350] ALSA device list:
[    9.727181]   No soundcards found.
[    9.737929] uart-pl011 80074000.serial: no DMA platform data
[    9.998528] EXT4-fs (mmcblk0p3): recovery complete
[   10.021308] EXT4-fs (mmcblk0p3): mounted filesystem with ordered data mode. Opts: (null)
[   10.030344] VFS: Mounted root (ext4 filesystem) on device 179:3.
[   10.042110] devtmpfs: mounted
[   10.047675] Freeing unused kernel memory: 324K
[   10.052218] This architecture does not have kernel memory protection.
[   10.058981] Run /sbin/init as init process
[   10.418092] EXT4-fs (mmcblk0p3): re-mounted. Opts: (null)
mount: mounting /dev/mmcblk0p1 on /mnt/sdcard failed: Invalid argument
Starting syslogd: OK
Starting klogd: OK
Running sysctl: OK
Starting mdev... OK
modprobe: can't change directory to '/lib/modules': No such file or directory
Initializing random number generator: OK
Saving random seed: [   15.269711] random: dd: uninitialized urandom read (512 bytes read)
OK
Starting network: [   16.079916] Generic PHY 800f0000.ethernet-1:05: attached PHY driver [Generic PHY] (mii_bus:phy_addr=800f0000.ethernet-1:05, irq=POLL)
OK
Starting ntpd: OK

Welcome to EasyArm-i.MX280a
EasyArm-i.MX280a login: [   19.228436] fec 800f0000.ethernet eth0: Link is Up - 100Mbps/Full - flow control off
[   40.266980] lcd-3v3: disabling
[   40.270673] can-3v3: disabling
[   40.273856] lcd-5v: disabling
[  164.826088] random: crng init done

Welcome to EasyArm-i.MX280a
EasyArm-i.MX280a login: root
Password: 
# cat /proc/version 
Linux version 5.5.5-g7070cea-dirty (wnavy@wnavy-vm) (gcc version 9.2.0 (crosstool-NG 1.24.0.105_5659366)) #1 Tue Jul 7 00:38:11 CST 2020
# cat /proc/cpuinfo 
processor       : 0
model name      : ARM926EJ-S rev 5 (v5l)
BogoMIPS        : 226.09
Features        : swp half thumb fastmult edsp java 
CPU implementer : 0x41
CPU architecture: 5TEJ
CPU variant     : 0x0
CPU part        : 0x926
CPU revision    : 5

Hardware        : Freescale MXS (Device Tree)
Revision        : 0000
Serial          : 0000000000000000
# top
Mem: 21708K used, 26160K free, 40K shrd, 304K buff, 4712K cached
CPU:   0% usr  28% sys   0% nic  71% idle   0% io   0% irq   0% sirq
Load average: 0.02 0.06 0.02 1/39 145
  PID  PPID USER     STAT   VSZ %VSZ %CPU COMMAND
  145   141 root     R     2640   5%  29% top
  139     1 root     S     5384  11%   0% /usr/sbin/ntpd -g
  104     1 root     S     3028   6%   0% /sbin/mdev -df
  141     1 root     S     2768   6%   0% -sh
    1     0 root     S     2640   5%   0% init
   88     1 root     S     2640   5%   0% /sbin/syslogd -n
   92     1 root     S     2640   5%   0% /sbin/klogd -n
   22     2 root     IW       0   0%   0% [kworker/0:1-eve]
   70     2 root     IW<      0   0%   0% [kworker/0:1H-kb]
    7     2 root     SW       0   0%   0% [ksoftirqd/0]
    5     2 root     IW       0   0%   0% [kworker/u2:0-ev]
    8     2 root     SW       0   0%   0% [kdevtmpfs]
   71     2 root     SW       0   0%   0% [jbd2/mmcblk0p3-]
    2     0 root     SW       0   0%   0% [kthreadd]
    3     2 root     IW       0   0%   0% [kworker/0:0-pm]
    4     2 root     IW<      0   0%   0% [kworker/0:0H-kb]
    6     2 root     IW<      0   0%   0% [mm_percpu_wq]
    9     2 root     SW       0   0%   0% [khungtaskd]
   10     2 root     SW       0   0%   0% [oom_reaper]
   11     2 root     IW<      0   0%   0% [writeback]
# 
```
