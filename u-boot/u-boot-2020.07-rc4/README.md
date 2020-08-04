# 准备开发环境

> Ubuntu 16.04.5 LTS

```bash
$ sudo ./install_build_package.sh
```

## [install_build_package.sh](./patch-imx280a/install_build_package.sh)

```bash
#!/bin/sh

package_list=(autoconf automake bc binutils binutils-gold bison build-essential bzip2 cmake curl device-tree-compiler dpkg flex g++ gawk gcc gettext git gperf libc6-dev libncurses-dev libncurses5-dev libpcre3-dev libssl-dev libtool libxml-parser-perl lzma make mtd-utils npm ocaml ocaml-findlib ocaml-nox patch pkg-config python python-yaml rcs repo sharutils subversion texinfo u-boot-tools unzip vim wget xz-utils zlib1g-dev chrpath libfdt-dev coccinelle lib32z1 lib32ncurses5-dev)

install_error=0
install_log=/tmp/build_package_install.log
echo "==============================" > $install_log
for p in ${package_list[*]};
do
    echo "=============================="
    echo "Installing package <$p> ..."
    apt-get install -y $p >> $install_log
    install_error="$?"
    if [[ "$install_error" == "0" ]]; then
        echo "Package <$p> install success!"
    else
        echo "Package <$p> install error!"
    fi
done

echo "Package install over, See log at $install_log"
```



# 升级arm-gcc工具链

> 原厂自带的gcc工具链(`arm-fsl-linux-gnueabi-gcc`)版本(`gcc version 4.4.4 (4.4.4_09.06.2010)`)太老了，用来编译新版u-boot(`u-boot-2017.09`)会报警告(最新版本的u-boot甚至会报错)，如下。

```bash
*** Your GCC is older than 6.0 and will not be supported starting in v2018.01.
```

> 索性升级一下gcc工具链吧，后面编译新版本kernel也会用得着。

