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
*    Filename: mipsDefinesCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Build configuration options for MIPS registers and constants.
*
****************************************************************************/

#ifndef MIPS_DEFINES_CFG_CUSTOM_H
#define MIPS_DEFINES_CFG_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif


/* ---- Include Files ---------------------------------------------------- */
/* ---- Error check ------------------------------------------------------ */

/* ---- Constants and Types ---------------------------------------------- */

#define  DCACHE_LINE_SIZE_BYTES  16
#define  ICACHE_LINE_SIZE_BYTES  16
#define  MMU_SUPPORT             1

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* MIPS_DEFINES_CFG_CUSTOM_H  */
