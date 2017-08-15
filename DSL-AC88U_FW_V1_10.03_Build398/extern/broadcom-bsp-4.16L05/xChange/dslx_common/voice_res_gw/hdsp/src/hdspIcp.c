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
*    Filename: hdspIcp.c
*
****************************************************************************
*    Description:
*
*     Implements for hausware idle CPU profiler.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <vrgTypes.h>
#include <xchgAssert.h>
#include <hapi_rm.h>
#include <hapi_icp.h>
#include <haus_types.h>
#include <hdspCmd.h>
#include <hdspIcp.h>
#include <hdspCfg.h>
#include <boardHalDsp.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/*
 * Idle CPU Profiler Statistic (32 bit version)
 */
typedef struct ICPSTATS32
{
   VRG_UINT32 calibrationCount;    /* Calibration Count                */
   VRG_UINT32 iterations;          /* Number of data points taken      */
   VRG_UINT32 currentCount;        /* Current count value              */
   VRG_UINT32 totalCountLow;       /* Total Count - low 32 bits        */
   VRG_UINT32 totalCountHigh;      /* Total Count - high 32 bits       */
   VRG_UINT32 avgCount;            /* Running average count            */
   VRG_UINT32 maxCount;            /* Maximum count                    */
   VRG_UINT32 minCount;            /* Minimum count                    */

} ICPSTATS32;


/* ---- Private Variables ------------------------------------------------ */

static ICPSTATS32    gIcpStats;
static unsigned int  gEventIntervalSec;
static VRG_BOOL      gVerboseOutput;


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   hdspIcpCalibrate
**
** PURPOSE:    To calibrate the idle CPU profiler.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void hdspIcpCalibrate( void )
{
   VRG_LOG_NOTICE((VRG_LOG_MOD_HDSP,  "On-demand calibration not supported yet!"
              "It is performed automatically during DSP initialization." ));
}


/*
*****************************************************************************
** FUNCTION:   hdspIcpStart
**
** PURPOSE:    To start the idle CPU profiler.
**
** PARAMETERS: verboseOutput     (in)  Display verbose stats (true or false).
**             eventIntervalSec  (in)  Generate ICP statistics periodically
**                                     at this inverval. Stats will be logged.
**                                     If 0 is specified, the ICP stats will
**                                     not be reported until hdspIcpStop()
**                                     is called.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void hdspIcpStart( VRG_BOOL verboseOutput, unsigned int eventIntervalSec )
{
   BOARD_HAL_DSP_CFG dspCfg;


   gEventIntervalSec = eventIntervalSec;
   gVerboseOutput    = verboseOutput;


   /*
   ** Make sure that we support ICP before continuing.
   */
   boardHalDspGetCfg( &dspCfg );
   if ( !dspCfg.isIcpEnabled )
   {
      VRG_LOG_NOTICE((VRG_LOG_MOD_HDSP,  "ICP not supported!" ));
      return;
   }


   /*
   ** Reset profiler statistics.
   */
   if ( HAPISUCCESS != hdspSendCmdAsync( HAPIRESMAN,
                                         HAPI_ICP_RESET_STATS_CMD,
                                         HSCMDDATA,
                                         0,
                                         0 ) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ERROR: Unable to reset profiler." ));
      return;
   }



   /*
   ** Set event reporting interval.
   */
   if ( eventIntervalSec )
   {
      if ( HAPISUCCESS != hdspSendCmdAsync( HAPIRESMAN,
                                            HAPI_ICP_SETREG1_CMD,
                                            HSCMDDATA,
                                            VRG_OFFSETOF( ICPREGS, eventInterval ),
                                            eventIntervalSec * 100 /* 10ms intervals */ ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ERROR: Unable to set event interval." ));
         return;
      }
   }



   /*
   ** Enable the profiler.
   */
   if ( HAPISUCCESS != hdspSendCmdAsync( HAPIRESMAN,
                                         HAPI_ICP_ENABLE_CMD,
                                         HSCMDDATA,
                                         1,
                                         0 ) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ERROR: Unable to enable profiler." ));
      return;
   }
}


