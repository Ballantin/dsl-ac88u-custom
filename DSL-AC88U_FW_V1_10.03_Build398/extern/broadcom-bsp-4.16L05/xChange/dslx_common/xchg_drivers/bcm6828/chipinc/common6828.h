/*
<:copyright-broadcom 
 
 Copyright (c) 2010 Broadcom 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom
          5300 California Avenue
          Irvine, California 92617
 All information contained in this document is Broadcom 
 company private, proprietary, and trade secret. 
 
:>
*/
/***********************************************************************/
/*                                                                     */
/*   MODULE:  6828_common.h                                           */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6828                                                 */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6828_MAP_COMMON_H
#define __BCM6828_MAP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define BCM6828_REG_BASE                 0xb0000000

#define BCM6828_PERF_BASE                   (BCM6828_REG_BASE + 0x00000000)  /* chip control */
#define BCM6828_TIMR_BASE                   (BCM6828_REG_BASE + 0x00000080)  /* timer registers */
#define BCM6828_NAND_INTR_BASE              (BCM6828_REG_BASE + 0x000000b0)  /* nand interrupt control */
#define BCM6828_GPIO_BASE                   (BCM6828_REG_BASE + 0x000000c0)  /* gpio registers */
#define BCM6828_UART_BASE                   (BCM6828_REG_BASE + 0x00000180)  /* uart registers */
#define BCM6828_UART1_BASE                  (BCM6828_REG_BASE + 0x000001a0)  /* uart registers */
#define BCM6828_NAND_REG_BASE               (BCM6828_REG_BASE + 0x00000200)  /* nand interrupt control */
#define BCM6828_OTP_BASE                    (BCM6828_REG_BASE + 0x00000400)
#define BCM6828_NAND_CACHE_BASE             (BCM6828_REG_BASE + 0x00000600)
#define BCM6828_SPI_BASE                    (BCM6828_REG_BASE + 0x00000800)  /* SPI master controller */
#define BCM6828_HSSPIM_BASE                 (BCM6828_REG_BASE + 0x00001000)  /* High-Speed SPI registers */
#define BCM6828_MISC_BASE                   (BCM6828_REG_BASE + 0x00001800)  /* Miscellaneous Registers */
#define BCM6828_LED_BASE                    (BCM6828_REG_BASE + 0x00001900)  /* LED control registers */
#define BCM6828_APM_BASE                    (BCM6828_REG_BASE + 0x00200000)  /* APM control registers */ 
#define BCM6828_PCM_BASE                    (BCM6828_REG_BASE + 0x00200200)  /* PCM control registers */
#define BCM6828_APM_HVG_BASE                (BCM6828_REG_BASE + 0x00200300)  /* APM HVG control registers */
#define BCM6828_APM_PCM_DMA_BASE            (BCM6828_REG_BASE + 0x00200800)  /* APM/PCM DMA control registers */
#define BCM6828_USB_CTL_BASE                (BCM6828_REG_BASE + 0x00002400)  /* USB 2.0 device control */
#define BCM6828_USB_EHCI_BASE               0x10002500  /* USB host registers */
#define BCM6828_USB_OHCI_BASE               0x10002600  /* USB host registers */
#define BCM6828_USBH_CFG_BASE               (BCM6828_REG_BASE + 0x00002700)
#define BCM6828_HSI_BASE                    (BCM6828_REG_BASE + 0x00002800)
#define BCM6828_DDR_BASE                    (BCM6828_REG_BASE + 0x00003000)  /* DDR IO Buf Control */
#define BCM6828_SWITCH_DMA_BASE             (BCM6828_REG_BASE + 0x00004000)
#define BCM6828_USB_DMA_BASE                (BCM6828_REG_BASE + 0x00004800)  /* USB 2.0 device DMA */
#define BCM6828_PCIE_BASE                   (BCM6828_REG_BASE + 0x006e0000)
#define BCM6828_SWITCH_BASE                 (BCM6828_REG_BASE + 0x00700000)

