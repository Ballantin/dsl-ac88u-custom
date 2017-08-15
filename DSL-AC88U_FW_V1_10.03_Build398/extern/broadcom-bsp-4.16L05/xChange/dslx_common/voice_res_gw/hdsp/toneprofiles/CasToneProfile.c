/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: hdspToneProfileNorthAmerica.c
*
****************************************************************************
*    Description:
*
*     This file provide country specific tone profiles
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <hapi_pte.h>
#include "hdspToneProfile.h"


/* ---- Public Variables ------------------------------------------------- */


/* CAS Tone names, power level, profile */

const HDSP_CUSTOM_TONE gCASTones[] =
{

/*************************************************
** DTAS onhook local - Profile 0              **
*************************************************/
   { "DTAS ONHOOK",
      {
         /* Whole Repeat Count */
         0,
         /* number of frequency groups */
         1,
         /* Freq Groups */
         {
            /* Group 1 */
            {
               /* Power level (in -dB) */
               16,
               /* 4 FREQUENCIES */
               {2130, 2750, 0, 0},              /* 2130, 2750, 0, 0 Hz */
               /* 4 AMPLITUDES */
               {22656, 22656, 0, 0},            /* Amplitude = 0 dBm */
               /* frequency group repeat */
               0,
               /* number of frequencies and states */
               HAPI_PTE_MKFREQGRP_INFO( 2, 1 ),
               {
                  /* State 0 */
                  {
                     ( HAPI_PTE_CADTYPE_PULSEON | HAPI_PTE_CADBIT_EVENT | HAPI_PTE_CADBIT_COMPLETE ),      /* Pulse Type */
                     0,   /* Pulse Next State */
                     ( HAPI_PTE_CADBIT_FREQ1 | HAPI_PTE_CADBIT_FREQ2 ),         /* Pulse Frequency Mask */
                     100,                     /* Pulse Time */
                  },
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            },
            /* Group 2 - not used */
            {
               /* Power level (in -dBM) */
               0,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               0,                    /* frequency group repeat */
               0,                    /* number of frequencies and states */
               {
                  /* State 0 - Not Used */
                  {0,0,0,0},
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            },
            /* Group 3 - not used */
            {
               /* Power level (in -dBM) */
               0,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               0,                    /* frequency group repeat */
               0,                    /* number of frequencies and states */
               {
                  /* State 0 - Not Used */
                  {0,0,0,0},
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            },
            /* Group 4 - not used */
            {
               /* Power level (in -dBM) */
               0,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               0,                    /* frequency group repeat */
               0,                    /* number of frequencies and states */
               {
                  /* State 0 - Not Used */
                  {0,0,0,0},
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            },
            /* Group 5 - not used */
            {
               /* Power level (in -dBM) */
               0,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               0,                    /* frequency group repeat */
               0,                    /* number of frequencies and states */
               {
                  /* State 0 - Not Used */
                  {0,0,0,0},
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            }
         }
      }
   },


/*************************************************
** DTAS offhook local - Profile 41              **
*************************************************/
   { "DTAS OFFHOOK",
      {
         /* Whole Repeat Count */
         0,
         /* number of frequency groups */
         2,
         /* Freq Groups */
         {
            /* Group 1 */
            {
               /* Power level (in -dB) */
               16,
               /* 4 FREQUENCIES */
               {2130, 2750, 0, 0},              /* 2130, 2750, 0, 0 Hz */
               /* 4 AMPLITUDES */
               {22656, 22656, 0, 0},            /* Amplitude = 0 dBm */
               /* frequency group repeat */
               0,
               /* number of frequencies and states */
               HAPI_PTE_MKFREQGRP_INFO( 2, 2 ),
               {
                  /* State 0 */
                  {
                     HAPI_PTE_CADTYPE_PULSEON,      /* Pulse Type */
                     HAPI_PTE_CADBITS_NEXTSTATE1,   /* Pulse Next State */
                     ( HAPI_PTE_CADBIT_FREQ1 | HAPI_PTE_CADBIT_FREQ2 ),         /* Pulse Frequency Mask */
                     80,                     /* Pulse Time */
                  },
                  /* State 1 */
                  {
                     ( HAPI_PTE_CADTYPE_PULSEOFF | HAPI_PTE_CADBIT_EVENT ),      /* Pulse Type */
                     HAPI_PTE_CADBITS_NEXTSTATE2,   /* Pulse Next State */
                     0,         /* Pulse Frequency Mask */
                     HDSP_DTMF_ACK_DELAY,                     /* Pulse Time */
                  },
               }
            },
            /* Group 2 */
            {
               /* Power level (in -dBM) */
               16,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               /* frequency group repeat */
               0,
               /* number of frequencies and states */
               HAPI_PTE_MKFREQGRP_INFO( 0, 1 ),
               {
                  /* State 0 */
                  {
                     ( HAPI_PTE_CADTYPE_PULSEOFF | HAPI_PTE_CADBIT_EVENT ),      /* Pulse Type */
                     0,   /* Pulse Next State */
                     0,         /* Pulse Frequency Mask */
                     HDSP_DELAY_PLACEHOLDER,                     /* Pulse Time */
                  },
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            },
            /* Group 3 - not used */
            {
               /* Power level (in -dBM) */
               0,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               0,                    /* frequency group repeat */
               0,                    /* number of frequencies and states */
               {
                  /* State 0 - Not Used */
                  {0,0,0,0},
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            },
            /* Group 4 - not used */
            {
               /* Power level (in -dBM) */
               0,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               0,                    /* frequency group repeat */
               0,                    /* number of frequencies and states */
               {
                  /* State 0 - Not Used */
                  {0,0,0,0},
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            },
            /* Group 5 - not used */
            {
               /* Power level (in -dBM) */
               0,
               {0, 0, 0, 0},              /* 4 FREQUENCIES */
               {0, 0, 0, 0},              /* 4 AMPLITUDES */
               0,                    /* frequency group repeat */
               0,                    /* number of frequencies and states */
               {
                  /* State 0 - Not Used */
                  {0,0,0,0},
                  /* State 1 - Not Used */
                  {0,0,0,0}
               }
            }
         }
      }
   }
};


/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */
