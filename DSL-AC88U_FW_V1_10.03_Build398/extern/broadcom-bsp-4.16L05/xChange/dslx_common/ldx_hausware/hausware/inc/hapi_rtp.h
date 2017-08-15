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
*      RTP service implements RFC 1889, RFC 1890 and RFC 2833 plus
*
****************************************************************************/

#ifndef HAPI_RTP_H                     /* support nested includes */
#define HAPI_RTP_H

/* ---- Include Files ---------------------------------------- */
#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* Hausware environment definitions */
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* Hausware API Global definitions */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif
#ifndef HAPI_PKT_H
#include <hapi_pkt.h>                  /* Hausware voice packet definitions */
#endif

/* Service name, version */
#define HAPI_RTP_SRV_NAME "RTP"

/* ---- Constants and Types ---------------------------------- */


/***************************************************************************
**        SERVICE COMMANDS
****************************************************************************/

/* Generic commands and events have the STD/EXT bit set in the HAPI_OPC_xxx define */
/* Also they don't require descriptions of OP1 and OP2 - see hapi_opcode.h for these */
#define HAPI_RTP_SETREGS_CMD        (HSF_MKHDR(HAPI_RTP_SRV, HAPI_OPC_SETREGS_CMD))
#define HAPI_RTP_SETREG1_CMD        (HSF_MKHDR(HAPI_RTP_SRV, HAPI_OPC_SETREG1_CMD))
#define HAPI_RTP_GETREGS_CMD        (HSF_MKHDR(HAPI_RTP_SRV, HAPI_OPC_GETREGS_CMD))
#define HAPI_RTP_GETSTATS_CMD       (HSF_MKHDR(HAPI_RTP_SRV, HAPI_OPC_GETSTATS_CMD))
#define HAPI_RTP_GETSTATE_CMD       (HSF_MKHDR(HAPI_RTP_SRV, HAPI_OPC_GETSTATE_CMD))
#define HAPI_RTP_REG_UPDATE_CMD     (HSF_MKHDR(HAPI_RTP_SRV, HAPI_OPC_REG_UPDATE_CMD))
#define HAPI_RTP_RESET_STATS_CMD    (HSF_MKHDR(HAPI_RTP_SRV, HAPI_OPC_RESET_STATS_CMD))

/* Non-generic commands and events */
#define HAPI_RTP_SETMODE_CMD         (HSF_MKCMD_STD(HAPI_RTP_SRV, 0))
      /* op1 - HAPI_RTP_MODE_IDLE or HAPI_RTP_MODE_ACTIVE */
      /* op2 - reserved (0) */

#define HAPI_RTP_SET_SSRC_CMD        (HSF_MKCMD_STD(HAPI_RTP_SRV, 1))
      /* op1 - 16 MSBs of the SSRC */
      /* op2 - 16 LSBs of the SSRC */

#define HAPI_RTP_TEST_CMD            (HSF_MKCMD_STD(HAPI_RTP_SRV, 2))
      /* Command used for testing purpose. The value of op1 and op2 are test specific */

#define HAPI_RTP_GEN_NTE_CMD         (HSF_MKCMD_STD(HAPI_RTP_SRV, 3))
      /*
       * Generate an ingress NTE (named telephony event) (RFC2833 event).
       * This is used specifically for non-DTMF events, such as Flash, etc.
       * Thus power levels and durations are not required.
       * op1 - event code in lower 8 bits (e.g. flash = 16)
       * op2 - reserved (0)
       */

#define HAPI_RTP_SET_INITIAL_SEQUENCE_NUMBER_CMD (HSF_MKCMD_STD(HAPI_RTP_SRV, 4))
      /* op1 - Initial sequence number; Must be enabled via configFlags otherwise
       *       this setting is ignored and a pseudo-random sequence number
       *       is generated.
       */
      /* op2 - reserved (0) */

      /* This is an internal command used to update the RTCP trigger
       * counter in an event-driven Hausware environment */
#define HAPI_RTP_UPDATE_RTCP_TRIGGER_CMD (HSF_MKCMD_STD(HAPI_RTP_SRV, 5))
      /* op1 - Time elapsed since last update */
      /* op2 - reserved (0) */

#define HAPI_RTP_PACKET_EXPAND_END_CMD (HSF_MKCMD_STD(HAPI_RTP_SRV, 6))
      /* op1 - Number of bytes to add to the end of RTP and RTCP packets */
      /* op2 - reserved (0) */

#define HAPI_RTP_GETSTATS_EXT_CMD (HSF_MKCMD_EXT(HAPI_RTP_SRV, 7))
      /* op1 - Length in bytes of HAPI_RTP_STATS */
      /* op2 - Pointer to HAPI_RTP_STATS */

#define HAPI_RTP_TRIPLICATE_SEND_TIMER_CMD (HSF_MKCMD_STD(HAPI_RTP_SRV, 8))
      /* op1 - Time elapsed */
      /* op2 - Reserved */

#define HAPI_RTP_VBD_PAYLOADMAP_SET_CMD (HSF_MKCMD_STD(HAPI_RTP_SRV, 9))
      /* op1 - VBD Payload Type */
      /* op2 - VBD Codec */

/* Commands for the 2nd register set */
#define HAPI_RTP_SETREGS2_CMD        (HSF_MKCMD_EXT(HAPI_RTP_SRV, 10))
#define HAPI_RTP_SETREG21_CMD        (HSF_MKCMD_STD(HAPI_RTP_SRV, 11))
#define HAPI_RTP_GETREGS2_CMD        (HSF_MKCMD_EXT(HAPI_RTP_SRV, 12))

/* statistical threshold monitor commands */
#define HAPI_RTP_ENABLE_MONITOR_CMD               (HSF_MKCMD_STD(HAPI_RTP_SRV, 13))
#define HAPI_RTP_RESET_ALL_MONITORS_CMD           (HSF_MKCMD_STD(HAPI_RTP_SRV, 14))
#define HAPI_RTP_GET_THRESHOLD1_CRITICAL_CMD      (HSF_MKCMD_STD(HAPI_RTP_SRV, 15))
#define HAPI_RTP_GET_THRESHOLD1_WARNING_CMD       (HSF_MKCMD_STD(HAPI_RTP_SRV, 16))
#define HAPI_RTP_GET_THRESHOLD1_CMD               (HSF_MKCMD_EXT(HAPI_RTP_SRV, 17))
#define HAPI_RTP_GET_THRESHOLDS_CMD               (HSF_MKCMD_EXT(HAPI_RTP_SRV, 18))
#define HAPI_RTP_SET_THRESHOLD1_CRITICAL_CMD      (HSF_MKCMD_STD(HAPI_RTP_SRV, 19))
#define HAPI_RTP_SET_THRESHOLD1_WARNING_CMD       (HSF_MKCMD_STD(HAPI_RTP_SRV, 20))
#define HAPI_RTP_SET_THRESHOLD1_CMD               (HSF_MKCMD_EXT(HAPI_RTP_SRV, 21))
#define HAPI_RTP_SET_THRESHOLDS_CMD               (HSF_MKCMD_EXT(HAPI_RTP_SRV, 22))

