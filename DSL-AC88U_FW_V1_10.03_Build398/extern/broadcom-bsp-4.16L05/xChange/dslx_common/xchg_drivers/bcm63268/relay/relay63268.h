/***************************************************************************
*
*    Copyright 2005  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*		This is the Legerity LE88221 - Single Channel Digital SLIC Device
*		driver definition.
*
****************************************************************************/
#ifndef RELAY_H    /* support nested includes */
#define RELAY_H

#include <xdrvRelay.h>
#include <bcmChip.h>
#include <board.h>

/* ---- Constants and Types ---------------------------------- */
typedef enum
{ 
   RELAY_SUCCESS = 0,
   RELAY_ERROR
} RELAY_STATUS;

typedef enum
{
   RELAY_MODE_DEACTIVE       = 0,
   RELAY_MODE_ACTIVE         = 1
} RELAY_MODE;

/* 
** Relay Driver information
*/
typedef struct RELAY_DRV
{
   /* Must be first element of this struct */
   const XDRV_RELAY_FUNCS *pDrvFuncs;

   /* 63xx Specific state info */
   int                    chan;        /* relay controled channel ID            */
   XDRV_BOOL              bDrvEnabled; /* driver state (enabled/disabled) */
   
   /*  63xx Specific config info */
   XDRV_UINT32            ctrlGpioPin;                        /* Gpio pin connected to relay */
   
} RELAY_DRV;

/* ---- Variable Externs ------------------------------------- */

/* ---- Function Prototypes ---------------------------------- */

RELAY_STATUS relayInit( RELAY_DRV *pDev );
RELAY_STATUS relayDeinit( RELAY_DRV *pDev );
#endif   /* RELAY_H */

