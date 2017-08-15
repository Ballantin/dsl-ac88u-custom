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
*    Filename: hdspHal.h
*
****************************************************************************
*    Description:
*
*     Interface for hausware HAL objects.
*
****************************************************************************/

#ifndef HDSP_HAL_H
#define HDSP_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <haus_hapi.h>


/*
** I can't include ldx_hapi.h because it includes xcfg header files, and we
** don't add LDX application specific xcfg directories to the HDSP public
** include search path. So, I'll just create my own macro for now.
*/
/* #include <ldx_hapi.h> */

#define hdsp_LDX_HandleCreate( hdl, dspNum )    ( ((dspNum) << 8) | (hdl) )


/* ---- Constants and Types ---------------------------------------------- */

#define hdspHalGetHandle( endpt, dspNum )  \
                     hdsp_LDX_HandleCreate(( (endpt) + HAPIHAL0 ), (dspNum) )

#define hdspHalGetDspIndex( handle )  \
                     ( ((handle) >> 8) & 0xFF )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_HAL_H  */
