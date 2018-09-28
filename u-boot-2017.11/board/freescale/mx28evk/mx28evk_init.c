#include <common.h>
#include <config.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/iomux-mx28.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/sys_proto.h>

//#include "mxs_init.h"

#include "mx28evk_init.h"

#define _IOMUX_SETUP_

#ifdef _IOMUX_SETUP_

#define	MUX_CONFIG_SSP0	(MXS_PAD_3V3 | MXS_PAD_8MA | MXS_PAD_PULLUP)
#define	MUX_CONFIG_GPMI	(MXS_PAD_3V3 | MXS_PAD_4MA | MXS_PAD_NOPULL)
#define	MUX_CONFIG_ENET	(MXS_PAD_3V3 | MXS_PAD_8MA | MXS_PAD_PULLUP)
#define	MUX_CONFIG_EMI	(MXS_PAD_3V3 | MXS_PAD_12MA | MXS_PAD_NOPULL)
#define	MUX_CONFIG_SSP2	(MXS_PAD_3V3 | MXS_PAD_4MA | MXS_PAD_PULLUP)
#define	MUX_CONFIG_LCD	(MXS_PAD_3V3 | MXS_PAD_4MA | MXS_PAD_NOPULL)

const iomux_cfg_t mx28evk_iomux_setup[] = {
	/* DUART */
	MX28_PAD_PWM0__DUART_RX,
	MX28_PAD_PWM1__DUART_TX,

	/* MMC0 */
	MX28_PAD_SSP0_DATA0__SSP0_D0 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DATA1__SSP0_D1 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DATA2__SSP0_D2 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DATA3__SSP0_D3 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DATA4__SSP0_D4 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DATA5__SSP0_D5 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DATA6__SSP0_D6 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DATA7__SSP0_D7 | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_CMD__SSP0_CMD | MUX_CONFIG_SSP0,
	MX28_PAD_SSP0_DETECT__SSP0_CARD_DETECT |
		(MXS_PAD_8MA | MXS_PAD_3V3 | MXS_PAD_NOPULL),
	MX28_PAD_SSP0_SCK__SSP0_SCK |
		(MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_NOPULL),
	#if 0
	/* write protect */
	MX28_PAD_SSP1_SCK__GPIO_2_12,
	/* MMC0 slot power enable */
	MX28_PAD_PWM3__GPIO_3_28 |
		(MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP),
	#else
	MX28_PAD_GPMI_CE1N__GPIO_0_17 | MUX_CONFIG_SSP0,
	#endif

#ifdef CONFIG_NAND_MXS
	/* GPMI NAND */
	MX28_PAD_GPMI_D00__GPMI_D0 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_D01__GPMI_D1 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_D02__GPMI_D2 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_D03__GPMI_D3 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_D04__GPMI_D4 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_D05__GPMI_D5 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_D06__GPMI_D6 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_D07__GPMI_D7 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_CE0N__GPMI_CE0N | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_RDY0__GPMI_READY0 | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_RDN__GPMI_RDN |
		(MXS_PAD_3V3 | MXS_PAD_8MA | MXS_PAD_PULLUP),
	MX28_PAD_GPMI_WRN__GPMI_WRN | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_ALE__GPMI_ALE | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_CLE__GPMI_CLE | MUX_CONFIG_GPMI,
	MX28_PAD_GPMI_RESETN__GPMI_RESETN | MUX_CONFIG_GPMI,
#endif

	/* FEC0 */
	MX28_PAD_ENET0_MDC__ENET0_MDC | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_MDIO__ENET0_MDIO | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_RX_EN__ENET0_RX_EN | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_TX_EN__ENET0_TX_EN | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_RXD0__ENET0_RXD0 | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_RXD1__ENET0_RXD1 | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_TXD0__ENET0_TXD0 | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_TXD1__ENET0_TXD1 | MUX_CONFIG_ENET,
	MX28_PAD_ENET_CLK__CLKCTRL_ENET | MUX_CONFIG_ENET,
	#if 0
	/* FEC0 Enable */
	MX28_PAD_SSP1_DATA3__GPIO_2_15 |
		(MXS_PAD_12MA | MXS_PAD_3V3),
	/* FEC0 Reset */
	MX28_PAD_ENET0_RX_CLK__GPIO_4_13 |
		(MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP),
	#else
	MX28_PAD_LCD_D16__GPIO_1_16 | (MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP),
	#endif

	/* FEC1 */
	#if 0
	MX28_PAD_ENET0_COL__ENET1_TX_EN | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_CRS__ENET1_RX_EN | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_RXD2__ENET1_RXD0 | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_RXD3__ENET1_RXD1 | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_TXD2__ENET1_TXD0 | MUX_CONFIG_ENET,
	MX28_PAD_ENET0_TXD3__ENET1_TXD1 | MUX_CONFIG_ENET,
	#endif

	/* EMI */
	MX28_PAD_EMI_D00__EMI_DATA0 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D01__EMI_DATA1 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D02__EMI_DATA2 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D03__EMI_DATA3 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D04__EMI_DATA4 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D05__EMI_DATA5 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D06__EMI_DATA6 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D07__EMI_DATA7 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D08__EMI_DATA8 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D09__EMI_DATA9 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D10__EMI_DATA10 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D11__EMI_DATA11 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D12__EMI_DATA12 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D13__EMI_DATA13 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D14__EMI_DATA14 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_D15__EMI_DATA15 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_ODT0__EMI_ODT0 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_DQM0__EMI_DQM0 | MUX_CONFIG_EMI,
	#if 0
	MX28_PAD_EMI_ODT1__EMI_ODT1 | MUX_CONFIG_EMI,
	#endif
	MX28_PAD_EMI_DQM1__EMI_DQM1 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_DDR_OPEN_FB__EMI_DDR_OPEN_FEEDBACK | MUX_CONFIG_EMI,
	MX28_PAD_EMI_CLK__EMI_CLK | MUX_CONFIG_EMI,
	MX28_PAD_EMI_DQS0__EMI_DQS0 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_DQS1__EMI_DQS1 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_DDR_OPEN__EMI_DDR_OPEN | MUX_CONFIG_EMI,

	MX28_PAD_EMI_A00__EMI_ADDR0 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A01__EMI_ADDR1 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A02__EMI_ADDR2 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A03__EMI_ADDR3 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A04__EMI_ADDR4 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A05__EMI_ADDR5 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A06__EMI_ADDR6 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A07__EMI_ADDR7 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A08__EMI_ADDR8 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A09__EMI_ADDR9 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A10__EMI_ADDR10 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A11__EMI_ADDR11 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A12__EMI_ADDR12 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A13__EMI_ADDR13 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_A14__EMI_ADDR14 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_BA0__EMI_BA0 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_BA1__EMI_BA1 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_BA2__EMI_BA2 | MUX_CONFIG_EMI,
	MX28_PAD_EMI_CASN__EMI_CASN | MUX_CONFIG_EMI,
	MX28_PAD_EMI_RASN__EMI_RASN | MUX_CONFIG_EMI,
	MX28_PAD_EMI_WEN__EMI_WEN | MUX_CONFIG_EMI,
	MX28_PAD_EMI_CE0N__EMI_CE0N | MUX_CONFIG_EMI,
	#if 0
	MX28_PAD_EMI_CE1N__EMI_CE1N | MUX_CONFIG_EMI,
	#endif
	MX28_PAD_EMI_CKE__EMI_CKE | MUX_CONFIG_EMI,

	/* SPI2 (for SPI flash) */
	#if 0
	MX28_PAD_SSP2_SCK__SSP2_SCK | MUX_CONFIG_SSP2,
	MX28_PAD_SSP2_MOSI__SSP2_CMD | MUX_CONFIG_SSP2,
	MX28_PAD_SSP2_MISO__SSP2_D0 | MUX_CONFIG_SSP2,
	MX28_PAD_SSP2_SS0__SSP2_D3 |
		(MXS_PAD_3V3 | MXS_PAD_8MA | MXS_PAD_PULLUP),
	#endif
	/* I2C */
	MX28_PAD_I2C0_SCL__I2C0_SCL,
	MX28_PAD_I2C0_SDA__I2C0_SDA,
	#if 1
	MX28_PAD_PWM0__I2C1_SCL,
	MX28_PAD_PWM1__I2C1_SDA,
	#endif

	/* LCD */
	MX28_PAD_LCD_D00__LCD_D0 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D01__LCD_D1 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D02__LCD_D2 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D03__LCD_D3 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D04__LCD_D4 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D05__LCD_D5 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D06__LCD_D6 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D07__LCD_D7 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D08__LCD_D8 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D09__LCD_D9 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D10__LCD_D10 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D11__LCD_D11 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D12__LCD_D12 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D13__LCD_D13 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D14__LCD_D14 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D15__LCD_D15 | MUX_CONFIG_LCD,
	#if 0
	MX28_PAD_LCD_D16__LCD_D16 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D17__LCD_D17 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D18__LCD_D18 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D19__LCD_D19 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D20__LCD_D20 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D21__LCD_D21 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D22__LCD_D22 | MUX_CONFIG_LCD,
	MX28_PAD_LCD_D23__LCD_D23 | MUX_CONFIG_LCD,
	#endif
	MX28_PAD_LCD_RD_E__LCD_VSYNC | MUX_CONFIG_LCD,
	MX28_PAD_LCD_WR_RWN__LCD_HSYNC | MUX_CONFIG_LCD,
	MX28_PAD_LCD_RS__LCD_DOTCLK | MUX_CONFIG_LCD,
	MX28_PAD_LCD_CS__LCD_ENABLE | MUX_CONFIG_LCD,
	MX28_PAD_LCD_RESET__GPIO_3_30 | MUX_CONFIG_LCD, /* LCD power */
	#if 0
	MX28_PAD_PWM2__GPIO_3_18 | MUX_CONFIG_LCD, /* LCD contrast */
	#else
	MX28_PAD_PWM3__GPIO_3_28 | MUX_CONFIG_LCD,
	#endif
};

