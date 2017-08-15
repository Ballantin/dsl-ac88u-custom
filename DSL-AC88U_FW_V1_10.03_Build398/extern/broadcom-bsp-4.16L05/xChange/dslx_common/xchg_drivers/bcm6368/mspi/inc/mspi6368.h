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
#ifndef MSPI6368_H    /* support nested includes */
#define MSPI6368_H

/* ---- Include Files ---------------------------------------- */
#include <xdrvIntCtrl.h>
#include <xdrvTypes.h>

#define EXPORT_SYMTAB 1
#include <kernelMainWrapper.h>
#include <linux/module.h>
/* ---- Constants and Types ---------------------------------- */

/*************************************************************************
**                         MSPI6368 COMMANDS
*************************************************************************/

typedef enum
{
   BCM_MSPI6368_INITREGS_CMD,
   BCM_MSPI6368_SETREGS_CMD,
   BCM_MSPI6368_GETREGS_CMD,
   BCM_MSPI6368_RXCALLBACK_INSTALL_CMD,
   BCM_MSPI6368_TXCALLBACK_INSTALL_CMD

} BCM_MSPI6368_CMD;

/*************************************************************************
**                   MSPI6368 REGISTERS AND DEFINES                     **
*************************************************************************/

/* By default we run MSPI in polling mode, the reason for this is that there is no *
 * significant advantage to running as a SPI Master in interrupt mode since all    *
 * communication is initiated by us, including reads ( all reads require a write ) */
#define MSPI6368_USE_INTERRUPTS 0

typedef enum
{
   BCM_MSPI6368_CTRL_MODE_POLLING,
   BCM_MSPI6368_CTRL_MODE_INTERRUPT

} BCM_MSPI6368_CTRL_MODE;

typedef struct BCM_MSPI6368_REGS
{
   XDRV_UINT32                  spiClkDiv;       /* serialClk Divider setting - default is (0.781 MHz) */
   XDRV_UINT32                  ssOffTime;       /* minimum time between subsequent transfers (in SCK clock cycles) */
   XDRV_UINT32                  byteSwap;        /* sets SPI to transfer using little endian */
   XDRV_UINT32                  fillByte;        /* data to be transferred during half duplex reads */
   XDRV_UINT32                  commandWord;     /* default command structure setup */
   BCM_MSPI6368_CTRL_MODE       mspiCtrlMode;

} BCM_MSPI6368_REGS;

/* ---- Variable Externs ------------------------------------- */
/* ---- Function Prototypes ---------------------------------- */

extern int mspi6368Init( void );
extern int mspi6368Deinit( void );
extern int mspi6368Open( void );
extern int mspi6368Close( void );
extern int mspi6368IOctl( BCM_MSPI6368_CMD command, void *cmdp );
extern int mspi6368Write( XDRV_UINT8 *pTxBuf, int chan, unsigned length, unsigned *pNumTxfr );
extern int mspi6368ReadWrite( XDRV_UINT8 *pTxBuf, XDRV_UINT8 *pRxBuf, int chan, unsigned length, unsigned *pNumTxfr );
extern int mspi6368Read( volatile XDRV_UINT8 *pRxBuf, unsigned length );

EXPORT_SYMBOL(mspi6368Init);
EXPORT_SYMBOL(mspi6368Deinit);
EXPORT_SYMBOL(mspi6368Open);
EXPORT_SYMBOL(mspi6368Close);
EXPORT_SYMBOL(mspi6368IOctl);
EXPORT_SYMBOL(mspi6368Write);
EXPORT_SYMBOL(mspi6368ReadWrite);
EXPORT_SYMBOL(mspi6368Read);
#endif   /* MSPI6368_H */




