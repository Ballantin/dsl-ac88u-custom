/*****************************************************************************
*
*  Copyright (c) 2004 Broadcom, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    usbl.h
*
*  @brief   Lower USB device driver header file
*
*  This file contains the lower device driver header information for the
*  USB module.
*/
/****************************************************************************/

#ifndef USBL_H
#define USBL_H


/* ---- Include Files ----------------------------------------------------- */

#include <xdrvTypes.h>


#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
    XDRV_SINT32 (* initFp)( XDRV_UINT32 usbNum, XDRV_UINT32 byteSwap );
    XDRV_SINT32 (* ohciRegBaseAddrGetFp)( XDRV_UINT32 usbNum, XDRV_UINT32 *addrP  );
}
USBL_FUNCS;


typedef struct
{
   /* This must be the first structure member (but why??) */
   const USBL_FUNCS *funcP;
   const XDRV_UINT32 usbCnt;
}
USBL_DRV;


/* ---- Public Variables -------------------------------------------------- */


/* ---- Public Function Prototypes ---------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* USBL_H */
