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
*    Filename: casDriverCfg.h
*
****************************************************************************
*    Description:
*
*     CAS driver default build options.  To override defaults in
*     casDriverCfg.h make a file called casDriverCfgCustom.h in your build
*     directory.
*
****************************************************************************/

#ifndef CAS_DRIVER_CFG_H
#define CAS_DRIVER_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <casDriverCfgCustom.h>


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/

/*
** Set to 1 to enable the CAS history log. The history log installs a periodic
** ISR that logs hookstate samples to a buffer, that can be processed by the
** client.
*/
#ifndef CAS_DRIVER_CFG_HISTORY_LOG_ENABLED
#define CAS_DRIVER_CFG_HISTORY_LOG_ENABLED   1
#endif



#ifdef __cplusplus
    }
#endif

#endif  /* CAS_DRIVER_CFG_H  */
