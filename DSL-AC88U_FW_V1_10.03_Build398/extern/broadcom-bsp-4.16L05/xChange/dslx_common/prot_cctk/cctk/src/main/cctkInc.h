/**********************************************************************************
** Copyright (c) 2011-2012 Broadcom
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
**      This file regroups all the necessary include files shared amongst all
**      the CCTK components, it is a single point of entry for ease of usage.
**
***********************************************************************************/
#ifndef __CCTKINC__H__INCLUDED__
#define __CCTKINC__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */

/* Configuration and generic includes (standard library, etc...).
**
** Note that MxConfig MUST preceed all other includes.
*/
#include <Config/MxConfig.h>
#include <Config/FrameworkCfg.h>
#include <Basic/MxMacro.h>

#include <stdlib.h>
#include <bosCfgCustom.h>
#include <bosTypes.h>
#include <bosMutex.h>
#include <bosTime.h>
#include <str.h>

/* Define the M5T namespace prior to any CCTK include.
*/
#define MX_NS                    MX_NAMESPACE(MXD_GNS)

#include <cctkCfg.h>
#include <cctkMainCfg.h>
#include <cctkExtCfg.h>

#if ( CCTKCFG_ASSERT_MODE == CCTKCFG_ASSERT_CCTK )
#include <assert.h>
#endif

/* SIP stack (M5T) includes.
*/
#include <Basic/MxEventNotifier.h>

#include <Cap/CSubAllocator.h>

#include <Crypto/CBase64.h>
#include <Crypto/CPrivateKey.h>
#include <Crypto/CUuidGenerator.h>

#include <Custom/SipTransport/CBrcmTransportSvc.h>

#include <ECom/CEComDelegatingUnknown.h>

#include <Network/NetworkTools.h>
#include <Network/CUdpSocket.h>
#include <Network/CTcpSocket.h>
#include <Network/CTcpServerSocket.h>
#include <Network/IAsyncSocket.h>
#include <Network/IAsyncIoSocket.h>
#include <Network/IAsyncUnconnectedIoSocket.h>
#include <Network/CAsyncSocketFactory.h>
#include <Network/MxAsyncSocketConstants.h>
#include <Network/IAsyncSocketQualityOfServiceOptions.h>

#include <Pki/CCertificate.h>

#include <SceCore/EComCLSID.h>
#include <SceCore/ISceCoreConfig.h>
#include <SceCore/ISceRequestHandler.h>
#include <SceCore/ISceShutdownMgr.h>

#include <SceCoreComponents/EComCLSID.h>
#include <SceCoreComponents/ISceBasicExtensionControl.h>
#include <SceCoreComponents/ISceBasicExtensionMgr.h>
#include <SceCoreComponents/ISceGenericEventInfo.h>
#include <SceCoreComponents/ISceGruuConfig.h>
#include <SceCoreComponents/ISceNewNotifierMgr.h>
#include <SceCoreComponents/ISceNewReferrerMgr.h>
#include <SceCoreComponents/ISceNotifier.h>
#include <SceCoreComponents/ISceNotifierHandler.h>
#include <SceCoreComponents/ISceNotifierMgr.h>
#include <SceCoreComponents/ISceOodRequestSender.h>
#include <SceCoreComponents/ISceOodRequestSenderMgr.h>
#include <SceCoreComponents/ISceReferHandler.h>
#include <SceCoreComponents/ISceReferrer.h>
#include <SceCoreComponents/ISceReferrerMgr.h>
#include <SceCoreComponents/ISceSipPacketObserver.h>
#include <SceCoreComponents/ISceSubscriber.h>
#include <SceCoreComponents/ISceSubscriberMgr.h>
#include <SceCoreComponents/ISceUaCapabilitiesConfig.h>
#include <SceCoreComponents/ISceUserAuthentication.h>
#include <SceCoreComponents/ISceUserAuthenticationMgr.h>
#include <SceCoreComponents/ISceUserConfig.h>
#include <SceCoreComponents/ISceUserIdentityConfig.h>
#include <SceCoreComponents/ISceUserSecurityConfig.h>

#include <SceMsp/IMspOfferAnswerSession.h>
#include <SceMsp/IMspOfferAnswerSessionMgr.h>