#define BCM6828_FAP0_BASE                   (BCM6828_REG_BASE + 0x00800000)
#define BCM6828_FAP0_CNTRL_BASE             (BCM6828_REG_BASE + 0x00801000)
#define BCM6828_FAP0_OUT_FIFO_BASE          (BCM6828_REG_BASE + 0x00801100)
#define BCM6828_FAP0_IN_FIFO_BASE           (BCM6828_REG_BASE + 0x00801200)
#define BCM6828_FAP0_DMA0_BASE              (BCM6828_REG_BASE + 0x00801300)
#define BCM6828_FAP0_DMA1_BASE              (BCM6828_REG_BASE + 0x00801320)
#define BCM6828_FAP0_TOKEN_BASE             (BCM6828_REG_BASE + 0x00801400)
#define BCM6828_FAP0_MSG_BASE               (BCM6828_REG_BASE + 0x00801700)
#define BCM6828_FAP0_DQM_BASE               (BCM6828_REG_BASE + 0x00801800)
#define BCM6828_FAP0_DQM_CNTRL_BASE         (BCM6828_REG_BASE + 0x00801a00)
#define BCM6828_FAP0_DQM_DATA_BASE          (BCM6828_REG_BASE + 0x00801c00)
#define BCM6828_FAP0_DQM_MIB_BASE           (BCM6828_REG_BASE + 0x00802000)
#define BCM6828_FAP0_QSM_UBUS_BASE          (BCM6828_REG_BASE + 0x00804000)
#define BCM6828_FAP0_QSM_SMI_BASE           (BCM6828_REG_BASE + 0x00c04000)
#define BCM6828_FAP0_PSM_BASE               (BCM6828_REG_BASE + 0x00820000)
#define BCM6828_FAP0_SMISB                  (BCM6828_REG_BASE + 0x00c00000)

#define BCM6828_FAP1_BASE                   (BCM6828_REG_BASE + 0x00a00000)
#define BCM6828_FAP1_CNTRL_BASE             (BCM6828_REG_BASE + 0x00a01000)
#define BCM6828_FAP1_IN_FIFO_BASE           (BCM6828_REG_BASE + 0x00a01100)
#define BCM6828_FAP1_OUT_FIFO_BASE          (BCM6828_REG_BASE + 0x00a01200)
#define BCM6828_FAP1_DMA0_BASE              (BCM6828_REG_BASE + 0x00a01300)
#define BCM6828_FAP1_DMA1_BASE              (BCM6828_REG_BASE + 0x00a01320)
#define BCM6828_FAP1_TOKEN_BASE             (BCM6828_REG_BASE + 0x00a01400)
#define BCM6828_FAP1_MSG_BASE               (BCM6828_REG_BASE + 0x00a01700)
#define BCM6828_FAP1_DQM_BASE               (BCM6828_REG_BASE + 0x00a01800)
#define BCM6828_FAP1_DQM_CNTRL_BASE         (BCM6828_REG_BASE + 0x00a01a00)
#define BCM6828_FAP1_DQM_DATA_BASE          (BCM6828_REG_BASE + 0x00a01c00)
#define BCM6828_FAP1_DQM_MIB_BASE           (BCM6828_REG_BASE + 0x00a02000)
#define BCM6828_FAP1_QSM_UBUS_BASE          (BCM6828_REG_BASE + 0x00a04000)
#define BCM6828_FAP1_QSM_SMI_BASE           (BCM6828_REG_BASE + 0x00e04000)
#define BCM6828_FAP1_PSM_BASE               (BCM6828_REG_BASE + 0x00a20000)
#define BCM6828_FAP1_SMISB                  (BCM6828_REG_BASE + 0x00e00000)



/*
#####################################################################
# System PLL Control Register
#####################################################################
*/


/*
#####################################################################
# GPIO Control Registers
#####################################################################
*/


