/*****************************************************************************
*
*  Copyright (c) 2004 Broadcom, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    chipl.h
*
*  @brief   Lower chip device driver header file
*
*  This file contains the lower device driver header information for the
*  chip module.
*/
/****************************************************************************/

#ifndef CHIPL_H
#define CHIPL_H


/* ---- Include Files ----------------------------------------------------- */

/* ---- Public Constants and Types ---------------------------------------- */

typedef enum
{
   CHIPL_DCACHE_UNCACHED = 0,       /* Uncached mode */
   CHIPL_DCACHE_WRITETHROUGH,       /* Writethrough mode */
   CHIPL_DCACHE_WRITEBACK           /* Writeback mode */
} CHIPL_DCACHE;


typedef enum
{
   CHIPL_BUS_SYNC = 0,              /* Synchronous mode */
   CHIPL_BUS_ASYNC                  /* Asynchronous mode */
} CHIPL_BUS;


typedef struct
{
   int (*chiplBlkDisablep)( int blk );
   int (*chiplBlkEnablep)( int blk );
   int (*chiplBlkIsEnabledp)( int blk );
   int (*chiplBlkResetp)( int blk );
   unsigned int (*chiplBusFreqGetp)( void );
   unsigned int (*chiplBusFreqSetp)( unsigned int freq );
   unsigned int (*chiplPeriphFreqGetp)( void );
   unsigned int (*chiplPeriphFreqSetp)( unsigned int freq );
   unsigned int (*chiplCoCpuFreqGetp)( int core );
   unsigned int (*chiplCoCpuFreqSetp)( int core, unsigned int freq );
   unsigned int (*chiplCpuFreqGetp)( int core );
   unsigned int (*chiplCpuFreqSetp)( int core, unsigned int freq );
   unsigned int (*chiplCpuFreqCfgGetp)( int core );
   void (*chiplResetp)( void );
   void (*chiplSoftResetp)( void );
   unsigned int (*chiplXtalFreqGetp)( void );
   int (*chiplBlkLowerToUpperp)( int blk );
   int (*chiplBlkUpperToLowerp)( int blk );
   unsigned int (*chipIdGetp)( void );
   unsigned int (*chipRevGetp)( void );
   int (*chiplCacheDataIsEnabledp)( int core );
   int (*chiplCacheDataPfcIsEnabledp)( int core );
   int (*chiplCacheDataRacIsEnabledp)( int core );
   int (*chiplCacheInstIsEnabledp)( int core );
   int (*chiplCacheInstPfcIsEnabledp)( int core );
   int (*chiplCacheInstRacIsEnabledp)( int core );
   CHIPL_DCACHE (*chiplCacheDataModeGetp)( int core );
   CHIPL_BUS (*chiplBusModeGetp)( int core );
} CHIPL_FUNCS;

#define chiplBlkDisable( drvp, blk )            (drvp)->funcp->chiplBlkDisablep( blk )
#define chiplBlkEnable( drvp, blk )             (drvp)->funcp->chiplBlkEnablep( blk )
#define chiplBlkIsEnabled( drvp, blk )          (drvp)->funcp->chiplBlkIsEnabledp( blk )
#define chiplBlkReset( drvp, blk )              (drvp)->funcp->chiplBlkResetp( blk )
#define chiplBusFreqGet( drvp )                 (drvp)->funcp->chiplBusFreqGetp()
#define chiplBusFreqSet( drvp, freq )           (drvp)->funcp->chiplBusFreqSetp( freq )
#define chiplPeriphFreqGet( drvp )              (drvp)->funcp->chiplPeriphFreqGetp()
#define chiplPeriphFreqSet( drvp, freq )        (drvp)->funcp->chiplPeriphFreqSetp( freq )
#define chiplCoCpuFreqGet( drvp, core )         (drvp)->funcp->chiplCoCpuFreqGetp( core )
#define chiplCoCpuFreqSet( drvp, core, freq )   (drvp)->funcp->chiplCoCpuFreqSetp( core, freq )
#define chiplCpuFreqGet( drvp, core )           (drvp)->funcp->chiplCpuFreqGetp( core )
#define chiplCpuFreqSet( drvp, core, freq )     (drvp)->funcp->chiplCpuFreqSetp( core, freq )
#define chiplCpuFreqCfgGet( drvp, core )        (drvp)->funcp->chiplCpuFreqCfgGetp( core )
#define chiplReset( drvp )                      (drvp)->funcp->chiplResetp()
#define chiplSoftReset( drvp )                  (drvp)->funcp->chiplSoftResetp()
#define chiplXtalFreqGet( drvp )                (drvp)->funcp->chiplXtalFreqGetp()
#define chiplBlkLowerToUpper( drvp, blk )       (drvp)->funcp->chiplBlkLowerToUpperp( blk )
#define chiplBlkUpperToLower( drvp, blk )       (drvp)->funcp->chiplBlkUpperToLowerp( blk )
#define chipIdGet( drvp )                       (drvp)->funcp->chipIdGetp( )
#define chipRevGet( drvp )                      (drvp)->funcp->chipRevGetp( )
#define chiplCacheDataIsEnabled( drvp, core )      (drvp)->funcp->chiplCacheDataIsEnabledp( core )
#define chiplCacheDataPfcIsEnabled( drvp, core )   (drvp)->funcp->chiplCacheDataPfcIsEnabledp( core )
#define chiplCacheDataRacIsEnabled( drvp, core )   (drvp)->funcp->chiplCacheDataRacIsEnabledp( core )
#define chiplCacheInstIsEnabled( drvp, core )      (drvp)->funcp->chiplCacheInstIsEnabledp( core )
#define chiplCacheInstPfcIsEnabled( drvp, core )   (drvp)->funcp->chiplCacheInstPfcIsEnabledp( core )
#define chiplCacheInstRacIsEnabled( drvp, core )   (drvp)->funcp->chiplCacheInstRacIsEnabledp( core )
#define chiplCacheDataModeGet( drvp, core )        (drvp)->funcp->chiplCacheDataModeGetp( core )
#define chiplBusModeGet( drvp, core )              (drvp)->funcp->chiplBusModeGetp( core )


typedef struct
{
   /* This must be the first structure member. */
   const CHIPL_FUNCS *funcp;

} CHIPL_DRV;


typedef enum
{
   CHIPL_CACHE_INST = 0,            /* Instruction cache */
   CHIPL_CACHE_INST_PFC,            /* Instruction prefetch cache */
   CHIPL_CACHE_INST_RAC,            /* Instruction read-ahead cache */
   CHIPL_CACHE_DATA,                /* Data cache */
   CHIPL_CACHE_DATA_PFC,            /* Data prefetch cache */
   CHIPL_CACHE_DATA_RAC,            /* Data read-ahead cache */
} CHIPL_CACHE;


#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Public Variables -------------------------------------------------- */


/* ---- Public Function Prototypes ---------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif
