/*
<:copyright-broadcom 
 
 Copyright (c) 2007 Broadcom 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom 
 company private, proprietary, and trade secret. 
 
:>
*/
/***********************************************************************/
/*                                                                     */
/*   MODULE:  map63268.h                                                */
/*   DATE:    05/30/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM63268 used by the DSP only                             */
/*                                                                     */
/***********************************************************************/
#ifndef __MAP_63268_H
#define __MAP_63268_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bcmtypesCommon.h>
#include "common63268.h"

/*
** Peripheral Controller
*/

#define BCM63268_IRQ_BITS 64
typedef struct  {
    uint64        IrqMask;
    uint64        IrqStatus;
} BCM63268IrqControl_t;

typedef struct BCM63268PerfControl {
    uint32        RevID;             /* (00) word 0 */
    uint32        blkEnables;        /* (04) word 1 */
#define BCM63268_NAND_CLK_EN      (1 << 20)
#define BCM63268_PHYMIPS_CLK_EN   (1 << 19)
#define BCM63268_FAP_CLK_EN       (1 << 18)
#define BCM63268_PCIE_CLK_EN      (1 << 17)
#define BCM63268_HS_SPI_CLK_EN    (1 << 16)
#define BCM63268_SPI_CLK_EN       (1 << 15)
#define BCM63268_IPSEC_CLK_EN     (1 << 14)
#define BCM63268_USBH_CLK_EN      (1 << 13)
#define BCM63268_USBD_CLK_EN      (1 << 12)
#define BCM63268_PCM_CLK_EN       (1 << 11)
#define BCM63268_ROBOSW_CLK_EN    (1 << 10)
#define BCM63268_SAR_CLK_EN       (1 << 9)
#define BCM63268_SWPKT_SAR_CLK_EN (1 << 8)
#define BCM63268_SWPKT_USB_CLK_EN (1 << 7)
#define BCM63268_WLAN_OCP_CLK_EN  (1 << 5)
#define BCM63268_MIPS_CLK_EN      (1 << 4)
#define BCM63268_ADSL_CLK_EN      (1 << 3)
#define BCM63268_ADSL_AFE_EN      (1 << 2)
#define BCM63268_ADSL_QPROC_EN    (1 << 1)
#define BCM63268_DISABLE_GLESS    (1 << 0)

    uint32        pll_control;       /* (08) word 2 */
#define BCM63268_SOFT_RESET              0x00000001      // 0

    uint32        deviceTimeoutEn;   /* (0c) word 3 */
    uint32        softResetB;        /* (10) word 4 */
#define BCM63268_SOFT_RST_WLAN_SHIM_UBUS (1 << 14)
#define BCM63268_SOFT_RST_FAP            (1 << 13)
#define BCM63268_SOFT_RST_DDR_PHY        (1 << 12)
#define BCM63268_SOFT_RST_WLAN_SHIM      (1 << 11)
#define BCM63268_SOFT_RST_PCIE_EXT       (1 << 10)
#define BCM63268_SOFT_RST_PCIE           (1 << 9)
#define BCM63268_SOFT_RST_PCIE_CORE      (1 << 8)
#define BCM63268_SOFT_RST_PCM            (1 << 7)
#define BCM63268_SOFT_RST_USBH           (1 << 6)
#define BCM63268_SOFT_RST_USBD           (1 << 5)
#define BCM63268_SOFT_RST_SWITCH         (1 << 4)
#define BCM63268_SOFT_RST_SAR            (1 << 3)
#define BCM63268_SOFT_RST_EPHY           (1 << 2)
#define BCM63268_SOFT_RST_IPSEC          (1 << 1)
#define BCM63268_SOFT_RST_SPI            (1 << 0)

    uint32        diagControl;        /* (14) word 5 */
    uint32        ExtIrqCfg;          /* (18) word 6*/
    uint32        unused1;            /* (1c) word 7 */
#define BCM63268_EI_SENSE_SHFT   0
#define BCM63268_EI_STATUS_SHFT  4
#define BCM63268_EI_CLEAR_SHFT   8
#define BCM63268_EI_MASK_SHFT    12
#define BCM63268_EI_INSENS_SHFT  16
#define BCM63268_EI_LEVEL_SHFT   20

     uint32        ExtdIrqMask_high;      /* (20) word 8 */
     uint32        ExtdIrqMask;           /* (24) word 9 */
     uint32        IrqMask_high;          /* (28) word 10 */
     uint32        IrqMask;               /* (2c) word 11 */
     uint32        ExtdIrqStatus_high;    /* (30) word 12 */
     uint32        ExtdIrqStatus;         /* (34) word 13 */   
     uint32        IrqStatus_high;        /* (38) word 14 */
     uint32        IrqStatus;             /* (3c) word 15 */
     
     uint32        ExtdIrqMask1_high;      /* (40) word 16 */
     uint32        ExtdIrqMask1;           /* (44) word 17 */
     uint32        IrqMask1_high;          /* (48) word 18 */
     uint32        IrqMask1;               /* (4c) word 19 */
     uint32        ExtdIrqStatus1_high;    /* (50) word 20 */
     uint32        ExtdIrqStatus1;         /* (54) word 21 */   
     uint32        IrqStatus1_high;        /* (58) word 22 */
     uint32        IrqStatus1;             /* (5c) word 23 */
     
     uint32        ExtdIrqMask2_high;      /* (60) word 24 */
     uint32        ExtdIrqMask2;           /* (64) word 25 */
     uint32        IrqMask2_high;          /* (68) word 26 */
     uint32        IrqMask2;               /* (6c) word 27 */
     uint32        ExtdIrqStatus2_high;    /* (70) word 28 */
     uint32        ExtdIrqStatus2;         /* (74) word 29 */   
     uint32        IrqStatus2_high;        /* (78) word 30 */
     uint32        IrqStatus2;             /* (7c) word 31 */

} BCM63268PerfControl;

#define BCM63268_PERF ((volatile BCM63268PerfControl * const) BCM63268_PERF_BASE)