#endif

#define	MUX_CONFIG_BOOTMODE_PAD	(MXS_PAD_3V3 | MXS_PAD_4MA | MXS_PAD_NOPULL)
const iomux_cfg_t mx28evk_iomux_boot[] = {
	MX28_PAD_LCD_D00__GPIO_1_0 | MUX_CONFIG_BOOTMODE_PAD,
	MX28_PAD_LCD_D01__GPIO_1_1 | MUX_CONFIG_BOOTMODE_PAD,
	MX28_PAD_LCD_D02__GPIO_1_2 | MUX_CONFIG_BOOTMODE_PAD,
	MX28_PAD_LCD_D03__GPIO_1_3 | MUX_CONFIG_BOOTMODE_PAD,
	MX28_PAD_LCD_D04__GPIO_1_4 | MUX_CONFIG_BOOTMODE_PAD,
	MX28_PAD_LCD_D05__GPIO_1_5 | MUX_CONFIG_BOOTMODE_PAD,
};

//copy from mxs_get_bootmode_index
static uint8_t mx28evk_get_bootmode_index(void)
{
	uint8_t bootmode = 0;
	int i;
	uint8_t masked;

#if defined(CONFIG_MX23)
	/* Setup IOMUX of bootmode pads to GPIO */
	mxs_iomux_setup_multiple_pads(mx28evk_iomux_boot, ARRAY_SIZE(mx28evk_iomux_boot));

	/* Setup bootmode pins as GPIO input */
	gpio_direction_input(MX23_PAD_LCD_D00__GPIO_1_0);
	gpio_direction_input(MX23_PAD_LCD_D01__GPIO_1_1);
	gpio_direction_input(MX23_PAD_LCD_D02__GPIO_1_2);
	gpio_direction_input(MX23_PAD_LCD_D03__GPIO_1_3);
	gpio_direction_input(MX23_PAD_LCD_D05__GPIO_1_5);

	/* Read bootmode pads */
	bootmode |= (gpio_get_value(MX23_PAD_LCD_D00__GPIO_1_0) ? 1 : 0) << 0;
	bootmode |= (gpio_get_value(MX23_PAD_LCD_D01__GPIO_1_1) ? 1 : 0) << 1;
	bootmode |= (gpio_get_value(MX23_PAD_LCD_D02__GPIO_1_2) ? 1 : 0) << 2;
	bootmode |= (gpio_get_value(MX23_PAD_LCD_D03__GPIO_1_3) ? 1 : 0) << 3;
	bootmode |= (gpio_get_value(MX23_PAD_LCD_D05__GPIO_1_5) ? 1 : 0) << 5;
#elif defined(CONFIG_MX28)
	/* The global boot mode will be detected by ROM code and its value
	 * is stored at the fixed address 0x00019BF0 in OCRAM.
	 */
#define GLOBAL_BOOT_MODE_ADDR 0x00019BF0
	bootmode = __raw_readl(GLOBAL_BOOT_MODE_ADDR);
#endif

	for (i = 0; i < ARRAY_SIZE(mxs_boot_modes); i++) {
		masked = bootmode & mxs_boot_modes[i].boot_mask;
		if (masked == mxs_boot_modes[i].boot_pads)
			break;
	}

	return i;
}

