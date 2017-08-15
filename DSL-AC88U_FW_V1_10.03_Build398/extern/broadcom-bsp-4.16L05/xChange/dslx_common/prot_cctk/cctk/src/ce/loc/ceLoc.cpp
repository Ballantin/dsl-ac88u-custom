/**********************************************************************************
** Copyright (c) 2008-2011 Broadcom Corporation
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
**      This file implements the Location information.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceLoc.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CE__LOC__EMPTY_STRING          ""

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeLoc::CeLoc
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
CeLoc::CeLoc( void ) :
   mbError     ( BOS_FALSE ),
   muLocSize   ( 0 ),
   muData      ( NULL ),
   muSize      ( 0 )
{

}


/***********************************************************************************
**  FUNCTION:   CeLoc::ceLocWrite
**
**  PURPOSE:    Writes an input location information.
**
**  PARAMETERS:
**              uSize - the size of the array to write to.
**              puData - pointer to the data to be written
**              uResGuid - the GUID of the location information owner.
**
**
**  RETURNS:    BOS_TRUE if the write was successful.
**              BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeLoc::ceLocWrite( BOS_UINT32 uSize, BOS_UINT8 *puData,
                            BOS_UINT32 uResGuid )
{
   MXD_GNS::CXmlWriter xmlWriter( this );
   const CFGSYS *pCfgSys = CFG_SYS;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );

   muData = puData;
   muSize = uSize;
   muLocSize = 0;

   memset( puData,
           0,
           uSize );

   BOS_BOOL bRes = xmlWriter.StartDocument();

   /* Add the location (presence) element and attributes.
   */
   bRes = bRes && xmlWriter.StartElementAttribute( CE_LOC__ELEMENT__PRESENCE );

   bRes = bRes && xmlWriter.WriteAttributes( CE_LOC__ATTRIBUTE__XMLNS,
                                             "urn:ietf:params:xml:ns:pidf" );
   bRes = bRes && xmlWriter.WriteAttributes(
                             CE_LOC__ATTRIBUTE__XMLNS_GP,
                             "urn:ietf:params:xml:ns:pidf:geopriv10" );
   bRes = bRes && xmlWriter.WriteAttributes(
                             CE_LOC__ATTRIBUTE__XMLNS_CL,
                             "urn:ietf:params:xml:ns:pidf:geopriv10:civicLoc" );
   if ( pCfgSys)
   {
      if ( pCfgSys->xtl.net.prot.gen.emgAddr.bUseGeoLoc )
      {
         bRes = bRes && xmlWriter.WriteAttributes(
                             CE_LOC__ATTRIBUTE__XMLNS_GML,
                             "http://www.opengis.net/gml");

      }
   }   

   bRes = bRes && xmlWriter.EndElementAttribute( CE_LOC__ELEMENT__PRESENCE );

   bRes = bRes && xmlWriter.StartElementAttribute( CE_LOC__ELEMENT__DEVICE );
   bRes = bRes && xmlWriter.WriteAttributes( CE_LOC__ATTRIBUTE__ID, "1" );
   bRes = bRes && xmlWriter.EndElementAttribute( CE_LOC__ELEMENT__DEVICE );

   bRes = bRes && xmlWriter.StartElement( CE_LOC__ELEMENT__STATUS );
   bRes = bRes && xmlWriter.StartElement( CE_LOC__ELEMENT__GEOPRIV );
   bRes = bRes && xmlWriter.StartElement( CE_LOC__ELEMENT__LOCATION_INFO );
   /*Populate according to location type from config*/
   if ( pCfgSys)
   {
      if ( !pCfgSys->xtl.net.prot.gen.emgAddr.bUseGeoLoc )
      {
         /*populate civic address*/
         bRes = bRes && xmlWriter.StartElement( CE_LOC__ELEMENT__CIVIC_ADDRESS );
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__COUNTRY,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcCountry ?
               reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcCountry) :
                     CE__LOC__EMPTY_STRING );
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__A1,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcSubDiv ?         
                  reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcSubDiv) :
                     CE__LOC__EMPTY_STRING );
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__A3,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcCity ?         
                  reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcCity) :
                     CE__LOC__EMPTY_STRING );
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__A6,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcStreet ?         
                  reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcStreet) :
                     CE__LOC__EMPTY_STRING );
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__HNO,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcHouseNum ?
                  reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcHouseNum) :
                     CE__LOC__EMPTY_STRING );
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__LOC,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcLocInfo ?
                  reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcLocInfo) :
                     CE__LOC__EMPTY_STRING );
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__PC,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcPostal ?
                  reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcPostal) :
                     CE__LOC__EMPTY_STRING );
         bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__CIVIC_ADDRESS );
      }
      else
      {
         /*popuate geo location info as a point*/
         bRes = bRes && xmlWriter.StartElementAttribute( CE_LOC__ELEMENT__POINT );
         /*srsName*/
         bRes = bRes && xmlWriter.WriteAttributes(
                             CE_LOC__ATTRIBUTE__SRSNAME,
                             pCfgSys->xtl.net.prot.gen.emgAddr.pcSrsName ?
                               reinterpret_cast<const char *> (
                                pCfgSys->xtl.net.prot.gen.emgAddr.pcSrsName) :
                                CE__LOC__EMPTY_STRING );
        bRes = bRes && xmlWriter.EndElementAttribute( CE_LOC__ELEMENT__POINT );
                       
         /*pos element*/                    
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__POS,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcPos ?
               reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcPos) :
                     CE__LOC__EMPTY_STRING );
         
         
         /*Add floor field with civic address if provided*/
         if (pCfgSys->xtl.net.prot.gen.emgAddr.pcFLR)
         {
           /*populate civic address*/
           bRes = bRes && xmlWriter.StartElement( CE_LOC__ELEMENT__CIVIC_ADDRESS );
           bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__FLR,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcFLR ?
                  reinterpret_cast<const char *> (
                     pCfgSys->xtl.net.prot.gen.emgAddr.pcFLR) :
                     CE__LOC__EMPTY_STRING );
           bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__CIVIC_ADDRESS ); 
         }
         bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__POINT );
                                                                                      
      }

   }
   bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__LOCATION_INFO );

   bRes = bRes && xmlWriter.StartElement( CE_LOC__ELEMENT__USAGE_RULES );
   bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__USAGE_RULES );

   bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__GEOPRIV );
   bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__STATUS );

   if ( pCfgSys )
   {
      if ( !pCfgSys->xtl.net.prot.gen.emgAddr.bUseGeoLoc )
      {
         bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__TIMESTAMP,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcTimeStamp ?         
               reinterpret_cast<const char *> (
                  pCfgSys->xtl.net.prot.gen.emgAddr.pcTimeStamp) :
                  CE__LOC__EMPTY_STRING );
      }
      else
      {
               bRes = bRes && xmlWriter.WriteElement( CE_LOC__ELEMENT__TIMESTAMP,
            pCfgSys->xtl.net.prot.gen.emgAddr.pcGeoTimeStamp ?         
               reinterpret_cast<const char *> (
                  pCfgSys->xtl.net.prot.gen.emgAddr.pcGeoTimeStamp) :
                  CE__LOC__EMPTY_STRING );
      }
   }

   bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__DEVICE );
   bRes = bRes && xmlWriter.EndElement( CE_LOC__ELEMENT__PRESENCE );
   bRes = bRes && xmlWriter.EndDocument();

   return bRes;
}