/*
** UART
*/
typedef struct BCM63268UartChannel {
    byte          unused0;
    byte          control;
#define BCM63268_BRGEN           0x80    /* Control register bit defs */
#define BCM63268_TXEN            0x40
#define BCM63268_RXEN            0x20
#define BCM63268_LOOPBK          0x10
#define BCM63268_TXPARITYEN      0x08
#define BCM63268_TXPARITYEVEN    0x04
#define BCM63268_RXPARITYEN      0x02
#define BCM63268_RXPARITYEVEN    0x01

    byte          config;
#define BCM63268_XMITBREAK       0x40
#define BCM63268_BITS5SYM        0x00
#define BCM63268_BITS6SYM        0x10
#define BCM63268_BITS7SYM        0x20
#define BCM63268_BITS8SYM        0x30
#define BCM63268_ONESTOP         0x07
#define BCM63268_TWOSTOP         0x0f
    /* 4-LSBS represent STOP bits/char
     * in 1/8 bit-time intervals.  Zero
     * represents 1/8 stop bit interval.
     * Fifteen represents 2 stop bits.
     */
    byte          fifoctl;
#define BCM63268_RSTTXFIFOS      0x80
#define BCM63268_RSTRXFIFOS      0x40
    /* 5-bit TimeoutCnt is in low bits of this register.
     *  This count represents the number of characters
     *  idle times before setting receive Irq when below threshold
     */
    uint32        baudword;
    /* When divide SysClk/2/(1+baudword) we should get 32*bit-rate
     */

    byte          txf_levl;       /* Read-only fifo depth */
    byte          rxf_levl;       /* Read-only fifo depth */
    byte          fifocfg;        /* Upper 4-bits are TxThresh, Lower are
                                   *      RxThreshold.  Irq can be asserted
                                   *      when rx fifo> thresh, txfifo<thresh
                                   */
    byte          prog_out;       /* Set value of DTR (Bit0), RTS (Bit1)
                                   *  if these bits are also enabled to GPIO_o
                                   */
#define BCM63268_DTREN   0x01
#define BCM63268_RTSEN   0x02

    byte          unused1;
    byte          DeltaIPEdgeNoSense;     /* Low 4-bits, set corr bit to 1 to
                                           * detect irq on rising AND falling
                                           * edges for corresponding GPIO_i
                                           * if enabled (edge insensitive)
                                           */
    byte          DeltaIPConfig_Mask;     /* Upper 4 bits: 1 for posedge sense
                                           *      0 for negedge sense if
                                           *      not configured for edge
                                           *      insensitive (see above)
                                           * Lower 4 bits: Mask to enable change
                                           *  detection IRQ for corresponding
                                           *  GPIO_i
                                           */
    byte          DeltaIP_SyncIP;         /* Upper 4 bits show which bits
                                           *  have changed (may set IRQ).
                                           *  read automatically clears bit
                                           * Lower 4 bits are actual status
                                           */

    uint16        intMask;                /* Same Bit defs for Mask and status */
    uint16        intStatus;
#define BCM63268_DELTAIP         0x0001
#define BCM63268_TXUNDERR        0x0002
#define BCM63268_TXOVFERR        0x0004
#define BCM63268_TXFIFOTHOLD     0x0008
#define BCM63268_TXREADLATCH     0x0010
#define BCM63268_TXFIFOEMT       0x0020
#define BCM63268_RXUNDERR        0x0040
#define BCM63268_RXOVFERR        0x0080
#define BCM63268_RXTIMEOUT       0x0100
#define BCM63268_RXFIFOFULL      0x0200
#define BCM63268_RXFIFOTHOLD     0x0400
#define BCM63268_RXFIFONE        0x0800
#define BCM63268_RXFRAMERR       0x1000
#define BCM63268_RXPARERR        0x2000
#define BCM63268_RXBRK           0x4000

    uint16        unused2;
    uint16        Data;                   /* Write to TX, Read from RX */
                                          /* bits 11:8 are BRK,PAR,FRM errors */

    uint32        unused3;
    uint32        unused4;
} BCM63268Uart;

#define BCM63268_UART ((volatile BCM63268Uart * const) BCM63268_UART_BASE)

/*
** Gpio Controller
*/