> [crosstool-NG](http://crosstool-ng.github.io/)是一个神器，专门用来编译生成自己的gcc工具链，我就是用`crosstool-ng`自己编译的最新版(`9.2.0`)的`arm-926ejs-linux-gnueabi-gcc`，具体编译方法参考另一篇文档:

## [crosstool-ng-armv5te-arm926ejs-build.md](./patch-imx280a/crosstool-ng-armv5te-arm926ejs-build.md)



# 下载和编译

```bash
$ wget https://ftp.denx.de/pub/u-boot/u-boot-2020.07-rc4.tar.bz2
$ tar -xf u-boot-2020.07-rc4.tar.bz2
$ cp 0001-u-boot-2020.07-rc4_port_for_easyarm_imx280a.patch ./u-boot-2020.07-rc4
$ cd u-boot-2020.07-rc4
$ patch -p1 < 0001-u-boot-2020.07-rc4_port_for_easyarm_imx280a.patch
$ ./build.sh
```


## [0001-u-boot-2020.07-rc4_port_for_easyarm_imx280a.patch](./patch-imx280a/0001-u-boot-2020.07-rc4_port_for_easyarm_imx280a.patch)

```diff
From 2f9791cb2a8ff763c850a82f1e51f8c575ed8abc Mon Sep 17 00:00:00 2001
From: WHJWNAVY <navy@mercku.com>
Date: Sat, 4 Jul 2020 00:24:27 +0800
Subject: [PATCH] u-boot-2020.07-rc4_port_for_easyarm_imx280a

---
 arch/arm/cpu/arm926ejs/mxs/spl_mem_init.c   | 16 ++++++++
 arch/arm/cpu/arm926ejs/mxs/spl_power_init.c |  5 ++-
 arch/arm/cpu/arm926ejs/mxs/u-boot-imx28.bd  | 12 +++---
 board/freescale/mx28evk/iomux.c             | 28 ++++++++++++-
 board/freescale/mx28evk/mx28evk.c           | 18 ++++++---
 build.sh                                    | 44 +++++++++++++++++++++
 configs/mx28evk_defconfig                   |  6 ++-
 configs/mx28evk_nand_defconfig              |  6 ++-
 include/common.h                            |  2 +
 include/configs/mx28evk.h                   |  4 ++
 include/configs/mxs.h                       |  5 +++
 setenv.sh                                   | 11 ++++++
 12 files changed, 139 insertions(+), 18 deletions(-)
 create mode 100644 build.sh
 create mode 100644 setenv.sh

diff --git a/arch/arm/cpu/arm926ejs/mxs/spl_mem_init.c b/arch/arm/cpu/arm926ejs/mxs/spl_mem_init.c
index a94803ee93..01441d50d3 100644
--- a/arch/arm/cpu/arm926ejs/mxs/spl_mem_init.c
+++ b/arch/arm/cpu/arm926ejs/mxs/spl_mem_init.c
@@ -29,11 +29,19 @@ __weak uint32_t mxs_dram_vals[] = {
 	0x00000000, 0x00000100, 0x00000000, 0x00000000,
 	0x00000000, 0x00000000, 0x00000000, 0x00000000,
 	0x00000000, 0x00000000, 0x00010101, 0x01010101,
+#if 0 /*CONFIG_IMX280A*/
 	0x000f0f01, 0x0f02020a, 0x00000000, 0x00010101,
 	0x00000100, 0x00000100, 0x00000000, 0x00000002,
 	0x01010000, 0x07080403, 0x06005003, 0x0a0000c8,
 	0x02009c40, 0x0002030c, 0x0036a609, 0x031a0612,
 	0x02030202, 0x00c8001c, 0x00000000, 0x00000000,
+#else
+	0x000f0f01, 0x0102020a, 0x00000000, 0x00000101,
+	0x00000100, 0x00000100, 0x00000000, 0x00000002,
+	0x01010000, 0x07080403, 0x0a008603, 0x0e0000c8,
+	0x0201046b, 0x00020514, 0x005b260f, 0x05230a22,
+	0x03050304, 0x00c80027, 0x00000000, 0x00000000,
+#endif
 	0x00012100, 0xffff0303, 0x00012100, 0xffff0303,
 	0x00012100, 0xffff0303, 0x00012100, 0xffff0303,
 	0x00000003, 0x00000000, 0x00000000, 0x00000000,
@@ -63,10 +71,18 @@ __weak uint32_t mxs_dram_vals[] = {
 	0x00000000, 0x00000000, 0x00000000, 0x00000000,
 	0x00000000, 0x00000000, 0x00000000, 0x00000000,
 	0x00000000, 0x00000000, 0x00010000, 0x00030404,
+#if 0 /*CONFIG_IMX280A*/
 	0x00000003, 0x00000000, 0x00000000, 0x00000000,
+#else
+	0x00000002, 0x00000000, 0x00000000, 0x00000000,
+#endif
 	0x00000000, 0x00000000, 0x00000000, 0x01010000,
 	0x01000000, 0x03030000, 0x00010303, 0x01020202,
+#if 0 /*CONFIG_IMX280A*/
 	0x00000000, 0x02040303, 0x21002103, 0x00061200,
+#else
+	0x00000000, 0x02050303, 0x21002103, 0x00061200,
+#endif
 	0x06120612, 0x04420442, 0x04420442, 0x00040004,
 	0x00040004, 0x00000000, 0x00000000, 0x00000000,
 	0x00000000, 0xffffffff
diff --git a/arch/arm/cpu/arm926ejs/mxs/spl_power_init.c b/arch/arm/cpu/arm926ejs/mxs/spl_power_init.c
index 35ea71a5ba..833596a0f2 100644
--- a/arch/arm/cpu/arm926ejs/mxs/spl_power_init.c
+++ b/arch/arm/cpu/arm926ejs/mxs/spl_power_init.c
@@ -751,8 +751,9 @@ static void mxs_batt_boot(void)
 	clrsetbits_le32(&power_regs->hw_power_5vctrl,
 		POWER_5VCTRL_CHARGE_4P2_ILIMIT_MASK,
 		0x8 << POWER_5VCTRL_CHARGE_4P2_ILIMIT_OFFSET);
-
+#if 0 /*CONFIG_IMX280A*/
 	mxs_power_enable_4p2();
+#endif
 }
 
 /**
@@ -1244,7 +1245,9 @@ void mxs_power_init(void)
 	mxs_ungate_power();
 
 	mxs_power_clock2xtal();
+#if 0 /*CONFIG_IMX280A*/
 	mxs_power_set_auto_restart();
+#endif
 	mxs_power_set_linreg();
 	mxs_power_setup_5v_detect();
 
diff --git a/arch/arm/cpu/arm926ejs/mxs/u-boot-imx28.bd b/arch/arm/cpu/arm926ejs/mxs/u-boot-imx28.bd
index c60615a456..891b5756a8 100644
--- a/arch/arm/cpu/arm926ejs/mxs/u-boot-imx28.bd
+++ b/arch/arm/cpu/arm926ejs/mxs/u-boot-imx28.bd
@@ -1,14 +1,14 @@
 sources {
-	u_boot_spl="spl/u-boot-spl.bin";
-	u_boot="u-boot.bin";
+	u_boot_spl="spl/u-boot-spl";
+	u_boot="u-boot";
 }
 
 section (0) {
-	load u_boot_spl > 0x0000;
-	load ivt (entry = 0x0014) > 0x8000;
+	load u_boot_spl;
+	load ivt (entry = u_boot_spl:_start) > 0x8000;
 	hab call 0x8000;
 
-	load u_boot > 0x40000100;
-	load ivt (entry = 0x40000100) > 0x8000;
+	load u_boot;
+	load ivt (entry = u_boot:_start) > 0x8000;
 	hab call 0x8000;
 }
diff --git a/board/freescale/mx28evk/iomux.c b/board/freescale/mx28evk/iomux.c
index cc0c858854..970a99bebb 100644
--- a/board/freescale/mx28evk/iomux.c
+++ b/board/freescale/mx28evk/iomux.c
@@ -22,28 +22,40 @@
 
 const iomux_cfg_t iomux_setup[] = {
 	/* DUART */
+#if 0 /*CONFIG_IMX280A*/
 	MX28_PAD_PWM0__DUART_RX,
 	MX28_PAD_PWM1__DUART_TX,
+#else
+    /* solve the conflict with DUART */
+    MX28_PAD_PWM0__GPIO_3_16,
+    MX28_PAD_PWM1__GPIO_3_17,
+	MX28_PAD_AUART0_CTS__DUART_RX,
+	MX28_PAD_AUART0_RTS__DUART_TX,
+#endif
 
 	/* MMC0 */
 	MX28_PAD_SSP0_DATA0__SSP0_D0 | MUX_CONFIG_SSP0,
 	MX28_PAD_SSP0_DATA1__SSP0_D1 | MUX_CONFIG_SSP0,
 	MX28_PAD_SSP0_DATA2__SSP0_D2 | MUX_CONFIG_SSP0,
 	MX28_PAD_SSP0_DATA3__SSP0_D3 | MUX_CONFIG_SSP0,
+#if 0 /*CONFIG_IMX280A*/
 	MX28_PAD_SSP0_DATA4__SSP0_D4 | MUX_CONFIG_SSP0,
 	MX28_PAD_SSP0_DATA5__SSP0_D5 | MUX_CONFIG_SSP0,
 	MX28_PAD_SSP0_DATA6__SSP0_D6 | MUX_CONFIG_SSP0,
 	MX28_PAD_SSP0_DATA7__SSP0_D7 | MUX_CONFIG_SSP0,
+#endif
 	MX28_PAD_SSP0_CMD__SSP0_CMD | MUX_CONFIG_SSP0,
 	MX28_PAD_SSP0_DETECT__SSP0_CARD_DETECT |
 		(MXS_PAD_8MA | MXS_PAD_3V3 | MXS_PAD_NOPULL),
 	MX28_PAD_SSP0_SCK__SSP0_SCK |
 		(MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_NOPULL),
+#if 0 /*CONFIG_IMX280A*/
 	/* write protect */
 	MX28_PAD_SSP1_SCK__GPIO_2_12,
 	/* MMC0 slot power enable */
 	MX28_PAD_PWM3__GPIO_3_28 |
 		(MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP),
+#endif
 
 #ifdef CONFIG_NAND_MXS
 	/* GPMI NAND */
@@ -75,13 +87,19 @@ const iomux_cfg_t iomux_setup[] = {
 	MX28_PAD_ENET0_TXD0__ENET0_TXD0 | MUX_CONFIG_ENET,
 	MX28_PAD_ENET0_TXD1__ENET0_TXD1 | MUX_CONFIG_ENET,
 	MX28_PAD_ENET_CLK__CLKCTRL_ENET | MUX_CONFIG_ENET,
+#if 0 /*CONFIG_IMX280A*/
 	/* FEC0 Enable */
 	MX28_PAD_SSP1_DATA3__GPIO_2_15 |
 		(MXS_PAD_12MA | MXS_PAD_3V3),
 	/* FEC0 Reset */
 	MX28_PAD_ENET0_RX_CLK__GPIO_4_13 |
 		(MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP),
-
+#else
+	/* FEC0 Reset */
+	MX28_PAD_LCD_RS__GPIO_1_26 |
+		(MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP),
+#endif
+#if 0 /*CONFIG_IMX280A*/
 	/* FEC1 */
 	MX28_PAD_ENET0_COL__ENET1_TX_EN | MUX_CONFIG_ENET,
 	MX28_PAD_ENET0_CRS__ENET1_RX_EN | MUX_CONFIG_ENET,
@@ -89,7 +107,7 @@ const iomux_cfg_t iomux_setup[] = {
 	MX28_PAD_ENET0_RXD3__ENET1_RXD1 | MUX_CONFIG_ENET,
 	MX28_PAD_ENET0_TXD2__ENET1_TXD0 | MUX_CONFIG_ENET,
 	MX28_PAD_ENET0_TXD3__ENET1_TXD1 | MUX_CONFIG_ENET,
-
+#endif
 	/* EMI */
 	MX28_PAD_EMI_D00__EMI_DATA0 | MUX_CONFIG_EMI,
 	MX28_PAD_EMI_D01__EMI_DATA1 | MUX_CONFIG_EMI,
@@ -109,7 +127,9 @@ const iomux_cfg_t iomux_setup[] = {
 	MX28_PAD_EMI_D15__EMI_DATA15 | MUX_CONFIG_EMI,
 	MX28_PAD_EMI_ODT0__EMI_ODT0 | MUX_CONFIG_EMI,
 	MX28_PAD_EMI_DQM0__EMI_DQM0 | MUX_CONFIG_EMI,
+#if 0 /*CONFIG_IMX280A*/
 	MX28_PAD_EMI_ODT1__EMI_ODT1 | MUX_CONFIG_EMI,
+#endif
 	MX28_PAD_EMI_DQM1__EMI_DQM1 | MUX_CONFIG_EMI,
 	MX28_PAD_EMI_DDR_OPEN_FB__EMI_DDR_OPEN_FEEDBACK | MUX_CONFIG_EMI,
 	MX28_PAD_EMI_CLK__EMI_CLK | MUX_CONFIG_EMI,
@@ -139,7 +159,9 @@ const iomux_cfg_t iomux_setup[] = {
 	MX28_PAD_EMI_RASN__EMI_RASN | MUX_CONFIG_EMI,
 	MX28_PAD_EMI_WEN__EMI_WEN | MUX_CONFIG_EMI,
 	MX28_PAD_EMI_CE0N__EMI_CE0N | MUX_CONFIG_EMI,
+#if 0 /*CONFIG_IMX280A*/
 	MX28_PAD_EMI_CE1N__EMI_CE1N | MUX_CONFIG_EMI,
+#endif
 	MX28_PAD_EMI_CKE__EMI_CKE | MUX_CONFIG_EMI,
 
 	/* SPI2 (for SPI flash) */
@@ -152,6 +174,7 @@ const iomux_cfg_t iomux_setup[] = {
 	MX28_PAD_I2C0_SCL__I2C0_SCL,
 	MX28_PAD_I2C0_SDA__I2C0_SDA,
 
+#if 0 /*CONFIG_IMX280A*/
 	/* LCD */
 	MX28_PAD_LCD_D00__LCD_D0 | MUX_CONFIG_LCD,
 	MX28_PAD_LCD_D01__LCD_D1 | MUX_CONFIG_LCD,
@@ -183,6 +206,7 @@ const iomux_cfg_t iomux_setup[] = {
 	MX28_PAD_LCD_CS__LCD_ENABLE | MUX_CONFIG_LCD,
 	MX28_PAD_LCD_RESET__GPIO_3_30 | MUX_CONFIG_LCD, /* LCD power */
 	MX28_PAD_PWM2__GPIO_3_18 | MUX_CONFIG_LCD, /* LCD contrast */
+#endif
 };
 
 #define HW_DRAM_CTL29	(0x74 >> 2)
diff --git a/board/freescale/mx28evk/mx28evk.c b/board/freescale/mx28evk/mx28evk.c
index e50b7db210..87e31b5ad5 100644
--- a/board/freescale/mx28evk/mx28evk.c
+++ b/board/freescale/mx28evk/mx28evk.c
@@ -50,12 +50,13 @@ int board_early_init_f(void)
 	gpio_direction_output(MX28_PAD_AUART2_RX__GPIO_3_8, 1);
 #endif
 
+#if 0 /*CONFIG_IMX280A*/
 	/* Power on LCD */
 	gpio_direction_output(MX28_PAD_LCD_RESET__GPIO_3_30, 1);
 
 	/* Set contrast to maximum */
 	gpio_direction_output(MX28_PAD_PWM2__GPIO_3_18, 1);
-
+#endif
 	return 0;
 }
 
@@ -111,7 +112,7 @@ int board_eth_init(bd_t *bis)
 	/* MX28EVK uses ENET_CLK PAD to drive FEC clock */
 	writel(CLKCTRL_ENET_TIME_SEL_RMII_CLK | CLKCTRL_ENET_CLK_OUT_EN,
 	       &clkctrl_regs->hw_clkctrl_enet);
-
+#if 0 // TODO: CONFIG_IMX280A
 	/* Power-on FECs */
 	gpio_direction_output(MX28_PAD_SSP1_DATA3__GPIO_2_15, 0);
 
@@ -119,31 +120,38 @@ int board_eth_init(bd_t *bis)
 	gpio_direction_output(MX28_PAD_ENET0_RX_CLK__GPIO_4_13, 0);
 	udelay(200);
 	gpio_set_value(MX28_PAD_ENET0_RX_CLK__GPIO_4_13, 1);
-
+#else
+	/* Reset FEC PHYs */
+	gpio_direction_output(MX28_PAD_LCD_RS__GPIO_1_26, 0);
+	udelay(200);
+	gpio_set_value(MX28_PAD_LCD_RS__GPIO_1_26, 1);
+#endif
 	ret = fecmxc_initialize_multi(bis, 0, 0, MXS_ENET0_BASE);
 	if (ret) {
 		puts("FEC MXS: Unable to init FEC0\n");
 		return ret;
 	}
 
+#if 0 /*CONFIG_IMX280A*/
 	ret = fecmxc_initialize_multi(bis, 1, 3, MXS_ENET1_BASE);
 	if (ret) {
 		puts("FEC MXS: Unable to init FEC1\n");
 		return ret;
 	}
+#endif
 
 	dev = eth_get_dev_by_name("FEC0");
 	if (!dev) {
 		puts("FEC MXS: Unable to get FEC0 device entry\n");
 		return -EINVAL;
 	}
-
+#if 0 /*CONFIG_IMX280A*/
 	dev = eth_get_dev_by_name("FEC1");
 	if (!dev) {
 		puts("FEC MXS: Unable to get FEC1 device entry\n");
 		return -EINVAL;
 	}
-
+#endif
 	return ret;
 }
 
diff --git a/build.sh b/build.sh
new file mode 100644
index 0000000000..c9462cc9d6
--- /dev/null
+++ b/build.sh
@@ -0,0 +1,44 @@
+#!/bin/sh
+
+echo "U-Boot build menu, please select your choice:"
+echo "  1   make distclean"
+echo "  2   config for mx28"
+echo "  3   build u-boot"
+echo "  4   make u-boot.sb"
+echo "  q   exit"
+
+source ./setenv.sh
+NUM_CPUS=$(cat /proc/cpuinfo | grep -c processor)
+TFTPBOOT=/home/work/tftpboot
+
+make_distclean() {
+    echo "make distclean"
+    make distclean
+}
+
+config_mx28() {
+    make mx28evk_defconfig
+}
+
+build_uboot() {
+    #make -j${NUM_CPUS} u-boot.sb
+    make -j${NUM_CPUS} spl/u-boot-spl
+    make -j${NUM_CPUS} u-boot
+}
+
+make_uboot_sb() {
+    elftosb -f imx28 -c arch/arm/cpu/arm926ejs/mxs/u-boot-imx28.bd -o u-boot.sb
+}
+
+read sel
+echo "select $sel"
+
+case $sel in
+    [1]) make_distclean;;
+    [2]) config_mx28;;
+    [3]) build_uboot;;
+    [4]) make_uboot_sb;;
+    [q]) exit 0;;
+    *)   echo "ONLY accept 1/2/3/4/q/Q"
+esac    
+
diff --git a/configs/mx28evk_defconfig b/configs/mx28evk_defconfig
index d660a7e92d..9a4336650f 100644
--- a/configs/mx28evk_defconfig
+++ b/configs/mx28evk_defconfig
@@ -12,13 +12,15 @@ CONFIG_NR_DRAM_BANKS=1
 CONFIG_SPL=y
 CONFIG_SPL_TEXT_BASE=0x00001000
 CONFIG_FIT=y
+CONFIG_BOOTDELAY=3
 # CONFIG_CONSOLE_MUX is not set
 CONFIG_SYS_CONSOLE_IS_IN_ENV=y
 CONFIG_VERSION_VARIABLE=y
-# CONFIG_DISPLAY_BOARDINFO is not set
+CONFIG_DISPLAY_BOARDINFO=y
 CONFIG_ARCH_MISC_INIT=y
 # CONFIG_SPL_FRAMEWORK is not set
 CONFIG_HUSH_PARSER=y
+CONFIG_SYS_PROMPT="zlg@imx280a# "
 CONFIG_CMD_BOOTZ=y
 # CONFIG_CMD_FLASH is not set
 CONFIG_CMD_GPIO=y
@@ -55,5 +57,5 @@ CONFIG_USB_STORAGE=y
 CONFIG_USB_HOST_ETHER=y
 CONFIG_USB_ETHER_ASIX=y
 CONFIG_USB_ETHER_SMSC95XX=y
-CONFIG_VIDEO=y
+# CONFIG_VIDEO is not set
 CONFIG_OF_LIBFDT=y
diff --git a/configs/mx28evk_nand_defconfig b/configs/mx28evk_nand_defconfig
index 7522bd0626..e048c8dc05 100644
--- a/configs/mx28evk_nand_defconfig
+++ b/configs/mx28evk_nand_defconfig
@@ -10,15 +10,17 @@ CONFIG_TARGET_MX28EVK=y
 CONFIG_SPL_SERIAL_SUPPORT=y
 CONFIG_NR_DRAM_BANKS=1
 CONFIG_SPL=y
+CONFIG_BOOTDELAY=3
 CONFIG_ENV_OFFSET_REDUND=0x380000
 CONFIG_SPL_TEXT_BASE=0x00001000
 # CONFIG_CONSOLE_MUX is not set
 CONFIG_SYS_CONSOLE_IS_IN_ENV=y
 CONFIG_VERSION_VARIABLE=y
-# CONFIG_DISPLAY_BOARDINFO is not set
+CONFIG_DISPLAY_BOARDINFO=y
 CONFIG_ARCH_MISC_INIT=y
 # CONFIG_SPL_FRAMEWORK is not set
 CONFIG_HUSH_PARSER=y
+CONFIG_SYS_PROMPT="zlg@imx280a# "
 CONFIG_CMD_BOOTZ=y
 # CONFIG_CMD_FLASH is not set
 CONFIG_CMD_GPIO=y
@@ -56,5 +58,5 @@ CONFIG_USB_STORAGE=y
 CONFIG_USB_HOST_ETHER=y
 CONFIG_USB_ETHER_ASIX=y
 CONFIG_USB_ETHER_SMSC95XX=y
-CONFIG_VIDEO=y
+# CONFIG_VIDEO is not set
 CONFIG_OF_LIBFDT=y
diff --git a/include/common.h b/include/common.h
index f0679ac705..12e36e9679 100644
--- a/include/common.h
+++ b/include/common.h
@@ -33,4 +33,6 @@
 # include <env_internal.h>
 #endif
 
+//#define DEBUG 1
+
 #endif	/* __COMMON_H_ */
diff --git a/include/configs/mx28evk.h b/include/configs/mx28evk.h
index 15b64ee03e..732e132c0d 100644
--- a/include/configs/mx28evk.h
+++ b/include/configs/mx28evk.h
@@ -15,7 +15,11 @@
 
 /* Memory configuration */
 #define PHYS_SDRAM_1			0x40000000	/* Base address */
+#if 0 /*CONFIG_IMX280A*/
 #define PHYS_SDRAM_1_SIZE		0x40000000	/* Max 1 GB RAM */
+#else
+#define PHYS_SDRAM_1_SIZE		0x04000000	/* Max 64 MB RAM */
+#endif
 #define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
 
 /* Environment */
diff --git a/include/configs/mxs.h b/include/configs/mxs.h
index 325c3ee00c..1675d3f852 100644
--- a/include/configs/mxs.h
+++ b/include/configs/mxs.h
@@ -118,8 +118,13 @@
 /* NAND */
 #ifdef CONFIG_CMD_NAND
 #define CONFIG_SYS_MAX_NAND_DEVICE	1
+#if 0 /*CONFIG_IMX280A*/
 #define CONFIG_SYS_NAND_BASE		0x60000000
 #define CONFIG_SYS_NAND_5_ADDR_CYCLE
+#else
+#define CONFIG_SYS_NAND_BASE		0x8000C000
+#define CONFIG_SYS_NAND_4_ADDR_CYCLE
+#endif
 #endif
 
 /* OCOTP */
diff --git a/setenv.sh b/setenv.sh
new file mode 100644
index 0000000000..6bb8d921a8
--- /dev/null
+++ b/setenv.sh
@@ -0,0 +1,11 @@
+#!/bin/bash
+
+
+export WORK_BASE=/home/work/easyarm280
+export ARCH=arm
+export CROSS_COMPILE=arm-926ejs-linux-gnueabi-
+#export CROSS_COMPILE=arm-fsl-linux-gnueabi-
+export CROSS_COMPILE_DIR=${WORK_BASE}/toolchain/arm-926ejs-linux-gnueabi/bin
+#export CROSS_COMPILE_DIR=${WORK_BASE}/toolchain/arm-fsl-linux-gnueabi/bin
+export PATH=${CROSS_COMPILE_DIR}:$PATH
+
-- 
2.17.1


```



# 烧写和启动

> 烧写镜像到sd卡并启动
```bash
.\cfimager.exe -a -f .\u-boot.sb -d h
```

> bootlog

```
SPL: Serial Console Initialised
SPL: Initialising Power Block
SPL: Switching CPU clock to 24MHz XTAL
SPL: Setting VDDD 25mV below DC-DC converters
SPL: Setting VDDA 25mV below DC-DC converters
SPL: Setting VDDIO 25mV below DC-DC converters
SPL: Starting 5V input detection comparator
SPL: Starting battery voltage measurement logic
SPL: Initialisating LRADC
SPL: Enabling LRADC battery measurement
SPL: LRADC channel 7 is present - configuring
SPL: LRADC channel 7 configuration complete
SPL: Configuring power source
SPL: Using default DC-DC clocksource
SPL: Pre-Configuring power block
SPL: Configuring power block to boot from battery
SPL: Setting VDDD 25mV below DC-DC converters
SPL: Setting VDDA 25mV below DC-DC converters
SPL: Setting VDDIO 25mV below DC-DC converters
SPL: Switching CPU core clock source to PLL
SPL: Initialising battery brown-out level to 3.0V
SPL: Switching VDDD to DC-DC converters
SPL: Enabling output rail protection
SPL: Setting VDDIO to 3V3 (brownout @ 3v15)
SPL: Setting VDDD to 1V55 (brownout @ 1v400)
SPL: Initialising FRAC0
SPL: FRAC0 Initialised
SPL: Configuring VDDA
SPL: Initialising mx28 SDRAM Controller
SPL: Setting mx28 board specific SDRAM parameters
SPL: Applying SDRAM parameters
SPL: Setting CPU and HBUS clock frequencies
initcall: 40066c4c


U-Boot 2020.07-rc4 (Jul 03 2020 - 22:13:58 +0800)

initcall: 400146c4
U-Boot code: 40002000 -> 40092E84  BSS: -> 400B5D40
initcall: 40014614
initcall: 40002a28
CPU:   Freescale i.MX28 rev1.2 at 454 MHz
BOOT:  SSP SD/MMC #0, 3V3
initcall: 40014dfc
initcall: 40014800
DRAM:  initcall: 40003f64
initcall: 40014a5c
Monitor len: 000B3D40
Ram size: 04000000
Ram top: 44000000
initcall: 400145e4
initcall: 40003c08
TLB table from 43ff0000 to 43ff4000
initcall: 40014868
initcall: 40014870
initcall: 4001465c
Reserving 719k for U-Boot at: 43f3c000
initcall: 400148b8
Reserving 4112k for malloc() at: 43b38000
initcall: 400147a8
Reserving 80 Bytes for Board Info at: 43b37fb0
initcall: 400145fc
initcall: 400148f0
Reserving 176 Bytes for Global Data at: 43b37f00
initcall: 40014928
initcall: 40014878
initcall: 40014880
initcall: 400148a8
initcall: 40014ac8
initcall: 4001461c
initcall: 400149c0

RAM Configuration:
Bank #0: 40000000 64 MiB

DRAM:  64 MiB
initcall: 40014640
New Stack Pointer is: 43b37ee0
initcall: 40014764
initcall: 40014888
initcall: 40014890
initcall: 400146f4
Relocation Offset is: 03f3a000
Relocating to 43f3c000, new gd at 43b37f00, sp at 43b37ee0
initcall: 40014858
initcall: 40014b78
initcall: 40014b80
initcall: 3c0dad0c (relocated to 40014d0c)
dram_bank_mmu_setup: bank: 0
initcall: 3c0dab98 (relocated to 40014b98)
initcall: 3c0dad24 (relocated to 40014d24)
initcall: 3c0dace8 (relocated to 40014ce8)
using memory 0x43b38000-0x43f3c000 for malloc()
initcall: 3c0da5b4 (relocated to 400145b4)
initcall: 3c0dacd4 (relocated to 40014cd4)
initcall: 3c0dad2c (relocated to 40014d2c)
initcall: 3c0c9f68 (relocated to 40003f68)
initcall: 3c0dad1c (relocated to 40014d1c)
initcall: 3c0dad34 (relocated to 40014d34)
initcall: 3c0e7280 (relocated to 40021280)
initcall: 3c0dacc4 (relocated to 40014cc4)
initcall: 3c0daca8 (relocated to 40014ca8)
Now running in RAM - U-Boot at: 43f3c000
initcall: 3c0dad3c (relocated to 40014d3c)
initcall: 3c0dac78 (relocated to 40014c78)
NAND:  use legacy bch geometry
128 MiB
initcall: 3c0dac58 (relocated to 40014c58)
MMC:   MXS MMC: 0
initcall: 3c0dac28 (relocated to 40014c28)
Loading Environment from MMC... clock is disabled (0Hz)
MMC0: Set 0 bits bus width
SPI0: Set freq rate to 400 KHz (requested 400 KHz)
MMC0: Set 1 bits bus width
clock is enabled (400000Hz)
SPI0: Set freq rate to 400 KHz (requested 400 KHz)
MMC0: Set 1 bits bus width
MMC0: CMD0
MMC0: CMD8
MMC0: CMD55
MMC0: CMD41
MMC0: CMD55
MMC0: CMD41
MMC0: CMD2
MMC0: CMD3
MMC0: CMD9
MMC0: CMD7
MMC0: CMD55
MMC0: CMD51
MMC0: CMD6
MMC0: CMD55
MMC0: CMD6
SPI0: Set freq rate to 400 KHz (requested 400 KHz)
MMC0: Set 4 bits bus width
MMC0: CMD6
clock is enabled (48000000Hz)
SPI0: Set freq rate to 48000 KHz (requested 48000 KHz)
MMC0: Set 4 bits bus width
MMC0: CMD55
MMC0: CMD13
MMC0: CMD16
MMC0: CMD17
part_init: try 'DOS': ret=0
MMC0: CMD16
MMC0: CMD18
MMC0: CMD12
*** Warning - bad CRC, using default environment

Using default environment
Selecting environment with bad CRC
initcall: 3c0dad48 (relocated to 40014d48)
initcall: 3c0e729c (relocated to 4002129c)
initcall: 3c0dac18 (relocated to 40014c18)
initcall: 3c0e1da4 (relocated to 4001bda4)
In:    serial
Out:   serial
Err:   serial
initcall: 3c0c891c (relocated to 4000291c)
initcall: 3c0c9804 (relocated to 40003804)
initcall: 3c0dabf8 (relocated to 40014bf8)
initcall: 3c0dabdc (relocated to 40014bdc)
Net:   eth_init: fec_probe(bd, 0, 0) @ 800f0000
fec_mii_setspeed: mii_speed 0000013e
fec_mii_setspeed: mii_speed 0000013e
Initial value for argc=3
Final value for argc=3
got MAC0 address from fuse: 00:04:29:2a:ae:02
Initial value for argc=3
Final value for argc=3
FEC0 [PRIME]
initcall: 3c0dabd0 (relocated to 40014bd0)
Initial value for argc=3
Final value for argc=3
### main_loop entered: bootdelay=3

### main_loop: bootcmd="mmc dev ${mmcdev}; if mmc rescan; then if run loadbootscript; then run bootscript; else if run loadimage; then run mmcboot; else run netboot; fi; fi; else run netboot; fi"
Hit any key to stop autoboot:  0 
zlg@imx280a# version 
U-Boot 2020.07-rc4 (Jul 03 2020 - 22:13:58 +0800)

arm-926ejs-linux-gnueabi-gcc (crosstool-NG 1.24.0.105_5659366) 9.2.0
GNU ld (crosstool-NG 1.24.0.105_5659366) 2.34

zlg@imx280a# bdinfo 
arch_number = 0x000009e3
boot_params = 0x40000100
DRAM bank   = 0x00000000
-> start    = 0x40000000
-> size     = 0x04000000
eth0name    = FEC0
ethaddr     = 00:04:29:2a:ae:02
current eth = FEC0
ip_addr     = 192.168.1.101
baudrate    = 115200 bps
TLB addr    = 0x43ff0000
relocaddr   = 0x43f3c000
reloc off   = 0x03f3a000
irq_sp      = 0x43b37ef0
sp start    = 0x43b37ee0
Build       = 32-bit

zlg@imx280a# mmcinfo 
Device: MXS MMC
Manufacturer ID: 82
OEM: 4a54
Name: NCard 
Bus Speed: 48000000
Mode: SD High Speed (50MHz)
Rd Block Len: 512
SD version 3.0
High Capacity: Yes
Capacity: 7.5 GiB
Bus Width: 4-bit
Erase Group Size: 512 Bytes

zlg@imx280a# nand info
Device 0: nand0, sector size 128 KiB
  Page size       2048 b
  OOB size          64 b
  Erase size    131072 b
  subpagesize     2048 b
  options     0x00000200
  bbt options 0x00008000
Initial value for argc=3
Final value for argc=3
Initial value for argc=3
Final value for argc=3
Initial value for argc=3
Final value for argc=3

zlg@imx280a# ping 192.168.1.100
eth_halt: wait for stop regs
eth_halt: done
Trying FEC0
fec_mii_setspeed: mii_speed 0000013e
fec_mdio_write: phy: 00 reg:00 val:0x8000
fec_mdio_write: phy: 00 reg:04 val:0x1e1
fec_mdio_write: phy: 00 reg:00 val:0x1200
fec_open: fec_open(dev)
fec_mdio_read: phy: 00 reg:01 val:0xffff
fec_mdio_read: phy: 00 reg:00 val:0xffff
fec_mdio_read: phy: 00 reg:05 val:0xffff
fec_mdio_read: phy: 00 reg:04 val:0xffff
fec_mdio_read: phy: 00 reg:00 val:0xffff
fec_mdio_read: phy: 00 reg:05 val:0xffff
fec_mdio_read: phy: 00 reg:04 val:0xffff
fec_open:Speed=100
Using FEC0 device
fec_send: status 0xc00 index 0 ret 0
fec_recv: ievent 0xe000000
fec_recv: status 0x800
fec_send: status 0x2c00 index 1 ret 0
fec_recv: stop
fec_recv: ievent 0xc000000
fec_recv: status 0x800
fec_send: status 0xc00 index 0 ret 0
fec_recv: stop
fec_recv: ievent 0xe000000
fec_recv: status 0x800
fec_recv: stop
eth_halt: wait for stop regs
eth_halt: done
host 192.168.1.100 is alive
zlg@imx280a# 
```
