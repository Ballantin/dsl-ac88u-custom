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
**      This file implements the Call Data Record resource.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceCdr.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeCdr::CeCdr
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
CeCdr::CeCdr( void )
{
   meCdrType = CeCdr::eCECDRTYPE_MAX;
   muCdrOwner = guCeInvalidGuid;

   muCdrPrevRes = guCeInvalidGuid;
   muCdrRes = guCeInvalidGuid;
   mCdrStartMs = 0;
   mCdrEndMs = 0;
   mbTgtDlg = BOS_FALSE;
   
   mCdr.timeStart = 0;
   mCdr.timeStop = 0;
   mCdr.bAnonymous = BOS_FALSE;
   mCdr.bEmergency = BOS_FALSE;
   mCdr.bAccepted = BOS_FALSE;

   memset( reinterpret_cast<void *> (mCdr.uCallId),
           0,
           sizeof( mCdr.uCallId ) );

   memset( reinterpret_cast<void *> (mCdr.uPeer),
           0,
           sizeof( mCdr.uPeer ) );

   memset( reinterpret_cast<void *> (&mCdr.clid),
           0,
           sizeof( mCdr.clid ) );         

   memset( reinterpret_cast<void *> (muLocal),
           0,
           sizeof( muLocal ) );

   memset( reinterpret_cast<void *> (muRemote),
           0,
           sizeof( muRemote ) );

   memset( reinterpret_cast<void *> (muFromTag),
           0,
           sizeof( muFromTag ) );

   memset( reinterpret_cast<void *> (muToTag),
           0,
           sizeof( muToTag ) );

   memset( reinterpret_cast<void *> (&mMetrics),
           0,
           sizeof( mMetrics ) );

   memset( reinterpret_cast<void *> (&mCnx),
           0,
           sizeof( mCnx ) );

   memset( reinterpret_cast<void *> (&meCdrTermType),
           0,
           sizeof( meCdrTermType ) );
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrStart
**
**  PURPOSE:    Called when the CDR is actually started.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrStart( void )
{
   mCdr.timeStart = time( NULL );
   mCdr.timeStop = 0;

   bosTimeGetMs( &mCdrStartMs );
   mCdrEndMs = 0;

   mCdr.bAnonymous = BOS_FALSE;
   mCdr.bEmergency = BOS_FALSE;
   mCdr.bAccepted = BOS_FALSE;
   
   mbTgtDlg = BOS_FALSE;

   meCdrTermType.termSuccess = CeCdr::eCECDRTERM_FAILED;

   memset( reinterpret_cast<void *> (mCdr.uCallId),
           0,
           sizeof( mCdr.uCallId ) );

   memset( reinterpret_cast<void *> (mCdr.uPeer),
           0,
           sizeof( mCdr.uPeer ) );

   memset( reinterpret_cast<void *> (&mCdr.clid),
           0,
           sizeof( mCdr.clid ) );

   memset( reinterpret_cast<void *> (muLocal),
           0,
           sizeof( muLocal ) );

   memset( reinterpret_cast<void *> (muRemote),
           0,
           sizeof( muRemote ) );

   memset( reinterpret_cast<void *> (muFromTag),
           0,
           sizeof( muFromTag ) );

   memset( reinterpret_cast<void *> (muToTag),
           0,
           sizeof( muToTag ) );

   memset( reinterpret_cast<void *> (&mMetrics),
           0,
           sizeof( mMetrics ) );

   memset( reinterpret_cast<void *> (&mCnx),
           0,
           sizeof( mCnx ) );
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrCid
**
**  PURPOSE:    Used to setup the call identification for this CDR.
**
**  PARAMETERS:
**              puCid - the call identification to copy over.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrCid( const BOS_UINT8 *puCid )
{
   BOS_UINT32 strLen;

   if ( ( puCid != NULL ) && 
        !strlen( reinterpret_cast<const char *> (mCdr.uCallId) ) )
   {
      strLen = strlen( reinterpret_cast<const char *> (puCid) );
      strncpy( reinterpret_cast<char *> ( mCdr.uCallId ),
               reinterpret_cast<const char *> (puCid),
               (( strLen > CCTK_STR_LEN ) ? CCTK_STR_LEN : strLen) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrFromTag
**
**  PURPOSE:    Used to setup the from tag identification for this CDR.
**
**  PARAMETERS:
**              puFromTag - the from tag identification to copy over.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrFromTag( const BOS_UINT8 *puFromTag )
{
   BOS_UINT32 strLen;

   if ( ( puFromTag != NULL ) &&
        !strlen( reinterpret_cast<const char *> (muFromTag) ) )
   {
      strLen = strlen( reinterpret_cast<const char *> (puFromTag) );
      strncpy( reinterpret_cast<char *> ( muFromTag ),
               reinterpret_cast<const char *> (puFromTag),
               (( strLen > CCTK_CLID_LEN ) ? CCTK_CLID_LEN : strLen) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrToTag
**
**  PURPOSE:    Used to setup the to tag identification for this CDR.
**
**  PARAMETERS:
**              puToTag - the to tag identification to copy over.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrToTag( const BOS_UINT8 *puToTag )
{
   BOS_UINT32 strLen;

   if ( ( puToTag != NULL ) &&
        !strlen( reinterpret_cast<const char *> (muToTag) ) )
   {
      strLen = strlen( reinterpret_cast<const char *> (puToTag) );
      strncpy( reinterpret_cast<char *> ( muToTag ),
               reinterpret_cast<const char *> (puToTag),
               (( strLen > CCTK_CLID_LEN ) ? CCTK_CLID_LEN : strLen) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrPeer
**
**  PURPOSE:    Used to setup the peer information for this CDR.
**
**  PARAMETERS:
**              puPeer - the peer information to copy over.
**              uSize  - the size of the information to set.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrPeer( const BOS_UINT8 *puPeer, const BOS_UINT32 uSize )
{
   if ( puPeer != NULL )
   {     
      strncpy( reinterpret_cast<char *> ( mCdr.uPeer ),
               reinterpret_cast<const char *> (puPeer),
               (( uSize > CCTK_STR_LEN ) ? CCTK_STR_LEN : uSize) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrLocal
**
**  PURPOSE:    Used to setup the local information for this CDR.
**
**  PARAMETERS:
**              puLocal - the local information to copy over.
**              uSize  - the size of the information to set.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      If originating a call, the puLocal will contain the SIP:From
**              header information.
**              If terminating a call, the puLocal will contain the SIP:To
**              header information.
***********************************************************************************/
void CeCdr::cdrLocal( const BOS_UINT8 *puLocal, const BOS_UINT32 uSize )
{
   if ( puLocal != NULL )
   {
      strncpy( reinterpret_cast<char *> ( muLocal ),
               reinterpret_cast<const char *> (puLocal),
               (( uSize > CCTK_STR_LEN ) ? CCTK_STR_LEN : uSize) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrRemote
**
**  PURPOSE:    Used to setup the remote information for this CDR.
**
**  PARAMETERS:
**              puRemote - the remote information to copy over.
**              uSize  - the size of the information to set.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      If originating a call, the puRemote will contain the SIP:To
**              header information.
**              If terminating a call, the puRemote will contain the SIP:From
**              header information.
***********************************************************************************/
void CeCdr::cdrRemote( const BOS_UINT8 *puRemote, const BOS_UINT32 uSize )
{
   if ( puRemote != NULL )
   {      
      strncpy( reinterpret_cast<char *> ( muRemote ),
               reinterpret_cast<const char *> (puRemote),
               (( uSize > CCTK_STR_LEN ) ? CCTK_STR_LEN : uSize) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrEnd
**
**  PURPOSE:    Called when the CDR is actually terminated.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrEnd( void )
{
   CCTKCDRINFO cdrInfo;
   BOS_BOOL incomingCall = BOS_FALSE;

   if ( mCdrEndMs > 0 )
   {
     /* the CDR has already been marked as terminated */
      return;
   }

   memset ( static_cast<void *> (&cdrInfo),
            0,
            sizeof ( CCTKCDRINFO ) );

   mCdr.timeStop = time( NULL );

   /* Terminate the association between a CDR and the resource, but not between
   ** the CDR and the owner.  Since the owner of the CDR may want to look back
   ** into the data record later on even when the resource was terminated.
   */
   muCdrRes = guCeInvalidGuid;

   bosTimeGetMs( &mCdrEndMs );

   /* calculate the call duration time for outgoing calls */
   if (mCnx.tConnTime > 0 )
   {
      bosTimeCalcDeltaMs(
         reinterpret_cast<const BOS_TIME_MS *> (&mCnx.tConnTime),
         reinterpret_cast<const BOS_TIME_MS *> (&mCdrEndMs),
         &mCnx.tCallDuration);
   }
         
   /* Publish an event to the upper layer application about the call data record
   ** that got terminated.
    */
   memcpy ( static_cast<void *> (&cdrInfo),
            static_cast<const void *> (&mCdr),
            sizeof ( CCTKCDR ) );
   cdrInfo.bIncoming =
      static_cast<BOS_BOOL> ( (meCdrType == CeCdr::eCECDRTYPE_IN) );
   
   if ( cdrInfo.bIncoming )
   {
      cdrInfo.cctkStats.incomingCalls++;
      switch( meCdrTermType.termSuccess )
      {
         case eCECDRTERM_CONNECTED:
            cdrInfo.cctkStats.incomingConnectedCalls++;
            break;
         case eCECDRTERM_FAILED:
            cdrInfo.cctkStats.incomingFailedCalls++;
            break;
         default:
            break;
      }
      if ( meCdrTermType.answered )
      {
         cdrInfo.cctkStats.incomingAnsweredCalls++;
      }
   }
   else
   {
      cdrInfo.cctkStats.outgoingCalls++;
      switch( meCdrTermType.termSuccess ) 
      {
         case eCECDRTERM_CONNECTED:
            cdrInfo.cctkStats.outgoingConnectedCalls++;
            break;
         case eCECDRTERM_FAILED:
            cdrInfo.cctkStats.outgoingFailedCalls++;
            break;

      }
      if ( meCdrTermType.answered )
      {
         cdrInfo.cctkStats.outgoingAnsweredCalls++;
      }
   }

   /* Copy the local and remote SIP URI's to the CDR info struct */
   strncpy( reinterpret_cast<char *> ( cdrInfo.localSip ),
         reinterpret_cast<const char *> (muLocal),
         (strlen((char *)muLocal) > CCTK_STR_LEN ) ? CCTK_STR_LEN : strlen((char *) muLocal));

   if( strlen((char *)muRemote))
   {
      strncpy( reinterpret_cast<char *> ( cdrInfo.remoteSip ),
            reinterpret_cast<const char *> (muRemote),
            (strlen((char *)muRemote) > CCTK_STR_LEN ) ? CCTK_STR_LEN : strlen((char *) muRemote));
   }
   else
   {
      /* If the remote field is empty, try the peer address instead
      ** this takes care of outgoing calls which dont get answered
      */
      if( strlen( (char*)&cdrInfo.cdr.uPeer[0]) )
      {
         strncpy( reinterpret_cast<char *> ( cdrInfo.remoteSip ),
            reinterpret_cast<const char *> ( &cdrInfo.cdr.uPeer[0]),
            (strlen((char *)&cdrInfo.cdr.uPeer[0]) > CCTK_STR_LEN ) ? CCTK_STR_LEN : strlen((char *)&cdrInfo.cdr.uPeer[0]));
      }
   }

   tkmEvent( muCdrOwner,
             guCeInvalidGuid,
             eCCTKEVT_CDR_INFO,
             static_cast<void *> (&cdrInfo) );
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrLocal
**
**  PURPOSE:    Used to setup the local connection information for this CDR.
**
**  PARAMETERS:
**              pAddr - the local connection address information.
**              pCodec    - the local codec information
**              uPrsrCode - the local connection sip parser codec information.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrLocal( BOS_IP_ADDRESS *pAddr, CCTKCODECINFO *pCodec, 
                      BOS_UINT8 uPrsrCode )
{
   if ( pAddr != NULL )
   {
      bosIpAddrCreateCopy( pAddr, &mCnx.ipLcl );
      mCnx.uPortLcl = pCodec->uRtpPort;
   }
   mCnx.uPtimeLcl = pCodec->uPtime;
   mCnx.uRtpCodeLcl = pCodec->uRtpCode;
   mCnx.uPrsrCodeLcl = uPrsrCode;
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrRemote
**
**  PURPOSE:    Used to setup the remote connection information for this CDR.
**
**  PARAMETERS:
**              pAddr - the remote connection address information.
**              pCodec    - the remote codec information
**              uPrsrCode - the remote connection sip parser codec information.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrRemote( BOS_IP_ADDRESS *pAddr, CCTKCODECINFO *pCodec, 
                       BOS_UINT8 uPrsrCode )
{
   if ( pAddr != NULL )
   {
      bosIpAddrCreateCopy( pAddr, &mCnx.ipRem );
      mCnx.uPortRem = pCodec->uRtpPort;
   }
   mCnx.uPtimeRem = pCodec->uPtime;
   mCnx.uRtpCodeRem = pCodec->uRtpCode;
   mCnx.uPrsrCodeRem = uPrsrCode;
}

/***********************************************************************************
**  FUNCTION:   CeCdr::cdrCallRespRcvTime
**
**  PURPOSE:    Called when receiving receiving 180/183/200 from outgoing calls, 
**              to update the call setup time, and the call connected time
**
**  PARAMETERS:
**              bConnected - TRUE if this is called due to receiving 200 OK
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrCallRespRcvTime( BOS_BOOL bConnected )
{
   BOS_TIME_MS curTime;
   bosTimeGetMs( &curTime );

   /* if the call setup time has not been calculated, calculate it now */
   if (mCnx.tSetupDuration == 0)
   {
      bosTimeCalcDeltaMs(
         reinterpret_cast<const BOS_TIME_MS *> (&mCdrStartMs),
         reinterpret_cast<const BOS_TIME_MS *> (&curTime),
         &mCnx.tSetupDuration);
   }
   
   /* if 200 OK is received, store the connection time so it can be used 
   ** to calculate the call duration
   */
   if ( bConnected )
   {
      mCnx.tConnTime = curTime;
   }
}

/***********************************************************************************
**  FUNCTION:   CeCdr::cdrSetTerm
**
**  PURPOSE:    Used to set the termination type for this CDR.
**
**  PARAMETERS:
**              termType - the termination type of the call.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      An answered call is not mutually exclusive to a failed or
**              connected call. Wheterh a call has failed or connected is
**              mutually exclusive.
***********************************************************************************/
void CeCdr::cdrSetTerm( CECDRTERMSUCCESS termType )
{
   if ( termType == CeCdr::eCECDRTERM_ANSWERED )
   {
      meCdrTermType.answered = BOS_TRUE;
   }
   else if ( termType == CeCdr::eCECDRTERM_CONNECTED ||
             termType == CeCdr::eCECDRTERM_FAILED )
   {
      meCdrTermType.termSuccess = termType;
   }
}  