#define HAPI_RTP_SET_NTP_TIMESTAMP_CMD            (HSF_MKCMD_EXT(HAPI_RTP_SRV, 23))

#define HAPI_RTP_SET_CSVOICE_CMD                  (HSF_MKCMD_EXT(HAPI_RTP_SRV, 24))
/***************************************************************************
**        SERVICE COMMAND RESPONSES
****************************************************************************/

#define HAPI_RTP_GETREGS_RSP      (HSF_MKRSP_EXT(HAPI_RTP_GETREGS_CMD))
#define HAPI_RTP_GETSTATE_RSP     (HSF_MKRSP_EXT(HAPI_RTP_GETSTATE_CMD))
#define HAPI_RTP_GETSTATS_EXT_RSP (HSF_MKRSP_EXT(HAPI_RTP_GETSTATS_EXT_CMD))

/* Responses for the 2nd register set */
#define HAPI_RTP_SETREGS2_RSP     (HSF_MKRSP_EXT(HAPI_RTP_SETREGS2_CMD))
#define HAPI_RTP_SETREG21_RSP     (HSF_MKRSP_STD(HAPI_RTP_SETREG21_CMD))
#define HAPI_RTP_GETREGS2_RSP     (HSF_MKRSP_EXT(HAPI_RTP_GETREGS2_CMD))

#define HAPI_RTP_GET_THRESHOLD1_CRITICAL_RSP     (HSF_MKRSP_STD(HAPI_RTP_GET_THRESHOLD1_CRITICAL_CMD))
#define HAPI_RTP_GET_THRESHOLD1_WARNING_RSP      (HSF_MKRSP_STD(HAPI_RTP_GET_THRESHOLD1_WARNING_CMD))
#define HAPI_RTP_GET_THRESHOLD1_RSP              (HSF_MKRSP_EXT(HAPI_RTP_GET_THRESHOLD1_CMD))
#define HAPI_RTP_GET_THRESHOLDS_RSP              (HSF_MKRSP_EXT(HAPI_RTP_GET_THRESHOLDS_CMD))

/***************************************************************************
**        SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

/* Non-generic events require specifying HSF_MKHDR_STD or HSF_MKHDR_EXT */

/* example event detected - fixed format */
#define HAPI_RTP_INGRESS_RTCP_REPORT_EVT    (HSF_MKEVT_EXT(HAPI_RTP_SRV, 0))
      /* bufp points to buffer containing entire RTCP report generated and sent to packet network in octet array */
      /* length is the number of octets in the buffer */

#define HAPI_RTP_EGRESS_RTCP_REPORT_EVT     (HSF_MKEVT_EXT(HAPI_RTP_SRV, 1))
      /* bufp points to buffer containing entire RTCP report generated and sent to packet network in octet array */
      /* length is the number of octets in the buffer */

#define HAPI_RTP_EGRESS_ERROR_EVT           (HSF_MKEVT_STD(HAPI_RTP_SRV, 2))
      /* op1 - error code as defined in HAPI_RTP_EGRESS_ERROR */
      /* op2 - reserved (0) */

#define HAPI_RTP_EGRESS_SSRC_COLLISION_EVT  (HSF_MKEVT_STD(HAPI_RTP_SRV, 3))
      /* op1 - higher order 16 bits of SSRC */
      /* op2 - lower order 16 bits of SSRC  */

#define HAPI_RTP_BYE_EVT                    (HSF_MKEVT_EXT(HAPI_RTP_SRV, 4))
      /* bufp points to the buffer containing entire RTCP BYE packet as received from the network in octet array */
      /* length is the number of octets in the buffer */

#define HAPI_RTP_TEST_EVT                  (HSF_MKEVT_STD(HAPI_RTP_SRV, 5))
      /* This event is used for testing purpose. op1 and op2 are test specific */

#define HAPI_RTP_GETSTATS_EVT               (HSF_MKEVT_EXT(HAPI_RTP_SRV, 6))
      /* bufp points to the buffer containing the collected RTCP statistics */
      /* length is the number of octets in the buffer */

#define HAPI_RTP_SID_INGRESS_EVT            (HSF_MKEVT_STD(HAPI_RTP_SRV, 7))
      /* op1 - reserved */
      /* op2 - reserved */

#define HAPI_RTP_EGRESS_DTMF_EVT            (HSF_MKEVT_STD(HAPI_RTP_SRV, 8))
      /* op1 - o = off, 1 = on */
      /* op2 - DTMF digit */

#define HAPI_RTP_THRESHOLD_MONITOR_EVT       (HSF_MKEVT_EXT(HAPI_RTP_SRV, 9))

/***************************************************************************
**        SERVICE API STRUCTURES AND ASSOCIATED DEFINES
****************************************************************************/

/* define the RTP Header Minimum Length as a public constant - See RFC 1889 Jan 1996 Section 5.1 */
#define HAPI_RTP_MIN_RTP_HEADER_LENGTH       12

#define HAPI_RTP_MIN_RFC2198_HEADER_LENGTH   5

/* define the minimum RTCP packet transmission interval in units of milliseconds */
#define HAPI_RTP_MIN_RTCP_PACKET_INTERVAL    5000  /* Minimum is 5000 milliseconds, see RFC 1889 January 1996 section 6.2. But we allow 1000 msec */

/* define CNG payload types */
#define HAPI_RTP_IETF_PT_CNG                 13       /* current CNG payload type */
#define HAPI_RTP_IETF_PT_CNG_ALT             19       /* older version CNG payload type */

