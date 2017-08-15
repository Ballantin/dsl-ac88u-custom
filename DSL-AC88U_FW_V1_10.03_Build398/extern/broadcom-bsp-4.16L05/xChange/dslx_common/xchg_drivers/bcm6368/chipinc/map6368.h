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
/*   MODULE:  map6368.h                                                */
/*   DATE:    01/05/10                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            6368 Chip for the DSP only.                              */
/*                                                                     */
/***********************************************************************/
#ifndef __MAP_6368_H
#define __MAP_6368_H

#include <bcmtypesCommon.h>
#include <common6368.h>

typedef struct BCM6368DDRControl {
    uint32    RevID;            /* 00 */
    uint32    PadSSTLMode;      /* 04 */
    uint32    CmdPadCntl;       /* 08 */
    uint32    DQPadCntl;        /* 0c */
    uint32    DQSPadCntl;       /* 10 */
    uint32    ClkPadCntl0;      /* 14 */
    uint32    MIPSDDRPLLCntl0;  /* 18 */
    uint32    MIPSDDRPLLCntl1;  /* 1c */
    uint32    MIPSDDRPLLConfig; /* 20 */
#define BCM6368_MIPSDDR_NDIV_SHFT       16
#define BCM6368_MIPSDDR_NDIV_MASK       (0x1ff<<BCM6368_MIPSDDR_NDIV_SHFT)
#define BCM6368_REF_MDIV_SHFT           8
#define BCM6368_REF_MDIV_MASK           (0xff<<BCM6368_REF_MDIV_SHFT)
#define BCM6368_MIPSDDR_P2_SHFT         4
#define BCM6368_MIPSDDR_P2_MASK         (0xf<<BCM6368_MIPSDDR_P2_SHFT)
#define BCM6368_MIPSDDR_P1_SHFT         0
#define BCM6368_MIPSDDR_P1_MASK         (0xf<<BCM6368_MIPSDDR_P1_SHFT)
    uint32    MIPSDDRPLLMDiv;   /* 24 */
#define BCM6368_DDR_MDIV_SHFT           8
#define BCM6368_DDR_MDIV_MASK           (0xff<<BCM6368_DDR_MDIV_SHFT)
#define BCM6368_MIPS_MDIV_SHFT          0
#define BCM6368_MIPS_MDIV_MASK          (0xff<<BCM6368_MIPS_MDIV_SHFT)
    uint32    DSLCorePhaseCntl; /* 28 */
    uint32    DSLCpuPhaseCntr;  /* 2c */
#define BCM6368_DSL_PHY_AFE_PI_CNTR_CYCLES_SHIFT    28
#define BCM6368_DSL_PHY_AFE_PI_CNTR_CYCLES_MASK     (0xF << BCM6368_DSL_PHY_AFE_PI_CNTR_CYCLES_SHIFT)
#define BCM6368_DSL_PHY_PI_CNTR_CYCLES_SHIFT        24
#define    BCM6368_DSL_PHY_PI_CNTR_CYCLES_MASK      (0xF << BCM6368_DSL_PHY_PI_CNTR_CYCLES_SHIFT)
#define BCM6368_DSL_PHY_AFE_PI_CNTR_EN              (1 << 22)
#define BCM6368_DSL_PHY_PI_CNTR_EN                  (1 << 21)
#define    BCM6368_DSL_CPU_PI_CNTR_EN               (1 << 20)
#define    BCM6368_DSL_CPU_PI_CNTR_CYCLES_SHIFT     16
#define    BCM6368_DSL_CPU_PI_CNTR_CYCLES_MASK      (0xF << BCM6368_DSL_CPU_PI_CNTR_CYCLES_SHIFT)
    uint32    MIPSPhaseCntl;    /* 30 */
#define    BCM6368_PH_CNTR_EN        (1 << 20)
    uint32    DDR1_2PhaseCntl0;  /* 34 */
    uint32    DDR3_4PhaseCntl0; /* 38 */
    uint32    VCDLPhaseCntl0;   /* 3c */
    uint32    VCDLPhaseCntl1;   /* 40 */
    uint32    WSliceCntl;       /* 44 */
    uint32    DeskewDLLCntl;    /* 48 */
    uint32    DeskewDLLReset;   /* 4c */
    uint32    DeskewDLLPhase;   /* 50 */
    uint32    AnalogTestCntl;   /* 54 */
    uint32    RdDQSGateCntl;    /* 58 */
    uint32    PLLTestReg;       /* 5c */
    uint32    Spare0;           /* 60 */
    uint32    Spare1;           /* 64 */
    uint32    Spare2;           /* 68 */
    uint32    CLBist;           /* 6c */
    uint32    LBistCRC;         /* 70 */
    uint32    UBUSPhaseCntl;    /* 74 */
    uint32    UBUSPIDeskewLLMB0; /* 78 */
    uint32    UBUSPIDeskewLLMB1; /* 7C */

} BCM6368DDRControl;

