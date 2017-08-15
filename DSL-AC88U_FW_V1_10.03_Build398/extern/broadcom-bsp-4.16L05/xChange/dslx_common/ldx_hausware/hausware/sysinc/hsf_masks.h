/***************************************************************************
*
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
****************************************************************************/
#ifndef HSF_MASKS_H
#define HSF_MASKS_H

/* ---- Include Files ---------------------------------------- */
#include <hapi_types.h>

/* ---- Constants and Types ---------------------------------- */

/*
 * These #define's used to be part of an enum, but it wouldn't compile under
 * some 16 bit processors so I changed everything to #define's
 */

#define HSF_IBD_NONE_MASK     0x0000uL    /* Do Not Use                 */
#define HSF_IBD_INTERNAL_MASK 0x0001uL    /* Internal Control           */
#define HSF_IBD_PACKET_MASK   0x0002uL    /* HAPI_PKT_HDR packet        */
#define HSF_IBD_IDLEP_MASK    0x0004uL    /* Idle Pattern MASK          */
#define HSF_IBD_TONE_MASK     0x0008uL    /* Tone MASK                  */

#define HSF_IBD_MODEM_MASK    0x0010uL    /* Fax/Modem Data Pump Mask   */
#define HSF_IBD_V8_MASK       0x0020uL    /* V.8 Mask                   */
#define HSF_IBD_CIRCUIT_MASK  0x0040uL    /* Circuit packet             */
#define HSF_IBD_DEBUG_MASK    0x0080uL    /* Debug Mask                 */
#define HSF_IBD_FAX_MASK      0x0100uL    /* Fax MASK                   */
#define HSF_IBD_LOWBAND_MASK  0x0200uL    /* lowband data packet        */
#define HSF_IBD_HIGHBAND_MASK 0x0400uL    /* high band data packet      */
#define HSF_IBD_RTP_MASK      0x0800uL    /* RTP packet                 */
#define HSF_IBD_RTCP_MASK     0x1000uL    /* RTCP packet                */
#define HSF_IBD_STATS_MASK    0x2000uL    /* Internal statistics        */

/*
 * When a BM Buffer is allocated for inband data, this bit
 * must also be set to indicate that the buffer must at some
 * point be freed if it doesn't go off the end of the stack.
 */

#define HSF_IBD_BMALLOC_MASK  0x8000uL    /* BM Buffer Allocated packet */

typedef  HAPI_UINT32   HSF_IBD_MASK;

/* ---- Variable Externs ------------------------------------- */
/* ---- Function Prototypes ---------------------------------- */

#endif

