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

#include <lhapi.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

void lhapiDprStatsInit( void )
{
}

void lhapiDprStatsGet( STATS_SET *statsp )
{
}

void lhapiDprStatsDiff( STATS_SET *seta, STATS_SET *setb )
{
}

void lhapiDprStatsPause( void )
{
}

void lhapiDprStatsUnpause( void )
{
}

void lhapiDprStatsSetPrescaler( HAPI_UINT16 prescale )
{
}

int  lhapiDprExclusiveExecStart( void )
{
   return( 0 );
}

void lhapiDprExclusiveExecEnd( int dummy )
{
}

