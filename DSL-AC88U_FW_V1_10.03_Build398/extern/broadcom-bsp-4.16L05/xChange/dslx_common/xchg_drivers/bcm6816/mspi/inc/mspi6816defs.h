/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Description:
*
****************************************************************************/
#ifndef MSPI6816DEFS_H 
#define MSPI6816DEFS_H

/* ---- Include Files ---------------------------------------- */

#include <bosSem.h>
#include <xdrvIntCtrl.h>
#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <bcm6816.h>

/* ---- Constants and Types ---------------------------------- */



#define MSPI_SUCCESS                           0
#define MSPI_ERROR                             1

#define INTERRUPT_ID_SPI_IRQ                 101

#define MSPI6816_MAX_RX_SIZE                1088
                                   
#define BCM6816_MSPI_TX_QUEUE_SIZE           544
#define BCM6816_MSPI_RX_QUEUE_SIZE           544

#define MMR6816_MSPI_INTSTATUS_CMNDDONE     0x01
#define MMR6816_MSPI_INTSTATUS_RXOVERFL     0x02
#define MMR6816_MSPI_INTSTATUS_TXUNDRFL     0x04
#define MMR6816_MSPI_INTSTATUS_TXOVERFL     0x08
#define MMR6816_MSPI_INTSTATUS_RXUNDRFl     0x10
#define MMR6816_MSPI_INTSTATUS_MASK         0x1F

#define MMR6816_MSPI_START_NOOP             0x00
#define MMR6816_MSPI_START_SOFT_RESET       0x01
#define MMR6816_MSPI_START_HARD_RESET       0x02
#define MMR6816_MSPI_START_IMMEDIATE        0x03
#define MMR6816_MSPI_START_NONBUF           0x0F

#define MMR6816_MSPI_STATUS_RXEMPTY         0x02
#define MMR6816_MSPI_STATUS_CMDBUSY         0x04
#define MMR6816_MSPI_STATUS_SERBUSY         0x08



typedef struct SpiControl1 {
  uint16        spiMsgCtl;              /* (0x0) control byte */
#define FULL_DUPLEX_RW                  0
#define HALF_DUPLEX_W                   1
#define HALF_DUPLEX_R                   2
#define SPI_MSG_TYPE_SHIFT              14
#define SPI_BYTE_CNT_SHIFT              0
  byte          spiMsgData[0x21e];      /* (0x02 - 0x21f) msg data */
  byte          unused0[0x1e0];
  byte          spiRxDataFifo[0x220];   /* (0x400 - 0x61f) rx data */
  byte          unused1[0xe0];

  uint16        spiCmd;                 /* (0x700): SPI command */
#define SPI_CMD_NOOP                    0
#define SPI_CMD_SOFT_RESET              1
#define SPI_CMD_HARD_RESET              2
#define SPI_CMD_START_IMMEDIATE         3

#define SPI_CMD_COMMAND_SHIFT           0
#define SPI_CMD_COMMAND_MASK            0x000f

#define SPI_CMD_DEVICE_ID_SHIFT         4
#define SPI_CMD_PREPEND_BYTE_CNT_SHIFT  8
#define SPI_CMD_ONE_BYTE_SHIFT          11
#define SPI_CMD_ONE_WIRE_SHIFT          12
#define SPI_DEV_ID_0                    0
#define SPI_DEV_ID_1                    1
#define SPI_DEV_ID_2                    2
#define SPI_DEV_ID_3                    3

  byte          spiIntStatus;           /* (0x702): SPI interrupt status */
  byte          spiMaskIntStatus;       /* (0x703): SPI masked interrupt status */

  byte          spiIntMask;             /* (0x704): SPI interrupt mask */
#define SPI_INTR_CMD_DONE               0x01
#define SPI_INTR_RX_OVERFLOW            0x02
#define SPI_INTR_INTR_TX_UNDERFLOW      0x04
#define SPI_INTR_TX_OVERFLOW            0x08
#define SPI_INTR_RX_UNDERFLOW           0x10
#define SPI_INTR_CLEAR_ALL              0x1f

  byte          spiStatus;              /* (0x705): SPI status */
#define SPI_RX_EMPTY                    0x02
#define SPI_CMD_BUSY                    0x04
#define SPI_SERIAL_BUSY                 0x08

  byte          spiClkCfg;              /* (0x706): SPI clock configuration */
#define SPI_CLK_0_391MHZ                1
#define SPI_CLK_0_781MHZ                2 /* default */
#define SPI_CLK_1_563MHZ                3
#define SPI_CLK_3_125MHZ                4
#define SPI_CLK_6_250MHZ                5
#define SPI_CLK_12_50MHZ                6
#define SPI_CLK_MASK                    0x07
#define SPI_SSOFFTIME_MASK              0x38
#define SPI_SSOFFTIME_SHIFT             3
#define SPI_BYTE_SWAP                   0x80

  byte          spiFillByte;            /* (0x707): SPI fill byte */
  byte          unused2; 
  byte          spiMsgTail;             /* (0x709): msgtail */
  byte          unused3; 
  byte          spiRxTail;              /* (0x70B): rxtail */
} SpiControl1;

typedef struct
{
   volatile SpiControl *p6816SpiRegs;

   int (*rxCallbackFnc)(XDRV_UINT8);
   int (*txCallbackFnc)(XDRV_UINT8);

   XDRV_UINT8  *pTxBuf;
   unsigned     txOffset;

   XDRV_UINT8   rxBuf[ MSPI6816_MAX_RX_SIZE ];
   unsigned     rxOffset;

   int          bytesRemaining;
   int          bytesQueued;

} BCM_MSPI6816_DRV;

#if MSPI6816_USE_INTERRUPTS
typedef void (*MSPI6816_ISR)(void *contextP);
#endif /* MSPI6816_USE_INTERRUPTS */

/* ---- Variable Externs ------------------------------------- */

extern BCM_MSPI6816_DRV  mspi6816Drv;
extern BCM_MSPI6816_REGS mspi6816Registers;

#if MSPI6816_USE_INTERRUPTS
extern BOS_SEM           mspi6816IsrSem;
#endif /* MSPI6816_USE_INTERRUPTS */

/* ---- Function Prototypes ---------------------------------- */

#if MSPI6816_USE_INTERRUPTS
unsigned int mspi6816Isr( XDRV_INT_CTRL_ISR_PARM contextp );
#endif /* MSPI6816_USE_INTERRUPTS */

void mspi6816intc_IrqDisable( void );
void mspi6816intc_IrqEnable( void );
void mspi6816intc_intVectIrqIsrInstall( XDRV_INT_CTRL_ISR isr );

#endif   /* MSPI6816DEFS_H */
