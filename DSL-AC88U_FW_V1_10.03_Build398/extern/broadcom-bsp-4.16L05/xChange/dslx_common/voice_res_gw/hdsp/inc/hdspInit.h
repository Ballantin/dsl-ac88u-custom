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
*    Filename: hdspInit.h
*
****************************************************************************
*    Description:
*
*     This file contains the public declarations for HDSP module initialization.
*
****************************************************************************/

#ifndef HDSP_INIT_H
#define HDSP_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <hapi.h>

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



VRG_SINT16 hdspInit
(
   HAPICALLBKP    eventCallback,
   int dspNum
);

VRG_SINT16 hdspDeinit( void );




#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_INIT_H  */
