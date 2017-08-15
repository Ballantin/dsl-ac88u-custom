/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*    Filename: vrgEndptGlobe.c
*
****************************************************************************
*    Description:
*
*      Voice Residential Gateway (VRG) Endpoint (around the) Globe
*      (Endpoint's international support module).
*
****************************************************************************/


/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/

#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <endpt.h>                     /* Endpoint include file */
#include <casCtl.h>                    /* CAS signalling support */
#include <classStm.h>                  /* CLASS state machine */
#include <hdspTone.h>                  /* DSP tone interface */
#include <xchgAssert.h>
#include <hapi_pte.h>
#include <vrgClass.h>
#include "vrgEndptGlobe.h"


/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/
/*
** Mapping form Endpoint ring signals around the Globe
*/
typedef struct EPGLOBE_PROV_RINGMAP
{
   EPSIG                 signal;   /* endpoint ring signal id */
   int                   provItem; /* provisioning id (if applicable) */
} EPGLOBE_PROV_RINGMAP;

typedef struct EPGLOBE_CAS_RINGMAP
{
   EPSIG                 signal;   /* endpoint ring signal id */
   CAS_CTL_RING_PATTERN  pattern;  /* CAS ring profile id */
} EPGLOBE_CAS_RINGMAP;

/*
** Country specific Mapping from Endpoint ring signals to provItem ID
*/

static const EPGLOBE_PROV_RINGMAP pEpGlobeRingMap[] =
{
   { EPSIG_RINGING,           EPPROV_RGCadence },
   { EPSIG_CALLID_RINGING,    EPPROV_RGCadence },
   { EPSIG_SPLASH,            EPPROV_RSCadence },
   { EPSIG_RING0,             EPPROV_R0Cadence },
   { EPSIG_RING1,             EPPROV_R1Cadence },
   { EPSIG_RING2,             EPPROV_R2Cadence },
   { EPSIG_RING3,             EPPROV_R3Cadence },
   { EPSIG_RING4,             EPPROV_R4Cadence },
   { EPSIG_RING5,             EPPROV_R5Cadence },
   { EPSIG_RING6,             EPPROV_R6Cadence },
   { EPSIG_RING7,             EPPROV_R7Cadence },
   { EPSIG_CALLID_RING0,      EPPROV_R0Cadence },
   { EPSIG_CALLID_RING1,      EPPROV_R1Cadence },
   { EPSIG_CALLID_RING2,      EPPROV_R2Cadence },
   { EPSIG_CALLID_RING3,      EPPROV_R3Cadence },
   { EPSIG_CALLID_RING4,      EPPROV_R4Cadence },
   { EPSIG_CALLID_RING5,      EPPROV_R5Cadence },
   { EPSIG_CALLID_RING6,      EPPROV_R6Cadence },
   { EPSIG_CALLID_RING7,      EPPROV_R7Cadence },
   { EPSIG_LAST,              -1               }
};

/*
** Country specific Mapping from Endpoint ring signals to CAS ringing profiles
*/

#if !VRG_COUNTRY_CFG_JAPAN
#define gEpRingMap_JAPAN  ((const EPGLOBE_CAS_RINGMAP*) NULL)
#else
static const EPGLOBE_CAS_RINGMAP pEpJapanRingMap[] =
{
   /* Japan ring profiles are not provisionable because of the line-reversals
   ** that they contain, which can not be controlled via existing MIBs */
   { EPSIG_RINGING,        CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RINGING, CAS_CTL_RING_JAPAN_IR },
   { EPSIG_SPLASH,         CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING0,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING1,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING2,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING3,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING4,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING5,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING6,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_RING7,          CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING0,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING1,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING2,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING3,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING4,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING5,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING6,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_CALLID_RING7,   CAS_CTL_RING_JAPAN_IR },
   { EPSIG_LAST,           0                     }
};
#endif



/*****************************************************************************
*  FUNCTION:   globeEndptGetRingPattern
*
*  PURPOSE:    Look up the ring pattern associated with the Endpt ring signal.
*
*  PARAMETERS: country    - country code
			   signal     - EPSIG id to look up the ring profile for      
*              pProfile   - pointer to CAS ring profile to return      
*              provItemId - pointer to prov item ID to return associated
*                            with this ring profile
*
*  RETURNS:    EPSTATUS
*
*  NOTES:      Given the Endpt ring signal, this function will return the static  
*              ring profile for special countries which does not support dynamic provisioning  
*              and return provItem ID for other countries.  
*****************************************************************************/
EPSTATUS globeEndptGetRingPattern( VRG_COUNTRY country, EPSIG signal, CAS_CTL_RING_PROFILE *pProfile, int *provItemId)
{
   const EPGLOBE_PROV_RINGMAP *pProvRingMap;
   const EPGLOBE_CAS_RINGMAP *pCASRingMap;
   /*
   ** Search the ring profile for the signal:
   */
   switch (country)
   {
	  case (VRG_COUNTRY_JAPAN):
	  {	 
	     pCASRingMap = pEpJapanRingMap;
         while ( (signal != pCASRingMap->signal) &&
                 (EPSIG_LAST != pCASRingMap->signal))
         {
            pCASRingMap++;
         }
	    
         if (pCASRingMap->signal != EPSIG_LAST)
         {	  
            /* Found the signal. Fill in the CAS profile info and the prov id */
            pProfile->pattern = pCASRingMap->pattern;
            pProfile->ringType = CAS_CTRL_STATIC_RING;
            pProfile->clidTxDelay = 0;          /* Let CAS profile handle clid delay */
		    *provItemId = -1;
		    return ( EPSTATUS_SUCCESS );
         }  
	   }
	break;

       default:
       {
	     pProvRingMap = pEpGlobeRingMap;
         XCHG_ASSERT( pProvRingMap != NULL);

         /* Search the corresponding prov id in the map for this signal */
         while ( (signal != pProvRingMap->signal) &&
                 (EPSIG_LAST != pProvRingMap->signal))
         {
            pProvRingMap++;
         }

         if (pProvRingMap->signal != EPSIG_LAST)
         {
            /* Found the signal. Fill in the prov id */
            pProfile = NULL;
            *provItemId = pProvRingMap->provItem;
            return ( EPSTATUS_SUCCESS );
	     } 
       }
	break;
	}
	
    VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "GLOBE ENDPT ERROR: prov id/CAS profile not found for: signal (%d)!!! ", signal));
    return ( EPSTATUS_UNKNOWN_TYPE );
}


