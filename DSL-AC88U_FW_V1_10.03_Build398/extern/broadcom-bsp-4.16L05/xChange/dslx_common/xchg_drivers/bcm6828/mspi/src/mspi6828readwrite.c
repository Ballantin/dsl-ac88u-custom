/***************************************************************************
*
*  Copyright (c) 2001 - 2002 Broadcom Corporation, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom Corporation
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom Corporation
*  company private, proprietary, and trade secret.
*
****************************************************************************
*  
*    Filename: mspi6828readwrite.c 
*  
****************************************************************************
*
*    Description:
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */
#include <mspi6828.h>
#include <mspi6828defs.h>

/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   mspiReadWrite
**
** PURPOSE:    Read from the MSPI driver
**
** PARAMETERS: pTxBuf   - pointer to the location to write from
**             pRxBuf   - pointer to the location to receive data
**             chan     - SPI channel to control
**             length   - length of buffer to store the receive data
**             pNumTxfr - # of bytes transferred (poll mode)
**                        0   (interrupt mode) 
**
** RETURNS:    MSPI_SUCCESS / MSPI_ERROR
**
*****************************************************************************
*/
int mspiReadWrite( XDRV_UINT8 *pTxBuf,
                       XDRV_UINT8 *pRxBuf,
                       int chan,
                       unsigned length,
                       unsigned *pNumTxfr )
{
   
   BCM_MSPI6828_DRV     *pMspiDrv;
   volatile SpiControl  *pMspiRegs;

   pMspiDrv  = &mspi6828Drv;
   pMspiRegs = pMspiDrv->p6828SpiRegs;

   /* Perform write operation first */
   if( mspiWrite( pTxBuf, chan, length, pNumTxfr ))
   {
      XDRV_LOG_ERROR(( XDRV_LOG_ERROR_MOD_XDRV, "ERROR: Failed on MSPI write"));
      return ( MSPI_ERROR );
   }
   
   
   /* Perform read operation */
   if( mspiRead( pRxBuf, length ))
   {
      XDRV_LOG_ERROR(( XDRV_LOG_ERROR_MOD_XDRV, "ERROR: Failed on MSPI read"));
      return ( MSPI_ERROR );
   }
   
   return( MSPI_SUCCESS );
}

