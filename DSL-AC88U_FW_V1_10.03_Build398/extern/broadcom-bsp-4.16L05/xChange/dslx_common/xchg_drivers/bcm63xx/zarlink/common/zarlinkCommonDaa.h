/***************************************************************************
*
*    Copyright 2008  Broadcom
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
*      This is common code for all Zarlink (Legerity) SLIC devices, that uses API-2
*
***************************************************************************/
#include <vp_api.h>
#include <vp_profile.h>

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xdrvDaa.h>

/* Constants declarations */
#define DAA_INIT_TIMEOUT 200

/*
 *  Function declarations
 */
XDRV_BOOL initZarlinkDaaDevice( VpDeviceType deviceType, int mspiDevId, XDRV_UINT8 deviceNum );
XDRV_BOOL zarlinkFxoLineIsOffhook( VpLineCtxType *pLineCtx );
void zarlinkFxoLineModeControl( VpLineCtxType *pLineCtx, XDRV_DAA_MODE mode );
void zarlinkProcessFxoEvents( VpDevCtxType* pDevCtx, XDRV_UINT32 deviceChan );