typedef struct BCM63268GpioControl {
    uint32      unused1;                    /* 00 */
    uint32      GPIODir;                    /* 04 */
    uint32      unused2;                    /* 08 */
    uint32      GPIOio;                     /* 0C */
    uint32      LEDCtrl;                    /* 10 */
    uint32      SpiSlaveCfg;                /* 14 */
    uint32      GPIOMode;                   /* 18 */
#define BCM63268_GPIO_MODE_EXT_IRQ3          (1<<27)
#define BCM63268_GPIO_MODE_EXT_IRQ2          (1<<26)
#define BCM63268_GPIO_MODE_EXT_IRQ1          (1<<25)
#define BCM63268_GPIO_MODE_EXT_IRQ0          (1<<24)
#define BCM63268_GPIO_MODE_EPHY3_LED         (1<<23)
#define BCM63268_GPIO_MODE_EPHY2_LED         (1<<22)
#define BCM63268_GPIO_MODE_EPHY1_LED         (1<<21)
#define BCM63268_GPIO_MODE_EPHY0_LED         (1<<20)
#define BCM63268_GPIO_MODE_ADSL_SPI_SSB      (1<<19)
#define BCM63268_GPIO_MODE_ADSL_SPI_CLK      (1<<18)
#define BCM63268_GPIO_MODE_ADSL_SPI_MOSI     (1<<17)
#define BCM63268_GPIO_MODE_ADSL_SPI_MISO     (1<<16)
#define BCM63268_GPIO_MODE_UART2_SDOUT       (1<<15)
#define BCM63268_GPIO_MODE_UART2_SDIN        (1<<14)
#define BCM63268_GPIO_MODE_UART2_SRTS        (1<<13)
#define BCM63268_GPIO_MODE_UART2_SCTS        (1<<12)
#define BCM63268_GPIO_MODE_NTR_PULSE         (1<<11)
#define BCM63268_GPIO_MODE_LS_SPIM_SSB3      (1<<10)
#define BCM63268_GPIO_MODE_LS_SPIM_SSB2      (1<<9)
#define BCM63268_GPIO_MODE_INET_LED          (1<<8)
#define BCM63268_GPIO_MODE_ROBOSW_LED1       (1<<7)
#define BCM63268_GPIO_MODE_ROBOSW_LED0       (1<<6)
#define BCM63268_GPIO_MODE_ROBOSW_LED_CLK    (1<<5)
#define BCM63268_GPIO_MODE_ROBOSW_LED_DATA   (1<<4)
#define BCM63268_GPIO_MODE_SYS_IRQ           (1<<3)
#define BCM63268_GPIO_MODE_SERIAL_LED_CLK    (1<<2)
#define BCM63268_GPIO_MODE_SERIAL_LED_DATA   (1<<1)
#define BCM63268_GPIO_MODE_USBD_LED          (1<<0)

    uint32      unused3[3];                 /* 1C - 24*/

    uint32      TestControl;                /* 28 */

    uint32      OscControl;                 /* 2C */
    uint32      RoboSWLEDControl;           /* 30 */
    uint32      RoboSWLEDLSR;               /* 34 */
    uint32      GPIOBaseMode;               /* 38 */
#define BCM63268_NAND_GPIO_OVERRIDE      (1<<2)
    uint32      RoboswEphyCtrl;             /* 3C */
#define BCM63268_EPHY_PLL_LOCK           (1<<27)
#define BCM63268_EPHY_ATEST_25MHZ_EN     (1<<26)
#define BCM63268_EPHY_PWR_DOWN_DLL       (1<<25)
#define BCM63268_EPHY_PWR_DOWN_BIAS      (1<<24)
#define BCM63268_EPHY_PWR_DOWN_TX_4      (1<<23)
#define BCM63268_EPHY_PWR_DOWN_TX_3      (1<<22)
#define BCM63268_EPHY_PWR_DOWN_TX_2      (1<<21)
#define BCM63268_EPHY_PWR_DOWN_TX_1      (1<<20)
#define BCM63268_EPHY_PWR_DOWN_RX_4      (1<<19)
#define BCM63268_EPHY_PWR_DOWN_RX_3      (1<<18)
#define BCM63268_EPHY_PWR_DOWN_RX_2      (1<<17)
#define BCM63268_EPHY_PWR_DOWN_RX_1      (1<<16)
#define BCM63268_EPHY_PWR_DOWN_SD_4      (1<<15)
#define BCM63268_EPHY_PWR_DOWN_SD_3      (1<<14)
#define BCM63268_EPHY_PWR_DOWN_SD_2      (1<<13)
#define BCM63268_EPHY_PWR_DOWN_SD_1      (1<<12)
#define BCM63268_EPHY_PWR_DOWN_RD_4      (1<<11)
#define BCM63268_EPHY_PWR_DOWN_RD_3      (1<<10)
#define BCM63268_EPHY_PWR_DOWN_RD_2      (1<<9)
#define BCM63268_EPHY_PWR_DOWN_RD_1      (1<<8)
#define BCM63268_EPHY_PWR_DOWN_4         (1<<7)
#define BCM63268_EPHY_PWR_DOWN_3         (1<<6)
#define BCM63268_EPHY_PWR_DOWN_2         (1<<5)
#define BCM63268_EPHY_PWR_DOWN_1         (1<<4)
#define BCM63268_EPHY_RST_4              (1<<3)
#define BCM63268_EPHY_RST_3              (1<<2)
#define BCM63268_EPHY_RST_2              (1<<1)
#define BCM63268_EPHY_RST_1              (1<<0)
    uint32      RoboswSwitchCtrl;           /* 40 */
#define BCM63268_RSW_SPI_MODE            (1<<11)
#define BCM63268_RSW_BC_SUPP_EN          (1<<10)
#define BCM63268_RSW_CLK_FREQ_MASK       (3<<8)
#define BCM63268_RSW_ENF_DFX_FLOW        (1<<7)
#define BCM63268_RSW_ENH_DFX_FLOW        (1<<6)
#define BCM63268_RSW_GRX_0_SETUP         (1<<5)
#define BCM63268_RSW_GTX_0_SETUP         (1<<4)
#define BCM63268_RSW_HW_FWDG_EN          (1<<3)
#define BCM63268_RSW_QOS_EN              (1<<2)
#define BCM63268_RSW_WD_CLR_EN           (1<<1)
#define BCM63268_RSW_MII_DUMP_FWDG       (1<<0)

    uint32      RegFileTmCtl;               /* 44 */
    uint32      RingOscCtrl0;               /* 48 */
    uint32      RingOscCtrl1;               /* 4C */

    uint32      unused4[6];                 /* 50 - 64 */
    uint32      DieRevID;                   /* 68 */
    uint32      unused5;                    /* 6c */
    uint32      DiagSelControl;             /* 70 */
    uint32      DiagReadBack;               /* 74 */
    uint32      DiagReadBackHi;             /* 78 */
    uint32      DiagMiscControl;            /* 7c */
} BCM63268GpioControl;

#define BCM63268_GPIO ((volatile BCM63268GpioControl * const) BCM63268_GPIO_BASE)

/* Number to mask conversion macro used for GPIODir and GPIOio */
#define BCM63268_GPIO_NUM_TOTAL_BITS_MASK        0x3f
#define BCM63268_GPIO_NUM_MAX_BITS_MASK          0x1f
#define BCM63268_GPIO_NUM_TO_MASK(X)             ( (((X) & BCM63268_GPIO_NUM_TOTAL_BITS_MASK) < 32) ? (1 << ((X) & BCM63268_GPIO_NUM_MAX_BITS_MASK)) : (0) )

/* Number to mask conversion macro used for GPIODir_high and GPIOio_high */
#define BCM63268_GPIO_NUM_MAX_BITS_MASK_HIGH     0x07
#define BCM63268_GPIO_NUM_TO_MASK_HIGH(X)        ( (((X) & BCM63268_GPIO_NUM_TOTAL_BITS_MASK) >= 32) ? (1 << ((X-32) & BCM63268_GPIO_NUM_MAX_BITS_MASK_HIGH)) : (0) )

/*
** Misc Register Set Definitions.
*/

