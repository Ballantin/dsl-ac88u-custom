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
/*   MODULE:  map6328.h                                                */
/*   DATE:    01/05/10                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            6328 Chip for the DSP only.                              */
/*                                                                     */
/***********************************************************************/
#ifndef __MAP_6328_H
#define __MAP_6328_H

#include <bcmtypesCommon.h>
#include <common6328.h>

typedef struct BCM6328DDRPhyControl {
    uint32 REVISION;               /* 0x00 */
    uint32 CLK_PM_CTRL;            /* 0x04 */
    uint32 unused0[2];             /* 0x08-0x10 */
    uint32 PLL_STATUS;             /* 0x10 */
    uint32 PLL_CONFIG;             /* 0x14 */
    uint32 PLL_PRE_DIVIDER;        /* 0x18 */
    uint32 PLL_DIVIDER;            /* 0x1c */
    uint32 PLL_CONTROL1;           /* 0x20 */
    uint32 PLL_CONTROL2;           /* 0x24 */
    uint32 PLL_SS_EN;              /* 0x28 */
    uint32 PLL_SS_CFG;             /* 0x2c */
    uint32 STATIC_VDL_OVERRIDE;    /* 0x30 */
    uint32 DYNAMIC_VDL_OVERRIDE;   /* 0x34 */
    uint32 IDLE_PAD_CONTROL;       /* 0x38 */
    uint32 ZQ_PVT_COMP_CTL;        /* 0x3c */
    uint32 DRIVE_PAD_CTL;          /* 0x40 */
    uint32 CLOCK_REG_CONTROL;      /* 0x44 */
    uint32 unused1[46];
} BCM6328DDRPhyControl;

typedef struct BCM6328DDRPhyByteLaneControl {
    uint32 REVISION;                /* 0x00 */
    uint32 VDL_CALIBRATE;           /* 0x04 */
    uint32 VDL_STATUS;              /* 0x08 */
    uint32 unused;                  /* 0x0c */
    uint32 VDL_OVERRIDE_0;          /* 0x10 */
    uint32 VDL_OVERRIDE_1;          /* 0x14 */
    uint32 VDL_OVERRIDE_2;          /* 0x18 */
    uint32 VDL_OVERRIDE_3;          /* 0x1c */
    uint32 VDL_OVERRIDE_4;          /* 0x20 */
    uint32 VDL_OVERRIDE_5;          /* 0x24 */
    uint32 VDL_OVERRIDE_6;          /* 0x28 */
    uint32 VDL_OVERRIDE_7;          /* 0x2c */
    uint32 READ_CONTROL;            /* 0x30 */
    uint32 READ_FIFO_STATUS;        /* 0x34 */
    uint32 READ_FIFO_CLEAR;         /* 0x38 */
    uint32 IDLE_PAD_CONTROL;        /* 0x3c */
    uint32 DRIVE_PAD_CTL;           /* 0x40 */
    uint32 CLOCK_PAD_DISABLE;       /* 0x44 */
    uint32 WR_PREAMBLE_MODE;        /* 0x48 */
    uint32 CLOCK_REG_CONTROL;       /* 0x4C */
    uint32 unused0[44];
} BCM6328DDRPhyByteLaneControl;

