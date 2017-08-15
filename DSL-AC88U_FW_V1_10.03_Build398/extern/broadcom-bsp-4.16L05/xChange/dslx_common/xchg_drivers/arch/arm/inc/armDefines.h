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
*    Filename: armDefines.h
*
****************************************************************************
*    Description:
*
*     ARM constants.
*
****************************************************************************/

#ifndef ARM_DEFINES_H
#define ARM_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
#ifndef  DCACHE_LINE_SIZE_BYTES
#define DCACHE_LINE_SIZE_BYTES 16
#endif

#ifndef  ICACHE_LINE_SIZE_BYTES
#define ICACHE_LINE_SIZE_BYTES 16
#endif

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


#ifdef __cplusplus
    }
#endif

#endif  /* ARM_DEFINES_H  */
