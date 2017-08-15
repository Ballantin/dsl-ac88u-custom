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
*    Filename: ipcSim.c
*
****************************************************************************
*    Description:
*
*     Implements the inter-processor communication (IPC) driver.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <ipcSim.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */

static unsigned WriteBuffer
(
   XDRV_IPC         *ipcDrv,
   XDRV_UINT16      *dp1,
   unsigned          len1,
   XDRV_UINT16      *dp2,
   unsigned          len2
);

static unsigned ReadBufferWithoutFree( XDRV_IPC *ipcDrv, XDRV_UINT16 **ppDestBuf );
static void FreeReadBuffers( XDRV_IPC *ipcDrv );
static void PrintIpcInfo( XDRV_IPC *ipcDrv, XDRV_IPC_PRINT_MODE mode );


/* ---- Private Variables ------------------------------------------------ */

/*
** IPC "meta-driver" interface
*/
static const XDRV_IPC_FUNCS gIpcDrvFuncs =
{
   ReadBufferWithoutFree,
   FreeReadBuffers,
   WriteBuffer,
   PrintIpcInfo
};


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   ipcSimInit
**
** PURPOSE:    To initialize the IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimInit( void )
{
}


/*
*****************************************************************************
** FUNCTION:   ipcSimDeinit
**
** PURPOSE:    To de-initialize the IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimDeinit( void )
{
}


/*
*****************************************************************************
** FUNCTION:   ipcSimOpen
**
** PURPOSE:    To create IPC driver.
**
** PARAMETERS: ipcDrv            (out) Created IPC driver object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimOpen( IPC_SIM_DRV *ipcDrv )
{

   ipcDrv->pDrvFuncs = &gIpcDrvFuncs;
}


/*
*****************************************************************************
** FUNCTION:   ipcSimClose
**
** PURPOSE:    To destroy a previously openned IPC driver.
**
** PARAMETERS: ipcDrv   (mod) IPC driver.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimClose( IPC_SIM_DRV *ipcDrv )
{
}


/*
*****************************************************************************
** FUNCTION:   WriteBuffer
**
** PURPOSE:    To write data to the DSP.
**
** PARAMETERS: ipcDrv   (mod) IPC driver.
**             dp1      (in)  Pointer to data1.
**             len1     (in)  Length of data1 in words.
**             dp2      (in)  Pointer to data2.
**             len2     (in)  Length of data2 in words.
**
** RETURNS:    Number of words written.
**
** NOTE:
*****************************************************************************
*/
static unsigned WriteBuffer
(
   XDRV_IPC         *ipcDrv,
   XDRV_UINT16      *dp1,
   unsigned          len1,
   XDRV_UINT16      *dp2,
   unsigned          len2
)
{
   (void) ipcDrv;
   (void) dp1;
   (void) dp2;

   return ( len1 + len2 );
}


/*
*****************************************************************************
** FUNCTION:   ReadBufferWithoutFree
**
** PURPOSE:    Reads data from DSP. The caller maintains ownership of the
**             buffer until FreeReadBuffers() is called.
**
** PARAMETERS: ipcDrv      (mod) IPC driver.
**             ppDestBuf   (out) Pointer to pointer to received data.
**
** RETURNS:    The number of words read.
**
** NOTE:
*****************************************************************************
*/
static unsigned ReadBufferWithoutFree( XDRV_IPC *ipcDrv, XDRV_UINT16 **ppDestBuf )
{
   (void) ipcDrv;
   (void) ppDestBuf;

   return ( 1 );
}


/*
*****************************************************************************
** FUNCTION:   FreeReadBuffers
**
** PURPOSE:    To free buffers that were previously read from the DSP with
**             calls to ReadBufferWithoutFree().
**
** PARAMETERS: ipcDrv      (mod) IPC driver.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void FreeReadBuffers( XDRV_IPC *ipcDrv )
{
   (void) ipcDrv;
}


/*
*****************************************************************************
** FUNCTION:   PrintIpcInfo
**
** PURPOSE:    Print stats and debug info related to a specific IPC driver.
**
** PARAMETERS: ipcDrv   (in)  IPC driver object.
**             mode     (in)  IPC print mode.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void PrintIpcInfo( XDRV_IPC *ipcDrv, XDRV_IPC_PRINT_MODE mode )
{
   (void) ipcDrv;
   (void) mode;
}
