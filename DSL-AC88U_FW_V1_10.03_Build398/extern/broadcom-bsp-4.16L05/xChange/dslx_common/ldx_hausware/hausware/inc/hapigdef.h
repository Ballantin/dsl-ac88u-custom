/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      Hausware API global definitions
*
****************************************************************************/

#ifndef HAPIGDEF_H                        /* support nested includes */
#define HAPIGDEF_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                      /* hausware environment */
#endif
                                          /* RESPONSES - error codes TBD */
typedef enum
{
   HAPIFAILURE = -1 ,            /* general failure */
   HAPISUCCESS = 0,              /* success         */

   HAPI_ERR_INVALID_REGS,        /* Invalid Register Setting */
   HAPI_ERR_UNKNOWN_MESSAGE,     /* Unknown Message received */
   HAPI_ERR_INVALID_CMD,         /* Message recognized but contains invalid parameters or content */
   HAPI_ERR_SRV_DISABLED,        /* Service is disabled. Command/message is ignored */
   HAPI_ERR_SRV_BUSY,            /* Service is busy. Command/message is ignored */

   HAPI_RESPONSE_DELAYED         /* Response will be sent in the future.
                                    Until then the OS doesn't free the
                                    response. Service saves the ptr.
                                    to fill in the response at a later
                                    time. */
} HAPIRESULT;

typedef enum
{
  HAPI_DIRECTION_INGRESS = 0,
  HAPI_DIRECTION_EGRESS = 1

} HAPI_DIRECTION;


#endif  /* HAPIGDEF_H */