/* ------------------------------------------------------------------------------ */
/* ---- BELOW THIS POINT, THE CALLBACK INTERFACE FROM THE XML IS IMPLEMENTED ---- */
/* ------------------------------------------------------------------------------ */


/***********************************************************************************
**  FUNCTION:   CeLoc::EvXmlParserHandlerStartElement
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
**  NOTES:      Not implemented, we do not support receiving location information.
***********************************************************************************/
void CeLoc::EvXmlParserHandlerStartElement( IN const char* pszElementName,
                                            IN MXD_GNS::CMarshaler* pAttributes,
                                            IN const uint32_t uOffset )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszElementName);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pAttributes);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uOffset);
}


/***********************************************************************************
**  FUNCTION:   CeLoc::EvXmlParserHandlerEndElement
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
**  NOTES:      Not implemented, we do not support receiving location information.
***********************************************************************************/
void CeLoc::EvXmlParserHandlerEndElement( IN const char* pszElementName,
                                          IN const uint32_t uOffset )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszElementName);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uOffset);
}


/***********************************************************************************
**  FUNCTION:   CeLoc::EvXmlParserHandlerCharacterData
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
**  NOTES:      Not implemented, we do not support receiving location information.
***********************************************************************************/
void CeLoc::EvXmlParserHandlerCharacterData( IN const char* pcText,
                                             IN const unsigned int uTextSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcText);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uTextSize);
}


/***********************************************************************************
**  FUNCTION:   CeLoc::EvXmlParserHandlerComment
**
**  PURPOSE:    Callback from XML reader when comment information was encountered.
**
**  PARAMETERS:
**              pszComments - pointer to the comment data.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      Not implemented, we do not support receiving location information.
***********************************************************************************/
void CeLoc::EvXmlParserHandlerComment( IN const char* pszComments )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszComments);
}


/***********************************************************************************
**  FUNCTION:   CeLoc::EvXmlParserHandlerDefault
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
**  NOTES:      Not implemented, we do not support receiving location information.
***********************************************************************************/
void CeLoc::EvXmlParserHandlerDefault( IN const char* pcText,
                                       IN const unsigned int uTextSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcText);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uTextSize);
}


/***********************************************************************************
**  FUNCTION:   CeLoc::EvXmlParserHandlerSkippedContent
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
**  NOTES:      Not implemented, we do not support receiving location information.
***********************************************************************************/
void CeLoc::EvXmlParserHandlerSkippedContent( IN const char* pcSkippedContent,
                                              IN unsigned int uSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pcSkippedContent);
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uSize);
}


/***********************************************************************************
**  FUNCTION:   CeLoc::EvXmlWriterOutputHandlerWrite
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
bool CeLoc::EvXmlWriterOutputHandlerWrite( IN const char* pcBuffer,
                                           IN unsigned int uBufferSize )
{
   if ( ( muLocSize + uBufferSize ) <= muSize )
   {
      strncpy( reinterpret_cast<char *> (&muData[muLocSize]),
               pcBuffer,
               uBufferSize );

      muLocSize += uBufferSize;
      
      return BOS_TRUE;
   }
   else
   {
      return BOS_FALSE;
   }
}
