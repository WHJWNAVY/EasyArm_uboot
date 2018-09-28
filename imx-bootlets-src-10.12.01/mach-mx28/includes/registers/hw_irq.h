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


#ifndef HW_IRQ_DEFS
#define HW_IRQ_DEFS  1

#define HW_IRQ_BATT_BROWNOUT                            (   0 )
#define HW_IRQ_VDDD0_BROWNOUT                           (   1 )
#define HW_IRQ_VDDIO_BROWNOUT                           (   2 )
#define HW_IRQ_VDDA_BROWNOUT                            (   3 )
#define HW_IRQ_VDD5V_DROOP                              (   4 )
#define HW_IRQ_DCDC4P2_BROWNOUT                         (   5 )
#define HW_IRQ_VDD5V                                    (   6 )
#define HW_IRQ_RESERVED_7                               (   7 )
#define HW_IRQ_CAN0                                     (   8 )
#define HW_IRQ_CAN1                                     (   9 )
#define HW_IRQ_LRADC_TOUCH                              (  10 )
#define HW_IRQ_RESERVED_11                              (  11 )
#define HW_IRQ_RESERVED_12                              (  12 )
#define HW_IRQ_HSADC                                    (  13 )
#define HW_IRQ_LRADC_THRESH0                            (  14 )
#define HW_IRQ_LRADC_THRESH1                            (  15 )
#define HW_IRQ_LRADC_CH0                                (  16 )
#define HW_IRQ_LRADC_CH1                                (  17 )
#define HW_IRQ_LRADC_CH2                                (  18 )
#define HW_IRQ_LRADC_CH3                                (  19 )
#define HW_IRQ_LRADC_CH4                                (  20 )
#define HW_IRQ_LRADC_CH5                                (  21 )
#define HW_IRQ_LRADC_CH6                                (  22 )
#define HW_IRQ_LRADC_CH7                                (  23 )
#define HW_IRQ_LRADC_BUTTON0                            (  24 )
#define HW_IRQ_LRADC_BUTTON1                            (  25 )
#define HW_IRQ_RESERVED_26                              (  26 )
#define HW_IRQ_BUS_PERFMON                              (  27 )
#define HW_IRQ_RTC_1MSEC                                (  28 )
#define HW_IRQ_RTC_ALARM                                (  29 )
#define HW_IRQ_RESERVED_30                              (  30 )
#define HW_IRQ_COMMS                                    (  31 )
#define HW_IRQ_EMI_ERROR                                (  32 )
#define HW_IRQ_RESERVED_33                              (  33 )
#define HW_IRQ_RESERVED_34                              (  34 )
#define HW_IRQ_RESERVED_35                              (  35 )
#define HW_IRQ_RESERVED_36                              (  36 )
#define HW_IRQ_RESERVED_37                              (  37 )
#define HW_IRQ_LCD                                      (  38 )
#define HW_IRQ_PXP                                      (  39 )
#define HW_IRQ_RESERVED_40                              (  40 )
#define HW_IRQ_BCH                                      (  41 )
#define HW_IRQ_GPMI                                     (  42 )
#define HW_IRQ_RESERVED_43                              (  43 )
#define HW_IRQ_RESERVED_44                              (  44 )
#define HW_IRQ_SPDIF_ERROR                              (  45 )
#define HW_IRQ_RESERVED_46                              (  46 )
#define HW_IRQ_DBG_UART                                 (  47 )
#define HW_IRQ_TIMER0                                   (  48 )
#define HW_IRQ_TIMER1                                   (  49 )
#define HW_IRQ_TIMER2                                   (  50 )
#define HW_IRQ_TIMER3                                   (  51 )
#define HW_IRQ_DCP_VMI                                  (  52 )
#define HW_IRQ_DCP                                      (  53 )
#define HW_IRQ_DCP_SECURE                               (  54 )
#define HW_IRQ_RESERVED_55                              (  55 )
#define HW_IRQ_RESERVED_56                              (  56 )
#define HW_IRQ_RESERVED_57                              (  57 )
#define HW_IRQ_SAIF1                                    (  58 )
#define HW_IRQ_SAIF0                                    (  59 )
#define SW_IRQ_60                                       (  60 )
#define SW_IRQ_61                                       (  61 )
#define SW_IRQ_62                                       (  62 )
#define SW_IRQ_63                                       (  63 )
#define HW_IRQ_RESERVED_64                              (  64 )
#define HW_IRQ_RESERVED_65                              (  65 )
#define HW_IRQ_SPDIF_DMA                                (  66 )
#define HW_IRQ_RESERVED_67                              (  67 )
#define HW_IRQ_I2C0_DMA                                 (  68 )
#define HW_IRQ_I2C1_DMA                                 (  69 )
#define HW_IRQ_AUART0_RX_DMA                            (  70 )
#define HW_IRQ_AUART0_TX_DMA                            (  71 )
#define HW_IRQ_AUART1_RX_DMA                            (  72 )
#define HW_IRQ_AUART1_TX_DMA                            (  73 )
#define HW_IRQ_AUART2_RX_DMA                            (  74 )
#define HW_IRQ_AUART2_TX_DMA                            (  75 )
#define HW_IRQ_AUART3_RX_DMA                            (  76 )
#define HW_IRQ_AUART3_TX_DMA                            (  77 )
#define HW_IRQ_AUART4_RX_DMA                            (  78 )
#define HW_IRQ_AUART4_TX_DMA                            (  79 )
#define HW_IRQ_SAIF0_DMA                                (  80 )
#define HW_IRQ_SAIF1_DMA                                (  81 )
#define HW_IRQ_SSP0_DMA                                 (  82 )
#define HW_IRQ_SSP1_DMA                                 (  83 )
#define HW_IRQ_SSP2_DMA                                 (  84 )
#define HW_IRQ_SSP3_DMA                                 (  85 )
#define HW_IRQ_LCD_DMA                                  (  86 )
#define HW_IRQ_HSADC_DMA                                (  87 )
#define HW_IRQ_GPMI_DMA                                 (  88 )
#define HW_IRQ_DIGCTL_DEBUG_TRAP                        (  89 )
#define HW_IRQ_RESERVED_90                              (  90 )
#define HW_IRQ_RESERVED_91                              (  91 )
#define HW_IRQ_USB1                                     (  92 )
#define HW_IRQ_USB0                                     (  93 )
#define HW_IRQ_USB1_WAKEUP                              (  94 )
#define HW_IRQ_USB0_WAKEUP                              (  95 )
#define HW_IRQ_SSP0_ERROR                               (  96 )
#define HW_IRQ_SSP1_ERROR                               (  97 )
#define HW_IRQ_SSP2_ERROR                               (  98 )
#define HW_IRQ_SSP3_ERROR                               (  99 )
#define HW_IRQ_ENET_SWI                                 ( 100 )
#define HW_IRQ_ENET_MAC0                                ( 101 )
#define HW_IRQ_ENET_MAC1                                ( 102 )
#define HW_IRQ_ENET_MAC0_1588                           ( 103 )
#define HW_IRQ_ENET_MAC1_1588                           ( 104 )
#define HW_IRQ_RESERVED_105                             ( 105 )
#define HW_IRQ_RESERVED_106                             ( 106 )
#define HW_IRQ_RESERVED_107                             ( 107 )
#define HW_IRQ_RESERVED_108                             ( 108 )
#define HW_IRQ_RESERVED_109                             ( 109 )
#define HW_IRQ_I2C1_ERROR                               ( 110 )
#define HW_IRQ_I2C0_ERROR                               ( 111 )
#define HW_IRQ_APP_UART0                                ( 112 )
#define HW_IRQ_APP_UART1                                ( 113 )
#define HW_IRQ_APP_UART2                                ( 114 )
#define HW_IRQ_APP_UART3                                ( 115 )
#define HW_IRQ_APP_UART4                                ( 116 )
#define HW_IRQ_RESERVED_117                             ( 117 )
#define HW_IRQ_RESERVED_118                             ( 118 )
#define HW_IRQ_RESERVED_119                             ( 119 )
#define HW_IRQ_RESERVED_120                             ( 120 )
#define HW_IRQ_RESERVED_121                             ( 121 )
#define HW_IRQ_RESERVED_122                             ( 122 )
#define HW_IRQ_GPIO4                                    ( 123 )
#define HW_IRQ_GPIO3                                    ( 124 )
#define HW_IRQ_GPIO2                                    ( 125 )
#define HW_IRQ_GPIO1                                    ( 126 )
#define HW_IRQ_GPIO0                                    ( 127 )