#define BCM6368_DDR ((volatile BCM6368DDRControl * const) BCM6368_DDR_BASE)

/*
** Peripheral Controller
*/
typedef struct BCM6368PerfControl {
     uint32        RevID;             /* (00) word 0 */
     uint32        blkEnables;        /* (04) word 1 */
#define BCM6368_IPSEC_CLK_EN     (1 << 18)
#define BCM6368_NAND_CLK_EN      (1 << 17)
#define BCM6368_DISABLE_GLESS    (1 << 16)
#define BCM6368_USBH_CLK_EN      (1 << 15)
#define BCM6368_PCM_CLK_EN       (1 << 14)
#define BCM6368_UTOPIA_CLK_EN    (1 << 13)
#define BCM6368_ROBOSW_CLK_EN    (1 << 12)
#define BCM6368_SAR_CLK_EN       (1 << 11)
#define BCM6368_USBD_CLK_EN      (1 << 10)
#define BCM6368_SPI_CLK_EN       (1 << 9)
#define BCM6368_SWPKT_SAR_CLK_EN (1 << 8)
#define BCM6368_SWPKT_USB_CLK_EN (1 << 7)
#define BCM6368_PHYMIPS_CLK_EN   (1 << 6)
#define BCM6368_VDSL_CLK_EN      (1 << 5)
#define BCM6368_VDSL_BONDING_EN  (1 << 4)
#define BCM6368_VDSL_AFE_EN      (1 << 3)
#define BCM6368_VDSL_QPROC_EN    (1 << 2)
   
     uint32        pll_control;       /* (08) word 2 */
#define BCM6368_SOFT_RESET              0x00000001      // 0

     uint32        deviceTimeoutEn;   /* (0c) word 3 */
     uint32        softResetB;        /* (10) word 4 */
#define BCM6368_SOFT_RST_PCM     (1 << 13)
#define BCM6368_SOFT_RST_USBH    (1 << 12)
#define BCM6368_SOFT_RST_USBD    (1 << 11)
#define BCM6368_SOFT_RST_SWITCH  (1 << 10)
#define BCM6368_SOFT_RST_SAR     (1 << 7)
#define BCM6368_SOFT_RST_EPHY    (1 << 6)
#define BCM6368_SOFT_RST_IPSEC   (1 << 4)
#define BCM6368_SOFT_RST_MPI     (1 << 3)
#define BCM6368_SOFT_RST_SPI     (1 << 0)

    uint32        unused1;            /* (14) word 5 */
    uint32        ExtIrqCfg;          /* (18) word 6*/
    uint32        ExtIrqCfg1;         /* (1c) word 7 */
#define BCM6368_EI_SENSE_SHFT   0
#define BCM6368_EI_STATUS_SHFT  4
#define BCM6368_EI_CLEAR_SHFT   8
#define BCM6368_EI_MASK_SHFT    12
#define BCM6368_EI_INSENS_SHFT  16
#define BCM6368_EI_LEVEL_SHFT   20

     uint32        IrqMask_high;      /* (20) word 8 */
     uint32        IrqMask;           /* (24) word 9 */
     uint32        IrqStatus_high;    /* (28) word 10 */
     uint32        IrqStatus;         /* (2c) word 11 */

     uint32        IrqMask1_high;     /* (30) word 13 */
     uint32        IrqMask1;          /* (34) word 14*/   
     uint32        IrqStatus1_high;   /* (38) word 15 */
     uint32        IrqStatus1;        /* (3c) word 16 */
} BCM6368PerfControl;

