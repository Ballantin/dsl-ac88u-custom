/***************************************************************************
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
****************************************************************************
*
*  Description:
*      This file is the public header API file for the vocoder wrappers.
*
****************************************************************************/

#ifndef  _HAPI_WRAP_DEFS_H
#define _HAPI_WRAP_DEFS_H

/*
** VAD types used by the vocoder wrappers
*/
typedef enum
{
   HAPI_VAD_TYPE_NONE,                     /* don't use any VAD */
   HAPI_VAD_TYPE_GENERIC,                  /* use the generic VAD */
   HAPI_VAD_TYPE_BUILTIN,                  /* use the built in VAD */
   HAPI_NUMVADTYPES

} HAPI_VAD_TYPES;

#endif
