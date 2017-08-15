//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_MX64BITSCUSTOM_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MX64BITSCUSTOM_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

// Verify that this header file is included by MxConfig.h through MxDef.h only.
// This verification is performed because many macros used throughout this file
// are defined in PreMxConfig.h, which is only included by MxConfig.h. If we
// were to allow the direct inclusion of MxDefUnalignedType.h, we would loose
// the configuration done in PreMxConfig.h.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H) || !defined(MXG_MXDEF_H)
    #error This file must be included through MxConfig.h via MxDef.h only!
#endif

#if defined(MXD_64BITS_SUPPORT_DISABLE) && \
    !defined(MXD_OS_NONE)
    #error "Can only define MXD_64BITS_SUPPORT_DISABLE when MXD_OS_NONE is defined"
#endif

#if defined(MXD_64BITS_CUSTOM_TYPE) && \
    !defined(MXD_OS_NONE)
    #error "Can only define MXD_64BITS_CUSTOM_TYPE when MXD_OS_NONE is defined"
#endif

#if !defined(MXD_64BITS_SUPPORT_DISABLE) && defined(MXD_64BITS_CUSTOM_TYPE)

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Struct: SUInt64
//========================================
//
// Summary:
//   Class that implements non-native 64 bits basic support.
//
// Description:
//   SUInt64 is a Class that implements non-native 64 bits basic support.
//
// Location:
//   Basic/Mx64BitsCustom.h
//
// See Also:
//
//==EDOC========================================================================

#if !defined(__cplusplus)
typedef struct
{
    uint32_t m_uLow;
    uint32_t m_uHigh;
} SUInt64;

#else
struct SUInt64
{
// Published Interface
//---------------------
    SUInt64()
    {
        m_uHigh = 0;
        m_uLow  = 0;
    }

    SUInt64(uint32_t uLow)
    {
        m_uHigh = 0;
        m_uLow  = uLow;
    }

    SUInt64(uint32_t uHigh, uint32_t uLow)
    {
        m_uHigh = uHigh;
        m_uLow  = uLow;
    }

    SUInt64(const SUInt64& stUInt64)
    {
        *this = stUInt64;
    }

    SUInt64 operator=(const uint32_t uInteger)
    {
        m_uHigh = 0;
        m_uLow  = uInteger;
        return *this;
    }

    SUInt64 operator+=(const SUInt64& rstUInt64)
    {
        // the Destination is the object
        add(*this, rstUInt64);
        return *this;
    }

    SUInt64 operator+(const SUInt64& rstUInt64) const
    {
        return SUInt64(*this) += rstUInt64;
    }

    SUInt64 operator+(int nInteger) const
    {
        return SUInt64(0, nInteger) += *this;
    }

    // Pre-increment operator
    //------------------------
    SUInt64 operator++()
    {
        // Preincrement Lower Half of the 64 Bits Integer. If Lower Half is
        // zero then carry has occured and the Upper Half is incremented.
        if (++m_uLow == 0)
        {
            m_uHigh++;
        }
        return *this;
    }

    // Post-increment operator denoted by int param
    //-----------------------------------------------
    SUInt64 operator++(int)
    {
        // Keep orignal value that will be returned
        SUInt64 stTempUInt64 = *this;
        
        // Preincrement Lower Half of the 64 Bits Integer. If Lower Half is
        // zero then carry has occured and the Upper Half is incremented.
        if (++m_uLow == 0)
        {
            m_uHigh++;
        }
        
        return stTempUInt64;
    }

    SUInt64 operator-=(const SUInt64& rstUInt64)
    {
        // the Destination is the object
        sub(*this, rstUInt64);
        return *this;
    }

    SUInt64 operator-(const SUInt64& rstUInt64) const
    {
        return SUInt64(*this) -= rstUInt64;
    }

    // Pre-decrement operator
    //-----------------------
    SUInt64 operator--()
    {
        // Postdecrement Lower Half of the 64 Bits Integer. If Lower Half was
        // zero then borrow has occured and the Upper Half is decremented.
        if (m_uLow-- == 0)
        {
            m_uHigh--;
        }
        return *this;
    }

    // Post-decrement operator denoted by int param
    //-----------------------------------------------
    SUInt64 operator--(int)
    {
        // Keep orignal value that will be returned
        SUInt64 stTempUInt64 = *this;
        
        // Postdecrement Lower Half of the 64 Bits Integer. If Lower Half is
        // zero then carry has occured and the Upper Half is incremented.
        if (m_uLow-- == 0)
        {
            m_uHigh--;
        }
        
        return stTempUInt64;
    }

    SUInt64 operator&=(const SUInt64& rstUInt64)
    {
        m_uHigh &= rstUInt64.m_uHigh;
        m_uLow  &= rstUInt64.m_uLow;
        return *this;
    }

    SUInt64 operator&(const SUInt64& rstUInt64) const
    {
        return SUInt64(*this) &= rstUInt64;
    }

    bool operator!=(const SUInt64& rstUInt64) const
    {
        return (m_uHigh != rstUInt64.m_uHigh || m_uLow != rstUInt64.m_uLow);
    }

    bool operator==(const SUInt64& rstUInt64) const
    {
        return !(*this != rstUInt64);
    }

    bool operator<(const SUInt64& rstUInt64) const
    {
        if (m_uHigh < rstUInt64.m_uHigh)
        {
            return true;
        }
        if (m_uHigh > rstUInt64.m_uHigh)
        {
            return false;
        }
        return m_uLow < rstUInt64.m_uLow;
    }

