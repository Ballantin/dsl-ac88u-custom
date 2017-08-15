/***************************************************************************
*    Copyright 2001  Broadcom
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
*    Filename: netCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in netCfg.h
*
****************************************************************************/

#ifndef NET_CFG_CUSTOM_H
#define NET_CFG_CUSTOM_H



#undef   NET_CFG_IFACE_GET_DEV_NAME
#define  NET_CFG_IFACE_GET_DEV_NAME    ccGetDevIfaceName


#endif   /* NET_CFG_CUSTOM_H */
