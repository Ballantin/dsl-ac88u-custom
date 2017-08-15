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
#ifndef MXG_CSIPDEFAULTDATALOGGER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPDEFAULTDATALOGGER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_MXCONFIG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_DEFAULT_DATA_LOGGER
#endif

#ifdef MXD_SIPSTACK_ENABLE_DEFAULT_DATA_LOGGER

#ifndef MXG_ISIPDATALOGGER_H
#include "SipTransport/ISipDataLogger.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipDefaultDataLogger
//========================================
//
// Description:
//  This class is the implementation of a default packet logger. It only traces
//  the raw data, coming from and going to sockets, in the form of a string of
//  ASCII characters. The trace does not represent the exact data if the latter
//  contains, for instance, non-printable characters or wide characters.
//
// Location:
//   SipTransport/CSipDefaultDataLogger.h
//
// See Also:
//   ISipDataLogger
//
//==EDOC========================================================================
class CSipDefaultDataLogger : public ISipDataLogger
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipDefaultDataLogger();

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipDefaultDataLogger();


    //-- << ISipDataLogger >>.
    //-------------------------

    // Summary:
    //   Outputs the raw data as an ASCII string through a single trace.
    //-------------------------------------------------------------------
    virtual void LogRawData(IN EDirection eDir,
                            IN const CSocketAddr& rLocalAddr,
                            IN const CSocketAddr& rPeerAddr,
                            IN const CBlob& rRawData,
                            IN unsigned int uSize);

    // Summary:
    //   Empty method.
    //-----------------
    virtual void LogSipPacket(IN EDirection eDir,
                              IN const CSipPacket& rPacket);


//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipDefaultDataLogger(IN const CSipDefaultDataLogger& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipDefaultDataLogger& operator=(IN const CSipDefaultDataLogger& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifdef MXD_SIPSTACK_ENABLE_DEFAULT_DATA_LOGGER

#endif //-- #ifndef MXG_CSIPDEFAULTDATALOGGER_H