#include <SceUaSsp/EComCLSID.h>
#include <SceUaSsp/IUaSspCall.h>
#include <SceUaSsp/IUaSspCallConfig.h>
#include <SceUaSsp/IUaSspCallEventInfo.h>
#include <SceUaSsp/IUaSspCallHandler.h>
#include <SceUaSsp/IUaSspCallMgr.h>
#include <SceUaSsp/IUaSspMwi.h>
#include <SceUaSsp/IUaSspMwiConfig.h>
#include <SceUaSsp/IUaSspMwiMgr.h>
#include <SceUaSsp/IUaSspNewCallMgr.h>
#include <SceUaSsp/IUaSspRegistration.h>
#include <SceUaSsp/IUaSspRegistrationConfig.h>
#include <SceUaSsp/IUaSspRegistrationMgr.h>

#include <SceSipTypes/SceTypesConverter.h>

#include <SdpMgmt/CSdpCapabilitiesMgr.h>

#include <SdpParser/CCryptoKeyParam.h>
#include <SdpParser/CCryptoSessionParam.h>

#include <ServicingThread/CEventDriven.h>
#include <ServicingThread/EComCLSID.h>
#include <ServicingThread/IActivationService.h>

#include <SipCore/EComCLSID.h>
#include <SipCore/ISipCoreConfig.h>
#include <SipCore/ISipCoreUser.h>
#include <SipCore/ISipServerEventControl.h>
#include <SipCore/SipCoreGlobals.h>
#include <SipCore/ISipContext.h>

#include <SipCoreSvc/CServerLocator.h>
#include <SipCoreSvc/CSipConnectionBlacklist.h>
#include <SipCoreSvc/CSipPersistentConnectionList.h>
#include <SipCoreSvc/EComCLSID.h>
#include <SipCoreSvc/ISipPersistentConnectionMgr.h>
#include <SipCoreSvc/ISipServerLocationSvc.h>

#include <SipParser/Authentication.h>
#include <SipParser/CAbsoluteUri.h>
#include <SipParser/CGenParamList.h>
#include <SipParser/CMessageSummary.h>
#include <SipParser/CNameAddr.h>
#include <SipParser/CSipStatusLine.h>
#include <SipParser/CTelUri.h>
#include <SipParser/CUriFactory.h>
#include <SipParser/IUri.h>
#include <SipParser/OptionTag.h>
#include <SipParser/StatusCode.h>
#include <SipParser/UriParameter.h>
#include <SipParser/CRequestLine.h>
#include <SipParser/CRawHeader.h>

#include <SipTransaction/CSipTransaction.h>

#include <SipTransport/CSipPacket.h>
#include <SipTransport/CSipTransportMgr.h>
#include <SipTransport/ISipDataLogger.h>
#include <SipTransport/ISipTlsContextFactory.h>
#include <SipTransport/ISipTransportUser.h>
#include <SipTransport/SipTransportGlobals.h>

#include <SipUserAgent/ISipUserAgentSvc.h>

#include <Startup/CSceUaSspInitializer.h>
#include <Startup/CSdpParserInitializer.h>

#include <Tls/CTlsContext.h>

#include <Xml/CXmlElement.h>
#include <Xml/CXmlParser.h>
#include <Xml/CXmlWriter.h>
#include <Xml/EComCLSID.h>
#include <Xml/IXmlDocument.h>
#include <Xml/IXmlParserHandler.h>
#include <Xml/IXmlWriterOutputHandler.h>


/* Call Control Toolkit includes.
*/
#include <cctkApi.h>
#include <cctkVer.h>
#include <cctkMainApi.h>
#include <cfgApi.h>
#include <ceApi.h>
#include <mseApi.h>
#include <utilApi.h>
#include <pmApi.h>
#include <nsNatApi.h>
#include <nsScApi.h>
#include <nsBrApi.h>
#if CCTKCFG_ICE_LITE_SUPPORT
#include <nsIceApi.h>
#endif


/* ---- Constants and Types ----------------------------------------------------- */

/* GUID related constants.
*/
extern const BOS_UINT32 guCeInvalidGuid;
extern const BOS_UINT32 guCeGuidSeed;


/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Function Prototypes -------------------------------------------------------- */

#endif /* __CCTKINC__H__INCLUDED__ */
