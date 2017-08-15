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
**      This file implements the Dialog information.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceDlg.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeDlg::CeDlg
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
CeDlg::CeDlg( void ) :
   mbError     ( BOS_FALSE ),
   mbDlgInfo   ( BOS_FALSE ),
   muDlgSize   ( 0 ),
   muVersion   ( 0 ),
   muData      ( NULL ),
   muSize      ( 0 )
{

}


/***********************************************************************************
**  FUNCTION:   CeDlg::ceDlgParse
**
**  PURPOSE:    Parse an input dialog information and extract the data of interest.
**
**  PARAMETERS:
**              uSize - the size of the data in input.
**              puData - pointer to the data.
**
**
**  RETURNS:    BOS_TRUE if the parsing and extraction of the data is successful
**              BOS_FALSE otherwise (could be a parsing error or a failure to
**              extract the intended data - result is the same)
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeDlg::ceDlgParse( BOS_UINT32 uSize, BOS_UINT8 *puData )
{
   MXD_GNS::CXmlParser xmlParser;

   mxt_result res = xmlParser.Parse( this,
                                     static_cast<const uint8_t*> (puData),
                                     static_cast<const uint32_t> (uSize) );

   /* The parsing may have succeeded, but the content validation may have
   ** failed.
   */
   if ( MX_RIS_S( res ) && mbError )
   {
     res = resFE_FAIL;
   }

   /* Final result.
   */
   if ( MX_RIS_S( res ) )
   {
      return BOS_TRUE;
   }
   else
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   CeDlg::ceDlgWrite
**
**  PURPOSE:    Writes an input dialog information.
**
**  PARAMETERS:
**              uSize - the size of the array to write to.
**              puData - pointer to the data to be written
**              uResGuid - the GUID of the dialog information owner.
**              bFull - BOS_TRUE if we need to set the full dialog information
**                      corresponding to this resource, BOS_FALSE otherwise.
**
**
**  RETURNS:    BOS_TRUE if the write was successful.
**              BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeDlg::ceDlgWrite( BOS_UINT32 uSize, BOS_UINT8 *puData,
                            BOS_UINT32 uResGuid, BOS_BOOL bFull )
{
   MXD_GNS::CXmlWriter xmlWriter( this );

   muData = puData;
   muSize = uSize;
   muDlgSize = 0;

   memset( puData,
           0,
           uSize );

   BOS_BOOL bRes = xmlWriter.StartDocument();
   BOS_UINT8 uVer[CE_DLG_VERSION_LEN + 1];

   memset( static_cast<void *> (uVer),
           0,
           sizeof( uVer ) );

   /* Add the dialog-info element and attributes.
   */
   bRes = bRes && xmlWriter.StartElementAttribute( CE_DLG__ELEMENT__DIALOG_INFO );
   
   snprintf( reinterpret_cast<char *> (uVer),
            (CE_DLG_VERSION_LEN + 1),
            "%d",
            static_cast<int> (muVersion) );
   bRes =
      bRes && xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__VERSION,
                                         reinterpret_cast<const char *> (uVer) );
   bRes = bRes && xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__STATE,
                                             CE_DLG__DIALOG_STATE_FULL );
   bRes = bRes && xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__XMLNS,
                                             "urn:ietf:params:xml:ns:dialog-info" );
   bRes = bRes && xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__ENTITY,
                                             "sip:anonynous@anonymous.invalid" );

   bRes = bRes && xmlWriter.EndElementAttribute( CE_DLG__ELEMENT__DIALOG_INFO );

   /* Add the dialog information element(s).  A 'full' view implies we set all
   ** dialog(s) detailled information.  A 'partial' view implies we set only the
   ** notion of whether dialog(s) is(are) present for this user.
   */
   if ( mbDlgInfo )
   {
      if ( bFull )
      {
         BOS_UINT32 uIx;

         for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
         {
            if ( CeMain::mpResCall[uIx] &&
                 ( uResGuid != guCeInvalidGuid ) &&
                 ( CeMain::mpResCall[uIx]->resourceGuid() == uResGuid ) )
            {
               CeCdr *pCdr;

               pCdr = CeMain::ceCdrMgr()->locateCdr(
                                    CeMain::mpResCall[uIx]->resourceParent(),
                                    CeMain::mpResCall[uIx]->resourceGuid() );
               if ( pCdr )
               {
                  bRes = bRes &&
                     xmlWriter.StartElementAttribute( CE_DLG__ELEMENT__DIALOG );
                  bRes = bRes &&
                     xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__ID, 
                                                reinterpret_cast<const char *> (
                                                   pCdr->cdrData()->uCallId ) );
                  bRes = bRes &&
                     xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__CALL_ID,
                                                reinterpret_cast<const char *> (
                                                   pCdr->cdrData()->uCallId ) );
                  bRes = bRes &&
                     xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__LOCAL_TAG,
                                                ( ( pCdr->cdrType() ==
                                                         CeCdr::eCECDRTYPE_IN ) ?
                                                   reinterpret_cast<const char *> (
                                                         pCdr->cdrToTag() ) : 
                                                   reinterpret_cast<const char *> (
                                                         pCdr->cdrFromTag() ) ) );
                  bRes = bRes &&
                     xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__REMOTE_TAG,
                                                ( ( pCdr->cdrType() ==
                                                         CeCdr::eCECDRTYPE_IN ) ?
                                                   reinterpret_cast<const char *> (
                                                         pCdr->cdrFromTag() ) : 
                                                   reinterpret_cast<const char *> (
                                                         pCdr->cdrToTag() ) ) );
                  bRes = bRes &&
                     xmlWriter.EndElementAttribute( CE_DLG__ELEMENT__DIALOG );

                  switch ( CeMain::mpResCall[uIx]->resourceState() )
                  {
                     case CeFsmBase::eCEFSMSTATE_CALL_CLEAN:
                     {
                        bRes = bRes &&
                           xmlWriter.WriteElement( CE_DLG__ELEMENT__STATE,
                                                   CE_DLG__DIALOG_STATE_RELEASING );
                     }
                     break;

                     case CeFsmBase::eCEFSMSTATE_CALL_CONNECT:
                     {
                        bRes = bRes &&
                           xmlWriter.WriteElement( CE_DLG__ELEMENT__STATE,
                                                   CE_DLG__DIALOG_STATE_CONFIRMED );
                     }
                     break;

                     default:
                     {
                        bRes = bRes &&
                           xmlWriter.WriteElement( CE_DLG__ELEMENT__STATE,
                                                   CE_DLG__DIALOG_STATE_TRYING );
                     }
                     break;
                  }
                  
                  bRes = bRes && xmlWriter.EndElement( CE_DLG__ELEMENT__DIALOG );
               }
            }
         }
      }
      else
      {
         bRes = bRes && xmlWriter.StartElementAttribute( CE_DLG__ELEMENT__DIALOG );
         bRes = bRes && xmlWriter.WriteAttributes( CE_DLG__ATTRIBUTE__ID, "1" );
         bRes = bRes && xmlWriter.EndElementAttribute( CE_DLG__ELEMENT__DIALOG );

         bRes = bRes && xmlWriter.WriteElement( CE_DLG__ELEMENT__STATE,
                                                CE_DLG__DIALOG_STATE_CONFIRMED );

         bRes = bRes && xmlWriter.EndElement( CE_DLG__ELEMENT__DIALOG );
      }
   }

   bRes = bRes && xmlWriter.EndElement( CE_DLG__ELEMENT__DIALOG_INFO );
   bRes = bRes && xmlWriter.EndDocument();

   return bRes;
}