/*
*****************************************************************************
** FUNCTION:   hdspIcpRestart
**
** PURPOSE:    Restart the ICP profiler, using the settings from the previous
**             call to hdspIcpStart().
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void hdspIcpRestart( void )
{
   hdspIcpStart( gVerboseOutput, gEventIntervalSec );
}


/*
*****************************************************************************
** FUNCTION:   hdspIcpStop
**
** PURPOSE:    To stop the idle CPU profiler.
**
** PARAMETERS: displayStats   (in)  If true, display current ICP stats (and
**                                  stop the profiler. If false, just stop the
**                                  profiler.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void hdspIcpStop( VRG_BOOL displayStats )
{
   ICPSTATS             icpStats;
   BOARD_HAL_DSP_CFG    dspCfg;


   /*
   ** Make sure that we support ICP before continuing.
   */
   boardHalDspGetCfg( &dspCfg );
   if ( !dspCfg.isIcpEnabled )
   {
      VRG_LOG_NOTICE((VRG_LOG_MOD_HDSP,  "ICP not supported!" ));
      return;
   }


   /* -----------------------------------------------------------------------
   ** Retrieve RAW ICP stats.
   **
   **
   ** Disable the profiler.
   */
   if ( HAPISUCCESS != hdspSendCmdAsync( HAPIRESMAN,
                                         HAPI_ICP_ENABLE_CMD,
                                         HSCMDDATA,
                                         0,
                                         0 ) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ERROR: Unable to disable profiler." ));
      return;
   }



   /* -----------------------------------------------------------------------
   ** Display stats.
   */
   if ( displayStats )
   {
      /*
      ** Get profiler statistics.
      */
      if ( HAPISUCCESS != hdspSendCmdData( HAPIRESMAN,
                                           HAPI_ICP_GETSTATS_CMD,
                                           HSCMDEXTDATA,
                                           sizeof( icpStats ),
                                           &icpStats ) )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ERROR: Unable to get stats." ));
         return;
      }


      hdspIcpDisplayStats( &icpStats );
   }

}



/*
*****************************************************************************
** FUNCTION:   hdspIcpDisplayStats
**
** PURPOSE:    Log ICP statistics.
**
** PARAMETERS: icpStats (in)  ICP stats to log.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void hdspIcpDisplayStats( ICPSTATS *icpStats )
{
#if HDSP_CFG_ICP_USE_FLOATS
   float                avgCount;
   float                minCpuUsagePercent;
   float                maxCpuUsagePercent;
   float                avgCpuUsagePercent;
   float                weightedAvgCpuUsagePercent;
#else /* !HDSP_CFG_ICP_USE_FLOATS */
   int                  avgCount;
   int                  minCpuUsagePercent;
   int                  maxCpuUsagePercent;
   int                  avgCpuUsagePercent;
   int                  weightedAvgCpuUsagePercent;
#endif /* !HDSP_CFG_ICP_USE_FLOATS */

   /* -----------------------------------------------------------------------
   ** Convert RAW stats into CPU usage stats.
   */

   /* Convert 16-bit IPC stats into 32-bit stats. */
   WRITE_UINT32( icpStats->calibrationCount,  gIcpStats.calibrationCount );
   WRITE_UINT32( icpStats->iterations,        gIcpStats.iterations );
   WRITE_UINT32( icpStats->currentCount,      gIcpStats.currentCount );
   WRITE_UINT32( icpStats->totalCountLow,     gIcpStats.totalCountLow );
   WRITE_UINT32( icpStats->totalCountHigh,    gIcpStats.totalCountHigh );
   WRITE_UINT32( icpStats->avgCount,          gIcpStats.avgCount );
   WRITE_UINT32( icpStats->maxCount,          gIcpStats.maxCount );
   WRITE_UINT32( icpStats->minCount,          gIcpStats.minCount );


#if HDSP_CFG_ICP_USE_FLOATS
   /*
   * Calculate the true average. We don't really want to do 64-bit math,
   ** so let's assume that the 32-bit 'totalCountHigh' value is 0.
   */
   avgCount = (float) gIcpStats.totalCountLow / gIcpStats.iterations;
#else /* !HDSP_CFG_ICP_USE_FLOATS */
   avgCount = gIcpStats.totalCountLow / gIcpStats.iterations;
#endif /* !HDSP_CFG_ICP_USE_FLOATS */
   if ( gIcpStats.totalCountHigh != 0 )
   {
      VRG_LOG_WARN((VRG_LOG_MOD_HDSP,  "*** WARNING: Overflow detected! Computed average stats not valid! ***" ));
   }


#if HDSP_CFG_ICP_USE_FLOATS
   /*
   ** Calculate percent CPU usage.
   */
   minCpuUsagePercent         = ( 100 - ( 100. * gIcpStats.maxCount / gIcpStats.calibrationCount ) );
   maxCpuUsagePercent         = ( 100 - ( 100. * gIcpStats.minCount / gIcpStats.calibrationCount ) );
   weightedAvgCpuUsagePercent = ( 100 - ( 100. * gIcpStats.avgCount / gIcpStats.calibrationCount ) );
   avgCpuUsagePercent         = ( 100 - ( 100. * avgCount / gIcpStats.calibrationCount ) );
