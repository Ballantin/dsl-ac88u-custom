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
****************************************************************************/

#ifndef HAPI_RM_H
#define HAPI_RM_H

/* ---- Include Files ---------------------------------------- */

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* Hausware environment definitions */
#endif

#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware API object ids          */
#endif

#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>               /* Hausware generic cmds/events     */
#endif

/* ---- Constants and Types ---------------------------------- */

/***************************************************************************
**        RESMAN COMMANDS
****************************************************************************/

/* Open a VHD */
#define HAPI_RM_OPEN_VHD_CMD                 (HSF_MKCMD_EXT(HAPI_RM_SRV, 1))
#define HAPI_RM_OPEN_VHD_RSP                 (HSF_MKRSP_EXT(HAPI_RM_OPEN_VHD_CMD))
      /* OP1: Length of HAPI_RM_OPEN_VHD_INFO     */
      /* OP2: Pointer to HAPI_RM_OPEN_VHD_INFO    */

/* Close a VHD */
#define HAPI_RM_CLOSE_VHD_CMD                (HSF_MKCMD_STD(HAPI_RM_SRV, 2))
#define HAPI_RM_CLOSE_VHD_RSP                (HSF_MKRSP_STD(HAPI_RM_CLOSE_VHD_CMD))
      /* OP1: VHD Handle   */
      /* OP2: reserved (0) */

/* Set the Ingress And Egress Frame Size for a VHD */
#define HAPI_RM_SET_FRAME_SIZE_CMD           (HSF_MKCMD_STD(HAPI_RM_SRV, 3))
#define HAPI_RM_SET_FRAME_SIZE_RSP           (HSF_MKRSP_STD(HAPI_RM_SET_FRAME_SIZE_CMD))
      /* OP1: VHD Handle   */
      /* OP2: Frame Size   */

/* Query the DSP buffer pool statistics */
#define HAPI_RM_DSP_BUFFER_QUERY_CMD         (HSF_MKCMD_EXT(HAPI_RM_SRV, 4))
#define HAPI_RM_DSP_BUFFER_QUERY_RSP         (HSF_MKRSP_EXT(HAPI_RM_DSP_BUFFER_QUERY_CMD))
      /* OP1: size of buffer pool statistics structure (in bytes)          */
      /* OP2: pointer to the buffer pool statistics (HAPI_RM_DSPBUFSTATS)  */

/* Close a VHD on the host only */ /* THis command is only support if building
                                      distributed hausware for the host */
#define HAPI_RM_CLOSE_VHD_ON_HOST_CMD        (HSF_MKCMD_STD(HAPI_RM_SRV, 5))
#define HAPI_RM_CLOSE_VHD_ON_HOST_RSP        (HSF_MKRSP_STD(HAPI_RM_CLOSE_VHD_ON_HOST_CMD))
      /* OP1: VHD Handle   */
      /* OP2: reserved (0) */

/* Initiate a stack switch */
#define HAPI_RM_STACK_SWITCH_CMD             (HSF_MKCMD_STD(HAPI_RM_SRV, 6))
#define HAPI_RM_STACK_SWITCH_RSP             (HSF_MKRSP_STD(HAPI_RM_STACK_SWITCH_CMD))
      /* OP1: VHD Handle       */
      /* OP2: Service Stack ID */

/* Query the Scratch and Stack memory statistics */
#define HAPI_RM_DSPMEMORY_QUERY_CMD          (HSF_MKCMD_EXT(HAPI_RM_SRV, 7))
#define HAPI_RM_DSPMEMORY_QUERY_RSP          (HSF_MKRSP_EXT(HAPI_RM_DSPMEMORY_QUERY_CMD))
      /* OP1: size of memory statistics structure (in bytes)                   */
      /* OP2: pointer to the memory statistics structure (HAPI_RM_DSPMEMSTATS) */

#define HAPI_RM_HOST_BUFFER_QUERY_CMD        (HSF_MKCMD_EXT(HAPI_RM_SRV, 8))
#define HAPI_RM_HOST_BUFFER_QUERY_RSP        (HSF_MKRSP_EXT(HAPI_RM_HOST_BUFFER_QUERY_CMD))
/* Query the DSP buffer pool statistics */
      /* OP1: size of buffer pool statistics structure (in bytes) */
      /* OP2: pointer to the buffer pool statistics (HAPI_RM_DSPBUFSTATS)  */

