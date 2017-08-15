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
*    Filename: assertHandler3341CfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in assertHandler3341Cfg.h
*
****************************************************************************/

#ifndef ASSERT_HANDLER_3341_CFG_CUSTOM_H
#define ASSERT_HANDLER_3341_CFG_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif
                                                 


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/

/* Task priority for IPC interrupt service task. */
#define ASSERT_3341_CFG_TASK_PRIORITY     MTA_TASK_DEFAULT





#ifdef __cplusplus
    }
#endif

#endif  /* ASSERT_HANDLER_3341_CFG_CUSTOM_H  */
