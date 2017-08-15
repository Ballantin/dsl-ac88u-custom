/****************************************************************************
*
*     Copyright (c) 2003 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    xdrvTypes.h
*
****************************************************************************/
#if !defined( XDRV_IPC_H )
#define XDRV_IPC_H

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* 
** IPC Print mode:
**    0 = info mode (stats and info only)
**    1 = full mode (dump the entire IPC buffer, along with the 
**                    stats and other info)
*/
typedef enum 
{
   XDRV_IPC_PRINT_MODE_INFO = 0,   
   XDRV_IPC_PRINT_MODE_FULL_DUMP,   
}  XDRV_IPC_PRINT_MODE;

/* --------------------------------------------------------------------------
** 'XDRV_IPC' is the base, generic IPC structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific IPC drivers.
**
** 'XDRV_IPC_FUNCS' must be the only structure member of 'XDRV_IPC'.
**
** Chip specific IPC drivers should provide a "derived" version of XDRV_IPC:
**
**    typedef struct IPC_CHIP_DRV
**    {
**       const XDRV_IPC_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } IPC_CHIP_DRV;
**
** Clients, which have access to XDRV_IPC structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_IPC struct as the first parameter. This struct can be cast to the chip
** specific version of the IPC structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_IPC_FUNCS *pDrvFuncs;

} XDRV_IPC;


/* --------------------------------------------------------------------------
** HPI API function pointers.
*/

typedef unsigned  (*xdrvIpcReadNoFree)( XDRV_IPC *pDrv, XDRV_UINT16 **bufpp );
typedef void      (*xdrvIpcFreeRead)  ( XDRV_IPC *pDrv );
typedef unsigned  (*xdrvIpcWrite)     ( XDRV_IPC *pDrv,
                                        XDRV_UINT16 *pMsg1, unsigned msgLength1,
                                        XDRV_UINT16 *pMsg2, unsigned msgLength2 );
typedef void      (*xdrvIpcPrint)     ( XDRV_IPC *pDrv, XDRV_IPC_PRINT_MODE mode );

/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_IPC_FUNCS
{
   xdrvIpcReadNoFree  ipcReadNoFree;
   xdrvIpcFreeRead    ipcFreeRead;
   xdrvIpcWrite       ipcWrite;
   xdrvIpcPrint       ipcPrint;

} XDRV_IPC_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the IPC.
*/

#define xdrvIpcReadNoFree( pDrv, ppBuf ) \
         ( (pDrv)->pDrvFuncs->ipcReadNoFree( pDrv, ppBuf ) )

#define xdrvIpcFreeRead( pDrv ) \
         ( (pDrv)->pDrvFuncs->ipcFreeRead( pDrv ) )

#define xdrvIpcWrite( pDrv, pMsg1, msgLength1, pMsg2, msgLength2 ) \
         ( (pDrv)->pDrvFuncs->ipcWrite( pDrv, pMsg1, msgLength1, pMsg2, msgLength2) )

#define xdrvIpcPrint( pDrv, mode ) \
         ( (pDrv)->pDrvFuncs->ipcPrint( pDrv, mode ) )


#endif /* XDRV_IPC_H */

