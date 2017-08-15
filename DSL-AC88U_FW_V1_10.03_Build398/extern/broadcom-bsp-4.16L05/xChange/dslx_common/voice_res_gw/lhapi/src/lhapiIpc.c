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
*    Filename: lhapi_ipc.c
*
****************************************************************************
*    Description:
*
*     Implements the "Low-level HAPI" API that is required to interface
*     to LDX. Conceptually, LDX has multiple interface "levels". At the top
*     level, clients interface to HAPI - hausware API. This interface is used
*     to open/close/control LDX (voice) channels. In addition, in a distributed
*     LDX architecture, there is a "low-level" interface that allow for buffers
*     to be passed between the distributed processors. LDX provides a generic
*     API that must be implemented by clients (since LDX has no understanding
*     of how buffers are exchanged between processors).
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <hapi.h>
#include <hapi_rm.h>
#include <xdrvIpc.h>
#include <boardHalIpc.h>
#include <xchgAssert.h>
#include <hapigdef.h>
#include <vrgLog.h>
#include "lhapiExtension.h"
#include "lhapiPrivate.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Private Constants ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   lhapiIpcOpenChannel
**
** PURPOSE:    Open an IPC channel between the host and DSP.
**
** PARAMETERS: vhdType     (in)  VHD type for channel to be openned.
**             dspNum      (in)  DSP number
**             ipcInfo     (out) pointer to structure containing ipc info
**
** RETURNS:    Handle to the IPC channel
**
** NOTE:
*****************************************************************************
*/
void *lhapiIpcOpenChannel( HAPI_UINT16 vhdType, HAPI_UINT16 dspNum, HAPI_RM_OPEN_VHD_INFO *vhdInfo )
{
   BOARD_HAL_IPC_CHANNEL_TYPE    ipcChanType;
   void                         *ipcHandle = NULL;

   if( lhapiIsHauswareDistributed() )
   {
       switch ( vhdType )
       {
          case ( HAPIRESMAN ):
          {
             ipcChanType = BOARD_HAL_IPC_MASTER_CONTROL_CHANNEL;
          }
          break;

          case ( HAPI_LINE_ECAN_VHD ):
          {
             ipcChanType = BOARD_HAL_IPC_CONTROL_CHANNEL;
          }
          break;

          case ( HAPI_GATEWAY_VHD ):        /* Fall through... */
          case ( HAPI_CONFERENCE_VHD ):
          case ( HAPI_PSTN_VHD ):
          {
             ipcChanType = BOARD_HAL_IPC_DATA_CHANNEL;
          }
          break;

          default:
          {
             XCHG_ASSERT( 0 );
             return (0);
          }
          break;
       }

       /* Open the actual IPC channel (last parm not used) */
       ipcHandle = boardHalIpcCreateChannel( dspNum, ipcChanType, vhdInfo->usrHandle, (void *)vhdInfo->ipcInfo );
   }
   
   return ( ipcHandle );
}

/*
*****************************************************************************
** FUNCTION:   lhapiIpcCloseChannel
**
** PURPOSE:    Close an IPC channel between the host and DSP.
**
** PARAMETERS: ipcHandle   (in)  IPC handle
**
** RETURNS:    What am I supposed to return??? This isn't documented by hausware...
**
** NOTE:
*****************************************************************************
*/
void  lhapiIpcCloseChannel( void *ipcHandle )
{
   if( lhapiIsHauswareDistributed() )
   {
       XCHG_ASSERT( ipcHandle != NULL );

       boardHalIpcDestroyChannel( ipcHandle );
   }
}

/*
*****************************************************************************
** FUNCTION:   lhapiIpcReadNoFree
**
** PURPOSE:    Reads data from DSP. The caller maintains ownership of the
**             buffer until lhapiIpcFreeRead() is called.
**
** PARAMETERS: ldxHandle   (in)  LDX handle.
**             ppBuf       (out) Pointer to pointer to received data.
**
** RETURNS:    The number of words read.
**
** NOTE:
*****************************************************************************
*/
unsigned lhapiIpcReadNoFree( void *ipcHandle, HAPI_UINT16 **ppBuf )
{
   if( lhapiIsHauswareDistributed() )
   {
       XCHG_ASSERT( ipcHandle != NULL );

       return( xdrvIpcReadNoFree( (XDRV_IPC*)ipcHandle, ppBuf ) );
   }
   else
   {
       return 0;
   }
}


/*
*****************************************************************************
** FUNCTION:   lhapiIpcFreeRead
**
** PURPOSE:    To free buffers that were previously read from the DSP with
**             calls to lhapiIpcReadNoFree().
**
** PARAMETERS: ldxHandle   (in)  LDX handle.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void lhapiIpcFreeRead( void *ipcHandle )
{
   if( lhapiIsHauswareDistributed() )
   {
       XCHG_ASSERT( ipcHandle != NULL );

       xdrvIpcFreeRead( (XDRV_IPC*)ipcHandle );
   }
}


/*
*****************************************************************************
** FUNCTION:   lhapiIpcWrite
**
** PURPOSE:    To send data to the DSP.
**
** PARAMETERS: ldxHandle   (in)  LDX handle.
**             msg1        (in)  Pointer to data1.
**             msgLength1  (in)  Length of data1 in words.
**             msg2        (in)  Pointer to data2.
**             msgLendth2  (in)  Length of data2 in words.
**
** RETURNS:    Number of words written.
**
** NOTE:
*****************************************************************************
*/
unsigned lhapiIpcWrite
(
   void        *ipcHandle,
   VRG_UINT16  *msg1,
   unsigned    msgLength1,
   VRG_UINT16  *msg2,
   unsigned    msgLength2
)
{
   if( lhapiIsHauswareDistributed() )
   {
       XCHG_ASSERT( ipcHandle != NULL );

       return( xdrvIpcWrite( (XDRV_IPC*)ipcHandle, msg1, msgLength1,
                              msg2, msgLength2 ) );
   }
   else
   {
       return 0;
   }
}
