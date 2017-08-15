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
*    Filename: ipc3341CfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in ipc3341Cfg.h
*
****************************************************************************/

#ifndef IPC_3341_CFG_CUSTOM_H
#define IPC_3341_CFG_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif
                                                 


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/

/* Task priority for IPC interrupt service task. */
#define IPC_3341_CFG_TASK_PRIORITY        MTA_TASK_HARDWARE


/* Task priority for IPC interrupt service task. */
#define IPC_3341_CFG_READ_IN_ISR_CONTEXT  1


#ifdef __cplusplus
    }
#endif

#endif  /* IPC_3341_CFG_CUSTOM_H  */