/*
** The following defines the RTCP packet types described in RFC 1889 January 1996 section 11.1.
** They will be returned as the "op1" parameter in HAPI_RTP_RTCP_EGRESS_REPORT_EVT. Each of these
** items are likely to be 32 bit constants allocated in global space. When used, these items must
** be converted to 8 bit quantities either implicitly or explicitly.
*/
typedef enum
{
   HAPI_RTP_RTCP_PACKET_TYPE_MIN    = 200,
   HAPI_RTP_RTCP_PACKET_TYPE_SR     = 200,
   HAPI_RTP_RTCP_PACKET_TYPE_RR     = 201,
   HAPI_RTP_RTCP_PACKET_TYPE_SDES   = 202,
   HAPI_RTP_RTCP_PACKET_TYPE_BYE    = 203,
   HAPI_RTP_RTCP_PACKET_TYPE_APP    = 204,
   HAPI_RTP_RTCP_PACKET_TYPE_XR     = 207,
   HAPI_RTP_RTCP_PACKET_TYPE_MAX    = 207
} HAPI_RTP_RTCP_PACKET_TYPE;

/*
** The following defines the SDES types described in RFC 1889 January 1996 section 11.2.
** They will be returned in "op2" in HAPI_RTP_RTCP_EGRESS_REPORT_EVT when "op1" is equal to
** HAPI_RTP_RTCP_PACKET_TYPE_SDES. Each of these items are likely to be 32 bit constants
** allocated in global space. When used, these items must be converted to 16 bit quantities either
** implicitly or explicitly.
*/
typedef enum
{
   HAPI_RTP_RTCP_SDES_TYPES_MIN = 0,
   HAPI_RTP_RTCP_SDES_END      = 0,
   HAPI_RTP_RTCP_SDES_CNAME    = 1,
   HAPI_RTP_RTCP_SDES_NAME     = 2,
   HAPI_RTP_RTCP_SDES_EMAIL    = 3,
   HAPI_RTP_RTCP_SDES_PHONE    = 4,
   HAPI_RTP_RTCP_SDES_LOC      = 5,
   HAPI_RTP_RTCP_SDES_TOOL     = 6,
   HAPI_RTP_RTCP_SDES_NOTE     = 7,
   HAPI_RTP_RTCP_SDES_PRIV     = 8,
   HAPI_RTP_RTCP_SDES_TYPES_MAX = 8
} HAPI_RTP_RTCP_SDES_TYPE;

/*
** The following defines the XR extended report block types as defined in RFC 3611 
** November 2003 section 4.
*/
typedef enum
{
   HAPI_RTP_RTCP_XR_BT_LOSS_RLE       = 1,
   HAPI_RTP_RTCP_XR_BT_DUPLICATE_RLE  = 2,
   HAPI_RTP_RTCP_XR_BT_RECEIPT_TIMES  = 3,
   HAPI_RTP_RTCP_XR_BT_REFERENCE_TIME = 4,
   HAPI_RTP_RTCP_XR_BT_DLRR           = 5,
   HAPI_RTP_RTCP_XR_BT_STATISTICS     = 6,
   HAPI_RTP_RTCP_XR_BT_VOIP           = 7,
   HAPI_RTP_RTCP_XR_BT_MAX            = 7
} HAPI_RTP_RTCP_XR_BT;

/*
** The following defines are codes used in the first parameter "op1" of HAPI_RTP_EGRESS_ERROR_EVT  Each of these items are likely
** to be 32 bit constants allocated in global space. When used, these items must be converted to 16 bit quantities either
** implicitly or explicitly.
*/
typedef enum
{
   HAPI_RTP_EGRESS_ERROR_CODE_MIN = 0,
   HAPI_RTP_EGRESS_ERROR_CLEARED                     = 0,  /* IMPORTANT - cleared must be zero */
   HAPI_RTP_EGRESS_ERROR_INVALID_PACKET_LENGTH       = 1,
   HAPI_RTP_EGRESS_ERROR_INVALID_PAYLOAD_TYPE        = 2,
   HAPI_RTP_EGRESS_ERROR_UNKNOWN_RTCP_PACKET_TYPE    = 3,
   HAPI_RTP_EGRESS_ERROR_WRONG_VERSION               = 4,
   HAPI_RTP_EGRESS_ERROR_NO_SUPPORT                  = 5,
   HAPI_RTP_EGRESS_ERROR_CODE_MAX = 5
} HAPI_RTP_EGRESS_ERROR;

/*
** IETF Codec types definition. See section RFC 1890 January 1996 table 2 (section 7). Each of these items are likely
** to be 32 bit constants allocated in global space. When used, these items must be converted to 16 bit quantities either
** implicitly or explicitly.
*/
typedef enum
{
   HAPI_RTP_IETF_PT_CODEC_G711U     = 0,
   HAPI_RTP_IETF_PT_CODEC_G726_32   = 2,
   HAPI_RTP_IETF_PT_CODEC_GSM       = 3,
   HAPI_RTP_IETF_PT_CODEC_G723      = 4,
   HAPI_RTP_IETF_PT_CODEC_G711A     = 8,
   HAPI_RTP_IETF_PT_CODEC_G722      = 9,
   HAPI_RTP_IETF_PT_CODEC_G728      = 15,
   HAPI_RTP_IETF_PT_CODEC_G729      = 18,
   HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST  = 96,
   HAPI_RTP_IETF_PT_CODEC_DYNAMIC_HIGHEST = 127,
   HAPI_RTP_IETF_PT_CODEC_MAX = 0x7f    /* 7 bits maximum is used for payload type - RFC 1889 January 1996 section 5.1 */
} HAPI_RTP_IETF_PT_CODEC;

/* Define the length of an SDES item */
#define HAPI_RTP_MAX_ITEM_SIZE  22

/* Define the length of CNAME */
#define HAPI_RTP_MAX_CNAME_SIZE  HAPI_RTP_MAX_ITEM_SIZE

/* Define the length of NAME */
#define HAPI_RTP_MAX_NAME_SIZE   HAPI_RTP_MAX_ITEM_SIZE

/* Define the length of EMAIL */
#define HAPI_RTP_MAX_EMAIL_SIZE  HAPI_RTP_MAX_ITEM_SIZE

/* Define the length of PHONE */
#define HAPI_RTP_MAX_PHONE_SIZE  HAPI_RTP_MAX_ITEM_SIZE

/* Define the length of LOC */
#define HAPI_RTP_MAX_LOC_SIZE    HAPI_RTP_MAX_ITEM_SIZE

/* Define the length of TOOL */
#define HAPI_RTP_MAX_TOOL_SIZE   HAPI_RTP_MAX_ITEM_SIZE

/* Define the length of NOTE */
#define HAPI_RTP_MAX_NOTE_SIZE   HAPI_RTP_MAX_ITEM_SIZE

/* Define the length of PRIV */
#define HAPI_RTP_MAX_PRIV_SIZE   HAPI_RTP_MAX_ITEM_SIZE