/***********************************************************************************
**  FUNCTION:   CeDlg::ceDlgRemNs
**
**  PURPOSE:    Removes namespace information from a given element.
**
**  PARAMETERS:
**              puData - pointer to the data to be looked at.
**
**
**  RETURNS:    Pointer to the modified information after processing.
**
**  NOTES:
***********************************************************************************/
const BOS_UINT8 *CeDlg::ceDlgRemNs( const BOS_UINT8 *puData )
{
   BOS_SINT32 uIx = strlen( reinterpret_cast<const char *> (puData) );
   BOS_BOOL bNotFound = BOS_TRUE;

   for ( ; uIx >= 0 && bNotFound ; uIx-- )
   {
      if ( puData[uIx] == ':' )
      {
         ++uIx;
         bNotFound = BOS_FALSE;
      }
   }

   return &puData[++uIx];
}


/* ------------------------------------------------------------------------------ */
/* ---- BELOW THIS POINT, THE CALLBACK INTERFACE FROM THE XML IS IMPLEMENTED ---- */
/* ------------------------------------------------------------------------------ */


/***********************************************************************************
**  FUNCTION:   CeDlg::EvXmlParserHandlerStartElement
**
**  PURPOSE:    Callback from XML reader when start element information was
**              encountered.
**
**  PARAMETERS:
**              pszElementName - pointer to the element name.
**              uOffset - offset of the element.
**              pAttributes - attributes associated with the element.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      The function does the miniamlistic processing required given the
**              current state of the art of what this is used for.
***********************************************************************************/
void CeDlg::EvXmlParserHandlerStartElement( IN const char* pszElementName,
                                            IN MXD_GNS::CMarshaler* pAttributes,
                                            IN const uint32_t uOffset )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uOffset);

   const BOS_UINT8 *pElemName =
      ceDlgRemNs( reinterpret_cast<const BOS_UINT8 *> (pszElementName) );

   if ( strCmpNoCase( reinterpret_cast<const char *> (pElemName),
                      CE_DLG__ELEMENT__DIALOG ) == 0)
   {
      /* There is at least one dialog information, we do not care about the
      ** content of it, we only care to know it was there.
      */
      mbDlgInfo = BOS_TRUE;
   }

   pAttributes->Clear();
}


