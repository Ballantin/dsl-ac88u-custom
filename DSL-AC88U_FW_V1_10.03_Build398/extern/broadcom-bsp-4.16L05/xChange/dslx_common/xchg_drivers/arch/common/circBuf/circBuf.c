/****************************************************************************
*
* Copyright © 2000-2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: circBuf.h
*
****************************************************************************
*    Description:
*
*     Implenents shared memory debug communications interface that allows
*     the host to read and write to the DSP's memory space.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <string.h>
#include <xdrvSpinLock.h>
#include <xdrvCircBuf.h>
#include <xdrvLog.h>

#include <bosSpinlock.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
#define CIRCBUF_SPINLOCK_SUPPORT       1
#define CIRCBUF_DEBUG                  0

typedef enum CBUF_OPER
{
   CBUF_READ,
   CBUF_WRITE     
} CBUF_OPER;

typedef struct
{
  XDRV_BOOL bLock;                              /* whether to use spinlocks or not */   
  XDRV_BOOL bFull;                              /* flag indicating buffer is full */
  XDRV_BOOL bEmpty;                             /* flag indicating buffer is empty */
#if CIRCBUF_SPINLOCK_SUPPORT             
  BOS_SPINLOCK spinLock;                        /* spinlock  */
#endif /* CIRCBUF_SPINLOCK_SUPPORT */   
  XDRV_UINT32 maxSize;                          /* Maximum size of circular buffer */
  XDRV_UINT32 readPtr;                          /* current read index of buffer */
  XDRV_UINT32 writePtr;                         /* current write index of buffer */
  char * bufp;                                  /* pointer to actual buffer */
  XDRV_CIRCBUF_ALLOCMEM_FP allocMem;            /* Function pointer for mem alloc function */
  XDRV_CIRCBUF_FREEMEM_FP freeMem;              /* Function pointer for mem free  function */   
} CIRCBUF_INFOBLK;

typedef struct
{
   XDRV_UINT32 operPtr;            /* Starting index for buffer operation */
   XDRV_UINT32 operLimitPtr;       /* Pointer to absolute last position for buffer operation */
   XDRV_UINT32 regOperBytes;       /* Number of bytes involved in non-wrap buffer operation */
   XDRV_UINT32 wrapOperPtr;        /* Starting index for wrap buffer operation */
   XDRV_UINT32 wrapOperBytes;      /* Number of bytes involved in wrap buffer operation */
   XDRV_UINT32 maxLength;          /* Max lenth of circ buffer */
   XDRV_UINT32 reqOperBytes;       /* Total number of bytes involved in buffer operation  */
} BUFOPER_INFOBLK;

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
static void setupBufOperInfoBlk( BUFOPER_INFOBLK * bufOper_infoBlk , CBUF_OPER oper);
static XDRV_SINT32 circBufCopy( CBUF_OPER oper, char** srcBufpp, char** destBufpp, XDRV_UINT32 ptr, XDRV_UINT32 numBytes );
static XDRV_SINT32 circBufOperation( XDRV_CIRCBUF_HDL * pCircBufHdl, CBUF_OPER oper, char   *pBuf, XDRV_UINT32 length );