/*
#####################################################################
# Miscellaneous Registers
#####################################################################
*/
#define BCM6828_MISC_MEMC_CONTROL                       0x10
   #define BCM6828_MISC_MEMC_CONTROL_MC_UBUS_ASYNC_MODE    (1<<3)
   #define BCM6828_MISC_MEMC_CONTROL_MC_LMB_ASYNC_MODE     (1<<2)

#define BCM6828_MISC_STRAP_BUS                          0x14
   #ifndef BCM6828_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT
   #define BCM6828_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT      21
   #endif
   
   #ifndef BCM6828_MISC_STRAP_BUS_MIPS_PLL_FVCO_MASK
   #define BCM6828_MISC_STRAP_BUS_MIPS_PLL_FVCO_MASK       (0xF<<BCM6828_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT)
   #endif

    /*
#####################################################################
# Memory Control Registers
#####################################################################
*/
#define BCM6828_DDR_CTL_CNFG                           0x000 
#define BCM6828_DDR_CTL_CSST                           0x004 
#define BCM6828_DDR_CTL_CSEND                          0x008 
#define BCM6828_DDR_CTL_ROW00_0                        0x010 
#define BCM6828_DDR_CTL_ROW00_1                        0x014 
#define BCM6828_DDR_CTL_ROW01_0                        0x018 
#define BCM6828_DDR_CTL_ROW01_1                        0x01c 
#define BCM6828_DDR_CTL_ROW20_0                        0x030 
#define BCM6828_DDR_CTL_ROW20_1                        0x034 
#define BCM6828_DDR_CTL_ROW21_0                        0x038 
#define BCM6828_DDR_CTL_ROW21_1                        0x03c 
#define BCM6828_DDR_CTL_COL00_0                        0x050 
#define BCM6828_DDR_CTL_COL00_1                        0x054 
#define BCM6828_DDR_CTL_COL01_0                        0x058 
#define BCM6828_DDR_CTL_COL01_1                        0x05c 
#define BCM6828_DDR_CTL_COL20_0                        0x070 
#define BCM6828_DDR_CTL_COL20_1                        0x074 
#define BCM6828_DDR_CTL_COL21_0                        0x078 
#define BCM6828_DDR_CTL_COL21_1                        0x07c 
#define BCM6828_DDR_CTL_BNK10                          0x090 
#define BCM6828_DDR_CTL_BNK32                          0x094 
#define BCM6828_DDR_CTL_DCMD                           0x100 
#define BCM6828_DDR_CTL_DMODE_0                        0x104 
#define BCM6828_DDR_CTL_DMODE_1                        0x108 
#define BCM6828_DDR_CTL_CLKS                           0x10c 
#define BCM6828_DDR_CTL_ODT                            0x110 
#define BCM6828_DDR_CTL_TIM1_0                         0x114 
#define BCM6828_DDR_CTL_TIM1_1                         0x118 
#define BCM6828_DDR_CTL_TIM2                           0x11c 
#define BCM6828_DDR_CTL_CTL_CRC                        0x120 
#define BCM6828_DDR_CTL_DOUT_CRC                       0x124 
#define BCM6828_DDR_CTL_DIN_CRC                        0x128 

#define BCM6828_PHY_CONTROL_REGS_REVISION               0x200 
#define BCM6828_PHY_CONTROL_REGS_CLK_PM_CTRL            0x204 
#define BCM6828_PHY_CONTROL_REGS_PLL_STATUS             0x210 
#define BCM6828_PHY_CONTROL_REGS_PLL_CONFIG             0x214 
#define BCM6828_PHY_CONTROL_REGS_PLL_PRE_DIVIDER        0x218 
#define BCM6828_PHY_CONTROL_REGS_PLL_DIVIDER            0x21c 
#define BCM6828_PHY_CONTROL_REGS_PLL_CONTROL1           0x220 
#define BCM6828_PHY_CONTROL_REGS_PLL_CONTROL2           0x224 
#define BCM6828_PHY_CONTROL_REGS_PLL_SS_EN              0x228 
#define BCM6828_PHY_CONTROL_REGS_PLL_SS_CFG             0x22c 
#define BCM6828_PHY_CONTROL_REGS_STATIC_VDL_OVERRIDE    0x230 
#define BCM6828_PHY_CONTROL_REGS_DYNAMIC_VDL_OVERRIDE   0x234 
#define BCM6828_PHY_CONTROL_REGS_IDLE_PAD_CONTROL       0x238 
#define BCM6828_PHY_CONTROL_REGS_ZQ_PVT_COMP_CTL        0x23c 
#define BCM6828_PHY_CONTROL_REGS_DRIVE_PAD_CTL          0x240 

