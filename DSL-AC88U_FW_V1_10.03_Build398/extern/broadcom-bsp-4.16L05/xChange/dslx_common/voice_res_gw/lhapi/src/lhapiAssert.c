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
*    Filename: lhapiAssert.c
*
****************************************************************************
*    Description:
*
*     Implements assert function for hausware lhapi interface.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <vrgLog.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


void lhapiAssertHandler( void )
{
   XCHG_ASSERT( 0 );
}

void lhapiAssertHandlerVerbose( char *filename, int lineNum )
{
   VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "Assert in file %s at line %d", filename, lineNum ));
   XCHG_ASSERT( 0 );
}
