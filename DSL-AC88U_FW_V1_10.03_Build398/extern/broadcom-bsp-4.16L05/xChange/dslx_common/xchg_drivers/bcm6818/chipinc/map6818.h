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
/*   MODULE:  map6818.h                                                */
/*   DATE:    05/30/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6818                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __MAP6818_H
#define __MAP6818_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bcmtypesCommon.h>
#include "common6818.h"

/*
** Misc Register Set Definitions.
*/


typedef struct BCM6818Misc {
  uint32        miscPllCtrl ;              /* (0x00) */
  uint32        miscVregCfg0;              /* (0x04) */
  uint32        miscVregCfg1;              /* (0x08) */
  uint32        miscVregCfg2;              /* (0x0C) */
  uint32        miscMemcControl ;          /* (0x10) MEMC Control     */
#define BCM6818_MISC_MEMC_CONTROL_DDR_SLEEP_MODE                           0x00000010
#define BCM6818_MISC_MEMC_CONTROL_MC_UBUS_ASYNC_MODE                       (1<<3)
#define BCM6818_MISC_MEMC_CONTROL_MC_LMB_ASYNC_MODE                        (1<<2)
#define BCM6818_MISC_MEMC_CONTROL_DDR_TEST_DONE                            0x00000002
#define BCM6818_MISC_MEMC_CONTROL_DDR_TEST_DISABLE                         0x00000001
  uint32        miscStrapBus ;             /* (0x14) Strap Register   */
//#define BCM6818_MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT                         27
//#define BCM6818_MISC_STRAP_BUS_MIPS_PLL_FVCO_MASK                          (0x1F<<MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT)
#define BCM6818_MISC_STRAP_BUS_HRD_RST_DELAY                               0x04000000
#define BCM6818_MISC_STRAP_BUS_ALT_BFC_EN                                  0x02000000
#define BCM6818_MISC_STRAP_BUS_DDR2_DDR3_SELECT                            0x00800000
#define BCM6818_MISC_STRAP_BUS_IXTAL_ADJ_MASK                              0x00600000
#define BCM6818_MISC_STRAP_BUS_IXTAL_ADJ_SHIFT                             21
#define BCM6818_MISC_STRAP_BUS_BYPASS_XTAL                                 0x00100000
//#define BCM6818_MISC_STRAP_BUS_TS                                          0x00080000
#define BCM6818_MISC_STRAP_BUS_APM_PICO_BOOT_ROM                           0x00040000
#define BCM6818_MISC_STRAP_BUS_TA                                          0x00020000
#define BCM6818_MISC_STRAP_BUS_ROBOSW_2_MODE_MASK                          0x00018000
#define BCM6818_MISC_STRAP_BUS_ROBOSW_2_MODE_SHIFT                         15
#define BCM6818_MISC_STRAP_BUS_ROBOSW_1_MODE_MASK                          0x00006000
#define BCM6818_MISC_STRAP_BUS_ROBOSW_1_MODE_SHIFT                         13
#define BCM6818_MISC_STRAP_BUS_BIST_CLRMEM_N                               0x00001000
#define BCM6818_MISC_STRAP_BUS_PLL_MIPS_WAIT_FAST_N                        0x00000200
#define BCM6818_MISC_STRAP_BUS_PLL_USE_LOCK                                0x00000100
#define BCM6818_MISC_STRAP_BUS_PCIE_ROOT_COMPLEX                           0x00000080
#define BCM6818_MISC_STRAP_BUS_LS_SPIM_ENABLED                             0x00000040
#define BCM6818_MISC_STRAP_BUS_USE_SPI_MASTER                              0x00000020
#define BCM6818_MISC_STRAP_BUS_SPI_CLK_FAST                                0x00000010
#define BCM6818_MISC_STRAP_BUS_SPI_BOOT_DELAY                              0x00000008
#define BCM6818_MISC_STRAP_BUS_MIPS_BOOT16                                 0x00000004
#define BCM6818_MISC_STRAP_BUS_BOOT_SEL_MASK                               0x00000003
#define BCM6818_MISC_STRAP_BUS_BOOT_SEL_SHIFT                              0
#define BCM6818_MISC_STRAP_BUS_BOOT_PARALLEL                               0x03
#define BCM6818_MISC_STRAP_BUS_BOOT_SERIAL                                 0x01
#define BCM6818_MISC_STRAP_BUS_BOOT_NAND                                   0x00
  uint32        miscStrapOverride ;        /* (0x18) Strap Override Reg */
#define BCM6818_MISC_STRAP_OVERRIDE_NAND_FLASH_2                           0x00000008
#define BCM6818_MISC_STRAP_OVERRIDE_NAND_FLASH_1                           0x00000004
#define BCM6818_MISC_STRAP_OVERRIDE_NAND_FLASH_0                           0x00000002
#define BCM6818_MISC_STRAP_OVERRIDE_STRAP_OVERRIDE                         0x00000001
  uint32        miscReserved[2];           /* (0x1C) */
  uint32        miscPllMdivCtrl;           /* (0x24) */
  uint32        miscReserved2[1];          /* (0x28) */
  uint32        miscDdrPllOutEnCh ;        /* (0x2C) DDR PLL Out En Ch Number */
#define BCM6818_MISC_DDR_PLL_OUTEN_CH_DDR_PLL_OUTEN_CH_MASK                0x000001FF
#define BCM6818_MISC_DDR_PLL_OUTEN_CH_DDR_PLL_OUTEN_CH_SHIFT               0

  uint32        miscGpioDiagOverlay ;      /* (0x30) GPIO Diag Overlay Control Reg */
#define BCM6818_MISC_GPIO_DIAG_OVERLAY_DIAG_OVERLAY_EN                     0x00000020
#define BCM6818_MISC_GPIO_DIAG_OVERLAY_DIAG_OVERLAY_MASK                   0x0000001F
#define BCM6818_MISC_GPIO_DIAG_OVERLAY_DIAG_OVERLAY_PORT_39_32             0x00000010
#define BCM6818_MISC_GPIO_DIAG_OVERLAY_DIAG_OVERLAY_PORT_31_24             0x00000008
#define BCM6818_MISC_GPIO_DIAG_OVERLAY_DIAG_OVERLAY_PORT_23_16             0x00000004
#define BCM6818_MISC_GPIO_DIAG_OVERLAY_DIAG_OVERLAY_PORT_15_8              0x00000002
#define BCM6818_MISC_GPIO_DIAG_OVERLAY_DIAG_OVERLAY_PORT_7_0               0x00000001
  uint32        miscGpioModeCtrlHi ;       /* (0x34) GPIO Pin Mode Control Reg Hi */
  uint32        miscIddqCtrl;              /* (0x38) */
#define BCM6818_MISC_IDDQ_CTRL_GMAC		(1<<16)
#define BCM6818_MISC_IDDQ_CTRL_USBH		(1<<15)
#define BCM6818_MISC_IDDQ_CTRL_APMH		(1<<14)
#define BCM6818_MISC_IDDQ_CTRL_FAP 		(1<<13)
#define BCM6818_MISC_IDDQ_CTRL_GPHY 		(1<<12)
#define BCM6818_MISC_IDDQ_CTRL_ROBOSW 		(1<<11)
#define BCM6818_MISC_IDDQ_CTRL_USBD 		(1<<10)
#define BCM6818_MISC_IDDQ_CTRL_AEPHY 		(1<<9)
#define BCM6818_MISC_IDDQ_CTRL_SATA_PHY		(1<<8)
#define BCM6818_MISC_IDDQ_CTRL_GPON		(1<<7)
#define BCM6818_MISC_IDDQ_CTRL_PCIE		(1<<6)
#define BCM6818_MISC_IDDQ_CTRL_MPI		(1<<5)
#define BCM6818_MISC_IDDQ_CTRL_DOSC		(1<<4)
#define BCM6818_MISC_IDDQ_CTRL_MEMC		(1<<3)
#define BCM6818_MISC_IDDQ_CTRL_MIPS		(1<<2)
#define BCM6818_MISC_IDDQ_CTRL_BRIDGE		(1<<1)
#define BCM6818_MISC_IDDQ_CTRL_IPSEC		(1<<0)

  uint32        miscFapIrqMask;            /* (0x3c) */
  uint32        miscFapExtraIrqMask;       /* (0x40) */
  uint32        miscFap2IrqMask;           /* (0x44) */
  uint32        miscFap2ExtraIrqMask;      /* (0x48) */
} BCM6818Misc ;

#define BCM6818_MISC ((volatile BCM6818Misc * const) BCM6818_MISC_BASE)

