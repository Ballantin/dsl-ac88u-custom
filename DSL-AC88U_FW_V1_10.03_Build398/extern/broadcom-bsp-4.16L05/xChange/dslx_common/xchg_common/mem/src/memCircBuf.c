/****************************************************************************
*  
*  Copyright (c) 2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.  
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*  Filename: memCircBuf.c
*
****************************************************************************
*  Description: This file contains the implementation of a lockless circular 
*               buffer API.
*
****************************************************************************/

/* ---- Include Files -------------------------------------------------------------- */
#include <string.h>
#include <bosSpinlock.h>
#include <memCircBuf.h>
#include <malloc.h>

/* ---- Constants and Types -------------------------------------------------------- */

/* IPC buffer descriptor */
typedef struct MEMCIRCBUFDESC
{
   struct MEMCIRCBUFDESC *nextp;       /* Next free buffer descriptor when in the free list */
   unsigned int          size;         /* Buffer size in bytes */
   unsigned char         bufIsFull;    /* Flag to indicate if buffer is full */
   unsigned int          readIndex;    /* Buffer read index */
   unsigned int          writeIndex;   /* Buffer write index */
   char                  *bufp;        /* Buffer pointer */
   BOS_SPINLOCK          lock;         /* Buffer lock */
} MEMCIRCBUFDESC;

/* ---- Private Variables ---------------------------------------------------------- */
/* Complete pool of circular memory buffer buffers */
static MEMCIRCBUFDESC memCircBuffers[MAX_BUFFERS];

/* Pointer to the next free circular memory buffer */
static MEMCIRCBUFDESC *memCircBufFreeDesc = NULL;

/* Driver initialized flag */
static unsigned int isMemCircBufInitialized = 0;

/* Lock for the circular buffer driver */
static BOS_SPINLOCK memCircBufLock;                 

/* ---- Private Function Prototypes ------------------------------------------------ */
static unsigned int memCircBufWritten(MEMCIRCBUFDESC *bufDesc);
static unsigned int memCircBufGetAllocSize(unsigned int *size);

/* ---- Functions ------------------------------------------------------------------ */