typedef struct BCM6328DDRControl {
    uint32 CNFG;                            /* 0x000 */
    uint32 CSST;                            /* 0x004 */
    uint32 CSEND;                           /* 0x008 */
    uint32 unused;                          /* 0x00c */
    uint32 ROW00_0;                         /* 0x010 */
    uint32 ROW00_1;                         /* 0x014 */
    uint32 ROW01_0;                         /* 0x018 */
    uint32 ROW01_1;                         /* 0x01c */
    uint32 unused0[4];
    uint32 ROW20_0;                         /* 0x030 */
    uint32 ROW20_1;                         /* 0x034 */
    uint32 ROW21_0;                         /* 0x038 */
    uint32 ROW21_1;                         /* 0x03c */
    uint32 unused1[4];
    uint32 COL00_0;                         /* 0x050 */
    uint32 COL00_1;                         /* 0x054 */
    uint32 COL01_0;                         /* 0x058 */
    uint32 COL01_1;                         /* 0x05c */
    uint32 unused2[4];
    uint32 COL20_0;                         /* 0x070 */
    uint32 COL20_1;                         /* 0x074 */
    uint32 COL21_0;                         /* 0x078 */
    uint32 COL21_1;                         /* 0x07c */
    uint32 unused3[4];
    uint32 BNK10;                           /* 0x090 */
    uint32 BNK32;                           /* 0x094 */
    uint32 unused4[26];
    uint32 DCMD;                            /* 0x100 */
    uint32 DMODE_0;                         /* 0x104 */
    uint32 DMODE_1;                         /* 0x108 */
    uint32 CLKS;                            /* 0x10c */
    uint32 ODT;                             /* 0x110 */
    uint32 TIM1_0;                          /* 0x114 */
    uint32 TIM1_1;                          /* 0x118 */
    uint32 TIM2;                            /* 0x11c */
    uint32 CTL_CRC;                         /* 0x120 */
    uint32 DOUT_CRC;                        /* 0x124 */
    uint32 DIN_CRC;                         /* 0x128 */
    uint32 unused5[53];

    BCM6328DDRPhyControl           PhyControl;             /* 0x200 */
    BCM6328DDRPhyByteLaneControl   PhyByteLane0Control;    /* 0x300 */
    BCM6328DDRPhyByteLaneControl   PhyByteLane1Control;    /* 0x400 */
    BCM6328DDRPhyByteLaneControl   PhyByteLane2Control;    /* 0x500 */
    BCM6328DDRPhyByteLaneControl   PhyByteLane3Control;    /* 0x600 */
    uint32 unused6[64];

    uint32 GCFG;                            /* 0x800 */
    uint32 LBIST_CFG;                       /* 0x804 */
    uint32 LBIST_SEED;                      /* 0x808 */
    uint32 ARB;                             /* 0x80c */
    uint32 PI_GCF;                          /* 0x810 */
    uint32 PI_UBUS_CTL;                     /* 0x814 */
    uint32 PI_MIPS_CTL;                     /* 0x818 */
    uint32 PI_DSL_MIPS_CTL;                 /* 0x81c */
    uint32 PI_DSL_PHY_CTL;                  /* 0x820 */
    uint32 PI_UBUS_ST;                      /* 0x824 */
    uint32 PI_MIPS_ST;                      /* 0x828 */
    uint32 PI_DSL_MIPS_ST;                  /* 0x82c */
    uint32 PI_DSL_PHY_ST;                   /* 0x830 */
    uint32 PI_UBUS_SMPL;                    /* 0x834 */
    uint32 TESTMODE;                        /* 0x838 */
    uint32 TEST_CFG1;                       /* 0x83c */
    uint32 TEST_PAT;                        /* 0x840 */
    uint32 TEST_COUNT;                      /* 0x844 */
    uint32 TEST_CURR_COUNT;                 /* 0x848 */
    uint32 TEST_ADDR_UPDT;                  /* 0x84c */
    uint32 TEST_ADDR;                       /* 0x850 */
    uint32 TEST_DATA0;                      /* 0x854 */
    uint32 TEST_DATA1;                      /* 0x858 */
    uint32 TEST_DATA2;                      /* 0x85c */
    uint32 TEST_DATA3;                      /* 0x860 */
} BCM6328DDRControl;

#define BCM6328_DDR ((volatile BCM6328DDRControl * const) BCM6328_DDR_BASE)


/*
** Peripheral Controller
*/

