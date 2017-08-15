/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  icp.h - Idle CPU Monitor (ICM) header file
*
*  PURPOSE:
*     This file contains the function prototypes for ICM.
*
*  NOTES:
*
****************************************************************************/

#ifndef ICM_H
#define ICM_H

/* ---- Include Files ---------------------------------------------------- */

#ifndef HAUS_TYPES_H
#include <haus_types.h>                   /* Hausware environment defines */
#endif
#ifndef HAPI_DPR_H
#include <hapi_dpr.h>
#endif
#ifndef ICM_CFG_H
#include <icm_cfg.h>
#endif

typedef struct
{
   STATS_SET stats_start_timer;            /* start timer */
   STATS_SET stats_stop_timer;             /* stop timer */
   UINT16 Icm_framecount;                   
   UINT32 total_idle_cycles;               /* total idle cycles */
   UINT16 lowcomplexity;                   /* low complexity flag */
   UINT32 total_idle_cycles_old;           /* debug use only */
} ICMSTATE; 


/* ---- Function Prototypes ---------------------------------------------- */
void icmStart( void );
void icmStop( void );
void icmUpdate( void );
void icmCheckComplexity( UINT16 framesize );
UINT16 icmGetComplexity(void);
UINT16 icmGetTotalcycles(void);

   
#endif /* ICM_H */
