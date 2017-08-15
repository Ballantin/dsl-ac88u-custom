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
*    Filename: flexiCalc6838.c
*
****************************************************************************
*    Description:
*
*     Implements abstraction interface for FlexiCalc generated header files
*     which contain APM specific configuration values for the 6838.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

/* General header files */
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <vrgCountry.h>
#include <string.h>
#include <flexiCalc6838.h>
#include <boardparms_voice.h>
#include <boardHalProv.h>

/* Flexicalc header files for Si32392 */
#include <FlexiCalc_BCM6838_Si32392_ARGENTINA.h>
#include <FlexiCalc_BCM6838_Si32392_AUSTRALIA.h>
#include <FlexiCalc_BCM6838_Si32392_AUSTRIA.h>
#include <FlexiCalc_BCM6838_Si32392_BELGIUM.h>
#include <FlexiCalc_BCM6838_Si32392_BRAZIL.h>
#include <FlexiCalc_BCM6838_Si32392_CHILE.h>
#include <FlexiCalc_BCM6838_Si32392_CHINA.h>
#include <FlexiCalc_BCM6838_Si32392_CZECH.h>
#include <FlexiCalc_BCM6838_Si32392_DENMARK.h>
#include <FlexiCalc_BCM6838_Si32392_ETSI.h>
#include <FlexiCalc_BCM6838_Si32392_FINLAND.h>
#include <FlexiCalc_BCM6838_Si32392_FRANCE.h>
#include <FlexiCalc_BCM6838_Si32392_GERMANY.h>
#include <FlexiCalc_BCM6838_Si32392_HUNGARY.h>
#include <FlexiCalc_BCM6838_Si32392_INDIA.h>
#include <FlexiCalc_BCM6838_Si32392_IRELAND.h>
#include <FlexiCalc_BCM6838_Si32392_ISRAEL.h>
#include <FlexiCalc_BCM6838_Si32392_ITALY.h>
#include <FlexiCalc_BCM6838_Si32392_JAPAN.h>
#include <FlexiCalc_BCM6838_Si32392_MEXICO.h>
#include <FlexiCalc_BCM6838_Si32392_NETHERLANDS.h>
#include <FlexiCalc_BCM6838_Si32392_NEW_ZEALAND.h>
#include <FlexiCalc_BCM6838_Si32392_NORTH_AMERICA.h>
#include <FlexiCalc_BCM6838_Si32392_NORWAY.h>
#include <FlexiCalc_BCM6838_Si32392_POLAND.h>
#include <FlexiCalc_BCM6838_Si32392_ROMANIA.h>
#include <FlexiCalc_BCM6838_Si32392_SINGAPORE.h>
#include <FlexiCalc_BCM6838_Si32392_SLOVAKIA.h>
#include <FlexiCalc_BCM6838_Si32392_SLOVENIA.h>
#include <FlexiCalc_BCM6838_Si32392_SPAIN.h>
#include <FlexiCalc_BCM6838_Si32392_SWEDEN.h>
#include <FlexiCalc_BCM6838_Si32392_SWITZERLAND.h>
#include <FlexiCalc_BCM6838_Si32392_TAIWAN.h>
#include <FlexiCalc_BCM6838_Si32392_TR57.h>
#include <FlexiCalc_BCM6838_Si32392_TURKEY.h>
#include <FlexiCalc_BCM6838_Si32392_UK.h>
#include <FlexiCalc_BCM6838_Si32392_UNITED_ARAB_EMIRATES.h>
#include <FlexiCalc_BCM6838_Si32392_CYPRUS.h>
#include <FlexiCalc_BCM6838_Si32392_WB_ARGENTINA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_AUSTRALIA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_AUSTRIA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_BELGIUM.h>
#include <FlexiCalc_BCM6838_Si32392_WB_BRAZIL.h>
#include <FlexiCalc_BCM6838_Si32392_WB_CHILE.h>
#include <FlexiCalc_BCM6838_Si32392_WB_CHINA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_CZECH.h>
#include <FlexiCalc_BCM6838_Si32392_WB_DENMARK.h>
#include <FlexiCalc_BCM6838_Si32392_WB_ETSI.h>
#include <FlexiCalc_BCM6838_Si32392_WB_FINLAND.h>
#include <FlexiCalc_BCM6838_Si32392_WB_FRANCE.h>
#include <FlexiCalc_BCM6838_Si32392_WB_GERMANY.h>
#include <FlexiCalc_BCM6838_Si32392_WB_HUNGARY.h>
#include <FlexiCalc_BCM6838_Si32392_WB_INDIA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_IRELAND.h>
#include <FlexiCalc_BCM6838_Si32392_WB_ISRAEL.h>
#include <FlexiCalc_BCM6838_Si32392_WB_ITALY.h>
#include <FlexiCalc_BCM6838_Si32392_WB_JAPAN.h>
#include <FlexiCalc_BCM6838_Si32392_WB_MEXICO.h>
#include <FlexiCalc_BCM6838_Si32392_WB_NETHERLANDS.h>
#include <FlexiCalc_BCM6838_Si32392_WB_NEW_ZEALAND.h>
#include <FlexiCalc_BCM6838_Si32392_WB_NORTH_AMERICA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_NORWAY.h>
#include <FlexiCalc_BCM6838_Si32392_WB_POLAND.h>
#include <FlexiCalc_BCM6838_Si32392_WB_ROMANIA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_SINGAPORE.h>
#include <FlexiCalc_BCM6838_Si32392_WB_SLOVAKIA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_SLOVENIA.h>
#include <FlexiCalc_BCM6838_Si32392_WB_SPAIN.h>
#include <FlexiCalc_BCM6838_Si32392_WB_SWEDEN.h>
#include <FlexiCalc_BCM6838_Si32392_WB_SWITZERLAND.h>
#include <FlexiCalc_BCM6838_Si32392_WB_TAIWAN.h>
#include <FlexiCalc_BCM6838_Si32392_WB_TR57.h>
#include <FlexiCalc_BCM6838_Si32392_WB_TURKEY.h>
#include <FlexiCalc_BCM6838_Si32392_WB_UK.h>
#include <FlexiCalc_BCM6838_Si32392_WB_UNITED_ARAB_EMIRATES.h>
#include <FlexiCalc_BCM6838_Si32392_WB_CYPRUS.h>

