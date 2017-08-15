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
*    Filename: boardHalIpc.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      Inter-Processor Communication (IPC) drivers present on a board.
*
****************************************************************************/

#ifndef BOARD_HAL_IPC_H
#define BOARD_HAL_IPC_H

#if __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvIpc.h>
#include <vrgTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** Supported IPC channel types. Each channel supports different packet rate and
** size requirements.
*/
typedef enum BOARD_HAL_IPC_CHANNEL_TYPE
{
   /* Master control channel. */
   BOARD_HAL_IPC_MASTER_CONTROL_CHANNEL,

   /* Used to transfer control packets associated with an IPC channel instance. */
   BOARD_HAL_IPC_CONTROL_CHANNEL,

   /* Used to transfer data(voice) packets associated with an IPC channel instance. */
   BOARD_HAL_IPC_DATA_CHANNEL

} BOARD_HAL_IPC_CHANNEL_TYPE;


/*
** Communication handle that can be used to uniquely identify the IPC channel
** on both processors.
*/
typedef VRG_SINT16   BOARD_HAL_IPC_COM_HANDLE;


/*
** Handshake values for IPC init
*/
#define   BOARD_HAL_IPC_DATA_READ 0x1234  /* Indicates data has been read    */
#define   BOARD_HAL_IPC_DATA_NEW  0x4321  /* Indicates new data is available */

/*
** IPC initialization table
**/
typedef struct
{
   VRG_UINT16  newDataFlag;      /* Indicates data is available */
   VRG_UINT16  ipcChanId;        /* ID of the first IPC channel */
   VRG_UINT16  dspToHostSize;    /* Size of DSP to Host buffer  */
   VRG_UINT16  hostToDspSize;    /* Size of Host to DSP buffer  */

} BOARD_HAL_IPC_INIT_TABLE;


/*
 ** IPC communication table
 */
typedef struct
{
   VRG_UINT16  ipcChanId;      /* IPC channel ID */
   VRG_UINT16  dspToHostSize;  /* Size of DSP to Host buffer  */
   VRG_UINT16  hostToDspSize;  /* Size of Host to DSP buffer  */

} BOARD_HAL_IPC_INIT_INFO;


/* IPC shared memory layout
 */
typedef struct
{
   BOARD_HAL_IPC_INIT_TABLE  initInfo;  /* IPC initialization info   */
   VRG_UINT16                mem[1];    /* Start of shared mem space */

} BOARD_HAL_IPC_LAYOUT;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalIpcInit
**
** PURPOSE:    Initialize the IPC
**
** PARAMETERS: pIpcMem           - pointer to IPC memory
**             ipcHeapsizeWords  - heap words available
**             dspNum            - the DSP number.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalIpcInit( int dspNum, volatile VRG_UINT16 *pIpcMem,
                     unsigned int ipcHeapSizeWords );


/*
*****************************************************************************
** FUNCTION:   boardHalIpcDeinit
**
** PURPOSE:    Shutdown the IPC
**
** PARAMETERS: dspNum - the DSP number.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalIpcDeinit( int dspNum );


/*
*****************************************************************************
** FUNCTION:   boardHalIpcCreateChannel
**
** PURPOSE:    To create an IPC channel.
**
** PARAMETERS: deviceNum   (in)  Device (DSP) index for which the IPC channel
**                               should be created.
**             chanType    (in)  Channel type to create.
**             comHandle   (in)  Communication handle that can be used to
**                               uniquely identify the IPC channel on both
**                               processors.
**             pIpcInfo    (out) pointer to IPC communication table
**
** RETURNS:    Pointer to IPC driver instance.
**
** NOTE:
*****************************************************************************
*/
XDRV_IPC* boardHalIpcCreateChannel
(
   unsigned int                  deviceNum,
   BOARD_HAL_IPC_CHANNEL_TYPE    chanType,
   BOARD_HAL_IPC_COM_HANDLE      comHandle,
   BOARD_HAL_IPC_INIT_INFO      *pIpcInfo
);


/*
*****************************************************************************
** FUNCTION:   boardHalIpcDestroyChannel
**
** PURPOSE:    To destroy a previously created IPC channel.
**
** PARAMETERS: ipcDrv   (in)  IPC driver.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void boardHalIpcDestroyChannel( XDRV_IPC  *ipcDrv );


/*
*****************************************************************************
** FUNCTION:   boardHalIpcReady
**
** PURPOSE:    To indicate to the DSP that the necessary IPC initialization
**             information is available
**
** PARAMETERS: dspNum - the DSP number to inform.
**
** RETURNS:    0 on success, else error code.
**
** NOTE:
*****************************************************************************
*/
int boardHalIpcReady( int dspNum );


/*
*****************************************************************************
** FUNCTION:   boardHalIpcDump
**
** PURPOSE:    Dump IPC info for all active channels
**
** PARAMETERS: comHandle   (in)  Communication handle that can be used to
**                               uniquely identify the IPC channel on both
**                               processors.
**             mode        (IN)  IPC dump mode (full dump or stats only)
**
** RETURNS:    nothing
**
** NOTE:      comHandle equal -1 means print all channels in use
*****************************************************************************
*/
void boardHalIpcDump( BOARD_HAL_IPC_COM_HANDLE comHandle, int mode );


#if __cplusplus
extern "C" {
#endif


#endif /* BOARD_HAL_IPC_H */
