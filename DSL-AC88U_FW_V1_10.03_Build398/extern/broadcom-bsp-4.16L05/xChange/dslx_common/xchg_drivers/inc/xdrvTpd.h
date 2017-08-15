/***************************************************************************
*    Copyright © 2000-2008 Broadcom                                        
*                                                                           
*    This program is the proprietary software of Broadcom and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized   
*    License").  Except as set forth in an Authorized License, Broadcom    
*    grants no license (express or implied), right to use, or waiver of any 
*    kind with respect to the Software, and Broadcom expressly reserves all 
*    rights in and to the Software and all intellectual property rights     
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO 
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM   
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.                               
*                                                                           
*                                                                           
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,          
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR  
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY        
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,    
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,    
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT  
*    OF USE OR PERFORMANCE OF THE SOFTWARE.                                 
*                                                                           
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL       
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, 
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN  
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR    
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE 
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: xdrvTpd.h
*
****************************************************************************
*    Description:
*      This file contains definitions related to the linetests
*
****************************************************************************/
#ifndef XDRV_TPD_H
#define XDRV_TPD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xdrvTypes.h>

/* ---- Constants and Types ---------------------------------------------- */

/* DROP test - Hazard-potencial Foreign-elctromotive force results */
typedef struct
{
   XDRV_SINT32 tipAC;
   XDRV_SINT32 tipDC;
   XDRV_SINT32 ringAC;
   XDRV_SINT32 ringDC;
   
} HSZDROPRSLT_HPFEF;

/* DROP test - Receiver Off Hook results */
typedef struct
{
   XDRV_SINT32 phoneOnhook;
   
} HSZDROPRSLT_ROH;

/* DROP test - Resistive Faults results */
typedef struct
{
   XDRV_UINT32 tipGroundImpedance;
   XDRV_UINT32 ringGroundImpedance;
   XDRV_UINT32 tipRingImpedance;
   
} HSZDROPRSLT_RFLT;

/* DROP test - Ringing results */
typedef struct
{
   XDRV_SINT32 RENValue1;
   XDRV_SINT32 RENValue2;
   XDRV_SINT32 tipRingVoltRingAC;
   XDRV_SINT32 iLoopRingAC;      
   
} HSZDROPRSLT_RNG;

/* DROP test - Calibration results */
typedef struct
{
   XDRV_SINT32 RngVolOffset;
   XDRV_SINT32 TipOffset;
   XDRV_SINT32 RingOffset;
   XDRV_SINT32 HvgOffset;
   
} HSZDROPRSLT_CAL;

/* DROP test - Complete results */
typedef struct
{
   HSZDROPRSLT_HPFEF  dropHpFefRslt;
   HSZDROPRSLT_ROH    dropRohRslt;
   HSZDROPRSLT_RFLT   dropRfltRslt;
   HSZDROPRSLT_RNG    dropRngRslt;
   HSZDROPRSLT_CAL    dropCalRslt;
   
} HSZDROPRSLT;

/* SELF test - Complete results */
typedef struct
{
   XDRV_SINT32 selfAssertPassed;
   XDRV_SINT32 selfDspCountPassed;
   XDRV_SINT32 selfHvgPassed;
   XDRV_SINT32 selfApmPassed;
   XDRV_SINT32 selfLoop;
   XDRV_SINT32 selfBattery;
   XDRV_SINT32 selfRing;
   XDRV_SINT32 selfCap;
   
} HSZSELFRSLT;

/* TPD Commands */
typedef enum XDRV_TPD_CMD_DROP
{
   XDRV_TPD_RUN_DROP_HPFEF        = 0x01,
   XDRV_TPD_RUN_DROP_RESFLT       = 0x02,
   XDRV_TPD_RUN_DROP_RING         = 0x04,
   XDRV_TPD_RUN_DROP_ROH          = 0x08, 
   XDRV_TPD_RUN_DROP_OFFHOOK      = 0x10, 
   XDRV_TPD_RUN_DROP_CALIBRATION  = 0x20
   
} XDRV_TPD_CMD_DROP;

/* TPD Commands */
typedef enum XDRV_TPD_CMD_SELF
{
   XDRV_TPD_RUN_SELF_ASSERT  = 0x01,
   XDRV_TPD_RUN_SELF_DSP     = 0x02,
   XDRV_TPD_RUN_SELF_HVG     = 0x04,
   XDRV_TPD_RUN_SELF_APM     = 0x08,
   XDRV_TPD_RUN_SELF_LOOP    = 0x10,
   XDRV_TPD_RUN_SELF_VOLTAGE = 0x20,
   XDRV_TPD_RUN_SELF_RING    = 0x40,
   XDRV_TPD_RUN_SELF_CAP     = 0x80,
   XDRV_TPD_RUN_SELF_ALL     = 0x3F
   
} XDRV_TPD_CMD_SELF;