#define BCM6368_PERF ((volatile BCM6368PerfControl * const) BCM6368_PERF_BASE)

/*
** UART
*/
typedef struct BCM6368UartChannel {
    byte          unused0;
    byte          control;
#define BCM6368_BRGEN           0x80    /* Control register bit defs */
#define BCM6368_TXEN            0x40
#define BCM6368_RXEN            0x20
#define BCM6368_LOOPBK          0x10
#define BCM6368_TXPARITYEN      0x08
#define BCM6368_TXPARITYEVEN    0x04
#define BCM6368_RXPARITYEN      0x02
#define BCM6368_RXPARITYEVEN    0x01

    byte          config;
#define BCM6368_XMITBREAK       0x40
#define BCM6368_BITS5SYM        0x00
#define BCM6368_BITS6SYM        0x10
#define BCM6368_BITS7SYM        0x20
#define BCM6368_BITS8SYM        0x30
#define BCM6368_ONESTOP         0x07
#define BCM6368_TWOSTOP         0x0f
    /* 4-LSBS represent STOP bits/char
     * in 1/8 bit-time intervals.  Zero
     * represents 1/8 stop bit interval.
     * Fifteen represents 2 stop bits.
     */
    byte          fifoctl;
#define BCM6368_RSTTXFIFOS      0x80
#define BCM6368_RSTRXFIFOS      0x40
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
#define BCM6368_DTREN   0x01
#define BCM6368_RTSEN   0x02

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
#define BCM6368_DELTAIP         0x0001
#define BCM6368_TXUNDERR        0x0002
#define BCM6368_TXOVFERR        0x0004
#define BCM6368_TXFIFOTHOLD     0x0008
#define BCM6368_TXREADLATCH     0x0010
#define BCM6368_TXFIFOEMT       0x0020
#define BCM6368_RXUNDERR        0x0040
#define BCM6368_RXOVFERR        0x0080
#define BCM6368_RXTIMEOUT       0x0100
#define BCM6368_RXFIFOFULL      0x0200
#define BCM6368_RXFIFOTHOLD     0x0400
#define BCM6368_RXFIFONE        0x0800
#define BCM6368_RXFRAMERR       0x1000
#define BCM6368_RXPARERR        0x2000
#define BCM6368_RXBRK           0x4000

    uint16        unused2;
    uint16        Data;                   /* Write to TX, Read from RX */
                                          /* bits 11:8 are BRK,PAR,FRM errors */

    uint32        unused3;
    uint32        unused4;
} BCM6368Uart;

#define BCM6368_UART ((volatile BCM6368Uart * const) BCM6368_UART_BASE)

/*
** Gpio Controller
*/

