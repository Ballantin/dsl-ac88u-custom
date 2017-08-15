/*****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
******************************************************************************
*
*  apmrsmp.h - APM Decimator and interpolator definitions
*
*  PURPOSE:
*
*     This file contains the definitions for the APM decimator and
*     interpolator.
*
*  NOTES:
*
*     The symbol LDX_BUILDING_ZSP_ASSEMBLY is used in this file to
*     preclude all C syntax that cannot be handled by the assembly
*     C preprocessor. This a kluge to allow assembly files to
*     include this file for constant definitions.
*
*****************************************************************************/

#ifndef  RESAMPLE_H
#define  RESAMPLE_H

/* ---- Include Files ----------------------------------------------------- */

#ifndef LDX_BUILDING_ZSP_ASSEMBLY
#  include <hapi_types.h>
#endif

#include <resample_cfg.h>


/* ---- Constants and Types ----------------------------------------------- */

/**
* Resample framesize constants
*/

#define RESAMPLE_FRAMESIZE_8KHZ     RESAMPLE_CFG_BASE_FRAMESIZE
#define RESAMPLE_FRAMESIZE_16KHZ    (RESAMPLE_FRAMESIZE_8KHZ * 2)
#define RESAMPLE_FRAMESIZE_32KHZ    (RESAMPLE_FRAMESIZE_8KHZ * 4)
#define RESAMPLE_FRAMESIZE_96KHZ    (RESAMPLE_FRAMESIZE_8KHZ * 12)

/**
* Decimation and interpolation constants.
*
* Note that calling functions should never reference these
* constants directly.
*/

#define DECIM96TO32TAPLEN           18
#define DECIM96TO32HISTLEN          (DECIM96TO32TAPLEN - 1)
#define DECIM96TO32FACTOR           (96/32)

#if RESAMPLE_CFG_INTERNATIONAL_SUPPORT
   #define DECIM32TO16TAPLEN        88
#else
   #define DECIM32TO16TAPLEN        60
#endif
#define DECIM32TO16HISTLEN          (DECIM32TO16TAPLEN - 1)
#define DECIM32TO16FACTOR           (32/16)

#if RESAMPLE_CFG_INTERNATIONAL_SUPPORT
   #define DECIM32TO8TAPLEN         88
#else
   #define DECIM32TO8TAPLEN         44
#endif
#define DECIM32TO8HISTLEN           (DECIM32TO8TAPLEN - 1)
#define DECIM32TO8FACTOR            (32/8)

#define INTER16TO32TAPLEN           30
#define INTER16TO32HISTLEN          (INTER16TO32TAPLEN - 1)
#define INTER16TO32FACTOR           (32/16)

#if RESAMPLE_CFG_INTERNATIONAL_SUPPORT
   #define INTER8TO32TAPLEN         22
#else
   #define INTER8TO32TAPLEN         16
#endif
#define INTER8TO32HISTLEN           (INTER8TO32TAPLEN - 1)
#define INTER8TO32FACTOR            (32/8)

/**
* Decimation and interpolation functions require the calling module
* to allocate memory padding. The memory pad sizes are defined
* as follows.
*/

#if RESAMPLE_CFG_WIDEBAND_SUPPORT
#  define DECIM_PAD_SIZE            DECIM96TO32TAPLEN
#  define INTER_PAD_SIZE            INTER16TO32TAPLEN
#else
#  define DECIM_PAD_SIZE            DECIM96TO32TAPLEN
#  define INTER_PAD_SIZE            INTER8TO32TAPLEN
#endif

#ifndef LDX_BUILDING_ZSP_ASSEMBLY

/* ---- Constants and Types ----------------------------------------------- */

#ifndef RESAMPLE_CFG_WIDEBAND_SUPPORT
#  error RESAMPLE_CFG_WIDEBAND_SUPPORT undefined.
#endif

/**
* Defines the decimator history structure. This structure consists
* of histories for the 2 stages.
*
* Normally the history requirements are taplen - 1 per filter but taplen
* words are allocated to ensure even size arrays. Assume that taplen
* is always even. The even size requirement is an optimization for
* the ZSP assembly code, where double stores starting at even address
* boundaries only take 1 cycle. Misaligned stores take 2 cycles.
*/

typedef struct
{
   HAPI_SINT16 hist96to32[DECIM96TO32TAPLEN];
#if RESAMPLE_CFG_WIDEBAND_SUPPORT
   HAPI_SINT16 hist32to16[DECIM32TO16TAPLEN];
#else
   HAPI_SINT16 hist32to8[DECIM32TO8TAPLEN];
#endif

} DECIMATEHIST;

/**
* Defines the interpolator history structure. This structure consists
* of history for the linear interpolation stage. The FIR stage history
* is assumed to be stored with the input data.
*/
typedef struct
{
  HAPI_SINT16 hist32to96;

} INTERPOLATEHIST;


/* ---- Variable Externs -------------------------------------------------- */

extern HAPI_SINT16 decim96to32fir[DECIM96TO32TAPLEN];

extern HAPI_SINT16 decim32to16fir[DECIM32TO16TAPLEN];
extern HAPI_SINT16 inter16to32fir[INTER16TO32FACTOR][INTER16TO32TAPLEN];

extern HAPI_SINT16 decim32to8fir[DECIM32TO8TAPLEN];
extern HAPI_SINT16 inter8to32fir[INTER8TO32FACTOR][INTER8TO32TAPLEN];

/* ---- Function Prototypes ----------------------------------------------- */

void apmDecimate96
(
   HAPI_SINT16 *insamp,
   HAPI_SINT16 *outsamp,
   DECIMATEHIST *hist
);

void apmInterpolate96
(
   HAPI_SINT16 *insamp,
   HAPI_SINT16 *outsamp,
   INTERPOLATEHIST *hist
);

#endif /* LDX_BUILDING_ZSP_ASSEMBLY */

#endif /* RESAMPLE_H */

