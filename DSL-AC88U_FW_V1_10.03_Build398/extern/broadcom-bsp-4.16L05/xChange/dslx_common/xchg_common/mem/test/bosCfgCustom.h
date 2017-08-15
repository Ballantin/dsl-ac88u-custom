/****************************************************************************
*
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosCfgCustom.h
*
*  @brief   BOS Custom Configuration.
*
****************************************************************************/

#if !defined( BOSCFGCUSTOM_H )
#define BOSCFGCUSTOM_H        /**< Include Guard                           */

#define BOS_LOG(x)   printf x ; fflush( stdout )

#define BOS_CFG_EVENT	      0
#define BOS_CFG_INIT          1
#define BOS_CFG_MUTEX         1
#define BOS_CFG_SEM           1
#define BOS_CFG_SLEEP         1
#define BOS_CFG_SOCKET        1
#define BOS_CFG_TASK          1
#define BOS_CFG_TIME          0

#define BOS_CFG_LOG_PARAM     0

#endif /* BOSCFGCUSTOM_H */