/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   circBufOperation
**
** PURPOSE:    perform an operation on the  circular buffer.
**
** PARAMETERS: pCircBufHdl (mod) circBuf object to read from.
**             oper        (in)  Operation to be performed ( CBUF_READ/CBUF_WRITE )
**             pBuf        (out) Data read from the circbuff is placed in this buffer.
**             length      (in)  Number of bytes to read.
**
** RETURNS:    Length of message read in bytes.
**
** NOTE:
*****************************************************************************
*/
static XDRV_SINT32 circBufOperation
(
   XDRV_CIRCBUF_HDL * pCircBufHdl,
   CBUF_OPER oper,
   char   *pBuf,
   XDRV_UINT32   length
)
{
   CIRCBUF_INFOBLK * pCircBufInfoBlk = (CIRCBUF_INFOBLK *)*pCircBufHdl;   
   BUFOPER_INFOBLK bufOper_infoBlk;
   XDRV_UINT32 numOperBytes = 0;  
   XDRV_UINT32 * operPtr      =  NULL;
   XDRV_UINT32 * operLimitPtr =  NULL;
   char * srcBufp =  NULL;
   char * destBufp = NULL;                    
      
   if( pCircBufInfoBlk)
   {              
      /* Fill out operation specific parameters */
      operPtr      =  ( oper == CBUF_READ ? &pCircBufInfoBlk->readPtr  : &pCircBufInfoBlk->writePtr );
      operLimitPtr =  ( oper == CBUF_READ ? &pCircBufInfoBlk->writePtr : &pCircBufInfoBlk->readPtr  );
      srcBufp      =  ( oper == CBUF_READ ? pCircBufInfoBlk->bufp      : pBuf                       );
      destBufp     =  ( oper == CBUF_READ ? pBuf                       : pCircBufInfoBlk->bufp      );

      /* Check if we can make an early exit */
      if( !length || (( oper == CBUF_READ ) && pCircBufInfoBlk->bEmpty) || (( oper == CBUF_WRITE ) && pCircBufInfoBlk->bFull)  )
      {
         /* Cant do anything, just return */            
         return 0;
      }                                                                                                          
                                                                                                                 
      /* Populate the operInfo block */                                                                                
      bufOper_infoBlk.operPtr            = *operPtr;
      bufOper_infoBlk.operLimitPtr       = *operLimitPtr;
      bufOper_infoBlk.maxLength          = pCircBufInfoBlk->maxSize; 
      bufOper_infoBlk.reqOperBytes       = length;   
      setupBufOperInfoBlk( &bufOper_infoBlk, oper );                                                                                 
                                                                                                                               
      /* Check if the operation wraps around the buffer */                                                                          
      if( bufOper_infoBlk.wrapOperBytes )                                                                                      
      {                                      
         numOperBytes += circBufCopy( oper, &srcBufp, &destBufp, bufOper_infoBlk.wrapOperPtr, bufOper_infoBlk.wrapOperBytes );
         
         /* Update operation pointer  for the subsequent operation from the beginning of the buffer */
         *operPtr = bufOper_infoBlk.operPtr;
      }
      
      /* Do our regular operation */
      if( bufOper_infoBlk.regOperBytes )
      {
         numOperBytes += circBufCopy( oper, &srcBufp, &destBufp, bufOper_infoBlk.operPtr, bufOper_infoBlk.regOperBytes );
         
         /* Update operPtr */
         *operPtr += bufOper_infoBlk.regOperBytes;
         *operPtr = *operPtr % pCircBufInfoBlk->maxSize;
      }
      
      /* Check if buffer is empty/full */
      if( numOperBytes )
      {
         /* If after a read/write, both pointer are the same -> buffer Empty/Full condition resp. */
         pCircBufInfoBlk->bEmpty = ( (oper == CBUF_READ)  && ( *operPtr == *operLimitPtr ) ? XDRV_TRUE : XDRV_FALSE );
         pCircBufInfoBlk->bFull  = ( (oper == CBUF_WRITE) && ( *operPtr == *operLimitPtr ) ? XDRV_TRUE : XDRV_FALSE );         
      }
                 
      return numOperBytes;
   }
   else
   {
         XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "%s: Invalid bufferhdl pointer %p \n", __FUNCTION__, pCircBufInfoBlk));
         return -1;               
   }   
}

/*
*****************************************************************************
** FUNCTION:   circBufCopy
**
** PURPOSE:    perform an operation on the  circular buffer.
**
** PARAMETERS: oper        (in)  Operation to be performed ( CBUF_READ/CBUF_WRITE )
**             srcBufpp    (in)  Pointer to src buffer pointer
**             destBufpp   (in)  Pointer to dest buffer pointer
**             ptr         (in)  Read/write index
**             numBytes    (in)  Num butest to read/write
**
** RETURNS:    num bytes written/read.
**
** NOTE:
*****************************************************************************
*/
static XDRV_SINT32 circBufCopy( CBUF_OPER oper, char** srcBufpp, char** destBufpp, XDRV_UINT32 ptr, XDRV_UINT32 numBytes )
{
#if CIRCBUF_DEBUG
   if( oper == CBUF_READ)
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "%s: ptr %d, src %p, dst %p, numBytes %d\n", (oper==CBUF_READ?"R":"W"), ptr, *srcBufpp, *destBufpp, numBytes));
   }
#endif /* CIRCBUF_DEBUG */
   
   char * srcBuffer = *srcBufpp;
   char * dstBuffer = *destBufpp;
   
   if( oper == CBUF_READ )
   {         
      /* READ */
      memcpy( dstBuffer, &(srcBuffer[ptr]), numBytes );
               
      /* Update destination pointer */
      *destBufpp += numBytes;               
   }
   else
   {
      /* WRITE */
      memcpy( &(dstBuffer[ptr]), srcBuffer, numBytes );
               
      /* Update source pointer */
      *srcBufpp += numBytes;                           
   }    
   
   return numBytes;  
}