typedef struct BCM63268Misc {
    uint32  unused1;                            /* 0x00 */
    uint32  miscSerdesCtrl;                     /* 0x04 */
    uint32  miscSerdesSts;                      /* 0x08 */
    uint32  miscIrqOutMask;                     /* 0x0C */
#define BCM63268_MISC_PCIE_EP_IRQ_MASK0                  (1<<0)
#define BCM63268_MISC_PCIE_EP_IRQ_MASK1                  (1<<1)

    uint32  miscMemcControl;                    /* 0x10 */
#define BCM63268_MISC_MEMC_CONTROL_MC_UBUS_ASYNC_MODE    (1<<3)
#define BCM63268_MISC_MEMC_CONTROL_MC_LMB_ASYNC_MODE     (1<<2)
#define BCM63268_MISC_MEMC_CONTROL_DDR_TEST_DONE         (1<<1)
#define BCM63268_MISC_MEMC_CONTROL_DDR_TEST_DISABLE      (1<<0)

    uint32  miscStrapBus;                       /* 0x14 */
#define BCM63268_MISC_STRAP_BUS_RESET_CFG_DELAY          (1<<18)
#define BCM63268_MISC_STRAP_BUS_RESET_OUT_SHIFT          16
#define BCM63268_MISC_STRAP_BUS_RESET_OUT_MASK           (3<<BCM63268_MISC_STRAP_BUS_RESET_OUT_SHIFT)
#define BCM63268_MISC_STRAP_BUS_SELECT_SPI_BOOT          (1<<15)
#define BCM63268_MISC_STRAP_BUS_HS_SPIM_24B_N_32B_ADDR   (1<<14)
#define BCM63268_MISC_STRAP_BUS_HS_SPIM_24B_N_32B_ADDR   (1<<14)
#define BCM63268_MISC_STRAP_BUS_HS_SPIM_CLK_SLOW_N_FAST  (1<<13)
#define BCM63268_MISC_STRAP_BUS_LS_SPIM_CLK_FAST_N_SLOW  (1<<12)
#define BCM63268_MISC_STRAP_BUS_LS_SPI_MASTER_N_SLAVE    (1<<11)
#define BCM63268_MISC_STRAP_BUS_PLL_USE_LOCK             (1<<10)
#define BCM63268_MISC_STRAP_BUS_PLL_MIPS_WAIT_FAST_N     (1<<9)
#define BCM63268_MISC_STRAP_BUS_ROBOSW_P4_MODE_SHIFT     7
#define BCM63268_MISC_STRAP_BUS_ROBOSW_P4_MODE_MASK      (3<<BCM63268_MISC_STRAP_BUS_ROBOSW_P4_MODE_SHIFT)
#define BCM63268_MISC_STRAP_BUS_HARD_RESET_DELAY         (1<<6)
#define BCM63268_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT      1
#define BCM63268_MISC_STRAP_BUS_MIPS_PLL_FVCO_MASK       (0x1F<<BCM63268_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT)
#define BCM63268_MISC_STRAP_BUS_PCIE_ROOT_COMPLEX        (1<<0)

    uint32  miscStrapOverride;              /* 0x18 */
    uint32  miscVregCtrl0;                  /* 0x1C */
    uint32  miscVregCtrl1;                  /* 0x20 */
    uint32  miscVregCtrl2;                  /* 0x24 */
    uint32  miscExtra2ChipsIrqMask;         /* 0x28 */
    uint32  miscExtra2ChipsIrqSts;          /* 0x2C */
    uint32  miscExtra2ChipsIrqMask1;        /* 0x30 */
    uint32  miscExtra2ChipsIrqSts1;         /* 0x34 */
    uint32  miscFapIrqMask;                 /* 0x38 */
    uint32  miscExtraFapIrqMask;            /* 0x3C */
    uint32  miscExtra2FapIrqMask;           /* 0x40 */
    uint32  miscAdsl_clock_sample;          /* 0x44 */
    uint32  miscIddq_control;               /* 0x48 */
    uint32  miscSleep;                      /* 0x4C */
    uint32  miscRtc_enable;                 /* 0x50 */
    uint32  miscRtc_count_L;                /* 0x54 */
    uint32  miscRtc_count_H;                /* 0x58 */
    uint32  miscRtc_event;                  /* 0x5C */
    uint32  miscWakeup_mask;                /* 0x60 */
    uint32  miscWakeup_status;              /* 0x64 */
} BCM63268Misc;

#define BCM63268_MISC ((volatile BCM63268Misc * const) BCM63268_MISC_BASE)

/*
** Pcm Controller
*/

typedef struct BCM63268PcmControlRegisters
{
    uint32 pcm_ctrl;                                    // 00 offset from PCM_BASE
#define  BCM63268_PCM_ENABLE              0x80000000     // PCM block master enable
#define  BCM63268_PCM_ENABLE_SHIFT        31
#define  BCM63268_PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  BCM63268_PCM_SLAVE_SEL_SHIFT     30
#define  BCM63268_PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  BCM63268_PCM_CLOCK_INV_SHIFT     29
#define  BCM63268_PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  BCM63268_PCM_FS_INVERT_SHIFT     28
#define  BCM63268_PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  BCM63268_PCM_FS_FREQ_16_8_SHIFT  27
#define  BCM63268_PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  BCM63268_PCM_FS_LONG_SHIFT       26
#define  BCM63268_PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  BCM63268_PCM_FS_TRIG_SHIFT       25
#define  BCM63268_PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  BCM63268_PCM_DATA_OFF_SHIFT      24
#define  BCM63268_PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  BCM63268_PCM_DATA_16_8_SHIFT     23
#define  BCM63268_PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  BCM63268_PCM_CLOCK_SEL_SHIFT     20
                                                        // 000 - 8192 Khz
                                                        // 001 - 4096 Khz
                                                        // 010 - 2048 Khz
                                                        // 011 - 1024 Khz
                                                        // 100 - 512 Khz
                                                        // 101 - 256 Khz
                                                        // 110 - 128 Khz
                                                        // 111 - reserved
#define  BCM63268_PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  BCM63268_PCM_LSB_FIRST_SHIFT     18
#define  BCM63268_PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  BCM63268_PCM_LOOPBACK_SHIFT      17
#define  BCM63268_PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in 63268
#define  BCM63268_PCM_EXTCLK_SEL_SHIFT    16
#define  BCM63268_PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 63268
#define  BCM63268_PCM_NTR_ENABLE_SHIFT    15
#define  BCM63268_PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  BCM63268_PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  BCM63268_PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min

    uint32 pcm_chan_ctrl;                       // 04
#define  BCM63268_PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  BCM63268_PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  BCM63268_PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  BCM63268_PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  BCM63268_PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  BCM63268_PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  BCM63268_PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  BCM63268_PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  BCM63268_PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  BCM63268_PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  BCM63268_PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  BCM63268_PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  BCM63268_PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  BCM63268_PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  BCM63268_PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  BCM63268_PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  BCM63268_PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  BCM63268_PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define  BCM63268_PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  BCM63268_PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  BCM63268_PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  BCM63268_PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  BCM63268_PCM_TX_IRQ              0x00000010

    uint32 pcm_pll_ctrl1;                       // 10
#define  BCM63268_PCM_PLL_PWRDN           0x80000000     // PLL PWRDN
#define  BCM63268_PCM_PLL_PWRDN_CH1       0x40000000     // PLL CH PWRDN
#define  BCM63268_PCM_PLL_REFCMP_PWRDN    0x20000000     // PLL REFCMP PWRDN
#define  BCM63268_PCM_CLK16_RESET         0x10000000     // 16.382 MHz PCM interface circuitry reset. 
#define  BCM63268_PCM_PLL_ARESET          0x08000000     // PLL Analog Reset
#define  BCM63268_PCM_PLL_DRESET          0x04000000     // PLL Digital Reset

    uint32 pcm_pll_ctrl2;                       // 14
    uint32 pcm_pll_ctrl3;                       // 18
    uint32 pcm_pll_ctrl4;                       // 1c

    uint32 pcm_pll_stat;                        // 20
#define  BCM63268_PCM_PLL_LOCK            0x00000001     // Asserted when PLL is locked to programmed frequency

    uint32 pcm_ntr_counter;                     // 24

    uint32 unused[6];
#define  BCM63268_PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[BCM63268_PCM_MAX_TIMESLOT_REGS];
#define  BCM63268_PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} BCM63268PcmControlRegisters;

#define BCM63268_PCM ((volatile BCM63268PcmControlRegisters * const) BCM63268_PCM_BASE)


typedef struct BCM63268PcmIudmaRegisters
{
    uint16 reserved0;
    uint16 ctrlConfig;
#define BCM63268_IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define BCM63268_IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define BCM63268_IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define BCM63268_IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define BCM63268_IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

    // Flow control Ch1
    uint16 reserved1;
    uint16 ch1_FC_Low_Thr;

    uint16 reserved2;
    uint16 ch1_FC_High_Thr;

    uint16 reserved3;
    uint16 ch1_Buff_Alloc;

    // Flow control Ch3
    uint16 reserved4;
    uint16 ch3_FC_Low_Thr;

    uint16 reserved5;
    uint16 ch3_FC_High_Thr;

    uint16 reserved6;
    uint16 ch3_Buff_Alloc;

    // Flow control Ch5
    uint16 reserved7;
    uint16 ch5_FC_Low_Thr;

    uint16 reserved8;
    uint16 ch5_FC_High_Thr;

    uint16 reserved9;
    uint16 ch5_Buff_Alloc;

    // Flow control Ch7
    uint16 reserved10;
    uint16 ch7_FC_Low_Thr;

    uint16 reserved11;
    uint16 ch7_FC_High_Thr;

    uint16 reserved12;
    uint16 ch7_Buff_Alloc;

    // Channel resets
    uint16 reserved13;
    uint16 channel_reset;
    
    uint16 reserved14;
    uint16 channel_debug;
    
    // Spare register
    uint32 dummy1;
    
    // Interrupt status registers
    uint16 reserved15;
    uint16 gbl_int_stat;
    
    // Interrupt mask registers
    uint16 reserved16;
    uint16 gbl_int_mask;
} BCM63268PcmIudmaRegisters;


typedef struct BCM63268PcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM63268_IUDMA_CONFIG_ENDMA       0x0001
#define BCM63268_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM63268_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM63268_IUDMA_INTSTAT_BDONE   0x0001
#define BCM63268_IUDMA_INTSTAT_PDONE   0x0002
#define BCM63268_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM63268_IUDMA_INTSTAT_MASK    0x0007
#define BCM63268_IUDMA_INTSTAT_ALL     BCM63268_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM63268_IUDMA_INTMASK_BDONE   0x0001
#define BCM63268_IUDMA_INTMASK_PDONE   0x0002
#define BCM63268_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM63268_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} BCM63268PcmIudmaChannelCtrl;


typedef struct BCM63268PcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM63268_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} BCM63268PcmIudmaStateRam;