typedef struct BCM6368GpioControl {
    uint32      GPIODir_high;               /* bits 36:32 */
    uint32      GPIODir;                    /* bits 31:00 */
    uint32      GPIOio_high;                /* bits 36:32 */
    uint32      GPIOio;                     /* bits 31:00 */
    uint32      LEDCtrl;
#define BCM6368_LED_ALL_STROBE          0x0f000000
#define BCM6368_LED3_STROBE             0x08000000
#define BCM6368_LED2_STROBE             0x04000000
#define BCM6368_LED1_STROBE             0x02000000
#define BCM6368_LED0_STROBE             0x01000000
#define BCM6368_LED_TEST                0x00010000
#define BCM6368_DISABLE_LINK_ACT_ALL    0x0000f000
#define BCM6368_DISABLE_LINK_ACT_3      0x00008000
#define BCM6368_DISABLE_LINK_ACT_2      0x00004000
#define BCM6368_DISABLE_LINK_ACT_1      0x00002000
#define BCM6368_DISABLE_LINK_ACT_0      0x00001000
#define BCM6368_LED_INTERVAL_SET_MASK   0x00000f00
#define BCM6368_LED_INTERVAL_SET_1280MS 0x00000700
#define BCM6368_LED_INTERVAL_SET_640MS  0x00000600
#define BCM6368_LED_INTERVAL_SET_320MS  0x00000500
#define BCM6368_LED_INTERVAL_SET_160MS  0x00000400
#define BCM6368_LED_INTERVAL_SET_80MS   0x00000300
#define BCM6368_LED_INTERVAL_SET_40MS   0x00000200
#define BCM6368_LED_INTERVAL_SET_20MS   0x00000100
#define BCM6368_LED_ON_ALL              0x000000f0
#define BCM6368_LED_ON_3                0x00000080
#define BCM6368_LED_ON_2                0x00000040
#define BCM6368_LED_ON_1                0x00000020
#define BCM6368_LED_ON_0                0x00000010
#define BCM6368_LED_ENABLE_ALL          0x0000000f
#define BCM6368_LED_ENABLE_3            0x00000008
#define BCM6368_LED_ENABLE_2            0x00000004
#define BCM6368_LED_ENABLE_1            0x00000002
#define BCM6368_LED_ENABLE_0            0x00000001
    uint32      SpiSlaveCfg;                /* 14 */
    uint32      GPIOMode;                   /* 18 */
#define BCM6368_GPIO_MODE_SPI_SSN5          (1<<31)
#define BCM6368_GPIO_MODE_SPI_SSN4          (1<<30)
#define BCM6368_GPIO_MODE_SPI_SSN3          (1<<29)
#define BCM6368_GPIO_MODE_SPI_SSN2          (1<<28)
#define BCM6368_GPIO_MODE_EBI_CS3           (1<<27)
#define BCM6368_GPIO_MODE_EBI_CS2           (1<<26)
#define BCM6368_GPIO_MODE_PCMCIA_VS2        (1<<25)
#define BCM6368_GPIO_MODE_PCMCIA_VS1        (1<<24)
#define BCM6368_GPIO_MODE_PCMCIA_CD2        (1<<23)
#define BCM6368_GPIO_MODE_PCMCIA_CD1        (1<<22)
#define BCM6368_GPIO_MODE_PCI_GNT0          (1<<20)
#define BCM6368_GPIO_MODE_PCI_REQ0          (1<<19)
#define BCM6368_GPIO_MODE_PCI_INTB          (1<<18)
#define BCM6368_GPIO_MODE_PCI_GNT1          (1<<17)
#define BCM6368_GPIO_MODE_PCI_REQ1          (1<<16)
#define BCM6368_GPIO_MODE_USBD_LED          (1<<14)
#define BCM6368_GPIO_MODE_ROBOSW_LED1       (1<<13)
#define BCM6368_GPIO_MODE_ROBOSW_LED0       (1<<12)
#define BCM6368_GPIO_MODE_ROBOSW_LED_CLK    (1<<11)
#define BCM6368_GPIO_MODE_ROBOSW_LED_DATA   (1<<10)
#define BCM6368_GPIO_MODE_EPHY3_LED         (1<<9)
#define BCM6368_GPIO_MODE_EPHY2_LED         (1<<8)
#define BCM6368_GPIO_MODE_EPHY1_LED         (1<<7)
#define BCM6368_GPIO_MODE_EPHY0_LED         (1<<6)
#define BCM6368_GPIO_MODE_INET_LED          (1<<5)
#define BCM6368_GPIO_MODE_SERIAL_LED_CLK    (1<<4)
#define BCM6368_GPIO_MODE_SERIAL_LED_DATA   (1<<3)
#define BCM6368_GPIO_MODE_SYS_IRQ           (1<<2)
#define BCM6368_GPIO_MODE_ANALOG_AFE_1      (1<<1)
#define BCM6368_GPIO_MODE_ANALOG_AFE_0      (1<<0)



    uint32      VregConfig;                 /* 1C */
    uint32      AuxLedInterval;             /* 20 */
#define BCM6368_AUX_LED_IN_7            0x80000000
#define BCM6368_AUX_LED_IN_6            0x40000000
#define BCM6368_AUX_LED_IN_5            0x20000000
#define BCM6368_AUX_LED_IN_4            0x10000000
#define BCM6368_AUX_LED_IN_MASK         0xf0000000
#define BCM6368_LED_IN_3                0x08000000
#define BCM6368_LED_IN_2                0x04000000
#define BCM6368_LED_IN_1                0x02000000
#define BCM6368_LED_IN_0                0x01000000
#define BCM6368_AUX_LED_TEST            0x00400000
#define BCM6368_USE_NEW_INTV            0x00200000
#define BCM6368_LED7_LNK_ORAND          0x00100000
#define BCM6368_LED7_LNK_MASK           0x000f0000
#define BCM6368_LED7_LNK_MASK_SHFT      16
#define BCM6368_LED7_ACT_MASK           0x0000f000
#define BCM6368_LED7_ACT_MASK_SHFT      12
#define BCM6368_AUX_FLASH_INTV          0x00000fc0
#define BCM6368_AUX_FLASH_INTV_SHFT     6
#define BCM6368_AUX_BLINK_INTV          0x0000003f
    uint32      AuxLedCtrl;                 /* 24 */
#define BCM6368_AUX_HW_DISAB_7          0x80000000
#define BCM6368_AUX_STROBE_7            0x40000000
#define BCM6368_AUX_MODE_7              0x30000000
#define BCM6368_AUX_MODE_SHFT_7         28
#define BCM6368_AUX_HW_DISAB_6          0x08000000
#define BCM6368_AUX_STROBE_6            0x04000000
#define BCM6368_AUX_MODE_6              0x03000000
#define BCM6368_AUX_MODE_SHFT_6         24
#define BCM6368_AUX_HW_DISAB_5          0x00800000
#define BCM6368_AUX_STROBE_5            0x00400000
#define BCM6368_AUX_MODE_5              0x00300000
#define BCM6368_AUX_MODE_SHFT_5         20
#define BCM6368_AUX_HW_DISAB_4          0x00080000
#define BCM6368_AUX_STROBE_4            0x00040000
#define BCM6368_AUX_MODE_4              0x00030000
#define BCM6368_AUX_MODE_SHFT_4         16
#define BCM6368_AUX_HW_DISAB_3          0x00008000
#define BCM6368_AUX_STROBE_3            0x00004000
#define BCM6368_AUX_MODE_3              0x00003000
#define BCM6368_AUX_MODE_SHFT_3         12
#define BCM6368_AUX_HW_DISAB_2          0x00000800
#define BCM6368_AUX_STROBE_2            0x00000400
#define BCM6368_AUX_MODE_2              0x00000300
#define BCM6368_AUX_MODE_SHFT_2         8
#define BCM6368_AUX_HW_DISAB_1          0x00000080
#define BCM6368_AUX_STROBE_1            0x00000040
#define BCM6368_AUX_MODE_1              0x00000030
#define BCM6368_AUX_MODE_SHFT_1         4
#define BCM6368_AUX_HW_DISAB_0          0x00000008
#define BCM6368_AUX_STROBE_0            0x00000004
#define BCM6368_AUX_MODE_0              0x00000003
#define BCM6368_AUX_MODE_SHFT_0         0

#define BCM6368_LED_STEADY_OFF          0x0
#define BCM6368_LED_FLASH               0x1
#define BCM6368_LED_BLINK               0x2
#define BCM6368_LED_STEADY_ON           0x3

    uint32      TestControl;                /* 28 */

    uint32      VDSLControl;                /* 2C */
#define BCM6368_VDSL_CORE_RESET         (1<<2)
#define BCM6368_VDSL_MIPS_RESET         (1<<1)
#define BCM6368_VDSL_MIPS_POR_RESET     (1<<0)
#define BCM6368_VDSL_CLK_RATIO_SHIFT    8
#define BCM6368_VDSL_CLK_RATIO_MSK        (0x1F << BCM6368_VDSL_CLK_RATIO_SHIFT)
    uint32      RoboSWLEDControl;           /* 30 */
    uint32      RoboSWLEDLSR;               /* 34 */
    uint32      GPIOBaseMode;               /* 38 */
#define BCM6368_EN_PCI_CLK_66           (1<<20)
#define BCM6368_EN_UTO_CLK_FAST         (1<<19)
#define BCM6368_EN_UTO                  (1<<18)
#define BCM6368_EN_GMII2                (1<<17)
#define BCM6368_EN_GMII1                (1<<16)
    uint32      RoboswEphyCtrl;             /* 3C */
#define BCM6368_RSW_HW_FWDG_EN          (1<<19)
#define BCM6368_RSW_MII_DUMP_FWDG       (1<<16)
#define BCM6368_EPHY_RST_4              (1<<9)
#define BCM6368_EPHY_RST_3              (1<<8)
#define BCM6368_EPHY_RST_2              (1<<7)
#define BCM6368_EPHY_RST_1              (1<<6)
#define BCM6368_EPHY_PWR_DOWN_4         (1<<5)
#define BCM6368_EPHY_PWR_DOWN_3         (1<<4)
#define BCM6368_EPHY_PWR_DOWN_2         (1<<3)
#define BCM6368_EPHY_PWR_DOWN_1         (1<<2)
    uint32      StrapBus;                   /* 40 */
#define BCM6368_UTOPIA_MASTER_ON        (1<<14)
    uint32      StrapOverride;              /* 44 */
    uint32      unused1[2];                 /* 48 */
    uint32      SerialLed;                  /* 50 */
    uint32      SerialLedCtrl;              /* 54 */
#define BCM6368_SER_LED_BUSY            (1<<3)
#define BCM6368_SER_LED_POLARITY        (1<<2)
#define BCM6368_SER_LED_DIV_1           0
#define BCM6368_SER_LED_DIV_2           1
#define BCM6368_SER_LED_DIV_4           2
#define BCM6368_SER_LED_DIV_8           3
#define BCM6368_SER_LED_DIV_MASK        0x3
#define BCM6368_SER_LED_DIV_SHIFT       0
    uint32    unused2[4];                   /* 58 */
    uint32    DieRevID;                     /* 68 */
    uint32    DiagMemStatus;                /* 6c */
    uint32    DiagSelControl;               /* 70 */
    uint32    DiagReadBack;                 /* 74 */
    uint32    DiagReadBackHi;               /* 78 */
    uint32    DiagMiscControl;              /* 7c */
#define BCM6368_EPHY_SA_RESET_N           0x00000300
#define BCM6368_EPHY_SA_TESTEN            0x00000500
#define BCM6368_EPHY_SA_CLOCK_RESET       0x0000d900
} BCM6368GpioControl;