typedef struct BCM6328PerfControl {
    uint32        RevID;             /* (00) word 0 */
    uint32        blkEnables;        /* (04) word 1 */
#define ROBOSW_CLK_EN    (1 << 11)
#define PCIE_CLK_EN      (1 << 10)
#define HS_SPI_CLK_EN    (1 << 9)
#define USBH_CLK_EN      (1 << 8)
#define USBD_CLK_EN      (1 << 7)
#define PCM_CLK_EN       (1 << 6)
#define SAR_CLK_EN       (1 << 5)
#define MIPS_CLK_EN      (1 << 4)
#define ADSL_CLK_EN      (1 << 3)
#define ADSL_AFE_EN      (1 << 2)
#define ADSL_QPROC_EN    (1 << 1)
#define PHYMIPS_CLK_EN   (1 << 0)

    uint32        unused0;           /* (08) word 2 */
    uint32        deviceTimeoutEn;   /* (0c) word 3 */
    uint32        softResetB;        /* (10) word 4 */
#define SOFT_RST_PCIE_HARD      (1 << 10)
#define SOFT_RST_PCIE_EXT       (1 << 9)
#define SOFT_RST_PCIE           (1 << 8)
#define SOFT_RST_PCIE_CORE      (1 << 7)
#define SOFT_RST_PCM            (1 << 6)
#define SOFT_RST_USBH           (1 << 5)
#define SOFT_RST_USBD           (1 << 4)
#define SOFT_RST_SWITCH         (1 << 3)
#define SOFT_RST_SAR            (1 << 2)
#define SOFT_RST_EPHY           (1 << 1)
#define SOFT_RST_SPI            (1 << 0)

    uint32        diagControl;        /* (14) word 5 */
    uint32        ExtIrqCfg;          /* (18) word 6*/
    uint32        unused1;            /* (1c) word 7 */
#define EI_SENSE_SHFT   0
#define EI_STATUS_SHFT  4
#define EI_CLEAR_SHFT   8
#define EI_MASK_SHFT    12
#define EI_INSENS_SHFT  16
#define EI_LEVEL_SHFT   20

    uint32        ExtraChipIrqMask;    /* (20) word 8 */
    uint32        ChipIrqMask;         /* (24) word 9 */
    uint32        ExtraChipIrqStatus;  /* (28) word 10 */
    uint32        ChipIrqStatus;       /* (2c) word 11 */

    uint32        ExtraChipIrqMask1;   /* (30) word 12 */
    uint32        ChipIrqMask1;        /* (34) word 13 */
    uint32        ExtraChipIrqStatus1; /* (38) word 14 */
    uint32        ChipIrqStatus1;      /* (3c) word 15 */

} BCM6328PerfControl;

#define BCM6328_PERF ((volatile BCM6328PerfControl * const) BCM6328_PERF_BASE)


/*
** UART
*/
typedef struct BCM6328UartChannel {
    byte          unused0;
    byte          control;
#define BCM6328_BRGEN           0x80    /* Control register bit defs */
#define BCM6328_TXEN            0x40
#define BCM6328_RXEN            0x20
#define BCM6328_LOOPBK          0x10
#define BCM6328_TXPARITYEN      0x08
#define BCM6328_TXPARITYEVEN    0x04
#define BCM6328_RXPARITYEN      0x02
#define BCM6328_RXPARITYEVEN    0x01

    byte          config;
#define BCM6328_XMITBREAK       0x40
#define BCM6328_BITS5SYM        0x00
#define BCM6328_BITS6SYM        0x10
#define BCM6328_BITS7SYM        0x20
#define BCM6328_BITS8SYM        0x30
#define BCM6328_ONESTOP         0x07
#define BCM6328_TWOSTOP         0x0f
    /* 4-LSBS represent STOP bits/char
     * in 1/8 bit-time intervals.  Zero
     * represents 1/8 stop bit interval.
     * Fifteen represents 2 stop bits.
     */
    byte          fifoctl;
#define BCM6328_RSTTXFIFOS      0x80
#define BCM6328_RSTRXFIFOS      0x40
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
#define BCM6328_DTREN   0x01
#define BCM6328_RTSEN   0x02

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
#define BCM6328_DELTAIP         0x0001
#define BCM6328_TXUNDERR        0x0002
#define BCM6328_TXOVFERR        0x0004
#define BCM6328_TXFIFOTHOLD     0x0008
#define BCM6328_TXREADLATCH     0x0010
#define BCM6328_TXFIFOEMT       0x0020
#define BCM6328_RXUNDERR        0x0040
#define BCM6328_RXOVFERR        0x0080
#define BCM6328_RXTIMEOUT       0x0100
#define BCM6328_RXFIFOFULL      0x0200
#define BCM6328_RXFIFOTHOLD     0x0400
#define BCM6328_RXFIFONE        0x0800
#define BCM6328_RXFRAMERR       0x1000
#define BCM6328_RXPARERR        0x2000
#define BCM6328_RXBRK           0x4000

    uint16        unused2;
    uint16        Data;                   /* Write to TX, Read from RX */
                                          /* bits 11:8 are BRK,PAR,FRM errors */

    uint32        unused3;
    uint32        unused4;
} BCM6328UartChannel;