/* TPD Return Status */
typedef enum XDRV_TPD_STATUS
{
   XDRV_TPD_SUCCESS,                 /* TPD success */
   XDRV_TPD_FAILURE,                 /* TPD failure */
   XDRV_TPD_ERR_ABORT                /* TPD test aborted */
   
} XDRV_TPD_STATUS;

/* --------------------------------------------------------------------------
** 'XDRV_TPD' is the base, generic TPD structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific TPD drivers.
**
** 'XDRV_TPD_FUNCS' must be the only structure member of 'XDRV_TPD'.
**
** Chip specific TPD drivers should provide a "derived" version of XDRV_TPD:
**
**    typedef struct TPD_CHIP_DRV
**    {
**       const XDRV_TPD_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } TPD_CHIP_DRV;
**
** Clients, which have access to XDRV_TPD structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_TPD struct as the first parameter. This struct can be cast to the chip
** specific version of the TPD structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_TPD_FUNCS *pDrvFuncs;

} XDRV_TPD;


/* --------------------------------------------------------------------------
** TPD API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvTpdRunDropTests
**
** PURPOSE:    Run TPD Drop Tests
**
** PARAMETERS: chan - channel to perform tests on
**             cmd  - tests to perform
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
typedef XDRV_TPD_STATUS (*xdrvTpdRunDropTests)( int chan, int cmd, HSZDROPRSLT *dropRsltp );

/*
*****************************************************************************
** FUNCTION:   xdrvTpdRunSelfTests
**
** PURPOSE:    Run TPD Self Tests
**
** PARAMETERS: chan - channel to perform tests on
**             cmd - tests to perform
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
typedef XDRV_TPD_STATUS (*xdrvTpdRunSelfTests)( int chan, int cmd, HSZSELFRSLT *selfRsltp );

/*
*****************************************************************************
** FUNCTION:   xdrvTtpdTestingSetup
**
** PURPOSE:    Set up TPD testing suite
**
** PARAMETERS: chan - channel to perform tests on
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
typedef XDRV_TPD_STATUS (*xdrvTpdTestingSetup)( int chan );

/*
*****************************************************************************
** FUNCTION:   xdrvTpdTestingShutdown
**
** PURPOSE:    Shutdown TPD testing suite
**
** PARAMETERS: chan      - channel to perform tests on
**             isEnabled - boolean indicating admin status of this line 
**                         (enabled = 1; disabled = 0)
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
typedef XDRV_TPD_STATUS (*xdrvTpdTestingShutdown)( int chan, XDRV_BOOL isEnabled );

/*
*****************************************************************************
** FUNCTION:   xdrvTpdChanReset
**
** PURPOSE:    Reset the given chan
**
** PARAMETERS: chan - Channel to reset
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
typedef XDRV_TPD_STATUS (*xdrvTpdChanReset)( int chan );

/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_TPD_FUNCS
{
   xdrvTpdRunDropTests       tpdRunDropTests;
   xdrvTpdRunSelfTests       tpdRunSelfTests;
   xdrvTpdTestingSetup       tpdTestingSetup;
   xdrvTpdTestingShutdown    tpdTestingShutdown;
   xdrvTpdChanReset          tpdChanReset;
} XDRV_TPD_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the SLIC.
*/

#define xdrvTpdRunDropTests( pDrv, chan, cmd, dropRsltp ) \
         ( pDrv->pDrvFuncs->tpdRunDropTests( chan, cmd, dropRsltp ) )

#define xdrvTpdRunSelfTests( pDrv, chan, cmd, selfRsltp ) \
         ( pDrv->pDrvFuncs->tpdRunSelfTests( chan, cmd, selfRsltp ) )

#define xdrvTpdTestingSetup( pDrv, chan ) \
         ( pDrv->pDrvFuncs->tpdTestingSetup( chan ) )

#define xdrvTpdTestingShutdown( pDrv, chan, isEnabled ) \
         ( pDrv->pDrvFuncs->tpdTestingShutdown( chan, isEnabled ) )

#define xdrvTpdChanReset( pDrv, chan ) \
         ( pDrv->pDrvFuncs->tpdChanReset( chan ) )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#if __cplusplus
}
#endif

#endif   /* XDRV_TPD_H */

