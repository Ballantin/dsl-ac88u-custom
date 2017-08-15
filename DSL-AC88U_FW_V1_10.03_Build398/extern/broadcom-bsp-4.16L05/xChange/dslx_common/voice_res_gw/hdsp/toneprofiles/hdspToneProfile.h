/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: hdspToneProfile.h
*
****************************************************************************
*    Description:
*
*     This file contains the public declarations for tone generation profiles
*
****************************************************************************/

#ifndef HDSP_TONE_PROFILE_H
#define HDSP_TONE_PROFILE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgCountry.h>
#include <hdspCfg.h>
#include <hapi_pte.h>
#include <hdspTone.h>


/* ---- Constants and Types ---------------------------------------------- */

#define HDSP_TONE_PROFILE_PLACE_HOLDER             \
{ "PLACE-HOLDER", 0,                               \
   {                                               \
      /* Frequency control */                      \
      HAPI_PTE_MKFREQCTRL(0, HAPI_PTE_SINEWAVE),   \
      /* 4 FREQUENCIES */                          \
      {0, 0, 0, 0},/* nr. of AMPLITUDES */         \
      {0, 0, 0, 0},                                \
      /* No Loop Counter used */                   \
      0,                                           \
      /* timeout - not used */                     \
      0,                                           \
      /* number of states used */                  \
      0,                                           \
                                                   \
      /* 16 STATES */                              \
      {                                            \
         /* State 0 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 1 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 2 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 3 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 4 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 5 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 6 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 7 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 8 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 9 - Not Used */                  \
         {0,0,0,0},                                \
         /* State 10 - Not Used */                 \
         {0,0,0,0},                                \
         /* State 11 - Not Used */                 \
         {0,0,0,0},                                \
         /* State 12 - Not Used */                 \
         {0,0,0,0},                                \
         /* State 13 - Not Used */                 \
         {0,0,0,0},                                \
         /* State 14 - Not Used */                 \
         {0,0,0,0},                                \
         /* State 15 - Not Used */                 \
         {0,0,0,0}                                 \
      }                                            \
   }                                               \
}

#define HDSP_FREQGROUP_UNUSED    \
{                                \
   0,                            \
   {0, 0, 0, 0},                 \
   {0, 0, 0, 0},                 \
   0,                            \
   0,                            \
   {                             \
      {0,0,0,0},                 \
      {0,0,0,0}                  \
   }                             \
}

/* Used as a placeholder pulse time in the tone profile to be
** replaced by a provisioned call waiting delay value */
#define HDSP_DELAY_PLACEHOLDER (0xFFFE)

/* Default time to wait in milliseconds for DTMFD ack signal
** from the customers telephone/handset */
#define HDSP_DTMF_ACK_DELAY 200


/* Used for SAS tone loops that are required to continuously repeat. */
#define HDSP_LARGE_LOOP       65534

/* Custom tone structure */
typedef struct
{
   const char    *name;
   HAPI_PTE_CP_PROFILE  profile;
} HDSP_CUSTOM_TONE;


/* ---- Variable Externs ------------------------------------------------- */

extern const HDSP_CUSTOM_TONE gCASTones[];




#if VRG_COUNTRY_CFG_CYPRUS
   extern const HDSP_CUSTOM_TONE gHdspToneProfile_CYPRUS[];
#else
   #define gHdspToneProfile_CYPRUS ((const HDSP_CUSTOM_TONE*) NULL)
#endif


/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_TONE_PROFILE_H  */