/*
*****************************************************************************
** FUNCTION:   setupBufOperInfoBlk
**
** PURPOSE:    Setup the buffer operation information block.
**
** PARAMETERS: bufOper_infoBlk (mod) pointer to operation info block
**             oper            (in)  Operation to be performed ( CBUF_READ/CBUF_WRITE )
**
** RETURNS:    Nothing
**
** NOTE: This function recalculates read/write indices based on wheter a buffer wrap is taking place
*****************************************************************************
*/
static void setupBufOperInfoBlk( BUFOPER_INFOBLK * bufOper_infoBlk, CBUF_OPER oper )
{
   bufOper_infoBlk->wrapOperPtr   = 0;                                                                             
   bufOper_infoBlk->wrapOperBytes = 0;                                                                             
   bufOper_infoBlk->regOperBytes = 0;                                                                                 
                                                                                                                   
#if CIRCBUF_DEBUG
   if( oper == CBUF_READ )
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "TOPR: wPtr %d, rPtr %d, mLen %d, req %d\n", 
              bufOper_infoBlk->operLimitPtr, bufOper_infoBlk->operPtr, bufOper_infoBlk->maxLength, bufOper_infoBlk->reqOperBytes));  
   }
   else
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "TOPW: wPtr %d, rPtr %d, mLen %d, req %d\n",                                                               
              bufOper_infoBlk->operPtr, bufOper_infoBlk->operLimitPtr, bufOper_infoBlk->maxLength, bufOper_infoBlk->reqOperBytes));  
   }                                                                                    
#endif /* CIRCBUF_DEBUG */
                                                                                                                                                       
   /* Handle writePtr wrapping */
   if( bufOper_infoBlk->operLimitPtr <= bufOper_infoBlk->operPtr )
   {
      /* Check if requested read wraps around the buffer */
      if( bufOper_infoBlk->operPtr + bufOper_infoBlk->reqOperBytes > bufOper_infoBlk->maxLength )
      {
         /* Our requested read wraps around the buffer calculat wrapread info*/
         bufOper_infoBlk->wrapOperPtr = bufOper_infoBlk->operPtr;
         bufOper_infoBlk->wrapOperBytes = (bufOper_infoBlk->maxLength ) - bufOper_infoBlk->operPtr;
         
         /* Reset the read ptr and requested read lenght */
         bufOper_infoBlk->operPtr = 0;
         
         /* Check if writepointer is at 0 index */ 
         if( bufOper_infoBlk->operLimitPtr == 0 )
         {
            /* this means that after our wrap read, our buff will be empty, no further reads required */
            bufOper_infoBlk->reqOperBytes = 0;
         }
         else
         {
            bufOper_infoBlk->reqOperBytes = bufOper_infoBlk->reqOperBytes - bufOper_infoBlk->wrapOperBytes;
         }
      }                    
   }
   
   /* Set our num Read bytes */
   bufOper_infoBlk->regOperBytes = bufOper_infoBlk->reqOperBytes;
      
   /* Ensure that our read doesnt go beyond the writeptr */
   if( bufOper_infoBlk->operLimitPtr > bufOper_infoBlk->operPtr )
   {
      if( bufOper_infoBlk->operPtr + bufOper_infoBlk->reqOperBytes > bufOper_infoBlk->operLimitPtr )
      {
         /* Restrict reading upto and including writeptr */
         bufOper_infoBlk->regOperBytes = bufOper_infoBlk->operLimitPtr - bufOper_infoBlk->operPtr;
      }         
   }

#if CIRCBUF_DEBUG
   if( oper == CBUF_READ )
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "R: wPtr %d, rPtr %d, rBytes %d, WrPtr %d, WrBytes %d, mLen %d, req %d\n",   
                  bufOper_infoBlk->operLimitPtr, bufOper_infoBlk->operPtr, bufOper_infoBlk->regOperBytes,
                  bufOper_infoBlk->wrapOperPtr, bufOper_infoBlk->wrapOperBytes, bufOper_infoBlk->maxLength,
                  bufOper_infoBlk->reqOperBytes));                                                              
   }
   else
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "W: wPtr %d, rPtr %d, wBytes %d, WwPtr %d, WwBytes %d, mLen %d, req %d\n",   
                  bufOper_infoBlk->operPtr, bufOper_infoBlk->operLimitPtr, bufOper_infoBlk->regOperBytes,
                  bufOper_infoBlk->wrapOperPtr, bufOper_infoBlk->wrapOperBytes,bufOper_infoBlk->maxLength,                                                                       
                  bufOper_infoBlk->reqOperBytes));                                                              
   }