/* DO NOT USE - Send command array */
#define HAPI_RM_CMD_ARRAY_CMD           (HSF_MKCMD_EXT(HAPI_RM_SRV, 9))
#define HAPI_RM_CMD_ARRAY_RSP           (HSF_MKRSP_EXT(HAPI_RM_CMD_ARRAY_CMD))
      /* OP1: size of extended buffer                   */
      /* OP2: pointer to buffer of commands             */

/* BOP Inject mode */
#define HAPI_RM_BOP_INJECT_MODE_CMD     (HSF_MKCMD_STD(HAPI_RM_SRV, 25))
#define HAPI_RM_BOP_INJECT_MODE_RSP     (HSF_MKRSP_STD(HAPI_RM_BOP_INJECT_MODE_CMD))
      /* OP1: SWB_INJECT_OVERWRITE/SWB_INJECT_ADD       */
      /* OP2: Reserved                                  */

#define HAPI_RM_ASSIGN_ACCESS_HANDLE_CMD (HSF_MKCMD_STD(HAPI_RM_SRV, 26))
#define HAPI_RM_ASSIGN_ACCESS_HANDLE_RSP (HSF_MKRSP_STD(HAPI_RM_ASSIGN_ACCESS_HANDLE_CMD))
      /* OP1:  New Handle              */
      /* OP2:  Access Point Index      */

#define HAPI_RM_TEST_OVERRUN_CMD (HSF_MKCMD_STD(HAPI_RM_SRV, 27))
#define HAPI_RM_TEST_OVERRUN_RSP (HSF_MKRSP_STD(HAPI_RM_TEST_OVERRUN_CMD))
      /* OP1:  Introduce addition cycles (in MIPS)        */
      /* OP2:  Reserved     */

#define HAPI_RM_VHD_ENABLE_CMD (HSF_MKCMD_STD(HAPI_RM_SRV, 28))
#define HAPI_RM_VHD_ENABLE_RSP (HSF_MKRSP_STD(HAPI_RM_VHD_ENABLE_CMD))
      /* OP1:  VHD handle        */
      /* OP2:  upper byte: direction (HAPI_RM_DIRECTION type)
       *                    to be enabled or disabled
       *       lower byte:  1 for enable, 0 for disable  */

#define HAPI_RM_PERIODIC_TIMER_CMD (HSF_MKCMD_STD(HAPI_RM_SRV, 29))
#define HAPI_RM_PERIODIC_TIMER_RSP (HSF_MKRSP_STD(HAPI_RM_PERIODIC_TIMER_CMD))
      /* OP1:  Setup a periodic timer */
      /* OP2:  Timer Value            */

#define HAPI_RM_NONPERIODIC_TIMER_CMD (HSF_MKCMD_STD(HAPI_RM_SRV, 30))
#define HAPI_RM_NONPERIODIC_TIMER_RSP (HSF_MKRSP_STD(HAPI_RM_NONPERIODIC_TIMER_CMD))
      /* OP1:  Setup a non-periodic timer */
      /* OP2:  Timer Value                */

#define HAPI_RM_TIMER_SYNC_CMD (HSF_MKCMD_STD(HAPI_RM_SRV, 31))
#define HAPI_RM_TIMER_SYNC_RSP (HSF_MKRSP_STD(HAPI_RM_TIMER_SYNC_CMD))
      /* OP1:  Time since last sync command        */
      /* OP2:  Reserved                            */

#define HAPI_RM_OFFLOAD_VHD_CMD (HSF_MKCMD_STD(HAPI_RM_SRV, 32))
#define HAPI_RM_OFFLOAD_VHD_RSP (HSF_MKRSP_STD(HAPI_RM_OFFLOAD_VHD_CMD))
      /* OP1:  VHD Handle to offload             */
      /* OP2:  1 start off load  0 stop offload  */

#define HAPI_RM_CFG_PULSE_SIGNAL_CMD        (HSF_MKCMD_EXT(HAPI_RM_SRV,33))
#define HAPI_RM_CFG_PULSE_SIGNAL_RSP        (HSF_MKRSP_EXT(HAPI_RM_CFG_PULSE_SIGNAL_CMD))
/* Configure the DSP to generate pulse signal */
      /* OP1: size of pulse signal parameter structure (in bytes) */
      /* OP2: pointer to the pulse signal parameter structure (HAPIRMPULSESIGNALS)  */


/***************************************************************************
**        RESMAN ASYNCHRONOUS EVENTS
****************************************************************************/

/* Resource Manager Error Event */
#define HAPI_RM_ERROR_EVT                    (HSF_MKEVT_STD(HAPI_RM_SRV, 0))
      /* OP1: Error Code      */
      /* OP2: Error sub-code  */

