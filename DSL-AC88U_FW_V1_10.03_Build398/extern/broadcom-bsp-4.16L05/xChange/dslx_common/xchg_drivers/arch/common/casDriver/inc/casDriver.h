/***************************************************************************
*    Copyright 2000  Broadcom
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
*    Filename: casDriver.h
*
****************************************************************************
*    Description:
*
*     Interface for ...
*
****************************************************************************/

#ifndef CAS_DRIVER_H
#define CAS_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvApm.h>
#include <xdrvSlic.h>
#include <xdrvCas.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

int casDriverInit( XDRV_CAS *casDriver, XDRV_SLIC *slicDrv, XDRV_APM *apmDrv,  
                   int chan, XDRV_BOOL bFastSlicSupported,
                   XDRV_BOOL bCasHistorySupported );
int casDriverDeinit( XDRV_CAS *casDriver );

int casDriverStartHistoryLog( int historyRateMsec, int debounceIntervalMsec );
int casDriverStopHistoryLog( void );

#ifdef __cplusplus
    }
#endif

#endif  /* CAS_DRIVER_H  */