#endif /* CIRCBUF_DEBUG */
}                         


/*
*****************************************************************************
** FUNCTION:   circBufCreate
**
** PURPOSE:    To create a debug communications drivers.
**
** PARAMETERS: pCircBufHdl  (out)  Pointer to store address of buffer handle.
**             size    (in) size in bytes of the buffer
**             lock    (in) Indicates whether locked or unlocked version of the API is used
**             allocmem(in) Ptr to mem alloc function taking int argument returning char *
**             freemem (in) Ptr to mem free function taking char * and returning void
** RETURNS:    0 on success, else error.
**
** NOTE:
*****************************************************************************
*/
XDRV_SINT32 circBufCreate
(
   XDRV_CIRCBUF_HDL * pCircBufHdl,
   XDRV_UINT32 size,
   XDRV_BOOL lock,
   XDRV_CIRCBUF_ALLOCMEM_FP allocMem,           
   XDRV_CIRCBUF_FREEMEM_FP freeMem
)
{
   CIRCBUF_INFOBLK * pCircBufInfoBlk = NULL;
   
   if( pCircBufHdl && size && allocMem && freeMem)
   {            
      /* malloc bufferInfo Block */
      pCircBufInfoBlk = (CIRCBUF_INFOBLK *)allocMem( sizeof(CIRCBUF_INFOBLK) );
      
      memset( (char*)pCircBufInfoBlk, 0, (sizeof(CIRCBUF_INFOBLK) ) );
      
      if( !pCircBufInfoBlk )
      {
         XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "%s: Failed to allocate %d bytes of memory for bufHdl %p\n", __FUNCTION__, sizeof(CIRCBUF_INFOBLK), pCircBufInfoBlk ));
         return -1;         
      }
      
#if CIRCBUF_SPINLOCK_SUPPORT
      /* Create Spinlock */
      if( lock )
      {
         pCircBufInfoBlk->bLock = XDRV_TRUE;
         bosSpinlockCreate( &(pCircBufInfoBlk->spinLock) );
      }
#endif /* CIRCBUF_SPINLOCK_SUPPORT */
            
      /* malloc actual buffer */
      pCircBufInfoBlk->bufp = (char *)allocMem( size );
      
      if( !pCircBufInfoBlk->bufp )
      {
         XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "%s: Failed to allocate %lu bytes of memory for buffer\n", __FUNCTION__, size ));
         return -1;         
      }
      else
      {
         /* Store maximum buffer size and set flags*/
         pCircBufInfoBlk->maxSize = size;
         pCircBufInfoBlk->bEmpty  = XDRV_TRUE;
         pCircBufInfoBlk->bFull   = XDRV_FALSE;
      }
      
      /* Store alloc/free function pointers */
      pCircBufInfoBlk->allocMem = allocMem;
      pCircBufInfoBlk->freeMem  = freeMem;
      
      /* Assign to handle and return */
      *pCircBufHdl = (XDRV_CIRCBUF_HDL)pCircBufInfoBlk;
      
      return 0;
   }
   else
   {
         XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "%s: Invalid bufferhdl pointer %p or size %lu bytes of memory for buffer\n", __FUNCTION__, pCircBufHdl, size ));
         return -1;               
   }
}

