//////////////////////////////////////////////////////////////////////////////
// Copyright 2005 - 2007 Broadcom.  All rights reserved.
//
// This program is the proprietary software of Broadcom and/or
// its licensors, and may only be used, duplicated, modified or distributed
// pursuant to the terms and conditions of a separate, written license
// agreement executed between you and Broadcom (an "Authorized License").
// Except as set forth in an Authorized License, Broadcom grants no license
// (express or implied), right to use, or waiver of any kind with respect to
// the Software, and Broadcom expressly reserves all rights in and to the
// Software and all intellectual property rights therein.  IF YOU HAVE NO
// AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY
// WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF
// THE SOFTWARE.
//
// Except as expressly set forth in the Authorized License,
// 1. This program, including its structure, sequence and organization,
//    constitutes the valuable trade secrets of Broadcom, and you shall use
//    all reasonable efforts to protect the confidentiality thereof, and to
//    use this information only in connection with your use of Broadcom
//    integrated circuit products.
// 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
//    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
//    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
//    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
//    IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS
//    FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS,
//    QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU
//    ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
// 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
//    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
//    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
//    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
//    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
//    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
//    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
//    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
//////////////////////////////////////////////////////////////////////////////


//
//
//==============================================================================
//==EDOC========================================================================
#ifndef __CBOSSINGLETON_H__INCLUDED__
#define __CBOSSINGLETON_H__INCLUDED__

#if defined( MXD_OS_BOS_ECOS )

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

#include <bosCfg.h>
#include <bosTypes.h>

MX_NAMESPACE_START(MXD_GNS)


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//===========================================================================
//====  NEW TYPE DEFINITIONS  ===============================================
//===========================================================================

//==SDOC========================================================================
// Class: CBosSingleton
//=======================
//
//  Description:
//      CBosSingleton class defines a singleton object that is used to wrap up
//      BOS calls.
//
//==EDOC========================================================================
class CBosSingleton
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CBosSingleton( void );

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CBosSingleton( void );

    // Summary:
    //  Returns the pointer to the singleton BOS wrapper object.  Most
    //  objects in the system will use this method rather than being passed a
    //  pointer to the object.  The singleton pointer will be set up in the
    //  base-class constructor.
    //
    //  \note  You must not delete this object!
    //
    //  \note   You should not store a pointer to the object that is returned,
    //          since it may be deleted and replaced with a new one.
    //
    //  \return
    //       A pointer to the BOS wrapper object that should be used by the
    //       system.
    ///
    //---------------
    static CBosSingleton *GetSingletonInstance( void );

//-- Hidden Data Members
//------------------------
private:

    /// This is the pointer to the singleton instance of this class, which will
    /// be returned by GetSingletonInstance().
    static CBosSingleton *mp_singletonInstance;

private:

    // Summary:
    //  Copy constructor - not supported.
    //-----------------------------------
    CBosSingleton(const CBosSingleton &otherInstance);

    // Summary:
    //  Assignement operator - not supported.
    //---------------------------------------
    CBosSingleton & operator = (const CBosSingleton &otherInstance);

};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- MXD_OS_BOS_ECOS

#endif //-- __CBOSSINGLETON_H__INCLUDED__