/*
** UART
*/
typedef struct BCM6818UartChannel {
    byte          unused0;
    byte          control;
#define BCM6818_BRGEN           0x80    /* Control register bit defs */
#define BCM6818_TXEN            0x40
#define BCM6818_RXEN            0x20
#define BCM6818_LOOPBK          0x10
#define BCM6818_TXPARITYEN      0x08
#define BCM6818_TXPARITYEVEN    0x04
#define BCM6818_RXPARITYEN      0x02
#define BCM6818_RXPARITYEVEN    0x01

    byte          config;
#define BCM6818_XMITBREAK       0x40
#define BCM6818_BITS5SYM        0x00
#define BCM6818_BITS6SYM        0x10
#define BCM6818_BITS7SYM        0x20
#define BCM6818_BITS8SYM        0x30
#define BCM6818_ONESTOP         0x07
#define BCM6818_TWOSTOP         0x0f
    /* 4-LSBS represent STOP bits/char
     * in 1/8 bit-time intervals.  Zero
     * represents 1/8 stop bit interval.
     * Fifteen represents 2 stop bits.
     */
    byte          fifoctl;
#define BCM6818_RSTTXFIFOS      0x80
#define BCM6818_RSTRXFIFOS      0x40
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
#define BCM6818_DTREN   0x01
#define BCM6818_RTSEN   0x02

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
#define BCM6818_DELTAIP         0x0001
#define BCM6818_TXUNDERR        0x0002
#define BCM6818_TXOVFERR        0x0004
#define BCM6818_TXFIFOTHOLD     0x0008
#define BCM6818_TXREADLATCH     0x0010
#define BCM6818_TXFIFOEMT       0x0020
#define BCM6818_RXUNDERR        0x0040
#define BCM6818_RXOVFERR        0x0080
#define BCM6818_RXTIMEOUT       0x0100
#define BCM6818_RXFIFOFULL      0x0200
#define BCM6818_RXFIFOTHOLD     0x0400
#define BCM6818_RXFIFONE        0x0800
#define BCM6818_RXFRAMERR       0x1000
#define BCM6818_RXPARERR        0x2000
#define BCM6818_RXBRK           0x4000

    uint16        unused2;
    uint16        Data;                   /* Write to TX, Read from RX */
                                          /* bits 11:8 are BRK,PAR,FRM errors */

    uint32        unused3;
    uint32        unused4;
} BCM6818Uart;

#define BCM6818_UART ((volatile BCM6818Uart * const) BCM6818_UART_BASE)

/*
** Peripheral Controller
*/
typedef struct BCM6818PerfControl {
    uint32        RevID;             /* (00) word 0 */
#define BCM6818_CHIP_ID_SHIFT   16
#define BCM6818_CHIP_ID_MASK    (0xffff << CHIP_ID_SHIFT)
#define BCM6818_REV_ID_MASK     0xff

    uint32        blkEnables;        /* (04) word 1 */
#define BCM6818_GMAC_UBUS_CLK_EN        (1 << 30)
#define BCM6818_APM_AUDIO_UBUS_CLKEN   (1 << 29)
#define BCM6818_MIPS_CLK_EN             (1 << 28)
#define BCM6818_FAP1_CLK_EN             (1 << 27)
#define BCM6818_TESTBUS_CLK_EN          (1 << 26)
#define BCM6818_APM_AUDIO_A_CLKEN       (1 << 25)
#define BCM6818_APM_AUDIO_B_CLKEN       (1 << 24)
#define BCM6818_NTP_CLK_EN              (1 << 23)
#define BCM6818_APM_PCM_CLKEN           (1 << 22)
#define BCM6818_APM_BMU_CLKEN           (1 << 21)
#define BCM6818_PCIE_CLK_EN             (1 << 20)
#define BCM6818_GPON_SER_CLK_EN         (1 << 19)
#define BCM6818_IPSEC_CLK_EN            (1 << 18)
#define BCM6818_NAND_CLK_EN             (1 << 17)
#define BCM6818_DISABLE_GLESS           (1 << 16)
#define BCM6818_ROBOSW_DMA_CLK_EN       (1 << 15)
#define BCM6818_APM_CLK_EN              (1 << 14)
#define BCM6818_FAP0_CLK_EN             (1 << 13)
#define BCM6818_ROBOSW_CLK_EN           (1 << 12)
#define BCM6818_HS_SPI_CLK_EN           (1 << 11)
#define BCM6818_USBH_CLK_EN             (1 << 10) /* Only one bit for USB CLK */
#define BCM6818_USBD_CLK_EN             (1 << 10)
#define BCM6818_SPI_CLK_EN              (1 << 9)
#define BCM6818_SWPKT_GPON_CLK_EN       (1 << 8)
#define BCM6818_SWPKT_USB_CLK_EN        (1 << 7)
#define BCM6818_GPON_CLK_EN             (1 << 6)

#define BCM6818_APM_AUDIO_COMMON_CLKEN  0 // TBD007 this is from 6828, where is in 6818?

    uint32        pll_control;       /* (08) word 2 */
#define BCM6818_SOFT_RESET              0x00000001      // 0

    uint32        deviceTimeoutEn;   /* (0c) word 3 */
    uint32        softResetB;        /* (10) word 4 */
#define BCM6818_SOFT_RST_GPON_UBUS2     (1 << 24)
#define BCM6818_SOFT_RST_SERDES_DIG     (1 << 23)
#define BCM6818_SOFT_RST_SERDES         (1 << 22)
#define BCM6818_SOFT_RST_SERDES_MDIO    (1 << 21)
#define BCM6818_SOFT_RST_SERDES_PLL     (1 << 20)
#define BCM6818_SOFT_RST_SERDES_HW      (1 << 19)
#define BCM6818_SOFT_RST_GPON           (1 << 18)
#define BCM6818_SOFT_RST_BMU            (1 << 17)
#define BCM6818_SOFT_RST_HVG            (1 << 16)
#define BCM6818_SOFT_RST_APM            (1 << 15)
#define BCM6818_SOFT_RST_ACP            (1 << 14)
#define BCM6818_SOFT_RST_PCM            (1 << 13)
#define BCM6818_SOFT_RST_USBH           (1 << 12)
#define BCM6818_SOFT_RST_USBD           (1 << 11)
#define BCM6818_SOFT_RST_SWITCH         (1 << 10)
#define BCM6818_SOFT_RST_FAP1           (1 << 9)
#define BCM6818_SOFT_RST_PCIE_HRST      (1 << 8)
#define BCM6818_SOFT_RST_FAP0           (1 << 7)
#define BCM6818_SOFT_RST_EPHY           (1 << 6)
#define BCM6818_SOFT_RST_PCIE           (1 << 5)
#define BCM6818_SOFT_RST_IPSEC          (1 << 4)
#define BCM6818_SOFT_RST_MPI            (1 << 3)
#define BCM6818_SOFT_RST_PCIE_EXT       (1 << 2)
#define BCM6818_SOFT_RST_PCIE_CORE      (1 << 1)
#define BCM6818_SOFT_RST_SPI            (1 << 0)

    uint32        diagControl;        /* (14) word 5 */
    uint32        ExtIrqCfg;          /* (18) word 6*/
    uint32        ExtIrqCfg1;         /* (1c) word 7 */
#define BCM6818_EI_SENSE_SHFT   0
#define BCM6818_EI_STATUS_SHFT  4
#define BCM6818_EI_CLEAR_SHFT   8
#define BCM6818_EI_MASK_SHFT    12
#define BCM6818_EI_INSENS_SHFT  16
#define BCM6818_EI_LEVEL_SHFT   20
     uint32        IrqMask_high;      /* (20) word 8 */
     uint32        IrqMask;           /* (24) word 9 */
     uint32        IrqStatus_high;    /* (28) word 10 */
     uint32        IrqStatus;         /* (2c) word 11 */

     uint32        IrqMask1_high;     /* (30) word 13 */
     uint32        IrqMask1;          /* (34) word 14 */
     uint32        IrqStatus1_high;   /* (38) word 15 */
     uint32        IrqStatus1;        /* (3c) word 16 */
//     BCM6818IrqControl_t IrqControl[2];
} BCM6818PerfControl;
#define BCM6818_PERF ((volatile BCM6818PerfControl * const) BCM6818_PERF_BASE)

/*
** APM/PCM IUDMA Controller
*/

typedef struct BCM6818ApmPcmIudmaRegisters
{
    uint16 reserved0;
    uint16 ctrlConfig;
#define BCM6818_IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define BCM6818_IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define BCM6818_IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define BCM6818_IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define BCM6818_IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

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
} BCM6818ApmPcmIudmaRegisters;

