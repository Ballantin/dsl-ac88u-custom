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
/*   MODULE:  map63381.h                                                */
/*   DATE:    05/30/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM63381 used by the DSP only                             */
/*                                                                     */
/***********************************************************************/
#ifndef __MAP_63381_H
#define __MAP_63381_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bcmtypesCommon.h>
#include "common63381.h"

/*
** Interrupt Controller
*/
#define IRQ_BITS 64
typedef struct  {
    uint64         IrqMask;
    uint64         ExtIrqMask;
    uint64         reserved[2];
    } IrqControl_t;

typedef struct BCM63381PerfControl {
     uint32        RevID;             /* (00) word 0 */
#define BCM63381_CHIP_ID_SHIFT   12
#define BCM63381_CHIP_ID_MASK    (0xfffff << BCM63381_CHIP_ID_SHIFT)
#define BCM63381_CHIP_VAR_SHIFT   8
#define BCM63381_CHIP_VAR_MASK    (0xf << BCM63381_CHIP_VAR_SHIFT)
#define BCM63381_REV_ID_MASK     0xff

    uint32        diagControl;        /* (04) word 1 */
    uint32        ExtIrqCfg;          /* (08) word 2*/
#define BCM63381_EI_CLEAR_SHFT   0
#define BCM63381_EI_SENSE_SHFT   8
#define BCM63381_EI_INSENS_SHFT  16
#define BCM63381_EI_LEVEL_SHFT   24

    uint32        ExtIrqSts;          /* (0c) word 3 */
#define BCM63381_EI_STATUS_SHFT  0
#define BCM63381_EI_MASK_SHFT    16

    uint64         IrqStatus;         /* (10) word 4 */
    uint64         ExtIrqStatus;      /* (18) word 6 */
    IrqControl_t   IrqControl[4];     /* (20) */
} BCM63381PerfControl;

#define BCM63381_PERF ((volatile BCM63381PerfControl * const) BCM63381_PERF_BASE)

/*
 * Gpio Controller
 */
typedef struct BCM63381GpioControl {
        uint32 GPIODir[5];      /* 0x00-0x10 */
        uint32 GPIOio[5];       /* 0x14-0x24 */
        uint32 SpiSlaveCfg;     /* 0x28 */
        uint32 unused;          /* 0x2c */
        uint32 TestControl;             /* 0x30 */
        uint32 TestPortBlockEnMSB;      /* 0x34 */
        uint32 TestPortBlockEnLSB;      /* 0x38 */
        uint32 TestPortBlockDataMSB;    /* 0x3c */
        uint32 TestPortBlockDataLSB;    /* 0x40 */
#define BCM63381_PINMUX_DATA_SHIFT       12
#define BCM63381_PINMUX_0                0
#define BCM63381_PINMUX_1                1
#define BCM63381_PINMUX_2                2
#define BCM63381_PINMUX_3                3
#define BCM63381_PINMUX_4                4
#define BCM63381_PINMUX_5                5
#define BCM63381_PINMUX_6                6
#define BCM63381_PINMUX_MSPI             PINMUX_3
#define BCM63381_PINMUX_MSPI_SS          PINMUX_3
#define BCM63381_PINMUX_PCM              PINMUX_1
#define BCM63381_PINMUX_GPIO             PINMUX_5
        uint32 TestPortCmd;             /* 0x44 */
#define BCM63381_LOAD_MUX_REG_CMD        0x21
        uint32 GPIOBaseMode;            /* 0x48 */
#define BCM63381_GPIO_BASE_USB_LED_OVERRIDE      (1<<11)
#define BCM63381_GPIO_BASE_VDSL_LED_OVERRIDE     (1<<10)
        uint32 DiagReadBack;            /* 0x4c */
        uint32 DiagReadBackHi;          /* 0x50 */
        uint32 GeneralPurpose;          /* 0x54 */
} BCM63381GpioControl;

#define BCM63381_GPIO ((volatile BCM63381GpioControl * const) BCM63381_GPIO_BASE)

/* Number to mask conversion macro used for GPIODir and GPIOio */
#define BCM63381_GPIO_NUM_TOTAL_BITS_MASK        0x3f
#define BCM63381_GPIO_NUM_MAX_BITS_MASK          0x1f
#define BCM63381_GPIO_NUM_TO_MASK(X)             ( (((X) & BCM63381_GPIO_NUM_TOTAL_BITS_MASK) < 32) ? (1 << ((X) & BCM63381_GPIO_NUM_MAX_BITS_MASK)) : (0) )

/* Number to mask conversion macro used for GPIODir_high and GPIOio_high */
#define BCM63381_GPIO_NUM_MAX_BITS_MASK_HIGH     0x07
#define BCM63381_GPIO_NUM_TO_MASK_HIGH(X)        ( (((X) & BCM63381_GPIO_NUM_TOTAL_BITS_MASK) >= 32) ? (1 << ((X-32) & BCM63381_GPIO_NUM_MAX_BITS_MASK_HIGH)) : (0) )

