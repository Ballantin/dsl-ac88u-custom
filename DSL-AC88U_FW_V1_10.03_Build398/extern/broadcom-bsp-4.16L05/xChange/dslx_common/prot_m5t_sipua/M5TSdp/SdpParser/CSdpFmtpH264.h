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
*      H.264 FMTP 
*
****************************************************************************/

#ifndef MXG_CSDPFMTPH264_H
#define MXG_CSDPFMTPH264_H

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


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSdpFmtpH264
//========================================
//
// Description:
//   This is the class that is an abstraction of an H.264 fmtp
//   attribute in a SDP packet as per RFC 3984 (RTP Payload Format for
//   H.264 Video).
//
//   The format of this fmtp attribute is based on the format of
//   CSdpFieldAttributeFmtp, but is more specific in its
//   format-specific-parameters.
//
//
// Location:
//   SdpParser/CSdpFmtpH264.h
//
// See Also:
//   CSdpFieldAttributeFmtp
//
//==EDOC========================================================================
class CSdpFmtpH264 : public CSdpFieldAttributeFmtp
{

// Enumerations
//-------------
public:
    // H.264 fmtp attribute parameters. These parameters are expressed as a
    // MIME media type string, in the form of a semicolon separated list of
    // parameter=value pairs.
    //-------------------------------------
    enum EParameter
    {

        ePROFILE_LEVEL_ID, // A hexadecimal representation of the following
                           // three bytes in the sequence parameter set NAL
                           // unit: 1) profile_idc, 2) a byte herein referred
                           // to as profile-iop, composed of the values of
                           // constraint_set0_flag, constraint_set1_flag,
                           // constraint_set2_flag, and reserved_zero_5bits
                           // in bit-significance order, starting from the
                           // most significant bit, and 3) level_idc.

        eMAX_MBPS,         // The value of max-mbps is an integer indicating
                           // the maximum macroblock processing rate in units
                           // of macroblocks per second.

        eMAX_FS,           // The value of max-fs is an integer indicating
                           // the maximum frame size in units of macroblocks.

        eMAX_CPB,          // The value of max-cpb is an integer indicating
                           // the maximum coded picture buffer size in units
                           // of 1000 bits for the VCL HRD parameters
                           // and in units of 1200 bits for the NAL HRD params.

        eMAX_DPB,          // The value of max-dpb is an integer indicating
                           // the maximum decoded picture buffer size in
                           // units of 1024 bytes.

        eMAX_BR,           // The value of max-br is an integer indicating
                           // the maximum video bit rate in units of 1000
                           // bits per second for the VCL HRD parameters
                           // and in units of 1200 bits per second for the
                           // NAL HRD parameters.

        eREDUNDANT_PIC_CAP,// When equal to 0, the parameter indicates that
                           // the receiver makes no attempt to use redundant
                           // coded pictures to correct incorrectly decoded
                           // primary coded pictures. When equal to 1, the
                           // receiver is capable of decoding any such
                           // redundant slice that covers a corrupted area
                           // in a primary decoded picture.

        eSPROP_PARAM_SETS, // This parameter MAY be used to convey any
                           // sequence and picture parameter set NAL units.

        ePARAMETER_ADD,    // This parameter MAY be used to signal whether
                           // the receiver of this parameter is allowed to
                           // add parameter sets in its signaling response
                           // using the sprop-parameter-sets parameter.

        ePACKET_MODE,      // This parameter signals the properties of an
                           // RTP payload type or the capabilities of a
                           // receiver implementation.

        eSPROP_INTERLEAVING_DEPTH,
                           // Specifies the maximum number of VCL NAL units
                           // that precede any VCL NAL unit in the NAL unit
                           // stream in transmission order and follow the VCL
                           // NAL unit in decoding order.

        eSPROP_DEINT_BUF_REQ,
                           // sprop-deint-buf-req signals the required size
                           // of the deinterleaving buffer for the NAL unit
                           // stream.

        eDEINT_BUF_CAP,    // This parameter signals the capabilities of a
                           // receiver implementation and indicates the
                           // amount of deinterleaving buffer space in units
                           // of bytes that the receiver has available for
                           // reconstructing the NAL unit decoding order.

        eSPROP_INIT_BUF_TIME,
                           // The parameter signals the initial buffering
                           // time that a receiver MUST buffer before
                           // starting decoding to recover the NAL unit
                           // decoding order from the transmission order.

        eSPROP_MAX_DON_DIFF,
                           // Integer in the range of 0 to 32767, inclusive.