/***********************************************************************************
**  FUNCTION:   CeDlg::EvXmlParserHandlerEndElement
**
**  PURPOSE:    Callback from XML reader when end element information was
**              encountered.
**
**  PARAMETERS:
**              pszElementName - pointer to the element name.
**              uOffset - offset of the element.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeDlg::EvXmlParserHandlerEndElement( IN const char* pszElementName,
                                          IN const uint32_t uOffset )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszElementName);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uOffset);
}


/***********************************************************************************
**  FUNCTION:   CeDlg::EvXmlParserHandlerCharacterData
**
**  PURPOSE:    Callback from XML reader when character data information was
**              encountered.
**
**  PARAMETERS:
**              pcText - pointer to the tesxt data.
**              uTextSize - the size of the data.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      The string passed is *not* NULL terminated.
***********************************************************************************/
void CeDlg::EvXmlParserHandlerCharacterData( IN const char* pcText,
                                             IN const unsigned int uTextSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcText);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uTextSize);
}


/***********************************************************************************
**  FUNCTION:   CeDlg::EvXmlParserHandlerComment
**
**  PURPOSE:    Callback from XML reader when comment information was encountered.
**
**  PARAMETERS:
**              pszComments - pointer to the comment data.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeDlg::EvXmlParserHandlerComment( IN const char* pszComments )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszComments);
}


/***********************************************************************************
**  FUNCTION:   CeDlg::EvXmlParserHandlerDefault
**
**  PURPOSE:    Callback from XML reader when default element was encountered.
**
**  PARAMETERS:
**              uTextSize - the size of the XML buffer.
**              pcText - pointer to the XML data.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeDlg::EvXmlParserHandlerDefault( IN const char* pcText,
                                       IN const unsigned int uTextSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcText);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uTextSize);
}


/***********************************************************************************
**  FUNCTION:   CeDlg::EvXmlParserHandlerSkippedContent
**
**  PURPOSE:    Callback from XML reader when skiped content was encountered.
**
**  PARAMETERS:
**              uSize - the size of the XML buffer.
**              pcSkippedContent - pointer to the XML data.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeDlg::EvXmlParserHandlerSkippedContent( IN const char* pcSkippedContent,
                                              IN unsigned int uSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcSkippedContent);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uSize);
}


/***********************************************************************************
**  FUNCTION:   CeDlg::EvXmlWriterOutputHandlerWrite
**
**  PURPOSE:    Callback from XML writer when data need to be written.
**
**  PARAMETERS:
**              uBufferSize - the size of the XML buffer.
**              pcBuffer - pointer to the XML data to be written
**
**
**  RETURNS:    BOS_TRUE if the write was successful.
**              BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
bool CeDlg::EvXmlWriterOutputHandlerWrite( IN const char* pcBuffer,
                                           IN unsigned int uBufferSize )
{
   if ( ( muDlgSize + uBufferSize ) <= muSize )
   {
      strncpy( reinterpret_cast<char *> (&muData[muDlgSize]),
               pcBuffer,
               uBufferSize );

      muDlgSize += uBufferSize;

      return BOS_TRUE;
   }
   else
   {
      return BOS_FALSE;
   }
}
