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
*  @file    uhsml.h
*
*  @brief   Lower uHSM device driver header file
*
*  This file contains the lower device driver header information for the
*  uHSM module.
*/
/****************************************************************************/

#ifndef UHSML_H
#define UHSML_H


/* ---- Include Files ----------------------------------------------------- */

#include <xdrvTypes.h>


#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
    XDRV_SINT32 (* initFp)( void );
    int (* readUint32Fp)(void *context, int address, int *data);
    int (* writeUint32Fp)(void *context, int address, int *data);
}
UHSML_FUNCS;


typedef struct
{
   /* This must be the first structure member (but why??) */
   const UHSML_FUNCS *funcP;
}
UHSML_DRV;


/* ---- Public Variables -------------------------------------------------- */


/* ---- Public Function Prototypes ---------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* UHSML_H */