/*
** DMA descriptor
*/
typedef struct BCM6818ApmPcmIudmaDesc {
  uint16        length;                 /* in bytes of data in buffer */
#define BCM6818_DMA_DESC_USEFPM    0x8000
#define BCM6818_DMA_DESC_MULTICAST 0x4000
#define BCM6818_DMA_DESC_BUFLENGTH 0x0fff
  uint16        status;                 /* buffer status */
#define BCM6818_DMA_OWN                0x8000  /* cleared by DMA, set by SW */
#define BCM6818_DMA_EOP                0x4000  /* last buffer in packet */
#define BCM6818_DMA_SOP                0x2000  /* first buffer in packet */
#define BCM6818_DMA_WRAP               0x1000  /* */
#define BCM6818_DMA_PRIO               0x0C00  /* Prio for Tx */
#define BCM6818_DMA_APPEND_BRCM_TAG    0x0200
#define BCM6818_DMA_APPEND_CRC         0x0100
#define BCM6818_USB_ZERO_PKT           (1<< 0) // Set to send zero length packet

  uint32        address;                /* address of data */
} BCM6818ApmPcmIudmaDesc;

typedef struct BCM6818ApmPcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM6818_IUDMA_CONFIG_ENDMA       0x0001
#define BCM6818_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM6818_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM6818_IUDMA_INTSTAT_BDONE   0x0001
#define BCM6818_IUDMA_INTSTAT_PDONE   0x0002
#define BCM6818_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM6818_IUDMA_INTSTAT_MASK    0x0007
#define BCM6818_IUDMA_INTSTAT_ALL     BCM6818_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM6818_IUDMA_INTMASK_BDONE   0x0001
#define BCM6818_IUDMA_INTMASK_PDONE   0x0002
#define BCM6818_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM6818_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} BCM6818ApmPcmIudmaChannelCtrl;


typedef struct BCM6818ApmPcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM6818_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} BCM6818ApmPcmIudmaStateRam;

#define BCM6818_MAX_APM_PCM_DMA_CHANNELS 6

typedef struct BCM6818ApmPcmIudma
{
   BCM6818ApmPcmIudmaRegisters regs;                                        //
   uint32 reserved1[52];                                             //
   BCM6818ApmPcmIudmaChannelCtrl ctrl[BCM6818_MAX_APM_PCM_DMA_CHANNELS];    //
   uint32 reserved2[40];                                             //
   BCM6818ApmPcmIudmaStateRam stram[BCM6818_MAX_APM_PCM_DMA_CHANNELS];      //

} BCM6818ApmPcmIudma;

#define BCM6818_APM_PCM_IUDMA ((volatile BCM6818ApmPcmIudma * const) BCM6818_APM_PCM_DMA_BASE)


/*
** PCM  Controller
*/