#define BCM6368_GPIO ((volatile BCM6368GpioControl * const) BCM6368_GPIO_BASE)

/* Number to mask conversion macro used for GPIODir and GPIOio */
#define BCM6368_GPIO_NUM_TOTAL_BITS_MASK        0x3f
#define BCM6368_GPIO_NUM_MAX_BITS_MASK          0x1f
#define BCM6368_GPIO_NUM_TO_MASK(X)             ( (((X) & BCM6368_GPIO_NUM_TOTAL_BITS_MASK) < 32) ? (1 << ((X) & BCM6368_GPIO_NUM_MAX_BITS_MASK)) : (0) )

/* Number to mask conversion macro used for GPIODir_high and GPIOio_high */
#define BCM6368_GPIO_NUM_MAX_BITS_MASK_HIGH     0x07
#define BCM6368_GPIO_NUM_TO_MASK_HIGH(X)        ( (((X) & BCM6368_GPIO_NUM_TOTAL_BITS_MASK) >= 32) ? (1 << ((X-32) & BCM6368_GPIO_NUM_MAX_BITS_MASK_HIGH)) : (0) )

/*
** Pcm Controller
*/

typedef struct BCM6368PcmControlRegisters
{
    uint32 pcm_ctrl;                                 // 00 offset from PCM_BASE
#define  BCM6368_PCM_ENABLE              0x80000000     // PCM block master enable
#define  BCM6368_PCM_ENABLE_SHIFT        31
#define  BCM6368_PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  BCM6368_PCM_SLAVE_SEL_SHIFT     30
#define  BCM6368_PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  BCM6368_PCM_CLOCK_INV_SHIFT     29
#define  BCM6368_PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  BCM6368_PCM_FS_INVERT_SHIFT     28
#define  BCM6368_PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  BCM6368_PCM_FS_FREQ_16_8_SHIFT  27
#define  BCM6368_PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  BCM6368_PCM_FS_LONG_SHIFT       26
#define  BCM6368_PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  BCM6368_PCM_FS_TRIG_SHIFT       25
#define  BCM6368_PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  BCM6368_PCM_DATA_OFF_SHIFT      24
#define  BCM6368_PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  BCM6368_PCM_DATA_16_8_SHIFT     23
#define  BCM6368_PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  BCM6368_PCM_CLOCK_SEL_SHIFT     20
                                                     // 000 - 8192 Khz
                                                     // 001 - 4096 Khz
                                                     // 010 - 2048 Khz
                                                     // 011 - 1024 Khz
                                                     // 100 - 512 Khz
                                                     // 101 - 256 Khz
                                                     // 110 - 128 Khz
                                                     // 111 - reserved
#define  BCM6368_PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  BCM6368_PCM_LSB_FIRST_SHIFT     18
#define  BCM6368_PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  BCM6368_PCM_LOOPBACK_SHIFT      17
#define  BCM6368_PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in BCM6368
#define  BCM6368_PCM_EXTCLK_SEL_SHIFT    16
#define  BCM6368_PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in BCM6368
#define  BCM6368_PCM_NTR_ENABLE_SHIFT    15
#define  BCM6368_PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  BCM6368_PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  BCM6368_PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min

    uint32 pcm_chan_ctrl;                            // 04
#define  BCM6368_PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  BCM6368_PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  BCM6368_PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  BCM6368_PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  BCM6368_PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  BCM6368_PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  BCM6368_PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  BCM6368_PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  BCM6368_PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  BCM6368_PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  BCM6368_PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  BCM6368_PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  BCM6368_PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  BCM6368_PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  BCM6368_PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  BCM6368_PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  BCM6368_PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  BCM6368_PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                          // 08
    uint32 pcm_int_mask;                             // 0c
#define  BCM6368_PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  BCM6368_PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  BCM6368_PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  BCM6368_PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  BCM6368_PCM_TX_IRQ              0x00000010

    uint32 pcm_pll_ctrl1;                            // 10
#define  BCM6368_PCM_PLL_PWRDN           0x80000000     // PLL PWRDN
#define  BCM6368_PCM_PLL_PWRDN_CH1       0x40000000     // PLL CH PWRDN
#define  BCM6368_PCM_PLL_REFCMP_PWRDN    0x20000000     // PLL REFCMP PWRDN
#define  BCM6368_PCM_CLK16_RESET         0x10000000     // 16.382 MHz PCM interface circuitry reset. 
#define  BCM6368_PCM_PLL_ARESET          0x08000000     // PLL Analog Reset
#define  BCM6368_PCM_PLL_DRESET          0x04000000     // PLL Digital Reset

    uint32 pcm_pll_ctrl2;                            // 14
    uint32 pcm_pll_ctrl3;                            // 18
    uint32 pcm_pll_ctrl4;                            // 1c

    uint32 pcm_pll_stat;                             // 20
#define  BCM6368_PCM_PLL_LOCK            0x00000001     // Asserted when PLL is locked to programmed frequency

    uint32 pcm_ntr_counter;                          // 24

    uint32 unused[6];

#define  BCM6368_PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                     // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[BCM6368_PCM_MAX_TIMESLOT_REGS];
#define  BCM6368_PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} BCM6368PcmControlRegisters;

#define BCM6368_PCM ((volatile BCM6368PcmControlRegisters * const) BCM6368_PCM_BASE)

typedef struct BCM6368PcmIudmaRegisters
{
    uint16 reserved0;
    uint16 ctrlConfig;
#define BCM6368_IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

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
} BCM6368PcmIudmaRegisters;

typedef struct BCM6368PcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM6368_IUDMA_CONFIG_ENDMA       0x0001
#define BCM6368_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM6368_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM6368_IUDMA_INTSTAT_BDONE   0x0001
#define BCM6368_IUDMA_INTSTAT_PDONE   0x0002
#define BCM6368_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM6368_IUDMA_INTSTAT_MASK    0x0007
#define BCM6368_IUDMA_INTSTAT_ALL     BCM6368_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM6368_IUDMA_INTMASK_BDONE   0x0001
#define BCM6368_IUDMA_INTMASK_PDONE   0x0002
#define BCM6368_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM6368_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} BCM6368PcmIudmaChannelCtrl;


typedef struct BCM6368PcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM6368_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} BCM6368PcmIudmaStateRam;

