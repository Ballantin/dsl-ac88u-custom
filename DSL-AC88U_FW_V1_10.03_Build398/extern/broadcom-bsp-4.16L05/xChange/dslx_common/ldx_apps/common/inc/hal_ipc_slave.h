/****************************************************************************
*     Copyright (c) 2002 Broadcom
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
****************************************************************************/
#ifndef HAL_IPC_SLAVE_H
#define HAL_IPC_SLAVE_H

#include "hal_ipc.h"
#include <xdrvIpc.h>

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ----------------------------------------------- */

typedef struct
{
   char                 inUse;
   volatile HAPI_UINT16 *pSharedMem;
   XDRV_IPC             *pIpcDrv;

} HAL_IPC_SLAVE_TABLE;

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------- */

void halIpcSlaveInit( volatile void *pIpcMem);
XDRV_IPC *halIpcSlave_InitUnused ( HAPI_UINT16 vhdType, HAL_IPC_INIT_INFO *info );
XDRV_IPC *halIpcSlave_InitUnusedExtended ( HAL_IPC_INIT_INFO *info, unsigned getParamsFromSharedMem );
void halIpcSlave_DeInitChannel( XDRV_IPC *ipcHandle );

#endif /* HAL_IPC_SLAVE_H */