#define BCM6328_UART ((volatile BCM6328UartChannel * const) BCM6328_UART_BASE)

/*
** Gpio Controller
*/

typedef struct BCM6328GpioControl {
    uint32      GPIODir;                   /* 0 */
    uint32      GPIODirHi;                 /* 4 */
    uint32      GPIOio;                    /* 8 */
    uint32      GPIOioHi;                  /* C */
    uint32      unused3;                    /* 10 */
    uint32      SpiSlaveCfg;                /* 14 */
    uint32      unused4;                    /* 18 */
    uint32      PinMuxSel[3];               /* 1C */
#define PIN_MUX_PCIE_CLKREQ(x)   ((x&0x3) << 22)
#define PIN_MUX_SEL_PCIE_CLKREQ 0x00c00000
#define PIN_MUX_SEL_USB_MASK    0x0f000000
#define PIN_MUX_SEL_USB_HOST    0x05000000
#define PIN_MUX_SEL_USB_DEV     0x06000000
    uint32      TestControl;                /* 28 */
    uint32      unused5;                    /* 2C */
    uint32      RoboSWLEDControl;           /* 30 */
    uint32      RoboSWLEDLSR;               /* 34 */
    uint32      unused6;                    /* 38 */
    uint32      RoboswEphyCtrl;             /* 3C */
#define EPHY_AUTO_PWR_DOWN_EN   (1<<29)
#define EPHY_IDDQ_FROM_PHY      (1<<28)
#define EPHY_PLL_LOCK           (1<<27)
#define EPHY_ATEST_25MHZ_EN     (1<<26)
#define EPHY_PWR_DOWN_DLL       (1<<25)
#define EPHY_PWR_DOWN_BIAS      (1<<24)
#define EPHY_PWR_DOWN_TX_4      (1<<23)
#define EPHY_PWR_DOWN_TX_3      (1<<22)
#define EPHY_PWR_DOWN_TX_2      (1<<21)
#define EPHY_PWR_DOWN_TX_1      (1<<20)
#define EPHY_PWR_DOWN_RX_4      (1<<19)
#define EPHY_PWR_DOWN_RX_3      (1<<18)
#define EPHY_PWR_DOWN_RX_2      (1<<17)
#define EPHY_PWR_DOWN_RX_1      (1<<16)
#define EPHY_PWR_DOWN_SD_4      (1<<15)
#define EPHY_PWR_DOWN_SD_3      (1<<14)
#define EPHY_PWR_DOWN_SD_2      (1<<13)
#define EPHY_PWR_DOWN_SD_1      (1<<12)
#define EPHY_PWR_DOWN_RD_4      (1<<11)
#define EPHY_PWR_DOWN_RD_3      (1<<10)
#define EPHY_PWR_DOWN_RD_2      (1<<9)
#define EPHY_PWR_DOWN_RD_1      (1<<8)
#define EPHY_PWR_DOWN_4         (1<<7)
#define EPHY_PWR_DOWN_3         (1<<6)
#define EPHY_PWR_DOWN_2         (1<<5)
#define EPHY_PWR_DOWN_1         (1<<4)
#define EPHY_RST_4              (1<<3)
#define EPHY_RST_3              (1<<2)
#define EPHY_RST_2              (1<<1)
#define EPHY_RST_1              (1<<0)
    uint32      RoboswSwitchCtrl;           /* 40 */
#define RSW_SPI_MODE            (1<<11)
#define RSW_BC_SUPP_EN          (1<<10)
#define RSW_CLK_FREQ_MASK       (3<<8)
#define RSW_ENF_DFX_FLOW        (1<<7)
#define RSW_ENH_DFX_FLOW        (1<<6)
#define RSW_GRX_0_SETUP         (1<<5)
#define RSW_GTX_0_SETUP         (1<<4)
#define RSW_HW_FWDG_EN          (1<<3)
#define RSW_QOS_EN              (1<<2)
#define RSW_WD_CLR_EN           (1<<1)
#define RSW_MII_DUMB_FWDG_EN    (1<<0)

    uint32      RegFileTmCtl;               /* 44 */
    uint32      unused7[8];                 /* 48 */
    uint32      DieRevID;                   /* 68 */
    uint32      unused8;                    /* 6c */
    uint32      DiagSelControl;             /* 70 */
    uint32      DiagReadBack;               /* 74 */
    uint32      DiagReadBackHi;             /* 78 */
    uint32      DiagMiscControl;            /* 7c */
} BCM6328GpioControl;

