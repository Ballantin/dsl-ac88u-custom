/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: debug.c
*
****************************************************************************
*    Description:
*
*     This file contains functions related to the Debug Module.
*     Debug module is used to retrieve info of interest from the DSP upon
*     failure conditions (assert, command timeouts, etc.). For now this is
*     going to dump out the entire IPC shared memory, but may be enhanced
*     in the future to dump internal DSP memory, registers, etc.
*
****************************************************************************/

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <string.h>
#include "debug.h"



/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */


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
void debugInit( DBGCOM *pDbgCom, XDRV_APM *pApmDrv )
{
}



/*
*****************************************************************************
** FUNCTION:   debugDeinit
**
** PURPOSE:    Shutdown the Debug module
**
** PARAMETERS: pDrv       - pointer to the Debug module info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void debugDeinit( void )
{
}

/*
*****************************************************************************
** FUNCTION:   debugRegisterChannel
**
** PURPOSE:    Register IPC channel with debug module
**
** PARAMETERS: pDrv    - pointer to the Debug module info structure
**             pIpcDrv - IPC channel to register
**
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int debugRegisterChannel( XDRV_IPC *pIpcDrv, XDRV_UINT16 handle )
{
   return ( 0 );
}

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
void debugQueryIpcChannel( XDRV_UINT16 handle)
{
}

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
void debugApmStatusDisplay( void )
{
}

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
void debugQueryAllIpcChannels( void )
{
}

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
int debugDspReadCnt( void )
{
   return ( 0 );
}

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
int debugDspForceAssert( void )
{
   return ( 0 );
}

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
int debugDspGetRegs( void )
{
   return ( 0 );
}

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
XDRV_UINT16 debugDspGetAssertStackPtr( void  )
{
   return ( 0 );
}

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
** RETURNS:    0 on success.
**
** NOTE:       When the DSP asserts, it is assumed that it will push the contents
**             of it's register set onto the stack, and then place the value
**             of the stack pointer into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
int debugDspLogAssertContext( void )
{
   return ( 0 );
}


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
void debugDspDumpDataMem( XDRV_UINT16 dspAddr, unsigned int amountToRead )
{
}