//#define _DDR_SIZE_GET_
#ifdef _DDR_SIZE_GET_

#define EMI_DDR2_SIZE_BASE              0x0000F100
#define EMI_DDR2_SIZE_BASE_ANOTHER      0x0000F400

static uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
    uint32_t crc = crcIn;
    uint32_t in = byte | 0x100;
    do
    {
        crc <<= 1;
        in <<= 1;
        if (in & 0x100)
            ++crc;
        if (crc & 0x10000)
            crc ^= 0x1021;
    }
    while (!(in & 0x10000));
    return crc & 0xffffu;
}

static uint16_t Cal_CRC16(const char *data, int size)
{
    int crc = 0;
    const char *dataEnd = data + size;
    while (data < dataEnd)
        crc = UpdateCRC16(crc, *data++);

    crc = UpdateCRC16(crc, 0);
    crc = UpdateCRC16(crc, 0);
    return crc & 0xffffu;
}

static int ddr2SizeDetect(void)
{
    char *pDDR2Size1 = (char *)EMI_DDR2_SIZE_BASE;
    char *pDDR2Size2 = (char *)EMI_DDR2_SIZE_BASE_ANOTHER;
    uint16_t crc, length;

    if (0xFE == *pDDR2Size1)
    {
        length = *(pDDR2Size1 + 1);
        if (length > 255)
        {
            length = 10;
        }
        crc = *(pDDR2Size1 + 2 + length);
        crc <<= 8;
        crc += *(pDDR2Size1 + 2 + length + 1);
        if (crc  == Cal_CRC16(pDDR2Size1, length + 2))
        {
            pDDR2Size1 += 2;
            if (strcmp(pDDR2Size1, "64M") == 0)
            {
                return 64;
            }
            else if (strcmp(pDDR2Size1, "128M") == 0)
            {
                return 128;
            }
        }
    }

    if (0xFE == *pDDR2Size2)
    {
        length = *(pDDR2Size2 + 1);
        if (length > 255)
        {
            length = 10;
        }
        crc = *(pDDR2Size2 + 2 + length);
        crc <<= 8;
        crc += *(pDDR2Size2 + 2 + length + 1);
        pDDR2Size2 += 2;
        if (strcmp(pDDR2Size2, "64M") == 0)
        {
            return 64;
        }
        else if (strcmp(pDDR2Size2, "128M") == 0)
        {
            return 128;
        }
    }

    return -1;
}
#endif

