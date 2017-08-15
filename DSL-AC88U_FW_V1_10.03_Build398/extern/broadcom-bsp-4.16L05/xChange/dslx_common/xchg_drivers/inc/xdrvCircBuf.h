/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: xdrvCircBuf.h
*
****************************************************************************
*    Description:
*
*     Interface for architecture/chip independent spin-lock API. A spin-lock is
*     a synchronization object used to provide protection against concurrent
*     access of data structures shared by more than one processor. It relies upon
*     architecture-specific instructions to perform atomic read-modify-writes.
*
*     Other methods of providing concurrency protection typically used on
*     uniprocessor systems (interrupt disable, mutex etc) are not sufficient
*     on multiprocessor systems. These traditional primitives can only provide
*     protection on a single processor.
*
*     The spin-lock is basically a flag, that is initially clear. When the
*     spin-lock is acquired, the flag is set. And when it is released, the flag
*     is cleared. If the flag is already set when an acquire is performed,
*     the caller will block (spin) until is is released by the owning processor.
*     For the spin-lock to work correctly, the test-modify-update of the flag
*     in the acquire function must be performed atomically.
*
*     It is important that the code protected by the spin-lock is extremely
*     short, since other processors attempting to acquire the lock will spin
*     until the lock is released.
*
*     It is also important to prevent the owner of a spin-lock from being
*     preempted (either by an interrupt, or context switch). This could result
*     in other processors spinning for long periods of time, if one processor
*     owns the spin-lock, but isn't executing the code protected by the lock.
*     To prevent this situation, interrupts are disabled prior to acquiring
*     the spin-lock, and enabled after releasing the spin-lock.
*
*     Spin-locks are not nestable. Nested calls will likely result in deadlock.
*
****************************************************************************/

#ifndef XDRV_CIRC_BUF_H
#define XDRV_CIRC_BUF_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvGlobalInterrupt.h>


/* ---- Constants and Types ---------------------------------------------- */

/* circBuff abstract data-type. */
typedef char *XDRV_CIRCBUF_HDL; 
typedef char * (*XDRV_CIRCBUF_ALLOCMEM_FP)( int );
typedef void (*XDRV_CIRCBUF_FREEMEM_FP )( char * );
                                       
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */
/* ---- Function Prototypes ---------------------------------- */

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
);

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
);

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
);

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
);


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_CIRC_BUF_H  */
