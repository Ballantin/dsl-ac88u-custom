/**********************************************************************************
** Copyright (c) 2013 Broadcom
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
**      This file defines the Call Data Record class managed by the Call Engine.
**
***********************************************************************************/
#ifndef __CE_CDR__H__INCLUDED__
#define __CE_CDR__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */
#define CE_CDR__PARAMETER_TAG    "tag"

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the call data record item element.
*/
class CeCdr
{
   public:

      friend class CeCdrMgr;

      enum CECDRTYPE
      {
         eCECDRTYPE_IN,              /* Incoming. */
         eCECDRTYPE_OUT,             /* Outgoing. */
         eCECDRTYPE_MAX,             /* Counter. */
      };

      enum CECDRTERMSUCCESS
      {
         eCECDRTERM_ANSWERED,
         eCECDRTERM_CONNECTED,
         eCECDRTERM_FAILED,
         eCECDRTERM_MAX,
      };

      /*
       * Whether a call has been connected or has failed is mutually exclusive.
       * However, both of these states are not mutually exlusive with a
       * call being answered or not.
       */
      struct CECDRTERMTYPE
      {
         CECDRTERMSUCCESS termSuccess;
         BOS_BOOL         answered;
      };

      struct CECDRCNX
      {
         BOS_IP_ADDRESS ipLcl;
         BOS_UINT16     uPortLcl;
         BOS_UINT32     uPtimeLcl;
         BOS_UINT8      uRtpCodeLcl;
         BOS_UINT8      uPrsrCodeLcl;
         BOS_IP_ADDRESS ipRem;
         BOS_UINT16     uPortRem;
         BOS_UINT32     uPtimeRem;
         BOS_UINT8      uRtpCodeRem;
         BOS_UINT8      uPrsrCodeRem;
         BOS_TIME_MS    tConnTime;
         BOS_TIME_MS    tSetupDuration;
         BOS_TIME_MS    tCallDuration;
 
      };

      CeCdr( void );

      virtual ~CeCdr( void );

      inline void cdrSetRes( BOS_UINT32 uId )
                                    { muCdrRes = uId; muCdrPrevRes = uId; };
      inline void cdrAnonymous( BOS_BOOL bAnonymous )
                                    { mCdr.bAnonymous = bAnonymous; };
      inline BOS_BOOL cdrIsAnonymous( void )
                                    { return mCdr.bAnonymous; };
      inline BOS_BOOL cdrIsEmergency( void )
                                    { return mCdr.bEmergency; };
      inline void cdrEmergency( BOS_BOOL bEmergency )
                                    { mCdr.bEmergency = bEmergency; };
      inline BOS_BOOL cdrIsAccepted( void )
                                    { return mCdr.bAccepted; };
      inline void cdrAccepted( BOS_BOOL bAccepted )
                                    { mCdr.bAccepted = bAccepted; };
      inline CCTKCDR *cdrData( void ) { return &mCdr; };
      inline CECDRTYPE cdrType( void ) { return meCdrType; };
      inline BOS_UINT8 *cdrFromTag( void ) { return muFromTag; };
      inline BOS_UINT8 *cdrToTag( void ) { return muToTag; };
      inline BOS_UINT8 *cdrLocal( void ) { return muLocal; };
      inline BOS_UINT8 *cdrRemote( void ) { return muRemote; };
      inline CCTKCALLMETRICS *cdrMetrics( void ) { return &mMetrics; };
      inline CECDRCNX *cdrCnx( void ) { return &mCnx; };
      inline BOS_BOOL cdrIsTgtDlg( void ) { return mbTgtDlg; };
      inline void cdrTgtDlg( BOS_BOOL bTgtDlg )
                                    { mbTgtDlg = bTgtDlg; };

      void cdrStart( void );
      void cdrEnd( void );
      void cdrCallRespRcvTime( BOS_BOOL bConnected );
      void cdrCid( const BOS_UINT8 *puCid );
      void cdrFromTag( const BOS_UINT8 *puFromTag );
      void cdrToTag( const BOS_UINT8 *puToTag );
      void cdrPeer( const BOS_UINT8 *puPeer, const BOS_UINT32 uSize );
      void cdrLocal( const BOS_UINT8 *puLocal, const BOS_UINT32 uSize );
      void cdrRemote( const BOS_UINT8 *puRemote, const BOS_UINT32 uSize );
      inline void cdrMetrics( CCTKCALLMETRICS *pMetrics );
      inline void cdrClidInfo( CCTKCLIDINFO *pClid );
      void cdrLocal( BOS_IP_ADDRESS *pAddr, CCTKCODECINFO *pCodec, 
                     BOS_UINT8 uPrsrCode );
      void cdrRemote( BOS_IP_ADDRESS *pAddr, CCTKCODECINFO *pCodec, 
                      BOS_UINT8 uPrsrCode );
      void cdrSetTerm( CECDRTERMSUCCESS termType );

   private:

      CECDRTYPE                meCdrType;
      CECDRTERMTYPE            meCdrTermType;

      CCTKCDR                  mCdr;

      BOS_UINT32               muCdrOwner;
      BOS_UINT32               muCdrPrevRes;
      BOS_UINT32               muCdrRes;
      BOS_UINT8                muLocal[CCTK_STR_LEN + 1];
      BOS_UINT8                muRemote[CCTK_STR_LEN + 1];
      BOS_UINT8                muFromTag[CCTK_CLID_LEN + 1];
      BOS_UINT8                muToTag[CCTK_CLID_LEN + 1];
      BOS_BOOL                 mbTgtDlg;

      BOS_TIME_MS              mCdrStartMs;
      BOS_TIME_MS              mCdrEndMs;

      CCTKCALLMETRICS          mMetrics;
      CECDRCNX                 mCnx;
};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeCdr::~CeCdr
**
**  PURPOSE:    Class desctructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline CeCdr::~CeCdr( void )
{
   /* Nothing to be done.
   */
}


/***********************************************************************************
**  FUNCTION:   CeCdr::cdrMetrics
**
**  PURPOSE:    Set the CDR metrics for this block.
**
**  PARAMETERS:
**              pMetrics - the metrics to be copied into the CDR block.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrMetrics( CCTKCALLMETRICS *pMetrics )
{
   if ( pMetrics )
   {
      memcpy( &mMetrics, pMetrics, sizeof( CCTKCALLMETRICS ) );
   }
}

/***********************************************************************************
**  FUNCTION:   CeCdr::cdrClidInfo
**
**  PURPOSE:    Set the CDR caller-id info for this block.
**
**  PARAMETERS:
**              pClid - the caller-id info to be copied into the CDR block.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeCdr::cdrClidInfo( CCTKCLIDINFO *pClid )
{
   if ( pClid )
   {
      memcpy( &mCdr.clid, pClid, sizeof( CCTKCLIDINFO ) );
   }
}



#endif /* __CE_CDR__H__INCLUDED__ */