#define BCM6328_GPIO ((volatile BCM6328GpioControl * const) BCM6328_GPIO_BASE)

/* Number to mask conversion macro used for GPIODir and GPIOio */
#define BCM6328_GPIO_NUM_TOTAL_BITS_MASK        0x3f
#define BCM6328_GPIO_NUM_MAX_BITS_MASK          0x1f
#define BCM6328_GPIO_NUM_TO_MASK(X)             ( (((X) & BCM6328_GPIO_NUM_TOTAL_BITS_MASK) < 32) ? (1 << ((X) & BCM6328_GPIO_NUM_MAX_BITS_MASK)) : (0) )

/* Number to mask conversion macro used for GPIODir_high and GPIOio_high */
#define BCM6328_GPIO_NUM_MAX_BITS_MASK_HIGH     0x07
#define BCM6328_GPIO_NUM_TO_MASK_HIGH(X)        ( (((X) & BCM6328_GPIO_NUM_TOTAL_BITS_MASK) >= 32) ? (1 << ((X-32) & BCM6328_GPIO_NUM_MAX_BITS_MASK_HIGH)) : (0) )


/*
** Misc Register Set Definitions.
*/

typedef struct BCM6328Misc {
  uint32        miscSerdesCtrl;                 /* 0x0000 */
#define BCM6328_SERDES_PCIE_ENABLE                      0x00000001
#define BCM6328_SERDES_PCIE_EXD_ENABLE                  (1<<15)
    
  uint32        miscSerdesSts;                  /* 0x0004 */
  uint32        miscIrqOutMask;                 /* 0x0008 */
#define BCM6328_MISC_PCIE_EP_IRQ_MASK0                  (1<<0)
#define BCM6328_MISC_PCIE_EP_IRQ_MASK1                  (1<<1)

  uint32        miscMemcControl;                /* 0x000c */
#define BCM6328_MISC_MEMC_CONTROL_MC_UBUS_ASYNC_MODE    (1<<3)
#define BCM6328_MISC_MEMC_CONTROL_MC_LMB_ASYNC_MODE     (1<<2)
#define BCM6328_MISC_MEMC_CONTROL_DDR_TEST_DONE         (1<<1)
#define BCM6328_MISC_MEMC_CONTROL_DDR_TEST_DISABLE      (1<<0)

  uint32        unused0[2];                     /* 0x0010 */

  uint32        miscVregCtrl0;                  /* 0x0018 */
#define BCM6328_MISC_VREG_1_200_MASK      (15)
#define BCM6328_MISC_VREG_1_225_V          (0)   /* default */
#define BCM6328_MISC_VREG_1_000_V          (1)  
#define BCM6328_MISC_VREG_1_025_V          (2)  
#define BCM6328_MISC_VREG_1_050_V          (3)  
#define BCM6328_MISC_VREG_1_075_V          (4)  
#define BCM6328_MISC_VREG_1_100_V          (5)  
#define BCM6328_MISC_VREG_1_125_V          (6)  
#define BCM6328_MISC_VREG_1_150_V          (7)  
#define BCM6328_MISC_VREG_1_175_V          (8)  
#define BCM6328_MISC_VREG_1_200_V          (9)  
#define BCM6328_MISC_VREG_1_250_V         (10)
#define BCM6328_MISC_VREG_1_275_V         (11)
#define BCM6328_MISC_VREG_1_300_V         (12)
#define BCM6328_MISC_VREG_1_325_V         (13)
#define BCM6328_MISC_VREG_1_350_V         (14)
#define BCM6328_MISC_VREG_1_375_V         (15)

  uint32        miscVregCtrl1;                  /* 0x001c */
  uint32        miscVregCtrl2;                  /* 0x0020 */
  uint32        miscLedXorReg;                  /* 0x0024 */
  uint32        unused1[2];                     /* 0x0028 */
  uint32        miscDdrPllTestCtrl;             /* 0x0030 */
  uint32        miscPadCtrlLow;                 /* 0x0034 */
  uint32        miscPadCtrlHigh;                /* 0x0038 */
  uint32        miscPeriphEcoReg;               /* 0x003c */
  uint32        unused2[2];                     /* 0x0040 */
  uint32        miscIddqCtrl;                   /* 0x0048 */
  uint32        miscAdslClkSample;              /* 0x004c */
  uint32        unused3[(0x0100 - 0x0050) / 4]; /* 0x0050 */
  uint32        miscAdslCtrl;                   /* 0x0100 */
  uint32        unused4[(0x0180 - 0x0104) / 4]; /* 0x0104 */
  uint32        miscMipsTestCtrl;               /* 0x0180 */
  uint32        unused5[(0x0200 - 0x0184) / 4]; /* 0x0184 */
  uint32        miscPllCtrlSysPll0;             /* 0x0200 */
  uint32        miscPllCtrlSysPll1;             /* 0x0204 */
  uint32        miscPllCtrlSysPll2;             /* 0x0208 */
  uint32        miscPllCtrlSysPll3;             /* 0x020c */
  uint32        miscPllCtrlDdrPll;              /* 0x0210 */
  uint32        miscPllCtrlXtalEcoReg;          /* 0x0214 */
  uint32        unused6[(0x0240 - 0x0218) / 4]; /* 0x0218 */
  uint32        miscStrapBus;                   /* 0x0240 */
#define BCM6328_MISC_STRAP_BUS_BOOT_SEL_SHIFT           18
#define BCM6328_MISC_STRAP_BUS_BOOT_SEL_MASK            (0x1<<MISC_STRAP_BUS_BOOT_SEL_SHIFT)
#define BCM6328_MISC_STRAP_BUS_BOOT_SERIAL              0x01
#define BCM6328_MISC_STRAP_BUS_BOOT_NAND                0x00
#define BCM6328_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT      7
#define BCM6328_MISC_STRAP_BUS_MIPS_PLL_FVCO_MASK       (0x1F<<BCM6328_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT)

  uint32        miscStrapOverride;              /* 0x0244 */
  uint32        unused7[(0x0280 - 0x0248) / 4]; /* 0x0248 */
  uint32        miscRtcSleepModeEn;             /* 0x0280 */
  uint32        miscRtcSleepRtcEn;              /* 0x0284 */
  uint32        miscRtcSleepRtcCountLow;        /* 0x0288 */
  uint32        miscRtcSleepRtcCountHigh;       /* 0x028c */
  uint32        miscRtcSleepRtcEvent;           /* 0x0290 */
  uint32        miscRtcSleepWakeupMask;         /* 0x0294 */
  uint32        miscRtcSleepWakeupStatus;       /* 0x0298 */
  uint32        miscRtcSleepDebounceCtrl;       /* 0x029c */
  uint32        miscRtcSleepCpuScratchPad;      /* 0x02a0 */
} BCM6328Misc;