/* Resource Manager Debug Event */
#define HAPI_RM_DEBUG_EVT                    (HSF_MKEVT_STD(HAPI_RM_SRV, 1))
      /* OP1: Event type -                               */
      /* OP1 = zero  : framesync jitter tracking event   */
      /* OP1 = one   : DSP assert event                  */
      /* OP1 = 2,3,. : reserved for future use           */
      /* OP2: parameter value                            */

/* Resource Manager VHD Stack Switch Event */
#define HAPI_RM_STACK_SWITCH_EVT           (HSF_MKEVT_STD(HAPI_RM_SRV, 2))
      /* OP1: VHD Handle       */
      /* OP2: Service Stack ID */

/* Resource Manager DSP ingress overrun Event */
#define HAPI_RM_DSP_INGRESS_OVERRUN_EVT    (HSF_MKEVT_STD(HAPI_RM_SRV, 3))
      /* OP1: 1 if overrun occured, false if normalcy returned       */
      /* OP2: timestamp in msec */

/* Resource Manager DSP egress underrun Event */
#define HAPI_RM_DSP_EGRESS_UNDERRUN_EVT    (HSF_MKEVT_STD(HAPI_RM_SRV, 4))
      /* OP1: 1 if underrun occured, false if normalcy returned       */
      /* OP2: timestamp in msec */

/* Resource Manager HOST ingress overrun Event */
#define HAPI_RM_HOST_INGRESS_OVERRUN_EVT    (HSF_MKEVT_STD(HAPI_RM_SRV, 5))
      /* OP1: 1 if overrun occured, false if normalcy returned       */
      /* OP2: timestamp in msec */


/***************************************************************************
**        RESMAN API DEFINITIONS
****************************************************************************/


/*
* Bit masks and shifts for the two OP2 fields in
* HAPI_RM_SET_INGRESS_MEDQ_MODE_CMD and HAPI_RM_SET_EGRESS_MEDQ_MODE_CMD
* commands
*
* Recommended Usage:
*
* (operand >> right-shift) & bit mask
*
* For example,
*
* (op2 >> HAPI_RM_MEDQ_CMD_MODE_RSHIFT) & HAPI_RM_MEDQ_CMD_MODE_BMASK;
*
*/
#define HAPI_RM_MEDQ_CMD_MODE_SHIFT          12
#define HAPI_RM_MEDQ_CMD_MODE_BMASK          0x000Fu

#define HAPI_RM_MEDQ_CMD_FRAMESIZE_SHIFT     0
#define HAPI_RM_MEDQ_CMD_FRAMESIZE_BMASK     0x0FFFu

#define HAPI_RM_ENABLE_SHIFT     0
#define HAPI_RM_ENABLE_BMASK     0x00FFu

#define HAPI_RM_DIRECTION_SHIFT     8
#define HAPI_RM_DIRECTION_BMASK     0xFF00u

#define HAPI_RM_GET_ENABLE(x) (x & HAPI_RM_ENABLE_BMASK) >> HAPI_RM_ENABLE_SHIFT
#define HAPI_RM_GET_DIRECTION(x) (x & HAPI_RM_DIRECTION_BMASK) >> HAPI_RM_DIRECTION_SHIFT

typedef enum
{
   HAPI_RM_DIRECTION_UNSPECIFIED = 0x0000,
   HAPI_RM_DIRECTION_INGRESS = 0x0001,
   HAPI_RM_DIRECTION_EGRESS = 0x0002,
} HAPI_RM_DIRECTION;

typedef enum
{
   HAPI_RM_VHD_NARROWBAND_MODE = 0,
   HAPI_RM_VHD_WIDEBAND_MODE = 1,
   HAPI_RM_VHD_MAX_MODES,
} HAPI_RM_VHD_MODE;


/* Structure used to open VHD
 */
typedef struct HAPI_RM_OPEN_VHD_INFO
{
   HAPI_UINT16 usrHandle;
   HAPI_UINT16 vhdType;
   HAPI_UINT16 ipcInfo[3];

} HAPI_RM_OPEN_VHD_INFO;

/*
** Structure used in HAPI_RM_DSP_BUFFER_QUERY_CMD
*/
typedef struct DSPBUFSTATS
{
   HAPI_UINT16 dspNumber;
   HAPI_UINT16 hapiMsgBufPoolnumFree;
   HAPI_UINT16 hapiMsgBufPoollowWater;
   HAPI_UINT16 hapiExtDataBufPoolnumFree;
   HAPI_UINT16 hapiExtDataBufPoollowWater;
   HAPI_UINT16 hapiBigExtDataBufPoolnumFree;
   HAPI_UINT16 hapiBigExtDataBufPoollowWater;

} HAPI_RM_DSPBUFSTATS;

