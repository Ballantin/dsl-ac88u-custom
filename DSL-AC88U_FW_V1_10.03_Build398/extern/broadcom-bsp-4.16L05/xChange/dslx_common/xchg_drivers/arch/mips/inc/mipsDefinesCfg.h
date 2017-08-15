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
*    Filename: mipsDefinesCfg.h
*
****************************************************************************
*    Description:
*
*     Build configuration options for MIPS registers and constants.
*
****************************************************************************/

#ifndef MIPS_DEFINES_CFG_H
#define MIPS_DEFINES_CFG_H

#ifdef __cplusplus
extern "C" {
#endif


/* ---- Include Files ---------------------------------------------------- */

#include <mipsDefinesCfgCustom.h>


/* ---- Error check ------------------------------------------------------ */

/*
** There are no default values for these chip-specific defines. I don't want
** to assume default values which may result in unexpected behaviour.
*/

#ifndef  DCACHE_LINE_SIZE_BYTES
//LH #error   "DCACHE_LINE_SIZE_BYTES must be defined in mipsDefinesCfgCustom.h"
#define DCACHE_LINE_SIZE_BYTES 0x1000
#endif

#ifndef  ICACHE_LINE_SIZE_BYTES
//LH #error   "ICACHE_LINE_SIZE_BYTES must be defined in mipsDefinesCfgCustom.h"
#define ICACHE_LINE_SIZE_BYTES 0x1000
#endif


/* ---- Constants and Types ---------------------------------------------- */

#ifndef  MMU_SUPPORT
#define MMU_SUPPORT     0
#endif

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* MIPS_DEFINES_CFG_H  */