#define BCM6368_MAX_PCM_DMA_CHANNELS 2

typedef struct BCM6368PcmIudma
{
   BCM6368PcmIudmaRegisters regs;                                        // 
   uint32 reserved1[110];                                             //            
   BCM6368PcmIudmaChannelCtrl ctrl[BCM6368_MAX_PCM_DMA_CHANNELS];        //
   uint32 reserved2[120];                                             //
   BCM6368PcmIudmaStateRam stram[BCM6368_MAX_PCM_DMA_CHANNELS];          //

} BCM6368PcmIudma;

#define BCM6368_PCM_IUDMA ((volatile BCM6368PcmIudma * const) BCM6368_PCM_DMA_BASE)


#define BCM6368_IUDMA_MAX_CHANNELS          32

/*
** DMA Channel Configuration (1 .. 32)
*/
typedef struct BCM6368DmaChannelCfg {
  uint32        cfg;                             /* (00) assorted configuration */
#define         BCM6368_DMA_ENABLE      0x00000001  /* set to enable channel */
#define         BCM6368_DMA_PKT_HALT    0x00000002  /* idle after an EOP flag is detected */
#define         BCM6368_DMA_BURST_HALT  0x00000004  /* idle after finish current memory burst */
  uint32        intStat;                         /* (04) interrupts control and status */
  uint32        intMask;                         /* (08) interrupts mask */
#define         BCM6368_DMA_BUFF_DONE   0x00000001  /* buffer done */
#define         BCM6368_DMA_DONE        0x00000002  /* packet xfer complete */
#define         BCM6368_DMA_NO_DESC     0x00000004  /* no valid descriptors */
  uint32        maxBurst;                        /* (0C) max burst length permitted */
} BCM6368DmaChannelCfg;

