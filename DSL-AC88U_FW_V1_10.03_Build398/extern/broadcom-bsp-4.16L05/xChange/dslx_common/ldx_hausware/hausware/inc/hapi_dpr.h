/****************************************************************************
*     Copyright (c) 2001 Broadcom
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
*****************************************************************************
*  hapi_dpr.h - development profiler definitions
*
*  PURPOSE:
*
*     This file contains global definitions for using the development
*     profiler within the DSP application.
****************************************************************************/
#ifndef HAPI_DPR_H
#define HAPI_DPR_H

/* ---- Include Files ---------------------------------------------------- */

#include <hapi_types.h>                /* HausWare environment definitions    */
#include <hapi_opcode.h>               /* Opcode information                  */

 
/* ---- Constants and Types ---------------------------------------------- */

typedef struct STATS_SET
{
   HAPI_UINT32 cycles;        /* Number of cycles       */

} STATS_SET;

/*
 * Development profiler commands
 */
#define HAPI_DPR_ENABLE_CMD      (HSF_MKHDR(HAPI_DPR_SRV, HAPI_OPC_ENABLE_CMD))
#define HAPI_DPR_SETREGS_CMD     (HSF_MKHDR(HAPI_DPR_SRV, HAPI_OPC_SETREGS_CMD))
#define HAPI_DPR_SETREG1_CMD     (HSF_MKHDR(HAPI_DPR_SRV, HAPI_OPC_SETREG1_CMD))
#define HAPI_DPR_RESET_STATS_CMD (HSF_MKHDR(HAPI_DPR_SRV, HAPI_OPC_RESET_STATS_CMD))
#define HAPI_DPR_GETREGS_CMD     (HSF_MKCMD_EXT(HAPI_DPR_SRV, 0))
#define HAPI_DPR_GETSTATS_CMD    (HSF_MKCMD_EXT(HAPI_DPR_SRV, 1))

/*
 * Development profiler responses
 */
#define HAPI_DPR_ENABLE_RSP      (HSF_MKRSP_STD(HAPI_DPR_ENABLE_CMD))
#define HAPI_DPR_SETREGS_RSP     (HSF_MKRSP_STD(HAPI_DPR_SETREGS_CMD))
#define HAPI_DPR_SETREG1_RSP     (HSF_MKRSP_STD(HAPI_DPR_SETREG1_CMD))
#define HAPI_DPR_RESET_STATS_RSP (HSF_MKRSP_STD(HAPI_DPR_RESET_STATS_CMD))
#define HAPI_DPR_GETSTATS_RSP    (HSF_MKRSP_EXT(HAPI_DPR_GETSTATS_CMD))
#define HAPI_DPR_GETREGS_RSP     (HSF_MKRSP_EXT(HAPI_DPR_GETREGS_CMD))

/*
 * Development profiler asynchronous events
 */
#define HAPI_DPR_STATS_EVT       (HSF_MKEVT_EXT(HAPI_DPR_SRV, 0))
   /* op1: Length of stats array structure
      op2: Pointer to stats array structure */

/*
 * Definitions of development blocks
 */

/* hlapmisr.c */
#define   HAPI_DPR_BLOCK_0          0   /* LHAPI Ingress Processing */
#define   HAPI_DPR_BLOCK_2          2   /* LHAPI Egress Processing */
#define   HAPI_DPR_BLOCK_3          3   /* KNL_FramSync */

#define   DPR_BLOCK_VHD_THREAD      33  /* VHD thread    */
#define   DPR_BLOCK_HAL_SHIM        34  /* HAL Shim      */
#define   DPR_BLOCK_VHD_OBD         35  /* OBD processing */


typedef enum
{
   DPR_DIRECTION_EGRESS = 0,
   DPR_DIRECTION_INGRESS,
   DPR_DIRECTION_BLOCK

} DPR_DIRECTION;

/*
 * Development profiler configuration registers
 */
typedef struct
{
   HAPI_UINT16 handle;      /* Handle Specific or Block number */
   HAPI_UINT16 direction;   /* Direction                       */
   HAPI_UINT16 srvId;       /* Service ID                      */

} DPRREGS;

/*
 * Development profiler statistics (16-bit version)
 */
typedef struct
{
   HAPI_UINT16  iterationsLow;  /* Number of data points taken     */
   HAPI_UINT16  iterationsHigh; /* Number of data points taken     */
   HAPI_UINT16  currentLow;     /* Current value                   */
   HAPI_UINT16  currentHigh;    /* Current value                   */
   HAPI_UINT16  totalLowLow;    /* Total - Low 32 bits             */
   HAPI_UINT16  totalLowHigh;   /* Total - Low 32 bits             */
   HAPI_UINT16  totalHighLow;   /* Total - High 32 bits            */
   HAPI_UINT16  totalHighHigh;  /* Total - High 32 bits            */
   HAPI_UINT16  avgLow;         /* Running average                 */
   HAPI_UINT16  avgHigh;        /* Running average                 */
   HAPI_UINT16  maxLow;         /* Maximum value                   */
   HAPI_UINT16  maxHigh;        /* Maximum value                   */
   HAPI_UINT16  minLow;         /* Minimum value                   */
   HAPI_UINT16  minHigh;        /* Minimum value                   */

} DPRSTATS;


#endif /* HAPI_DPR_H */
