/***************************************************************************
*    Copyright 2006  Broadcom
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
*    Filename: xdrvHeapManager.h
*
****************************************************************************
*    Description:
*
*     Public interface for Heap Manager.
*
****************************************************************************/

#ifndef XDRV_HEAP_MANAGER_H
#define XDRV_HEAP_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvHeapAddMemory
**
** PURPOSE:    Add Memory block to Heap.
**
** PARAMETERS: pMemoryBegin    (in)  beginning of Memory block.
**             memorySizeBytes (in)  size of the Memory block.
**
** RETURNS:    N/A.
**
** NOTE:       This is used to add unused static memory block to the Heap
*****************************************************************************
*/
void xdrvHeapAddMemory(void *pMemoryBegin, unsigned long memorySizeBytes);


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_HEAP_MANAGER_H  */
