/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: vrgEndptCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in vrgCfg.h
*
****************************************************************************/


#ifndef VRG_ENDPT_CFG_CUSTOM_H
#define VRG_ENDPT_CFG_CUSTOM_H

#include <globalCfg.h>


/* Number of VRG endpoints */
#define VRG_ENDPT_CFG_NUM_ENDPT              GLOBAL_CFG_MAX_ENDPT


/* Number of VRG connections */
#define VRG_ENDPT_CFG_NUM_CNX                GLOBAL_CFG_MAX_CNX


/* Number of "lite" VRG connections. Lite-connections do not support advanced
 * voice codecs. They are G.711-only connections. */
#define VRG_ENDPT_CFG_NUM_LITE_CNX           GLOBAL_CFG_MAX_LITE_CNX


/* VRG task priority. */
#define VRG_ENDPT_CFG_DISP_TASK_PRIORITY     BOS_CFG_TASK_MED_HIGH_VAL

#if VRG_TPD_SUPPORT
/* Enabled telephony port diags. */
#define VRG_ENDPT_CFG_USE_TPD                1

/* Use MLT API instead of TPD CTL */
#define VRG_ENDPT_CFG_USE_MLT                1
#endif

/* Enable Call Monitoring Tone Detection Service */
#define VRG_ENDPT_CFG_ENDPT_CMTD_SUPPORT     1
#define VRG_ENDPT_CFG_PSTN_SUPPORT           1
#define HDSP_CFG_CLIDRX                      1

#define VRG_PSTN_TEST    0

/* Enabled flexible conferencing. */
#define FLEXIBLE_CONFERENCING_SUPPORTED      1


/* Enabled packetization time check in endpoint driver */
#define VRG_ENDPT_PACKET_TIME_CHECK          1

/* disable faxr debug info output */
#define VRG_ENDPT_CFG_FAXR_DEBUG_INFO        0

/* Egress packets coming from the network are looped back in the DSP
** and sent back to the network as ingress packets */
#define VRG_ENDPT_CFG_EGRESSPACKET_LOOPBACK  0

/* Enable pre-loaded RTP payload maps (for codecs using static payload maps) */
#define VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP   1

/* Enable two-way speech path when connection is created, but the line is still on-hook. */
#define VRG_ENDPT_CFG_TWO_WAY_PATH_IN_ONHOOK    1

/* Force provisioning integrity check and return error during initialization 
** if ANY provisioning parameter is not set */
#define VRG_ENDPT_CFG_FORCE_PROV_INTEGRITY_CHECK   0

/* G168 Testing */
#ifdef G168_TEST
#define VRG_ENDPT_CFG_G168_TEST              1
#endif

#endif   /* VRG_ENDPT_CFG_CUSTOM_H */
