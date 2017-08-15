/***************************************************************************
*
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
*    Filename: cbf.h
*    Creation Date: 12 September 2002 (v0.00)
*
****************************************************************************
*    Description:
*
*      Circular buffer macros
*          cbf_qPointer   - read and write access into queue at given offset
* 	       cbf_qSize      - returns the current number of elements in queue
* 	       cbf_qRoom      - returns the current number of empty elements in queue
* 	       cbf_qError     - checks if over-run or under-run has occured
*          cbf_isPowerOf2 - true if Qlength is power of 2
*
****************************************************************************/

/* 	This file contains macros which are used to access
 *	circular buffers.  These macros allow multiple readers and single
 *	writers.  The write pointer (or read pointer) may wrap around
 *	several times and an over-run (or under-run) will still be detected.
 *	This allows the over-run and under-run checks to be delayed and done
 *	in lower priority tasks.  Since there are no critical regions in
 *  these macros, interrupts are not disabled.
 *
 *	The inputs to the macros are as follows:
 *
 *	qMaxSize	- this is the maximum size of the circular buffer.  This
 *				  size MUST be a power of 2 such as 8, 16, 2048 etc. This
 *				  parameter should be a constant.
 *
 *	qBasep		- pointer to the base of a circular buffer.  A circular
 *				  buffer is an array containing any type - including
 *				  structures.
 *
 *	qOffset 	- this is an offset into a circular buffer.  After
 *				  writing/reading an element in the queue, qOffset is
 *				  incremented by one to point to the next free/full
 *				  element in the queue. qOffset need only be advanced
 *				  by one regardless of the actual size of the elements
 *				  in the queue.
 *
 *	Since the read and write pointers to a circular buffer are actually
 *	offsets into the buffer, the SAME pointer may be used to offset into
 *	TWO different circular buffers of the same size.  This is useful
 *	for syncronizing read and writes in two different buffers.
 *
 *	To read an element from a queue do the following:
 *
 *		#include "cbf.h"
 *		Float 			queue[Q_MAX_SIZE];
 *  	CBF_Q_OFFSET		readOffset, writeOffset;
 *		Float			element;
 *
 *  	readOffset = writeOffset = 0;
 *		element = *cbf_qPointer(Q_MAX_SIZE, queue, readOffset++);
 *
 *	To write an element to a queue do the following:
 *
 *		*cbf_qPointer(Q_MAX_SIZE, queue, writeOffset++) = element;
 *
 */

#ifndef _CBF_H
#define _CBF_H


/* ---- Include Files ---------------------------------------- */

#include <bosTypes.h>


/* ---- Constant and Type Declarations ----------------------- */

/*
 * This must be the widest unsigned integer width for the
 * CPU of interest (e.g. 68K = 32 bits) or the unsigned
 * subtract will not work properly.
 */

typedef BOS_UINT32 CBF_Q_OFFSET;	/* offset into queue */

#define cbf_qPointer(qMaxSize, qBasep, qOffset) \
	((qBasep) + (int) ((qOffset) & ((qMaxSize) - 1)))

#define cbf_qSize(qWriteOffset, qReadOffset) \
	((qWriteOffset) - (qReadOffset))

#define cbf_qRoom(qMaxSize, qWriteOffset, qReadOffset) \
	((qMaxSize) - (qWriteOffset) + (qReadOffset))

#define cbf_qError(qMaxSize, qWriteOffset, qReadOffset) \
	(cbf_qSize((qWriteOffset), (qReadOffset)) > (qMaxSize))

#define cbf_isPowerOf2(qMaxSize) !((qMaxSize) & ((qMaxSize) - 1))


#endif /* _CBH_H */
