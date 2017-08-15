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
*      HausWare API for Gain FNC object.
*
****************************************************************************/

#ifndef HAPI_GAIN_H
#define HAPI_GAIN_H

/* ---- Include Files ---------------------------------------------------- */


#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* HausWare API Global definitions  */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>               /* HausWare generic cmds/events     */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* HausWare API IDs                 */
#endif

/* ---- Constants and Types ---------------------------------------------- */

/* volume and gain values                                                  */
/* - Q12 format                                                            */

#define HAPI_GAIN_12DB    0x0400      /*-12 dB gain */
#define HAPI_GAIN_6DB     0x0800      /* -6 dB gain */
#define HAPI_GAIN_4DB     0x0a18      /* -4 dB gain */
#define HAPI_GAIN0DB      0x1000      /*  0 dB gain */
#define HAPI_GAIN2DB      0x1424      /*  2 dB gain */
#define HAPI_GAIN6DB      0x2000      /*  6 dB gain */
#define HAPI_GAIN12DB     0x4000      /* 12 dB gain */

/****************************************************************************
* GAIN COMMANDS                                                             *
****************************************************************************/

typedef enum
{
   HAPI_GAIN_INGRESS = 0,              /* set ingress gain */
   HAPI_GAIN_EGRESS                    /* set egress gain */

} HAPI_GAIN_CONFIG;

#define HAPI_GAIN_SET_GAIN_CMD          (HSF_MKCMD_STD(HAPI_GAIN_SRV, 0))
   /* OP1: HAPI_GAIN_INGRESS, HAPI_GAIN_EGRESS                             */
   /* OP2: linear multiplier - Q12 format. sbbb.bbbbbbbbbbbb [-8, 7.99)    */


/****************************************************************************
* GAIN RESPONSES                                                            *
****************************************************************************/

/* No responses are currently defined. */

/****************************************************************************
* GAIN ASYNCHRONOUS  EVENTS                                                 *
****************************************************************************/

/* No events are currently defined. */

/****************************************************************************
* STRUCTURES AND ASSOCIATED DEFINITIONS                                     *
****************************************************************************/

/* No structures are currently defined. */

#endif /* HAPI_GAIN_H */
