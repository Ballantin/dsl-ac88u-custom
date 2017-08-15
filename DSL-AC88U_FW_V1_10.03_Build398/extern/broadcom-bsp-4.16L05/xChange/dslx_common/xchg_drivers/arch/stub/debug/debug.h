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
*    Filename: debug.h
*
****************************************************************************
*    Description:
*
*     This file contains defines and functions related to the Debug module.
*     Debug module is used to retrieve info of interest from the DSP upon
*     failure conditions (assert, command timeouts, etc.). For now this is
*     going to dump out the entire IPC shared memory, but may be enhanced
*     in the future to dump internal DSP memory, registers, etc.
*
****************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#if __cplusplus
extern "C" {
#endif

#include <xdrvTypes.h>
#include <xdrvIpc.h>
//#include <dbgCom.h>
typedef void DBGCOM;

#include <xdrvApm.h>

/* Max number of channels Debug module would attempt to debug */

#define DEBUG_MAX_NUMBER_OF_CHANNELS    20

typedef struct
{
   XDRV_IPC*   ipcDrvp;
   XDRV_UINT16 handle;
   XDRV_BOOL   debugDone;
} DEBUG_CHANNEL ;

/*
**  Debug driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_DEBUG' base-class(structure).
** The base DEBUG structure specifies a set of function pointers that defines
** the generic API that must be provided by all chip specific DEBUG drivers.
**
** When the  DEBUG driver initialization function is invoked, the base DEBUG
** structure function pointers will be intialized to a set of specific
** DEBUG driver functions. Therefore, given a pointer to the base DEBUG driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific DEBUG driver function.
**
** DEBUG driver state that is specific to the  should be stored in the
** 'DEBUG_DRV' structure. A pointer to 'XDRV_DEBUG_FUNCS' must be the first
** member of the 'DEBUG_DRV' struct.
*/
typedef struct
{
   /* Pointer to DSP debug communication driver */
   DBGCOM   *pDbgCom;

   /* Pointer to APM driver structure */
   XDRV_APM *pApmDrv;

   /* Number of IPC channels registered with the module */
   int registerCount;

   /*  debug module internal state info. */
   DEBUG_CHANNEL ChanState[DEBUG_MAX_NUMBER_OF_CHANNELS];

} DEBUG_DRV;


/*
*****************************************************************************
** FUNCTION:   debugInit
**
** PURPOSE:    Initialize the Debug module
**
** PARAMETERS: pDbgCom - pointer to the DSP debug communication driver
**             pApmDrv - pointer to APM driver structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void debugInit( DBGCOM  *pDbgCom, XDRV_APM *pApmDrv );



/*
*****************************************************************************
** FUNCTION:   debugDeinit
**
** PURPOSE:    Shutdown the Debug module
**
** PARAMETERS:
**
** RETURNS:    none
**
*****************************************************************************
*/
void debugDeinit( void );

/*
*****************************************************************************
** FUNCTION:   debugRegisterChannel
**
** PURPOSE:    Register channel with debug module
**
** PARAMETERS: pDrv    - pointer to the Debug module info structure
**             pIpcDrv - IPC channel to register
**             handle  - channel handle
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int debugRegisterChannel
(
   XDRV_IPC *pIpcDrv,
   XDRV_UINT16 handle
);

/*
*****************************************************************************
** FUNCTION:   debugQueryIpcChannel
**
** PURPOSE:    Query DSP Channel for info of interest
**
** PARAMETERS: handle  - channel to query
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void debugQueryIpcChannel( XDRV_UINT16 handle );

/*
*****************************************************************************
** FUNCTION:   debugQueryAllIpcChannels
**
** PURPOSE:    Query all DSP Channels for info of interest
**
** PARAMETERS:
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void debugQueryAllIpcChannels( void );


/*
*****************************************************************************
** FUNCTION:   debugDspReadCnt
**
** PURPOSE:    To read the value of the DSP's background & interrupt counters.
**
** PARAMETERS: none.
**
** RETURNS:    0 on success
**
** NOTE:
*****************************************************************************
*/
int debugDspReadCnt( void );

/*
*****************************************************************************
** FUNCTION:   debugDspForceAssert
**
** PURPOSE:    Force DSP assert.
**
** PARAMETERS: none.
**
** RETURNS:    0 on success
**
** NOTE:
*****************************************************************************
*/
int debugDspForceAssert( void );

/*
*****************************************************************************
** FUNCTION:   debugDspLogAssertContext
**
** PURPOSE:    To log the state of the DSP register set and stack context. This
**             function is intented to be called after the DSP asserts.
**
** PARAMETERS: pDebugCom         (mod) Debug-com interface used to read DSP
**                                     regitser set and stack context.
**
** RETURNS:    Nothing.
**
** NOTE:       When the DSP asserts, it is assumed that it will push the contents
**             of it's register set onto the stack, and then place the value
**             of the stack pointer into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
int debugDspLogAssertContext( void  );

/*
*****************************************************************************
** FUNCTION:   debugDspGetAssertStackPtr
**
** PURPOSE:    Get DSP assert stack Ptr.
**
** PARAMETERS:
**
** RETURNS:    Assert stack pointer.
**
** NOTE:       When the DSP asserts, it overwrites the "doing fine" indicator
**             with the actual stack pointer. This routine can be used to
**             Check if the DSP is doing fine
*****************************************************************************
*/
XDRV_UINT16 debugDspGetAssertStackPtr( void  );

/*
*****************************************************************************
** FUNCTION:   debugDspGetRegs
**
** PURPOSE:    Get a snapshot of DSP registers.
**
** PARAMETERS: none.
**
** RETURNS:    0 on success
**
** NOTE:       When the DSP receives the getreg command, it places a copy
**             of its registers into an internal memory region, and then place
**             the address of that place into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
int debugDspGetRegs( void );

/*
*****************************************************************************
** FUNCTION:   debugDspDumpDataMem
**
** PURPOSE:    To log DSP data memory to the console.
**
** PARAMETERS: dspAddr        (in)  DSP data address to start log from.
**             amountToRead   (in)  Amount of data to dump.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void debugDspDumpDataMem( XDRV_UINT16 dspAddr, unsigned int amountToRead );

/*
*****************************************************************************
** FUNCTION:   debugApmStatusDisplay
**
** PURPOSE:    Query APM status information
**
** PARAMETERS:
**
** RETURNS:
**
*****************************************************************************
*/
void debugApmStatusDisplay( void );

#if __cplusplus
}
#endif


#endif   /* DEBUG_H */