/*
** Structure used in HAPI_RM_DSPMEMORY_QUERY_CMD
*/
typedef struct HAPIRMMEMSTATS
{
   HAPI_UINT16 dspNumber;
   HAPI_UINT16 numScratchLeft;
   HAPI_UINT16 numStackLeft;

} HAPI_RM_MEMSTATS;

/*
** Structure used in HAPI_RM_CFG_PULSE_SIGNAL_CMD

*/
typedef struct PULSESIGNALPARMS
{
   HAPI_UINT16 pulseLineHdl;     /* PXD index to generate the pulse on */
   HAPI_UINT16 pulseFrequency;  /* Pulse frequency (in kHz) */
   HAPI_UINT16 pulseAmplitude;  /* Pulse amplitude (in dB) */
   HAPI_UINT16 pulseDuration;   /* Pulse duration (in ms) */
   HAPI_UINT16 pulsePeriod;     /* Pulse period (in ms) */
   HAPI_UINT16 pulseCount;      /* Number of times to generate the pulse */

} HAPI_RM_PULSE_SIGNAL_PARMS;

/*
 * Following is used to decide whether IO injection overwrites
 * or adds to APM data
 */
typedef enum
{
   SWB_INJECT_OVERWRITE = 0, /* Overwrite APM data */
   SWB_INJECT_ADD            /* Add to APM data    */

} SWB_INJECT_MODE;

typedef enum
{
   HAPI_RM_ERR_UNKNOWN_VHD = 1,  /* Couldn't find the indicated VHD        */
   HAPI_RM_ERR_UNKNOWN_STACK,    /* Couldn't find the indicated stack Type */
   HAPI_RM_ERR_UNKNOWN_SRV,      /* Couldn't find the indicated service    */
   HAPI_RM_ERR_UNKNOWN_PXD,      /* Couldn't find the indictaed Pxd        */
   HAPI_RM_ERR_UNKNOWN_DEV,      /* Couldn't find the indicated device     */
   HAPI_RM_ERR_INVALID_HANDLE,   /* Invalid handle provided                */
   HAPI_RM_ERR_VHD_ALREADY_OPEN, /* Indicated handle is already open       */
   HAPI_RM_ERR_VHD_OPEN_FAILED,  /* Unable to open the VHD                 */
   HAPI_RM_ERR_SWB_FULL,         /* SwitchBoard is full                    */
   HAPI_RM_ERR_NO_SOCKET_MEM,    /* Unable to allocate memory for socket   */
   HAPI_RM_ERR_NO_MSG_MEM,       /* Ran out of message buffers             */
   HAPI_RM_ERR_NO_DATA_MSG_MEM,  /* Ran out of data message buffers        */
   HAPI_RM_ERR_NO_MSG_EXT_MEM,   /* Ran out of buffers for Msg Ext Data    */

   /*
   ** Error codes from for hapi layer
   */
   HAPI_RM_ERR_HHAPI_COMM_ERROR         = 0x0100,
   HAPI_RM_ERR_HHAPI_DSP_NOT_READY      = 0x0200,
   HAPI_RM_ERR_HHAPI_CBK_REG_FAIL       = 0x0300,
   HAPI_RM_ERR_HHAPI_BAD_CBK            = 0x0400,
   HAPI_RM_ERR_HHAPI_BAD_VHDHDL         = 0x0500,
   HAPI_RM_ERR_HHAPI_INVALID_REQID      = 0x0600,
   HAPI_RM_ERR_HHAPI_MSGQ_ERROR         = 0x0700,
   HAPI_RM_ERR_HHAPI_BAD_TX_MSG_LENGTH  = 0x0800,
   HAPI_RM_ERR_HHAPI_BAD_RX_MSG_LENGTH  = 0x0900,
   HAPI_RM_ERR_HHAPI_DSPNUM_INVALID     = 0x0a00,
   HAPI_RM_ERR_HHAPI_DSPNUM_UNAVAILABLE = 0x0b00,
   HAPI_RM_ERR_HHAPI_TCB_NONE_FREE      = 0x0c00,
   HAPI_RM_ERR_HHAPI_MSQ_SEND           = 0x0d00,
   HAPI_RM_ERR_HHAPI_DSPWRITEERROR      = 0x0e00

} HAPI_RM_ERROR_CODE;

#endif      /* HAPI_RM_H */

