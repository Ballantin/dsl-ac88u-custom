/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom Corporation
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
**      This file implements the STUN support within the Call Control Toolkit.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <nsNatStun.h>
#include <nsNatKa.h>

#if CCTKCFG_KEEP_ALIVE

/* ---- Public Variables -------------------------------------------------------- */
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
extern CCTKTRACEHDLR gpCctkTraceCb;
#endif

/* ---- Private Constants and Types --------------------------------------------- */
#define NS_NAT_STUN__TRACE_MSG_LEN 256

/* ---- Private Variables ------------------------------------------------------- */
NSNATSTUNBLK *NsNatStun::mpNsNatReq[NS__NAT_STUN__MAX_CONCURENT_REQUESTS];
NsNatKa *NsNatStun::mpNsNatKa;


/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   NsNatStun::NsNatStun
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
NsNatStun::NsNatStun( NsNatKa *gpNatKa )
{
   STUNC_CFG stunClientCfg; 
   STUN_CFG stunCfg;
   BOS_UINT32 uIx;

   mpNsNatKa = gpNatKa;

   for ( uIx = 0 ; uIx < NS__NAT_STUN__MAX_CONCURENT_REQUESTS ; uIx++ )
   {
      mpNsNatReq[uIx] = BRCM_NEW( NSNATSTUNBLK );
      if ( mpNsNatReq[uIx] )
      {
         mpNsNatReq[uIx]->uResGuid = guCeInvalidGuid;
         mpNsNatReq[uIx]->uStunHdl = guCeInvalidGuid;
      }
   }

   stunCfg.crc32CB = NsNatStun::EvStunCrc32;
   stunCfg.hmacCB = NsNatStun::EvStunHmac;
   stunCfg.logCB = NsNatStun::EvStunLog;
   stunCfg.rand32CB = NsNatStun::EvStunRand32;
   stunCfg.sendPktCB = NsNatStun::EvStunSendPtk;

   stunClientCfg.pStunCfg = &stunCfg;
   
   if ( stuncStartup( &stunClientCfg ) == STUNC_FAILURE )
   {
      CCTK_TRACEI0( "NsNatStun - Stun client failure" );
   }
   else
   {
      CCTK_TRACED0( "NsNatStun - Stun client startup" );
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::~NsNatStun
**
**  PURPOSE:    Class Destructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
NsNatStun::~NsNatStun()
{
   BOS_UINT32 uIx;

   mpNsNatKa = NULL;

   stuncShutdown();   

   for ( uIx = 0 ; uIx < NS__NAT_STUN__MAX_CONCURENT_REQUESTS ; uIx++ )
   {
      if ( mpNsNatReq[uIx] )
      {
         BRCM_DELETE( mpNsNatReq[uIx] );
         mpNsNatReq[uIx] = NULL;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::nsNatStunBind
**
**  PURPOSE:    Issue a STUN binding request on behalf of the resource.
**
**  PARAMETERS:
**              uResGuid - the resource GUID for which we need to send this keep
**                         alive binding request.
**              uSockGuid - the 'socket' GUID associated with this resource (in
**                          essence this identified the socket on which the STUN
**                          binding is to be sent).
**              eType - the type of binding requested.  This dictates how to
**                      interpret the result of the actual STUN action since it
**                      may differ based on different intended use.
**              uMaxTx - maximum number of STUN transmissions before declaring
**                       failure
**
**
**  RETURNS:    Nothing
**
**  NOTES:      Currently, we only handle 'keep alive' type of binding.
***********************************************************************************/
void NsNatStun::nsNatStunBind( BOS_UINT32 uResGuid, BOS_UINT32 uSockGuid,
                               NSNATSTUN eType, BOS_UINT32 uMaxTx )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx = guCeInvalidGuid;

   if ( eType != eNSNATSTUN_KEEPALIVE )
   {
      /* Exit now for binding we do not support.
      */
      return;
   }

   for ( uIx = 0 ; uIx < NS__NAT_STUN__MAX_CONCURENT_REQUESTS ; uIx++ )
   {
      if ( (mpNsNatReq[uIx]->uResGuid == guCeInvalidGuid) &&
           (mpNsNatReq[uIx]->uStunHdl == guCeInvalidGuid) &&
           (uJx == guCeInvalidGuid) )
      {
         uJx = uIx;
         break;
      }
   }
   
   if ( uJx != guCeInvalidGuid  )
   {
      BOS_UINT32 uReqHdl;
      STUN_TRANSPORT dest; /* Not important. */
      STUN_RETRANS   reTx;

      memset ( &reTx,
               0,
               sizeof ( STUN_RETRANS ) );

#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
      reTx.uRc = (BOS_UINT8)uMaxTx;
#endif

      bosIpAddrCreateZero( BOS_IPADDRESS_TYPE_V4,
                           &dest.ipAddr );
      dest.port = 0;

      if ( stuncPublicAddr( uSockGuid,
                            &dest,
                            NULL,       /* pSecret */
                            0,          /* options */
                            &uReqHdl,
                            EvStunCb,
                            NsNatStun::EvStunSendPtk,
                            &reTx
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                            ,NULL       /* pEvtData */
#endif
#if NTCFG_ICE_SUPPORT
                            ,0
                            ,0
#endif
                          ) != STUNC_SUCCESS )
      {
         mpNsNatKa->nsNatKaFailureA( uResGuid );
      }
      else
      {
         mpNsNatReq[uIx]->uResGuid = uResGuid;
         mpNsNatReq[uIx]->uStunHdl = uReqHdl;
         mpNsNatReq[uIx]->eType = eNSNATSTUN_KEEPALIVE;
      }
   }
   else
   {
      mpNsNatKa->nsNatKaFailureA( uResGuid );
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::nsNatStunIsStun
**
**  PURPOSE:    Function used to check whether the content of the buffer passed
**              is a STUN message or not.
**
**  PARAMETERS:
**              puData - pointer to the data buffer.
**              uLen - size of the data in the buffer.
**
**
**  RETURNS:    BOS_TRUE if the data pointed to by puData is a STUN message.
**              BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsNatStun::nsNatStunIsStun( BOS_UINT8 *puData,
                                     BOS_UINT32 uLen )
{
   return stunIsStunMsg( puData,
                         static_cast<BOS_UINT16> (uLen),
                         STUN_INSP_BASIC );
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::nsNatStunProcess
**
**  PURPOSE:    Function invoked when a STUN message data has been received and
**              need to be processed.
**
**  PARAMETERS:
**              uSockGuid - the identifier for the receiver of this packet.
**              puData - pointer to the data buffer.
**              uLen - size of the data in the buffer.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      Upon processing of this message, the registered callback for STUN
**              module result will be invoked.  This is where the final steps are
**              taken as far as resolution of the STUN request engaged.
***********************************************************************************/
void NsNatStun::nsNatStunProcess( BOS_UINT32 uSockGuid,
                                  BOS_UINT8 *puData,
                                  BOS_UINT32 uLen )
{
   STUN_TRANSPORT transport;  /* Not important. */

   bosIpAddrCreateZero( BOS_IPADDRESS_TYPE_V4,
                        &transport.ipAddr );
   transport.port = 0;

   stunRecvPacket( uSockGuid,
                   &transport,
                   &transport,
                   puData,
                   static_cast<BOS_UINT16> (uLen) );
}


/* ------------------------------------------------------------------------------ */
/* ---- BELOW THIS POINT, THE CALLBACK INTERFACE FROM THE STUN IS IMPLEMENTED --- */
/* ------------------------------------------------------------------------------ */

/***********************************************************************************
**  FUNCTION:   NsNatStun::EvStunCb
**
**  PURPOSE:    Callback from the STUN module following an action engaged on
**              behalf of the user.
**
**  PARAMETERS:
**              reqHdl  - request handle used to match the response to a query.
**              pEvtData - event information.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void NsNatStun::EvStunCb( const BOS_UINT32 reqHdl,
                          const STUNC_EVT_DATA *pEvtData )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx = guCeInvalidGuid;

   for ( uIx = 0 ; uIx < NS__NAT_STUN__MAX_CONCURENT_REQUESTS ; uIx++ )
   {
      if ( (mpNsNatReq[uIx]->uResGuid != guCeInvalidGuid) &&
           (mpNsNatReq[uIx]->uStunHdl == reqHdl) )
      {
         uJx = uIx;
         break;
      }
   }

   /* Match the request callback to an existing request, process the
   ** response accordingly.
   */
   if ( uJx != guCeInvalidGuid )
   {
      switch( pEvtData->evt )
      {
         case STUNC_EVT_TX_TIMEOUT:
         case STUNC_EVT_RES_ERROR:
         case STUNC_EVT_SYSTEM_ERR:
         {
            if ( mpNsNatReq[uJx]->eType == eNSNATSTUN_KEEPALIVE )
            {
               /* This is seen as a keep alive failure.
               */
               if ( mpNsNatKa )
               {
                  mpNsNatKa->nsNatKaFailureA( mpNsNatReq[uJx]->uResGuid );
               }
            }
         }
         break;

         case STUNC_EVT_MAPPED_ADDRESS:
         case STUNC_EVT_SHARED_SECRET:
         default:
         {
            if ( mpNsNatReq[uJx]->eType == eNSNATSTUN_KEEPALIVE )
            {
               /* This is seen as a keep alive success.
               */
               if ( mpNsNatKa )
               {
                  mpNsNatKa->nsNatKaSuccessA( mpNsNatReq[uJx]->uResGuid );
               }
            }
         }
         break;
      }

      /* Release the STUN control block so it can be re-used later on.
      */
      mpNsNatReq[uJx]->uResGuid = guCeInvalidGuid;
      mpNsNatReq[uJx]->uStunHdl = guCeInvalidGuid;
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::EvStunSendPtk
**
**  PURPOSE:    Callback from the STUN module when a packet needs to be sent to
**              a given destination.
**
**  PARAMETERS:
**              sid     [IN] - socket id
**              pDest   [IN] - destination IP and port
**              pData   [IN] - pointer to data to be sent
**              len     [IN] - length of data in number of bytes
**
**
**  RETURNS:    Nothing
**
**  NOTES:      Currently, only the keep alive mechanism is making use of the
**              STUN processing, so any data to be sent by the STUN processing is
**              automatically relayed to the keep alive.  This may not be true in
**              the future (when full ICE is supported), but it is valid for now.
***********************************************************************************/
void NsNatStun::EvStunSendPtk( const BOS_UINT32      sid,
                               const STUN_TRANSPORT  *pDest,
                               const BOS_UINT8       *pData,
                               const BOS_UINT16      len )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pDest );

   /* Pass the information directly to the keep alive processing for sending.
   */
   if ( mpNsNatKa )
   {
      mpNsNatKa->nsNatKaSendA( sid,
                               pData,
                               len );
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::EvStunRand32
**
**  PURPOSE:    Callback from the STUN module for generating 32bit random values.
**
**  PARAMETERS:
**              pRand [OUT] - the generated random value
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void NsNatStun::EvStunRand32( BOS_UINT32 *pRand )
{
   *pRand = (BOS_UINT32)rand();
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::EvStunHmac
**
**  PURPOSE:    Callback from the STUN module for generating hashed data based
**              on the information provided.
**
**  PARAMETERS:
**              alg    [IN] - hmac algorithm
**              pKey   [IN] - hash key, keylen
**              pData  [IN] - data value and length
**              pHmac [OUT] - the hashed data
**
**
**  RETURNS:    Nothing
**
**  NOTES:      Required for supporting shared-secret and integrity check
**              operations
***********************************************************************************/
void NsNatStun::EvStunHmac( const STUN_HMAC_TYPE alg,
                            const STUN_DATA      *pKey,
                            const STUN_DATA      *pData,
                            STUN_DATA            *pHmac )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( alg );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pKey );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pHmac );
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::EvStunHmac
**
**  PURPOSE:    Callback from the STUN module for performing CRC32 check on input
**              data.
**
**  PARAMETERS:
**              pData [IN] - data
**              len   [IN] - data length
**
**
**  RETURNS:    Calculated CRC.
**
**  NOTES:      Required when using STUN_INSP_FINGERPRINT.
***********************************************************************************/
BOS_UINT32 NsNatStun::EvStunCrc32( const BOS_UINT8   *pData,
                                   const BOS_UINT16  len )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( len );

   return 0;
}


/***********************************************************************************
**  FUNCTION:   NsNatStun::EvStunLog
**
**  PURPOSE:    Callback from the STUN module with log information.
**
**  PARAMETERS:
**              pFormat [IN] - format string
**              ...    [IN] - data
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void NsNatStun::EvStunLog( const char *pFormat, ... )
{
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_CCTK )
   /* Always assume those logs are debug ones.
   */
   BOS_UINT16 uLevel = CCTK_TRACELVL_DEBUG;

   if ( gpCctkTraceCb != NULL &&
        pFormat != NULL &&
        strlen( pFormat ) > 0 &&
        UTIL_BMP_IS_ENABLED( uLevel, CFG_SYS->xtl.loc.debug.trace.uLevel ) )
{
      va_list argList;
      BOS_UINT8 cMsg[NS_NAT_STUN__TRACE_MSG_LEN + 1];

      va_start( argList, pFormat );
      vStrPrintf( reinterpret_cast< char* >( cMsg ),
                  NS_NAT_STUN__TRACE_MSG_LEN,
                  pFormat,
                  argList );
      cMsg[NS_NAT_STUN__TRACE_MSG_LEN] = '\0';

      ( *gpCctkTraceCb )( uLevel, CCTK_TRACEGRP_STUN, cMsg );
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pFormat );
#endif
}


#endif /* CCTKCFG_KEEP_ALIVE */
