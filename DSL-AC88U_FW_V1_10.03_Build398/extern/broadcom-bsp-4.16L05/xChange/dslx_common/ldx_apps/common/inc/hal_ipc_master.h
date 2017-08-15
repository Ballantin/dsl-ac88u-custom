/****************************************************************************
*
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
*****************************************************************************/

#if !defined( HAL_IPC_MASTER_H )
#define HAL_IPC_MASTER_H

/* ---- Include Files ---------------------------------------------------- */

#include "hal_ipc.h"
#include <xdrvIpc.h>

/* ---- Constants and Types ---------------------------------------------- */
#define HAL_IPC_MASTER_STARTUP_CHANID  0x30

#define HAL_CFG_IPC_LOG                  0
#define HAL_IPC_LOG_MAX_LINES          200
#define HAL_IPC_LOG_MAX_LINE_LENGTH_W   80

typedef struct
{
   unsigned     msgLength;
   unsigned     timeStamp;
   HAPI_UINT16  msgBuf[HAL_IPC_LOG_MAX_LINE_LENGTH_W];

} HAL_IPC_LOGLINE;

typedef struct
{
   char         inUse;
   char         hasBeenUsed;
   HAPI_UINT16  vhdType;
   XDRV_IPC     *ipcDrv;

   HAPI_UINT16  startOffset;
   HAPI_UINT16  dspToHostSize;
   HAPI_UINT16  hostToDspSize;

#if HAL_CFG_IPC_LOG
   unsigned         logLineIdx;
   HAL_IPC_LOGLINE  logLine[HAL_IPC_LOG_MAX_LINES];
#endif

} HAL_IPC_MASTER_TABLE;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

void halIpcMasterInit
(
   unsigned             dspNum,
   volatile HAPI_UINT16 *pIpcMem,
   unsigned int         ipcHeapSizeWords
);

XDRV_IPC *halIpcMaster_InitUnused
(
   HAPI_UINT16    vhdType,
   HAPI_UINT16    dspNum,
   HAL_IPC_INIT_INFO *info
);

XDRV_IPC *halIpcMaster_InitUnusedExtended
(
   HAPI_UINT16    vhdType,
   HAPI_UINT16    dspNum,
   HAL_IPC_INIT_INFO *info,
   unsigned       useSharedMemToCommunicateParams

);



void halIpcMaster_DeInitChannel( XDRV_IPC *ipcHandle );

void halIpcPrintStats( void );

#if HAL_CFG_IPC_LOG
void halIpcLogInit( void );
void halIpcLogMsg( void *ipcHandle, HAPI_UINT16 *pBuf, unsigned numWords );
void halIpcLogDump( void );
void halIpcLogEnable( void );
void halIpcLogDisable( void );
#endif

#endif /* HAL_IPC_MASTER_H */
