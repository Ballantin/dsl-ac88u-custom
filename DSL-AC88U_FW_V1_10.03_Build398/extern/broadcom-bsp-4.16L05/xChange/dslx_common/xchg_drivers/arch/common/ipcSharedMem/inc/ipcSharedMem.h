/****************************************************************************
*     Copyright (c) 2002 Broadcom
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
****************************************************************************/
#if !defined( IPCSHAREDMEM_H )
#define IPCSHAREDMEM_H

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvIpc.h>
#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

typedef struct
{
   /* Must be first element of this struct. */
   const XDRV_IPC_FUNCS   *pDrvFuncs;

   void *ipcHandle;

} IPCSHAREDMEM_DRV;


/* ---- Function Prototypes ---------------------------------------------- */

void ipcSharedMemInit( void );


IPCSHAREDMEM_DRV *ipcSharedMemOpenMaster( volatile XDRV_UINT16 *pMem,
                                        unsigned dspToHostMemSize,
                                        unsigned hostToDspMemSize );

IPCSHAREDMEM_DRV *ipcSharedMemOpenSlave( volatile XDRV_UINT16 *pMem,
                                       unsigned dspToHostMemSize,
                                       unsigned hostToDspMemSize );

void ipcSharedMemCloseMaster( IPCSHAREDMEM_DRV *ipcDrv );
void ipcSharedMemCloseSlave( IPCSHAREDMEM_DRV *ipcDrv );


void ipcSharedMemProcess( IPCSHAREDMEM_DRV *pIpcDrv );


#endif /* IPCSHAREDMEM_H */
