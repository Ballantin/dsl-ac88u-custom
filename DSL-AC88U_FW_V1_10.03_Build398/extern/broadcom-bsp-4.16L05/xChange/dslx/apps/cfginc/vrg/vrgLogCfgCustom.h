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
*    Filename: vrgLogCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in vrgLog.h
*
****************************************************************************/

#ifndef VRG_LOG_CFG_CUSTOM_H
#define VRG_LOG_CFG_CUSTOM_H

#include <linux/bcm_log.h> 

#define VRG_LOG_MOD_BOARDHAL     BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_CAS          BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_EPT          BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_HBT          BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_HDSP         BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_TPD          BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_PROV         BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_PROBE        BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_DECT         BCM_LOG_ID_VOICE_EPT
#define VRG_LOG_MOD_LHAPI        BCM_LOG_ID_VOICE_EPT

#endif   /* VRG_LOG_CFG_CUSTOM_H */
