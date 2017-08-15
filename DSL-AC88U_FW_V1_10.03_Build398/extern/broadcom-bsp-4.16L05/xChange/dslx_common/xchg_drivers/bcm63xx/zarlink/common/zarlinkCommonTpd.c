/****************************************************************************
*
* Copyright (c) 2000-2014 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
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
*****************************************************************************
*
*    Filename: zarlinkCommonTpd.c
*    Creation Date: 04 Oct 2000 (v0.00)
*    VSS Info:
*        $Revision: 44 $
*        $Date: 2/21/03 6:56p $
*
****************************************************************************
*    Description:
*
*      This file contains functions related to linetests for the Zarlink
*      SLIC on the 33xx reference designs
*
****************************************************************************/

#include <bosTask.h>
#include <bosSleep.h>
#include <math.h>
#include <board.h>
#include <xdrvCfg.h>
#include <xdrvLog.h>
#include <xdrvTimer.h>
#include <xdrvSlic.h>
#include <xdrvApm.h>
#include <xdrvTpd.h>
#include <zarlinkCommonTpd.h>
#include <zarlinkCommonTpdIntApi.h>
#include <zarlinkCommonSlic.h>
#include <vrgLog.h>
#include <casCtl.h>
#include <globalCfg.h>

/* ---- Local Type Definitions ------------------------------- */
/* ---- Private Constants ------------------------------------ */

/* TPD Driver Pointers */
static XDRV_SLIC *pSlicDriver[GLOBAL_CFG_MAX_ENDPT];
static XDRV_DEBUG *pDebugDriver[GLOBAL_CFG_MAX_ENDPT];

#if XDRV_ZARLINK_TPD_SUPPORT
#include <boardHalTpd.h>
LtTestCtxType ltTestCtx[GLOBAL_CFG_MAX_ENDPT];
LtTestStatusType ltLastStatus[GLOBAL_CFG_MAX_ENDPT];
uint16 ltLastEventId[GLOBAL_CFG_MAX_ENDPT];
#endif

/* ---- Private Function Prototypes -------------------------- */

/* SLIC "meta-driver" interface functions */
static XDRV_TPD_STATUS tpdRunDropTests( int chan, int cmd, HSZDROPRSLT *dropRsltp );
static XDRV_TPD_STATUS tpdRunSelfTests( int chan, int cmd, HSZSELFRSLT *selfRsltp );
static XDRV_TPD_STATUS tpdTestingSetup( int chan );
static XDRV_TPD_STATUS tpdTestingShutdown( int chan, XDRV_BOOL isEnabled );
static XDRV_TPD_STATUS tpdChanReset( int chan );

/* SLIC "meta-driver" interface */
static const XDRV_TPD_FUNCS tpdZarlinkDrvFuncs =
{
   tpdRunDropTests,
   tpdRunSelfTests,
   tpdTestingSetup,
   tpdTestingShutdown,
   tpdChanReset
};

/* Private Functions */
static XDRV_UINT16 linetestDropHazardForeign(int chan, HSZDROPRSLT_HPFEF *hpfefRsltp);
static XDRV_UINT16 linetestDropROH(int chan, HSZDROPRSLT_ROH *rohRsltp);
static XDRV_UINT16 linetestDropResFlt(int chan, HSZDROPRSLT_RFLT *rfltRsltp);
static XDRV_UINT16 linetestDropRing(int chan, HSZDROPRSLT_RNG *rngRsltp);
static XDRV_UINT16 linetestDropCalibration(int chan, HSZDROPRSLT_CAL *calRsltp);
static XDRV_UINT16 linetestDropOffhook(int chan);

static XDRV_UINT16 linetestSelfAssert(int chan, XDRV_SINT32 *selfAssertPassed);
static XDRV_UINT16 linetestSelfDspCount(int chan, XDRV_SINT32 *selfDspCountPassed);
static XDRV_UINT16 linetestSelfHvg(int chan, XDRV_SINT32 *selfHvgPassed);
static XDRV_UINT16 linetestSelfApm(int chan, XDRV_SINT32 *selfApmPassed);

static void tpdCasEnable( int chan );
static void tpdCasDisable( int chan );