#define BCM6328_MISC ((volatile BCM6328Misc * const) BCM6328_MISC_BASE)

/*
** Pcm Controller
*/

typedef struct BCM6328PcmControlRegisters
{
    uint32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define  BCM6328_PCM_ENABLE              0x80000000     // PCM block master enable
#define  BCM6328_PCM_ENABLE_SHIFT        31
#define  BCM6328_PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  BCM6328_PCM_SLAVE_SEL_SHIFT     30
#define  BCM6328_PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  BCM6328_PCM_CLOCK_INV_SHIFT     29
#define  BCM6328_PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  BCM6328_PCM_FS_INVERT_SHIFT     28
#define  BCM6328_PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  BCM6328_PCM_FS_FREQ_16_8_SHIFT  27
#define  BCM6328_PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  BCM6328_PCM_FS_LONG_SHIFT       26
#define  BCM6328_PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  BCM6328_PCM_FS_TRIG_SHIFT       25
#define  BCM6328_PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  BCM6328_PCM_DATA_OFF_SHIFT      24
#define  BCM6328_PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  BCM6328_PCM_DATA_16_8_SHIFT     23
#define  BCM6328_PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  BCM6328_PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define  BCM6328_PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  BCM6328_PCM_LSB_FIRST_SHIFT     18
#define  BCM6328_PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  BCM6328_PCM_LOOPBACK_SHIFT      17
#define  BCM6328_PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in 6328
#define  BCM6328_PCM_EXTCLK_SEL_SHIFT    16
#define  BCM6328_PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 6328
#define  BCM6328_PCM_NTR_ENABLE_SHIFT    15
#define  BCM6328_PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  BCM6328_PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  BCM6328_PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min

    uint32 pcm_chan_ctrl;                       // 04
#define  BCM6328_PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  BCM6328_PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  BCM6328_PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  BCM6328_PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  BCM6328_PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  BCM6328_PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  BCM6328_PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  BCM6328_PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  BCM6328_PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  BCM6328_PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  BCM6328_PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  BCM6328_PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  BCM6328_PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  BCM6328_PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  BCM6328_PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  BCM6328_PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  BCM6328_PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  BCM6328_PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define  BCM6328_PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  BCM6328_PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  BCM6328_PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  BCM6328_PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  BCM6328_PCM_TX_IRQ              0x00000010

    uint32 pcm_pll_ctrl1;                       // 10
#define  BCM6328_PCM_PLL_PWRDN           0x80000000     // PLL PWRDN
#define  BCM6328_PCM_PLL_PWRDN_CH1       0x40000000     // PLL CH PWRDN
#define  BCM6328_PCM_PLL_REFCMP_PWRDN    0x20000000     // PLL REFCMP PWRDN
#define  BCM6328_PCM_CLK16_RESET         0x10000000     // 16.382 MHz PCM interface circuitry reset. 
#define  BCM6328_PCM_PLL_ARESET          0x08000000     // PLL Analog Reset
#define  BCM6328_PCM_PLL_DRESET          0x04000000     // PLL Digital Reset

    uint32 pcm_pll_ctrl2;                       // 14
    uint32 pcm_pll_ctrl3;                       // 18
    uint32 pcm_pll_ctrl4;                       // 1c

    uint32 pcm_pll_stat;                        // 20
#define  BCM6328_PCM_PLL_LOCK            0x00000001     // Asserted when PLL is locked to programmed frequency

    uint32 pcm_ntr_counter;                     // 24

    uint32 unused[6];
#define  BCM6328_PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[BCM6328_PCM_MAX_TIMESLOT_REGS];
#define  BCM6328_PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} BCM6328PcmControlRegisters;

#define BCM6328_PCM ((volatile BCM6328PcmControlRegisters * const) BCM6328_PCM_BASE)

typedef struct BCM6328PcmIudmaRegisters
{
    uint16 reserved0;
    uint16 ctrlConfig;
#define BCM6328_IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define BCM6328_IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define BCM6328_IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define BCM6328_IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define BCM6328_IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

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
} BCM6328PcmIudmaRegisters;

typedef struct BCM6328PcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM6328_IUDMA_CONFIG_ENDMA       0x0001
#define BCM6328_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM6328_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM6328_IUDMA_INTSTAT_BDONE   0x0001
#define BCM6328_IUDMA_INTSTAT_PDONE   0x0002
#define BCM6328_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM6328_IUDMA_INTSTAT_MASK    0x0007
#define BCM6328_IUDMA_INTSTAT_ALL     BCM6328_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM6328_IUDMA_INTMASK_BDONE   0x0001
#define BCM6328_IUDMA_INTMASK_PDONE   0x0002
#define BCM6328_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM6328_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} BCM6328PcmIudmaChannelCtrl;


typedef struct BCM6328PcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM6328_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} BCM6328PcmIudmaStateRam;

