/****************************************************************************
*
* Copyright © 2000-2008 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: xdrvDebug.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface for debug module.
*
****************************************************************************/

#ifndef XDRV_DEBUG_H
#define XDRV_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* --------------------------------------------------------------------------
** 'XDRV_DEBUG' is the base, generic Debug structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific Debug drivers.
**
** 'XDRV_DEBUG_FUNCS' must be the only structure member of 'XDRV_DEBUG'.
**
** Chip specific Debug drivers should provide a "derived" version of XDRV_DEBUG:
**
**    typedef struct DEBUG_CHIP_DRV
**    {
**       const XDRV_DEBUG_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } DEBUG_CHIP_DRV;
**
** Clients, which have access to XDRV_DEBUG structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_DEBUG struct as the first parameter. This struct can be cast to the chip
** specific version of the Debug structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_DEBUG_FUNCS *pDrvFuncs;

} XDRV_DEBUG;


/* --------------------------------------------------------------------------
** DEBUG API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvDebugDspReadCnt
**
** PURPOSE:    To read the value of the DSP's background & interrupt counters.
**
** PARAMETERS: pDrv - pointer to the Debug driver device structure.
**
** RETURNS:    0 on success
**
** NOTE:
*****************************************************************************
*/
typedef int (*xdrvDebugDspReadCnt)( void );

/*
*****************************************************************************
** FUNCTION:   xdrvDebugDspForceAssert
**
** PURPOSE:    Force DSP assert.
**
** PARAMETERS: pDrv - pointer to the Debug driver device structure.
**
** RETURNS:    0 on success
**
** NOTE:
*****************************************************************************
*/
typedef int (*xdrvDebugDspForceAssert)( void );

/*
*****************************************************************************
** FUNCTION:   xdrvDebugDspLogAssertContext
**
** PURPOSE:    To log the state of the DSP register set and stack context. This
**             function is intented to be called after the DSP asserts.
**
** PARAMETERS: pDrv - pointer to the Debug driver device structure.
**
** RETURNS:    0 on success
**
** NOTE:       When the DSP asserts, it is assumed that it will push the contents
**             of it's register set onto the stack, and then place the value
**             of the stack pointer into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
typedef int (*xdrvDebugDspLogAssertContext)( void );

/*
*****************************************************************************
** FUNCTION:   xdrvDebugDspGetAssertStackPtr
**
** PURPOSE:    Get DSP assert stack Ptr.
**
** PARAMETERS: pDrv - pointer to the Debug driver device structure.
**
** RETURNS:    Assert stack pointer.
**
** NOTE:       When the DSP asserts, it overwrites the "doing fine" indicator
**             with the actual stack pointer. This routine can be used to
**             Check if the DSP is doing fine
*****************************************************************************
*/
typedef unsigned int (*xdrvDebugDspGetAssertStackPtr)( void );

/*
*****************************************************************************
** FUNCTION:   xdrvDebugDspGetRegs
**
** PURPOSE:    Get a snapshot of DSP registers.
**
** PARAMETERS: pDrv - pointer to the Debug driver device structure.
**
** RETURNS:    0 on success
**
** NOTE:       When the DSP receives the getreg command, it places a copy
**             of its registers into an internal memory region, and then place
**             the address of that place into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
typedef int (*xdrvDebugDspGetRegs)( void );

/*
*****************************************************************************
** FUNCTION:   xdrvDebugDspDumpDataMem
**
** PURPOSE:    To log DSP data memory to the console.
**
** PARAMETERS: pDrv           - pointer to the Debug driver device structure.
**             dspAddr        - DSP data address to start log from.
**             amountToRead   - Amount of data to dump.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
typedef void (*xdrvDebugDspDumpDataMem)( unsigned int dspAddr, 
                                         unsigned int amountToRead );

/*
*****************************************************************************
** FUNCTION:   xdrvDebugDspStackShow
**
** PURPOSE:    To display stack usage statistics for the DSP.
**
** PARAMETERS: pDrv - pointer to the Debug driver device structure.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
typedef void (*xdrvDebugDspStackShow)( void );


/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_DEBUG_FUNCS
{
   xdrvDebugDspReadCnt           debugDspReadCnt;
   xdrvDebugDspForceAssert       debugDspForceAssert;
   xdrvDebugDspLogAssertContext  debugDspLogAssertContext;
   xdrvDebugDspGetAssertStackPtr debugDspGetAssertStackPtr;
   xdrvDebugDspGetRegs           debugDspGetRegs;
   xdrvDebugDspDumpDataMem       debugDspDumpDataMem;
   xdrvDebugDspStackShow         debugDspStackShow;

} XDRV_DEBUG_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the Debug.
*/

#define xdrvDebugDspReadCnt( pDrv ) \
         ( (pDrv->pDrvFuncs) ? pDrv->pDrvFuncs->debugDspReadCnt() : (-1) )

#define xdrvDebugDspForceAssert( pDrv ) \
         ( (pDrv->pDrvFuncs) ? pDrv->pDrvFuncs->debugDspForceAssert() : (-1) )

#define xdrvDebugDspLogAssertContext( pDrv ) \
         ( (pDrv->pDrvFuncs) ? pDrv->pDrvFuncs->debugDspLogAssertContext() : (-1) )

#define xdrvDebugDspGetAssertStackPtr( pDrv ) \
         ( (pDrv->pDrvFuncs) ? pDrv->pDrvFuncs->debugDspGetAssertStackPtr() : (-1) )

#define xdrvDebugDspGetRegs( pDrv ) \
         ( (pDrv->pDrvFuncs) ? pDrv->pDrvFuncs->debugDspGetRegs() : (-1) )

#define xdrvDebugDspDumpDataMem( pDrv, dspAddr, amountToRead ) \
         ( pDrv->pDrvFuncs->debugDspDumpDataMem( (dspAddr), (amountToRead) ) )

#define xdrvDebugDspStackShow( pDrv ) \
         ( pDrv->pDrvFuncs->debugDspStackShow() )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_DEBUG_H  */