#if XDRV_ZARLINK_TPD_SUPPORT
static void tpdExecuteTest( VpLineCtxType *pLineCtx, LtTestIdType testId, unsigned int chan,
                            LtTestAttributesType *pAttributes, LtTestTempType *pTemp,
                            LtTestResultType *pResult, LtTestCtxType *pTestCtx );
#endif

/*
*****************************************************************************
** FUNCTION:   linetestDropHazardForeign
**
** PURPOSE:    Runs the Hazardous Potential and the Foreign Electomotive
**             Force tests on a specified channel
**
** PARAMETERS: chan       - channel to run the tests on
**             hpfefRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             HAZARD_FAIL if Hazardous Potential test fails,
**             FOREIGN_FAIL if Foreign Electomotive Force test fails,
**             PASS if both pass
*****************************************************************************
*/
static XDRV_UINT16 linetestDropHazardForeign( int chan, HSZDROPRSLT_HPFEF *hpfefRsltp )
{
#if XDRV_ZARLINK_TPD_SUPPORT
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pSlicDriver[chan];
   VpLineCtxType     *pLineCtx = pDev->pChanLineCtx;
   LtTestAttributesType ltAttrib;
   LtTestTempType ltTemp;
   LtTestResultType ltResult;
   LtLineVResultType *pResult = &ltResult.result.lineV;

   /* Initialize the results to zero */
   memset( hpfefRsltp, 0, sizeof( HSZDROPRSLT_HPFEF ));

   /* Setup test attributes */
   ltAttrib.inputs.pLineVInp = NULL;
   ltAttrib.topology.pVp886Topology = NULL;
   ltAttrib.calFactors.dummyPtr = NULL;
   ltAttrib.criteria.pLintVCrt = NULL;
   ltAttrib.ltDbgFlag = LT_DBG_ERRR | LT_DBG_WARN;

   chan = boardHalPcmTpdMapLineId( chan );
   
   /* Execute test */
   tpdExecuteTest( pLineCtx, LT_TID_LINE_V, chan, &ltAttrib,
                   &ltTemp, &ltResult, &ltTestCtx[chan] );

   /* Store results */
   hpfefRsltp->tipAC = abs(pResult->vAcTip);
   hpfefRsltp->tipDC = abs(pResult->vDcTip);
   hpfefRsltp->ringAC = abs(pResult->vAcRing);
   hpfefRsltp->ringDC = abs(pResult->vDcRing);

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of HAZ & FOR V readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: tipAC  = %.8d, tipDC  = %.8d \n", (int)hpfefRsltp->tipAC, (int)hpfefRsltp->tipDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: ringAC = %.8d, ringDC = %.8d \n", (int)hpfefRsltp->ringAC, (int)hpfefRsltp->ringDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: vAcDiff = %.8d, vDcDiff = %.8d \n", (int)pResult->vAcDiff, (int)pResult->vDcDiff));   
#else
   /* Initialize the results to zero */
   memset( hpfefRsltp, 0, sizeof( HSZDROPRSLT_HPFEF ));
#endif

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropOffhook
**
** PURPOSE:    Runs the Offhook simulation test on a specifeid channel
**
** PARAMETERS: chan - channel to run the test on
**
** RETURNS:    SUCCESS or FAILURE
**
*****************************************************************************
*/
static XDRV_UINT16 linetestDropOffhook( int chan )
{
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropROH
**
** PURPOSE:    Runs the Receiver Off Hook test on a specifeid channel
**
** PARAMETERS: chan     - channel to run the test on
**             rohRsltp - pointer to the structure to post the results to
**
** RETURNS:    SUCCESS or FAILURE
**
*****************************************************************************
*/
static XDRV_UINT16 linetestDropROH( int chan, HSZDROPRSLT_ROH *rohRsltp )
{
#if XDRV_ZARLINK_TPD_SUPPORT
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pSlicDriver[chan];
   VpLineCtxType     *pLineCtx = pDev->pChanLineCtx;
   LtTestAttributesType ltAttrib;
   LtTestTempType ltTemp;
   LtTestResultType ltResult;
   LtRohResultType *pResult = &ltResult.result.roh;

   /* Initialize the results to zero */
   memset( rohRsltp, 0, sizeof( HSZDROPRSLT_ROH ));

   /* Setup test attributes */
   ltAttrib.inputs.pRohInp = NULL;
   ltAttrib.topology.pVp886Topology = NULL;
   ltAttrib.calFactors.dummyPtr = NULL;
   ltAttrib.criteria.pRohCrt = NULL;
   ltAttrib.ltDbgFlag = LT_DBG_ERRR | LT_DBG_WARN;

   /* Clear test status tracking variables */
   ltLastEventId[chan] = 0; 
   ltLastStatus[chan] = LT_STATUS_RUNNING;

   chan = boardHalPcmTpdMapLineId( chan );

   /* Execute test */
   tpdExecuteTest( pLineCtx, LT_TID_ROH, chan, &ltAttrib,
                   &ltTemp, &ltResult, &ltTestCtx[chan] );

   switch( pResult->fltMask)
   {
      case LT_ROHM_TEST_PASSED:
        rohRsltp->phoneOnhook = XDRV_TRUE;
        break;
      case LT_ROHM_RES_LOOP: /* 0x02 */
         XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: A differential resistive loop was detected. \n"));  
         break;
      case LT_ROHM_OUT_OF_RANGE_LOOP: /* 0x04 */
         XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: The differential loop is beyond the measurement capabilities of the device. \n")); 
         break;
      case LT_ROHM_MSRMNT_STATUS:
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "roh: Measurement error! measStatus=0x%.8X", (unsigned int)pResult->measStatus)); 
         break;
      default:
         break;
   }
   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ROH readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: phoneOnhook = %.8d ", (int)rohRsltp->phoneOnhook ));  
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: fltMask     =0x%.8X", (unsigned int)pResult->fltMask ));  
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: rLoop1 = %.8d, rLoop2 = %.8d", (int)pResult->rLoop1, (int)pResult->rLoop2 ));   
#else
   /* Initialize the results to zero */
   memset( rohRsltp, 0, sizeof( HSZDROPRSLT_ROH ));
#endif

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropResFlt
**
** PURPOSE:    Runs the Resistive Faults test on a specified channel
**
** PARAMETERS: chan      - channel to run the test on
**             rohRsltp  - pointer to the result structure of the previous test
**             rfltRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
static XDRV_UINT16 linetestDropResFlt(int chan, HSZDROPRSLT_RFLT *rfltRsltp )
{
#if XDRV_ZARLINK_TPD_SUPPORT
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pSlicDriver[chan];
   VpLineCtxType     *pLineCtx = pDev->pChanLineCtx;
   LtTestAttributesType ltAttrib;
   LtTestTempType ltTemp;
   LtTestResultType ltResult;
   LtResFltResultType *pResult = &ltResult.result.resFlt;

   /* Initialize the results to zero */
   memset( rfltRsltp, 0, sizeof( HSZDROPRSLT_RFLT ));

   /* Setup test attributes */
   ltAttrib.inputs.pResFltInp = NULL;
   ltAttrib.topology.pVp886Topology = NULL;
   ltAttrib.calFactors.dummyPtr = NULL;
   ltAttrib.criteria.pResFltCrt = NULL;
   ltAttrib.ltDbgFlag = LT_DBG_ERRR | LT_DBG_WARN;

   chan = boardHalPcmTpdMapLineId( chan );
   
   /* Execute test */
   tpdExecuteTest( pLineCtx, LT_TID_RES_FLT, chan, &ltAttrib,
                   &ltTemp, &ltResult, &ltTestCtx[chan] );

   /* Store results */ 
   rfltRsltp->tipGroundImpedance = pResult->rtg / 10;
   rfltRsltp->tipRingImpedance = pResult->rtr / 10;
   rfltRsltp->ringGroundImpedance = pResult->rrg / 10;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ResFLT readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "tipGroundImpedance = %.8d", (int)rfltRsltp->tipGroundImpedance ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ringGroundImpedance = %.8d", (int)rfltRsltp->ringGroundImpedance ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "tipRingImpedance = %.8d", (int)rfltRsltp->tipRingImpedance ));
#else
   /* Initialize the results to zero */
   memset( rfltRsltp, 0, sizeof( HSZDROPRSLT_RFLT ));
#endif

   return ( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropRing
**
** PURPOSE:    Runs the Ringer test on a specified channel
**
** PARAMETERS: chan     - channel to run the test on
**             rngRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
static XDRV_UINT16 linetestDropRing(int chan, HSZDROPRSLT_RNG *rngRsltp)
{
#if XDRV_ZARLINK_TPD_SUPPORT
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pSlicDriver[chan];
   VpLineCtxType     *pLineCtx = pDev->pChanLineCtx;
   LtTestAttributesType ltAttrib;
   LtTestTempType ltTemp;
   LtTestResultType ltResult;
   LtRingersResultType *pResult = &ltResult.result.ringers;

   /* Initialize the results to zero */
   memset( rngRsltp, 0, sizeof( HSZDROPRSLT_RNG ));

   /* Setup test attributes */
   ltAttrib.inputs.pRingersInp = NULL;
   ltAttrib.topology.pVp886Topology = NULL;
   ltAttrib.calFactors.dummyPtr = NULL;
   ltAttrib.criteria.pRingersCrt = NULL;
   ltAttrib.ltDbgFlag = LT_DBG_ERRR | LT_DBG_WARN;
   
   chan = boardHalPcmTpdMapLineId( chan );
   
   /* Execute test */
   tpdExecuteTest( pLineCtx, LT_TID_RINGERS, chan, &ltAttrib,
                   &ltTemp, &ltResult, &ltTestCtx[chan] );

   /* Store results */
   rngRsltp->RENValue1 = pResult->ren / 1000;
   rngRsltp->RENValue2 = pResult->ren % 1000;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of RING readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: REN = %.8d.%.3d ", (int)rngRsltp->RENValue1, (int)rngRsltp->RENValue2 ));   
#else
   /* Initialize the results to zero */
   memset( rngRsltp, 0, sizeof( HSZDROPRSLT_RNG ));
#endif

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropCalibration
**
** PURPOSE:    Runs the calibration tests for this platform
**
** PARAMETERS: chan     - channel to run the test on
**             calRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
static XDRV_UINT16 linetestDropCalibration(int chan, HSZDROPRSLT_CAL *calRsltp)
{
   /* Nothing to do here */
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfAssert
**
** PURPOSE:    Runs the self assert test on a specifeid channel
**
** PARAMETERS: chan           - channel to run the tests on
**             selfAssertTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
static XDRV_UINT16 linetestSelfAssert( int chan, XDRV_SINT32 *selfAssertPassed )
{
   if ( xdrvDebugDspGetAssertStackPtr( pDebugDriver[chan] ) )
   {
      *selfAssertPassed = XDRV_FALSE;
   }
   else
   {
      *selfAssertPassed = XDRV_TRUE;
   }

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfDspCount
**
** PURPOSE:    Runs the self dsp count test on a specifeid channel
**
** PARAMETERS: chan         - channel to run the tests on
**             dspCountTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
static XDRV_UINT16 linetestSelfDspCount( int chan, XDRV_SINT32 *selfDspCountPassed )
{
   if ( xdrvDebugDspReadCnt( pDebugDriver[chan] ) )
   {
      *selfDspCountPassed = XDRV_FALSE;
   }
   else
   {
      *selfDspCountPassed = XDRV_TRUE;
   }

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfHvg
**
** PURPOSE:    Runs the self hvg test on a specifeid channel
**
** PARAMETERS: chan        - channel to run the tests on
**             selfHvgTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
static XDRV_UINT16 linetestSelfHvg( int chan, XDRV_SINT32 *selfHvgPassed )
{
   *selfHvgPassed = XDRV_TRUE;
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfApm
**
** PURPOSE:    Runs the self apm test on a specifeid channel
**
** PARAMETERS: chan        - channel to run the tests on
**             selfApmTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
static XDRV_UINT16 linetestSelfApm( int chan, XDRV_SINT32 *selfApmPassed )
{
   *selfApmPassed = XDRV_TRUE;
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   zarlinkTpdInitChannel
**
** PURPOSE:    TPD initialization - called once for each channel during initialization
**
** PARAMETERS: chan - channel to initialize
**             tpdDrv - pointer to TPD driver
**             slicDrv - pointer to SLIC driver
**             apmDrv - pointer to APM driver
**             debugDrv - pointer to DEBUG driver
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:
*****************************************************************************
*/
int zarlinkTpdInitChannel( int chan, XDRV_TPD *tpdDrv, XDRV_SLIC *slicDrv,
                           XDRV_APM *apmDrv, XDRV_DEBUG *debugDrv )
{
   /* Set the "meta-driver" interface */
   tpdDrv->pDrvFuncs = &tpdZarlinkDrvFuncs;

   if ( chan < GLOBAL_CFG_MAX_ENDPT )
   {
      pSlicDriver[chan] = slicDrv;
      pDebugDriver[chan] = debugDrv;
   }
   else
   {
      return ( XDRV_TPD_FAILURE );
   }

   return ( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   tpdRunDropTests
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
static XDRV_TPD_STATUS tpdRunDropTests( int chan, int cmd, HSZDROPRSLT *dropRsltp )
{
   /*
   ** Test 1: Run the Hazardous Voltage and Foreign Electromotive tests
   */
   if( cmd & XDRV_TPD_RUN_DROP_HPFEF )
   {
      linetestDropHazardForeign( chan, &dropRsltp->dropHpFefRslt );
   }

   /*
   ** Test 2: Run the Resistive Faults test
   */
   if( cmd & XDRV_TPD_RUN_DROP_RESFLT )
   {
      linetestDropResFlt( chan, &dropRsltp->dropRfltRslt );
   }

   /*
   ** Test 3: Run the Receiver Off Hook test
   */
   if( cmd & XDRV_TPD_RUN_DROP_ROH )
   {
      linetestDropROH( chan, &dropRsltp->dropRohRslt );
   }

   /*
   ** Test 4: Run the Ringer test
   */
   if( cmd & XDRV_TPD_RUN_DROP_RING )
   {
      linetestDropRing( chan, &dropRsltp->dropRngRslt );
   }

   /*
   ** Test 5: Run the Offhook Simulation Test
   */
   if( cmd & XDRV_TPD_RUN_DROP_OFFHOOK )
   {
      linetestDropOffhook( chan );
   }

   /*
   ** Test 6: Run the Calibration Tests
   */
   if( cmd & XDRV_TPD_RUN_DROP_CALIBRATION )
   {
      linetestDropCalibration( chan, &dropRsltp->dropCalRslt );
   }

   return ( XDRV_TPD_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   tpdRunSelfTests
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
static XDRV_TPD_STATUS tpdRunSelfTests( int chan, int cmd, HSZSELFRSLT *selfRsltp )
{
   /*
   ** Test 1: Run the Assert tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_ASSERT )
   {
      linetestSelfAssert( chan, &selfRsltp->selfAssertPassed );
   }

   /*
   ** Test 2: Run the DSP Count tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_DSP )
   {
      linetestSelfDspCount( chan, &selfRsltp->selfDspCountPassed );
   }

   /*
   ** Test 3: Run the HVG Diagnostic tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_HVG )
   {
      linetestSelfHvg( chan, &selfRsltp->selfHvgPassed );
   }

   /*
   ** Test 4: Run the APM tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_APM )
   {
      linetestSelfApm( chan, &selfRsltp->selfApmPassed );
   }

   return ( XDRV_TPD_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   tpdTestingSetup
**
** PURPOSE:    Set up TPD testing suite
**
** PARAMETERS: chan - channel to perform tests on
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdTestingSetup( int chan )
{
   /* Disable CAS */
   tpdCasDisable( chan );

   /* Return SLIC to default hook state */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCF );

   /* Wait for state change to complete */
   bosSleep( 40 );

   return ( XDRV_TPD_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   tpdTestingShutdown
**
** PURPOSE:    Shutdown TPD testing suite
**
** PARAMETERS: chan - channel to perform tests on
**             isEnabled - boolean indicating admin status of this line 
**                         (enabled = 1; disabled = 0)
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdTestingShutdown( int chan, XDRV_BOOL isEnabled )
{
   /* 
   ** Need to put the line into a correct state based on current admin status.
   ** If the line was administratively disabled, we want to disconnect the physical 
   ** interface and if it was enabled, we want to restore to regular operating state 
   */
   if ( isEnabled )
   {
      /* Reenable CAS */
      tpdCasEnable( chan );
   }
   else
   {
      /* Disable CAS */
      tpdCasDisable( chan );
   }

   return ( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
*
*  tpdCasDisable - Disables CAS service on a specified channel
*
*****************************************************************************
*
*  PURPOSE:
*     Disables CAS service on a specified channel
*
*  PARAMETERS:
*     chan - index of the channel to disable
*
*  RETURNS:
*
*  NOTE:
*
*****************************************************************************
*/
static void tpdCasDisable( int chan )
{
   /* Send CAS control command to go service denied state */
   casCtlSetState( chan, CAS_CTL_FXO_SERVICEDENIED );

   /* Wait for CAS to properly shutdown */
   bosSleep( 40 );
}

/*
*****************************************************************************
*
*  tpdCasEnable - Enables CAS service on a specified channel
*
*****************************************************************************
*
*  PURPOSE:
*     Enables CAS service on a specified channel
*
*  PARAMETERS:
*     chan - index of the channel to enable
*
*  RETURNS:
*
*  NOTE:
*
*****************************************************************************
*/
static void tpdCasEnable( int chan )
{
   /* Send CAS control command to go idle state with normal polarity */
   casCtlSetState( chan, CAS_CTL_FXO_IDLE );

   /* Wait for CAS to properly start up */
   bosSleep( 40 );
}

/*
*****************************************************************************
** FUNCTION:   tpdChanReset
**
** PURPOSE:    Reset the given channel
**
** PARAMETERS: chan - channel to reset
**
** RETURNS:    XDRV_TPD_ERR_ABORT if we were interupted by a BOS_STATUS_RESET
**             XDRV_TPD_SUCCESS otherwise
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdChanReset( int chan )
{
   return XDRV_TPD_SUCCESS;
}

#if XDRV_ZARLINK_TPD_SUPPORT
/*
*****************************************************************************
** FUNCTION:   tpdExecuteTest
**
** PURPOSE:    Execute the selected test
**
** PARAMETERS: Test parameters as per zarlink LtStartTest command
**
** RETURNS:    None
**
*****************************************************************************
*/
static void tpdExecuteTest( VpLineCtxType *pLineCtx, LtTestIdType testId, unsigned int chan,
                            LtTestAttributesType *pAttributes, LtTestTempType *pTemp,
                            LtTestResultType *pResult, LtTestCtxType *pTestCtx )
{
   LtTestStatusType status;
   int i;

   /* Clear test status tracking variables */
   ltLastEventId[chan] = 0; 
   ltLastStatus[chan] = LT_STATUS_RUNNING;

   /* Start test */
   status = LtStartTest( pLineCtx, testId, chan,
                         pAttributes, pTemp, pResult, pTestCtx );

   /* Wait for new test results */
   for( i = 0; i < 100; i++ )
   {
      bosSleep(100);
      if( ltLastStatus[chan] != LT_STATUS_RUNNING )
      {
         break;
      }
   }

   /* Check status */
   if( ltLastStatus[chan] != LT_STATUS_DONE )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Aborting test on line %d!!!\n", chan )); 
      status = LtAbortTest( &ltTestCtx[chan] );
      if( status == LT_STATUS_RUNNING )
      {
         for( i = 0; i < 100; i++ )
         {
            bosSleep(100);
            if( ltLastStatus[chan] != LT_STATUS_RUNNING )
            {
               break;
            }
         }
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   tpdGetTestCtx
**
** PURPOSE:    Retrieve the test structure for the selected channel
**
** PARAMETERS: chan - channel in use
**
** RETURNS:    pointer to test structure
**
*****************************************************************************
*/
LtTestCtxType* tpdGetTestCtx( unsigned int chan )
{
   if( chan < GLOBAL_CFG_MAX_ENDPT )
   {
      return &ltTestCtx[chan];
   }
   else
   {
      return NULL;
   }
}

/*
*****************************************************************************
** FUNCTION:   tpdUpdateTestStatus
**
** PURPOSE:    Function for event handler to post test status updates
**
** PARAMETERS: chan - channel in use
**
** RETURNS:    none
**
*****************************************************************************
*/
void tpdUpdateTestStatus( LtTestStatusType status, uint8 channelId, uint16 eventId )
{
   if( channelId < GLOBAL_CFG_MAX_ENDPT )
   {
      ltLastEventId[channelId] = eventId; 
      ltLastStatus[channelId] = status;
   }
   else
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "Test status channel exceeds maximum value: %d\n", channelId )); 
   }
}
#endif

