/****************************************************************************
*  
*  Copyright © 2008 Broadcom Corporation
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
****************************************************************************
*
*    Filename: flexiCalc6828.c
*
****************************************************************************
*    Description:
*
*     Implements abstraction interface for FlexiCalc generated header files
*     which contain APM specific configuration values for the 6828.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

/* General header files */
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <vrgCountry.h>
#include <string.h>
#include <flexiCalc6828.h>
#include <boardparms_voice.h>
#include <boardHalProv.h>

/* Flexicalc header files for Si3239 */
#include <FlexiCalc_BCM6828_Si3239_NORTH_AMERICA.h>
#include <FlexiCalc_BCM6828_Si3239_JAPAN.h>
#include <FlexiCalc_BCM6828_Si3239_UK.h>
#include <FlexiCalc_BCM6828_Si3239_SWEDEN.h>
#include <FlexiCalc_BCM6828_Si3239_GERMANY.h>
#include <FlexiCalc_BCM6828_Si3239_FRANCE.h>
#include <FlexiCalc_BCM6828_Si3239_TR57.h>
#include <FlexiCalc_BCM6828_Si3239_BELGIUM.h>
#include <FlexiCalc_BCM6828_Si3239_FINLAND.h>
#include <FlexiCalc_BCM6828_Si3239_ITALY.h>
#include <FlexiCalc_BCM6828_Si3239_ETSI.h>
#include <FlexiCalc_BCM6828_Si3239_NETHERLANDS.h>
#include <FlexiCalc_BCM6828_Si3239_SWITZERLAND.h>
#include <FlexiCalc_BCM6828_Si3239_BRAZIL.h>
#include <FlexiCalc_BCM6828_Si3239_DENMARK.h>
#include <FlexiCalc_BCM6828_Si3239_CHILE.h>
#include <FlexiCalc_BCM6828_Si3239_HUNGARY.h>
#include <FlexiCalc_BCM6828_Si3239_POLAND.h>
#include <FlexiCalc_BCM6828_Si3239_AUSTRIA.h>
#include <FlexiCalc_BCM6828_Si3239_IRELAND.h>
#include <FlexiCalc_BCM6828_Si3239_CZECH.h>
#include <FlexiCalc_BCM6828_Si3239_SLOVAKIA.h>
#include <FlexiCalc_BCM6828_Si3239_ROMANIA.h>
#include <FlexiCalc_BCM6828_Si3239_SLOVENIA.h>
#include <FlexiCalc_BCM6828_Si3239_MEXICO.h>
#include <FlexiCalc_BCM6828_Si3239_AUSTRALIA.h>
#include <FlexiCalc_BCM6828_Si3239_UNITED_ARAB_EMIRATES.h>


#include <FlexiCalc_BCM6828_Si3239_CHINA.h>
#include <FlexiCalc_BCM6828_Si3239_SPAIN.h>
#include <FlexiCalc_BCM6828_Si3239_NEW_ZEALAND.h>
#include <FlexiCalc_BCM6828_Si3239_NORWAY.h>
#include <FlexiCalc_BCM6828_Si3239_TAIWAN.h>
#include <FlexiCalc_BCM6828_Si3239_INDIA.h>

#include <FlexiCalc_BCM6828_Si3239_WB_CHINA.h>
#include <FlexiCalc_BCM6828_Si3239_WB_NORTH_AMERICA.h>