    bool operator<=(const SUInt64& rstUInt64) const
    {
        return (*this == rstUInt64 || *this < rstUInt64);
    }

    bool operator>(const SUInt64& rstUInt64) const
    {
        return (*this != rstUInt64 && !(*this < rstUInt64));
    }

    bool operator>=(const SUInt64& rstUInt64) const
    {
        return (*this == rstUInt64 || *this > rstUInt64);
    }

    SUInt64 operator<<=(const SUInt64& rstUInt64)
    {
        shl(*this, rstUInt64.m_uLow);
        return *this;
    }

    SUInt64 operator<<=(const int count)
    {
        shl(*this, count);
        return *this;
    }

    SUInt64 operator<<(const SUInt64& rstUInt64) const
    {
        return SUInt64(*this) <<= rstUInt64.m_uLow;
    }

    SUInt64 operator<<(const int count) const
    {
        return SUInt64(*this) <<= count;
    }

    SUInt64 operator>>=(const SUInt64& rstUInt64)
    {
        shr(*this, rstUInt64.m_uLow);
        return *this;
    }

    SUInt64 operator>>=(const int count)
    {
        shr(*this, count);
        return *this;
    }

    SUInt64 operator>>(const SUInt64& rstUInt64) const
    {
        return SUInt64(*this) >>= rstUInt64.m_uLow;
    }

    SUInt64 operator>>(const int count) const
    {
        return SUInt64(*this) >>= count;
    }

    SUInt64 operator|=(const SUInt64& rstUInt64)
    {
        m_uHigh |= rstUInt64.m_uHigh;
        m_uLow  |= rstUInt64.m_uLow;
        return *this;
    }

    SUInt64 operator|(const SUInt64& rstUInt64) const
    {
        return SUInt64(*this) |= rstUInt64;
    }

    operator int() const
    {
        return (int)m_uLow;
    }

    operator uint32_t() const
    {
        return m_uLow;
    }

    operator bool() const
    {
        return (*this != SUInt64(0));
    }

// Hidden Methods
//----------------
private:
    void add(SUInt64& rstDest, const SUInt64& rstSrc)
    {
        // Add both Halves of the 64 Bits Integer. If Lower Half is
        // smaller after addition then carry has occured and the Upper
        // Half is incremented.
        rstDest.m_uLow  += rstSrc.m_uLow;
        rstDest.m_uHigh += rstSrc.m_uHigh;
        if (rstDest.m_uLow < rstSrc.m_uLow)
        {
            rstDest.m_uHigh++;
        }
    }

    void sub(SUInt64& rstDest, const SUInt64& rstSrc)
    {
        // If Destination Lower Half is smaller than Source Lower Half
        // then borrow will occur and the Upper Half is decremented
        if (rstDest.m_uLow < rstSrc.m_uLow)
        {
            rstDest.m_uHigh--;
        }
        // Substract both Halves of the 64 Bits Integer.
        rstDest.m_uLow  -= rstSrc.m_uLow;
        rstDest.m_uHigh -= rstSrc.m_uHigh;
    }

    void shl(SUInt64& stDest, const int uCount)
    {
        if (uCount >= 32)
        {
            // In this case we can perform an optimization
            // knowing that the Low part will end up being
            // '0' if shifted by 32 or more counts.

            // The High part is the Low part shifted the remaining count value
            stDest.m_uHigh = stDest.m_uLow << (uCount - 32);
            // No need to shift the Low part
            stDest.m_uLow = 0;
        }
        else
        {
            // In this case the result is a portion of the Low part
            // and the High part.

            // The High part is the old High part shifted left
            // ored with the bits from the Low part that
            // would have been shifted in the High part if
            // a native 64 bits operation was used.
            stDest.m_uHigh = (stDest.m_uHigh << uCount) |
                             (stDest.m_uLow  >> (32 - uCount));
            // Low part is simply shifted left
            stDest.m_uLow = stDest.m_uLow << uCount;
        }
    }

    void shr(SUInt64& stDest, const int uCount)
    {
        if (uCount >= 32)
        {
            // In this case we can perform an optimization
            // knowing that the high part will end up being
            // '0' if shifted by 32 or more counts.

            // The Low part is the High part shifted the remaining count value
            stDest.m_uLow = stDest.m_uHigh >> (uCount - 32);
            // No need to shift the High part
            stDest.m_uHigh = 0;
        }
        else
        {
            // In this case the result is a portion of the High part
            // and the Low part.

            // The Low part is the old Low part shifted right
            // ored with the bits from the High part that
            // would have been shifted in the Low part if
            // a native 64 bits operation was used.
            stDest.m_uLow = (stDest.m_uLow  >> uCount) |
                            (stDest.m_uHigh << (32 - uCount));
            // High part is simply shifted right
            stDest.m_uHigh = stDest.m_uHigh >> uCount;
        }
    }

// Hidden Data Members
//---------------------
private:
#if defined(MXD_LITTLE_ENDIAN)
    uint32_t m_uLow;
#endif
    uint32_t m_uHigh;
#if defined(MXD_BIG_ENDIAN)
    uint32_t m_uLow;
#endif
};
#endif // #if !defined(__cplusplus)

#endif // #if !defined(MXD_64BITS_SUPPORT_DISABLE) && defined(MXD_64BITS_CUSTOM_TYPE)
//M5T_INTERNAL_USE_END
#endif // MXG_MX64BITSCUSTOM_H