////////////////////////////////////////////////////////////////
///   IRQ ENUMERATIONS
////////////////////////////////////////////////////////////////

#ifndef __LANGUAGE_ASM__
typedef enum
{
	FIRST_IRQ_ICOLL_VECTOR              = 0,
	VECTOR_IRQ_BATT_BROWNOUT                            =   0 ,
	VECTOR_IRQ_VDDD0_BROWNOUT                           =   1 ,
	VECTOR_IRQ_VDDIO_BROWNOUT                           =   2 ,
	VECTOR_IRQ_VDDA_BROWNOUT                            =   3 ,
	VECTOR_IRQ_VDD5V_DROOP                              =   4 ,
	VECTOR_IRQ_DCDC4P2_BROWNOUT                         =   5 ,
	VECTOR_IRQ_VDD5V                                    =   6 ,
	VECTOR_IRQ_RESERVED_7                               =   7 ,
	VECTOR_IRQ_CAN0                                     =   8 ,
	VECTOR_IRQ_CAN1                                     =   9 ,
	VECTOR_IRQ_LRADC_TOUCH                              =  10 ,
	VECTOR_IRQ_RESERVED_11                              =  11 ,
	VECTOR_IRQ_RESERVED_12                              =  12 ,
	VECTOR_IRQ_HSADC                                    =  13 ,
	VECTOR_IRQ_LRADC_THRESH0                            =  14 ,
	VECTOR_IRQ_LRADC_THRESH1                            =  15 ,
	VECTOR_IRQ_LRADC_CH0                                =  16 ,
	VECTOR_IRQ_LRADC_CH1                                =  17 ,
	VECTOR_IRQ_LRADC_CH2                                =  18 ,
	VECTOR_IRQ_LRADC_CH3                                =  19 ,
	VECTOR_IRQ_LRADC_CH4                                =  20 ,
	VECTOR_IRQ_LRADC_CH5                                =  21 ,
	VECTOR_IRQ_LRADC_CH6                                =  22 ,
	VECTOR_IRQ_LRADC_CH7                                =  23 ,
	VECTOR_IRQ_LRADC_BUTTON0                            =  24 ,
	VECTOR_IRQ_LRADC_BUTTON1                            =  25 ,
	VECTOR_IRQ_RESERVED_26                              =  26 ,
	VECTOR_IRQ_BUS_PERFMON                              =  27 ,
	VECTOR_IRQ_RTC_1MSEC                                =  28 ,
	VECTOR_IRQ_RTC_ALARM                                =  29 ,
	VECTOR_IRQ_RESERVED_30                              =  30 ,
	VECTOR_IRQ_COMMS                                    =  31 ,
	VECTOR_IRQ_EMI_ERROR                                =  32 ,
	VECTOR_IRQ_RESERVED_33                              =  33 ,
	VECTOR_IRQ_RESERVED_34                              =  34 ,
	VECTOR_IRQ_RESERVED_35                              =  35 ,
	VECTOR_IRQ_RESERVED_36                              =  36 ,
	VECTOR_IRQ_RESERVED_37                              =  37 ,
	VECTOR_IRQ_LCD                                      =  38 ,
	VECTOR_IRQ_PXP                                      =  39 ,
	VECTOR_IRQ_RESERVED_40                              =  40 ,
	VECTOR_IRQ_BCH                                      =  41 ,
	VECTOR_IRQ_GPMI                                     =  42 ,
	VECTOR_IRQ_RESERVED_43                              =  43 ,
	VECTOR_IRQ_RESERVED_44                              =  44 ,
	VECTOR_IRQ_SPDIF_ERROR                              =  45 ,
	VECTOR_IRQ_RESERVED_46                              =  46 ,
	VECTOR_IRQ_DBG_UART                                 =  47 ,
	VECTOR_IRQ_TIMER0                                   =  48 ,
	VECTOR_IRQ_TIMER1                                   =  49 ,
	VECTOR_IRQ_TIMER2                                   =  50 ,
	VECTOR_IRQ_TIMER3                                   =  51 ,
	VECTOR_IRQ_DCP_VMI                                  =  52 ,
	VECTOR_IRQ_DCP                                      =  53 ,
	VECTOR_IRQ_DCP_SECURE                               =  54 ,
	VECTOR_IRQ_RESERVED_55                              =  55 ,
	VECTOR_IRQ_RESERVED_56                              =  56 ,
	VECTOR_IRQ_RESERVED_57                              =  57 ,
	VECTOR_IRQ_SAIF1                                    =  58 ,
	VECTOR_IRQ_SAIF0                                    =  59 ,
	VECTOR_IRQ_60                                       =  60 ,
	VECTOR_IRQ_61                                       =  61 ,
	VECTOR_IRQ_62                                       =  62 ,
	VECTOR_IRQ_63                                       =  63 ,
	VECTOR_IRQ_RESERVED_64                              =  64 ,
	VECTOR_IRQ_RESERVED_65                              =  65 ,
	VECTOR_IRQ_SPDIF_DMA                                =  66 ,
	VECTOR_IRQ_RESERVED_67                              =  67 ,
	VECTOR_IRQ_I2C0_DMA                                 =  68 ,
	VECTOR_IRQ_I2C1_DMA                                 =  69 ,
	VECTOR_IRQ_AUART0_RX_DMA                            =  70 ,
	VECTOR_IRQ_AUART0_TX_DMA                            =  71 ,
	VECTOR_IRQ_AUART1_RX_DMA                            =  72 ,
	VECTOR_IRQ_AUART1_TX_DMA                            =  73 ,
	VECTOR_IRQ_AUART2_RX_DMA                            =  74 ,
	VECTOR_IRQ_AUART2_TX_DMA                            =  75 ,
	VECTOR_IRQ_AUART3_RX_DMA                            =  76 ,
	VECTOR_IRQ_AUART3_TX_DMA                            =  77 ,
	VECTOR_IRQ_AUART4_RX_DMA                            =  78 ,
	VECTOR_IRQ_AUART4_TX_DMA                            =  79 ,
	VECTOR_IRQ_SAIF0_DMA                                =  80 ,
	VECTOR_IRQ_SAIF1_DMA                                =  81 ,
	VECTOR_IRQ_SSP0_DMA                                 =  82 ,
	VECTOR_IRQ_SSP1_DMA                                 =  83 ,
	VECTOR_IRQ_SSP2_DMA                                 =  84 ,
	VECTOR_IRQ_SSP3_DMA                                 =  85 ,
	VECTOR_IRQ_LCD_DMA                                  =  86 ,
	VECTOR_IRQ_HSADC_DMA                                =  87 ,
	VECTOR_IRQ_GPMI_DMA                                 =  88 ,
	VECTOR_IRQ_DIGCTL_DEBUG_TRAP                        =  89 ,
	VECTOR_IRQ_RESERVED_90                              =  90 ,
	VECTOR_IRQ_RESERVED_91                              =  91 ,
	VECTOR_IRQ_USB1                                     =  92 ,
	VECTOR_IRQ_USB0                                     =  93 ,
	VECTOR_IRQ_USB1_WAKEUP                              =  94 ,
	VECTOR_IRQ_USB0_WAKEUP                              =  95 ,
	VECTOR_IRQ_SSP0_ERROR                               =  96 ,
	VECTOR_IRQ_SSP1_ERROR                               =  97 ,
	VECTOR_IRQ_SSP2_ERROR                               =  98 ,
	VECTOR_IRQ_SSP3_ERROR                               =  99 ,
	VECTOR_IRQ_ENET_SWI                                 = 100 ,
	VECTOR_IRQ_ENET_MAC0                                = 101 ,
	VECTOR_IRQ_ENET_MAC1                                = 102 ,
	VECTOR_IRQ_ENET_MAC0_1588                           = 103 ,
	VECTOR_IRQ_ENET_MAC1_1588                           = 104 ,
	VECTOR_IRQ_RESERVED_105                             = 105 ,
	VECTOR_IRQ_RESERVED_106                             = 106 ,
	VECTOR_IRQ_RESERVED_107                             = 107 ,
	VECTOR_IRQ_RESERVED_108                             = 108 ,
	VECTOR_IRQ_RESERVED_109                             = 109 ,
	VECTOR_IRQ_I2C1_ERROR                               = 110 ,
	VECTOR_IRQ_I2C0_ERROR                               = 111 ,
	VECTOR_IRQ_APP_UART0                                = 112 ,
	VECTOR_IRQ_APP_UART1                                = 113 ,
	VECTOR_IRQ_APP_UART2                                = 114 ,
	VECTOR_IRQ_APP_UART3                                = 115 ,
	VECTOR_IRQ_APP_UART4                                = 116 ,
	VECTOR_IRQ_RESERVED_117                             = 117 ,
	VECTOR_IRQ_RESERVED_118                             = 118 ,
	VECTOR_IRQ_RESERVED_119                             = 119 ,
	VECTOR_IRQ_RESERVED_120                             = 120 ,
	VECTOR_IRQ_RESERVED_121                             = 121 ,
	VECTOR_IRQ_RESERVED_122                             = 122 ,
	VECTOR_IRQ_GPIO4                                    = 123 ,
	VECTOR_IRQ_GPIO3                                    = 124 ,
	VECTOR_IRQ_GPIO2                                    = 125 ,
	VECTOR_IRQ_GPIO1                                    = 126 ,
	VECTOR_IRQ_GPIO0                                    = 127 ,

	END_IRQ_ICOLL_VECTOR                = 128
} ICOLL_IRQ_enums_t;
#endif

////////////////////////////////////////////////////////////////
///   FIQ ENUMERATIONS
////////////////////////////////////////////////////////////////

#ifndef __LANGUAGE_ASM__
typedef enum
{
	FIRST_FIQ_ICOLL_VECTOR    = 32,
	VECTOR_FIQ_BATT_BRNOUT    = 32,
	VECTOR_FIQ_VDDD_BRNOUT    = 33,
	VECTOR_FIQ_VDDIO_BRNOUT   = 34,
	VECTOR_FIQ_VDD18_BRNOUT   = 35,
	LAST_FIQ_ICOLL_VECTOR     = 35
} ICOLL_FIQ_enums_t;
#endif

#endif