/* Flexicalc header files for Si32392 */
#include <FlexiCalc_BCM6828_Si32392_NORTH_AMERICA.h>
#include <FlexiCalc_BCM6828_Si32392_JAPAN.h>
#include <FlexiCalc_BCM6828_Si32392_UK.h>
#include <FlexiCalc_BCM6828_Si32392_SWEDEN.h>
#include <FlexiCalc_BCM6828_Si32392_GERMANY.h>
#include <FlexiCalc_BCM6828_Si32392_FRANCE.h>
#include <FlexiCalc_BCM6828_Si32392_TR57.h>
#include <FlexiCalc_BCM6828_Si32392_BELGIUM.h>
#include <FlexiCalc_BCM6828_Si32392_FINLAND.h>
#include <FlexiCalc_BCM6828_Si32392_ITALY.h>
#include <FlexiCalc_BCM6828_Si32392_ETSI.h>
#include <FlexiCalc_BCM6828_Si32392_NETHERLANDS.h>
#include <FlexiCalc_BCM6828_Si32392_SWITZERLAND.h>
#include <FlexiCalc_BCM6828_Si32392_BRAZIL.h>
#include <FlexiCalc_BCM6828_Si32392_DENMARK.h>
#include <FlexiCalc_BCM6828_Si32392_CHILE.h>
#include <FlexiCalc_BCM6828_Si32392_HUNGARY.h>
#include <FlexiCalc_BCM6828_Si32392_POLAND.h>
#include <FlexiCalc_BCM6828_Si32392_AUSTRIA.h>
#include <FlexiCalc_BCM6828_Si32392_IRELAND.h>
#include <FlexiCalc_BCM6828_Si32392_CZECH.h>
#include <FlexiCalc_BCM6828_Si32392_SLOVAKIA.h>
#include <FlexiCalc_BCM6828_Si32392_ROMANIA.h>
#include <FlexiCalc_BCM6828_Si32392_SLOVENIA.h>
#include <FlexiCalc_BCM6828_Si32392_MEXICO.h>
#include <FlexiCalc_BCM6828_Si32392_AUSTRALIA.h>
#include <FlexiCalc_BCM6828_Si32392_UNITED_ARAB_EMIRATES.h>


#include <FlexiCalc_BCM6828_Si32392_CHINA.h>
#include <FlexiCalc_BCM6828_Si32392_SPAIN.h>
#include <FlexiCalc_BCM6828_Si32392_NEW_ZEALAND.h>
#include <FlexiCalc_BCM6828_Si32392_NORWAY.h>
#include <FlexiCalc_BCM6828_Si32392_TAIWAN.h>
#include <FlexiCalc_BCM6828_Si32392_INDIA.h>

#include <FlexiCalc_BCM6828_Si32392_WB_CHINA.h>
#include <FlexiCalc_BCM6828_Si32392_WB_NORTH_AMERICA.h>

/* Flexicalc header files for L9530 */
#include <FlexiCalc_BCM6828_L9530_NORTH_AMERICA.h>
#include <FlexiCalc_BCM6828_L9530_JAPAN.h>
#include <FlexiCalc_BCM6828_L9530_UK.h>
#include <FlexiCalc_BCM6828_L9530_SWEDEN.h>
#include <FlexiCalc_BCM6828_L9530_GERMANY.h>
#include <FlexiCalc_BCM6828_L9530_FRANCE.h>
#include <FlexiCalc_BCM6828_L9530_TR57.h>
#include <FlexiCalc_BCM6828_L9530_BELGIUM.h>
#include <FlexiCalc_BCM6828_L9530_FINLAND.h>
#include <FlexiCalc_BCM6828_L9530_ITALY.h>
#include <FlexiCalc_BCM6828_L9530_ETSI.h>
#include <FlexiCalc_BCM6828_L9530_NETHERLANDS.h>
#include <FlexiCalc_BCM6828_L9530_SWITZERLAND.h>
#include <FlexiCalc_BCM6828_L9530_BRAZIL.h>
#include <FlexiCalc_BCM6828_L9530_DENMARK.h>
#include <FlexiCalc_BCM6828_L9530_CHILE.h>
#include <FlexiCalc_BCM6828_L9530_HUNGARY.h>
#include <FlexiCalc_BCM6828_L9530_POLAND.h>
#include <FlexiCalc_BCM6828_L9530_AUSTRIA.h>
#include <FlexiCalc_BCM6828_L9530_IRELAND.h>
#include <FlexiCalc_BCM6828_L9530_CZECH.h>
#include <FlexiCalc_BCM6828_L9530_SLOVAKIA.h>
#include <FlexiCalc_BCM6828_L9530_ROMANIA.h>
#include <FlexiCalc_BCM6828_L9530_SLOVENIA.h>
#include <FlexiCalc_BCM6828_L9530_MEXICO.h>
#include <FlexiCalc_BCM6828_L9530_AUSTRALIA.h>
#include <FlexiCalc_BCM6828_L9530_UNITED_ARAB_EMIRATES.h>


