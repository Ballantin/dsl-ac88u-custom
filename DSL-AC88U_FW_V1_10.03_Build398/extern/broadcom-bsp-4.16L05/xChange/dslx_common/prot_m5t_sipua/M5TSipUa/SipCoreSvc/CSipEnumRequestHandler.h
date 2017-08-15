//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPENUMREQUESTHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPENUMREQUESTHANDLER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#ifndef MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXD_SIPSTACK_ENUM_DEFAULT_ZONE
    // Uses the ENUM zone defined into the framework.
    //------------------------------------------------
    #define MXD_SIPSTACK_ENUM_DEFAULT_ZONE g_szENUM_DEFAULT_ZONE
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CTelUri;
class ISipEnumRequestHandlerMgr;

//==SDOC========================================================================
//== Class: CSipEnumRequestHandler
//========================================
//
// Description:
//   This class is used to manage the ENUM queries asynchronously. Results from
//   the asynchronous method GetEnumUriA are reported through the
//   ISipEnumRequestHandlerMgr interface.
//
//   This object may be released by using the Terminate method.
//
// Location:
//   SipCoreSvc/CSipEnumRequestHandler.h
//
// See Also:
//   GetEnumUri, ISipEnumRequestHandlerMgr, CSipEnumSvc
//
//==EDOC========================================================================
class CSipEnumRequestHandler : protected CEventDriven
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
private:
    enum
    {
        eMSG_GET_ENUM_URI_A,
    };

//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //---------------------

    // Summary:
    //  Sets the E164 zone suffix for ENUM requests.
    //-----------------------------------------------
    void SetE164ZoneSuffix(IN const CString& rstrE164ZoneSuffix);

    // Summary:
    //  Gets an instance of this object.
    //-----------------------------------
    static CSipEnumRequestHandler* Instance();

    // Summary:
    //  Releases that instance.
    //--------------------------
    void Terminate();

    // Summary:
    //  Generates an asynchronous ENUM request.
    //------------------------------------------
    mxt_result GetEnumUriA(IN TO CTelUri*                   pTelUri,
                           IN TO CList<SEnumUri>*           plstEnumUri,
                           IN    ISipEnumRequestHandlerMgr* pMgr,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                           IN    mxt_opaque                 opq,
                           IN    mxt_opaque                 opqDnsUser);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipEnumRequestHandler();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipEnumRequestHandler();

    //-- << Inherited from IMessageServiceMgr >>
    //--------------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool         bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler*  pParameter);

    //-- << Internal Methods >>
    //---------------------------

    // Summary:
    //  Generates an ENUM request.
    //-----------------------------
    void InternalGetEnumUriA(IN CMarshaler* pParameter);

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipEnumRequestHandler(IN const CSipEnumRequestHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipEnumRequestHandler& operator=(IN const CSipEnumRequestHandler& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:

    // The e164 zone suffix to append to the ENUM request.
    //-----------------------------------------------------
    CString m_strE164ZoneSuffix;

    CMutex m_mutexRequestCounter;

    unsigned int m_uRequestCounter;

    bool m_bShutdownRequested;

    static CSipEnumRequestHandler* ms_pInstance;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  SetE164ZoneSuffix
//==
//==============================================================================
//
//  Parameters:
//      rstrE164ZoneSuffix:
//       The zone suffix to append to the FQDN form of the AUS. If the string is
//       empty, this parameter defaults to the zone defined by
//       MXD_SIPSTACK_ENUM_DEFAULT_ZONE.
//
//  Description:
//      Configures the ENUM zone suffix.
//
//  See also:
//      SipStackCfg.h
//
//==EDOC========================================================================
inline void CSipEnumRequestHandler::SetE164ZoneSuffix(IN const CString& rstrE164ZoneSuffix)
{
    m_strE164ZoneSuffix =
        rstrE164ZoneSuffix == "" ? MXD_SIPSTACK_ENUM_DEFAULT_ZONE :
                                   rstrE164ZoneSuffix;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPENUMREQUESTHANDLER_H

