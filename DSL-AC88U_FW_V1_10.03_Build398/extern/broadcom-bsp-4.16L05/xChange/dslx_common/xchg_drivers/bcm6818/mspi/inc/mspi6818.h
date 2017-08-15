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
#ifndef MSPI6818_H    /* support nested includes */
#define MSPI6818_H

/* ---- Include Files ---------------------------------------- */

#include <bosCfg.h>
#include <xdrvIntCtrl.h>
#include <xdrvTypes.h>

#define EXPORT_SYMTAB 1

/* ---- Constants and Types ---------------------------------- */

/*************************************************************************
**                         MSPI6818 COMMANDS
*************************************************************************/

typedef enum
{
   BCM_MSPI6818_INITREGS_CMD,
   BCM_MSPI6818_SETREGS_CMD,
   BCM_MSPI6818_GETREGS_CMD,
   BCM_MSPI6818_RXCALLBACK_INSTALL_CMD,
   BCM_MSPI6818_TXCALLBACK_INSTALL_CMD

} BCM_MSPI6818_CMD;

/*************************************************************************
**                   MSPI6818 REGISTERS AND DEFINES                     **
*************************************************************************/

/* By default we run MSPI in polling mode, the reason for this is that there is no *
 * significant advantage to running as a SPI Master in interrupt mode since all    *
 * communication is initiated by us, including reads ( all reads require a write ) */
#define MSPI6818_USE_INTERRUPTS 0

typedef enum
{
   BCM_MSPI6818_CTRL_MODE_POLLING,
   BCM_MSPI6818_CTRL_MODE_INTERRUPT

} BCM_MSPI6818_CTRL_MODE;

typedef struct BCM_MSPI6818_REGS
{
   XDRV_UINT32                  spiClkDiv;       /* serialClk Divider setting - default is (0.781 MHz) */
   XDRV_UINT32                  ssOffTime;       /* minimum time between subsequent transfers (in SCK clock cycles) */
   XDRV_UINT32                  byteSwap;        /* sets SPI to transfer using little endian */
   XDRV_UINT32                  fillByte;        /* data to be transferred during half duplex reads */
   XDRV_UINT32                  commandWord;     /* default command structure setup */
   BCM_MSPI6818_CTRL_MODE       mspiCtrlMode;

} BCM_MSPI6818_REGS;

/* ---- Variable Externs ------------------------------------- */
/* ---- Function Prototypes ---------------------------------- */

extern int mspiInit( void );
extern int mspiDeinit( void );
extern int mspiOpen( void );
extern int mspiClose( void );
extern int mspiIOctl( BCM_MSPI6818_CMD command, void *cmdp );
extern int mspiWrite( XDRV_UINT8 *pTxBuf, int chan, unsigned length, unsigned *pNumTxfr );
extern int mspiReadWrite( XDRV_UINT8 *pTxBuf, XDRV_UINT8 *pRxBuf, int chan, unsigned length, unsigned *pNumTxfr );
extern int mspiRead( volatile XDRV_UINT8 *pRxBuf, unsigned length );

#if BOS_OS_LINUXKERNEL

/* 
** Export MSPI symbols for possible use by other Linux kernel modules.
** TODO: Move this OS-dependent code outside of xchg_drivers
*/

#include <kernelMainWrapper.h>
#include <linux/module.h>
#include <linux/kernel.h>

EXPORT_SYMBOL(mspiInit);
EXPORT_SYMBOL(mspiDeinit);
EXPORT_SYMBOL(mspiOpen);
EXPORT_SYMBOL(mspiClose);
EXPORT_SYMBOL(mspiIOctl);
EXPORT_SYMBOL(mspiWrite);
EXPORT_SYMBOL(mspiReadWrite);
EXPORT_SYMBOL(mspiRead);

#endif /* BOS_OS_LINUXKERNEL */

#endif   /* MSPI6818_H */




