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
****************************************************************************
*    Filename: cmtdCtlCountryProfile.c
*
****************************************************************************
*    Description:
*
*     Contains country-specific calling tone profiles.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <vrgLog.h>
#include <vrgCountry.h>
#include "cmtdCtlFilterCoefs.h"
#include "cmtdCtlCountryProfile.h"

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/***************************************************************************
*  NORTH AMERICA
****************************************************************************/
#define CMTD_NORTH_AMERICA_DIAL_TONE_FILTER     1
const CMTD_STMPARAMS northAmericaDialToneParam =
{
    CMDT350_440_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt350_440coeffs,
    1,
    {   0, 0,
        {
            {
                {150, 65535, CMTD_NORTH_AMERICA_DIAL_TONE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_NORTH_AMERICA_RINGBACK_FILTER      1
const CMTD_STMPARAMS northAmericaRingbackParam =
{
    CMDT440_480_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt440_480coeffs,
    2,
    {   0, 0,
        {
            {
                {1900, 2100, CMTD_NORTH_AMERICA_RINGBACK_FILTER, 0}, 
                {3800, 4200, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_NORTH_AMERICA_BUSY_TONE_FILTER     1
const CMTD_STMPARAMS northAmericaBusyToneParam =
{
    CMDT480_620_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt480_620coeffs,
    2,
    {   0, 0,
        {
            {
                {475, 525, CMTD_NORTH_AMERICA_BUSY_TONE_FILTER, 0}, 
                {475, 525, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_NORTH_AMERICA_CONG_TONE_FILTER     1
const CMTD_STMPARAMS northAmericaCongToneParam =
{
    CMDT400_NOTCH_FILTER_SIZE,	                  
    ( VRG_SINT16 * )cmdt480_620coeffs,   			  
    1,
    {   0, 0,
        {   
            {
                {225, 275, CMTD_NORTH_AMERICA_CONG_TONE_FILTER, 0}, 
                {225, 275, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_NORTH_AMERICA[] = {
   &northAmericaDialToneParam,      /* Dial Tone */
   &northAmericaRingbackParam,      /* Ringback Tone */
   &northAmericaBusyToneParam,      /* Busy Tone */
   &northAmericaCongToneParam,      /* Congestion Tone */
   &northAmericaBusyToneParam       /* "Release Tone" */
};

/***************************************************************************
*  FRANCE
****************************************************************************/
#if VRG_COUNTRY_CFG_FRANCE

#define CMTD_FRANCE_DIAL_TONE_FILTER            1
const CMTD_STMPARAMS franceDialToneParam =
{
    CMDT440_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt440coeffs,
    1,
    {   0, 0,
        {   
            {
                {250, 65535, CMTD_FRANCE_DIAL_TONE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_FRANCE_RINGBACK_FILTER            1
const CMTD_STMPARAMS franceRingbackParam =
{
    CMDT440_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt440coeffs,
    2,
    {   0, 0,
        {   
            {
                {1350, 1650, CMTD_FRANCE_RINGBACK_FILTER, 0}, 
                {3150, 3850, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_FRANCE_BUSY_TONE_FILTER            1
const CMTD_STMPARAMS franceBusyToneParam =
{
    CMDT440_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt440coeffs,
    2,
    {   0, 0,
        {   
            {
                {450, 550, CMTD_FRANCE_BUSY_TONE_FILTER, 0}, 
                {450, 550, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_FRANCE[] = {
   &franceDialToneParam,      /* Dial Tone */
   &franceRingbackParam,      /* Ringback Tone */
   &franceBusyToneParam,      /* Busy Tone */
   NULL,                      /* Congestion Tone */
   &franceBusyToneParam       /* "Release Tone" */
};
#endif

/***************************************************************************
*  GERMANY
****************************************************************************/
#if VRG_COUNTRY_CFG_GERMANY

#define CMTD_GERMANY_BUSY_TONE_FILTER           1
const CMTD_STMPARAMS germanyBusyToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,	              /* Germany's filter coeff array size */
    ( VRG_SINT16 * )cmdt425coeffs,   			  /* Germany's filter coefficients */
    2,
    {   0, 0,
        {   
            {
                {475, 525, CMTD_GERMANY_BUSY_TONE_FILTER, 0}, 
                {475, 525, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_GERMANY[] = {
   NULL,                      /* Dial Tone */
   NULL,                      /* Ringback Tone */
   &germanyBusyToneParam,     /* Busy Tone */
   NULL,                      /* Congestion Tone */
   &germanyBusyToneParam      /* "Release Tone" */
};
#endif

/***************************************************************************
*  ITALY
****************************************************************************/
#if VRG_COUNTRY_CFG_ITALY

#define CMTD_ITALY_DIAL_TONE_FILTER             1
const CMTD_STMPARAMS italyDialToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt425coeffs,
    1,
    {   0, 0,
        {   
            {
                {180, 220, CMTD_ITALY_DIAL_TONE_FILTER, 0}, 
                {180, 220, CMTD_SILENCE_FILTER, 0}, 
                {540, 65535, CMTD_ITALY_DIAL_TONE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_ITALY_BUSY_TONE_FILTER             1
const CMTD_STMPARAMS italyBusyToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt425coeffs,
    2,
    {   0, 0,
        {   
            {
                {450, 550, CMTD_ITALY_BUSY_TONE_FILTER, 0}, 
                {450, 550, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_ITALY[] = {
   &italyDialToneParam,    /* Dial Tone */
   NULL,                   /* Ringback Tone */
   &italyBusyToneParam,    /* Busy Tone */
   NULL,                   /* Congestion Tone */
   &italyBusyToneParam     /* "Release Tone" */
};
#endif

/***************************************************************************
*  JAPAN
****************************************************************************/
#if VRG_COUNTRY_CFG_JAPAN

#define CMTD_JAPAN_DIAL_TONE_FILTER             1
const CMTD_STMPARAMS japanDialToneParam =
{
    CMDT400MOD20_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt400mod20coeffs,
    1,
    {   0, 0,
        {   
            {
                {250, 65535, CMTD_JAPAN_DIAL_TONE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_JAPAN_RINGBACK_FILTER              1
const CMTD_STMPARAMS japanRingbackParam =
{
    CMDT400MOD20_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt400mod20coeffs,
    2,
    {   0, 0,
        {
            {
                {950, 1050, CMTD_JAPAN_RINGBACK_FILTER, 0}, 
                {1900, 2100, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_JAPAN_BUSY_TONE_FILTER             1
const CMTD_STMPARAMS japanBusyToneParam =
{
    CMDT400MOD20_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt400mod20coeffs,
    2,
    {   0, 0,
        {   
            {
                {475, 525, CMTD_JAPAN_BUSY_TONE_FILTER, 0}, 
                {475, 525, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_JAPAN[] = {
   &japanDialToneParam,    /* Dial Tone */
   &japanRingbackParam,    /* Ringback Tone */
   &japanBusyToneParam,    /* Busy Tone */
   NULL,                   /* Congestion Tone */
   &japanBusyToneParam     /* "Release Tone" */
};
#endif

/***************************************************************************
*  NETHERLANDS
****************************************************************************/
#if VRG_COUNTRY_CFG_NETHERLANDS

#define CMTD_NETHERLANDS_DIAL_TONE_FILTER             1
const CMTD_STMPARAMS netherlandsDialToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt425coeffs,
    1,
    {   0, 0,
        {   
            {
                {450, 65535, CMTD_NETHERLANDS_DIAL_TONE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_NETHERLANDS_BUSY_TONE_FILTER             1
const CMTD_STMPARAMS netherlandsBusyToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt425coeffs,
    2,
    {   0, 0,
        {   
            {
                {400, 600, CMTD_NETHERLANDS_BUSY_TONE_FILTER, 0}, 
                {400, 600, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_NETHERLANDS_CONG_TONE_FILTER             1
const CMTD_STMPARAMS netherlandsCongToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt425coeffs,
    2,
    {   0, 0,
        {   
            {
                {200, 300, CMTD_NETHERLANDS_CONG_TONE_FILTER, 0}, 
                {200, 300, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_NETHERLANDS[] = {
   &netherlandsDialToneParam,    /* Dial Tone */
   NULL,                         /* Ringback Tone */
   &netherlandsBusyToneParam,    /* Busy Tone */
   &netherlandsCongToneParam,    /* Congestion Tone */
   &netherlandsBusyToneParam     /* "Release Tone" */
};
#endif

/***************************************************************************
*  UK
****************************************************************************/
#if VRG_COUNTRY_CFG_UK

#define CMTD_UK_DIAL_TONE_FILTER     1
const CMTD_STMPARAMS ukDialToneParam =
{
    CMDT400_NOTCH_FILTER_SIZE,	                  
    ( VRG_SINT16 * )cmdt350_440coeffs,   			  
    1,
    {   0, 0,
        {   
            {
                {675, 65535, CMTD_UK_DIAL_TONE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_UK_RINGBACK_TONE_FILTER     1
const CMTD_STMPARAMS ukRingbackToneParam =
{
    CMDT400_450_NOTCH_FILTER_SIZE,	                  
    ( VRG_SINT16 * )cmdt400_450coeffs,   			  
    2,
    {   0, 0,
        {   
            {
                {380, 420, CMTD_UK_RINGBACK_TONE_FILTER, 0}, 
                {180, 220, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};


#define CMTD_UK_BUSY_TONE_FILTER     1
const CMTD_STMPARAMS ukBusyToneParam =
{
    CMDT400_NOTCH_FILTER_SIZE,	                  
    ( VRG_SINT16 * )cmdt400coeffs,   			  
    2,
    {   0, 0,
        {   
            {
                {335, 415, CMTD_UK_BUSY_TONE_FILTER, 0}, 
                {335, 415, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_UK_CONG_TONE_FILTER     1
const CMTD_STMPARAMS ukCongToneParam =
{
    CMDT400_NOTCH_FILTER_SIZE,	                  
    ( VRG_SINT16 * )cmdt400coeffs,   			  
    2,
    {   0, 0,
        {   
            {
                {360, 440, CMTD_UK_CONG_TONE_FILTER, 0}, 
                {320, 380, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_UK[] = {
   &ukDialToneParam,     /* Dial Tone */
   &ukRingbackToneParam, /* Ringback Tone */
   &ukBusyToneParam,     /* Busy Tone */
   &ukCongToneParam,     /* Congestion Tone */
   &ukCongToneParam      /* "Release Tone" */
};
#endif

/***************************************************************************
*  AUSTRALIA
****************************************************************************/
#if VRG_COUNTRY_CFG_AUSTRALIA

#define CMTD_AUSTRALIA_DIAL_TONE_FILTER             1
const CMTD_STMPARAMS australiaDialToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt425coeffs,
    1,
    {   0, 0,
        {   
            {
                {250, 65535, CMTD_AUSTRALIA_DIAL_TONE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

#define CMTD_AUSTRALIA_BUSY_TONE_FILTER             1
const CMTD_STMPARAMS australiaBusyToneParam =
{
    CMDT425_NOTCH_FILTER_SIZE,
    ( VRG_SINT16 * )cmdt425coeffs,
    2,
    {   0, 0,
        {   
            {
                {335, 415, CMTD_AUSTRALIA_BUSY_TONE_FILTER, 0}, 
                {335, 415, CMTD_SILENCE_FILTER, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0}, 
                {0, 0, 0, 0} 
            }
        }
    }
};

const CMTD_STMPARAMS* stmParams_AUSTRALIA[] = {
   &australiaDialToneParam,    /* Dial Tone */
   NULL,                       /* Ringback Tone */
   &australiaBusyToneParam,    /* Busy Tone */
   NULL,                       /* Congestion Tone */
   &australiaBusyToneParam     /* "Release Tone" */
};
#endif

/***************************************************************************
*  OTHER COUNTRIES
****************************************************************************/

/* 
** We must add an entry here for every new country we add to countryArchive.h
** if it is not already defined in the above CMTD_STMPARAMS.
*/
#if VRG_COUNTRY_CFG_BELGIUM
const CMTD_STMPARAMS* stmParams_BELGIUM = NULL;
#endif
#if VRG_COUNTRY_CFG_BRAZIL
const CMTD_STMPARAMS* stmParams_BRAZIL = NULL;
#endif
#if VRG_COUNTRY_CFG_CHILE
const CMTD_STMPARAMS* stmParams_CHILE = NULL;
#endif
#if VRG_COUNTRY_CFG_CHINA
const CMTD_STMPARAMS* stmParams_CHINA = NULL;
#endif
#if VRG_COUNTRY_CFG_CZECH
const CMTD_STMPARAMS* stmParams_CZECH = NULL;
#endif
#if VRG_COUNTRY_CFG_DENMARK
const CMTD_STMPARAMS* stmParams_DENMARK = NULL;
#endif
#if VRG_COUNTRY_CFG_ETSI
const CMTD_STMPARAMS* stmParams_ETSI = NULL;
#endif
#if VRG_COUNTRY_CFG_FINLAND
const CMTD_STMPARAMS* stmParams_FINLAND = NULL;
#endif
#if VRG_COUNTRY_CFG_HUNGARY
const CMTD_STMPARAMS* stmParams_HUNGARY = NULL;
#endif
#if VRG_COUNTRY_CFG_INDIA
const CMTD_STMPARAMS* stmParams_INDIA = NULL;
#endif
#if VRG_COUNTRY_CFG_NEW_ZEALAND
const CMTD_STMPARAMS* stmParams_NEW_ZEALAND = NULL;
#endif
#if VRG_COUNTRY_CFG_MEXICO
const CMTD_STMPARAMS* stmParams_MEXICO = NULL;
#endif
#if VRG_COUNTRY_CFG_SPAIN
const CMTD_STMPARAMS* stmParams_SPAIN = NULL;
#endif
#if VRG_COUNTRY_CFG_SWEDEN
const CMTD_STMPARAMS* stmParams_SWEDEN = NULL;
#endif
#if VRG_COUNTRY_CFG_NORWAY
const CMTD_STMPARAMS* stmParams_NORWAY = NULL;
#endif
#if VRG_COUNTRY_CFG_SWITZERLAND
const CMTD_STMPARAMS* stmParams_SWITZERLAND = NULL;
#endif
#if VRG_COUNTRY_CFG_TR57
const CMTD_STMPARAMS* stmParams_TR57 = NULL;
#endif
#if VRG_COUNTRY_CFG_TAIWAN
const CMTD_STMPARAMS* stmParams_TAIWAN = NULL;
#endif
#if VRG_COUNTRY_CFG_POLAND
const CMTD_STMPARAMS* stmParams_POLAND = NULL;
#endif
#if VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES
const CMTD_STMPARAMS* stmParams_UNITED_ARAB_EMIRATES = NULL;
#endif
#if VRG_COUNTRY_CFG_CYPRUS
const CMTD_STMPARAMS* stmParams_CYPRUS = NULL;
#endif



/* Begin Country-related archives */
#undef COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) (CMTD_STMPARAMS**)&stmParams_##country,

CMTD_STMPARAMS* *cmtdCallingToneProfiles[] = {
   /* 
   ** This will include all the supported countries' CMTD_STMPARAMS
   ** into one single array.
   */
   #include <countryArchive.h>
};

/* ---- Private Variables ------------------------------------------------ */
