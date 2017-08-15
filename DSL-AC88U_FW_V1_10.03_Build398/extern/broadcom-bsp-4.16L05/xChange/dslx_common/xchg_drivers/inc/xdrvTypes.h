/*****************************************************************************
*  Copyright © 2000-2008 Broadcom.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/


/*
*
****************************************************************************
*
*    Filename: xdrvTypes.h
*
****************************************************************************
*    Description:
*
*     Common types and macros that are shared by all driver modules.
*
*****************************************************************************/


#ifndef XDRV_TYPES_H
#define XDRV_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/* -----------------------------------------------------------------------
** Simple types - use 'long' for 32-bit types because this type is 32-bits
** on both the MIPS and ZSP. 'int' is 32-bits on the MIPS, but only 16-bits
** on the ZSP.
**
** __ASSEMBLER__ is typically defined by the pre-processor when operating
** on an assembly file. This may not be true of all toolchains. It may be
** required to add a -D__ASSEMBLER__ to the makefile rule for assembly
** source files for some toolchains.
*/
#ifndef __ASSEMBLER__
typedef unsigned char       XDRV_UINT8;
typedef signed char         XDRV_SINT8;
typedef unsigned short      XDRV_UINT16;
typedef signed short        XDRV_SINT16;
typedef unsigned long       XDRV_UINT32;
typedef long                XDRV_SINT32;
typedef unsigned long long  XDRV_UINT64;
typedef long long           XDRV_SINT64;
#endif   /* !__ASSEMBLER__ */


/* Boolean */
#define XDRV_TRUE  1
#define XDRV_FALSE 0

#ifndef __ASSEMBLER__
typedef  int   XDRV_BOOL;               /** XDRV_TRUE or XDRV_FALSE */
#endif


#define XDRV_INLINE   static inline


/* xchg_drivers API return codes. */
#define  XDRV_STATUS_OK     0
#define  XDRV_STATUS_ERR   -1

#ifndef __ASSEMBLER__
typedef int    XDRV_STATUS;
#endif



/* Ring Waveform Types */
#ifndef __ASSEMBLER__
typedef enum
{
   XDRV_RINGWAVEFORM_SINE,
   XDRV_RINGWAVEFORM_TRAPEZOID,
} XDRV_RINGWAVEFORM_MODE;
#endif



/* --------------------------------------------------------------------------
** Helper macros.
*/

#ifndef __ASSEMBLER__

#define XDRV_ARRAY_LEN(array) ( sizeof( array ) / sizeof( array[ 0 ]))

#define XDRV_MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define XDRV_MAX(a, b)  (((a) > (b)) ? (a) : (b))

#endif   /* !__ASSEMBLER__ */


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_TYPES_H  */