#else /* !HDSP_CFG_ICP_USE_FLOATS */
   minCpuUsagePercent         = ( 100 - ( 100 * (gIcpStats.maxCount/100) / (gIcpStats.calibrationCount/100) ) );
   maxCpuUsagePercent         = ( 100 - ( 100 * (gIcpStats.minCount/100) / (gIcpStats.calibrationCount/100) ) );
   weightedAvgCpuUsagePercent = ( 100 - ( 100 * (gIcpStats.avgCount/100) / (gIcpStats.calibrationCount/100) ) );
   avgCpuUsagePercent         = ( 100 - ( 100 * ((gIcpStats.totalCountLow/100)/gIcpStats.iterations) / (gIcpStats.calibrationCount/100) ) );
#endif /* ! HDSP_CFG_ICP_USE_FLOATS */




   /* -----------------------------------------------------------------------
   ** Display stats.
   */
   if ( gVerboseOutput )
   {
#if HDSP_CFG_ICP_USE_FLOATS
      char  floatBuf[64];
#endif

      #define COLUMN_WIDTH    28

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ICP Statictics:" ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u",   COLUMN_WIDTH, "calibrationCount:",     (unsigned int)gIcpStats.calibrationCount ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u",   COLUMN_WIDTH, "iterations:",           (unsigned int)gIcpStats.iterations       ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u",   COLUMN_WIDTH, "currentCount:",         (unsigned int)gIcpStats.currentCount     ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u",   COLUMN_WIDTH, "totalCountLow:",        (unsigned int)gIcpStats.totalCountLow    ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u",   COLUMN_WIDTH, "totalCountHigh:",       (unsigned int)gIcpStats.totalCountHigh   ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u",   COLUMN_WIDTH, "avgCount (weighted):",  (unsigned int)gIcpStats.avgCount         ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u",   COLUMN_WIDTH, "maxCount:",             (unsigned int)gIcpStats.maxCount         ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %u", COLUMN_WIDTH, "minCount:",             (unsigned int)gIcpStats.minCount         ));


#if HDSP_CFG_ICP_USE_FLOATS
      /* VRG_LOG might be re-directed to strPrint for string formatting, and strPrintf
      ** doesn't support floats. Need to use sprintf, which isn't bounded, be careful.. */
      sprintf( floatBuf, "%3.2f", avgCount );
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %s",   COLUMN_WIDTH, "avgCount:",                   floatBuf ));
      sprintf( floatBuf, "%3.2f", minCpuUsagePercent );
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %s",   COLUMN_WIDTH, "minCpuUsagePercent:",         floatBuf ));
      sprintf( floatBuf, "%3.2f", maxCpuUsagePercent );
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %s",   COLUMN_WIDTH, "maxCpuUsagePercent:",         floatBuf ));
      sprintf( floatBuf, "%3.2f", weightedAvgCpuUsagePercent );
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %s",   COLUMN_WIDTH, "weightedAvgCpuUsagePercent:", floatBuf ));
      sprintf( floatBuf, "%3.2f", avgCpuUsagePercent );
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "\t%-*s %s", COLUMN_WIDTH, "avgCpuUsagePercent:",         floatBuf ));
#else /* !HDSP_CFG_ICP_USE_FLOATS */
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "avgCount: %d", avgCount ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "minCpuUsagePercent: %d", minCpuUsagePercent ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "maxCpuUsagePercent: %d", maxCpuUsagePercent));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "weightedAvgCpuUsagePercent: %d", weightedAvgCpuUsagePercent ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "avgCpuUsagePercent: %d", avgCpuUsagePercent ));
#endif /* !HDSP_CFG_ICP_USE_FLOATS */
   }
   else
   {
#if HDSP_CFG_ICP_USE_FLOATS
      char  minFloatBuf[64], maxFloatBuf[64], avgFloatBuf[64];

      /* VRG_LOG might be re-directed to strPrint for string formatting, and strPrintf
      ** doesn't support floats. Need to use sprintf, which isn't bounded, be careful.. */
      sprintf( minFloatBuf, "%3.2f", minCpuUsagePercent );
      sprintf( maxFloatBuf, "%3.2f", maxCpuUsagePercent );
      sprintf( avgFloatBuf, "%3.2f", avgCpuUsagePercent );

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "ICP (min, max, avg): %s %s %s",
                minFloatBuf,
                maxFloatBuf,
                avgFloatBuf ));
#else /* !HDSP_CFG_ICP_USE_FLOATS */
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "ICP (min, max, avg): %d %d %d",
                minCpuUsagePercent,
                maxCpuUsagePercent,
                avgCpuUsagePercent ));
#endif /* !HDSP_CFG_ICP_USE_FLOATS */
   }
}
