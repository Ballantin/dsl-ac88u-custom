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
*    Description:
*
*      HausWare API for Sidetone FNC object.
*
****************************************************************************/

#ifndef HAPI_SDTN_H
#define HAPI_SDTN_H

/* ---- Include Files ---------------------------------------------------- */


#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* HausWare API Global definitions  */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* HausWare generic cmds/events     */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* HausWare API IDs                 */
#endif

/* ---- Constants and Types ---------------------------------------------- */

/****************************************************************************
* SDTN COMMANDS                                                             *
****************************************************************************/

#define HAPI_SDTN_ENABLE_CMD           (HSF_MKCMD_STD(HAPI_SDTN_PXD, 0))
   /* OP1: TRUE to enable, FALSE to disable                                */
   /* OP2: reserved = 0                                                    */

#define HAPI_SDTN_SET_VOL_CMD          (HSF_MKCMD_STD(HAPI_SDTN_PXD, 1))
   /* OP1: positive Q12 multiplier representing gain between 0 and 8       */
   /*      to be applied to the send signal.                               */
   /* OP2: reserved = 0                                                    */

/****************************************************************************
* SDTN RESPONSES                                                            *
****************************************************************************/

/* No responses are currently defined. */

/****************************************************************************
* SDTN ASYNCHRONOUS  EVENTS                                                 *
****************************************************************************/

/* No events are currently defined. */

/****************************************************************************
* STRUCTURES AND ASSOCIATED DEFINITIONS                                     *
****************************************************************************/

/* No structures are currently defined. */

#endif /* HAPI_SDTN_H */
