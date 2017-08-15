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
*    Filename: exceptionHdlrCmtMipsCfg.h
*
****************************************************************************
*    Description:
*
*     Build configuration options for concurrent multi-threaded (CMT)
*     MIPS core exception handler.
*
****************************************************************************/

#ifndef EXCEPTION_HDRL_CMT_MIPS_CFG_H
#define EXCEPTION_HDRL_CMT_MIPS_CFG_H

#ifdef __cplusplus
extern "C" {
#endif


/* ---- Include Files ---------------------------------------------------- */

#include <exceptionHdlrCmtMipsCfgCustom.h>


/* ---- Constants and Types ---------------------------------------------- */


#ifndef EXCEPTION_HDRL_CMT_CFG_TASK_PRIORITY
#define EXCEPTION_HDRL_CMT_CFG_TASK_PRIORITY     BOS_TASK_CLASS_MED
#endif


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* EXCEPTION_HDRL_CMT_MIPS_CFG_H  */
