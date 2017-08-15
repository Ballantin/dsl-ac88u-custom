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
*    Filename: silabsCommonTpd.c
*    Creation Date: 03 Feb 2015 (v0.00)
*    VSS Info:
*        $Revision: 01 $
*        $Date: 02/03/15 6:56p $
*
****************************************************************************
*    Description:
*
*      This file contains functions related to linetests for the Silabs
*      SLIC on the 63xx reference designs
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
#include <silabsCommonTpd.h>
#include <silabsCommonSlic.h>
#include <vrgLog.h>
#include <casCtl.h>
#include <globalCfg.h>
#include <boardHalTpd.h>

#if XDRV_SILABS_TPD_SUPPORT
#include <proslic_mlt.h>
#endif

#if XDRV_SILABS_TPD_SUPPORT
ProSLICMLTType gProSLICMLT;
#endif
/* ---- Local Type Definitions ------------------------------- */
/* ---- Private Constants ------------------------------------ */

/* TPD Driver Pointers */
static XDRV_SLIC *pSlicDriver[GLOBAL_CFG_MAX_ENDPT];
static XDRV_DEBUG *pDebugDriver[GLOBAL_CFG_MAX_ENDPT];

/* ---- Private Function Prototypes -------------------------- */

/* SLIC "meta-driver" interface functions */
static XDRV_TPD_STATUS tpdRunDropTests( int chan, int cmd, HSZDROPRSLT *dropRsltp );
static XDRV_TPD_STATUS tpdRunSelfTests( int chan, int cmd, HSZSELFRSLT *selfRsltp );
static XDRV_TPD_STATUS tpdTestingSetup( int chan );
static XDRV_TPD_STATUS tpdTestingShutdown( int chan, XDRV_BOOL isEnabled );
static XDRV_TPD_STATUS tpdChanReset( int chan );

/* SLIC "meta-driver" interface */
static const XDRV_TPD_FUNCS tpdSilabsDrvFuncs =
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
#if XDRV_SILABS_TPD_SUPPORT
   int status;
   int activeChan;
   int samples = 30;
   int done = RC_MLT_TEST_RUNNING;
   SI_SLIC_CHANNEL_INFO *pDev; 
   ProSLIC_mlt_foreign_voltages_state vState;
   
   activeChan = boardHalPcmTpdMapLineId( chan );
   pDev = (SI_SLIC_CHANNEL_INFO *)pSlicDriver[activeChan];
   
   /* Initialize the results to zero */
   memset( hpfefRsltp, 0, sizeof( HSZDROPRSLT_HPFEF ));
   memset( &gProSLICMLT, 0, sizeof( ProSLICMLTType ));
   
   gProSLICMLT.pProslic = (proslicChanType_ptr)pDev->pProSlicChan;
   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "gProSLICMLT.pProslic->deviceId->chipType:%d", gProSLICMLT.pProslic->deviceId->chipType));  
   
   /* Call initialization function */
   status = ProSLIC_mlt_init_foreign_voltages(&vState, samples);
   if ( status == RC_MLT_NO_ERROR )
   {
     do
     {
       /* Call MLT Function */
        done = ProSLIC_mlt_foreign_voltages(&gProSLICMLT, &vState);
       
        /* Wait for poll period */
         bosSleep(10);
      
      } while(done == RC_MLT_TEST_RUNNING);
      
   }
   
   /* Process Return Code */
   if(done != RC_MLT_TEST_COMPLETE)
   {
     if( done == RC_MLT_TEST_RUNNING )
     {
        XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "### RC_MLT_TEST_RUNNING ###" ));
     }
     return XDRV_TPD_FAILURE;
   }
   
   /* Store results */
   hpfefRsltp->tipAC = abs(gProSLICMLT.hazVAC.measTG);
   hpfefRsltp->tipDC = abs(gProSLICMLT.hazVDC.measTG);
   hpfefRsltp->ringAC = abs(gProSLICMLT.hazVAC.measRG);
   hpfefRsltp->ringDC = abs(gProSLICMLT.hazVDC.measRG);

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** "));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of HAZ & FOR V readings ... \n"));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: tipAC  = %.8d, tipDC  = %.8d \n", (int)hpfefRsltp->tipAC, (int)hpfefRsltp->tipDC));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: ringAC = %.8d, ringDC = %.8d \n", (int)hpfefRsltp->ringAC, (int)hpfefRsltp->ringDC));

