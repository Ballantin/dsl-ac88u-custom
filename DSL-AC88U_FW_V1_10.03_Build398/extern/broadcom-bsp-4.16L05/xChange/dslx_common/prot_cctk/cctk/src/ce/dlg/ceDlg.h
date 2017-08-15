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
**      This file defines the Call Dialog information class managed by the
**      Call Engine.
**
**      This class is used to manage dialog information per RFC 4235 support.
**      For the time being, its usage is limited to the actual need of the
**      Call Engine application and in consequence does not implement a full
**      blown RFC 4235 information set.
**
***********************************************************************************/
#ifndef __CE_DLG__H__INCLUDED__
#define __CE_DLG__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */

#define CE_DLG__ELEMENT__DIALOG_INFO         "dialog-info"
#define CE_DLG__ELEMENT__DIALOG              "dialog"
#define CE_DLG__ELEMENT__STATE               "state"

#define CE_DLG__ATTRIBUTE__VERSION           "version"
#define CE_DLG__ATTRIBUTE__XMLNS             "xmlns"
#define CE_DLG__ATTRIBUTE__STATE             "state"
#define CE_DLG__ATTRIBUTE__ENTITY            "entity"
#define CE_DLG__ATTRIBUTE__ID                "id"
#define CE_DLG__ATTRIBUTE__CALL_ID           "call-id"
#define CE_DLG__ATTRIBUTE__LOCAL_TAG         "local-tag"
#define CE_DLG__ATTRIBUTE__REMOTE_TAG        "remote-tag"


#define CE_DLG__DIALOG_STATE_FULL            "full"
#define CE_DLG__DIALOG_STATE_CONFIRMED       "confirmed"
#define CE_DLG__DIALOG_STATE_TRYING          "trying"
#define CE_DLG__DIALOG_STATE_RELEASING       "releasing"


#define CE_DLG_VERSION_LEN                   12
#define CE_DLG_EVENT_LEN                     512


#define CE_DLG__CONTENT_TYPE                 "application"
#define CE_DLG__CONTENT_SUB_TYPE             "dialog-info+xml"


/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */
class CXmlWriter;
class CeMain;

/*
** This class defines the call dialog information element.
*/
class CeDlg : private MXD_GNS::IXmlParserHandler,
              private MXD_GNS::IXmlWriterOutputHandler
{
   public:

      friend class CeMain;

      CeDlg( void );

      virtual ~CeDlg( void );

      inline BOS_BOOL ceDlgInfo( void ) { return mbDlgInfo; };
      inline void ceDlgSet( BOS_BOOL bDlgInfo ) { mbDlgInfo = bDlgInfo; };
      inline void ceDlgVerUp( void ) { muVersion++; };

      BOS_BOOL ceDlgParse( BOS_UINT32 uSize,
                           BOS_UINT8 *puData );

      BOS_BOOL ceDlgWrite( BOS_UINT32 uSize,
                           BOS_UINT8 *puData,
                           BOS_UINT32 uResGuid,
                           BOS_BOOL bFull );

      const BOS_UINT8 *ceDlgRemNs( const BOS_UINT8 *puData );

   private:

      BOS_BOOL mbError;

      BOS_BOOL mbDlgInfo;

      BOS_UINT32 muDlgSize;
      BOS_UINT32 muVersion;

      BOS_UINT8 *muData;
      BOS_UINT32 muSize;

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

      /* ---- IXmlWriterOutputHandler
      */
      bool EvXmlWriterOutputHandlerWrite( IN const char* pcBuffer,
                                          IN unsigned int uBufferSize );
};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeDlg::~CeDlg
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
inline CeDlg::~CeDlg( void )
{
   /* Nothing to be done.
   */
}


#endif /* __CE_DLG__H__INCLUDED__ */