/* Flexicalc header files for L9540 */
#include <FlexiCalc_BCM6838_L9540_ARGENTINA.h>
#include <FlexiCalc_BCM6838_L9540_AUSTRALIA.h>
#include <FlexiCalc_BCM6838_L9540_AUSTRIA.h>
#include <FlexiCalc_BCM6838_L9540_BELGIUM.h>
#include <FlexiCalc_BCM6838_L9540_BRAZIL.h>
#include <FlexiCalc_BCM6838_L9540_CHILE.h>
#include <FlexiCalc_BCM6838_L9540_CHINA.h>
#include <FlexiCalc_BCM6838_L9540_CZECH.h>
#include <FlexiCalc_BCM6838_L9540_DENMARK.h>
#include <FlexiCalc_BCM6838_L9540_ETSI.h>
#include <FlexiCalc_BCM6838_L9540_FINLAND.h>
#include <FlexiCalc_BCM6838_L9540_FRANCE.h>
#include <FlexiCalc_BCM6838_L9540_GERMANY.h>
#include <FlexiCalc_BCM6838_L9540_HUNGARY.h>
#include <FlexiCalc_BCM6838_L9540_INDIA.h>
#include <FlexiCalc_BCM6838_L9540_IRELAND.h>
#include <FlexiCalc_BCM6838_L9540_ISRAEL.h>
#include <FlexiCalc_BCM6838_L9540_ITALY.h>
#include <FlexiCalc_BCM6838_L9540_JAPAN.h>
#include <FlexiCalc_BCM6838_L9540_MEXICO.h>
#include <FlexiCalc_BCM6838_L9540_NETHERLANDS.h>
#include <FlexiCalc_BCM6838_L9540_NEW_ZEALAND.h>
#include <FlexiCalc_BCM6838_L9540_NORTH_AMERICA.h>
#include <FlexiCalc_BCM6838_L9540_NORWAY.h>
#include <FlexiCalc_BCM6838_L9540_POLAND.h>
#include <FlexiCalc_BCM6838_L9540_ROMANIA.h>
#include <FlexiCalc_BCM6838_L9540_SINGAPORE.h>
#include <FlexiCalc_BCM6838_L9540_SLOVAKIA.h>
#include <FlexiCalc_BCM6838_L9540_SLOVENIA.h>
#include <FlexiCalc_BCM6838_L9540_SPAIN.h>
#include <FlexiCalc_BCM6838_L9540_SWEDEN.h>
#include <FlexiCalc_BCM6838_L9540_SWITZERLAND.h>
#include <FlexiCalc_BCM6838_L9540_TAIWAN.h>
#include <FlexiCalc_BCM6838_L9540_TR57.h>
#include <FlexiCalc_BCM6838_L9540_TURKEY.h>
#include <FlexiCalc_BCM6838_L9540_UK.h>
#include <FlexiCalc_BCM6838_L9540_UNITED_ARAB_EMIRATES.h>
#include <FlexiCalc_BCM6838_L9540_CYPRUS.h>
#include <FlexiCalc_BCM6838_L9540_WB_ARGENTINA.h>
#include <FlexiCalc_BCM6838_L9540_WB_AUSTRALIA.h>
#include <FlexiCalc_BCM6838_L9540_WB_AUSTRIA.h>
#include <FlexiCalc_BCM6838_L9540_WB_BELGIUM.h>
#include <FlexiCalc_BCM6838_L9540_WB_BRAZIL.h>
#include <FlexiCalc_BCM6838_L9540_WB_CHILE.h>
#include <FlexiCalc_BCM6838_L9540_WB_CHINA.h>
#include <FlexiCalc_BCM6838_L9540_WB_CZECH.h>
#include <FlexiCalc_BCM6838_L9540_WB_DENMARK.h>
#include <FlexiCalc_BCM6838_L9540_WB_ETSI.h>
#include <FlexiCalc_BCM6838_L9540_WB_FINLAND.h>
#include <FlexiCalc_BCM6838_L9540_WB_FRANCE.h>
#include <FlexiCalc_BCM6838_L9540_WB_GERMANY.h>
#include <FlexiCalc_BCM6838_L9540_WB_HUNGARY.h>
#include <FlexiCalc_BCM6838_L9540_WB_INDIA.h>
#include <FlexiCalc_BCM6838_L9540_WB_IRELAND.h>
#include <FlexiCalc_BCM6838_L9540_WB_ISRAEL.h>
#include <FlexiCalc_BCM6838_L9540_WB_ITALY.h>
#include <FlexiCalc_BCM6838_L9540_WB_JAPAN.h>
#include <FlexiCalc_BCM6838_L9540_WB_MEXICO.h>
#include <FlexiCalc_BCM6838_L9540_WB_NETHERLANDS.h>
#include <FlexiCalc_BCM6838_L9540_WB_NEW_ZEALAND.h>
#include <FlexiCalc_BCM6838_L9540_WB_NORTH_AMERICA.h>
#include <FlexiCalc_BCM6838_L9540_WB_NORWAY.h>
#include <FlexiCalc_BCM6838_L9540_WB_POLAND.h>
#include <FlexiCalc_BCM6838_L9540_WB_ROMANIA.h>
#include <FlexiCalc_BCM6838_L9540_WB_SINGAPORE.h>
#include <FlexiCalc_BCM6838_L9540_WB_SLOVAKIA.h>
#include <FlexiCalc_BCM6838_L9540_WB_SLOVENIA.h>
#include <FlexiCalc_BCM6838_L9540_WB_SPAIN.h>
#include <FlexiCalc_BCM6838_L9540_WB_SWEDEN.h>
#include <FlexiCalc_BCM6838_L9540_WB_SWITZERLAND.h>
#include <FlexiCalc_BCM6838_L9540_WB_TAIWAN.h>
#include <FlexiCalc_BCM6838_L9540_WB_TR57.h>
#include <FlexiCalc_BCM6838_L9540_WB_TURKEY.h>
#include <FlexiCalc_BCM6838_L9540_WB_UK.h>
#include <FlexiCalc_BCM6838_L9540_WB_UNITED_ARAB_EMIRATES.h>
#include <FlexiCalc_BCM6838_L9540_WB_CYPRUS.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* Create a static array of each country's FlexiCalc parameters for Si32392 */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) flexicalc##country##Archive32392,
static const APM6838_FLEXICALC_CFG *gFlexiValues32392[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

/* Create a static array of each country's FlexiCalc WB parameters for Si32392 */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) flexicalcWB##country##Archive32392,
static const APM6838_FLEXICALC_CFG *gFlexiValuesWB32392[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

/* Create a static array of each country's FlexiCalc parameters for Le9540 */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) flexicalc##country##Archive9540,
static const APM6838_FLEXICALC_CFG *gFlexiValues9540[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

/* Create a static array of each country's FlexiCalc WB parameters for Le9540 */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) flexicalcWB##country##Archive9540,
static const APM6838_FLEXICALC_CFG *gFlexiValuesWB9540[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   flexiCalcGetCfg6838
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
const APM6838_FLEXICALC_CFG* flexiCalcGetCfg6838
(
   VRG_COUNTRY country,
   VRG_UINT32  sampleRate,
   BP_VOICE_DEVICE_TYPE type
)
{
   const APM6838_FLEXICALC_CFG *flexiCfg=0;

   XCHG_ASSERT( country < VRG_COUNTRY_MAX );

   XCHG_ASSERT( type == BP_VD_SILABS_32392 || type == BP_VD_ZARLINK_9540 || type == BP_VD_ZARLINK_9541);

   if (sampleRate == 8000)
   {
      switch( type )
      {
        case BP_VD_ZARLINK_9540:
        case BP_VD_ZARLINK_9541:
          flexiCfg = gFlexiValues9540[ country ];
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
      switch( type )
      {
         case BP_VD_ZARLINK_9540:
         case BP_VD_ZARLINK_9541:
            flexiCfg = gFlexiValuesWB9540[ country ];
         break;
         case BP_VD_SILABS_32392:
            flexiCfg = gFlexiValuesWB32392[ country ];
         break;
         default:
         {
            switch( country )
            {
               case VRG_COUNTRY_CHINA:
               {
                  switch( type )
                  {
                    case BP_VD_ZARLINK_9540:
                      flexiCfg = flexicalcWBCHINAArchive9540;
                      break;
                    case BP_VD_SILABS_32392:
                      flexiCfg = flexicalcWBCHINAArchive32392;
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
                     case BP_VD_ZARLINK_9540:
                        flexiCfg = flexicalcWBNORTH_AMERICAArchive9540;
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
      }
   }

   XCHG_ASSERT( flexiCfg != NULL );

   /* Make sure this is the correct configuration for the corresponding SLIC */
   if ( (type == BP_VD_SILABS_32392 && flexiCfg->slic_type != 0x32392) || (type == BP_VD_ZARLINK_9540 && flexiCfg->slic_type != 0x9540) || (type == BP_VD_ZARLINK_9541 && flexiCfg->slic_type != 0x9541) )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "FlexiCalc: Invalid slic type (0x%x), expects either 0x32392 or 0x9540 !!!", flexiCfg->slic_type));
      XCHG_ASSERT(0);      
   }

   return ( flexiCfg );
}
