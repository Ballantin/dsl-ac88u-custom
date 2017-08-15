/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************/

#if !defined( LDX_SOCKETCONFIG_H )
#define LDX_SOCKETCONFIG_H

/* ---- Include Files ---------------------------------------------------- */

#include <stddef.h>            /* Needed for size_t */

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

#if !defined( LDX_STACKARCHIVE_H )
#  include <ldx_stackarchive.h>
#endif

#if !defined( HSF_MASKS_H )
#  include <hsf_masks.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */

/*
 * VHD Options
 *    These options are used in the VHD archive.
 */
typedef enum
{
   HAPI_VHDOPT_NONE                   = 0x0000u,  /* No options selected               */
   HAPI_VHDOPT_DISABLE_INGRESS_THREAD = 0x0001u,  /* Disable the ingress thread        */
   HAPI_VHDOPT_DISABLE_EGRESS_THREAD  = 0x0002u,  /* Disable the egress thread         */
   HAPI_VHDOPT_DEFAULT_WIDEBAND       = 0x0004u,  /* Wideband by default               */
   HAPI_VHDOPT_DEFAULT_NARROWBAND     = 0x0008u,  /* Narrowband by default             */
   HAPI_VHDOPT_DSP_ONLY               = 0x0010u,  /* Indicates DSP only                */
   HAPI_VHDOPT_HOST_ONLY              = 0x0020u,  /* Indicates HOST only               */
} HAPI_VHDOPT;

/****************************************************************************
*
* LDX_SocketConfig contains configuration data which used when instantiating
* a socket.
*/

typedef struct
{
   HAPI_UINT16          socketScratchSize;
   HAPI_UINT16          socketHeapSize;
   LDX_StackArchive     *archive;
   HAPI_UINT16          numStackTypes;
   HAPI_UINT16          defaultStackId;   /* default ID */

   HAPI_VHDOPT          vhdOpt;

   HSF_IBD_MASK         tosRecvMask;
   HSF_IBD_MASK         bosRecvMask;


   /*
    * Narrowband history requirements used by the VHD frame change
    * logic to determine the amount of memory to allocate. Currently,
    * wideband services do not require history and thus they are not
    * declared here.
    */
   size_t               ingressNarrowbandMedqHist;
   size_t               egressNarrowbandMedqHist;

   /*
    * The following media queue sizes are the maximum media queue buffer
    * size requirements including history.
    */
   size_t               ingressMediaQSize;
   size_t               egressMediaQSize;

} LDX_SocketConfig;

#endif   /* LDX_SOCKETCONFIG_H */
