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
#ifndef MXG_CUAIMPPPAGESENDER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPPAGESENDER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_IUAIMPPPAGESENDER_H
#include "SceUaImpp/IUaImppPageSender.h"
#endif

#ifndef MXG_ISCEOODREQUESTSENDERMGR_H
#include "SceCoreComponents/ISceOodRequestSenderMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class IEComUnknown;
class ISceOodRequestSender;
class IUaImppPageSenderMgr;

//==============================================================================
//== Class: CUaImppPageSender
//========================================
//
// Description:
//   This class implements the Page Sender. It is used to send MESSAGE requests.
//
// Location:
//   SceUaImpp/CUaImppPageSender.h
//
//==============================================================================
class CUaImppPageSender : private CEComDelegatingUnknown,
                          private IUaImppPageSender,
                          private ISceOodRequestSenderMgr
{
//-- Friend Declarations.
    friend class CSceUaImppInitializer;


//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppPageSender();
    static void FinalizeCUaImppPageSender();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result InitializeInstance();

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);



    //-- << IUaImppPageSender >>
    virtual mxt_result SetManager(IN IUaImppPageSenderMgr* pMgr);
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);
    virtual void SetOpaque(IN mxt_opaque opq);
    virtual mxt_opaque GetOpaque();
    virtual mxt_result SetPeerAddr(IN const CNameAddr& rPeerAddr);
    virtual mxt_result SendMessage(IN mxt_opaque opqTransaction,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN TO CSceCallerPreferences* pCallerPreferences = NULL);

    //-- << ISceOodRequestSenderMgr >>
    virtual void EvOodResponse(IN ISceOodRequestSender* pComponent,
                               IN ISceUserConfig* pUserConfig,
                               IN mxt_opaque opqTransaction,
                               IN const CSipPacket& rResponse);
    



    // Summary:
    //  Default Constructor.
    CUaImppPageSender(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaImppPageSender();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppPageSender(IN const CUaImppPageSender& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppPageSender& operator=(IN const CUaImppPageSender& rSrc);


//-- Hidden Data Members.
protected:
private:

    // The manager to report the events to.
    IUaImppPageSenderMgr* m_pMgr;

    // The aggregated object.
    IEComUnknown* m_pOodRequestSenderUnknown;

    // The internal ISceOodRequestSender.
    ISceOodRequestSender* m_pOodRequestSender;

};


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CUAIMPPPAGESENDER_H

//M5T_INTERNAL_USE_END
