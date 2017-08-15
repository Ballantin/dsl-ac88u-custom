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
*    Filename: heartbeatCfg.h
*
****************************************************************************
*    Description:
*
*     CAS control default build options.  To override defaults in
*     heartbeatCfg.h make a file called heartbeatCfgCustom.h in your build
*     directory.
*
****************************************************************************/

#ifndef HEARTBEAT_CFG_H
#define HEARTBEAT_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <heartbeatCfgCustom.h>


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/

/* Task priority for CAS */
#ifndef HEARTBEAT_CFG_TASK_PRIORITY
#define HEARTBEAT_CFG_TASK_PRIORITY                BOS_TASK_CLASS_HIGH
#endif

#ifdef __cplusplus
    }
#endif

#endif  /* HEARTBEAT_CFG_H  */


