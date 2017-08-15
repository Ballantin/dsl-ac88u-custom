/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Description:
*     HausWare API object identifiers.
*
****************************************************************************/

#ifndef HAPIIDS_H
#define HAPIIDS_H

/* ---- Include Files ---------------------------------------- */

/* ---- Constants and Types ---------------------------------- */

/* HAPI IDs */
typedef enum
{
   /* Services */
   HAPI_NULL_SRV        = 0x00, /* Reserved service id */
   HAPI_RSVD0_SRV       = 0x01, /* Reserved - free for the taking */
   HAPI_CODEC_SRV       = 0x02, /* Distributed PVE codec srv on the DSP */
   HAPI_PVE_SRV         = 0x03,
//Unused   HAPI_AAL2TR_SRV      = 0x04, /* AAL2 Trunking Service (i.e. I.366.2) */
//Unused   HAPI_AAL2SAR_SRV     = 0x05, /* AAL2 SAR Service (i.e. I.366.1) */
   HAPI_RTP_SRV         = 0x06,
   HAPI_CDIS_SRV        = 0x07,
   HAPI_CLIDTX_SRV      = 0x08,
   HAPI_PTE_SRV         = 0x09,
   HAPI_HSD_SRV         = 0x0a,
//Unused   HAPI_LED_SRV         = 0x0b,
//Unused   HAPI_V21D_SRV        = 0x0c,
   HAPI_V21_SRV         = 0x0d,
   HAPI_V34_SRV         = 0x0e,
   HAPI_V8_SRV          = 0x0f,
//Unused   HAPI_MT_SRV          = 0x10,
   HAPI_VFAX_SRV        = 0x11,
   HAPI_FAXR_SRV        = 0x12,
   HAPI_T30_SRV         = 0x13,
//Unused   HAPI_CASM_SRV        = 0x14,
   HAPI_UI4H_SRV        = 0x15,           // unused, but hapi_ui4h.h still exists, so this is needed for makedefs
//Unused   HAPI_AAL2OVSUB_SRV   = 0x16,
   HAPI_T38_SRV         = 0x17,
//Unused   HAPI_HDLC_SRV        = 0x18, /* HDLC service */
   HAPI_LDX_SRV         = 0x19, /* LDX (Low Density xChange) pseudo service */ // unused, but hapi_ldx.h still exists, so this is needed for makedefs
   HAPI_CMTD_SRV        = 0x1a,  /* Call Monitoring Tone Detector service */
//Unused   HAPI_MIX_SRV         = 0x1b,
//Unused   HAPI_CONF_SRV        = 0x1c,
//Unused   HAPI_REDIRECTOR_SRV  = 0x1d,   /* spicebus redirector */
//Unused   HAPI_MFR2D_SRV       = 0x1e,
//Unused   HAPI_AALMI_SRV       = 0x1f,
//Unused   HAPI_TESTLINE_SRV    = 0x20,
//Unused   HAPI_PTP_SRV         = 0x21,
//Unused   HAPI_AAL1TDM_SRV     = 0x22, /* bottom of stack AAL1 service */
//Unused   HAPI_MLL_SRV         = 0x23, /* modem link layer service */
//Unused   HAPI_TESTMT_SRV      = 0x24, /* service for testing modem termination */
//Unused   HAPI_BERT_SRV        = 0x25, /* Bit Error Rate Test service */
//Unused   HAPI_AAL1NET_SRV     = 0x26, /* top of stack AAL1 service */
//Unused   HAPI_L2TP_SRV        = 0x27, /* Layer 2 Tunneling Protocol (L2TP) service */
//Unused   HAPI_RSVD3_SRV       = 0x28, /* Reserved - free for the taking */
//Unused   HAPI_XCODE_SRV       = 0x29, /* Transcoder service */
//Unused   HAPI_RTPUDP_SRV      = 0x2a, /* Minimalist RTP/UDP/IP service */
//Unused   HAPI_ICON_SRV        = 0x2b, /* Interconnect test service */
//Unused   HAPI_RTPBOS_SRV      = 0x2c,
//Unused   HAPI_UI4HBOS_SRV     = 0x2d,
   HAPI_PCM_SRV         = 0x2e,
   HAPI_V22B_SRV        = 0x2f,
   HAPI_NET_SRV         = 0x30,
   HAPI_V23_SRV         = 0x31,
   HAPI_V26T_SRV        = 0x32,
   HAPI_V32B_SRV        = 0x33,
   HAPI_CLIDRX_SRV      = 0x34,
   HAPI_BSC_SRV         = 0x35,  /* band splitter/combiner */
   HAPI_LINE_SRV        = 0x36,  /* Line service */
   HAPI_AGC_SRV         = 0x37,  /* AGC service */

   /* PxDs */
   HAPI_GAIN_SRV        = 0x38,  /* Gain stages */
   
   HAPI_ECAN_SRV        = 0x3a,  /* line ECAN */ 
//Unused   HAPI_FDS_PXD         = 0x3b,  /* full duplex speakerphone (AEC) */
   HAPI_HPF_PXD         = 0x3c,  /* highpass filter */
   HAPI_CALIB_PXD       = 0x3e,  /* calibration */
   HAPI_SDTN_PXD        = 0x3f,  /* sidetone */
   HAPI_EQU_PXD         = 0x40,  /* equalization */

//Unused   HAPI_TEST1_PXD,
   HAPI_TESTMSG_SRV,
   HAPI_CONF_TOS_SRV    = 0x41,  /* Top of stack (TOS) conference service */
   HAPI_CONF_BOS_SRV    = 0x42,  /* Bottom of stack (BOS) conference service */
   HAPI_AEC_SRV         = 0x43,  /* new AEC */ 
   HAPI_HEC_SRV         = 0x44,  /* full duplex speakerphone (AEC) */
   HAPI_DTAS_SRV        = 0x45,  /* DTAS detection service */

   /* Risc engine services run from 0x80 upwards */
   HAPI_RM_SRV          = 0x80, /* Resource Manager service */
//Unused   HAPI_PROF_SRV        = 0x81, /* CPU profiling service */
//Unused   HAPI_CAPTURE_SRV     = 0x82, /* Channel Capture service */
   HAPI_ICP_SRV         = 0x83, /* Idle CPU Profiler Service */
   HAPI_DPR_SRV         = 0x84, /* Development Profiler Service */
   HAPI_SWB_SRV         = 0x85, /* Switchboard service */
   HAPI_HALSHIM_SRV     = 0x86, /* HAL Shim */
   HAPI_USER_SRV        = 0x87, /* Custom User Service */

   HAPI_INTERNAL_ID     = 0x88, /* Reserved internal ID */

   /* Reserved service ID that does not require responses.
    * This ID would be useful to internal framework functions that
    * send messages to the RM, for example, but do not
    * require responses to be generated. */
   HAPI_NO_RESP_SRV     = 0xff

} HAPISRVID;

/* Service ID aliases */
#define HAPI_HOST_SRV      HAPI_NULL_SRV


/* Channel stack definitions */
typedef enum
{
   HAPI_VOICE_STACK        = 0x01,
   HAPI_FAX_STACK          = 0x02,
//Unused   HAPI_MT_STACK           = 0x03,
//Unused   HAPI_REDIRECTOR_STACK   = 0x04,
//Unused   HAPI_AALMI_STACK        = 0x05,
//Unused   HAPI_TESTLINE_STACK     = 0x06,
//Unused   HAPI_AAL1NET_STACK      = 0x07,
//Unused   HAPI_XCODE_STACK        = 0x08,
   HAPI_TONE_STACK         = 0x09,
//Unused   HAPI_NULL_STACK         = 0x0a,
   HAPI_PSTN_STACK         = 0x0b,
   HAPI_LINE_AEC_STACK     = 0x0c,
   HAPI_LINE_ECAN_STACK    = 0x0d

} HAPISTACKID;

#endif /* HAPIIDS_H */
