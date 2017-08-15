/**********************************************************************************
** Copyright © 2008 - 2009 Broadcom
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
**      This file defines the User-Agent Profile information class managed by the
**      Call Engine.
**
**      This class is used to manage user-agent profile information per
**      draft-ietf-sipping-config-framework.
**      For the time being, its usage is limited to the actual need of the
**      Call Engine application and in consequence does not implement a full
**      blown draft-ietf-sipping-config-framework information set.
**
***********************************************************************************/
#ifndef __CE_UAP__H__INCLUDED__
#define __CE_UAP__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */

#define CE_UAP__ELEMENT__SET_ELEMENT         "SetElement"

#define CE_UAP__ELEMENT__CFV_CNT             "CFVForwardedCallCount"
#define CE_UAP__ELEMENT__SCF_CNT             "SCFForwardedCallCount"

#define CE_UAP__FEATURE_CFV                  "cfv"
#define CE_UAP__FEATURE_SCF                  "scf"
#define CE_UAP__FEATURE_DND                  "dnd"

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */
class CeMain;

/*
** This class defines the user-agent profile information element.
*/
class CeUap : private MXD_GNS::IXmlParserHandler
{
   public:

      friend class CeMain;

      CeUap( void );

      virtual ~CeUap( void );

      BOS_BOOL ceUapParse( BOS_UINT32 uSize,
                           BOS_UINT8 *puData );

      BOS_BOOL ceUapDnd( void ) { return mbDnd; };
      BOS_BOOL ceUapCfv( void ) { return mbCfv; };
      BOS_BOOL ceUapScf( void ) { return mbScf; };

      BOS_UINT32 ceUapScfCnt( void ) { return muScfCnt; };
      BOS_UINT32 ceUapCfvCnt( void ) { return muCfvCnt; };

      const BOS_UINT8 *ceUapRemNs( const BOS_UINT8 *puData );

   private:

      BOS_BOOL mbError;

      BOS_UINT8 *muData;
      BOS_UINT32 muSize;

      /* Feature flag that the user-agent profile information may carry to
      ** inform of the status of those features.
      **
      ** All those features are telephony feature per Bellcore specification.
      */
      BOS_BOOL mbCfv;      /* Call forward variable. */
      BOS_BOOL mbScf;      /* Selective call forward. */
      BOS_BOOL mbDnd;      /* Do not disturb. */

      /* Specific feature information that one may be receiving for some
      ** of the above features.
      */
      BOS_UINT32 muCfvCnt;   /* CFV call forwaded count. */
      BOS_UINT32 muScfCnt;   /* SCF call forwaded count. */


      /* Internal data used when parsing the content data.
      */
      BOS_BOOL mbSetElement;
      BOS_BOOL mbCntCfvElement;
      BOS_BOOL mbCntScfElement;

      /* ---- Callback(s) hook-up from the XML interface. ---- */

      /* ---- IXmlParserHandler
      */
      void EvXmlParserHandlerStartElement( IN const char* pszElementName,
                                           IN MXD_GNS::CMarshaler* pAttributes,
                                           IN const uint32_t uOffset );

      void EvXmlParserHandlerEndElement( IN const char* pszElementName,
                                         IN const uint32_t uOffset );

      void EvXmlParserHandlerCharacterData( IN const char* pcText,
                                            IN const unsigned int uTextSize );

      void EvXmlParserHandlerComment( IN const char* pszComments );

      void EvXmlParserHandlerDefault( IN const char* pcText,
                                      IN const unsigned int uTextSize );

      void EvXmlParserHandlerSkippedContent( IN const char* pcSkippedContent,
                                             IN unsigned int uSize );

};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeUap::~CeUap
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
inline CeUap::~CeUap( void )
{
   /* Nothing to be done.
   */
}


#endif /* __CE_UAP__H__INCLUDED__ */
