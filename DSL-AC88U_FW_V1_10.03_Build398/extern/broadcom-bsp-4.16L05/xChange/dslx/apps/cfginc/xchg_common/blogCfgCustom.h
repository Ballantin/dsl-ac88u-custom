/***************************************************************************
*
*     Copyright (c) 2002 Broadcom, All Rights Reserved.
*     Contains proprietary and confidential information.
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*      This file contains the custom configuration for the blog module
*
****************************************************************************/

#ifndef _BLOGCFGCUSTOM_H
#define _BLOGCFGCUSTOM_H

#ifdef BOS_OS_LinuxKernel 

#include <linux/types.h>
#include <linux/init.h>
#include <linux/bcm_log.h>

#define BOS_OVERRIDE_NATIVE_BLOG       1
#define BOS_LOG_MOD_BOS                BCM_LOG_ID_VOICE_BOS

#define blogStrAlways(a)               BCM_LOG_NOTICE    a
#define blogStrAlwaysNoFields(a)       BCM_LOG_NOTICE    a
#define blogStrFnc(a)                  BCM_LOG_NOTICE    a
#define blogStrErr(a)                  BCM_LOG_ERROR     a
#define blogStrWarn(a)                 BCM_LOG_ERROR     a
#define blogStrInfo(a)                 BCM_LOG_INFO      a

#endif

#endif  /* _BLOGCFGCUSTOM_H  */