#define BCM63268_MAX_PCM_DMA_CHANNELS 2

typedef struct BCM63268PcmIudma
{
   BCM63268PcmIudmaRegisters regs;                                        // 
   uint32 reserved1[110];                                         //            
   BCM63268PcmIudmaChannelCtrl ctrl[BCM63268_MAX_PCM_DMA_CHANNELS];        //
   uint32 reserved2[120];                                         //
   BCM63268PcmIudmaStateRam stram[BCM63268_MAX_PCM_DMA_CHANNELS];          //

} BCM63268PcmIudma;

#define BCM63268_PCM_IUDMA ((volatile BCM63268PcmIudma * const) BCM63268_PCM_DMA_BASE)


#define BCM63268_IUDMA_MAX_CHANNELS          32

/*
** DMA Channel Configuration (1 .. 32)
*/
typedef struct BCM63268DmaChannelCfg {
  uint32        cfg;                    /* (00) assorted configuration */
#define         BCM63268_DMA_ENABLE      0x00000001  /* set to enable channel */
#define         BCM63268_DMA_PKT_HALT    0x00000002  /* idle after an EOP flag is detected */
#define         BCM63268_DMA_BURST_HALT  0x00000004  /* idle after finish current memory burst */
  uint32        intStat;                /* (04) interrupts control and status */
  uint32        intMask;                /* (08) interrupts mask */
#define         BCM63268_DMA_BUFF_DONE   0x00000001  /* buffer done */
#define         BCM63268_DMA_DONE        0x00000002  /* packet xfer complete */
#define         BCM63268_DMA_NO_DESC     0x00000004  /* no valid descriptors */
  uint32        maxBurst;               /* (0C) max burst length permitted */
#define         BCM63268_DMA_DESCSIZE_SEL 0x00040000  /* DMA Descriptor Size Selection */
} BCM63268DmaChannelCfg;

/*
** DMA State RAM (1 .. 16)
*/
typedef struct BCM63268DmaStateRam {
  uint32        baseDescPtr;            /* (00) descriptor ring start address */
  uint32        state_data;             /* (04) state/bytes done/ring offset */
  uint32        desc_len_status;        /* (08) buffer descriptor status and len */
  uint32        desc_base_bufptr;       /* (0C) buffer descrpitor current processing */
} BCM63268DmaStateRam;


/*
** DMA Registers
*/
typedef struct BCM63268DmaRegs {
    uint32 controller_cfg;              /* (00) controller configuration */
#define BCM63268_DMA_MASTER_EN           0x00000001
#define BCM63268_DMA_FLOWC_CH1_EN        0x00000002
#define BCM63268_DMA_FLOWC_CH3_EN        0x00000004

    // Flow control Ch1
    uint32 flowctl_ch1_thresh_lo;           /* 004 */
    uint32 flowctl_ch1_thresh_hi;           /* 008 */
    uint32 flowctl_ch1_alloc;               /* 00c */
#define BCM63268_DMA_BUF_ALLOC_FORCE     0x80000000

    // Flow control Ch3
    uint32 flowctl_ch3_thresh_lo;           /* 010 */
    uint32 flowctl_ch3_thresh_hi;           /* 014 */
    uint32 flowctl_ch3_alloc;               /* 018 */

    // Flow control Ch5
    uint32 flowctl_ch5_thresh_lo;           /* 01C */
    uint32 flowctl_ch5_thresh_hi;           /* 020 */
    uint32 flowctl_ch5_alloc;               /* 024 */

    // Flow control Ch7
    uint32 flowctl_ch7_thresh_lo;           /* 028 */
    uint32 flowctl_ch7_thresh_hi;           /* 02C */
    uint32 flowctl_ch7_alloc;               /* 030 */

    uint32 ctrl_channel_reset;              /* 034 */
    uint32 ctrl_channel_debug;              /* 038 */
    uint32 reserved1;                       /* 03C */
    uint32 ctrl_global_interrupt_status;    /* 040 */
    uint32 ctrl_global_interrupt_mask;      /* 044 */

    // Unused words
    uint8 reserved2[0x200-0x48];

    // Per channel registers/state ram
    BCM63268DmaChannelCfg chcfg[BCM63268_IUDMA_MAX_CHANNELS];/* (200-3FF) Channel configuration */
    union {
        BCM63268DmaStateRam     s[BCM63268_IUDMA_MAX_CHANNELS];
        uint32                 u32[4 * BCM63268_IUDMA_MAX_CHANNELS];
    } stram;                                /* (400-5FF) state ram */
} BCM63268DmaRegs;