/* Encoding type to IETF payload type map is a table of entries of UIN16s. The more significant
** 8 bits store the IETF payload type and the less 8 bits store the encoder type.
**
** bit position 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
** Usage        | IETF payload type | Encoding type |
**
** The following macro, HAPI_RTP_ENCODING_TO_PT_MAP_MK_ENTRY, takes the IETF payload type and encoding
** type and makes the resulting HAPI_UINT16 value
*/
#define HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(ietfPt,codecNum)  (((ietfPt & 0xff) << 8) | (codecNum & 0xff))
#define HAPI_RTP_GET_PAYLOAD_TYPE_FROM_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(entry)  ((entry & 0xff00) >> 8)
#define HAPI_RTP_GET_CODEC_FROM_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(entry) (entry & 0x00ff)

/*
** The following constant, HAPI_RTP_MEDIA_PAYLOAD_TYPE_DEFAULT_MAP, is the default encoding to IETF payload type
** mapping used.
*/
#define HAPI_RTP_MEDIA_PAYLOAD_TYPE_DEFAULT_MAP \
{ \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711U,               HAPI_PKT_VC_G711_ULAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711A,               HAPI_PKT_VC_G711_ALAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+19), HAPI_PKT_VC_G726_32),  \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G728,                HAPI_PKT_VC_G728_16), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G729,                HAPI_PKT_VC_G729_8), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G723,                HAPI_PKT_VC_G7231_VARIABLE),  \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_GSM,                 HAPI_PKT_VC_GSM_FR_13), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST,      HAPI_PKT_VC_G726_16), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+1),  HAPI_PKT_VC_G726_24), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+2),  HAPI_PKT_VC_G726_40), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+3),  HAPI_PKT_VC_G729_118), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+4),  HAPI_PKT_VC_PCM_CLEAR), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+5),  HAPI_PKT_VC_G729A_8), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+7),  HAPI_PKT_VC_BV16), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+8),  HAPI_PKT_VC_LINPCM128), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+9),  HAPI_PKT_VC_G711WB_ULAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+10), HAPI_PKT_VC_G711WB_ALAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+11), HAPI_PKT_VC_BV32), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+12), HAPI_PKT_VC_LINPCM256), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+13), HAPI_PKT_VC_G722_64_MODE_2), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+14), HAPI_PKT_VC_G722_64_MODE_3), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_G722),              HAPI_PKT_VC_G722_64_MODE_1),\
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+15), HAPI_PKT_VC_ILBC20), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+16), HAPI_PKT_VC_ILBC30), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+17), HAPI_PKT_VC_G7221_24), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+18), HAPI_PKT_VC_G7221_32), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+20), HAPI_PKT_VC_GSM_EFR_122), \
   /* Reserve HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+21 for RFC2833 NTE) */ \
   /* Reserve HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+22 for RFC2198)     */ \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+23), HAPI_PKT_VC_AMR_WB_VARIABLE), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+24), HAPI_PKT_VC_GSM_AMR_VARIABLE), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_DYNAMIC_LOWEST+25), HAPI_PKT_VC_AMR_WB_OA_VARIABLE), \
}

/* End system delay values 
 * These values do not include the effects of superpacketization
 * and the jitter buffer. For example, the total end system delay 
 * for G. 711, 10ms superpackets, with 10 ms of jitter is 
 * 24 + 10 + 10 = 44 ms.
 *
 * This array matches the HAPI_PKT_VOICECODE structure in
 * hapi_pkt.h
 */
#define HAPI_RTCP_XR_END_SYSTEM_DELAY_DEFAULT \
{ \
   24,   /* G711 - uLaw - 64 kbps */ \
   24,   /* G711 - aLaw - 64 kbps */ \
   24,   /* G.726 - 16 kbps */ \
   24,   /* G.726 - 24 kbps */ \
   24,   /* G.726 - 32 kbps */ \
   24,   /* G.726 - 40 kbps */ \
   29,   /* G.729(A) - 8 kbps */ \
   29,   /* G.729(A) - 8 kbps */ \
   29,   /* G.729E - 11.8 kbps */ \
   31,   /* G.723.1 - 5.3 kbps */ \
   31,   /* G.723.1 - 6.3 kbps */ \
   24,   /* G.728 - 16 kbps */ \
   24,   /* BroadVoice Narrow Band 16 kbps */ \
   24,   /* linear PCM - 128 kbps */ \
   50,   /* iLBC speech coder - 20 ms frame / 15.2 kbps */ \
   50,   /* iLBC speech coder - 30 ms frame / 13.3 kbps */ \
   0,    /* GSM FR - 13 kbps */ \
   0,    /* GSM EFR - 12.2 kbps */ \
   0,    /* GSM AMR - 4.75 kbps */ \
   0,    /* GSM AMR - 5.15 kbps */ \
   0,    /* GSM AMR - 5.90 kbps */ \
   0,    /* GSM AMR - 6.70 kbps */ \
   0,    /* GSM AMR - 7.40 kbps */ \
   0,    /* GSM AMR - 7.95 kbps */ \
   0,    /* GSM AMR - 10.2 kbps */ \
   0,    /* GSM AMR - 12.2 kbps */ \
   0,    /* GSM AMR OA- 4.75 kbps */ \
   0,    /* GSM AMR OA- 5.15 kbps */ \
   0,    /* GSM AMR OA- 5.90 kbps */ \
   0,    /* GSM AMR OA- 6.70 kbps */ \
   0,    /* GSM AMR OA- 7.40 kbps */ \
   0,    /* GSM AMR OA- 7.95 kbps */ \
   0,    /* GSM AMR OA- 10.2 kbps */ \
   0,    /* GSM AMR OA- 12.2 kbps */ \
   0,    /* Reserved */ \
   0,    /* Reserved */ \
   26,   /* G711 wideband - uLaw - 128 kbps */ \
   26,   /* G711 wideband - aLaw - 128 kbps */ \
   26,   /* BroadVoice Wide Band 32 kbps */ \
   26,   /* linear PCM - 256 kbps */ \
   28,   /* G.722 - 64 kbps, mode 1 => 64 kbps audio */ \
   28,   /* G.722 - 64 kbps, mode 2 => 56 kbps audio */ \
   28,   /* G.722 - 64 kbps, mode 3 => 48 kbps audio */ \
   46,   /* G.722.1 - 24 kbps, wideband */ \
   46,   /* G.722.1 - 32 kbps, wideband */ \
   0,    /* AMR-WB 6.60 kbps, wideband */ \
   0,    /* AMR-WB 8.85 kbps, wideband */ \
   0,    /* AMR-WB 12.65 kbps, wideband */ \
   0,    /* AMR-WB 14.25 kbps, wideband */ \
   0,    /* AMR-WB 15.85 kbps, wideband */ \
   0,    /* AMR-WB 18.25 kbps, wideband */ \
   0,    /* AMR-WB 19.85 kbps, wideband */ \
   0,    /* AMR-WB 23.05 kbps, wideband */ \
   0,    /* AMR-WB 23.85 kbps, wideband */ \
   0,    /* AMR-WB OA 6.60 kbps, wideband  */ \
   0,    /* AMR-WB OA 8.85 kbps, wideband  */ \
   0,    /* AMR-WB OA 12.65 kbps, wideband */ \
   0,    /* AMR-WB OA 14.25 kbps, wideband */ \
   0,    /* AMR-WB OA 15.85 kbps, wideband */ \
   0,    /* AMR-WB OA 18.25 kbps, wideband */ \
   0,    /* AMR-WB OA 19.85 kbps, wideband */ \
   0,    /* AMR-WB OA 23.05 kbps, wideband */ \
   0     /* AMR-WB OA 23.85 kbps, wideband */ \
}

