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
#ifndef ZARLINK_COMMON_H
#define ZARLINK_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


#include <vp_api.h>
#include <vp_profile.h>
#include <xdrvTypes.h>



/*
 *  Function declarations
 */
void zarlinkProcessEvents( VpDevCtxType* pDevCtx, VpEventCategoryType *category_save, XDRV_UINT16 *eventid_save, XDRV_UINT32 deviceChan );



#ifdef __cplusplus
    }
#endif

#endif  /* ZARLINK_COMMON_H  */
