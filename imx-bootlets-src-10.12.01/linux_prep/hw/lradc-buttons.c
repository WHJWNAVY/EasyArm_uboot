/*
 * Low-level LRADC keypad driver
 *
 * Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 SigmaTel, Inc
 * Copyright 2008 Embedded Alley Solutions, Inc
 * Copyright 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program  is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#include <stmp3xxx.h>
#include <keys.h>
#include <lradc_buttons.h>
#include <init.h>

#define TARGET_VDDIO_LRADC_VAL	3350
#define LRADC_NOISE_MARGIN	100

static u32 lradc_get_ch_value (int channel);

static u32 lradc_decode_button (int raw_button)
{
	struct lradc_keycode *codes = lradc_keycodes;

	while(codes->raw != -1) {
		if (raw_button > (codes->raw - LRADC_NOISE_MARGIN) &&
		    raw_button < (codes->raw + LRADC_NOISE_MARGIN))
			return codes->kcode;
		codes++;
	}

	return NO_KEY;
}

static u32 lradc_read_button (void)
{
	u32 raw_button, normalized_raw_button;
	u32 vddio;

	vddio = lradc_get_ch_value(VDDIO_VOLTAGE_CH);
	raw_button = lradc_get_ch_value(lradc_keypad_ch);
	normalized_raw_button = (raw_button * TARGET_VDDIO_LRADC_VAL)/vddio;

	return lradc_decode_button(normalized_raw_button);
}

static u32 lradc_get_ch_value (int channel)
{
	u32 channel_mask = (1 << channel);

	/* Clear the interrupt flag */
	HW_LRADC_CTRL1_CLR(channel_mask);

	/* Kick of a conversion */
	HW_LRADC_CTRL0_SET(BF_LRADC_CTRL0_SCHEDULE(channel_mask));

	/* wait for completion */
	while ((HW_LRADC_CTRL1_RD() & channel_mask) != channel_mask) ;

	/* Clear the interrupt flag */
	HW_LRADC_CTRL1_CLR(channel_mask);

	/* read ch 6 value. */
	return HW_LRADC_CHn_RD(channel) & BM_LRADC_CHn_VALUE;
}

static void lradc_keypad_hwinit ()
{
	/* Clear the Clock Gate and SFTRST for normal operation */
	HW_LRADC_CTRL0_CLR(BM_LRADC_CTRL0_SFTRST);
	HW_LRADC_CTRL0_CLR(BM_LRADC_CTRL0_CLKGATE);

	/* Disable on-chip ground reference */
	HW_LRADC_CTRL0_CLR(BM_LRADC_CTRL0_ONCHIP_GROUNDREF);

	/* Configure 6Mhz frequency */
	HW_LRADC_CTRL3_CLR(BM_LRADC_CTRL3_CYCLE_TIME);
	HW_LRADC_CTRL3_SET(BF_LRADC_CTRL3_CYCLE_TIME(LRADC_CLOCK_6MHZ));

	/* Select VddIO input on channel 6 */
	HW_LRADC_CTRL4_CLR(BM_LRADC_CTRL4_LRADC6SELECT);
	HW_LRADC_CTRL4_SET(BF_LRADC_CTRL4_LRADC6SELECT(lradc_vddio_ch));

	/*
	 * Clear the divide by two for channel 6 since it has a HW
	 * divide-by-two built in, and enable this feature for the
	 * button channel
	 */
	HW_LRADC_CTRL2_CLR(BF_LRADC_CTRL2_DIVIDE_BY_TWO(1<<VDDIO_VOLTAGE_CH));
	HW_LRADC_CTRL2_SET(BF_LRADC_CTRL2_DIVIDE_BY_TWO(1<<lradc_keypad_ch));

	/* Clear the accumulator & NUM_SAMPLES */
	HW_LRADC_CHn_CLR(VDDIO_VOLTAGE_CH, 0xFFFFFFFF);
	HW_LRADC_CHn_CLR(lradc_keypad_ch, 0xFFFFFFFF);
}

struct keys_driver lradc_keys_driver = {
	.id		= KEYS_DRV_LRADC,
	.get_keys	= lradc_read_button,
};

static void lradc_keypad_init (void)
{
	lradc_keypad_hwinit();
	set_keys_driver(&lradc_keys_driver);
}

hw_initcall(lradc_keypad_init);
