/*****************************************************************************
*
*  Copyright 2005  Broadcom
*  All Rights Reserved
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California  92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
****************************************************************************
*    Description:
*
*      H.263 FMTP 
*
****************************************************************************/
#ifndef MXG_CSDPFMTPH263_H
#define MXG_CSDPFMTPH263_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
//----------------------------
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h" // MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT
#endif

#if defined( MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT )

//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSDPFIELDATTRIBUTEFMTP_H
#include "SdpParser/CSdpFieldAttributeFmtp.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#define SDP_FMTP_H263_MIN_PROFILE   0
#define SDP_FMTP_H263_MAX_PROFILE   10
#define SDP_FMTP_H263_MIN_LEVEL     0
#define SDP_FMTP_H263_MAX_LEVEL     100

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSdpFmtpH263
//========================================
//
// Description:
//   This is the class that is an abstraction of an H.263 fmtp
//   attribute in a SDP packet as per RFC 3555 (MIME Type Registration of
//   RTP Payload Formats).
//
//   The format of this fmtp attribute is based on the format of
//   CSdpFieldAttributeFmtp, but it more specific in its
//   format-specific-parameters.
//
//  <PRE>
//      h263-event-fmtp-attribute   = "fmtp:" format "profile="profile-param; "level="level-param
//      profile-param               = 1*DIGIT
//      level-param                 = 1*DIGIT
//  </PRE>
//
// Location:
//   SdpParser/CSdpFmtpH263.h
//
// See Also:
//   CSdpFieldAttributeFmtp
//
//==EDOC========================================================================
class CSdpFmtpH263 : public CSdpFieldAttributeFmtp
{

// Enumerations
//-------------
public:
    // H.263 fmtp attribute parameters. These parameters are expressed as a
    // MIME media type string, in the form of a semicolon separated list of
    // parameter=value pairs.
    //-------------------------------------
    enum EParameter
    {

        ePROFILE,       // H.263 profile number, in the range 0 through 10,
                        // specifying the supported H.263 annexes/subparts.

        eLEVEL          // Level of bitstream operation, in the range 0
                        // through 100, specifying the level of computational
                        // complexity of the decoding process.
    };

    static const char* ms_apszParameterMap[];

    static const int ms_defaultProfile;
    static const int ms_defaultLevel;


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default destructor.
    //----------------------
    CSdpFmtpH263();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSdpFmtpH263(IN const CSdpFmtpH263& rFrom);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSdpFmtpH263();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSdpFmtpH263& operator=(IN const CSdpFmtpH263& rFrom);


    //-- << Public interface >>
    //-------------------------------------------------

    // Summary:
    //  Generates copy of the current object.
    //----------------------------------------
    virtual GO CSdpFieldAttributeFmtp* GenerateCopy() const;

    // Summary:
    //  Gets H.263 profile supported.
    //---------------------------------------------
    int GetProfile(void);

    // Summary:
    //  Sets H.263 level supported.
    //------------------------------------------------------------
    int GetLevel(void);


    // Summary:
    //  Sets H.263 profile supported.
    //---------------------------------------------
    void SetProfile(IN int profile);

    // Summary:
    //  Sets H.263 level supported.
    //------------------------------------------------------------
    void SetLevel(IN int level);


    //-- << CSdpFieldAttributeFmtp >>
    //-------------------------------------------------

    // Summary:
    //  Serializes the Fmtp field value in m_strValue and returns its value.
    //  Also used by Serialize to add the format-specific-parameters to the
    //  blob.
    //-----------------------------------------------------------------------
    virtual const char* GetValue() const;


    //-- << CSdpParser >>
    //-------------------------------------------------

    // Summary:
    //  Parse the data. Can return any type of EParserResult.
    //--------------------------------------------------------
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //  Validate and check the validity of the parsed data.
    //------------------------------------------------------
    virtual bool Validate();

    // Summary:
    //  Reset the data in the parser.
    //--------------------------------
    virtual void Reset();


//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CSdpFmtpH263& rFrom) const;


    // MXI_PATCH (rvigneault 2005-08-05): This is to avoid the method to be
    // called by the application. The method could be called if the object is
    // cast into a CSdpFieldAttributeFmtp. A better way to do this would be not
    // to use CSdpFieldAttributeFmtp as basic class (a common class should be
    // the basic class of both).
    //--------------------------------------------------------------------------

    // Summary:
    //  Set the value of the Fmtp field attribute to the string.
    //-----------------------------------------------------------
    virtual void SetValue(IN const char* pszValue);


//-- Hidden Data Members
//------------------------
private:

    // Profile and level supported by H.263 codec.
    //----------------------------------------------------------------------
    int m_profile;
    int m_level;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==SDOC========================================================================
//==
//==  GetProfileSupport
//==
//==============================================================================
//
//  Parameters:
//    profile:
//      H.263 profile number, in the range 0 through 10, specifying the
//      supported H.263 annexes/subparts.
//
//  Description:
//      Sets profile supported by H.263 codec.
//
//  See Also:
//
//
//==EDOC========================================================================
inline void CSdpFmtpH263::SetProfile(IN int profile)
{
   // H.263 supports profiles 0-10.
   if (( profile >= SDP_FMTP_H263_MIN_PROFILE ) && ( profile <= SDP_FMTP_H263_MAX_PROFILE ))
   {
      m_profile = profile;
   }
}

//==SDOC========================================================================
//==
//==  SetLevelSupport
//==
//==============================================================================
//
//  Parameters:
//    level:
//      Level of bitstream operation, in the range 0 through 100, specifying
//      the level of computational complexity of the decoding process.
//
//  Description:
//      Sets level supported by H.263 codec.
//
//  See Also:
//
//
//==EDOC========================================================================
inline void CSdpFmtpH263::SetLevel(IN int level)
{
   // H.263 supports levels 0-100.
   if (( level >= SDP_FMTP_H263_MIN_LEVEL ) && ( level <= SDP_FMTP_H263_MAX_LEVEL ))
   {
      m_level = level;
   }
}

//==SDOC========================================================================
//==
//==  GetProfileSupport
//==
//==============================================================================
//
//  Returns:
//      H.263 profile number, in the range 0 through 10, specifying the
//      supported H.263 annexes/subparts.
//
//  Description:
//      Gets profile supported by H.263 codec.
//
//  See Also:
//
//
//==EDOC========================================================================
inline int CSdpFmtpH263::GetProfile(void)
{
    return ( m_profile );
}

//==SDOC========================================================================
//==
//==  GetLevelSupport
//==
//==============================================================================
//
//  Returns:
//      Level of bitstream operation, in the range 0 through 100, specifying
//      the level of computational complexity of the decoding process.
//
//  Description:
//      Gets level supported by H.263 codec.
//
//  See Also:
//
//
//==EDOC========================================================================
inline int CSdpFmtpH263::GetLevel(void)
{
    return ( m_level );
}


inline GO CSdpFieldAttributeFmtp* CSdpFmtpH263::GenerateCopy() const
{
    return MX_NEW(CSdpFmtpH263)(*this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined( MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT )

#endif // #ifndef MXG_CSDPFMTPH263_H
