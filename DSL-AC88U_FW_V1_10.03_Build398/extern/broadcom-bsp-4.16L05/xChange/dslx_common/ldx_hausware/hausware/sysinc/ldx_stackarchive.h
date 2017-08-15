/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
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
*****************************************************************************/
#if !defined( LDX_STACKARCHIVE_H )
#define LDX_STACKARCHIVE_H

/* ---- Include Files ---------------------------------------------------- */

#include <hapi_types.h>
#include <ldx_hapi.h>              /* Needed for  LDX_StackType */
#include <ldx_servicearchive.h>

/* ---- Constants and Types ---------------------------------------------- */

typedef struct LDX_StackArchive
{
   const LDX_StackType        stackId;
   const HAPI_UINT16          framesize;
   const LDX_ServiceArchive   *srvarc;
   const HAPI_UINT16          numServices;

} LDX_StackArchive;

#endif /* LDX_STACKARCHIVE_H */