#define BCM63268_SW_DMA ((volatile BCM63268DmaRegs * const) BCM63268_SWITCH_DMA_BASE)

/*
** DMA Buffer
*/
typedef struct BCM63268DmaDesc {
  uint16        length;                 /* in bytes of data in buffer */
#define          BCM63268_DMA_DESC_USEFPM    0x8000
#define          BCM63268_DMA_DESC_MULTICAST 0x4000
#define          BCM63268_DMA_DESC_BUFLENGTH 0x0fff
  uint16        status;                 /* buffer status */
#define          BCM63268_DMA_OWN                0x8000  /* cleared by DMA, set by SW */
#define          BCM63268_DMA_EOP                0x4000  /* last buffer in packet */
#define          BCM63268_DMA_SOP                0x2000  /* first buffer in packet */
#define          BCM63268_DMA_WRAP               0x1000  /* */
#define          BCM63268_DMA_PRIO               0x0C00  /* Prio for Tx */
#define          BCM63268_DMA_APPEND_BRCM_TAG    0x0200
#define          BCM63268_DMA_APPEND_CRC         0x0100
#define          BCM63268_USB_ZERO_PKT           (1<< 0) // Set to send zero length packet

  uint32        address;                /* address of data */
} BCM63268DmaDesc;

/*
** 16 Byte DMA Buffer 
*/
typedef struct {
  uint16        length;                 /* in bytes of data in buffer */
#define          BCM63268_DMA_DESC_USEFPM        0x8000
#define          BCM63268_DMA_DESC_MULTICAST     0x4000
#define          BCM63268_DMA_DESC_BUFLENGTH     0x0fff
  uint16        status;                 /* buffer status */
#define          BCM63268_DMA_OWN                0x8000  /* cleared by DMA, set by SW */
#define          BCM63268_DMA_EOP                0x4000  /* last buffer in packet */
#define          BCM63268_DMA_SOP                0x2000  /* first buffer in packet */
#define          BCM63268_DMA_WRAP               0x1000  /* */
#define          BCM63268_DMA_PRIO               0x0C00  /* Prio for Tx */
#define          BCM63268_DMA_APPEND_BRCM_TAG    0x0200
#define          BCM63268_DMA_APPEND_CRC         0x0100
#define          BCM63268_USB_ZERO_PKT           (1<< 0) // Set to send zero length packet

  uint32        address;                 /* address of data */
  uint32        control;
#define         BCM63268_GEM_ID_MASK             0x001F
  uint32        reserved;
} BCM63268DmaDesc16;

/*
** DECT IP Device Registers
*/
typedef enum  BCM63268_DECT_SHM_ENABLE_BITS
{
   BCM63268_DECT_SHM_IRQ_DSP_INT,
   BCM63268_DECT_SHM_IRQ_DSP_IRQ_OUT,
   BCM63268_DECT_SHM_IRQ_DIP_INT,
   BCM63268_DECT_SHM_H2D_BUS_ERR,
   BCM63268_DECT_SHM_IRQ_TX_DMA_DONE,                 
   BCM63268_DECT_SHM_IRQ_RX_DMA_DONE,
   BCM63268_DECT_SHM_IRQ_PLL_PHASE_LOCK = BCM63268_DECT_SHM_IRQ_RX_DMA_DONE + 2, /* Skip reserved bit */
   BCM63268_DECT_SHM_REG_DSP_BREAK,
   BCM63268_DECT_SHM_REG_DIP_BREAK,
   BCM63268_DECT_SHM_REG_IRQ_TO_IP = BCM63268_DECT_SHM_REG_DIP_BREAK + 2, /* Skip reserved bit */
   BCM63268_DECT_SHM_TX_DMA_DONE_TO_IP,
   BCM63268_DECT_SHM_RX_DMA_DONE_TO_IP, 
} BCM63268_DECT_SHM_ENABLE_BITS;

typedef struct BCM63268DECTShimControl 
{
   uint32 dect_shm_ctrl;                     /*  0xb000b000  DECT shim control registers                    */
#define BCM63268_APB_SWAP_MASK             0x0000C000
#define BCM63268_APB_SWAP_16_BIT           0x00000000
#define BCM63268_APB_SWAP_8_BIT            0x00004000
#define BCM63268_AHB_SWAP_MASK             0x00003000
#define BCM63268_AHB_SWAP_16_BIT           0x00003000
#define BCM63268_AHB_SWAP_8_BIT            0x00002000
#define BCM63268_AHB_SWAP_ACCESS           0x00001000
#define BCM63268_AHB_SWAP_NONE             0x00000000
#define BCM63268_DECT_PULSE_COUNT_ENABLE   0x00000200
#define BCM63268_PCM_PULSE_COUNT_ENABLE    0x00000100
#define BCM63268_DECT_SOFT_RESET           0x00000010
#define BCM63268_PHCNT_CLK_SRC_PLL         0x00000008
#define BCM63268_PHCNT_CLK_SRC_XTAL        0x00000000
#define BCM63268_DECT_CLK_OUT_PLL          0x00000004
#define BCM63268_DECT_CLK_OUT_XTAL         0x00000000
#define BCM63268_DECT_CLK_CORE_PCM         0x00000002
#define BCM63268_DECT_CLK_CORE_DECT        0x00000000
#define BCM63268_DECT_PLL_REF_PCM          0x00000001
#define BCM63268_DECT_PLL_REF_DECT         0x00000000

   uint32 dect_shm_pcm_clk_cntr;             /*  0xb000b004  PCM clock counter                              */
   uint32 dect_shm_dect_clk_cntr;            /*  0xb000b008  DECT clock counter                             */
   uint32 dect_shm_dect_clk_cntr_sh;         /*  0xb000b00c  DECT clock counter snapshot                    */
   uint32 dect_shm_irq_enable;               /*  0xb000b010  DECT interrupt enable register                 */
   uint32 dect_shm_irq_status;               /*  0xb000b014  DECT Interrupt status register                 */
   uint32 dect_shm_irq_trig;                 /*  0xb000b018  DECT DSP ext IRQ trigger and IRQ test register */
   uint32 dect_shm_dma_status;               /*  0xb000b01c  DECT DMA STATUS register                       */
   uint32 dect_shm_xtal_ctrl;                /*  0xb000b020  DECT analog tunable XTAL control register      */
   uint32 dect_shm_bandgap_ctrl;             /*  0xb000b024  DECT analog bandgap control register           */
   uint32 dect_shm_afe_tx_ctrl;              /*  0xb000b028  DECT analog TX DAC control register            */
   uint32 dect_shm_afe_test_ctrl;            /*  0xb000b02c  DECT analog test control register              */
   uint32 dect_shm_pll_reg_0;                /*  0xb000b030  DECT PLL configuration register 0              */
   uint32 dect_shm_pll_reg_1;                /*  0xb000b034  DECT PLL configuration register 1              */
#define  BCM63268_PLL_PWRDWN                          0x01000000   
#define  BCM63268_PLL_REFCOMP_PWRDOWN                 0x02000000
#define  BCM63268_PLL_NDIV_PWRDOWN                    0x04000000
#define  BCM63268_PLL_CH1_PWRDOWN                     0x08000000
#define  BCM63268_PLL_CH2_PWRDOWN                     0x10000000
#define  BCM63268_PLL_CH3_PWRDOWN                     0x20000000
#define  BCM63268_PLL_DRESET                          0x40000000
#define  BCM63268_PLL_ARESET                          0x80000000

   uint32 dect_shm_pll_reg_2;                /*  0xb000b038  DECT PLL Ndiv configuration register           */
   uint32 dect_shm_pll_reg_3;                /*  0xb000b03c  DECT PLL Pdiv and Mdiv configuration register  */
} BCM63268DECTShimControl;

