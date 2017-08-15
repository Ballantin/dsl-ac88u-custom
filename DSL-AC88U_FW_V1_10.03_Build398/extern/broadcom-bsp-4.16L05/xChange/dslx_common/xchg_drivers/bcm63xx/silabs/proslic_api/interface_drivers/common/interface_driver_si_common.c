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
*    Filename: interface_driver_si_common.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the timer interface for
*      proslic api
*
****************************************************************************/

#include "si_voice_datatypes.h"
#include "interface_driver_si_common.h"

#include <bosMutex.h>


BOS_MUTEX spiSiMutex = {0};

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
void spiSiInit()
{
   if( spiSiMutex.mutex == NULL )
   {   
      bosMutexCreate( "SPI-MUTEX", &spiSiMutex );
   }
}

void spiSiDeinit()
{
   if( spiSiMutex.mutex != NULL )
   {
      bosMutexDestroy( &spiSiMutex );
   }
}


