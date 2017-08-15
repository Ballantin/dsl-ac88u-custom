/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
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
*    Filename: mspi6362open.c
*
****************************************************************************
*
*    Description:
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include <string.h>
#include <bcmChip.h>
#include <bosSleep.h>
#include <mspi.h>
#include <mspi_Defs.h>
#include <xdrvIntCtrl.h>

/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   mspiOpen
**
** PURPOSE:    Open the MSPI driver
**
** PARAMETERS: None
**
** RETURNS:    MSPI_SUCCESS / MSPI_ERROR
**
*****************************************************************************
*/
int mspiOpen( void )
{
   BCM_MSPI_DRV *pMspiDrv;
   int result;

   /* Setup Driver Structure */
   pMspiDrv = &mspiDrv;

   pMspiDrv->pSpiRegs = SPI;

   pMspiDrv->rxCallbackFnc = NULL;
   pMspiDrv->txCallbackFnc = NULL;

   /* Disable MSPI interrupts */
   mspi6362intc_IrqDisable();
 
#if MSPI_USE_INTERRUPTS
  /* Register an interrupt handler */
   mspi6362intc_intVectIrqIsrInstall( mspiIsr );
#endif /* MSPI_USE_INTERRUPTS */

   /*
   ** Use IOctl to set registers to default values
   */
   result = mspiIOctl( BCM_MSPI_INITREGS_CMD, NULL );

   if ( result != MSPI_SUCCESS )
   {
      return result;
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "INFO: IRQ MASK 0x%016llX", (unsigned long long)(PERF->IrqControl[0].IrqMask)));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "INFO: IRQ STATUS 0x%016llX", (unsigned long long)(PERF->IrqControl[0].IrqStatus)));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "INFO: BLOCK ENABLES 0x%x", (unsigned int)(PERF->blkEnables)));
   
   return( MSPI_SUCCESS );
}

