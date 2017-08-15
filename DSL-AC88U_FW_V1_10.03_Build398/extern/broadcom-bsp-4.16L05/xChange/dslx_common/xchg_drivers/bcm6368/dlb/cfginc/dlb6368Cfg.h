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
*    Filename: dlb6368Cfg.h
*
****************************************************************************
*    Description:
*
*     Build configuration options for dynamic load balancing (DLB)
*     on the 6368.
*
****************************************************************************/

#ifndef DLB_6368_CFG_H
#define DLB_6368_CFG_H

#ifdef __cplusplus
extern "C" {
#endif


/* ---- Include Files ---------------------------------------------------- */

#include <dlb6368CfgCustom.h>


/* ---- Constants and Types ---------------------------------------------- */


/* Task priority for load balancing task. */
#ifndef DLB_6368_CFG_TASK_PRIORITY
#define DLB_6368_CFG_TASK_PRIORITY     BOS_TASK_CLASS_HIGH
#endif


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* DLB_6368_CFG_H  */
