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
*    Description:
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include <mspi6816.h>
#include <mspi6816defs.h>
#include <bosSem.h>

/* ---- Public Variables ------------------------------------- */

#if MSPI6816_USE_INTERRUPTS
BOS_SEM           mspi6816IsrSem;   /* MSPI ISR semaphore */
#endif /* MSPI6816_USE_INTERRUPTS */

BCM_MSPI6816_DRV  mspi6816Drv;      /* MSPI Driver */

/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */

extern XDRV_BOOL SpiClockControl(XDRV_BOOL setFlag, XDRV_BOOL enableFlag);

/*
*****************************************************************************
** FUNCTION:   mspiInit
**
** PURPOSE:    Initialize the MSPI module
**
** PARAMETERS: none
**
** RETURNS:    MSPI_SUCCESS
**
*****************************************************************************
*/
int mspiInit( void )
{
#if MSPI6816_USE_INTERRUPTS
   /* Create semaphore for MSPI ISR's */
   bosSemCreate( "MSPI6816ISR", 0, 1, &mspi6816IsrSem ); 
   
   /* Give semaphore for the very first TX transfer */
   if( bosSemGive( &mspi6816IsrSem ))
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Failed to give semaphore"));
      return ( MSPI_ERROR );
   }
   
   if( bosSemTimedTake( &mspi6816IsrSem, 100 ))
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Failed to take semaphore"));
      return ( MSPI_ERROR );
   }   
   
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "SUCCESS: Took semaphore"));
   
   if( bosSemGive( &mspi6816IsrSem ))
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Failed to give semaphore"));
      return ( MSPI_ERROR );
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "SUCCESS: Gave semaphore"));
#endif /* MSPI6816_USE_INTERRUPTS */
   
#if 0  /** NOTE: SPI enable should be fixed for 6816 */
   /* Enable SPI Block */
   SpiClockControl( 1, 1 );
#endif    

   return( MSPI_SUCCESS );
}

