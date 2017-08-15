/***************************************************************************
*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
* :>
*
****************************************************************************
*
*    Filename: vrgTypes.h
*
****************************************************************************
*    Description:
*
*     Common types and macros that are shared by all Voice Residential
*     Gateway (VRG) modules.
*
****************************************************************************/

#ifndef VRG_TYPES_H
#define VRG_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/* Simple types */
typedef unsigned char   VRG_UINT8;
typedef signed char     VRG_SINT8;
typedef unsigned short  VRG_UINT16;
typedef signed short    VRG_SINT16;
typedef unsigned long   VRG_UINT32;
typedef long            VRG_SINT32;


/* Boolean */
typedef  int   VRG_BOOL;               /** VRG_TRUE or VRG_FALSE */
#define VRG_TRUE  1
#define VRG_FALSE 0               
#define VRG_MAX(x,y)              ( ( (x) > (y) ) ? (x) : (y) )

/*-------------------------------------------------------------------------
* OFFSETOF returns the offset (in units of sizeof) of a structure member
* from the beginning of a structure
*/

#define VRG_OFFSETOF( structName, memberName )  ( (size_t)&((structName *)0)->memberName )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* VRG_TYPES_H  */
