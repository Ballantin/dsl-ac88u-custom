//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_IXCAPHTTPCLIENTMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXCAPHTTPCLIENTMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_XCAPCLIENTCFG_H
#include "Config/XcapClientCfg.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CMimeObject;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//<GROUP TID_PACKAGE_XCAP_HTTP_VARS>
// This is the proprietary HTTP return code used when unable to resolve host
// name.
const unsigned int uUNABLE_TO_RESOLVE_HOSTNAME = 701;

//<GROUP TID_PACKAGE_XCAP_HTTP_VARS>
// This is the proprietary HTTP return code used when unable to connect to
// the server.
const unsigned int uUNABLE_TO_CONNECT = 702;

//<GROUP TID_PACKAGE_XCAP_HTTP_VARS>
// This is the proprietary HTTP return code used when unable to verify server
// certificate.
const unsigned int uUNABLE_TO_VERIFY_CERTIFICATES = 703;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IXcapHttpClientMgr
//========================================
//<GROUP TID_PACKAGE_XCAP_HTTP_CLASSES>
//
//  Summary:
//      This interface defines the event that the IXcapHttpClient can report.
//
// Description:
//  This interface defines the event that the IXcapHttpClient can report.
//
// Location:
//   Http/IXcapHttpClientMgr.h
//
// See Also:
//   IXcapHttpClient
//
//==============================================================================
class IXcapHttpClientMgr
{
//-- Friend Declarations

//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvGetResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of a SendGet operation.
    //
    //  Parameters:
    //    opqRequest:
    //      The opaque parameter passed to the SendGet operation is returned
    //      through this parameter. This can serve as a transaction identifier.
    //
    //    uResponse:
    //      The HTTP response code received.
    //
    //    pszETagHeaderValue:
    //      The entity tag received. It can be NULL if no entity tag was
    //      received.
    //
    //    pMimeObject:
    //      Pointer to a MIME object containing the paylaod that was received in
    //      the response. It can be NULL if no payload was received.
    //
    //  Description:
    //      Reports the result of a SendGet operation.
    //
    //==========================================================================
    virtual void EvGetResponse(IN mxt_opaque opqRequest,
                               IN unsigned int uResponse,
                               IN const char* pszETagHeaderValue,
                               IN TOA CMimeObject* pMimeObject) = 0;

    //==========================================================================
    //==
    //==  EvPutResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of a SendPut operation.
    //
    //  Parameters:
    //    opqRequest:
    //      The opaque parameter passed to the SendPut operation is returned
    //      through this parameter. This can serve as a transaction identifier.
    //
    //    uResponse:
    //      The HTTP response code received.
    //
    //    pszETagHeaderValue:
    //      The entity tag received. It can be NULL if no entity tag was
    //      received.
    //
    //    pMimeObject:
    //      Pointer to a MIME object containing the paylaod that was received in
    //      the response. It can be NULL if no payload was received.
    //
    //  Description:
    //      Reports the result of a SendPut operation.
    //
    //==========================================================================
    virtual void EvPutResponse(IN mxt_opaque opqRequest,
                               IN unsigned int uResponse,
                               IN const char* pszETagHeaderValue,
                               IN TOA CMimeObject* pMimeObject) = 0;

    //==========================================================================
    //==
    //==  EvDeleteResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of a SendDelete operation.
    //
    //  Parameters:
    //    opqRequest:
    //      The opaque parameter passed to the SendGet operation is returned
    //      through this parameter. This can serve as a transaction identifier.
    //
    //    uResponse:
    //      The HTTP response code received.
    //
    //    pszETagHeaderValue:
    //      The entity tag received. It can be NULL if no entity tag was 
    //      received.
    //
    //    pMimeObject:
    //      Pointer to a MIME object containing the paylaod that was received in
    //      the response. It can be NULL if no payload was received.
    //
    //  Description:
    //      Reports the result of a SendDelete operation.
    //
    //==========================================================================
    virtual void EvDeleteResponse(IN mxt_opaque opqRequest,
                                  IN unsigned int uResponse,
                                  IN const char* pszETagHeaderValue,
                                  IN TOA CMimeObject* pMimeObject) = 0;

    //==========================================================================
    //==
    //==  EvAuthenticationRequired
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the server has requested authentication information in
    //      order to proceed with the request.
    //
    //  Parameters:
    //    pszRealm:
    //      The realm reported by the server that is requesting authentication
    //      information.
    //
    //  Description:
    //      Reports that the server has requested authentication information in
    //      order to proceed with the request.
    //
    //      This event is always reported before the actual EvGetResponse,
    //      EvPutResponse, or EvDeleteResponse. To be more precise, when an HTTP
    //      challenge is received, there are always two events reported. The
    //      first is EvAuthenticationRequired and the second is the response
    //      event corresponding to the request that was challenged.
    //
    //      When this event is reported, the user of the IXcapHttpClient must
    //      try to get a password from the user or from local storage and then
    //      call IXcapHttpClient::Authenticate. This processing can be done
    //      asynchronously. Note that Authenticate MUST be called even
    //      if there is no password to send in order to release resources.
    //
    //==========================================================================
    virtual void EvAuthenticationRequired(IN const char* pszRealm) = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IXcapHttpClientMgr() {};

    // Summary:
    //  Destructor.
    virtual ~IXcapHttpClientMgr() {};


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IXcapHttpClientMgr(IN const IXcapHttpClientMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IXcapHttpClientMgr& operator=(IN const IXcapHttpClientMgr& rSrc);


//-- Hidden Data Members
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IXCAPHTTPCLIENTMGR_H

