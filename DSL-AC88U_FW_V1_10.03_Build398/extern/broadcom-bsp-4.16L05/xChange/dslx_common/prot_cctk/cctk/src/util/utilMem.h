/**********************************************************************************
** Copyright © 2008 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file defines the Memory Management main.
**
***********************************************************************************/
#ifndef __UTIL_MEM__H__INCLUDED__
#define __UTIL_MEM__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Class Definition -------------------------------------------------------- */
#if CCTKCFG_MEMORY_MONITORING
#if CCTKCFG_DEBUG

/* This class defines the Memory Management main */
class UtilMem : public MX_NS CMemoryAllocator::IMemoryBlockAccumulator
{
/* ---- Constants and Types ----------------------------------------------------- */
public:
   /* Memory allocation table tracing policy */
   typedef enum UTILMEM_TRACEMODE
   {
      eUTILMEM_TRACEMODE_NONE,         /* Do not trace memory allocation table */
      eUTILMEM_TRACEMODE_CCTK,         /* Trace memory blocks allocated by
                                       ** CCTK only.
                                       */
      eUTILMEM_TRACEMODE_ALL           /* Trace the entire memory allocation
                                       ** table.
                                       */
   } UTILMEM_TRACEMODE;

   /* Memory statistics */
   typedef struct UTILMEMSTAT
   {
      BOS_UINT32 uMemAllocCctk;        /* Total amount of memory allocated
                                       ** by CCTK
                                       */
      BOS_UINT32 uMemAllocAll;         /* Total amount of memory allocated */
      BOS_UINT32 uNumAllocCctk;        /* Total number of memory blocks allocated
                                       ** by CCTK
                                       */
      BOS_UINT32 uNumAllocAll;         /* Total number of memory blocks allocated */
   } UTILMEMSTAT;

/* ---- Function Prototypes ----------------------------------------------------- */
public:
   /* Constructors / Destructors */
   UtilMem();
   ~UtilMem();

   /* Memory Management API functions */
   void getMemStatistics( UTILMEM_TRACEMODE eTraceMode, UTILMEMSTAT &rMemStat );

   /* CMemoryAllocator::IMemoryBlockAccumulator */
   void Accumulate( MX_NS CMemoryAllocator::CMemoryBlock *pMemoryBlock );

/* ---- Variables --------------------------------------------------------------- */
private:
   /* Cached memory allocation table tracing policy */
   UTILMEM_TRACEMODE meMemTableTrace;

   /* Cached memory allocation statistics */
   UTILMEMSTAT mMemStatistics;
};
#endif /* CCTKCFG_DEBUG */

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/* Memory Management module (de)initialization routines */
UTILSTATUS  utilMemInit( void );
UTILSTATUS  utilMemDeInit( void );

#ifdef __cplusplus
}
#endif

/* ---- Inline Functions -------------------------------------------------------- */

#endif /* CCTKCFG_MEMORY_MONITORING */

#endif /* __UTIL_MEM__H__INCLUDED__ */
