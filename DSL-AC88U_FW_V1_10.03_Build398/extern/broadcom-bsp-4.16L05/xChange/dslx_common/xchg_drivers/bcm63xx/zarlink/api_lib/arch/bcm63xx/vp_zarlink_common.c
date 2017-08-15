/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: vp_zarlink_common.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the timer interface for
*      proslic api
*
****************************************************************************/

#include "vp_zarlink_common.h"

#include <bosMutex.h>


BOS_MUTEX spiZarMutex = {0};

/*
******************************************************************************
** FUNCTION:  spiInit
**
** PURPOSE:   Initializes SPI related items.
**
** PARAMETERS: None
**
** RETURNS: None
**
** NOTES: Allow only 1 mutex for SPI accesses.
**
*****************************************************************************
*/
void spiZarInit()
{
   if( spiZarMutex.mutex == NULL )
   {   
      bosMutexCreate( "SPI-MUTEX", &spiZarMutex );
   }
}

void spiZarDeinit()
{
   if( spiZarMutex.mutex != NULL )
   {
      bosMutexDestroy( &spiZarMutex );
   }
}


