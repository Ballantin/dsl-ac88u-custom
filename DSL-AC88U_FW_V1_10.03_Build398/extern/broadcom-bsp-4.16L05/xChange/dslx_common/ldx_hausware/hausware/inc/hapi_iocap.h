/***************************************************************************
*
* Copyright (c) 2003 Broadcom, All Rights Reserved.
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
*      HAL Capture API
*
****************************************************************************/

#ifndef HAPI_IOCAP_H
#define HAPI_IOCAP_H

/* ---- Include Files ---------------------------------------------------- */

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* HAPI environment definitions     */
#endif

#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware API object ids          */
#endif

#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>               /* Hausware generic cmds/events     */
#endif

/* ---- Constants and Types ---------------------------------------------- */
#define HAL_CAPTURE_ENABLED      1

#define HAL_CAP_MAX_CHAN 10

/* Its important that the buffer size is in multiples of the largest possible samples per interrupt */
#define HAL_CAP_MAX_SAMP_RATE_KHZ    16
#define HAL_CAP_MIN_PCMINT_RATE_MS   10
#define HAL_CAP_MAX_SAMP_PER_INT     (HAL_CAP_MAX_SAMP_RATE_KHZ * HAL_CAP_MIN_PCMINT_RATE_MS)        
#define HAL_CAP_NUM_SAMPLES          ((HAL_CAP_MAX_SAMP_PER_INT) * 50)  // 8,000 samples total = 0.5sec WB, 1sec NB capture

typedef struct {
   int enabled;
   int chan;
   void *  ingressCircBufHdl;
   void *  egressCircBufHdl;
} HAL_CAP_INFO;

typedef enum HAL_CAPTURE_DIRECTION
{
   HAL_CAPTURE_DIRECTION_INGRESS = 0, /* Don't change the numbering since it's used as array index */
   HAL_CAPTURE_DIRECTION_EGRESS,
   HAL_CAPTURE_DIRECTION_MAX
} HAL_CAPTURE_DIRECTION;


// /***************************************************************************
// **        HAL SHIM COMMANDS
// ****************************************************************************/

// /* Enable/disable a HAL Shim Device */
// #define HAPI_IOCAP_ENABLE_CMD          (HSF_MKHDR(HAPI_IOCAP_SRV, HAPI_OPC_ENABLE_CMD))
// #define HAPI_IOCAP_ENABLE_RSP          (HSF_MKRSP_STD(HAPI_IOCAP_ENABLE_CMD))
//       /* OP1: TRUE to enable; FALSE to disable                          */
//       /* OP2: Reserved                                                  */

// /* Flush IBD Queue */
// #define HAPI_IOCAP_FLUSH_CMD           (HSF_MKCMD_STD(HAPI_IOCAP_SRV, 1))
// #define HAPI_IOCAP_FLUSH_RSP           (HSF_MKRSP_STD(HAPI_IOCAP_FLUSH_CMD))
//       /* OP1: Reserved                                                  */
//       /* OP2: Reserved                                                  */

// /* Configure the HAL device */
// #define HAPI_IOCAP_CONFIG_CMD          (HSF_MKCMD_STD(HAPI_IOCAP_SRV, 4))
// #define HAPI_IOCAP_CONFIG_RSP          (HSF_MKRSP_STD(HAPI_IOCAP_CONFIG_CMD))
//       /* OP1: IBD mask              */
//       /* OP2: Samples per HAL tick  */



#endif      /* HAPI_IOCAP_H */
