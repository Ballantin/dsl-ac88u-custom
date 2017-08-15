/**********************************************************************************
** Copyright © 2007-2009 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file contains the configuration verification logic for cctkCfg.h.
**
***********************************************************************************/

#ifndef __CCTK_CFG_CHK__H__INCLUDED__
#define __CCTK_CFG_CHK__H__INCLUDED__

#if ( CCTKCFG_CCTK_MODE < CCTKCFG_CCTK_SIP ||                                      \
      CCTKCFG_CCTK_MODE > CCTKCFG_CCTK_IMS )
   #error CCTKCFG_CCTK_MODE is defined with an invalid macro
#endif

#if ( ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS ) &&                                 \
      ( !CCTKCFG_NETWORK_ASSERTED_ID ||                                            \
        !CCTKCFG_PRIVACY ||                                                        \
        CCTKCFG_TEL_MODE == CCTKCFG_TEL_DISABLED ) )
   #error Other features are necessary for enabling CCTKCFG_CCTK_IMS
#endif

#if ( CCTKCFG_TEL_MODE < CCTKCFG_TEL_DISABLED ||                                   \
      CCTKCFG_TEL_MODE > CCTKCFG_TEL_WITHOUT_ENUM )
   #error CCTKCFG_TEL_MODE is defined with an invalid macro
#endif

#if ( CCTKCFG_DEBUG &&                                                             \
      CCTKCFG_ASSERT_MODE == CCTKCFG_ASSERT_DISABLED )
   #error CCTKCFG_ASSERT_MODE must be enabled to enable CCTKCFG_DEBUG
#endif

#if ( CCTKCFG_ASSERT_MODE < CCTKCFG_ASSERT_DISABLED ||                             \
      CCTKCFG_ASSERT_MODE > CCTKCFG_ASSERT_CCTK )
   #error CCTKCFG_ASSERT_MODE is defined with an invalid macro
#endif

#if ( CCTKCFG_TRACE_MODE < CCTKCFG_TRACE_DISABLED ||                               \
      CCTKCFG_TRACE_MODE > CCTKCFG_TRACE_CCTK )
   #error CCTKCFG_TRACE_MODE is defined with an invalid macro
#endif

#if ( CCTKCFG_MAX_HOSTADDR < 1 )
   #error CCTKCFG_MAX_HOSTADDR must be at least 1
#endif

#if ( CCTKCFG_MAX_ACCESSNET < 1 )
   #error CCTKCFG_MAX_ACCESSNET must be at least 1
#endif

#if ( CCTKCFG_MAX_OBPROXY < 1 )
   #error CCTKCFG_MAX_OBPROXY must be at least 1
#endif

#if ( CCTKCFG_MAX_REALM < 1 )
   #error CCTKCFG_MAX_REALM must be at least 1
#endif

#if ( CCTKCFG_MAX_ADDRLST < 1 )
   #error CCTKCFG_MAX_ADDRLST must be at least 1
#endif

#if ( CCTKCFG_MAX_GENHDRLST < 1 )
   #error CCTKCFG_MAX_GENHDRLST must be at least 1
#endif

#if ( CCTKCFG_MAX_PROFCFGLST < 1 )
   #error CCTKCFG_MAX_PROFCFGLST must be at least 1
#endif

#if ( CCTKCFG_MAX_USR < 1 )
   #error CCTKCFG_MAX_USR must be at least 1
#endif

#if ( CCTKCFG_MAX_CALL < 1 )
   #error CCTKCFG_MAX_CALL must be at least 1
#endif

#if ( CCTKCFG_MAX_EVT < 1 )
   #error CCTKCFG_MAX_EVT must be at least 1
#endif

#if ( CCTKCFG_MAX_NOT < 1 )
   #error CCTKCFG_MAX_NOT must be at least 1
#endif

#if ( CCTKCFG_MAX_CDR < CCTKCFG_MAX_USR )
   #error CCTKCFG_MAX_CDR must be at least equal to CCTKCFG_MAX_USR
#endif

#if ( CCTKCFG_MAX_AUDIO_CODEC < 1 )
   #error CCTKCFG_MAX_AUDIO_CODEC must be at least 1
#endif

#if CCTKCFG_VIDEO
#if ( CCTKCFG_MAX_VIDEO_CODEC < 0 )
   #error CCTKCFG_MAX_VIDEO_CODEC must be at least 0
#endif
#endif

#if ( CCTKCFG_MAX_CRYPTO < 0 )
   #error CCTKCFG_MAX_CRYPTO must be at least 0
#endif

#if ( CCTKCFG_MAX_CRYPTO_SESPARM < 0 )
   #error CCTKCFG_MAX_CRYPTO_SESPARM must be at least 0
#endif

#if ( CCTKCFG_MAX_ASSUMED_ID < 0 )
   #error CCTKCFG_MAX_ASSUMED_ID must be at least 0
#endif

#if ( CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS < 2 )
   #error CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS must be at least 2
#endif

#if ( CCTKCFG_MAX_ALERT_INFO < 1 )
   #error CCTKCFG_MAX_ALERT_INFO must be at least 1
#endif

#if ( CCTKCFG_RES_STATE_DEPTH < 2 )
   #error CCTKCFG_RES_STATE_DEPTH must be at least 2
#endif

#if ( CCTKCFG_RES_EVENT_DEPTH < 2 )
   #error CCTKCFG_RES_EVENT_DEPTH must be at least 2
#endif

#if ( CCTKCFG_RES_FSM_DEPTH < 2 )
   #error CCTKCFG_RES_FSM_DEPTH must be at least 2
#endif


#endif /* __CCTK_CFG_CHK__H__INCLUDED__ */
