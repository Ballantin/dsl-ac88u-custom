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
*    Filename: hdspIcp.h
*
****************************************************************************
*    Description:
*
*     Interface for hausware idle CPU profiler.
*
****************************************************************************/

#ifndef HDSP_ICP_H
#define HDSP_ICP_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <hapi_icp.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


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
void hdspIcpCalibrate( void );


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
void hdspIcpStart( VRG_BOOL verboseOutput, unsigned int eventIntervalSec );


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
void hdspIcpRestart( void );


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
void hdspIcpStop( VRG_BOOL displayStats );


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
void hdspIcpDisplayStats( ICPSTATS *icpStats );


#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_ICP_H  */
