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
******************************************************************************
*
*  Description:
*     STUNC test client header
*
*****************************************************************************/

#ifndef NATTRVRSLTEST_H
#define NATTRVRSLTEST_H

/* ---- Include Files ----------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------- */
#define NATTRVRSLTEST_ADDR_BUFFER_SIZE     100
#define NATTRVRSLTEST_SDP_BUFFER_SIZE      256

/* ---- Variable Externs -------------------------------------------------- */
extern char gLocalHost[];
extern int gTlsPort;
extern int gTurnUdpPort;
extern int gTurnTcpPort;
extern int gTurnTlsPort;
extern int gUdpPort[];
extern int gStuncStart;
extern int gTurncStart;
extern int gIceStart;
extern int gSyncEvt;
extern char gTurnHost[];
extern int gTurnPort;
extern char gStunHost[];
extern int gStunPort;
extern int gStunMaxWait;
extern int gBandwidth;
extern int gLifetime;
extern int gReqdTransport;
extern char gReqdIPAddr[];
extern int gReqdPort;
extern int gPortProps;
extern char gActiveDestIPAddr[];
extern int gActiveDestPort;
extern char gCnxRemoteIPAddr[];
extern int gCnxRemotePort;
extern char gSendDataRemoteIPAddr[];
extern int gSendDataRemotePort;
extern int gDataLen;
extern int gTurnOrgTransport;
extern int gAllocateHdl;

/* ICE specific variables */
extern int gIceAgentId;
extern int gIcePrefLocal;
extern int gIcePrefHost;
extern int gIcePrefSrflx;
extern int gIcePrefPrflx;
extern int gIcePrefRelay;
extern int gIceRtpPort[];
extern int gIceRtcpPort[];
extern int gIceBRtp[];
extern int gIceBRtcp[];
extern int gIcePrefOverride;
extern int gIceLiteMode;
extern int gIceAggressiveNom;
extern int gIceMaxWait;
extern int gIceSdpAgentId;
extern int gIceSdpRole;
extern int gIceRSdpAgentId;
extern int gIceRSdpRole;

extern char gSdpLine1[];
extern char gSdpLine2[];
extern char gSdpLine3[];
extern char gSdpLine4[];

extern "C"
{
extern int gTurnctest_AllocateError;
extern int gTurnctest_ActiveDestError;
extern int gTurnctest_ConnectError;
extern int gStuntest_BindingError;
extern int gStuntest_SharedSecretError;
}

/* ---- Macros ------------------------------------------------------------ */

/* ---- Function Prototypes ----------------------------------------------- */
extern void nattrvrsltestInit( void );
extern void nattrvrsltestStart( void );
extern void nattrvrsltestEnd( void );
extern void nattrvrsltestStuncClearUsrPwdCache( void );
extern void nattrvrsltestShowUsrPwdCache( void );
extern void nattrvrsltestStuncSharedSecretReq( void );
extern void nattrvrsltestStuncBindingReq( void );
extern void nattrvrsltestTurncAllocateReq( void );
extern void nattrvrsltestTurncActiveDestReq( void );
extern void nattrvrsltestTurncConnectReq( void );
extern void nattrvrsltestTurncSendDataReq( void );
extern void nattrvrsltestTurncDeallocate( void );
extern void nattrvrsltestTurncReceiveData( void );
extern void nattrvrsltestTurncConnectStatusInd( void );
extern void nattrvrsltestIceGatherCndts( void );
extern void nattrvrsltestIceAgentDump( void );
extern void nattrvrsltestIcetestCreateAgent( void );
extern void nattrvrsltestIceSetPortParms( void );
extern void nattrvrsltestIceDeleteAgent( void );
extern void nattrvrsltestIceSdpEncode( void );
extern void nattrvrsltestIceSdpParse( void );
extern void nattrvrsltestIceDLogSet( void );


#endif

