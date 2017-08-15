/****************************************************************************
*
* Copyright © 2001-2008 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: bosConf_custom.h
*    Creation Date: 21 September 2001 (v0.00)
*    VSS Info:
*        $Revision: 2 $
*        $Date: 1/24/03 5:54p $
*
****************************************************************************
*    Description:
*
*      BOS custom application configuration file
*
****************************************************************************/

#ifndef BOSCFGCUSTOM_H
#define BOSCFGCUSTOM_H

/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

/* OS specification - one must be selected here in custom file */
#if defined(XCBUILD_OS_VXWORKS)
#define BOS_OS_VXWORKS        1
#elif defined(XCBUILD_OS_ECOS)
#define BOS_OS_ECOS           1
#else
#error "XCBUILD_OS_XXX not defined"
#endif

/* Module specification - select module support */
#define BOS_CFG_INIT          1
#define BOS_CFG_MUTEX         1
#define BOS_CFG_SEM           1
#define BOS_CFG_SLEEP         1
#define BOS_CFG_TASK          1
#define BOS_CFG_TIME          1
#define BOS_CFG_SOCKET        1
#define BOS_CFG_LOG           0

#define BOS_CFG_IP_ADDRESS     1
#define BOS_CFG_IP_ADDR_V6_SUPPORT  1

#endif   /* BOSCFGCUSTOM_H */