/*
***************************************************************************************
** Name:    memCircBufInit
** Purpose: Initialize the memory buffer descriptors.
** Input:   None.
** Output:  BUF_STATUS_OK   => Success
**          BUF_STATUS_FAIL => Failure
** Remarks: None.
***************************************************************************************
*/
BUF_STATUS memCircBufInit(void)
{
   unsigned int i = 0;

   /* Check if driver is already initialized */
   if ( isMemCircBufInitialized )
   {
      printk("%s: Driver already initialized!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }
   
   /* Create circular buffer driver lock */
   if ( bosSpinlockCreate(&memCircBufLock) != BOS_STATUS_OK )
   {
      printk("%s: Failed to create circular buffer driver lock!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Initialize circular memory buffer descriptors */
   for(memCircBufFreeDesc = &memCircBuffers[i]; i < MAX_BUFFERS; i++, memCircBuffers[i-1].nextp = &memCircBuffers[i])
   {
      memCircBuffers[i].readIndex = memCircBuffers[i].writeIndex = memCircBuffers[i].size = 0; 
      memCircBuffers[i].bufIsFull ^= memCircBuffers[i].bufIsFull;
      memCircBuffers[i].bufp = NULL;
   }
   memCircBuffers[i-1].nextp = NULL;

   isMemCircBufInitialized = 1;

   return BUF_STATUS_OK;
}

/*
***************************************************************************************
** Name:    memCircBufDeinit
** Purpose: Deinitialize the memory buffer descriptors.
** Input:   None.
** Output:  BUF_STATUS_OK   => Success
**          BUF_STATUS_FAIL => Failure
** Remarks: None.
***************************************************************************************
*/
BUF_STATUS memCircBufDeinit(void)
{
   unsigned int i;

   /* Check if driver is initialized */
   if ( !isMemCircBufInitialized )
   {
      printk("%s: Driver not previously initialized!\n", __FUNCTION__);
      return BUF_STATUS_OK;
   }

   if ( bosSpinlockDestroy(&memCircBufLock) != BOS_STATUS_OK )
   {
      printk("%s: Failed to destroy circular buffer driver lock!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   for(i = 0; i < MAX_BUFFERS; i++)
   {
      /* Free buffer if necessary */
      if ( memCircBuffers[i].bufp )
      {
         free(memCircBuffers[i].bufp);
      }

      /* Zero out the buffer descriptor */
      memset(&memCircBuffers[i], 0, sizeof(MEMCIRCBUFDESC));
   }
   memCircBufFreeDesc = NULL;   

   isMemCircBufInitialized = 0;

   return BUF_STATUS_OK;
}

/*
***************************************************************************************
** Name:    memCircBufAlloc
** Purpose: Allocate an circular moemory buffer and return it's associated handle.
** Input:   bufDrv          => Place holder for new buffer handle.
**          size            => Size of the buffer to be allocated.
** Output:  BUF_STATUS_OK   => Success
**          BUF_STATUS_FAIL => Failure
** Remarks:    None.
***************************************************************************************
*/
BUF_STATUS memCircBufAlloc(MEMCIRCBUFDRV *bufDrv, unsigned int size)
{
   BUF_STATUS status = BUF_STATUS_OK;
   unsigned long flags;
   void         *pbuf = NULL;

   /* Check if driver is initialized */
   if ( !isMemCircBufInitialized )
   {
      printk("%s: Driver not initialized!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Initialize buffer descriptor */
   if ( (pbuf = malloc(memCircBufGetAllocSize(&size))) == NULL )
   {
      printk("%s: Unable to allocate buffer!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Acquire driver lock */
   if ( bosSpinlockAcquire(&memCircBufLock, &flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to acquire circular buffer driver lock!\n", __FUNCTION__);
      free(pbuf);
      return BUF_STATUS_FAIL; 
   }

   /* Check if buffer is available */
   if ( memCircBufFreeDesc == NULL )
   {
      printk("%s: No free buffer available!\n", __FUNCTION__);
      status = BUF_STATUS_FAIL;
      goto exit; 
   }
   
   /* Create circular buffer driver lock */
   if ( bosSpinlockCreate(&memCircBufFreeDesc->lock) != BOS_STATUS_OK )
   {
      printk("%s: Failed to create circular buffer lock!\n", __FUNCTION__);
      status = BUF_STATUS_FAIL; 
      goto exit;
   }

   /* Initialize buffer descriptor */
   memCircBufFreeDesc->bufp = (char *)pbuf;
   pbuf = NULL;
   memCircBufFreeDesc->readIndex = memCircBufFreeDesc->writeIndex = 0;
   memCircBufFreeDesc->size = size;

   /* Adjust buffer descriptor pointers */
   *bufDrv = memCircBufFreeDesc - memCircBuffers;
   memCircBufFreeDesc = memCircBufFreeDesc->nextp;
   memCircBuffers[*bufDrv].nextp = NULL;

exit:
   /* Release driver lock */
   if ( bosSpinlockRelease(&memCircBufLock, flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to release circular buffer driver lock!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   if(pbuf != NULL)
   {
      free(pbuf);
   }

   return status;
}

/*
***************************************************************************************
** Name:    memCircBufFree
** Purpose: Free the circular memory buffer associated with the specified handle.
** Input:   bufDrv          => Buffer handle.
** Output:  BUF_STATUS_OK   => Success
**          BUF_STATUS_FAIL => Failure
** Remarks:    None.
***************************************************************************************
*/
BUF_STATUS memCircBufFree(MEMCIRCBUFDRV *bufDrv)
{
   BUF_STATUS status = BUF_STATUS_OK;
   MEMCIRCBUFDESC *bufDesc;
   unsigned long flags;
   void          *pbuf = NULL;

   /* Check if driver is initialized */
   if ( !isMemCircBufInitialized )
   {
      printk("%s: Driver not initialized!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Check if the buffer index is valid */
   if ( !IS_VALID(*bufDrv) )
   {
      printk("%s: Invalid buffer index!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Get buffer descriptor */
   bufDesc = &memCircBuffers[*bufDrv];

   /* Acquire driver lock */
   if ( bosSpinlockAcquire(&memCircBufLock, &flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to acquire circular buffer driver lock!\n", __FUNCTION__);
      return BUF_STATUS_FAIL;
   }

   /* Deallocate buffer descriptor */
   pbuf = (void *)bufDesc->bufp;
   bufDesc->bufp = NULL;
   bufDesc->readIndex = bufDesc->writeIndex = bufDesc->size = 0;
   bufDesc->bufIsFull = 0;
   
   /* Destroy buffer lock */
   if ( bosSpinlockDestroy(&bufDesc->lock) != BOS_STATUS_OK )
   {
      printk("%s: Failed to destroy circular buffer driver lock!\n", __FUNCTION__);
      status = BUF_STATUS_FAIL; 
      goto exit;  
   }   

   /* Adjust descriptor pointers */
   bufDesc->nextp = memCircBufFreeDesc;
   memCircBufFreeDesc = bufDesc;
   *bufDrv = (MEMCIRCBUFDRV)-1;

exit:

   /* Release driver lock */
   if ( bosSpinlockRelease(&memCircBufLock, flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to release circular buffer driver mutex!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   if(pbuf != NULL)
   {
      free(pbuf);
   }

   return status;
}

/*
***************************************************************************************
** Name:    memCircBufRead
** Purpose: Read data from the buffer associated with the specified handle.
** Input:   bufDrv          => Buffer handle.
**          dest            => The destination buffer for the read data.
**          count           => Amount of bytes to read.(memCircBufWritten(bufDesc) < count)
** Output:  BUF_STATUS_OK   => Success
**          BUF_STATUS_FAIL => Failure
**          BUF_STATUS_EMPTY=> Buffer empty. Request cannot be satisfied.
** Remarks:    None.
***************************************************************************************
*/
BUF_STATUS memCircBufRead(MEMCIRCBUFDRV bufDrv, char *dest, unsigned int count)
{
   BUF_STATUS status = BUF_STATUS_OK;
   MEMCIRCBUFDESC *bufDesc;
   unsigned int firstHalf; 
   unsigned long flags;

   /* Check if driver is initialized */
   if ( !isMemCircBufInitialized )
   {
      printk("%s: Driver not initialized!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Check if the buffer index is valid */
   if ( !IS_VALID(bufDrv) )
   {
      printk("%s: Invalid buffer index!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Get buffer descriptor */
   bufDesc = &memCircBuffers[bufDrv];

   /* Acquire buffer lock */
   if ( bosSpinlockAcquire(&bufDesc->lock, &flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to acquire circular buffer lock!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Check if the read operation empties the buffer */
   if ( !bufDesc->bufIsFull || count > bufDesc->size )
   {
      if (memCircBufWritten(bufDesc) < count)
      {
         status = BUF_STATUS_EMPTY;
         goto exit;
      }
   }

   /* Read request can be accomdated. Read the data from memory buffer */
   if ( bufDesc->readIndex + count >= bufDesc->size )
   {
      firstHalf = bufDesc->size - bufDesc->readIndex;
      memcpy((void*)dest, (void*)&bufDesc->bufp[bufDesc->readIndex], firstHalf);
      memcpy((void*)&dest[firstHalf], (void*)bufDesc->bufp, count - firstHalf);      
   }
   else
   {
      memcpy((void*)dest, (void*)&bufDesc->bufp[bufDesc->readIndex], count);
   }

   /* Reset buffer flag */ 
   bufDesc->bufIsFull ^= bufDesc->bufIsFull;

   /* Update read index. The '&' operator can only be used if the buffer
    * size is a power of 2. It is safe to use the operator in our case 
    * since we force all buffers to be powers of 2. 
    */
   bufDesc->readIndex = ((bufDesc->readIndex + count) & (bufDesc->size - 1)); 

exit:
   /* Release buffer lock */
   if ( bosSpinlockRelease(&bufDesc->lock, flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to release circular buffer lock!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   return status;
}

/*
***************************************************************************************
** Name:    memCircBufWrite
** Purpose: Write data to the buffer associated with the specified handle.
** Input:   bufDrv          => Buffer handle
**          src             => Source buffer to read data from.
**          count           => Amount of bytes to write. 
** Output:  BUF_STATUS_OK   => Success
**          BUF_STATUS_FAIL => Failure
**          BUF_STATUS_FULL => Buffer full. Request cannot be satisfied.
** Remarks:    None.
***************************************************************************************
*/
BUF_STATUS memCircBufWrite(MEMCIRCBUFDRV bufDrv, char *src, unsigned int count)
{
   BUF_STATUS status = BUF_STATUS_OK;
   MEMCIRCBUFDESC *bufDesc;
   unsigned int firstHalf; 
   unsigned long flags;

   /* Check if driver is initialized */
   if ( !isMemCircBufInitialized )
   {
      printk("%s: Driver not initialized!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Check if the buffer index is valid */
   if ( !IS_VALID(bufDrv) )
   {
      printk("%s: Invalid buffer index!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Get buffer descriptor */
   bufDesc = &memCircBuffers[bufDrv];

   /* Acquire buffer lock */
   if ( bosSpinlockAcquire(&bufDesc->lock, &flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to acquire circular buffer mutex!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   /* Check if the write operation fill up the memory buffer */
   if ( (bufDesc->bufIsFull) || ((bufDesc->size - memCircBufWritten(bufDesc)) < count) )
   {
      status = BUF_STATUS_FULL;
      goto exit;
   }
  
   /* Write request can be accomdated. Write the data to the buffer */
   if ( bufDesc->writeIndex + count >= bufDesc->size )
   {
      firstHalf = bufDesc->size - bufDesc->writeIndex;
      memcpy((void*)&bufDesc->bufp[bufDesc->writeIndex], (void*)src, firstHalf);
      memcpy((void*)bufDesc->bufp, (void*)&src[firstHalf], count - firstHalf);      
   }
   else
   {
      memcpy((void*)&bufDesc->bufp[bufDesc->writeIndex], (void*)src, count);
   }

   /* Update write index. The '&' operator can only be used if the buffer
    * size is a power of 2. It is safe to use the operator in our case 
    * since we force all buffers to be powers of 2. 
    */
   bufDesc->writeIndex = ((bufDesc->writeIndex + count) & (bufDesc->size - 1));

   /* Set buffer flag appropriately */ 
   bufDesc->bufIsFull = (bufDesc->writeIndex == bufDesc->readIndex);

exit:
   /* Release buffer lock */
   if ( bosSpinlockRelease(&bufDesc->lock, flags) != BOS_STATUS_OK )
   {
      printk("%s: Failed to release circular buffer mutex!\n", __FUNCTION__);
      return BUF_STATUS_FAIL; 
   }

   return status;
}


/*
***************************************************************************************
** Name:    memCircBufWritten
** Purpose: Amount of data written in a buffer associated with specified handle.
** Input:   bufDesc => Pointer to the buffer descriptor.
** Output:  Amount of bytes written to the buffer.
** Remarks:    None.
***************************************************************************************
*/
static unsigned int memCircBufWritten(MEMCIRCBUFDESC *bufDesc)
{
   return (bufDesc->writeIndex >= bufDesc->readIndex) ? bufDesc->writeIndex - bufDesc->readIndex 
                                                      : bufDesc->size - (bufDesc->readIndex - bufDesc->writeIndex);
}

/*
***************************************************************************************
** Name:    memCircBufGetAllocSize
** Purpose: Amount of size to be allocated for a buffer. The size is a power of 2.
** Input:   size => Size of buffer requested. May not be a power of 2.
** Output:  Size to be allocated for the memory buffer. Size is a power of 2.
** Remarks:    None.
***************************************************************************************
*/
static unsigned int memCircBufGetAllocSize(unsigned int *size)
{
   int mask;

   /* Find the largest pwower of 2 that can accomodate the requested buffer size */
   for (mask = 1; mask > 0 && mask < *size; mask <<= 1);

   return (*size = mask);
}