#define BCM6828_PHY_BYTE_LANE_0_REVISION                0x300 
#define BCM6828_PHY_BYTE_LANE_0_VDL_CALIBRATE           0x304 
#define BCM6828_PHY_BYTE_LANE_0_VDL_STATUS              0x308 
#define BCM6828_PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_MAX  63
#define BCM6828_PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_STRT 8
#define BCM6828_PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_MASK (0x3f << PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_STRT)
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_0          0x310 
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_1          0x314 
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_2          0x318 
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_3          0x31c 
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_4          0x320 
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_5          0x324 
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_6          0x328 
#define BCM6828_PHY_BYTE_LANE_0_VDL_OVERRIDE_7          0x32c 
#define BCM6828_PHY_BYTE_LANE_0_READ_CONTROL            0x330 
#define BCM6828_PHY_BYTE_LANE_0_READ_FIFO_STATUS        0x334 
#define BCM6828_PHY_BYTE_LANE_0_READ_FIFO_CLEAR         0x338 
#define BCM6828_PHY_BYTE_LANE_0_IDLE_PAD_CONTROL        0x33c 
#define BCM6828_PHY_BYTE_LANE_0_DRIVE_PAD_CTL           0x340 
#define BCM6828_PHY_BYTE_LANE_0_CLOCK_PAD_DISABLE       0x344 
#define BCM6828_PHY_BYTE_LANE_0_WR_PREAMBLE_MODE        0x348 

#define BCM6828_PHY_BYTE_LANE_1_REVISION                0x400 
#define BCM6828_PHY_BYTE_LANE_1_VDL_CALIBRATE           0x404 
#define BCM6828_PHY_BYTE_LANE_1_VDL_STATUS              0x408 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_0          0x410 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_1          0x414 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_2          0x418 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_3          0x41c 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_4          0x420 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_5          0x424 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_6          0x428 
#define BCM6828_PHY_BYTE_LANE_1_VDL_OVERRIDE_7          0x42c 
#define BCM6828_PHY_BYTE_LANE_1_READ_CONTROL            0x430 
#define BCM6828_PHY_BYTE_LANE_1_READ_FIFO_STATUS        0x434 
#define BCM6828_PHY_BYTE_LANE_1_READ_FIFO_CLEAR         0x438 
#define BCM6828_PHY_BYTE_LANE_1_IDLE_PAD_CONTROL        0x43c 
#define BCM6828_PHY_BYTE_LANE_1_DRIVE_PAD_CTL           0x440 
#define BCM6828_PHY_BYTE_LANE_1_CLOCK_PAD_DISABLE       0x444 
#define BCM6828_PHY_BYTE_LANE_1_WR_PREAMBLE_MODE        0x448 

