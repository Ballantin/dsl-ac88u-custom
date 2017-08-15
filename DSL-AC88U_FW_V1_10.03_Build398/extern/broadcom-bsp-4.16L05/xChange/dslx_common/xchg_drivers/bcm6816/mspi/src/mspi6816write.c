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
*    Filename: mspiWrite.c
*
****************************************************************************
*
*    Description:
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include <bosSleep.h>
#include <mspi6816.h>
#include <mspi6816defs.h>



/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   mspiWrite
**
** PURPOSE:    Write to the MSPI driver
**
** PARAMETERS: pTxBuf   - pointer to the location to write from
**             chan     - SPI channel to write to
**             length   - length of buffer to store the receive data (if any)
**             pNumTxfr - # of bytes transferred (poll mode)
**                        0                      (interrupt mode)
**
** RETURNS:    MSPI_SUCCESS / MSPI_ERROR
**
*****************************************************************************
*/
int mspiWrite( XDRV_UINT8 *pTxBuf,
                   int chan,
                   unsigned length,
                   unsigned *pNumTxfr )
{
   BCM_MSPI6816_DRV     *pMspiDrv;
   volatile SpiControl  *pMspiRegs;
   unsigned             indx;
   signed               bytesRemaining;
   unsigned             bytesWritten = 0;
   unsigned             numBytesToXfer;

   pMspiDrv  = &mspi6816Drv;
   pMspiRegs = pMspiDrv->p6816SpiRegs;

#if MSPI6816_USE_INTERRUPTS
   if( length > MSPI6816_MAX_RX_SIZE )
   {
      if( pMspiRegs->spiIntMask != 0x00 )
      {
         /* Length must be less than MAX_RX_SIZE in interrupt mode */
         XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: invalid length"));
         return ( MSPI_ERROR );
      }
   }

   /* Ensure that the previous TX transfer is done */
   if( pMspiRegs->spiIntMask != 0x00 )
   {
      if( bosSemTimedTake( &mspi6816IsrSem, 100 ))
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Failed to take semaphore"));
         return ( MSPI_ERROR );
      }
   }
#endif /* MSPI6816_USE_INTERRUPTS */

   /*
   ** Write up to MSPI_NUM_QUEUE_SIZE elements per enable cycle
   */
   bytesRemaining = length;

   while( bytesRemaining > 0 )
   {
      /*
      ** The MSPI can only transmit at most MSPI_NUM_QUEUE_SIZE at a time
      */
      if( bytesRemaining >= ( BCM6816_MSPI_TX_QUEUE_SIZE - 2 ))
      {
         numBytesToXfer = ( BCM6816_MSPI_TX_QUEUE_SIZE - 2 );
      }
      else
      {
         numBytesToXfer = bytesRemaining;
      }

      /*
      ** Write control bytes into tx queue
      */
      pMspiRegs->spiMsgCtl = numBytesToXfer;

      /*
      ** Write tx data into tx queue
      */
      for( indx = 0; indx < numBytesToXfer; indx++ )
      {
         pMspiRegs->spiMsgData[indx] = pTxBuf[indx + bytesWritten];
      }

#if MSPI6816_USE_INTERRUPTS
     /* If interrupt mode (not polling mode)
      ** Then fill in driver variables and exit.  The ISR will take care of
      **      the rest of the transfer
      ** Else wait for MSPI to complete write.
      */
      if( pMspiRegs->spiIntMask != 0x00 )
      {
         pMspiDrv->bytesRemaining = length;
         pMspiDrv->bytesQueued = numBytesToXfer;
         pMspiDrv->pTxBuf = pTxBuf;
         pMspiDrv->txOffset = numBytesToXfer;
         pMspiDrv->rxOffset = 0;

         /*
         ** In interrupt mode indicate 0 have been transferred as of yet.
         */
         *pNumTxfr = 0;

         /* Clear MSPI finished flags and begin transmission */
         pMspiRegs->spiIntStatus |= MMR6816_MSPI_INTSTATUS_MASK;
         pMspiRegs->spiCmd = SPI_CMD_START_IMMEDIATE << SPI_CMD_COMMAND_SHIFT | 
                            chan << SPI_CMD_DEVICE_ID_SHIFT | 
                            0 << SPI_CMD_PREPEND_BYTE_CNT_SHIFT |
#ifdef SPI_CMD_ONE_BYTE                             
                            1 << SPI_CMD_ONE_BYTE_SHIFT; /* For Si3226, Le88276 */
#else                                                       
                            0 << SPI_CMD_ONE_BYTE_SHIFT; /* For Si3239, Le88276 */
#endif
         return( MSPI_SUCCESS );
      }
      else
#endif /* MSPI6816_USE_INTERRUPTS */
      {
        /* Clear MSPI finished flags and begin transmission */
         pMspiRegs->spiIntStatus |= MMR6816_MSPI_INTSTATUS_MASK;
         pMspiRegs->spiCmd = SPI_CMD_START_IMMEDIATE << SPI_CMD_COMMAND_SHIFT | 
                            chan << SPI_CMD_DEVICE_ID_SHIFT | 
                            0 << SPI_CMD_PREPEND_BYTE_CNT_SHIFT |
#ifdef SPI_CMD_ONE_BYTE                             
                            1 << SPI_CMD_ONE_BYTE_SHIFT; /* For Si3226, Le88276 */
#else                                                       
                            0 << SPI_CMD_ONE_BYTE_SHIFT; /* For Si3239, Le88276 */
#endif                           

         while(!(pMspiRegs->spiIntStatus & MMR6816_MSPI_INTSTATUS_CMNDDONE));
         /*
         ** Read out rx data
         */
         for( indx=0; indx < numBytesToXfer; indx++ )
         {
            pMspiDrv->rxBuf[indx + bytesWritten] = pMspiRegs->spiRxDataFifo[indx];
         }

         /*
         ** Update bytes written/remaining
         */
         bytesWritten   += numBytesToXfer;
         bytesRemaining -= numBytesToXfer;
      }
   } /* bytesRemaining */

   *pNumTxfr = bytesWritten;

   return( MSPI_SUCCESS );
}


