/*
 * Copyright 2008-2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * DO NOT MANUAL EDIT THIS FILE!
 *
 */

#define HW_FUSE_SPDIF_DISABLE_ADDR             0x008       // addr=C0A0[1]     , bit=001:001
#define HW_FUSE_SPDIF_DISABLE_BM               0x00000002
#define HW_FUSE_TIMER0_DISABLE_ADDR            0x008       // addr=C0A0[2]     , bit=002:002
#define HW_FUSE_TIMER0_DISABLE_BM              0x00000004
#define HW_FUSE_TIMER1_DISABLE_ADDR            0x008       // addr=C0A0[3]     , bit=003:003
#define HW_FUSE_TIMER1_DISABLE_BM              0x00000008
#define HW_FUSE_TIMER2_DISABLE_ADDR            0x008       // addr=C0A0[4]     , bit=004:004
#define HW_FUSE_TIMER2_DISABLE_BM              0x00000010
#define HW_FUSE_TIMER3_DISABLE_ADDR            0x008       // addr=C0A0[5]     , bit=005:005
#define HW_FUSE_TIMER3_DISABLE_BM              0x00000020
#define HW_FUSE_ROT_DISABLE_ADDR               0x008       // addr=C0A0[6]     , bit=006:006
#define HW_FUSE_ROT_DISABLE_BM                 0x00000040
#define HW_FUSE_PWM0_DISABLE_ADDR              0x008       // addr=C0A0[7]     , bit=007:007
#define HW_FUSE_PWM0_DISABLE_BM                0x00000080
#define HW_FUSE_PWM1_DISABLE_ADDR              0x008       // addr=C0A0[8]     , bit=008:008
#define HW_FUSE_PWM1_DISABLE_BM                0x00000100
#define HW_FUSE_PWM2_DISABLE_ADDR              0x008       // addr=C0A0[9]     , bit=009:009
#define HW_FUSE_PWM2_DISABLE_BM                0x00000200
#define HW_FUSE_PWM3_DISABLE_ADDR              0x008       // addr=C0A0[10]    , bit=010:010
#define HW_FUSE_PWM3_DISABLE_BM                0x00000400
#define HW_FUSE_PWM4_DISABLE_ADDR              0x008       // addr=C0A0[11]    , bit=011:011
#define HW_FUSE_PWM4_DISABLE_BM                0x00000800
#define HW_FUSE_PWM5_DISABLE_ADDR              0x008       // addr=C0A0[12]    , bit=012:012
#define HW_FUSE_PWM5_DISABLE_BM                0x00001000
#define HW_FUSE_PWM6_DISABLE_ADDR              0x008       // addr=C0A0[13]    , bit=013:013
#define HW_FUSE_PWM6_DISABLE_BM                0x00002000
#define HW_FUSE_PWM7_DISABLE_ADDR              0x008       // addr=C0A0[14]    , bit=014:014
#define HW_FUSE_PWM7_DISABLE_BM                0x00004000
#define HW_FUSE_LRADC_TEMP0_DISABLE_ADDR       0x008       // addr=C0A0[16]    , bit=016:016
#define HW_FUSE_LRADC_TEMP0_DISABLE_BM         0x00010000
#define HW_FUSE_LRADC_TEMP1_DISABLE_ADDR       0x008       // addr=C0A0[17]    , bit=017:017
#define HW_FUSE_LRADC_TEMP1_DISABLE_BM         0x00020000
#define HW_FUSE_LRADC_BUTTON0_DISABLE_ADDR     0x008       // addr=C0A0[18]    , bit=018:018
#define HW_FUSE_LRADC_BUTTON0_DISABLE_BM       0x00040000
#define HW_FUSE_LRADC_BUTTON1_DISABLE_ADDR     0x008       // addr=C0A0[19]    , bit=019:019
#define HW_FUSE_LRADC_BUTTON1_DISABLE_BM       0x00080000
#define HW_FUSE_LRADC_TOUCH_DISABLE_ADDR       0x008       // addr=C0A0[20]    , bit=020:020
#define HW_FUSE_LRADC_TOUCH_DISABLE_BM         0x00100000
#define HW_FUSE_LRADC0_DISABLE_ADDR            0x008       // addr=C0A0[21]    , bit=021:021
#define HW_FUSE_LRADC0_DISABLE_BM              0x00200000
#define HW_FUSE_LRADC1_DISABLE_ADDR            0x008       // addr=C0A0[22]    , bit=022:022
#define HW_FUSE_LRADC1_DISABLE_BM              0x00400000
#define HW_FUSE_LRADC2_DISABLE_ADDR            0x008       // addr=C0A0[23]    , bit=023:023
#define HW_FUSE_LRADC2_DISABLE_BM              0x00800000
#define HW_FUSE_LRADC3_DISABLE_ADDR            0x008       // addr=C0A0[24]    , bit=024:024
#define HW_FUSE_LRADC3_DISABLE_BM              0x01000000
#define HW_FUSE_LRADC4_DISABLE_ADDR            0x008       // addr=C0A0[25]    , bit=025:025
#define HW_FUSE_LRADC4_DISABLE_BM              0x02000000
#define HW_FUSE_LRADC5_DISABLE_ADDR            0x008       // addr=C0A0[26]    , bit=026:026
#define HW_FUSE_LRADC5_DISABLE_BM              0x04000000
#define HW_FUSE_LRADC6_DISABLE_ADDR            0x008       // addr=C0A0[27]    , bit=027:027
#define HW_FUSE_LRADC6_DISABLE_BM              0x08000000
#define HW_FUSE_RTC_DISABLE_ADDR               0x008       // addr=C0A0[28]    , bit=028:028
#define HW_FUSE_RTC_DISABLE_BM                 0x10000000
#define HW_FUSE_RTC_ALARM_DISABLE_ADDR         0x008       // addr=C0A0[29]    , bit=029:029
#define HW_FUSE_RTC_ALARM_DISABLE_BM           0x20000000
#define HW_FUSE_RTC_WATCHDOG_DISABLE_ADDR      0x008       // addr=C0A0[30]    , bit=030:030
#define HW_FUSE_RTC_WATCHDOG_DISABLE_BM        0x40000000
#define HW_FUSE_APPUART0_DISABLE_ADDR          0x009       // addr=C0B0[10]    , bit=042:042
#define HW_FUSE_APPUART0_DISABLE_BM            0x00000400
#define HW_FUSE_APPUART0_HI_SPEED_DISABLE_ADDR 0x009       // addr=C0B0[11]    , bit=043:043
#define HW_FUSE_APPUART0_HI_SPEED_DISABLE_BM   0x00000800
#define HW_FUSE_APPUART1_DISABLE_ADDR          0x009       // addr=C0B0[12]    , bit=044:044
#define HW_FUSE_APPUART1_DISABLE_BM            0x00001000
#define HW_FUSE_APPUART1_HI_SPEED_DISABLE_ADDR 0x009       // addr=C0B0[13]    , bit=045:045
#define HW_FUSE_APPUART1_HI_SPEED_DISABLE_BM   0x00002000
#define HW_FUSE_APPUART2_DISABLE_ADDR          0x009       // addr=C0B0[14]    , bit=046:046
#define HW_FUSE_APPUART2_DISABLE_BM            0x00004000
#define HW_FUSE_APPUART2_HI_SPEED_DISABLE_ADDR 0x009       // addr=C0B0[15]    , bit=047:047
#define HW_FUSE_APPUART2_HI_SPEED_DISABLE_BM   0x00008000
#define HW_FUSE_APPUART3_DISABLE_ADDR          0x009       // addr=C0B0[16]    , bit=048:048
#define HW_FUSE_APPUART3_DISABLE_BM            0x00010000
#define HW_FUSE_APPUART3_HI_SPEED_DISABLE_ADDR 0x009       // addr=C0B0[17]    , bit=049:049
#define HW_FUSE_APPUART3_HI_SPEED_DISABLE_BM   0x00020000
#define HW_FUSE_APPUART4_DISABLE_ADDR          0x009       // addr=C0B0[18]    , bit=050:050
#define HW_FUSE_APPUART4_DISABLE_BM            0x00040000
#define HW_FUSE_APPUART4_HI_SPEED_DISABLE_ADDR 0x009       // addr=C0B0[19]    , bit=051:051
#define HW_FUSE_APPUART4_HI_SPEED_DISABLE_BM   0x00080000
#define HW_FUSE_SSP0_DISABLE_ADDR              0x00a       // addr=C0C0[0]     , bit=064:064
#define HW_FUSE_SSP0_DISABLE_BM                0x00000001
#define HW_FUSE_SSP0_MEMSTK_DISABLE_ADDR       0x00a       // addr=C0C0[1]     , bit=065:065
#define HW_FUSE_SSP0_MEMSTK_DISABLE_BM         0x00000002
#define HW_FUSE_SSP0_SD_DISABLE_ADDR           0x00a       // addr=C0C0[2]     , bit=066:066
#define HW_FUSE_SSP0_SD_DISABLE_BM             0x00000004
#define HW_FUSE_SSP1_DISABLE_ADDR              0x00a       // addr=C0C0[3]     , bit=067:067
#define HW_FUSE_SSP1_DISABLE_BM                0x00000008
#define HW_FUSE_SSP1_MEMSTK_DISABLE_ADDR       0x00a       // addr=C0C0[4]     , bit=068:068
#define HW_FUSE_SSP1_MEMSTK_DISABLE_BM         0x00000010
#define HW_FUSE_SSP1_SD_DISABLE_ADDR           0x00a       // addr=C0C0[5]     , bit=069:069
#define HW_FUSE_SSP1_SD_DISABLE_BM             0x00000020
#define HW_FUSE_SSP2_DISABLE_ADDR              0x00a       // addr=C0C0[6]     , bit=070:070
#define HW_FUSE_SSP2_DISABLE_BM                0x00000040
#define HW_FUSE_SSP2_MEMSTK_DISABLE_ADDR       0x00a       // addr=C0C0[7]     , bit=071:071
#define HW_FUSE_SSP2_MEMSTK_DISABLE_BM         0x00000080
#define HW_FUSE_SSP2_SD_DISABLE_ADDR           0x00a       // addr=C0C0[8]     , bit=072:072
#define HW_FUSE_SSP2_SD_DISABLE_BM             0x00000100
#define HW_FUSE_SSP3_DISABLE_ADDR              0x00a       // addr=C0C0[9]     , bit=073:073
#define HW_FUSE_SSP3_DISABLE_BM                0x00000200
#define HW_FUSE_SSP3_MEMSTK_DISABLE_ADDR       0x00a       // addr=C0C0[10]    , bit=074:074
#define HW_FUSE_SSP3_MEMSTK_DISABLE_BM         0x00000400
#define HW_FUSE_SSP3_SD_DISABLE_ADDR           0x00a       // addr=C0C0[11]    , bit=075:075
#define HW_FUSE_SSP3_SD_DISABLE_BM             0x00000800
#define HW_FUSE_DCP_HASH_DISABLE_ADDR          0x00a       // addr=C0C0[16]    , bit=080:080
#define HW_FUSE_DCP_HASH_DISABLE_BM            0x00010000
#define HW_FUSE_DCP_CRYPTO_DISABLE_ADDR        0x00a       // addr=C0C0[17]    , bit=081:081
#define HW_FUSE_DCP_CRYPTO_DISABLE_BM          0x00020000
#define HW_FUSE_GPMI_DISABLE_ADDR              0x00a       // addr=C0C0[20]    , bit=084:084
#define HW_FUSE_GPMI_DISABLE_BM                0x00100000
#define HW_FUSE_CAN0_DISABLE_ADDR              0x00a       // addr=C0C0[21]    , bit=085:085
#define HW_FUSE_CAN0_DISABLE_BM                0x00200000
#define HW_FUSE_CAN1_DISABLE_ADDR              0x00a       // addr=C0C0[22]    , bit=086:086
#define HW_FUSE_CAN1_DISABLE_BM                0x00400000
#define HW_FUSE_HSADC_DISABLE_ADDR             0x00a       // addr=C0C0[23]    , bit=087:087
#define HW_FUSE_HSADC_DISABLE_BM               0x00800000
#define HW_FUSE_SAIF0_DISABLE_ADDR             0x00a       // addr=C0C0[24]    , bit=088:088
#define HW_FUSE_SAIF0_DISABLE_BM               0x01000000
#define HW_FUSE_SAIF1_DISABLE_ADDR             0x00a       // addr=C0C0[25]    , bit=089:089
#define HW_FUSE_SAIF1_DISABLE_BM               0x02000000
#define HW_FUSE_PACKAGE_TYPE_ADDR              0x00b       // addr=C0D0[2:0]   , bit=098:096
#define HW_FUSE_PACKAGE_TYPE_BM                0x00000007
#define HW_FUSE_EMI_DRAM_DISABLE_ADDR          0x00b       // addr=C0D0[6]     , bit=102:102
#define HW_FUSE_EMI_DRAM_DISABLE_BM            0x00000040
#define HW_FUSE_EMI_LARGE_DRAM_DISABLE_ADDR    0x00b       // addr=C0D0[7]     , bit=103:103
#define HW_FUSE_EMI_LARGE_DRAM_DISABLE_BM      0x00000080
#define HW_FUSE_ENET_SWITCH_RESET_BY_SW_ADDR   0x00b       // addr=C0D0[11]    , bit=107:107
#define HW_FUSE_ENET_SWITCH_RESET_BY_SW_BM     0x00000800
#define HW_FUSE_ENET_SWITCH_RESET_BY_SW_CHIP_ADDR 0x00b       // addr=C0D0[12]    , bit=108:108
#define HW_FUSE_ENET_SWITCH_RESET_BY_SW_CHIP_BM   0x00001000
#define HW_FUSE_WDOG_PERFORM_POR_ADDR          0x00b       // addr=C0D0[13]    , bit=109:109
#define HW_FUSE_WDOG_PERFORM_POR_BM            0x00002000
#define HW_FUSE_CPU_CLK_LIMIT_ADDR             0x00b       // addr=C0D0[15:14] , bit=111:110
#define HW_FUSE_CPU_CLK_LIMIT_BM               0x0000c000
#define HW_FUSE_JTAG_LOCKOUT_ADDR              0x00b       // addr=C0D0[16]    , bit=112:112
#define HW_FUSE_JTAG_LOCKOUT_BM                0x00010000
#define HW_FUSE_USB0_OTG_DISABLE_ADDR          0x00c       // addr=C0E0[0]     , bit=128:128
#define HW_FUSE_USB0_OTG_DISABLE_BM            0x00000001
#define HW_FUSE_USB0_HOST_DISABLE_ADDR         0x00c       // addr=C0E0[1]     , bit=129:129
#define HW_FUSE_USB0_HOST_DISABLE_BM           0x00000002
#define HW_FUSE_USB0_DEV_DISABLE_ADDR          0x00c       // addr=C0E0[2]     , bit=130:130
#define HW_FUSE_USB0_DEV_DISABLE_BM            0x00000004
#define HW_FUSE_USB0_HS_DISABLE_ADDR           0x00c       // addr=C0E0[3]     , bit=131:131
#define HW_FUSE_USB0_HS_DISABLE_BM             0x00000008
#define HW_FUSE_USB1_OTG_DISABLE_ADDR          0x00c       // addr=C0E0[4]     , bit=132:132
#define HW_FUSE_USB1_OTG_DISABLE_BM            0x00000010
#define HW_FUSE_USB1_HOST_DISABLE_ADDR         0x00c       // addr=C0E0[5]     , bit=133:133
#define HW_FUSE_USB1_HOST_DISABLE_BM           0x00000020
#define HW_FUSE_USB1_DEV_DISABLE_ADDR          0x00c       // addr=C0E0[6]     , bit=134:134
#define HW_FUSE_USB1_DEV_DISABLE_BM            0x00000040
#define HW_FUSE_USB1_HS_DISABLE_ADDR           0x00c       // addr=C0E0[7]     , bit=135:135
#define HW_FUSE_USB1_HS_DISABLE_BM             0x00000080
#define HW_FUSE_ENET_MAC0_DISABLE_ADDR         0x00c       // addr=C0E0[16]    , bit=144:144
#define HW_FUSE_ENET_MAC0_DISABLE_BM           0x00010000
#define HW_FUSE_ENET_MAC1_DISABLE_ADDR         0x00c       // addr=C0E0[17]    , bit=145:145
#define HW_FUSE_ENET_MAC1_DISABLE_BM           0x00020000
#define HW_FUSE_ENET_SWITCH_DISABLE_ADDR       0x00c       // addr=C0E0[18]    , bit=146:146
#define HW_FUSE_ENET_SWITCH_DISABLE_BM         0x00040000
#define HW_FUSE_PXP_DISABLE_ADDR               0x00c       // addr=C0E0[20]    , bit=148:148
#define HW_FUSE_PXP_DISABLE_BM                 0x00100000
#define HW_FUSE_LCDIF_DISABLE_ADDR             0x00c       // addr=C0E0[21]    , bit=149:149
#define HW_FUSE_LCDIF_DISABLE_BM               0x00200000
#define HW_FUSE_CUST_JTAG_LOCKOUT_ADDR         0x00f       // addr=C110[0]     , bit=224:224
#define HW_FUSE_CUST_JTAG_LOCKOUT_BM           0x00000001
#define HW_FUSE_RTC_XTAL_32000_PRESENT_ADDR    0x00f       // addr=C110[1]     , bit=225:225
#define HW_FUSE_RTC_XTAL_32000_PRESENT_BM      0x00000002
#define HW_FUSE_RTC_XTAL_32768_PRESENT_ADDR    0x00f       // addr=C110[2]     , bit=226:226
#define HW_FUSE_RTC_XTAL_32768_PRESENT_BM      0x00000004
#define HW_FUSE_CUST_DISABLE_JANUSDRM10_ADDR   0x00f       // addr=C110[30]    , bit=254:254
#define HW_FUSE_CUST_DISABLE_JANUSDRM10_BM     0x40000000
#define HW_FUSE_CUST_DISABLE_WMADRM9_ADDR      0x00f       // addr=C110[31]    , bit=255:255
#define HW_FUSE_CUST_DISABLE_WMADRM9_BM        0x80000000