#define BCM6828_DDR_CTL_GCFG                           0x800 
/* These bits are really not HW status bits, but ddr init routines use it as a temporary variable holding space */
#define BCM6828_DDR_CTL_GCFG_MEMINITDONE(x)     ((x & 0x1)<<8)
#define BCM6828_DDR_CTL_GCFG_MEMINITDONE_STRT   (8)
#define BCM6828_DDR_CTL_GCFG_MEMINITDONE_MSK    (1<<DDR_CTL_GCFG_MEMINITDONE_STRT)
#define BCM6828_DDR_CTL_ARB                            0x80c 
#define BCM6828_DDR_CTL_PI_GCF                         0x810 
#define BCM6828_DDR_CTL_PI_UBUS_CTL                    0x814 
#define BCM6828_DDR_CTL_PI_MIPS_CTL                    0x818 
#define BCM6828_DDR_CTL_PI_DSL_MIPS_CTL                0x81c 
#define BCM6828_DDR_CTL_PI_DSL_PHY_CTL                 0x820 
#define BCM6828_DDR_CTL_PI_UBUS_ST                     0x824 
#define BCM6828_DDR_CTL_PI_MIPS_ST                     0x828 
#define BCM6828_DDR_CTL_PI_DSL_MIPS_ST                 0x82c 
#define BCM6828_DDR_CTL_PI_DSL_PHY_ST                  0x830 
#define BCM6828_DDR_CTL_PI_UBUS_SMPL                   0x834
#define BCM6828_DDR_CTL_PI_UBUS_SMPL_SAMPLING_PERIOD_STRT (28)
#define BCM6828_DDR_CTL_TESTMODE                       0x838 
#define BCM6828_DDR_CTL_TEST_CFG1                      0x83c 
#define BCM6828_DDR_CTL_TEST_PAT                       0x840 
#define BCM6828_DDR_CTL_TEST_COUNT                     0x844 
#define BCM6828_DDR_CTL_TEST_CURR_COUNT                0x848 
#define BCM6828_DDR_CTL_TEST_ADDR_UPDT                 0x84c 
#define BCM6828_DDR_CTL_TEST_ADDR                      0x850 
#define BCM6828_DDR_CTL_TEST_DATA0                     0x854 
#define BCM6828_DDR_CTL_TEST_DATA1                     0x858 
#define BCM6828_DDR_CTL_TEST_DATA2                     0x85c 
#define BCM6828_DDR_CTL_TEST_DATA3                     0x860 

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define BCM6828_UART0CONTROL     0x01
#define BCM6828_UART0CONFIG      0x02
#define BCM6828_UART0RXTIMEOUT   0x03
#define BCM6828_UART0BAUD        0x04
#define BCM6828_UART0FIFOCFG     0x0a
#define BCM6828_UART0INTMASK     0x10
#define BCM6828_UART0INTSTAT     0x12
#define BCM6828_UART0DATA        0x17

#define BCM6828_BRGEN            0x80   /* Control register bit defs */
#define BCM6828_TXEN             0x40
#define BCM6828_RXEN             0x20
#define BCM6828_LOOPBK           0x10
#define BCM6828_TXPARITYEN       0x08
#define BCM6828_TXPARITYEVEN     0x04
#define BCM6828_RXPARITYEN       0x02
#define BCM6828_RXPARITYEVEN     0x01

#define BCM6828_XMITBREAK        0x40   /* Config register */
#define BCM6828_BITS5SYM         0x00
#define BCM6828_BITS6SYM         0x10
#define BCM6828_BITS7SYM         0x20
#define BCM6828_BITS8SYM         0x30
#define BCM6828_ONESTOP          0x07
#define BCM6828_TWOSTOP          0x0f

#define BCM6828_DELTAIP         0x0001
#define BCM6828_TXUNDERR        0x0002
#define BCM6828_TXOVFERR        0x0004
#define BCM6828_TXFIFOTHOLD     0x0008
#define BCM6828_TXREADLATCH     0x0010
#define BCM6828_TXFIFOEMT       0x0020
#define BCM6828_RXUNDERR        0x0040
#define BCM6828_RXOVFERR        0x0080
#define BCM6828_RXTIMEOUT       0x0100
#define BCM6828_RXFIFOFULL      0x0200
#define BCM6828_RXFIFOTHOLD     0x0400
#define BCM6828_RXFIFONE        0x0800
#define BCM6828_RXFRAMERR       0x1000
#define BCM6828_RXPARERR        0x2000
#define BCM6828_RXBRK           0x4000



#ifdef __cplusplus
}
#endif

#endif