#include <FlexiCalc_BCM6828_L9530_CHINA.h>
#include <FlexiCalc_BCM6828_L9530_NEW_ZEALAND.h>
#include <FlexiCalc_BCM6828_L9530_SPAIN.h>
#include <FlexiCalc_BCM6828_L9530_NORWAY.h>
#include <FlexiCalc_BCM6828_L9530_TAIWAN.h>
#include <FlexiCalc_BCM6828_L9530_INDIA.h>

#include <FlexiCalc_BCM6828_L9530_WB_CHINA.h>
#include <FlexiCalc_BCM6828_L9530_WB_NORTH_AMERICA.h>
 

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* Create a static array of each country's FlexiCalc parameters for Si3239 */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) flexicalc##country##Archive3239,
static const APM6828_FLEXICALC_CFG *gFlexiValues3239[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

/* Create a static array of each country's FlexiCalc parameters for Si32392 */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) flexicalc##country##Archive32392,
static const APM6828_FLEXICALC_CFG *gFlexiValues32392[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

/* Create a static array of each country's FlexiCalc parameters for Le9530 */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) flexicalc##country##Archive9530,
static const APM6828_FLEXICALC_CFG *gFlexiValues9530[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   flexiCalcGetCfg6828
**
** PURPOSE:    Get APM flexicalc config.
**
** PARAMETERS: country     Current country code
**             sampleRate  APM sample rate
**
** RETURNS:    Pointer to flexivalue structure
**
*****************************************************************************
*/
const APM6828_FLEXICALC_CFG* flexiCalcGetCfg6828
(
   VRG_COUNTRY country,
   VRG_UINT32  sampleRate,
   BP_VOICE_DEVICE_TYPE type
)
{
   const APM6828_FLEXICALC_CFG *flexiCfg=0;

   XCHG_ASSERT( country < VRG_COUNTRY_MAX );

   XCHG_ASSERT( type == BP_VD_SILABS_3239 || type == BP_VD_SILABS_32392 || type == BP_VD_ZARLINK_9530 );

   if (sampleRate == 8000)
   {
      switch( type )
      {
        case BP_VD_ZARLINK_9530:
          flexiCfg = gFlexiValues9530[ country ];
          break;
        case BP_VD_SILABS_3239:
          flexiCfg = gFlexiValues3239[ country ];
          break;
        case BP_VD_SILABS_32392:
          flexiCfg = gFlexiValues32392[ country ];
          break;
        default:
             XCHG_ASSERT( 0 );
      }
   }
   else
   {
      /* wideband flexiCalc values available for a few countries only */
      switch( country )
      {
         case VRG_COUNTRY_CHINA:
         {
            switch( type )
            {
              case BP_VD_ZARLINK_9530:
                flexiCfg = flexicalcWBCHINAArchive9530;
                break;
              case BP_VD_SILABS_3239:
                flexiCfg = flexicalcWBCHINAArchive3239;
                break;
              case BP_VD_SILABS_32392:
                flexiCfg = flexicalcWBCHINAArchive3239;
                break;
              default:
                XCHG_ASSERT( 0 );
            }
         }
         break;

         default: 
         {
            /* Default to North American WideBand settings for all countries with NO wideband support */
            switch( type )
            {
              case BP_VD_ZARLINK_9530:
                flexiCfg = flexicalcWBNORTH_AMERICAArchive9530;
                break;
              case BP_VD_SILABS_3239:
                flexiCfg = flexicalcWBNORTH_AMERICAArchive3239;
                break;
              case BP_VD_SILABS_32392:
                flexiCfg = flexicalcWBNORTH_AMERICAArchive32392;
                break;
              default:
                XCHG_ASSERT( 0 );
            }
         }
         break;
      }
            
   }

   XCHG_ASSERT( flexiCfg != NULL );

   /* Make sure this is the correct configuration for the corresponding SLIC */
   if ( (type == BP_VD_SILABS_3239 && flexiCfg->slic_type != 0x3239) || (type == BP_VD_SILABS_32392 && flexiCfg->slic_type != 0x32392) || (type == BP_VD_ZARLINK_9530 && flexiCfg->slic_type != 0x9530) )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "FlexiCalc: Invalid slic type (0x%x), expects either 0x3239 or 0x9530 !!!", flexiCfg->slic_type));
      XCHG_ASSERT(0);      
   }

   return ( flexiCfg );
}