static uint32_t mx28evk_mem_get_size(void)
{
    uint32_t size = 0;
    #ifdef _DDR_SIZE_GET_
    int iSizeMb = ddr2SizeDetect();

	if (iSizeMb == 128)
    {
		size = PHYS_SDRAM_1_SIZE;
	} 
    else
    {
		size = PHYS_SDRAM_1_SIZE >> 1;
	}
    #else
    size = (PHYS_SDRAM_1_SIZE >> 1);//64M
    #endif
	return size;
}

uint8_t mx28evk_bootmode_set(void)
{
    uint8_t bootmode = 0;
    struct mxs_spl_data *data = (struct mxs_spl_data *)
		((CONFIG_SYS_TEXT_BASE - sizeof(struct mxs_spl_data)) & ~0xf);

    bootmode = mx28evk_get_bootmode_index();

    data->boot_mode_idx = bootmode;

    return bootmode;
}

uint32_t mx28evk_memsize_set(void)
{
    uint32_t memsize = 0;
    struct mxs_spl_data *data = (struct mxs_spl_data *)
		((CONFIG_SYS_TEXT_BASE - sizeof(struct mxs_spl_data)) & ~0xf);

    memsize = mx28evk_mem_get_size();

    data->mem_dram_size = memsize;

    return memsize;
}

//copy from mxs_common_spl_init
static void mx28evk_common_init(const iomux_cfg_t *iomux_setup,
			const unsigned int iomux_size)
{
    #ifdef _IOMUX_SETUP_
	mxs_iomux_setup_multiple_pads(iomux_setup, iomux_size);
    #endif

	mx28evk_bootmode_set();
	mx28evk_memsize_set();
}

int mx28evk_board_init(void)
{
    #ifdef _IOMUX_SETUP_
	mx28evk_common_init(mx28evk_iomux_setup, ARRAY_SIZE(mx28evk_iomux_setup));
    #else
    mx28evk_common_init(NULL, 0);
    #endif

    return 0;
}
