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
*    Filename: lhapiIcp.c
*
****************************************************************************
*    Description:
*
*     Implements hausware idle CPU profiler interface.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <ldx_icp.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   lhapiIcpCalibrate
**
** PURPOSE:    Calibrate idle CPU profiler.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void lhapiIcpCalibrate( void )
{	     
   /*
   ** Do nothing. ICP is currently not supported for the host processor,
   ** only the DSP. However, we still need this function or else the
   ** host image will not link when XCFG_ICP_SUPPORT is enabled.
   */
#ifdef BRCM_NONDIST_BUILD
   icpSetCount( 0 );
#endif
}