/*
** PCM Registers
*/
typedef struct BCM63381PcmControlRegisters
{
    uint32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define  BCM63381_PCM_ENABLE              0x80000000     // PCM block master enable
#define  BCM63381_PCM_ENABLE_SHIFT        31
#define  BCM63381_PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  BCM63381_PCM_SLAVE_SEL_SHIFT     30
#define  BCM63381_PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  BCM63381_PCM_CLOCK_INV_SHIFT     29
#define  BCM63381_PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  BCM63381_PCM_FS_INVERT_SHIFT     28
#define  BCM63381_PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  BCM63381_PCM_FS_FREQ_16_8_SHIFT  27
#define  BCM63381_PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  BCM63381_PCM_FS_LONG_SHIFT       26
#define  BCM63381_PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  BCM63381_PCM_FS_TRIG_SHIFT       25
#define  BCM63381_PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  BCM63381_PCM_DATA_OFF_SHIFT      24
#define  BCM63381_PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  BCM63381_PCM_DATA_16_8_SHIFT     23
#define  BCM63381_PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  BCM63381_PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define  BCM63381_PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  BCM63381_PCM_LSB_FIRST_SHIFT     18
#define  BCM63381_PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  BCM63381_PCM_LOOPBACK_SHIFT      17
#define  BCM63381_PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in 63381
#define  BCM63381_PCM_EXTCLK_SEL_SHIFT    16
#define  BCM63381_PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 63381
#define  BCM63381_PCM_NTR_ENABLE_SHIFT    15
#define  BCM63381_PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  BCM63381_PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  BCM63381_PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min
#define  BCM63381_PCM_AP_SEL              0x00000001     // 1 - Connect pcm to pads, 0 - Connects audio test interface

    uint32 pcm_chan_ctrl;                       // 04
#define  BCM63381_PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  BCM63381_PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  BCM63381_PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  BCM63381_PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  BCM63381_PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  BCM63381_PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  BCM63381_PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  BCM63381_PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  BCM63381_PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  BCM63381_PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  BCM63381_PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  BCM63381_PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  BCM63381_PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  BCM63381_PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  BCM63381_PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  BCM63381_PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  BCM63381_PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  BCM63381_PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define  BCM63381_PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  BCM63381_PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  BCM63381_PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  BCM63381_PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  BCM63381_PCM_TX_IRQ              0x00000010

    uint32        reg_pcm_clk_cntl_0;             // (0x210) PCM Clock Control 0 (NCO_FCW_MISC)
    uint32        reg_pcm_clk_cntl_1;             // (0x214) PCM Clock Control 1 (NCO_SCALE)
    uint32        reg_pcm_clk_cntl_2;             // (0x218) PCM Clock Control 2
#define   BCM63381_PCM_NCO_SHIFT          0x0000000f
#define   BCM63381_PCM_NCO_MUX_CNTL       0x00000030
#define   BCM63381_PCM_NCO_LOAD_MISC      0x00000040
#define   BCM63381_PCM_NCO_SOFT_INIT      0x00000080

    uint32        pcm_fcw_readback;            // FCW after scaling
    uint32        pcm_zds_intf;                    // ZSI Config register ( New in 63381 )
    uint32        pcm_msif_intf;                   // ISI Config register ( New in 63381 )
    uint32        pcm_ntr_counter;                 // NTR pulse counter ( New in 63381 )
    uint32        pcm_ntr_div_clk_ctrl;            // NTR feedback clock divider ( New in 63138 )

    uint32 unused[4];
#define  PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[PCM_MAX_TIMESLOT_REGS];
#define  PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} BCM63381PcmControlRegisters;

#define BCM63381_PCM ((volatile BCM63381PcmControlRegisters * const) BCM63381_PCM_BASE)

typedef struct BCM63381PcmIudmaRegisters
{
   uint32 ctrlConfig;
#define IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

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

   uint16 reserved13;
   uint16 channelReset;

   uint16 reserved14;
   uint16 channelDebug;

   uint32 reserved15;

   uint32 gbl_int_stat;
   #define IUDMA_IRQ_TX_PCM      0x00000002
   #define IUDMA_IRQ_RX_PCM      0x00000001

   uint32 gbl_int_mask;
} BCM63381PcmIudmaRegisters;

typedef struct BCM63381PcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM63381_IUDMA_CONFIG_ENDMA       0x0001
#define BCM63381_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM63381_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM63381_IUDMA_INTSTAT_BDONE   0x0001
#define BCM63381_IUDMA_INTSTAT_PDONE   0x0002
#define BCM63381_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM63381_IUDMA_INTSTAT_MASK    0x0007
#define BCM63381_IUDMA_INTSTAT_ALL     BCM63381_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM63381_IUDMA_INTMASK_BDONE   0x0001
#define BCM63381_IUDMA_INTMASK_PDONE   0x0002
#define BCM63381_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM63381_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} BCM63381PcmIudmaChannelCtrl;


typedef struct BCM63381PcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM63381_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} BCM63381PcmIudmaStateRam;

#define BCM63381_MAX_PCM_DMA_CHANNELS 2

typedef struct BCM63381PcmIudma
{
   BCM63381PcmIudmaRegisters regs;                                     //
   uint32 reserved1[110];                                      //
   BCM63381PcmIudmaChannelCtrl ctrl[BCM63381_MAX_PCM_DMA_CHANNELS];    //
   uint32 reserved2[120];                                      //
   BCM63381PcmIudmaStateRam stram[BCM63381_MAX_PCM_DMA_CHANNELS];      //

} BCM63381PcmIudma;

#define BCM63381_PCM_IUDMA ((volatile BCM63381PcmIudma * const) BCM63381_PCM_DMA_BASE)


#ifdef __cplusplus
}
#endif

#endif /* __MAP_63381_H */

