/**********************************************************************************
** Copyright © 2008 - 2009 Broadcom Corporation
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
**      This file implements the User-Agent Profile information.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceUap.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeUap::CeUap
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
CeUap::CeUap( void ) :
   mbError         ( BOS_FALSE ),
   muData          ( NULL ),
   muSize          ( 0 ),
   mbSetElement    ( BOS_FALSE ),
   mbCntCfvElement ( BOS_FALSE ),
   mbCntScfElement ( BOS_FALSE )
{

}


/***********************************************************************************
**  FUNCTION:   CeUap::ceUapParse
**
**  PURPOSE:    Parse an input user-agent profile information and extract the data
**              of interest.
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
BOS_BOOL CeUap::ceUapParse( BOS_UINT32 uSize,
                            BOS_UINT8 *puData )
{
   MXD_GNS::CXmlParser xmlParser;

   mbCfv = BOS_FALSE;
   mbScf = BOS_FALSE;
   mbDnd = BOS_FALSE;

   mbSetElement = BOS_FALSE;
   mbCntCfvElement = BOS_FALSE;
   mbCntScfElement = BOS_FALSE;

   muCfvCnt = guCeInvalidGuid;
   muScfCnt = guCeInvalidGuid;

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
**  FUNCTION:   CeUap::ceUapRemNs
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
const BOS_UINT8 *CeUap::ceUapRemNs( const BOS_UINT8 *puData )
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
**  FUNCTION:   CeUap::EvXmlParserHandlerStartElement
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
void CeUap::EvXmlParserHandlerStartElement( IN const char* pszElementName,
                                            IN MXD_GNS::CMarshaler* pAttributes,
                                            IN const uint32_t uOffset )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uOffset);

   const BOS_UINT8 *pElemName =
      ceUapRemNs( reinterpret_cast<const BOS_UINT8 *> (pszElementName) );

   /* Set element - determine the feature that is included in this particular
   ** information block.
   */
   if ( strCmpNoCase( reinterpret_cast<const char *> (pElemName),
                      CE_UAP__ELEMENT__SET_ELEMENT ) == 0)
   {
      mbSetElement = BOS_TRUE;
   }
   /* Call forward variable - counter element.
   */
   else if ( strCmpNoCase( reinterpret_cast<const char *> (pElemName),
                           CE_UAP__ELEMENT__CFV_CNT ) == 0)
   {
      mbCntCfvElement = BOS_TRUE;
   }
   /* Selective call forwarding - counter element.
   */
   else if ( strCmpNoCase( reinterpret_cast<const char *> (pElemName),
                           CE_UAP__ELEMENT__SCF_CNT ) == 0)
   {
      mbCntScfElement = BOS_TRUE;
   }

   pAttributes->Clear();
}


/***********************************************************************************
**  FUNCTION:   CeUap::EvXmlParserHandlerEndElement
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
**  NOTES:      Used to reset the status information that dictates what the
**              content of a specific element we care about should have been.
***********************************************************************************/
void CeUap::EvXmlParserHandlerEndElement( IN const char* pszElementName,
                                          IN const uint32_t uOffset )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uOffset);

   const BOS_UINT8 *pElemName =
      ceUapRemNs( reinterpret_cast<const BOS_UINT8 *> (pszElementName) );

   /* Set element - determine the feature that is included in this particular
   ** information block.
   */
   if ( strCmpNoCase( reinterpret_cast<const char *> (pElemName),
                      CE_UAP__ELEMENT__SET_ELEMENT ) == 0)
   {
      mbSetElement = BOS_FALSE;
   }
   /* Call forward variable - counter element.
   */
   else if ( strCmpNoCase( reinterpret_cast<const char *> (pElemName),
                           CE_UAP__ELEMENT__CFV_CNT ) == 0)
   {
      mbCntCfvElement = BOS_FALSE;
   }
   /* Selective call forwarding - counter element.
   */
   else if ( strCmpNoCase( reinterpret_cast<const char *> (pElemName),
                           CE_UAP__ELEMENT__SCF_CNT ) == 0)
   {
      mbCntScfElement = BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   CeUap::EvXmlParserHandlerCharacterData
**
**  PURPOSE:    Callback from XML reader when character data information was
**              encountered.
**
**  PARAMETERS:
**              pcText - pointer to the text data.
**              uTextSize - the size of the data.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      The string passed is *not* NULL terminated.
***********************************************************************************/
void CeUap::EvXmlParserHandlerCharacterData( IN const char* pcText,
                                             IN const unsigned int uTextSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uTextSize);

   /* This should be one of the three feature we care about.
   */
   if ( mbSetElement )
   {
      if ( !strCmpNoCaseSize ( pcText,
                               CE_UAP__FEATURE_CFV,
                               strlen ( CE_UAP__FEATURE_CFV ) ) )
      {
         mbCfv = BOS_TRUE;
      }
      else if ( !strCmpNoCaseSize ( pcText,
                                    CE_UAP__FEATURE_SCF,
                                    strlen ( CE_UAP__FEATURE_SCF ) ) )
      {
         mbScf = BOS_TRUE;
      }
      else if ( !strCmpNoCaseSize ( pcText,
                                    CE_UAP__FEATURE_DND,
                                    strlen ( CE_UAP__FEATURE_DND ) ) )
      {
         mbDnd = BOS_TRUE;
      }
   }
   else if ( mbCntCfvElement || mbCntScfElement )
   {
      BOS_UINT32 uRet;

      if ( isdigit ( *pcText ) )
      {
         uRet = strtoul ( pcText, NULL, 0 );

         if ( mbCntCfvElement )
         {
            muCfvCnt = uRet;
         }
         else if ( mbCntScfElement )
         {
            muScfCnt = uRet;
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeUap::EvXmlParserHandlerComment
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
void CeUap::EvXmlParserHandlerComment( IN const char* pszComments )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszComments);
}


/***********************************************************************************
**  FUNCTION:   CeUap::EvXmlParserHandlerDefault
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
void CeUap::EvXmlParserHandlerDefault( IN const char* pcText,
                                       IN const unsigned int uTextSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcText);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uTextSize);
}


/***********************************************************************************
**  FUNCTION:   CeUap::EvXmlParserHandlerSkippedContent
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
void CeUap::EvXmlParserHandlerSkippedContent( IN const char* pcSkippedContent,
                                              IN unsigned int uSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcSkippedContent);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uSize);
}
