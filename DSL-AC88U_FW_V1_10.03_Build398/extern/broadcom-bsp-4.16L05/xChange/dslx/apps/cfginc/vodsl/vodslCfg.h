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
*    Filename: vodslCfg.h
*
****************************************************************************
*    Description:
*
*      vodsl application defines
*
****************************************************************************/

#ifndef VODSL_CFG_H
#define VODSL_CFG_H

#include <globalCfg.h>

/* These defines set the maximum physical endpoints  * 
 * in the system visible to the vodsl application    */
#define VODSL_MAX_PHYS_ENDPT     GLOBAL_CFG_MAX_ENDPT

#define VODSL_MAX_PHYS_FXS_ENDPT (GLOBAL_CFG_MAX_ENDPT-NUM_FXO_CHANNELS)

#define VODSL_MAX_PHYS_FXO_ENDPT NUM_FXO_CHANNELS      

/* This defines the number of active accounts in the vodsl application *
 * Currently set to the maximum number of physical endpoints, this     *
 * value can be increased in the case of sip applications supporting   *
 * multiple sip account to physical account mappings                   */
#define VODSL_MAX_VOIP_ENDPT     GLOBAL_CFG_MAX_ENDPT  

/* This define is used by the the gui/cli to determin how many codecs   * 
 * to display on the gui/cli. Also used by sip application to determine * 
 * size of preferred codec list                                         */                                                                     
#define VODSL_MAX_PREF_ENCODER   6

#define VODSL_MAX_CNX            GLOBAL_CFG_MAX_CNX

/* This defines the number of service providers that can be configured *
 * Currently set to the maximum number of VOIP endpoints because there *
 * can only be as many voice profiles as there are SIP accounts        */
#define VODSL_MAX_VOICE_PROFILE  VODSL_MAX_VOIP_ENDPT

#define DECT_MODULE_PATH "/lib/modules/2.6.30/extra/dect.ko"
//Todo: Add support for custom config
//#include <vodslCfg_custom.h>

#endif /* VODSL_CFG_H */