/*
** Maximum number of codecs supported. If you change this number, the above list also changes.
*/
#define HAPI_RTP_MAX_N_CODECS             30

/*
** RTP Register set.  To ensure endian independence all
** elements must be 16-bits
*/
typedef struct
{
   /*
    * configFlags field is a 16 register. Every bit is used independently to configure
    * various behaviours for the service. Each bit is identified by the bit masks definition
    * such as HAPI_UI4H_CONFIGFLAGS_RTCP_USE_ALT_IP defined later in this file
    */
   HAPI_UINT16   configFlags;
   HAPI_UINT16   rtcpPacketInterval;     /* interval for RTCP packet transmission in units of milliseconds */
                                    /* for more information, refer to RFC 1889 January 1996 section 6.2 */
                                    /* HAPI_UINT16 makes a maximum of about 65 seconds (64K msec) */

   HAPI_UINT16   rfc2198PayloadType;     /* lower order 7 bits specify the payload type used for RFC 2198 Redundant audio */
                                    /* higher order 9 bits are reserved */
   HAPI_UINT16   ntePayloadType;         /* lower order 7 bits specify the payload type used for RFC 2833 payload audio */
   HAPI_UINT16   nteToneLevel;           /* Tone Power Level over-ride */
   HAPI_UINT16   mediaPayloadTypeMap[HAPI_RTP_MAX_N_CODECS];   /* maps codec numbers to payload type. Left 8 bits */
                                                          /* (more significant 8 bits) stores payload type, and the */
                                                          /* other 8 bits store the codec number */
   HAPI_UINT16   cnameLength;                       /* Length of canonical name (CNAME) in bytes */
   HAPI_UINT16   cnameFrequency;                    /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   cname[HAPI_RTP_MAX_CNAME_SIZE/2];  /* Array of characters CNAME. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
   HAPI_UINT16   nameLength;                        /* Length of user name (NAME) in bytes */
   HAPI_UINT16   nameFrequency;                     /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   name[HAPI_RTP_MAX_NAME_SIZE/2];    /* Array of characters for NAME. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
   HAPI_UINT16   emailLength;                       /* Length of email address (EMAIL) in bytes */
   HAPI_UINT16   emailFrequency;                    /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   email[HAPI_RTP_MAX_EMAIL_SIZE/2];  /* Array of characters for EMAIL. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
   HAPI_UINT16   phoneLength;                       /* Length of phone number (PHONE) in bytes */
   HAPI_UINT16   phoneFrequency;                    /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   phone[HAPI_RTP_MAX_PHONE_SIZE/2];  /* Array of characters for PHONE. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
   HAPI_UINT16   locLength;                         /* Length of user location (LOC) in bytes */
   HAPI_UINT16   locFrequency;                      /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   loc[HAPI_RTP_MAX_LOC_SIZE/2];      /* Array of characters for LOC. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
   HAPI_UINT16   toolLength;                        /* Length of tool name (TOOL) in bytes */
   HAPI_UINT16   toolFrequency;                     /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   tool[HAPI_RTP_MAX_TOOL_SIZE/2];    /* Array of characters for TOOL. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
   HAPI_UINT16   noteLength;                        /* Length of notice / status (NOTE) in bytes */
   HAPI_UINT16   noteFrequency;                     /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   note[HAPI_RTP_MAX_NOTE_SIZE/2];    /* Array of characters for NOTE. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
   HAPI_UINT16   privLength;                        /* Length of private extensions (PRIV) in bytes */
   HAPI_UINT16   privFrequency;                     /* Frequency CNAME would be included in SDES */
   HAPI_UINT16   priv[HAPI_RTP_MAX_PRIV_SIZE/2];    /* Array of characters for PRIV. UINT16 is used */
                                               /* instead of UINT8 because registers must use UINT16 */
/* JWONG - [add] Asymmetric payload mapping */
   HAPI_UINT16   rfc2198EgressPayloadType;     /* lower order 7 bits specify the payload type used for RFC 2198 Redundant audio */
                                    /* higher order 9 bits are reserved */
   HAPI_UINT16   nteEgressPayloadType;         /* lower order 7 bits specify the payload type used for RFC 2833 payload audio */
   HAPI_UINT16   mediaEgressPayloadTypeMap[HAPI_RTP_MAX_N_CODECS];   /* maps codec numbers to payload type. Left 8 bits */
                                                          /* (more significant 8 bits) stores payload type, and the */
                                                          /* other 8 bits store the codec number */
/* JWONG - [end] Asymmetric payload mapping */

   /* RTCP XR (RFC 3611 November 2003) configuration Flags */
   HAPI_UINT16   rtcpXrConfigFlags;                           /* RTCP XR Configuration register */
   HAPI_UINT16   rtcpXrLossRleThinning;                       /* RTCP XR Loss RLE Report Thinning value */
   HAPI_UINT16   rtcpXrDuplicateRleThinning;                  /* RTCP XR Duplicate RLE Report Thinning value */
   HAPI_UINT16   rtcpXrReceiptTimesThinning;                  /* RTCP XR Packet Receipt Times Report Thinning value */
   HAPI_UINT16   rtcpXrEndSystemDelay[ HAPI_PKT_VC_ALG_MAX ]; /* RTCP XR End System Delay Constants */

   HAPI_UINT16   rfc2198RedundancyLevels; /* Levels of redundancy to use in RFC 2198 */
   HAPI_UINT16   rtpSecHeaderSize;        /* Additional header for authentication */

} HAPI_RTP_REGS;

