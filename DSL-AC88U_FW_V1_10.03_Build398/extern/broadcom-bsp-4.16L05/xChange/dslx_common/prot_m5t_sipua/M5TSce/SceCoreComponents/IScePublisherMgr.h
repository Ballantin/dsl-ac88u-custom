//==============================================================================
//==============================================================================
//
//              Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCEPUBLISHERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEPUBLISHERMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CNameAddr;
class IScePublisher;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IScePublisherMgr
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//   This interface defines the event that can be reported by the presence
//   publisher component.
//
// Description:
//   This interface defines the event that can be reported by the presence
//   publisher component.
//
// Location:
//   SceCoreComponents/IScePublisherMgr.h
//
// See Also:
//   IScePublisher
//
//==============================================================================
class IScePublisherMgr
{
//-- Friend Declarations


//-- Published Interface
public:


    // Summary:
    //   The possible status reported as a parameter to the EvPublicationStatus.
    //
    // Description:
    //   This enumeration defines the possible status reported as a parameter to
    //   the EvPublicationStatus.
    enum EPublicationStatus
    {
        // Description:
        //  This status reports that the publication was not yet possible,
        //  most likely due to an error condition. The publisher component will
        //  retry the publication after a certain time-out period. This period
        //  is either:
        //      - The value of the Retry-After header in the response returned
        //        by the server, or
        //      - The delay configured in
        //        IUaImppPresencePublisherConfig::SetPublicationRetryDelay
        eINACTIVE,

        // Description:
        //  This status reports that the publisher component is currently
        //  trying to send the presence state to the server. If this succeeds,
        //  the next status would be eACTIVE. If this fails, the status would
        //  be either eINACTIVE or eFAILED.
        eTRYING,

        // Description:
        //  This status reports that the publication was successful.
        eACTIVE,

        // Description:
        //  This status reports that the publication is either being refreshed
        //  or updated. If the update or refresh succeeds, the next status
        //  would be eACTIVE. If it fails, the status would be either eINACTIVE
        //  or eFAILED.
        eACTIVE_TRYING,

        // Description:
        //  This status is reported after the application has called
        //  IUaImppPresencePublisher::Terminate. It informs that the component
        //  is proceeding with the termination of the published state on the
        //  server. The application must wait for eTERMINATED to be reported
        //  before it releases the component.
        eACTIVE_TERMINATING,

        // Description:
        //  This status is reported when the published state was successfully
        //  cleared from the server. This status is final. The application must
        //  release the corresponding publisher component once it reports this
        //  event.
        eTERMINATED,

        // Description:
        //  This status is reported when the publisher component is unable to
        //  publish the presence state to the server. This event is only
        //  reported when the publisher component thinks that it will not be
        //  possible, with the current configuration, to successfully send a
        //  publication request to the server. This status is final. The
        //  application must release the corresponding publisher component once
        //  it reports this event.
        eFAILED
    };


    //==========================================================================
    //==
    //==  EvPublicationStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the publication status.
    //
    //  Parameters:
    //    pPublisher:
    //      The IUaImppPresencePublisher component reporting the event.
    //
    //    eStatus:
    //      The actual status being reported.
    //
    //  Description:
    //      Reports the publication status.
    //
    //  See Also:
    //      EPublicationStatus
    //
    //==========================================================================
    virtual void EvPublicationStatus(IN IScePublisher* pPublisher,
                                     IN EPublicationStatus eStatus) = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IScePublisherMgr(){}

    // Summary:
    //  Destructor.
    virtual ~IScePublisherMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IScePublisherMgr(IN const IScePublisherMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IScePublisherMgr& operator=(IN const IScePublisherMgr& rSrc);


//-- Hidden Data Members
protected:
private:

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEPUBLISHERMGR_H