#define BCM6328_MAX_PCM_DMA_CHANNELS 2

typedef struct BCM6328PcmIudma
{
   BCM6328PcmIudmaRegisters regs;                                        // 
   uint32 reserved1[110];                                         //            
   BCM6328PcmIudmaChannelCtrl ctrl[BCM6328_MAX_PCM_DMA_CHANNELS];        //
   uint32 reserved2[120];                                         //
   BCM6328PcmIudmaStateRam stram[BCM6328_MAX_PCM_DMA_CHANNELS];          //

} BCM6328PcmIudma;

#define BCM6328_PCM_IUDMA ((volatile BCM6328PcmIudma * const) BCM6328_PCM_DMA_BASE)


#define BCM6328_IUDMA_MAX_CHANNELS          32

/*
** DMA Channel Configuration (1 .. 32)
*/
typedef struct BCM6328DmaChannelCfg {
  uint32        cfg;                    /* (00) assorted configuration */
#define         DMA_ENABLE      0x00000001  /* set to enable channel */
#define         DMA_PKT_HALT    0x00000002  /* idle after an EOP flag is detected */
#define         DMA_BURST_HALT  0x00000004  /* idle after finish current memory burst */
  uint32        intStat;                /* (04) interrupts control and status */
  uint32        intMask;                /* (08) interrupts mask */
#define         DMA_BUFF_DONE   0x00000001  /* buffer done */
#define         DMA_DONE        0x00000002  /* packet xfer complete */
#define         DMA_NO_DESC     0x00000004  /* no valid descriptors */
  uint32        maxBurst;               /* (0C) max burst length permitted */
#define         DMA_DESCSIZE_SEL 0x00040000  /* DMA Descriptor Size Selection */
} BCM6328DmaChannelCfg;