/* Second register set */
typedef struct
{
   HAPI_UINT16   configFlags2;

} HAPI_RTP_REGS2;


/*
 * configFlags field in the HAPI_RTP_REGS usage: Each bit is used independently to configure
 * the behaviour of the RTP service. The following defines the bit positions in the configFlags field
 */
#define HAPI_RTP_EGRESS_REPORT_ERROR_TO_HOST_ENABLE_MASK          (1 << 0)
/* 1 => all errors detected in received packets are reported to the host */
/* 0 => above bit set to 0, errored packets are silently dropped and a statistics is incremented */

#define HAPI_RTP_INGRESS_SEND_RTCP_PACKETS_ENABLE_MASK            (1 << 1)
/* 1 => RTCP packets will be generated and sent to the packet network */
/* 0 =>, no RTCP packets will be generated in the ingress direction   */

#define HAPI_RTP_INGRESS_SEND_RTCP_PACKETS_TO_HOST_ENABLE_MASK    (1 << 2)
/* 1 => all ingress RTCP packets are reported to the host */
/* 0 => no such report/event is sent to the host             */

#define HAPI_RTP_EGRESS_SEND_RTCP_PACKETS_TO_HOST_ENABLE_MASK     (1 << 3)
/* 1 => all valid RTCP packets received causes a report/event to be sent to the host */
/*      to inform the host the type of RTCP packets received. */
/* 0 => If set to 0, no such report/event is sent to the host */

#define HAPI_RTP_EGRESS_CHECK_FOR_SSRC_COLLISION_MASK             (1 << 4)
/* 1 => SSRC collision detection is performed */
/* 0 => SSRC collision detection is NOT performed - need this for loop back */

#define HAPI_RTP_EGRESS_DROP_REDUNDANT_PAYLOAD_MASK               (1 << 5)
/* 1 => the redundant part of egress RFC2198 packets are ignored */
/* 0 => all parts of egress RFC2198 packets are processed */

#define HAPI_RTP_INGRESS_RFC2198_ENABLE_MASK                      (1 << 6)
/* 1 => ingress packets are always processed as RFC2198 packets */
/* 0 => ingress regular packets (not RFC2198) are always generated for all packets */

#define HAPI_RTP_INGRESS_GENERATE_RTCP_BYE_PACKET_ENABLE_MASK     (1 << 7)
/* 1 => Ingress RTCP BYE packets are generated and sent to the network */
/* 0=> No ingress RTCP BYE packes are generated */

#define HAPI_RTP_EGRESS_ACCEPT_RTCP_BYE_PACKET_ENABLE_MASK        (1 << 8)
/* 1 => Egress RTCP packets received from the network are accepted and sent to the host */
/* 0 => Egress RTCP packets received from the network are ignored */
#define HAPI_RTP_QOS_EVENT_ENABLE_MASK                            (1 << 9)
/* 1 => Enable RTP QOS related events (ingress SID generation / egress Tone relay packets reception) */
/* 0 => Disable RTP QOS related events */

#define HAPI_RTP_INGRESS_RANDOMIZE_RTCP_INTERVAL_MASK             (1 << 10)
/* 1 => Ingress RTCP packets would be randomized based on the packet interval */
/* 0 => Ingress RTCP packets would be generated on a fixed interval */

#define HAPI_RTP_INGRESS_SEND_RTCP_XR_PACKETS_ENABLE_MASK         (1 << 11)
/* 1 => RTCP XR packets will be generated and sent to the packet network */
/* 0 => no RTCP XR packets will be generated in the ingress direction   */

#define HAPI_RTP_USE_USER_DEFINED_INITIAL_SEQUENCE_NUMBER_MASK    (1 << 12)
/* 1 => RTP packets will use the user defined initial sequence number from RTP registers */
/* 0 => RTP packets will be generated with a pseudo-random initial sequence number */

#define HAPI_RTP_USE_USER_DEFINED_SSRC_MASK                       (1 << 13)
/* 1 => RTP packets will use the user defined SSRC number */
/* 0 => RTP packets will be generated with a pseudo-random SSRC number */

#define HAPI_RTP_VBD_V152_ENABLE_MASK                             (1 << 14)
/* 1 => V.152 Voiceband Data is enabled for VBD mode */
/* 0 => V.152 Voiceband Data is disabled for VBD mode */

#define HAPI_RTP_VBD_V152_INGRESS_RFC2198_ENABLE_MASK             (1 << 15)
/* 1 => RTP Redundancy is enabled for VBD mode */
/* 0 => RTP Redundancy is disabled for VBD mode */

/*
 * Following are configFlag2 bit definitions
 */
#define HAPI_RTP_INGRESS_SEND_RTP_PACKETS_DISABLE_MASK            (1 << 0)
/* 1 => RTP packets will not be sent to the packet network */
/* 0 => RTP packets will be generated in the ingress direction */

#define HAPI_RTP_EGRESS_PROCESS_RTP_PACKETS_DISABLE_MASK          (1 << 1)
/* 1 => RTP packets will be recorded in receive statistics and will not be processed */
/* 0 => RTP packets will be sent to DSP for processing */

#define HAPI_RTP_TONE_TRIPLICATE_REDUNDANCY_MASK                  (1 << 2)
/* 1 => Enable triplicate tone redundancy */
/* 0 => Disable triplicate tone redundancy */

#define HAPI_RTP_GR303_NUERA_ENABLE_MASK                          (1 << 3)
/* 1 => Enable GR303 Nuera support -- send on / off hook packets with 127 payload type */
/* 0 => Disable GR303 Nuera support */

#define HAPI_RTP_RFC2833_KEEP_ALIVE_ENABLE_MASK                   (1 << 4)
/* 1 => Enable GR303 Keep Alive Message (sending onhook/offhook message periodically) */
/* 0 => Disable GR303 Keep Alive Message */

#define HAPI_RTP_SUBTRACT_DTMF_ENABLE_MASK                        (1 << 5)
/* 1 => Enable Subtraction of DTMF tones (do not generate any DTMF packets) */
/* 0 => Disable DTMF subtraction */

/* Alliu, G726 ANNEX E compliance */
#define HAPI_RTP_G726_ANNEX_E_ENABLE_MASK                         (1 << 6)
/* 1 => Enable G726 Annex E compliance nibble swap */
/* 0 => Disable nibble swap */
/* Alliu, END */

