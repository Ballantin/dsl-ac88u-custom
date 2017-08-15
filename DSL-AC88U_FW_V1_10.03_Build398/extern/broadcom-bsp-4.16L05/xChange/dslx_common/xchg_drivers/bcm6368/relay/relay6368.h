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
#ifndef RELAY_6368_H    /* support nested includes */
#define RELAY_6368_H

#include <xdrvRelay.h>
#include <bcm6368.h>
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
typedef struct RELAY_6368_DRV
{
   /* Must be first element of this struct */
   const XDRV_RELAY_FUNCS *pDrvFuncs;

   /* 6368 Specific state info */
   int                    chan;        /* relay controled channel ID            */
   XDRV_BOOL              bDrvEnabled; /* driver state (enabled/disabled) */
   
   /*  6368 Specific config info */
   unsigned short         ctrlGpioPin;                        /* Gpio pin connected to relay */
   
} RELAY_6368_DRV;

/* ---- Variable Externs ------------------------------------- */

/* ---- Function Prototypes ---------------------------------- */

RELAY_STATUS relay6368Init( RELAY_6368_DRV *pDev );
RELAY_STATUS relay6368Deinit( RELAY_6368_DRV *pDev );
#endif   /* RELAY_6368_H */