typedef struct BCM6818PcmControlRegisters
{
    uint32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define BCM6818_PCM_ENABLE              0x80000000     // PCM block master enable
#define BCM6818_PCM_ENABLE_SHIFT        31
#define BCM6818_PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define BCM6818_PCM_SLAVE_SEL_SHIFT     30
#define BCM6818_PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define BCM6818_PCM_CLOCK_INV_SHIFT     29
#define BCM6818_PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define BCM6818_PCM_FS_INVERT_SHIFT     28
#define BCM6818_PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define BCM6818_PCM_FS_FREQ_16_8_SHIFT  27
#define BCM6818_PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define BCM6818_PCM_FS_LONG_SHIFT       26
#define BCM6818_PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define BCM6818_PCM_FS_TRIG_SHIFT       25
#define BCM6818_PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define BCM6818_PCM_DATA_OFF_SHIFT      24
#define BCM6818_PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define BCM6818_PCM_DATA_16_8_SHIFT     23
#define BCM6818_PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define BCM6818_PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define BCM6818_PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define BCM6818_PCM_LSB_FIRST_SHIFT     18
#define BCM6818_PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define BCM6818_PCM_LOOPBACK_SHIFT      17
#define BCM6818_PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in 6818
#define BCM6818_PCM_EXTCLK_SEL_SHIFT    16
#define BCM6818_PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 6818
#define BCM6818_PCM_NTR_ENABLE_SHIFT    15
#define BCM6818_PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define BCM6818_PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define BCM6818_PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min
#define BCM6818_PCM_AP_SEL              0x00000001     // 1 - Connect pcm to pads, 0 - Connects audio test interface

    uint32 pcm_chan_ctrl;                       // 04
#define BCM6818_PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define BCM6818_PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define BCM6818_PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define BCM6818_PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define BCM6818_PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define BCM6818_PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define BCM6818_PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define BCM6818_PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define BCM6818_PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define BCM6818_PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define BCM6818_PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define BCM6818_PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define BCM6818_PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define BCM6818_PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define BCM6818_PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define BCM6818_PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define BCM6818_PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define BCM6818_PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define BCM6818_PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define BCM6818_PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define BCM6818_PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define BCM6818_PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define BCM6818_PCM_TX_IRQ              0x00000010

    uint32        reg_pcm_clk_cntl_0;             // (0x210) PCM Clock Control 0 (NCO_FCW_MISC)
    uint32        reg_pcm_clk_cntl_1;             // (0x214) PCM Clock Control 1 (NCO_SCALE)
    uint32        reg_pcm_clk_cntl_2;             // (0x218) PCM Clock Control 2
#define BCM6818_PCM_NCO_SHIFT          0x0000000f
#define BCM6818_PCM_NCO_MUX_CNTL       0x00000030
#define BCM6818_PCM_NCO_LOAD_MISC      0x00000040
#define BCM6818_PCM_NCO_SOFT_INIT      0x00000080

    uint32        reserved;
    uint32        pcm_zds_intf;                    // ZSI Config register ( New in 6818 )
    uint32        pcm_msif_intf;                   // ISI Config register ( New in 6818 )

    uint32 unused[6];
#define BCM6818_PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[BCM6818_PCM_MAX_TIMESLOT_REGS];
#define BCM6818_PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} BCM6818PcmControlRegisters;

#define BCM6818_PCM ((volatile BCM6818PcmControlRegisters * const) BCM6818_PCM_BASE)

/*
** Gpio Controller
*/
typedef struct BCM6818GpioControl {
    uint32      GPIODir_high;               /* bits 36:32 */
    uint32      GPIODir;                    /* bits 31:00 */
    uint32      GPIOio_high;                /* bits 36:32 */
    uint32      GPIOio;                     /* bits 31:00 */
    uint32      LEDCtrl;
    uint32      SpiSlaveCfg;                /* 14 */
    uint32      GPIOMode;                   /* 18 */
    uint32      VregConfig;                 /* 1C */
    uint32      AuxLedInterval;             /* 20 */
    uint32      AuxLedCtrl;                 /* 24 */
    uint32      TestControl;                /* 28 */
    uint32      OscControl;                 /* 2C */
    uint32      RoboSWLEDControl;           /* 30 */
    uint32      RoboSWLEDLSR;               /* 34 */
    uint32      GPIOBaseMode;               /* 38 */
    uint32      RoboswEphyCtrl;             /* 3C */
    uint32      MiiPadCtrl;                 /* 40 */
    uint32      unused1[1];                 /* 44 */
    uint32      RingOscCtrl0;               /* 48 */
    uint32      RingOscCtrl1;               /* 4C */
    uint32      SerialLed;                  /* 50 */
    uint32      SerialLedCtrl;              /* 54 */
    uint32    SerialLedBlink;               /* 58 */
    uint32    SerdesCtl;                    /* 5c */
    uint32    SerdesStatus;                 /* 60 */
    uint32    SataPhyPwrdwn;                /* 64 */
    uint32    DieRevID;                     /* 68 */
    uint32    unused2;                      /* 6c */
    //uint32    DiagMemStatus;              /* 6c */
    uint32    DiagSelControl;               /* 70 */
    uint32    DiagReadBack;                 /* 74 */
    uint32    DiagReadBackHi;               /* 78 */
    uint32    DiagMiscControl;              /* 7c */
} BCM6818GpioControl;

#define BCM6818_GPIO ((volatile BCM6818GpioControl * const) BCM6818_GPIO_BASE)

/* Number to mask conversion macro used for GPIODir and GPIOio */
#define BCM6818_GPIO_NUM_MAX                    40
#define BCM6818_GPIO_NUM_TO_MASK(X)             ( (((X) & BCM6818_BP_GPIO_NUM_MASK) <= BCM6818_GPIO_NUM_MAX) ? ((uint64)1 << ((X) & BCM6818_BP_GPIO_NUM_MASK)) : (0) )	

/* Some GPIO defs to comply with flexible hal framework */
#define BCM6818_GPIO_NUM_TOTAL_BITS_MASK        BCM6818_GPIO_NUM_MAX //Max possible GPIOs in the system
#define BCM6818_GPIO_NUM_MAX_BITS_MASK          0x1f            // Max GPIO bits in a register


typedef struct BCM6818ApmControlRegisters
{
     uint16 apm_dev_diag_sel;
#define   BCM6818_APM_DIAG_HI_SEL                 0xff00
#define   BCM6818_APM_DIAG_LO_SEL                 0x00ff

     uint16 apm_dev_irq_status;                     // (0x00) read only block interrupt status
#define   BCM6818_DEV_INTERRUPT                   0x0001        // DMA interrupt pending
#define   BCM6818_APM_INTERRUPT_1                 0x0002        // APM interrupt pending from bank 1
#define   BCM6818_APM_INTERRUPT_2                 0x0004        // APM interrupt pending from bank 2
#define   BCM6818_APM_INTERRUPT_3                 0x0008        // APM interrupt pending from bank 3
#define   BCM6818_PCM_INTERRUPT                   0x0010        // PCM interrupt pending

// semantic from IUDMA perpesctive
// Tx mem to APM
// Rx APM to mem
     uint32 apm_dev_irq_pend;                       // (0x04) DMA interrupt pending register
#define   BCM6818_DMA_A_RX                        0x00000001    // Ch A receive channel interrupt
#define   BCM6818_DMA_A_TX                        0x00000002    // Ch A transmit channel interrupt
#define   BCM6818_DMA_B_RX                        0x00000004    // Ch B receive channel interrupt
#define   BCM6818_DMA_B_TX                        0x00000008    // Ch B transmit channel interrupt
#define   BCM6818_DMA_TX_UNDERFLOW_A              0x00000010    // Ch A transmit channel underflow
#define   BCM6818_DMA_TX_UNDERFLOW_B              0x00000020    // Ch B transmit channel underflow
#define   BCM6818_DMA_RX_OVERFLOW_A               0x00000040    // Ch A receive channel overflow
#define   BCM6818_DMA_RX_OVERFLOW_B               0x00000080    // Ch B receive channel overflow
#define   BCM6818_DMA_PCM_RX                      0x00000100    // PCM Rx DMA IRQ
#define   BCM6818_DMA_PCM_TX                      0x00000200    // PCM Tx DMA IRQ
#define   BCM6818_DEV_BMU_IRQ                     0x00000400    // BMU block IRQ

     uint32 apm_dev_irq_mask;                       // (0x08)DMA interrupt mask register

// Note semantic change
// IUDMA refers to mem to periph as Tx
// and periph to mem as Rx
// APM core refers to path from mem as Rx
// and path to mem as Tx

     uint32 apm_dev_control;                        // (0x0c) Device control register
#define   BCM6818_RX_PACKET_SIZE_A                0x000000ff    // Number of samples to form quasi packet to mem channel A
#define   BCM6818_RX_PACKET_SIZE_B                0x0000ff00    // Number of samples to form quasi packet to mem channel B

#define   BCM6818_RX_DMA_ENABLE_A                 0x10000000    // Enable for Ch A DMA to mem
#define   BCM6818_RX_DMA_ENABLE_A_SHIFT           28

#define   BCM6818_RX_DMA_ENABLE_B                 0x20000000    // Enable for Ch B DMA to mem
#define   BCM6818_RX_DMA_ENABLE_B_SHIFT           29

#define   BCM6818_TX_DMA_ENABLE_A                 0x40000000    // Enable for Ch A DMA from mem
#define   BCM6818_TX_DMA_ENABLE_A_SHIFT           30

#define   BCM6818_TX_DMA_ENABLE_B                 0x80000000    // Enable for Ch B DMA from mem
#define   BCM6818_TX_DMA_ENABLE_B_SHIFT           31

// APM core registers
     uint32 reg_apm_coeff_wr_data;                  // (0x10) 20 bit 2's comp coefficient to be written into coeff RAM
#define   BCM6818_COEFF_PROG_INPUT                0x000FFFFF

     uint32 reg_apm_coeff_config;                   // (0x14)
#define   BCM6818_COEFF_PROG_ADDR                 0x000000FF    //8 bit address into coefficient RAM space
#define   BCM6818_COEFF_PROG_WE                   0x00000100    //1=write into memory 0= read from memory
#define   BCM6818_EQ_TX_ACCESS_COEFF_RAM_A        0x00010000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer
#define   BCM6818_EQ_RX_ACCESS_COEFF_RAM_A        0x00020000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer
#define   BCM6818_HISPD_HYBAL_ACCESS_COEFF_RAM_A  0x00040000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6818_LOSPD_HYBAL_ACCESS_COEFF_RAM_A  0x00080000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6818_YFLTR_ACCESS_COEFF_RAM_A        0x00100000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a hybrid balance
#define   BCM6818_ASRC_INT_ACCESS_COEFF_RAM_A     0x00200000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b hybrid balance
#define   BCM6818_ASRC_DEC_ACCESS_COEFF_RAM_A     0x00400000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a y filter
#define   BCM6818_EQ_TX_ACCESS_COEFF_RAM_B        0x00800000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter
#define   BCM6818_EQ_RX_ACCESS_COEFF_RAM_B        0x01000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter
#define   BCM6818_HISPD_HYBAL_ACCESS_COEFF_RAM_B  0x02000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6818_LOSPD_HYBAL_ACCESS_COEFF_RAM_B  0x04000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6818_YFLTR_ACCESS_COEFF_RAM_B        0x08000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc interpolator
#define   BCM6818_ASRC_INT_ACCESS_COEFF_RAM_B     0x10000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc decimator
#define   BCM6818_ASRC_DEC_ACCESS_COEFF_RAM_B     0x20000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc decimator

#define   BCM6818_APM_HYBAL_NUM_TAPS            5
#define   BCM6818_APM_YFLTR_FIR_NUM_TAPS       18
#define   BCM6818_APM_YFLTR_IIR1_NUM_TAPS       1
#define   BCM6818_APM_YFLTR_IIR2_NUM_TAPS       5
#define   BCM6818_APM_YFLTR_BLOCK_SIZE         28
#define   BCM6818_APM_RX_EQ_NUM_TAPS           80
#define   BCM6818_APM_TX_EQ_NUM_TAPS           80 
#define   BCM6818_APM_ASRC_DEC_NUM_TAPS       120
#define   BCM6818_APM_ASRC_INT_NUM_TAPS       120

     uint32 reg_cha_test_config;                    // (0x18)
#define   BCM6818_RX_IN_MUX_CTRL                  0x00000003    //2 bits mux control  at eq input 0=test stimulus 1=rx_data_in 2=loopback from tx path channel a
#define   BCM6818_RX_IN_MUX_CTRL_SHIFT            0

#define   BCM6818_NETWORK_LPBK                    0x00000004    //1=take data inputs and loopback to data outputs (network side) channel a
#define   BCM6818_NETWORK_LPBK_SHIFT              2

#define   BCM6818_RX_98K_MUX_CTRL                 0x00000018    //1=bypass 0=use eq output on rx side channel a
#define   BCM6818_RX_98K_MUX_CTRL_SHIFT           3

#define   BCM6818_ADC_MUX_CTRL                    0x00000060    //2 bits mux at input to tx path 0=loopback 1=adc out 2=test stimulus channel a
#define   BCM6818_ADC_MUX_CTRL_SHIFT              5

#define   BCM6818_ADC_DAC_LPBK                    0x00000080    //1=take 4 bit ADC codes and send them to the DEM and DAC
#define   BCM6818_ADC_DAC_LPBK_SHIFT              7

#define   BCM6818_RX_TX_98K_LPBK                  0x00000100    //1=loopback from rx to tx path channel a
#define   BCM6818_RX_TX_98K_LPBK_SHIFT            8

#define   BCM6818_SLEW_INSTANT                    0x00000200    //1=slew scale, 0=make scale changes instantaneously
#define   BCM6818_SLEW_INSTANT_SHIFT              9

#define   BCM6818_ASRC_EN                         0x00000400    //1=asrc and eq are active 0=inactive
#define   BCM6818_ASRC_EN_SHIFT                   10


     uint32 reg_chb_test_config;                    // (0x1c)

     uint32 reg_cha_eq_config;                      // (0x20)
#define   BCM6818_EQ_RX_NUM_TAPS                  0x0000007f    //7 bit number of recieve taps
#define   BCM6818_EQ_RX_NUM_TAPS_SHIFT            0

#define   BCM6818_EQ_TX_NUM_TAPS                  0x00007f00    //7 bit number of transmit taps
#define   BCM6818_EQ_TX_NUM_TAPS_SHIFT            8

#define   BCM6818_EQ_TX_SHFT                      0x000f0000    //4 bits controls output shifter (powers of 2 gain control) for rx path channel a
#define   BCM6818_EQ_TX_SHFT_SHIFT                16

#define   BCM6818_EQ_RX_SHFT                      0x00f00000    //4 bits controls output shifter (powers of 2 gain control) for tx path channel a
#define   BCM6818_EQ_RX_SHFT_SHIFT                20

#define   BCM6818_EQ_RX_IMP_RESP                  0x01000000    //1=use ext RAM for eq coefficients 0=use int ROM for eq coefficients channel a
#define   BCM6818_EQ_RX_IMP_RESP_SHIFT            24

#define   BCM6818_EQ_TX_IMP_RESP                  0x02000000    //1 the eq has an impulse shape response, 0= use the RAM or ROM for eq coefficients
#define   BCM6818_EQ_TX_IMP_RESP_SHIFT            25

#define   BCM6818_EQ_RX_ENABLE                    0x04000000    //equalizer transmit enable
#define   BCM6818_EQ_RX_ENABLE_SHIFT              26

#define   BCM6818_EQ_TX_ENABLE                    0x08000000    //equalizer receive enable
#define   BCM6818_EQ_TX_ENABLE_SHIFT              27

#define   BCM6818_SOFT_INIT                       0x80000000    //initializes state machines and registers
#define   BCM6818_SOFT_INIT_SHIFT                 31

     uint32 reg_cha_hybal_config;                   // (0x24)
#define   BCM6818_HISPD_HYBAL_SHFT                0x00000007    //
#define   BCM6818_HISPD_HYBAL_SHFT_SHIFT          0

#define   BCM6818_LOSPD_HYBAL_SHFT                0x00000038    //
#define   BCM6818_LOSPD_HYBAL_SHFT_SHIFT          3

#define   BCM6818_HISPD_HYBAL_EN                  0x00000080    //
#define   BCM6818_HISPD_HYBAL_EN_SHIFT            7

#define   BCM6818_LOSPD_HYBAL_EN                  0x00000100    //
#define   BCM6818_LOSPD_HYBAL_EN_SHIFT            8

#define   BCM6818_HISPD_HYBAL_SMPL_OFFSET         0x00007000    //
#define   BCM6818_HISPD_HYBAL_SMPL_OFFSET_SHIFT   12

#define   BCM6818_YFLTR_EN                        0x00008000    //1=y-filter is active 0=y-filter outputs 0 only
#define   BCM6818_YFLTR_EN_SHIFT                  15

#define   BCM6818_HPF_EN                          0x00010000    //1=high pass filter is active 0=high pass filter outputs 0 only
#define   BCM6818_HPF_EN_SHIFT                    16

#define   BCM6818_LOSPD_HYBAL_SMPL_OFFSET         0x3f000000    //
#define   BCM6818_LOSPD_HYBAL_SMPL_OFFSET_SHIFT   24

     uint32 reg_cha_int_config_1;                   // (0x28)
#define   BCM6818_ASRC_INT_NUM_SECT               0x001f0000    //5 bits--if symmetric it represents 1/2 of the number of sections (minus 1) in the filter channel a
#define   BCM6818_ASRC_INT_NUM_SECT_SHIFT         16            //  if asymmetric, it represents the number of sections (minus 1) channel a

#define   BCM6818_ASRC_INT_HRNR_SHFT              0x03000000    //2 bits defines shifting at output of asrc int hrnr channel a
#define   BCM6818_ASRC_INT_HRNR_SHFT_SHIFT        24

#define   BCM6818_ASRC_INT_SYM_ASYM               0x04000000    //1=filter coefficients are symmetrical (only 1/2 are stored) channel a
#define   BCM6818_ASRC_INT_SYM_ASYM_SHIFT         26

#define   BCM6818_ASRC_INT_COEFF_SEL              0x18000000    //
#define   BCM6818_ASRC_INT_COEFF_SEL_SHIFT        27

     uint32 reg_cha_int_config_2;                   // (0x2c)
#define   BCM6818_ASRC_INT_SCALE                  0x0000ffff    //16 bit scale value channel a
#define   BCM6818_ASRC_INT_SCALE_SHIFT            0

#define   BCM6818_ASRC_INT_FIR_SHFT               0x00070000    //3 bits defines shifting at output of mac before writing into data RAM channel a
#define   BCM6818_ASRC_INT_FIR_SHFT_SHIFT         16

#define   BCM6818_ASRC_SLEW_SPEED                 0x00780000    //
#define   BCM6818_ASRC_SLEW_SPEED_SHIFT           19

     uint32 reg_cha_pm_config_1;                    // (0x30)
#define   BCM6818_PM_ON_TIME                      0x00000fff    //12 bit number in 2 msec units for on-time channel a
#define   BCM6818_PM_ON_TIME_SHIFT                0

#define   BCM6818_PM_OFF_TIME                     0x0fff0000    //12 bit number in 2 msec units for off-time channel a
#define   BCM6818_PM_OFF_TIME_SHIFT               16

#define   BCM6818_PM_FREQ_16_12                   0x10000000    //1=16 kHz frequency 0=12 kHz frequency channel a
#define   BCM6818_PM_FREQ_16_12_SHIFT             28

#define   BCM6818_PM_TIME_MS                      2 / 5


     uint32 reg_cha_pm_config_2;                    // (0x34)
#define   BCM6818_PM_SCALE                        0x0000ffff    //16 bit 2's complement number that scales the output from full scale channel a
#define   BCM6818_PM_SCALE_SHIFT                  0

#define   BCM6818_PM_RISE_INC                     0x00ff0000    //8 bit number goes into accumulator for rise time channel a
#define   BCM6818_PM_RISE_INC_SHIFT               16

#define   BCM6818_PM_FALL_INC                     0xff000000    //8 bit number goes into accumulator for fall time channel a
#define   BCM6818_PM_FALL_INC_SHIFT               24

     uint32 reg_cha_pm_config_3;                    // (0x38)
#define   BCM6818_PM_BURST_LENGTH                 0x0000ffff    //16 bit number defines the number of bursts to send out
#define   BCM6818_PM_BURST_LENGTH_SHIFT           0

#define   BCM6818_PM_BURST_CONT                   0x00010000    //1=send out infinite number of bursts 0=send out "pm_burst_length"
#define   BCM6818_PM_BURST_CONT_SHIFT             16

#define   BCM6818_PM_BURST_START                  0x00020000    //strobe the indicates that a burst should start (this will be edge detected)
#define   BCM6818_PM_BURST_START_SHIFT            17

#define   BCM6818_PM_ENABLE                       0x00040000    //1=pulse metering is active and on, 0=inactive and off channel a
#define   BCM6818_PM_ENABLE_SHIFT                 18

     uint32 reg_cha_cic_config;                     // (0x3c)
#define   BCM6818_CIC_DEC_SHFT                    0x00000007    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM6818_CIC_DEC_SHFT_SHIFT              0

#define   BCM6818_CIC_INC_SHFT                    0x00000038    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM6818_CIC_INC_SHFT_SHIFT              3

     uint32 reg_cha_asrc_dec_config;                // (0x40)
#define   BCM6818_ASRC_DEC_SCALE                  0x0000ffff    //16 bit 2's complement scale value channel a
#define   BCM6818_ASRC_DEC_SCALE_SHIFT            0
#define   BCM6818_ASRC_DEC_SCALE_FACTOR           32 / 25       //conversion factor relative to nominal value

#define   BCM6818_ASRC_DEC_NUM_SECT               0x001f0000    //5 bit number 0-20 defines the number of sections in the filter channel a
#define   BCM6818_ASRC_DEC_NUM_SECT_SHIFT         16            //   if symmetric it equals the number of sections/2 -1
                                                        //   if asymmetric it equals the number of sections -1
#define   BCM6818_ASRC_DEC_SHFT                   0x0f000000    //4 bits defines msb location at input [38:31] channel a
#define   BCM6818_ASRC_DEC_SHFT_SHIFT             24

#define   BCM6818_ASRC_DEC_SYM_ASYM               0x10000000    //1=symmetric filter 0=asymmetric filter channel a
#define   BCM6818_ASRC_DEC_SYM_ASYM_SHIFT         28

#define   BCM6818_ASRC_DEC_COEFF_SEL              0x60000000    //ASRC DEC coefficient select
#define   BCM6818_ASRC_DEC_COEFF_SEL_SHIFT        29

     uint32 reg_cha_fcw_config_1;                   // (0x44)
#define   BCM6818_FCW_SCALE                       0xffffffff    //32 bit unsigned scale value for frequency control word channel a

     uint32 reg_cha_fcw_config_2;                   // (0x48)
#define   BCM6818_FCW_SHFT                        0x0000000f    //4 bit shift control for fcw channel a
#define   BCM6818_FCW_SHFT_SHIFT                  0

#define   BCM6818_CLOCK_SOURCE                    0x00000000    //2 bit mux control 0=DPLL 1=misc 2=NTP 3=TRC
#define   BCM6818_CLOCK_SOURCE_SHIFT              4


     uint32 reg_cha_digmod_config;                  // (0x4c)
#define   BCM6818_DIGMOD_DEM_TONE                 0x00000001    //1=suppress tones at fs/2 channel a
#define   BCM6818_DIGMOD_DEM_TONE_SHIFT           0

#define   BCM6818_DIGMOD_DEM_DIS                  0x00000002    //1=use balanced code as output instead of dynamic element matcher channel a
#define   BCM6818_DIGMOD_DEM_DIS_SHIFT            1

#define   BCM6818_DIGMOD_DITH_SCALE               0x0000ff00    // new 10/1
#define   BCM6818_DIGMOD_DITH_SCALE_SHIFT         8


// CH B use same bit position defines as Ch A
     uint32 reg_chb_eq_config;                      // (0x50)
     uint32 reg_chb_hybal_config;                   // (0x54)
     uint32 reg_chb_int_config_1;                   // (0x58)
     uint32 reg_chb_int_config_2;                   // (0x5c)

     uint32 reg_chb_pm_config_1;                    // (0x60)
     uint32 reg_chb_pm_config_2;                    // (0x64)
     uint32 reg_chb_pm_config_3;                    // (0x68)
     uint32 reg_chb_cic_config;                     // (0x6c)
     uint32 reg_chb_asrc_dec_config;                // (0x70)
     uint32 reg_chb_fcw_config_1;                   // (0x74)
     uint32 reg_chb_fcw_config_2;                   // (0x78)
     uint32 reg_chb_digmod_config;                  // (0x7c)

     uint32 reg_fcw_config_1;                       // (0x80)
#define   BCM6818_FCW_REG                         0xffffffff    //32 bit frequency control word from NTP block

     uint32 reg_fcw_config_2;                       // (0x84)
#define   BCM6818_LOAD_NTP                        0x00000001    // triggers pulse to load FCW to NTP
#define   BCM6818_LOAD_NTP_SHIFT                  0

#define   BCM6818_LOAD_MISC_A                     0x00000002    // triggers pulse to load FCW to ch A
#define   BCM6818_LOAD_MISC_A_SHIFT               1

#define   BCM6818_LOAD_MISC_B                     0x00000004    // triggers pulse to load FCW to ch B
#define   BCM6818_LOAD_MISC_B_SHIFT               2

#define   BCM6818_LOAD_FCW_NOM_A                  0x00000008    //strobe to load nominal FCW value
#define   BCM6818_LOAD_FCW_NOM_A_SHIFT            3

#define   BCM6818_LOAD_FCW_NOM_B                  0x00000010    //strobe to load nominal FCW value
#define   BCM6818_LOAD_FCW_NOM_B_SHIFT            4

#define   BCM6818_FCW_LIMIT                       0x00000f00    //4 bits defines allowable error on FCW relative to nominal
#define   BCM6818_FCW_LIMIT_SHIFT                 8

     uint32 reg_ntp_config_1;                       // (0x88)
#define   BCM6818_NTP_TIME_DVD                    0x0000ffff    //16 bit divide value for time counter (16'd374 is default)

     uint32 reg_ntp_config_2;                       // (0x8c)
#define   BCM6818_NTP_READ_TIME                   0x00000001    //strobe asks for new time count value

     uint32 reg_ring_config_1;                      // (0x90)
#define   BCM6818_RING_OFFSET                     0x0000ffff    //
#define   BCM6818_RING_OFFSET_SHIFT               0

#define   BCM6818_RING_SCALE                      0xffff0000    //
#define   BCM6818_RING_SCALE_SHIFT                16

     uint32 reg_ring_config_2;                      // (0x94)
#define   BCM6818_RING_OMEGA_NORM_A               0x000000ff    //the omega man is Charlton Heston on channel A
#define   BCM6818_RING_OMEGA_NORM_A_SHIFT         0

#define   BCM6818_RING_OMEGA_NORM_B               0x0000ff00    //the omega man is Charlton Heston on channel B
#define   BCM6818_RING_OMEGA_NORM_B_SHIFT         8

#define   BCM6818_RING_CREST_FACTOR               0x00070000    //3 bit crest factor
#define   BCM6818_RING_CREST_FACTOR_SHIFT         16

#define   BCM6818_RING_GEN_ENABLE_A               0x00100000    //1=enabled
#define   BCM6818_RING_GEN_ENABLE_A_SHIFT         20

#define   BCM6818_RING_GEN_ENABLE_B               0x01000000    //1=enabled
#define   BCM6818_RING_GEN_ENABLE_B_SHIFT         24

  uint32        reg_ring_config_3;                      // (0x98)
#define   BCM6818_RING_BREAK_PHASE_A              0x000000ff    //break phase
#define   BCM6818_RING_BREAK_PHASE_A_SHIFT        0

#define   BCM6818_RING_BREAK_PHASE_B              0x0000ff00    //break phase
#define   BCM6818_RING_BREAK_PHASE_B_SHIFT        8

#define   BCM6818_RING_BREAK_PHASE_SIGN_A         0x10000000    //1=break phase sign
#define   BCM6818_RING_BREAK_PHASE_SIGN_A_SHIFT   28

#define   BCM6818_RING_BREAK_PHASE_SIGN_B         0x20000000    //1=break phase sign
#define   BCM6818_RING_BREAK_PHASE_SIGN_B_SHIFT   29

#define   BCM6818_RING_START_CADENCE_A            0x40000000    //1=start cadence
#define   BCM6818_RING_START_CADENCE_A_SHIFT      30

#define   BCM6818_RING_START_CADENCE_B            0x80000000    //1=start cadence
#define   BCM6818_RING_START_CADENCE_B_SHIFT      31

     uint32 reg_ring_config_4;                      // (0x9c)
#define   BCM6818_RING_STOP_IMMED_A               0x00000001    // stop ring now!
#define   BCM6818_RING_STOP_IMMED_A_SHIFT         0

#define   BCM6818_RING_STOP_IMMED_B               0x00000002    // stop ring now!
#define   BCM6818_RING_STOP_IMMED_B_SHIFT         1

#define   BCM6818_RING_STOP_NEXT_BREAK_A          0x00000004    // stop ring at next break
#define   BCM6818_RING_STOP_NEXT_BREAK_A_SHIFT    2

#define   BCM6818_RING_STOP_NEXT_BREAK_B          0x00000008    // stop ring at next break
#define   BCM6818_RING_STOP_NEXT_BREAK_B_SHIFT    3

     uint32 reg_ser_config;                         // (0xa0)
#define   BCM6818_SER_MUX_SEL                     0x00000003
#define   BCM6818_SER_MUX_SEL_SHIFT               0

//#define   BCM6818_SER_TST_SHFT_CTRL               0x07800000    //
#define   BCM6818_AUD_SER_TST_FCW_MUX_CTRL        0x08000000    //
#define   BCM6818_AUD_SER_TST_FCW_MUX_CTRL_SHIFT  27

//#define   BCM6818_SER_TST_LOAD_REGFILE            0x20000000    //1->use channel b requests for transfer 0-> use channel a
#define   BCM6818_SER_TST_CLKS                    0x40000000    //1=use clocks supplied by SER 0=generate internal clocks
#define   BCM6818_SER_TST_CLKS_SHIFT              30

#define   BCM6818_SOFT_INIT_AP                    0x80000000
#define   BCM6818_SOFT_INIT_AP_SHIFT              31

     uint32 reg_spare;                              // (0xa4)
//#define   BCM6818_SER_TST_SCALE                   0xffffffff

     uint32 reg_stats_config;                            // (0xa8)
#define   BCM6818_START_STATS                     0x00008000    //[15]
#define   BCM6818_STATS_MUX_CTL_VADCA             0x00000800    //[11]
#define   BCM6818_STATS_MUX_CTL_VADCB             0x00000400    //[10]
#define   BCM6818_STATS_CHOP_ENABLE_A             0x00000200    //[9]
#define   BCM6818_STATS_CHOP_ENABLE_B             0x00000100    //[8]
#define   BCM6818_STATS_DURATION                  0x000000f0    //[7:4]
#define   BCM6818_STATS_CHOP_PERIOD               0x0000000f    //[3:0]

  // APM Status Registers
     uint32 reg_apm_status_1;                       // (0xac)
#define   BCM6818_DIGMOD_SAT_QUANT                0x01000000    //strobe indicating quantizer has saturated
#define   BCM6818_DIGMOD_SAT_COUNT_0              0x00ff0000    //8 bit counter of saturation on integrator 0 channel a
#define   BCM6818_DIGMOD_SAT_COUNT_1              0x0000ff00    //8 bit counter of saturation on integrator 1 channel a
#define   BCM6818_DIGMOD_SAT_COUNT_QUANT          0x000000ff    //8 bit counter of saturation on quantizer channel a

     uint32 reg_apm_status_2;                       // (0xb0) same as status 1 for ch B

     uint32 reg_apm_status_3;                       // (0xb4)
#define   BCM6818_AP_XMT_UFLOW                    0x00200000    //level indicating xnt fifo is empty when read attempted
#define   BCM6818_AP_XMT_OVLOW                    0x00100000    //level indicating xmt fifo is full when write attempted
#define   BCM6818_AP_NCO_SATURATED                0x00080000    // NCO is saturated
#define   BCM6818_AP_XMT_FIFO_DEPTH               0x00070000    //3 bit depth number on xmt fifo
#define   BCM6818_AP_RCV_UFLOW                    0x00000020    //level indicating rcv fifo is empty when read attempted
#define   BCM6818_AP_RCV_OVLOW                    0x00000010    //level indicating rcv fifo is full when write attempted
#define   BCM6818_AP_RCV_FIFO_DEPTH               0x00000007    //3 bit depth number on receive fifo

     uint32 reg_apm_status_4;                       // (0xb8)
#define   BCM6818_EQ_RX_COEFF                     0xffff0000    //16 bit 2's complement coefficients from RAM
#define   BCM6818_EQ_TX_COEFF                     0x0000ffff    //16 bit coefficient read from RAM or ROM

     uint32 reg_apm_status_5;                       // (0xbc)
#define   BCM6818_ASRC_INT_COEFF                  0xffff0000    //16 bit 2's comp current coefficient value
#define   BCM6818_ASRC_DEC_COEFF                  0x0000ffff    //16 bit coefficient from RAM or ROM

     uint32 reg_apm_status_6;                       // (0xc0)
#define   BCM6818_YFLTR_COEFF                     0x000fffff    //20 bit 2's complement coefficients from RAM

// Status 9-7 same as 6-4 for Ch B
     uint32 reg_apm_status_7;                       // (0xc4)
     uint32 reg_apm_status_8;                       // (0xc8)
     uint32 reg_apm_status_9;                       // (0xcc)

     uint32 reg_apm_status_10;                      // (0xd0)
#define   BCM6818_NTP_TIME_COUNT_INTEGER          0x0001ffff    //17 integer bits (in seconds)

     uint32 reg_apm_status_11;                      // (0xd4)
#define   BCM6818_NTP_TIME_COUNT_FRACT            0x0003ffff    //18 fractional bits (in seconds)

     uint32 reg_apm_status_12;                      // (0xd8)
#define   BCM6818_RING_CADENCE_STOPPED_A          0x00000008    //
#define   BCM6818_RING_ON_OFF_A                   0x00000004    //
#define   BCM6818_RING_CADENCE_STOPPED_B          0x00000002    //
#define   BCM6818_RING_ON_OFF_B                   0x00000001    //

     uint32 reg_apm_status_13;                      // (0xdc)
#define   BCM6818_HISPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM6818_HISPD_HYBAL_COEFF_B             0x0000ffff    //

     uint32 reg_apm_status_14;                      // (0xe0)
#define   BCM6818_LOSPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM6818_LOSPD_HYBAL_COEFF_B             0x0000ffff    //

     uint32 reg_apm_stats_1;                        // (0xe4)
#define   BCM6818_MEAN                            0xffffffff    // 16 bit mean stats

     uint32 reg_apm_stats_2;                        // (0xe8), same as reg_apm_stats_1 for channel B
     uint32 reg_apm_stats_3;                        // (0xec)
#define   BCM6818_MEAN_SQUARE_LO                  0xffffffff    // lower 32 bits of 40 bit mean square value

     uint32 reg_apm_stats_4;                        // (0xf0)
#define   BCM6818_MEAN_SAT                        0x00040000
#define   BCM6818_MEAN_SQUARE_SAT                 0x00020000
#define   BCM6818_STATS_BUSY                      0x00010000    // 1 bit stats busy
#define   BCM6818_MEAN_SQUARE_HI                  0x00003fff    // upper 14 bits of 46 bit mean square value

     uint32 reg_apm_stats_5;                        // (0xf4), same as reg_apm_stats_3 for channel B
     uint32 reg_apm_stats_6;                        // (0xf8), same as reg_apm_stats_4 for channel B

     uint32 reg_apm_audio_fcw_a;                    // (0xfc)
     uint32 reg_apm_audio_fcw_b;                    // (0x100)

     uint32 reg_apm_irq_pend_1;                     // (0x104) Ch A status
#define   BCM6818_PM_BURST_DONE                   0x80000000    //strobe indicating that PM burst has completed
#define   BCM6818_ASRC_INT_DONE_SLEWING           0x40000000    //strobe indicating that slewing has completed
#define   BCM6818_ASRC_DEC_DONE_SLEWING           0x20000000    //strobe indicating that slewing has completed
#define   BCM6818_RX_PATH_SAT                     0x10000000    //strobed high when audio + pulse meter + y-filter exceeds 16 bit number
#define   BCM6818_EQ_TX_SAT                       0x08000000    //strobe indicates that the result on the eq tx path was saturated
#define   BCM6818_EQ_RX_SAT                       0x04000000    //strobe indicates that the result on the rx path was saturated at output
#define   BCM6818_EQ_TX_RQST_ERR                  0x02000000    //strobe indicating a tx_rqst was asked for while a tx was pending
#define   BCM6818_EQ_RX_RQST_ERR                  0x01000000    //strobe indicating a rx_rqst was asked for while a rx was pending
#define   BCM6818_LOSPD_HYBAL_SAT                 0x00800000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6818_HISPD_HYBAL_SAT                 0x00400000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6818_HISPD_HYBAL_RQST_ERR            0x00200000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM6818_YFLTR_IIR1_SAT                  0x00100000    //strobe indicating iir-1 is saturating
#define   BCM6818_YFLTR_FIR_SAT                   0x00080000    //strobe indicating fir is saturating
#define   BCM6818_YFLTR_IIR2_SAT                  0x00040000    //strobe indicating iir-2 is saturating
#define   BCM6818_YFLTR_IIR1_ACCUM_SAT            0x00020000    //strobe indicating iir1 accumulator saturated
#define   BCM6818_YFLTR_FIR_ACCUM_SAT             0x00010000    //strobe indicating fir accumulator saturated
#define   BCM6818_YFLTR_IIR2_ACCUM_SAT            0x00008000    //strobe indicating iir2 accumulator saturated
#define   BCM6818_YFLTR_RQST_ERR                  0x00004000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM6818_NCO_SAT                         0x00002000    //strobe indicates scaling and shifting saturated the result
#define   BCM6818_ASRC_INT_HRNR_TIM_ERR           0x00001000    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM6818_ASRC_INT_HRNR_SAT               0x00000800    //strobe indicating data has saturated at the mac output
#define   BCM6818_ASRC_INT_FIR_TIM_ERR            0x00000400    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM6818_ASRC_INT_FIR_BANK_ERR           0x00000200    //strobe indicating that horner ngn asked for a bank switch while fir was busy
#define   BCM6818_ASRC_INT_FIR_SAT                0x00000100    //strobe indicating data has saturated at the mac output
#define   BCM6818_ASRC_INT_HRNR_MAC_SAT           0x00000080    //strobe indicating ser mac in hrnr block has saturated
#define   BCM6818_CIC_INT_SAT                     0x00000040    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6818_DIGMOD_SAT_0                    0x00000020    //strobe indicating the integrator 0 has saturated
#define   BCM6818_DIGMOD_SAT_1                    0x00000010    //strobe indicating integrator 1 has saturated
#define   BCM6818_TX_PATH_SAT                     0x00000008
#define   BCM6818_CIC_DEC_SAT                     0x00000004    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6818_ASRC_DEC_TIM_ERR                0x00000002    //strobe asserted when "input_data_ready" is asserted while ngn is busy
#define   BCM6818_ASRC_DEC_SAT                    0x00000001    //strobe indicating that the input was saturated

     uint32 reg_apm_irq_mask_1;                     // (0x108)

     uint32 reg_apm_irq_pend_2;                     // (0x10c) same as APM_IRQ_1 for Ch B
     uint32 reg_apm_irq_mask_2;                     // (0x110)

     uint32 reg_apm_irq_pend_3;                     // (0x114)
#define   BCM6818_STATS_DONE_A                    0x00000400    //strobe indicating statistics gathered and computed chA
#define   BCM6818_STATS_DONE_B                    0x00000200    //strobe indicating statistics gathered and computed chB
#define   BCM6818_FCW_OOB_A                       0x00000100    //level indicating that FCW error is too large
#define   BCM6818_FCW_OOB_B                       0x00000080    //level indicating that FCW error is too large
#define   BCM6818_HPF_SAT_A                       0x00000040    //strobe indicating high pass filter has saturated, Channel A
#define   BCM6818_HPF_SAT_B                       0x00000020    //strobe indicating high pass filter has saturated, Channel B
#define   BCM6818_ASRC_INT_DELTA_ADJ_DONE_A       0x00000010    //strobe indicating TODO
#define   BCM6818_ASRC_INT_DELTA_ADJ_DONE_B       0x00000008    //strobe indicating TODO
#define   BCM6818_LOSPD_HYBAL_RQST_ERR_A          0x00000004    //strobe indicating a output_rqst was asked for while computation engine was busy, channel A
#define   BCM6818_LOSPD_HYBAL_RQST_ERR_B          0x00000002    //strobe indicating a output_rqst was asked for while computation engine was busy, channel B
#define   BCM6818_NTP_TIME_VALID                  0x00000001    //strobe in clk_200 domain (4 clock periods)

     uint32 reg_apm_irq_mask_3;                     // (0x118)
     uint32 reg_spare_2;                            // (0x11c)

     uint32 reg_codec_config_0;                     // (0x120)
#define   BCM6818_REF_CONTROL                     0x00000007    // control for bandgap voltage switching matrix
#define   BCM6818_REF_REFAMP_PD                   0x00000010    // power down for 50uA ref amp
#define   BCM6818_REF_BIASAMP_PD                  0x00000020    // power down for 10uA bias amp
#define   BCM6818_REF_BANDGAP_PD                  0x00000040    // power down for bandgap
#define   BCM6818_REF_SPARE                       0x00000f00    // reserved for future use

     uint32 reg_codec_config_1;                     // (0x124) VTX1
#define   BCM6818_VTX_PGA_MUX                     0x00000300    // analog loopback VDAC out to MICPGA in
#define   BCM6818_VTX_PG                          0x00001800    // gain setting
#define   BCM6818_VTX_CALEN                       0x00002000    // calibration enable
#define   BCM6818_VTX_OSCRST                      0x00004000    // unstable status reset
#define   BCM6818_VTX_PWD                         0x00008000    // ADC power down
#define   BCM6818_VTX_VOCM_CTL                    0x00070000    // opamp output common mode control
#define   BCM6818_VTX_FBIAS_CTL                   0x00300000    // flash bias control
#define   BCM6818_VTX_INT1BIAS_CTL                0x00c00000    // int1 bias control
#define   BCM6818_VTX_INT2BIAS_CTL                0x03000000    // int2 bias control
#define   BCM6818_VTX_VOS_ENA                     0x04000000    // ADC voltage offset enable
#define   BCM6818_VTX_SHUFF_DIS                   0x08000000    // shuffle disable
#define   BCM6818_VTX_SHUFF_DELAY_DIS             0x10000000    // shuffle feedback delay disable
#define   BCM6818_VTX_DITHER_DIS                  0x20000000    // dither disable
#define   BCM6818_VTX_DCANCEL_DIS                 0x40000000    // dither cancellation algorithm disable

#define   BCM6818_PGA_CTL_VADC                    0x30000000
#define   BCM6818_PGA_CTL_VADC_SHIFT                      28

#define   BCM6818_PGA_GAIN_CTL_VDAC_A             0x00000100
#define   BCM6818_PGA_GAIN_CTL_VDAC_A_SHIFT                8

#define   BCM6818_PGA_GAIN_CTL_VDAC_B             0x00000010
#define   BCM6818_PGA_GAIN_CTL_VDAC_B_SHIFT                4

     uint32 reg_codec_config_2;                     // (0x128) same as config 16 for VTX2

     uint32 reg_codec_config_3;                     // (0x12c) VRX1
#define   BCM6818_VRX_PG                          0x00000003    // PGA gain setting
#define   BCM6818_VRX_ASP_BYPASS                  0x00000080    // bypass testmode
#define   BCM6818_VRX_AUXPG                       0x00000300    // aux PGA gain setting
#define   BCM6818_VRX_PD                          0x00008000    // power down
#define   BCM6818_VRX_CMCTL                       0x00070000    // common mode voltage control
#define   BCM6818_VRX_PDDELAY                     0x00300000
#define   BCM6818_VRX_IDAC_CM_CTL                 0x03000000    // IDAC common mode voltage control
#define   BCM6818_VRX_ALOOPBACK                   0x0c000000    // Analog loopback test mode

     uint32 reg_codec_config_4;                     // (0x130) same as config 18 for VRX2
#define   BCM6818_VREF_CTL_VDAC                   0x70000000
#define   BCM6818_PWRDN_REF_VDAC                  0x01000000
#define   BCM6818_PWRDN_REF_VDAC_A                0x00020000
#define   BCM6818_TESTEN_VDAC_A                   0x00010000
#define   BCM6818_DRVVOCMCTL_VDAC                 0x00007000
#define   BCM6818_PGA_GAIN_CTL_VDAC_A             0x00000100
#define   BCM6818_PWRDN_REF_VDAC_B                0x00000040
#define   BCM6818_TESTEN_VDAC_B                   0x00000020
#define   BCM6818_PGA_GAIN_CTL_VDAC_B             0x00000010

     uint32 reg_codec_config_5;                     // (0x134) 32Khz Ring Gen config
#define   BCM6818_RING_PD                         0x00000001
#define   BCM6818_RING_RESETB                     0x00000002
#define   BCM6818_REF_RESETB                      0x00000004
#define   BCM6818_VTX1_RESETB                     0x00000008
#define   BCM6818_VTX2_RESETB                     0x00000010
#define   BCM6818_VRX1_RESETB                     0x00000020
#define   BCM6818_VRX2_RESETB                     0x00000040

     uint32 reg_codec_config_6;                     // (0x138) reserved
     uint32 reg_codec_config_7;                     // (0x13c) reserved
     uint32 reg_codec_config_8;                     // (0x140) reserved

     uint32 reg_apm_codec_fb_0;                     // (0x144) 32Khz Feedback
     uint32 reg_mbist_0;                            // (0x148) MBIST control register
#define   BCM6818_P16_CORE_INST_MEM_TM            0x0000000f
#define   BCM6818_P16_CORE_INST_ROM_TM            0x000001f0
#define   BCM6818_BMU_BUS_MAIN_IF_TM              0x00000600
#define   BCM6818_APM_EQ_DATA_RAM_TM_RX_A         0x00001800
#define   BCM6818_APM_EQ_DATA_RAM_TM_RX_B         0x00006000
#define   BCM6818_APM_EQ_DATA_RAM_TM_TX_A         0x00018000
#define   BCM6818_APM_EQ_DATA_RAM_TM_TX_B         0x00060000
#define   BCM6818_APM_EQ_COEFF_RAM_TM_RX_A        0x00180000
#define   BCM6818_APM_EQ_COEFF_RAM_TM_RX_B        0x00600000
#define   BCM6818_APM_EQ_COEFF_RAM_TM_TX_A        0x01800000
#define   BCM6818_APM_EQ_COEFF_RAM_TM_TX_B        0x06000000
#define   BCM6818_APM_Y_FLTR_TM_A                 0x18000000
#define   BCM6818_APM_Y_FLTR_TM_B                 0x60000000
     uint32 reg_mbist_1;                            // (0x14c) MBIST control register
#define   BCM6818_ASRC_DEC_COEFF_RAM_TM_A         0x00000003
#define   BCM6818_ASRC_DEC_DATA_RAM_TM_A          0x0000000c
#define   BCM6818_ASRC_DEC_COEFF_RAM_TM_B         0x00000030
#define   BCM6818_ASRC_DEC_DATA_RAM_TM_B          0x000000c0
#define   BCM6818_ASRC_INTERPFIR_COEFF_RAM_TM_A   0x00000300
#define   BCM6818_ASRC_INTERPFIR_DATA_RAM_TM_A    0x00000c00
#define   BCM6818_ASRC_INTERPFIR_COEFF_RAM_TM_B   0x00003000
#define   BCM6818_ASRC_INTERPFIR_DATA_RAM_TM_B    0x0000c000
     uint32 reg_otp_0;                              // (0x150) OTP read back, otp_apm[31:0]
#define   BCM6818_APM_OTP_TRIM                    0x00000FC0
#define   BCM6818_APM_OTP_TRIM_SHIFT                       6
     uint32 reg_otp_1;                              // (0x154) OTP read back, otp_apm[63:32]
     uint32 reg_diag_readback;                      // (0x158) diag bus read back

     uint32 audio_spare2;                           // (0x15c) Audio Spare Register 2
     uint32 dpll_control;                           // (0x160) DPLL control register
#define   BCM6818_DPLL_SOFT_INIT                  0x80000000
#define   BCM6818_DPLL_FORCE_ACQ                  0x40000000
#define   BCM6818_DPLL_OPEN_LOOP                  0x20000000
#define   BCM6818_DPLL_CONST_K                    0x10000000
#define   BCM6818_DPLL_PHS_THSHLD_SHIFT           4
     uint32 dpll_nom_freq;                          // (0x164) DPLL nominal frequency (control)
     uint32 dpll_div;                               // (0x168) DPLL divide register
#define   BCM6818_DPLL_REF_DIV_SHIFT              16
     uint32 dpll_acq_freq;                          // (0x16c) DPLL acquired frequency
     uint32 dpll_status;                            // (0x170) DPLL status register
#define   BCM6818_DPLL_IN_SYNC                    0x80000000
#define   BCM6818_DPLL_ACQ_FREQ_VALID             0x40000000
#define   BCM6818_DPLL_IN_SYNC                    0x80000000
#define   BCM6818_DPLL_K0_SHIFT                   8

} BCM6818ApmControlRegisters;

#define BCM6818_APM ((volatile BCM6818ApmControlRegisters * const) BCM6818_APM_BASE)

#ifdef __cplusplus
}
#endif

#endif /* __MAP6818_H */