/* 
 * Following are the bit definitions for the RTCP XR config register 
 */
#define HAPI_RTP_RTCP_XR_GENERATE_LOSS_RLE_REPORT                 (1 << 0)
/* 1 => Generate RTCP XR Loss RLE Report Block */
/* 0 => Disable Report */

#define HAPI_RTP_RTCP_XR_GENERATE_DUPLICATE_RLE_REPORT            (1 << 1)
/* 1 => Generate RTCP XR Duplicate RLE Report Block */
/* 0 => Disable Report */

#define HAPI_RTP_RTCP_XR_GENERATE_PACKET_RECEIPT_TIMES_REPORT     (1 << 2)
/* 1 => Generate RTCP XR Packet Receipt Times Report Block */
/* 0 => Disable Report */

#define HAPI_RTP_RTCP_XR_GENERATE_RECEIVER_REFERENCE_TIME_REPORT  (1 << 3)
/* 1 => Generate RTCP XR Receiver Reference Time Report Block */
/* 0 => Disable Report */

#define HAPI_RTP_RTCP_XR_GENERATE_DLRR_REPORT                     (1 << 4)
/* 1 => Generate RTCP XR DLRR Report Block */
/* 0 => Disable Report */

#define HAPI_RTP_RTCP_XR_GENERATE_STATISTICS_REPORT               (1 << 5)
/* 1 => Generate RTCP XR Statistics Report Block */
/* 0 => Disable Report */

#define HAPI_RTP_RTCP_XR_GENERATE_VOIP_REPORT                     (1 << 6)
/* 1 => Generate RTCP XR VoIP Report Block */
/* 0 => Disable Report */

typedef struct
{
   HAPI_UINT16 currentZone;
   HAPI_UINT16 monitorID;
   union {
      HAPI_SINT16 sValue;
      HAPI_UINT16 uValue;
   } thresholdValue;
   union {
      HAPI_SINT16 sValue;
      HAPI_UINT16 uValue;
   } currentValue;
} HAPI_RTP_MONITOR_STATE;


typedef struct
{
   HAPI_UINT16 hysteresisInZone:8;
   HAPI_UINT16 currentZone:2;
   HAPI_UINT16 prevZone:2;
   HAPI_SINT32 currentValue;

} HAPI_RTP_THRESHOLD_MONITOR;

/*
** RTP stats threshold monitor.  
*/

typedef struct
{
   HAPI_UINT16      enableThresholdMonitor;
   union {
      HAPI_SINT16      sValue;
      HAPI_UINT16      uValue;
   } criticalLevels;
   union {
      HAPI_SINT16      sValue;
      HAPI_UINT16      uValue;
   } warningLevels;

} HAPI_RTP_THRESHOLD_LEVEL_REGS;

typedef struct
{
   HAPI_RTP_THRESHOLD_LEVEL_REGS      NLR_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      GLD_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      BLD_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      GD_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      BD_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      JDR_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      RTD_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      ESD_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      SL_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      NL_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      RERL_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      NSR_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      XSR_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      MLQ_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      MCQ_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      JBN_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      JBM_Threshold;
   HAPI_RTP_THRESHOLD_LEVEL_REGS      IAJ_Threshold;

} HAPI_RTP_THRESHOLD_REGS;

/*
 * Following are RTP stats threshold monitor bit definitions
 */
#define HAPI_RTP_MONITOR_NLR            0      /* Network Loss Rate */
#define HAPI_RTP_MONITOR_GLD            1      /* Gap Loss Density */
#define HAPI_RTP_MONITOR_BLD            2      /* Burst Loss Density */
#define HAPI_RTP_MONITOR_GD             3      /* Gap Duration */
#define HAPI_RTP_MONITOR_BD             4      /* Burst Duration */
#define HAPI_RTP_MONITOR_JDR            5      /* Jitter Discard Rate */
#define HAPI_RTP_MONITOR_RTD            6      /* Round Trip Delay */
#define HAPI_RTP_MONITOR_ESD            7      /* End System Delay */
#define HAPI_RTP_MONITOR_SL             8      /* Signal Level */
#define HAPI_RTP_MONITOR_NL             9      /* Noise Level */
#define HAPI_RTP_MONITOR_RERL           10     /* Residual Echo Return Loss */
#define HAPI_RTP_MONITOR_NSR            11     /* R-Factor */
#define HAPI_RTP_MONITOR_XSR            12     /* External R-Factor */
#define HAPI_RTP_MONITOR_MLQ            13     /* Mean Opinion Score for Listening Quality */
#define HAPI_RTP_MONITOR_MCQ            14     /* Mean Opinion Score for Conversational Quality */
#define HAPI_RTP_MONITOR_JBN            15     /* Jitter Buffer Nominal Delay */
#define HAPI_RTP_MONITOR_JBM            16     /* Jitter Buffer Maximum Delay */
#define HAPI_RTP_MONITOR_IAJ            17     /* Inter-arrival Jitter */
#define HAPI_RTP_MONITOR_MAX            18

/*
** RTP stats.  To ensure endian independence all
** elements must be 16-bits
*/

/* first - local source statistics used in RTCP packets meaningful within the context of an RTP session */
typedef struct
{
   HAPI_UINT16   ingressRtpPacketsHigh;                 /* 16 MSBs of number of ingress packets sent     */
   HAPI_UINT16   ingressRtpPacketsLow;                  /* 16 LSBs of number of ingress packets sent     */
   HAPI_UINT16   ingressRtpBytesHigh;                   /* 16 MSBs of number of bytes in ingress packets */
   HAPI_UINT16   ingressRtpBytesLow;                    /* 16 LSBs of number of bytes in ingress packets */
} HAPI_RTP_LOCAL_SESSION_STATS;