        eMAX_RCMD_NALU_SIZE,
                           // The value of the parameter indicates the largest
                           // NALU size in bytes that the receiver can handle
                           // efficiently.
        eLEVEL_ASYMMETRY_ALLOWED,
                           // The value of the parameter indicates whether
                           // level asymmetry, i.e. sending media encoded at
                           // a different level in the offerer-to-answerer
                           // direction than the level in the answerer-to-offerer
                           // direction, is allowed.

    };


    // H.264 profile types. All decoders conforming to a specific profile
    // have to support all features of that profile. Profile values are
    // defined in the H.264 spec.
    enum EProfile
    {
       eBASELINE  = 66,
       eMAIN      = 77,
       eEXTENDED  = 88
    };


    // Level of H.264 bitstream operation, specifying the upper limits for the
    // picture size (in macroblocks), the decoderprocessing rate (in macroblocks
    // per second), the size of the multi-picture buffers, the video bit-rate
    // and the video buffer size complexity of the decoding process.
    enum ELevel
    {
       eLEVEL_1_0 = 10,
       eLEVEL_1_1 = 11,
       eLEVEL_1_2 = 12,
       eLEVEL_1_3 = 13,
       eLEVEL_2_0 = 20,
       eLEVEL_2_1 = 21,
       eLEVEL_2_2 = 22,
       eLEVEL_3_0 = 30,
       eLEVEL_3_1 = 31,
       eLEVEL_3_2 = 32,
       eLEVEL_4_0 = 40,
       eLEVEL_4_1 = 41,
       eLEVEL_4_2 = 42,
       eLEVEL_5_0 = 50,
       eLEVEL_5_1 = 51
    };


    // If set, constraint 0, 1, or 2 of the Baseline profile, the Main profile,
    // or the Extended profile, respectively, are obeyed in the NAL unit
    // stream.
    enum EConstrait
    {
       eSET0 = 0x80,
       eSET1 = 0x40,
       eSET2 = 0x20
    };


    /* H.264 RTP stream packetization mode types. */
    enum EPacketizationMode
    {
       /* Primarily intended for low-delay applications that are compatible with
       ** systems using ITU-T Recommendation H.241. Fragmented and aggregated
       ** packets not supported. */
       eSINGLE_NAL         = 0,

       /* It is primarily intended for low-delay applications. Fragmented and
       ** aggregated packets are supported. */
       eNON_INTERLEAVED    = 1,

       /* The interleaved mode is targeted for systems that do not require very
       ** low end-to-end latency. The interleaved mode allows transmission of
       ** NAL units out of NAL unit decoding order. */
       eINTERLEAVED        = 2
    };


    static const char* ms_apszParameterMap[];



//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default destructor.
    //----------------------
    CSdpFmtpH264();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSdpFmtpH264(IN const CSdpFmtpH264& rFrom);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSdpFmtpH264();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSdpFmtpH264& operator=(IN const CSdpFmtpH264& rFrom);


    //-- << Public interface >>
    //-------------------------------------------------

    // Summary:
    //  Generates copy of the current object.
    //----------------------------------------
    virtual GO CSdpFieldAttributeFmtp* GenerateCopy() const;

    // Summary:
    //  Accessors
    //---------------------------------------------
    EProfile GetProfile(void);
    ELevel GetLevel(void);
    char GetConstraintSet0(void);
    char GetConstraintSet1(void);
    char GetConstraintSet2(void);
    uint8_t GetConstraintSets(void);
    int GetMaxMbps(void);
    int GetMaxBr(void);
    EPacketizationMode GetPacketizationMode(void);
    bool GetParameterSetsHex(OUT char* buf, IN int maxBufLen);
    bool GetParameterAdd(void);

    void SetProfile(IN EProfile profile);
    void SetLevel(IN ELevel level);
    void SetConstraintSet0(IN char bConstrait);
    void SetConstraintSet1(IN char bConstrait);
    void SetConstraintSet2(IN char bConstrait);
    void SetConstraintSets(IN uint8_t constraits);
    void SetMaxMbps(IN int maxMbps);
    void SetMaxBr(IN int maxBr);
    void SetPacketizationMode(IN EPacketizationMode mode);
    void SetParameterSetsHex(IN char* buf);
    void SetParameterAdd(IN bool bParameterAdd);



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

    void ParseParameterSetsBase64(IN char* buf);


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
    bool operator==(IN const CSdpFmtpH264& rFrom) const;


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

