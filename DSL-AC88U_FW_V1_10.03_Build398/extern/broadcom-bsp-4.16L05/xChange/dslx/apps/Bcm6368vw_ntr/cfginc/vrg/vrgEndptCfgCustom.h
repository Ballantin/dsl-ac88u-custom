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
#define VRG_ENDPT_CFG_EVPR_TASK_PRIORITY     MTA_TASK_DEFAULT


/* Enabled telephony port diags. */
#define VRG_ENDPT_CFG_USE_TPD                0


#define VRG_ENDPT_CFG_ENDPT_CMTD_SUPPORT     0
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

#endif   /* VRG_ENDPT_CFG_CUSTOM_H */
