// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
//=================================================================================
// Copyright © 2007-2008 Broadcom
//
// This program  is the  proprietary software  of Broadcom  Corporation and/or  its
// licensors, and may only be used, duplicated, modified or distributed pursuant to
// the  terms and  conditions of  a separate,  written license  agreement executed
// between you and Broadcom (an "Authorized  License").  Except as set forth in  an
// Authorized License, Broadcom  grants no license  (express or implied),  right to
// use, or waiver of any kind with respect to the Software, and Broadcom  expressly
// reserves all rights in and to the Software and all intellectual property  rights
// therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
// SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
// USE OF THE SOFTWARE.
//
// Except as expressly set forth in the Authorized License,
//
// 1.      This  program,  including  its  structure,  sequence  and  organization,
// constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
// reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
// information only  in connection  with your  use of  Broadcom integrated  circuit
// products.
//
// 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
// AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
// WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
// THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
// TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
// OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
// CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
// PERFORMANCE OF THE SOFTWARE.
//
// 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
// LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
// EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
// OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
// PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
// SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
// REMEDY.
//=================================================================================

//=================================================================================
//
//    Description:
//
//     The Client Event Data class stores important data received on client events.
//
//=================================================================================

#ifndef MXG_CSCECLIENTEVENTDATA_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCECLIENTEVENTDATA_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members.

//-- Interface Realized and/or Parent.

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CNameAddr;
class IUaSspCall;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceClientEventData
//==============================================================================
//
// Summary:
//   Container for data related to client events.
//
// Description:
//   Holds data for client events that might have happen while a response has
//   been received.
//
// Location:
//   Custom/SceCoreComponents/CSceClientEventData.h
//
//==============================================================================
class CSceClientEventData
{
//-- Published Interface.
public:
    // Summary:
    //  Constructor.
    CSceClientEventData();

    // Summary:
    //  Destructor.
    virtual ~CSceClientEventData();

    //-- << EvAssertedIdentity related methods >>
    void StoreEvAssertedIdParam(IN TO CNameAddr* pSipIdentity,
                                IN TO CNameAddr* pTelIdentity);
    void RetrieveEvAssertedIdParam(OUT GO CNameAddr*& rpNaiSip,
                                   OUT GO CNameAddr*& rpNaiTel);

//-- Hidden Methods.
protected:
private:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSceClientEventData(IN const CSceClientEventData& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceClientEventData& operator=(IN const CSceClientEventData& rSrc);

//-- Hidden Data Members.
protected:
private:

    // EvAssertedIdentity data member.
    CNameAddr* m_pSipIdentity;
    CNameAddr* m_pTelIdentity;
};
//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCECLIENTEVENTDATA_H
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173