    // fmtp parameters.
    //----------------------------------------------------------------------
    EProfile            m_profile;
    uint8_t             m_constraints;
    ELevel              m_level;
    int                 m_maxMbps;
    int                 m_maxBr;
    EPacketizationMode  m_packetizationMode;
    CString             m_parameterSetsHex;
    CString             m_parameterSetsBase64;
    bool                m_parameterAdd;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==SDOC========================================================================
//==
//==  Set accessor methods.
//==
//==============================================================================
//
//  Parameters:
//      Specified fmtp parameter value.
//
//  Description:
//      Sets specified fmtp parameter value.
//
//  See Also:
//
//
//==EDOC========================================================================
inline void CSdpFmtpH264::SetProfile(IN CSdpFmtpH264::EProfile profile)
{
   m_profile = profile;
}

inline void CSdpFmtpH264::SetLevel(IN CSdpFmtpH264::ELevel level)
{
   m_level = level;
}

inline void CSdpFmtpH264::SetConstraintSet0(IN char bConstrait)
{
   if ( bConstrait )
   {
      m_constraints |= eSET0;
   }
   else
   {
      m_constraints &= (~eSET0);
   }
}

inline void CSdpFmtpH264::SetConstraintSet1(IN char bConstrait)
{
   if ( bConstrait )
   {
      m_constraints |= eSET1;
   }
   else
   {
      m_constraints &= (~eSET1);
   }
}

inline void CSdpFmtpH264::SetConstraintSet2(IN char bConstrait)
{
   if ( bConstrait )
   {
      m_constraints |= eSET2;
   }
   else
   {
      m_constraints &= (~eSET2);
   }
}

inline void CSdpFmtpH264::SetConstraintSets(IN uint8_t constraints)
{
   m_constraints = constraints;
}

inline void CSdpFmtpH264::SetMaxMbps(IN int maxMbps)
{
   m_maxMbps = maxMbps;
}

inline void CSdpFmtpH264::SetMaxBr(IN int maxBr)
{
   m_maxBr = maxBr;
}

inline void CSdpFmtpH264::SetPacketizationMode(IN CSdpFmtpH264::EPacketizationMode mode)
{
   m_packetizationMode = mode;
}

inline void CSdpFmtpH264::SetParameterAdd(IN bool bParameterAdd)
{
   m_parameterAdd = bParameterAdd;
}



//==SDOC========================================================================
//==
//==  Get accessor methods.
//==
//==============================================================================
//
//  Returns:
//      Specified fmtp parameter value.
//
//  Description:
//      Gets specified fmtp parameter value.
//
//  See Also:
//
//
//==EDOC========================================================================
inline CSdpFmtpH264::EProfile CSdpFmtpH264::GetProfile(void)
{
   return ( m_profile );
}

inline CSdpFmtpH264::ELevel CSdpFmtpH264::GetLevel(void)
{
   return ( m_level );
}

inline char CSdpFmtpH264::GetConstraintSet0(void)
{
   return (( m_constraints & eSET0 ) == eSET0);
}

inline char CSdpFmtpH264::GetConstraintSet1(void)
{
   return (( m_constraints & eSET1 ) == eSET1);
}

inline char CSdpFmtpH264::GetConstraintSet2(void)
{
   return (( m_constraints & eSET2 ) == eSET2);
}

inline uint8_t CSdpFmtpH264::GetConstraintSets(void)
{
   return ( m_constraints );
}

inline int CSdpFmtpH264::GetMaxMbps(void)
{
   return ( m_maxMbps );
}

inline int CSdpFmtpH264::GetMaxBr(void)
{
   return ( m_maxBr );
}

inline CSdpFmtpH264::EPacketizationMode CSdpFmtpH264::GetPacketizationMode(void)
{
   return ( m_packetizationMode );
}

inline bool CSdpFmtpH264::GetParameterSetsHex(OUT char* buf, IN int maxBufLen)
{
    int numBytesWritten;

    numBytesWritten = MxSnprintf(OUT buf, maxBufLen, "%s", m_parameterSetsHex.CStr());
    buf[ maxBufLen - 1 ] = '\0';

    return ( numBytesWritten < maxBufLen );
}

inline bool CSdpFmtpH264::GetParameterAdd(void)
{
   return ( m_parameterAdd );
}


//==SDOC========================================================================
//==
//==
//==
//==============================================================================
//
//  Returns:
//
//
//  Description:
//
//
//  See Also:
//
//
//==EDOC========================================================================
inline GO CSdpFieldAttributeFmtp* CSdpFmtpH264::GenerateCopy() const
{
    return MX_NEW(CSdpFmtpH264)(*this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined( MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT )

#endif // #ifndef MXG_CSDPFMTPH264_H
