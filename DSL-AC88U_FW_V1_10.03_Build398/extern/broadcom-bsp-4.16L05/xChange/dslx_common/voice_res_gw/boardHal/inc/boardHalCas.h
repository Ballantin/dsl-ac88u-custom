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
*    Filename: halCas.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      Channel Associated Signaling (CAS) drivers present on a board.
*
****************************************************************************/

#ifndef BOARD_HAL_CAS_H
#define BOARD_HAL_CAS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvCas.h>
#include <vrgTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

#define BOARD_HAL_VCM_HW_DEBOUNCE_MS   0     /* no h/w debouncing */
#define BOARD_HAL_VCM_SW_DEBOUNCE_MS   15    /* use only s/w debouncing */


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalCasInit
**
** PURPOSE:    Initialize the CAS drivers.
**
** PARAMETERS: fastSlicSupported (in) True if fast SLIC mode changing should
**                                    be supported on the platform.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalCasInit( VRG_BOOL fastSlicSupported, VRG_BOOL bCasHistorySupported );


/*
*****************************************************************************
** FUNCTION:   boardHalCasDeinit
**
** PURPOSE:    De-initialize the CAS drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalCasDeinit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalCasGetDriver
**
** PURPOSE:    Retrieve the CAS drivers associated with a specified endpt.
**
** PARAMETERS: chan  (in)  Endpt number (0 referenced) whose driver is to
**                         be retrieved.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
XDRV_CAS* boardHalCasGetDriver( int chan );


#ifdef __cplusplus
    }
#endif

#endif  /* BOARD_HAL_CAS_H  */