#else
   /* Initialize the results to zero */
   memset( hpfefRsltp, 0, sizeof( HSZDROPRSLT_HPFEF ));
#endif
   return ( XDRV_TPD_SUCCESS );
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
   return XDRV_TPD_SUCCESS;
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
#if XDRV_SILABS_TPD_SUPPORT
   int status;
   int activeChan;
   int done = RC_MLT_TEST_RUNNING;
   ProSLIC_mlt_roh_state rState;
   SI_SLIC_CHANNEL_INFO *pDev; 
   
   activeChan = boardHalPcmTpdMapLineId( chan );
   pDev = (SI_SLIC_CHANNEL_INFO *)pSlicDriver[activeChan];
   
   /* Initialize the results to zero */
   memset( rohRsltp, 0, sizeof( HSZDROPRSLT_ROH ));
   memset( &gProSLICMLT, 0, sizeof( ProSLICMLTType ));
   
   gProSLICMLT.pProslic = (proslicChanType_ptr)pDev->pProSlicChan;
   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "gProSLICMLT.pProslic->deviceId->chipType:%d", gProSLICMLT.pProslic->deviceId->chipType));  
   
   /* Call initialization function */
   status = ProSLIC_mlt_init_receiver_offhook(&rState);
   if ( status == RC_MLT_NO_ERROR )
   {
      do
      {
         /* Call MLT Function */
         done = ProSLIC_mlt_receiver_offhook(&gProSLICMLT, &rState);
         
         /* Wait for poll period */
         bosSleep(10);
      
      } while(done == RC_MLT_TEST_RUNNING);
   }
   
   /* Process Return Code */
   if(done != RC_MLT_TEST_COMPLETE)
   {
      if( done == RC_MLT_TEST_RUNNING )
      {
         XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "### RC_MLT_TEST_RUNNING ###" ));
      }
      return XDRV_TPD_FAILURE;
   }
  
   switch(gProSLICMLT.roh.rohTrue)
   { 
      case RC_NONE:
        rohRsltp->phoneOnhook = XDRV_TRUE;
        break;
      case RC_MLT_ROH_FAIL_ROH: 
         XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: Offhook receiver detected. \n"));  
         break;
      case RC_MLT_ROH_FAIL_RESFAULT: 
         XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: Resistive fault detected. \n")); 
         break;
      default:
         break;
   }


   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ROH readings ... \n")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: phoneOnhook = %.8d ", (int)rohRsltp->phoneOnhook ));  
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
#if XDRV_SILABS_TPD_SUPPORT
   int status;
   int activeChan;
   int done = RC_MLT_TEST_RUNNING;
   ProSLIC_mlt_rmeas_state rState;
   SI_SLIC_CHANNEL_INFO *pDev; 
   
   activeChan = boardHalPcmTpdMapLineId( chan );
   pDev = (SI_SLIC_CHANNEL_INFO *)pSlicDriver[activeChan];
   
   /* Initialize the results to zero */
   memset( rfltRsltp, 0, sizeof( HSZDROPRSLT_RFLT ));
   memset( &gProSLICMLT, 0, sizeof( ProSLICMLTType ));
   
   gProSLICMLT.pProslic = (proslicChanType_ptr)pDev->pProSlicChan;
   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "gProSLICMLT.pProslic->deviceId->chipType:%d", gProSLICMLT.pProslic->deviceId->chipType));  
   
   /* Call initialization function */
   status = ProSLIC_mlt_init_resistive_faults(&rState);
   if ( status == RC_MLT_NO_ERROR )
   {
      do
      {
         /* Call MLT Function */
         done = ProSLIC_mlt_resistive_faults(&gProSLICMLT, &rState);
         
         /* Wait for poll period */
         bosSleep(10);
      
      } while(done == RC_MLT_TEST_RUNNING);
   }
   
   /* Process Return Code */
   if(done != RC_MLT_TEST_COMPLETE)
   {
      if( done == RC_MLT_TEST_RUNNING )
      {
         XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "### RC_MLT_TEST_RUNNING ###" ));
      }
      return XDRV_TPD_FAILURE;
   }
   
   /* Store results */ 
   rfltRsltp->tipRingImpedance    = gProSLICMLT.resFaults.measTR / 10;
   rfltRsltp->tipGroundImpedance  = gProSLICMLT.resFaults.measTG / 10;
   rfltRsltp->ringGroundImpedance = gProSLICMLT.resFaults.measRG / 10;
   
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
#if XDRV_SILABS_TPD_SUPPORT
   int status;
   int activeChan;
   int done = RC_MLT_TEST_RUNNING;
   ProSLIC_mlt_ren_state rState;
   SI_SLIC_CHANNEL_INFO *pDev;
   
   activeChan = boardHalPcmTpdMapLineId( chan );
   pDev = (SI_SLIC_CHANNEL_INFO *)pSlicDriver[activeChan];

   /* Initialize the results to zero */
   memset( rngRsltp, 0, sizeof( HSZDROPRSLT_RNG ));
   memset( &gProSLICMLT, 0, sizeof( ProSLICMLTType ));
   
   /* REN Calibration */
   ProSLIC_mlt_init_ren_cal(&gProSLICMLT);

   gProSLICMLT.pProslic = (proslicChanType_ptr)pDev->pProSlicChan; 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "gProSLICMLT.pProslic->deviceId->chipType:%d", gProSLICMLT.pProslic->deviceId->chipType));
   
   /* Call initialization function */
   status = ProSLIC_mlt_init_ren(&rState);
   if ( status == RC_MLT_NO_ERROR )
   {
      do
      {
         /* Call MLT Function */
         done = ProSLIC_mlt_ren(&gProSLICMLT, &rState);

         /* Wait for poll period */
         bosSleep(10);

      } while(done == RC_MLT_TEST_RUNNING);
   }
   
   /* Process Return Code */
   if(done != RC_MLT_TEST_COMPLETE)
   {
      if( done == RC_MLT_TEST_RUNNING )
      {
         XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "### RC_MLT_TEST_RUNNING ###" ));
      }
      if( done == RC_MLT_FALSE_RINGTRIP )
      {
         XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "### RC_MLT_FALSE_RINGTRIP ###" ));
      }
      if( done == RC_MLT_ALARM_ON_RING )
      {
         XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "### RC_MLT_ALARM_ON_RING ###" ));
      }
      if( done == RC_MLT_RINGSTART_ERR )
      {
         XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "### RC_MLT_RINGSTART_ERR ###" ));
      }
      return XDRV_TPD_FAILURE;
   }

   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "gProSLICMLT.ren.renValue = %d", (int)gProSLICMLT.ren.renValue));
   
   /* Store results */
   rngRsltp->RENValue1 = gProSLICMLT.ren.renValue;
   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "Summary of RING readings ... \n")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_SLIC, "ring: REN = %d", (int)rngRsltp->RENValue1 ));   