/*
** DMA State RAM (1 .. 16)
*/
typedef struct BCM6328DmaStateRam {
  uint32        baseDescPtr;            /* (00) descriptor ring start address */
  uint32        state_data;             /* (04) state/bytes done/ring offset */
  uint32        desc_len_status;        /* (08) buffer descriptor status and len */
  uint32        desc_base_bufptr;       /* (0C) buffer descrpitor current processing */
} BCM6328DmaStateRam;

/*
** DMA Registers
*/
typedef struct BCM6328DmaRegs {
    uint32 controller_cfg;              /* (00) controller configuration */
#define DMA_MASTER_EN           0x00000001
#define DMA_FLOWC_CH1_EN        0x00000002
#define DMA_FLOWC_CH3_EN        0x00000004

    // Flow control Ch1
    uint32 flowctl_ch1_thresh_lo;           /* 004 */
    uint32 flowctl_ch1_thresh_hi;           /* 008 */
    uint32 flowctl_ch1_alloc;               /* 00c */
#define DMA_BUF_ALLOC_FORCE     0x80000000

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
    BCM6328DmaChannelCfg chcfg[BCM6328_IUDMA_MAX_CHANNELS];/* (200-3FF) Channel configuration */
    union {
        BCM6328DmaStateRam     s[BCM6328_IUDMA_MAX_CHANNELS];
        uint32          u32[4 * BCM6328_IUDMA_MAX_CHANNELS];
    } stram;                                /* (400-5FF) state ram */
} BCM6328DmaRegs;

#define BCM6328SW_DMA ((volatile BCM6328DmaRegs * const) BCM6328SWITCH_DMA_BASE)
/*
** DMA Buffer 
*/
typedef struct BCM6328DmaDesc {
  union {
    struct {
  uint16        length;                 /* in bytes of data in buffer */
#define          DMA_DESC_USEFPM    0x8000
#define          DMA_DESC_MULTICAST 0x4000
#define          DMA_DESC_BUFLENGTH 0x0fff
  uint16        status;                 /* buffer status */
#define          DMA_OWN                0x8000  /* cleared by DMA, set by SW */
#define          DMA_EOP                0x4000  /* last buffer in packet */
#define          DMA_SOP                0x2000  /* first buffer in packet */
#define          DMA_WRAP               0x1000  /* */
#define          DMA_PRIO               0x0C00  /* Prio for Tx */
#define          DMA_APPEND_BRCM_TAG    0x0200
#define          DMA_APPEND_CRC         0x0100
#define          USB_ZERO_PKT           (1<< 0) // Set to send zero length packet
    }           word1;
    uint32      word0;
  } word2;

  uint32        address;                /* address of data */
} BCM6328DmaDesc;

#endif /* __MAP_6328_H */