/* Second - statistics used in RTCP packets meaningful within the context of an RTP session */
typedef struct
{
   HAPI_UINT16   egressRtpJitterHigh;                   /* 16 MSBs of the interarrival jitter */
   HAPI_UINT16   egressRtpJitterLow;                    /* 16 LSBs of the interarrival jitter */
   HAPI_UINT16   ingressRtpLatencyHigh;                 /* 16 MSBs of the round-trip delay - a smoothed version */
   HAPI_UINT16   ingressRtpLatencyLow;                  /* 16 LSBs of the round-trip delay - a smoothed version */
   HAPI_UINT16   ingressPeakRtpLatencyHigh;             /* 16 MSBs of the peak round-trip delay */
   HAPI_UINT16   ingressPeakRtpLatencyLow;              /* 16 LSBs of the peak round-trip delay */
   HAPI_UINT16   egressRtpLossFraction;                 /* Fraction of RTP data packets lost since the previous SR/RR.
                                                           This is a 8-bit number. */
   HAPI_UINT16   jitterMin;                             /* Jitter buffer nominal delay */
   HAPI_UINT16   jitterMax;                             /* Jitter buffer absolute maximum delay */
   HAPI_UINT16   MOSlq;                                 /* MOS LQ value from RTCP-XR */
   HAPI_UINT16   MOScq;                                 /* MOS CQ value from RTCP-XR */
   HAPI_UINT16   reserved;
   HAPI_UINT32   meanAbsoluteJitter;                    /* Defined in Sect 4.6.2.9.6 of RFC6035, and ITU-T G.1020 */
   HAPI_UINT32   maxMeanAbsoluteJitter;
} HAPI_RTP_SESSION_STATS;

/* Third - statistics collected continously across RTP sessions */
typedef struct
{
   HAPI_UINT16   ingressRtpPacketsHigh;                 /* 16 MSBs of number of ingress packets sent */
   HAPI_UINT16   ingressRtpPacketsLow;                  /* 16 LSBs of number of ingress packets sent */
   HAPI_UINT16   ingressRtpBytesHigh;                   /* 16 MSBs of number of bytes in ingress voice and SID RTP packets  */
   HAPI_UINT16   ingressRtpBytesLow;                    /* 16 LSBs of number of bytes in ingress voice and SID RTP packets */
   HAPI_UINT16   egressRtpPacketsHigh;                  /* 16 MSBs of number of egress packets received including errored packets */
   HAPI_UINT16   egressRtpPacketsLow;                   /* 16 MSBs of number of egress packets received including errored packets */
   HAPI_UINT16   egressRtpBytesHigh;                    /* 16 MSBs of number of bytes in egress voice and SID RTP packets  */
   HAPI_UINT16   egressRtpBytesLow;                     /* 16 LSBs of number of bytes in egress voice and SID RTP packets  */
   HAPI_UINT16   egressRtpCumulativeLostPacketsHigh;    /* 16 MSBs of number of egress packets lost */
   HAPI_UINT16   egressRtpCumulativeLostPacketsLow;     /* 16 LSBs of number of egress packets lost */
   HAPI_UINT16   egressRtpDiscardedPacketsHigh;         /* 16 MSBs of the number of egress packets discarded */
   HAPI_UINT16   egressRtpDiscardedPacketsLow;          /* 16 LSBs of the number of egress packets discarded */
   HAPI_UINT16   ingressRtpNoProfileDiscardedPackets;   /* No. of ingress packets discarded because no profile found */
   HAPI_UINT16   ingressRtcpPackets;                    /* No. of ingress RTCP packets generated and sent */
                                                        /* Non zero possbile only when XCFG_RTP_RTCP_TX_SUPPORT is set to 1 */
   HAPI_UINT16   egressRtcpPackets;                     /* No. of egress RTCP packets received including errored packets */
   HAPI_UINT16   ingressRtcpXrPackets;                  /* No. of ingress RTCP XR packets generated */
   HAPI_UINT16   egressRtcpXrPackets;                   /* No. of egress RTCP XR packets received */
   HAPI_UINT16   reserved;
} HAPI_RTP_CUMULATIVE_STATS;

/*
 * Stats consists of per call/session stats and cumulative stats
 */
typedef struct
{
   HAPI_RTP_CUMULATIVE_STATS  cumulative;           /* stats cumulative cross all sessions */
   HAPI_RTP_SESSION_STATS     session;                 /* per call/sessino stats */
   HAPI_RTP_LOCAL_SESSION_STATS sessionLocal;       /* per call/sessino stats for the local source */
} HAPI_RTP_STATS;

/*
** HAPI_RTP_STATE structure is returned via HAPI_RTP_GETSTATE_RSP
*/
typedef struct
{
   HAPI_UINT16   higherRemoteSsrc;                             /* Higher order 16 bits of SSRC of the current remote RTP entity */
   HAPI_UINT16   lowerRemoteSsrc;                              /* lower order 16 bits of the SSRC of the current remote RTP entity */
   HAPI_UINT16   sequenceNum;                                  /* sequence number to be inserted into the outgoing RTP packet */
   HAPI_UINT16   higherLocalSsrc;                              /* higher order 16 bits of local SSRC */
   HAPI_UINT16   lowerLocalSsrc;                               /* lower order 16 bits of local SSRC */
   HAPI_UINT16   mode;                                         /* Idle or active - see HAPI_RTP_MODE */
/* JWONG - [add] Added base time stamp generation */
   HAPI_UINT16   higherBaseTimestamp;                          /* higher order initial 16 bits timestamp */
   HAPI_UINT16   lowerBaseTimestamp;                           /* lower order initial 16 bits timestamp */
   HAPI_UINT16   bNewBaseTimeStampSet;                         /* Flag indicating generation of new base timestamp */
/* JWONG - [end] Added base time stamp generation */
} HAPI_RTP_STATE;

/*
** Define 3 structures, which are simply each of the REGS, STATE, and STATS
** structures, prefixed with a srvid field. This is done to support the
** net service holding the slave services' data for mode switching and
** maintaining a consistent API across all modes.
*/
HAPI_SRVID_STRUCT(RTP_REGS, regs)
HAPI_SRVID_STRUCT(RTP_STATE, state)
HAPI_SRVID_STRUCT(RTP_STATS, stats)

/*
** Define a single amalgamated structure of RTP DATA that is
** stored in the supervisor. A single structure simplifies the
** mechanism used to set and retrieve pointers to here.
*/
typedef struct
{
   HAPI_RTP_REGS_SRVID rtpRegs;
   HAPI_RTP_STATE_SRVID rtpState;
   HAPI_RTP_STATS_SRVID rtpStats;
} HAPI_RTP_DATA;

typedef enum
{
   HAPI_RTP_MODE_IDLE = 0,
   HAPI_RTP_MODE_ACTIVE = 1
} HAPI_RTP_MODE;

/*
** RTP NTP timestamps
*/

typedef struct
{
   HAPI_UINT16 ntpTimeStampSecHigh;
   HAPI_UINT16 ntpTimeStampSecLow;
   HAPI_UINT16 ntpTimeStampUsecHigh;
   HAPI_UINT16 ntpTimeStampUsecLow;
} HAPI_RTP_NTP_TIMESTAMP;


#endif      /* HAPI_RTP_H */
