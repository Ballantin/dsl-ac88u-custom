/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <stdlib.h>  /* Needed for rand() */

#include <bosTime.h>

#include <hapi.h>
#include <lhapi.h>

/* These are needed for the randomization functions  */
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/types.h>

#include <asm/uaccess.h>


/* ---- Private Constants ------------------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */
static HAPI_UINT32 net_random(void);

/* ---- Functions -------------------------------------------------------- */

static int firsttime = 1;
static HAPI_UINT32 lasttime_sec;
static HAPI_UINT32 lasttime_usec;
static BOS_TIME_MS prevTime;

static HAPI_UINT32 net_rand_seed = 152L;

static HAPI_UINT32 net_random(void)
{
	net_rand_seed=net_rand_seed*69069L+1;
        return net_rand_seed^jiffies;
}

/****************************************************************************
*
****************************************************************************/
int lhapiRtpTimeGet( HAPI_UINT32 *sec, HAPI_UINT32 *uSec )
{
   BOS_TIME_MS curTime;
   BOS_TIME_MS deltaBosTimeMs;

   HAPI_UINT32 deltaTimeMs;
 
   if (firsttime)
   {
      bosTimeGetMs(&prevTime);
      firsttime = 0;
      lasttime_sec = (HAPI_UINT32)net_random(); 
      lasttime_usec = (HAPI_UINT32)net_random();
   }
 
   bosTimeGetMs(&curTime);
   bosTimeCalcDeltaMs(&prevTime, &curTime, &deltaBosTimeMs);

   /*
   ** Need a "better" (correct) way to convert between BOS_TIME and an integer
   */
   deltaTimeMs =  (HAPI_UINT32)deltaBosTimeMs;

   *sec  = (lasttime_sec  + (deltaTimeMs / 1000));
   *uSec = (lasttime_usec + (deltaTimeMs % 1000) * 1000) % 1000000;
 
   /* Handle overflow */
   if (lasttime_usec >= *uSec)
   {
      /*
      ** If deltaTimeMs is exactly 1sec or 0 then we will get to here
      */
      if( deltaTimeMs != 0 )
      {
         (*sec)++;
      }
   }

   /* Remember last entry */                                                          
   prevTime = curTime;

   lasttime_sec  = *sec;
   lasttime_usec = *uSec;

   return( 0 );
}

/*
*****************************************************************************
** FUNCTION:   lhapiRtpTimeGet
**
** PURPOSE:    Calculates the elapsed time in seconds and microseconds since
**             the last invocation of this function.
**
** PARAMETERS: sec  - Pointer to the elapsed time in seconds.
**             uSec - Poitner to the elapsed time in microseconds.
**
** RETURNS:    none
**
*****************************************************************************
*/
void lhapiRtpTimeSet( HAPI_UINT32 sec, HAPI_UINT32 uSec )
{
   lasttime_sec = sec;
   lasttime_usec = uSec;
} /* lhapiRtpTimeSet */

/****************************************************************************
*
****************************************************************************/
HAPI_UINT16 lhapiRtpRand16(void)
{
   return( (HAPI_UINT16)net_random() );
}