#else
   /* Initialize the results to zero */
   memset( rngRsltp, 0, sizeof( HSZDROPRSLT_RNG ));
#endif

   return XDRV_TPD_SUCCESS;
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
   return XDRV_TPD_SUCCESS;
}

/*
*****************************************************************************
** FUNCTION:   silabsTpdInitChannel
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
int silabsTpdInitChannel( int chan, XDRV_TPD *tpdDrv, XDRV_SLIC *slicDrv,
                           XDRV_APM *apmDrv, XDRV_DEBUG *debugDrv )
{
   /* Set the "meta-driver" interface */
   tpdDrv->pDrvFuncs = &tpdSilabsDrvFuncs;
   
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
   return XDRV_TPD_SUCCESS;
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
   /* Disable CAS by sending CAS control command to go service denied state */
   casCtlSetState( chan, CAS_CTL_FXO_SERVICEDENIED );

   /* Wait for CAS to properly shutdown */
   bosSleep( 40 );

   return XDRV_TPD_SUCCESS;
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
   if (isEnabled)
   {
      /* Reenable CAS by sending CAS control command to go idle state with normal polarity */
      casCtlSetState( chan, CAS_CTL_FXO_IDLE );

      /* Wait for CAS to properly start up */
      bosSleep( 40 );
   }
   else
   {
      /* Disable CAS by sending CAS control command to go service denied state */
      casCtlSetState( chan, CAS_CTL_FXO_SERVICEDENIED );

      /* Wait for CAS to properly shutdown */
      bosSleep( 40 );
   }
   return XDRV_TPD_SUCCESS;
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

