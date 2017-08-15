/****************************************************************************
*  Copyright (c) 2002 Broadcom
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

#if !defined( HAL_IPC_H )
#define HAL_IPC_H

/* ---- Include Files ---------------------------------------------------- */

#include <hapi_types.h>
#include <xdrvIpc.h>

/* ---- Constants and Types ---------------------------------------------- */

/* Specifies possible values of newDataFlag in
 * HAL_IPC_INIT_TABLE
 */
#define   HAL_IPC_DATA_READ 0x1234  /* Indicates data has been read    */
#define   HAL_IPC_DATA_NEW  0x4321  /* Indicates new data is available */

/* IPC communication table
 */
typedef struct
{
   HAPI_UINT16  newDataFlag;    /* Indicates data is available */
   HAPI_UINT16  ipcChanId;      /* ID of the first IPC channel */
   HAPI_UINT16  dspToHostSize;  /* Size of DSP to Host buffer  */
   HAPI_UINT16  hostToDspSize;  /* Size of Host to DSP buffer  */

} HAL_IPC_INIT_TABLE;

/* IPC communication table
 */
typedef struct
{
   HAPI_UINT16  ipcChanId;      /* IPC channel ID */
   HAPI_UINT16  dspToHostSize;  /* Size of DSP to Host buffer  */
   HAPI_UINT16  hostToDspSize;  /* Size of Host to DSP buffer  */

} HAL_IPC_INIT_INFO;

/* IPC Shared memory layout
 */
typedef struct
{
   HAL_IPC_INIT_TABLE  initInfo;  /* IPC initialization info   */
   HAPI_UINT16         mem[1];    /* Start of shared mem space */

} HAL_IPC_LAYOUT;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#endif /* HAL_IPC_H */