/*
** DMA State RAM (1 .. 16)
*/
typedef struct BCM6368DmaStateRam {
  uint32        baseDescPtr;            /* (00) descriptor ring start address */
  uint32        state_data;             /* (04) state/bytes done/ring offset */
  uint32        desc_len_status;        /* (08) buffer descriptor status and len */
  uint32        desc_base_bufptr;       /* (0C) buffer descrpitor current processing */
} BCM6368DmaStateRam;


/*
** DMA Registers
*/
typedef struct BCM6368DmaRegs {
    uint32 controller_cfg;              /* (00) controller configuration */
#define BCM6368_DMA_MASTER_EN           0x00000001
#define BCM6368_DMA_FLOWC_CH1_EN        0x00000002
#define BCM6368_DMA_FLOWC_CH3_EN        0x00000004

    // Flow control Ch1
    uint32 flowctl_ch1_thresh_lo;           /* 004 */
    uint32 flowctl_ch1_thresh_hi;           /* 008 */
    uint32 flowctl_ch1_alloc;               /* 00c */
#define BCM6368_DMA_BUF_ALLOC_FORCE     0x80000000

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
    BCM6368DmaChannelCfg chcfg[BCM6368_IUDMA_MAX_CHANNELS];/* (200-3FF) Channel configuration */
    union {
        BCM6368DmaStateRam     s[BCM6368_IUDMA_MAX_CHANNELS];
        uint32          u32[4 * BCM6368_IUDMA_MAX_CHANNELS];
    } stram;                                /* (400-5FF) state ram */
} BCM6368DmaRegs;

/*
** DMA Buffer 
*/
typedef struct BCM6368DmaDesc {
  uint16        length;                 /* in bytes of data in buffer */
#define          BCM6368_DMA_DESC_USEFPM        0x8000
#define          BCM6368_DMA_DESC_MULTICAST     0x4000
#define          BCM6368_DMA_DESC_BUFLENGTH     0x0fff
  uint16        status;                 /* buffer status */
#define          BCM6368_DMA_OWN                0x8000  /* cleared by DMA, set by SW */
#define          BCM6368_DMA_EOP                0x4000  /* last buffer in packet */
#define          BCM6368_DMA_SOP                0x2000  /* first buffer in packet */
#define          BCM6368_DMA_WRAP               0x1000  /* */
#define          BCM6368_DMA_APPEND_BRCM_TAG    0x0200
#define          BCM6368_DMA_APPEND_CRC         0x0100
#define          BCM6368_USB_ZERO_PKT           (1<< 0) // Set to send zero length packet

  uint32        address;                /* address of data */
} BCM6368DmaDesc;

#endif /* __MAP_6368_H */