#define BCM63268_DECT_CTRL ((volatile BCM63268DECTShimControl * const) BCM63268_DECT_SHIM_CTRL_BASE)

typedef struct BCM63268DECTShimDmaControl 
{
   uint32 dect_shm_dma_ctrl;                 /*  0xb000b050  DECT DMA control register                      */
#define  BCM63268_DMA_CLEAR                           0x80000000
#define  BCM63268_DMA_SWAP_16_BIT                     0x03000000
#define  BCM63268_DMA_SWAP_8_BIT                      0x02000000
#define  BCM63268_DMA_SWAP_NONE                       0x01000000
#define  BCM63268_DMA_SUBWORD_SWAP_MASK               0x03000000
#define  BCM63268_TRIG_CNT_CLK_SEL_PCM                0x00800000
#define  BCM63268_TRIG_CNT_IRQ_EN                     0x00400000
#define  BCM63268_RX_CNT_TRIG_EN                      0x00200000   
#define  BCM63268_TX_CNT_TRIG_EN                      0x00100000 
#define  BCM63268_RX_INT_TRIG_EN                      0x00080000 	
#define  BCM63268_TX_INT_TRIG_EN                      0x00040000 	
#define  BCM63268_RX_REG_TRIG_EN                      0x00020000 	
#define  BCM63268_TX_REG_TRIG_EN                      0x00010000 
#define  BCM63268_RX_TRIG_FIRST                       0x00008000
#define  BCM63268_MAX_BURST_CYCLE_MASK                0x00001F00
#define  BCM63268_MAX_BURST_CYCLE_SHIFT               8
#define  BCM63268_RX_EN_3                             0x00000080   
#define  BCM63268_RX_EN_2                             0x00000040   
#define  BCM63268_RX_EN_1                             0x00000020   
#define  BCM63268_RX_EN_0                             0x00000010   
#define  BCM63268_TX_EN_3                             0x00000008   
#define  BCM63268_TX_EN_2                             0x00000004   
#define  BCM63268_TX_EN_1                             0x00000002   
#define  BCM63268_TX_EN_0                             0x00000001   
 
   uint32 dect_shm_dma_trig_cnt_preset;      /*  0xb000b054  DECT DMA trigger counter preset value                */
   uint32 dect_shm_dma_ddr_saddr_tx_s0;      /*  0xb000b058  DECT DMA DDR buffer starting address for TX slot 0   */
   uint32 dect_shm_dma_ddr_saddr_tx_s1;      /*  0xb000b05c  DECT DMA DDR buffer starting address for TX slot 1   */
   uint32 dect_shm_dma_ddr_saddr_tx_s2;      /*  0xb000b060  DECT DMA DDR buffer starting address for TX slot 2   */
   uint32 dect_shm_dma_ddr_saddr_tx_s3;      /*  0xb000b064  DECT DMA DDR buffer starting address for TX slot 3   */
   uint32 dect_shm_dma_ddr_saddr_rx_s0;      /*  0xb000b068  DECT DMA DDR buffer starting address for RX slot 0   */
   uint32 dect_shm_dma_ddr_saddr_rx_s1;      /*  0xb000b06c  DECT DMA DDR buffer starting address for RX slot 1   */
   uint32 dect_shm_dma_ddr_saddr_rx_s2;      /*  0xb000b070  DECT DMA DDR buffer starting address for RX slot 2   */
   uint32 dect_shm_dma_ddr_saddr_rx_s3;      /*  0xb000b074  DECT DMA DDR buffer starting address for RX slot 3   */
   uint32 dect_shm_dma_ahb_saddr_tx_s01;     /*  0xb000b078  DECT DMA AHB shared memory buffer starting address for TX slots 0 and 1  */
   uint32 dect_shm_dma_ahb_saddr_tx_s23;     /*  0xb000b07c  DECT DMA AHB shared memory buffer starting address for TX slots 2 and 3  */
   uint32 dect_shm_dma_ahb_saddr_rx_s01;     /*  0xb000b080  DECT DMA AHB shared memory buffer starting address for RX slots 0 and 1  */
   uint32 dect_shm_dma_ahb_saddr_rx_s23;     /*  0xb000b084  DECT DMA AHB shared memory buffer starting address for RX slots 2 and 3  */
   uint32 dect_shm_dma_xfer_size_tx;         /*  0xb000b088  DECT DMA TX slots transfer size of each trigger      */
   uint32 dect_shm_dma_xfer_size_rx;         /*  0xb000b08c  DECT DMA RX slots transfer size of each trigger      */
   uint32 dect_shm_dma_buf_size_tx;          /*  0xb000b090  DECT DMA TX slots memory buffer size                 */
   uint32 dect_shm_dma_buf_size_rx;          /*  0xb000b094  DECT DMA RX slots memory buffer size                 */
   uint32 dect_shm_dma_offset_addr_tx_s01;   /*  0xb000b098  DECT DMA access offset address for TX slots 0 and 1  */
   uint32 dect_shm_dma_offset_addr_tx_s23;   /*  0xb000b09c  DECT DMA access offset address for TX slots 2 and 3  */
   uint32 dect_shm_dma_offset_addr_rx_s01;   /*  0xb000b0a0  DECT DMA access offset address for RX slots 0 and 1  */
   uint32 dect_shm_dma_offset_addr_rx_s23;   /*  0xb000b0a4  DECT DMA access offset address for RX slots 2 and 3  */
   uint32 dect_shm_dma_xfer_cntr_tx;         /*  0xb000b0a8  DECT DMA transfer count per slot in number of DMA transfer size */
   uint32 dect_shm_dma_xfer_cntr_rx;         /*  0xb000b0a8  DECT DMA transfer count per slot in number of DMA transfer size */   
} BCM63268DECTShimDmaControl;

