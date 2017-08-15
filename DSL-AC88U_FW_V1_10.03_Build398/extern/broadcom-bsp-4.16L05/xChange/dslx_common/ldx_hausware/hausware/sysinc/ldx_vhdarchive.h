/****************************************************************************
*
*     Copyright (c) 2001-2002 Broadcom
*           All Rights Reserved
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
****************************************************************************/

#if !defined( LDX_VHDARCHIVE_H )
#define LDX_VHDARCHIVE_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( LDX_HAPI_H )
#  include "ldx_hapi.h"
#endif

#if !defined( LDX_SOCKETCONFIG_H )
#  include "ldx_socketconfig.h"
#endif


/* ---- Constants and Types ---------------------------------------------- */


/****************************************************************************
*
* The VHD_Archive structure contains a list of VHD's that can be instantiated.
*/

typedef struct
{
   LDX_ChannelType   archiveType;
   LDX_ChannelLevel  level;
   LDX_SocketConfig  *pSocketConfig;

} VHD_Archive;

#endif /* LDX_VHDARCHIVE_H */