/*
*****************************************************************************
** FUNCTION:   circBufDestroy
**
** PURPOSE:    To de-init a circBuf object.
**
** PARAMETERS: pCircBufHdl  (mod) circBuf object to de-allocate.
**
** RETURNS:    0 on success, else error.
**
** NOTE:
*****************************************************************************
*/
XDRV_SINT32 circBufDestroy
(
   XDRV_CIRCBUF_HDL * pCircBufHdl
)
{
   CIRCBUF_INFOBLK * pCircBufInfoBlk = (CIRCBUF_INFOBLK *)*pCircBufHdl;

   if( pCircBufInfoBlk)
   {  
      
#if CIRCBUF_SPINLOCK_SUPPORT
      unsigned long flags;
       
      /* Aquire Lock required */
      if( pCircBufInfoBlk->bLock )
      {
         bosSpinlockAcquire( &(pCircBufInfoBlk->spinLock), &flags );
      }
#endif /* CIRCBUF_SPINLOCK_SUPPORT */

      /* free actual data buffer */
      pCircBufInfoBlk->freeMem( pCircBufInfoBlk->bufp );                
      pCircBufInfoBlk->bufp = NULL;

#if CIRCBUF_SPINLOCK_SUPPORT
      if( pCircBufInfoBlk->bLock )
      {
         bosSpinlockRelease( &(pCircBufInfoBlk->spinLock), flags );
      }
      
      /* Destroy Spinlock */
      if( pCircBufInfoBlk->bLock )
      {
         bosSpinlockDestroy( &(pCircBufInfoBlk->spinLock) ); 
         pCircBufInfoBlk->bLock = XDRV_FALSE;        
      }
#endif /* CIRCBUF_SPINLOCK_SUPPORT */
            
      /* Free bufferInfo Block */
      pCircBufInfoBlk->freeMem( (char*)pCircBufInfoBlk );
      pCircBufInfoBlk = NULL;
      *pCircBufHdl = NULL;
      
      return 0;
   }
   else
   {
         XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "%s: Invalid bufferhdl pointer %p \n", __FUNCTION__, *pCircBufHdl));
         return -1;               
   }   
}

/*
*****************************************************************************
** FUNCTION:   circBufRead
**
** PURPOSE:    To read data from the the circular buffer.
**
** PARAMETERS: pCircBufHdl  (mod) circBuf object to read from.
**             pBuf        (out) Data read from the circbuff is placed in this buffer.
**             length      (in)  Number of bytes to read.
**
** RETURNS:    Length of message read in bytes.
**
** NOTE:
*****************************************************************************
*/
XDRV_SINT32 circBufRead
(
   XDRV_CIRCBUF_HDL * pCircBufHdl,
   char   *pBuf,
   XDRV_UINT32   length
)
{
   CIRCBUF_INFOBLK * pCircBufInfoBlk = (CIRCBUF_INFOBLK *)*pCircBufHdl;
   XDRV_UINT32 numBytesRead = 0;
   
#if CIRCBUF_SPINLOCK_SUPPORT
   unsigned long flags;
   
   if( pCircBufInfoBlk->bLock )
   {
      bosSpinlockAcquire( &(pCircBufInfoBlk->spinLock), &flags );
   }
#endif /* CIRCBUF_SPINLOCK_SUPPORT */

   numBytesRead = circBufOperation( pCircBufHdl, CBUF_READ, pBuf, length );
   
#if CIRCBUF_SPINLOCK_SUPPORT
   if( pCircBufInfoBlk->bLock )
   {
      bosSpinlockRelease( &(pCircBufInfoBlk->spinLock), flags );
   }
#endif /* CIRCBUF_SPINLOCK_SUPPORT */   
   
   return numBytesRead;
}
   
      
/*
*****************************************************************************
** FUNCTION:   circBufWrite
**
** PURPOSE:    To write data to the circular buffer.
**
** PARAMETERS: pCircBufHdl (mod) circBuf object to write to.
**             pBuf        (in)  Data written to the circbuff is read from this buffer.
**             length      (in)  Number of bytes to read.
**
** RETURNS:    Number of bytes written.
**
** NOTE:
*****************************************************************************
*/
XDRV_SINT32 circBufWrite
(
   XDRV_CIRCBUF_HDL * pCircBufHdl,
   char         *pBuf,
   XDRV_UINT32 length
)
{
   XDRV_UINT32 numBytesWritten = 0;
   CIRCBUF_INFOBLK * pCircBufInfoBlk = (CIRCBUF_INFOBLK *)*pCircBufHdl;

#if CIRCBUF_SPINLOCK_SUPPORT
   unsigned long flags;

   if( pCircBufInfoBlk->bLock )
   {
      bosSpinlockAcquire( &(pCircBufInfoBlk->spinLock), &flags );
   }
#endif /* CIRCBUF_SPINLOCK_SUPPORT */

   numBytesWritten = circBufOperation( pCircBufHdl, CBUF_WRITE, pBuf, length );
   
#if CIRCBUF_SPINLOCK_SUPPORT
   if( pCircBufInfoBlk->bLock )
   {
      bosSpinlockRelease( &(pCircBufInfoBlk->spinLock), flags );
   }
#endif /* CIRCBUF_SPINLOCK_SUPPORT */   
   
   return numBytesWritten;   
}