#define BCM63268_DECT_DMA_CTRL ((volatile BCM63268DECTShimDmaControl * const) BCM63268_DECT_SHIM_DMA_CTRL_BASE)


typedef struct BCM63268ahbRegisters
{
   uint16 dsp_main_sync0;     /* 0xb0e57f80 DSP main counter outputs sel reg 0 */
   uint16 dsp_main_sync1;     /* 0xb0e57f82 DSP main counter outputs sel reg 1 */
   uint16 dsp_main_cnt;       /* 0xb0e57f84 DSP main counter reg */
   uint16 reserved1;          /* 0xb0e57f86 Reserved */
   uint16 reserved2;          /* 0xb0e57f88 Reserved */
   uint16 reserved3;          /* 0xb0e57f8a Reserved */
   uint16 reserved4;          /* 0xb0e57f8c Reserved */
   uint16 reserved5;          /* 0xb0e57f8e Reserved */
   uint16 reserved6;          /* 0xb0e57f90 Reserved */
   uint16 dsp_ram_out0;       /* 0xb0e57f92 DSP RAM output register 0 */
   uint16 dsp_ram_out1;       /* 0xb0e57f94 DSP RAM output register 1 */
   uint16 dsp_ram_out2;       /* 0xb0e57f96 DSP RAM output register 2 */
   uint16 dsp_ram_out3;       /* 0xb0e57f98 DSP RAM output register 3 */
   uint16 dsp_ram_in0;        /* 0xb0e57f9a DSP RAM input register 0 */
   uint16 dsp_ram_in1;        /* 0xb0e57f9c DSP RAM input register 1 */
   uint16 dsp_ram_in2;        /* 0xb0e57f9e DSP RAM input register 2 */
   uint16 dsp_ram_in3;        /* 0xb0e57fa0 DSP RAM input register 3 */
   uint16 dsp_zcross1_out;    /* 0xb0e57fa2 DSP RAM zero crossing 1 output reg */
   uint16 dsp_zcross2_out;    /* 0xb0e57fa4 DSP RAM zero crossing 2 output reg */
   uint16 reserved7;          /* 0xb0e57fa6 Reserved */
   uint16 reserved8;          /* 0xb0e57fa8 Reserved */
   uint16 reserved9;          /* 0xb0e57faa Reserved */
   uint16 reserved10;         /* 0xb0e57fac Reserved */
   uint16 reserved11;         /* 0xb0e57fae Reserved */
   uint16 reserved12;         /* 0xb0e57fb0 Reserved */
   uint16 reserved13;         /* 0xb0e57fb2 Reserved */
   uint16 reserved14;         /* 0xb0e57fb4 Reserved */
   uint16 reserved15;         /* 0xb0e57fb6 Reserved */
   uint16 reserved16;         /* 0xb0e57fb8 Reserved */
   uint16 reserved17;         /* 0xb0e57fba Reserved */
   uint16 dsp_main_ctrl;      /* 0xb0e57fbc DSP main counter control and preset reg */
   uint16 reserved18;         /* 0xb0e57fbe Reserved */
   uint16 reserved19;         /* 0xb0e57fc0 Reserved */
   uint16 reserved20;         /* 0xb0e57fc2 Reserved */
   uint16 reserved21;         /* 0xb0e57fc4 Reserved */
   uint16 reserved22;         /* 0xb0e57fc6 Reserved */
   uint16 reserved23;         /* 0xb0e57fc8 Reserved */
   uint16 reserved24;         /* 0xb0e57fca Reserved */
   uint16 reserved25;         /* 0xb0e57fce Reserved */
   uint16 dsp_ctrl;           /* 0xb0e57fd0 DSP control reg */
   uint16 dsp_pc;             /* 0xb0e57fd2 DSP program counter */
   uint16 dsp_pc_start;       /* 0xb0e57fd4 DSP program counter start */
   uint16 dsp_irq_start;      /* 0xb0e57fd6 DSP interrupt vector start */
   uint16 dsp_int;            /* 0xb0e57fd8 DSP to system bus interrupt vector */
   uint16 dsp_int_mask;       /* 0xb0e57fda DSP to system bus interrupt vector mask */
   uint16 dsp_int_prio1;      /* 0xb0e57fdc DSP interrupt mux 1 */
   uint16 dsp_int_prio2;      /* 0xb0e57fde DSP interrupt mux 2 */
   uint16 dsp_overflow;       /* 0xb0e57fe0 DSP to system bus interrupt overflow reg */
   uint16 dsp_jtbl_start;     /* 0xb0e57fe2 DSP jump table start address */
   uint16 reserved26;         /* 0xb0e57fe4 Reserved */
   uint16 reserved27;         /* 0xb0e57fe6 Reserved */
   uint16 reserved28;         /* 0xb0e57fe8 Reserved */
   uint16 reserved29;         /* 0xb0e57fea Reserved */
   uint16 reserved30;         /* 0xb0e57fec Reserved */
   uint16 reserved31;         /* 0xb0e57fee Reserved */
   uint16 dsp_debug_inst;     /* 0xb0e57ff0 DSP debug instruction register */
   uint16 reserved32;         /* 0xb0e57ff2 Reserved */
   uint16 dsp_debug_inout_l;  /* 0xb0e57ff4 DSP debug data (LSW) */
   uint16 dsp_debug_inout_h;  /* 0xb0e57ff6 DSP debug data (MSW) */
   uint16 reserved33;         /* 0xb0e57ff8 Reserved */
   uint16 reserved34;         /* 0xb0e57ffa Reserved */
   uint16 reserved35;         /* 0xb0e57ffc Reserved */
   uint16 reserved36;         /* 0xb0e57ffe Reserved */
} BCM63268ahbRegisters;

#define BCM63268_AHB_REGISTERS ((volatile BCM63268ahbRegisters * const) BCM63268_DECT_AHB_REG_BASE)


#ifdef __cplusplus
}
#endif

#endif /* __MAP_63268_H */

