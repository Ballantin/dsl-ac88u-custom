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
*    Filename: xdrvHpi.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface to the
*     Host Port Interface (HPI).
*
****************************************************************************/

#ifndef XDRV_HPI_H
#define XDRV_HPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* --------------------------------------------------------------------------
** 'XDRV_HPI' is the base, generic HPI structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific HPI drivers.
**
** 'XDRV_HPI_FUNCS' must be the only structure member of 'XDRV_HPI'.
**
** Chip specific HPI drivers should provide a "derived" version of XDRV_HPI:
**
**    typedef struct HPI_CHIP_DRV
**    {
**       const XDRV_HPI_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } HPI_CHIP_DRV;
**
** Clients, which have access to XDRV_HPI structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_HPI struct as the first parameter. This struct can be cast to the chip
** specific version of the HPI structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_HPI_FUNCS *pDrvFuncs;

} XDRV_HPI;


/* --------------------------------------------------------------------------
** HPI API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvHpiTxFunc
**
** PURPOSE:    Transmit data to HPI
**
** PARAMETERS: pDrv        - input - pointer to the hpi driver structure
**             pHdr        - input - Address of data header to write
**             hdrSize     - input - Number of (16-bit) words in header to write
**             pPayload    - input - Address of data payload to write
**             payloadSize - input - Number of (16-bit) words in payload to write
**
** RETURNS:    Total number of 16-bit words that were successfully written.
**
** NOTES       The data to be transmitted may consist of a single contiguous
**             buffer, or it may consist of two seperate non-contiguous buffers
**             (header and payload). If it consists of a single buffer, a pointer
**             to the buffer should be passed to 'pHdr', and the total buffer length
**             passed to 'hdrSize'. In this case, 'pPayload' must be set to NULL
**             and 'payloadSize' to zero.
**
*****************************************************************************
*/
typedef XDRV_UINT16 ( *xdrvHpiTxFunc )
(
   XDRV_HPI      *pDrv,
   XDRV_UINT16   *pHdr,
   XDRV_UINT16    hdrSize,
   XDRV_UINT16   *pPayload,
   XDRV_UINT16    payloadSize
);

/*
*****************************************************************************
** FUNCTION:   xdrvHpiRxFunc
**
** PURPOSE:    Reads data from HPI.
**
** PARAMETERS: pDrv         - input - pointer to the hpi driver structure
**             rxWordp      - output - pointer to a buffer to receive data
**             maxRxWords   - input - maximum number of words to read
**
** RETURNS:    wordCount    - the number of words read
**
** NOTE:
*****************************************************************************
*/
typedef XDRV_UINT16 ( *xdrvHpiRxFunc )
(
   XDRV_HPI      *pDrv,
   XDRV_UINT16   *rxWordp,
   XDRV_UINT16    maxRxWords
);


/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_HPI_FUNCS
{
   xdrvHpiTxFunc  hpiTx;
   xdrvHpiRxFunc  hpiRx;
} XDRV_HPI_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the HPI.
*/
#define xdrvHpiTx( pDrv, pHdr, hdrSize, pPayload, payloadSize ) \
         ( pDrv->pDrvFuncs->hpiTx( pDrv, pHdr, hdrSize, pPayload, payloadSize ) )


#define xdrvHpiRx( pDrv, rxWordp, maxRxWords ) \
         ( pDrv->pDrvFuncs->hpiRx( pDrv, rxWordp, maxRxWords ) )


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_HPI_H  */
