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
*    Filename: xdrvItpc.h
*
****************************************************************************
*    Description:
*
*     Interface for inter-thread processor communication (ITPC) module.
*     This provides an interrupt based communication layer between the two
*     cores contained within a single Broadcom CMT (concurrent
*     multi-threaded) processor.
*
*     The API allows users to register handlers (i.e. ISRs) for each type
*     of command that can be sent between cores (thread-processors). The
*     handler will be invoked when a command is sent from one core
*     to another. A response may also be returned to the sending core.
*
*     The API supports both asynchronous and synchronous commands. Asynchronous
*     commands are sent without blocking for an acknowledgement
*     of successful completion of the command by the other thread-processor,
*     or response data. Synchronous commands block until the other thread
*     processor has acknowledged reception of the command (and optionally
*     provided a response). This will result in a serialization of synchronous
*     command requests.
*
*     The ITPC module is simply a transport layer. It does not
*     understand the commands-types, or contents of the data structures that
*     are sent between the thread-processors. These are application specific.
*
*     As of June 2005, Broadcom CMT cores are only used with the MIPS
*     architecture. This API is intended to be architecture/chip independent.
*     However, it may need to be tweaked to meet the requirements of future
*     CMT cores. For example, it is currently assumed that thread-processors
*     will signal each other via software interrupts. Future CMT cores
*     may use a different mechanism. This, amoung other things, may need to
*     be further abstracted.
*
****************************************************************************/

#ifndef XDRV_ITPC_H
#define XDRV_ITPC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvIntCtrl.h>
#include <xdrvTypes.h>
#include <xdrvSpinLock.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Maximum number of addresses printed out when stack dump is provided by the DSP */
#define XDRV_ITPC_DSP_STACK_PTR_MAX          30

/*
** Id's of command-types that can be sent between thread-processors. Note that
** the definition of the command-types and associated data structures are
** outside the scope of the ITPC module. This module does not understand
** the types of commands or data content transmitted. It's simply a
** transport layer.
*/
typedef enum XDRV_ITPC_CMD_ID
{
   XDRV_ITCP_START_CMD_ID = 0,

   /* Commands are muxed off a software interrupt, and we use a 32-bit status
   ** register bit-field to indicate pending commands. Therefore, at most, we
   ** can support 32 command types. */
   XDRV_ITCP_MAX_CMD_ID   = 31

} XDRV_ITPC_CMD_ID;


/*
** Command information that will be passed between thread processors.
*/
typedef void* XDRV_ITPC_DATA;

typedef struct XDRV_ITPC_CMD
{
   /* Type of command sent between thread-processors. */
   XDRV_ITPC_CMD_ID          cmdId;

   /* Optional data associated with command. Only supported for synchronous cmds. */
   XDRV_ITPC_DATA           *cmdData;

   /* Optional response data. Only supported for synchronous cmds. */
   XDRV_ITPC_DATA           *rspData;

} XDRV_ITPC_CMD;



/* Status register bit-field that indicates which commands are pending. */
typedef unsigned int XDRV_ITPC_STATUS_REG;

/*
** Shared memory data structures used to send commands between thread-processors.
*/
typedef struct XDRV_ITPC_SHARED_MEM_DATA
{
   /* Data associated with synchronous commands sent to other thread-processor. */
   volatile XDRV_ITPC_CMD         *syncSendCmd;

   /* Data associated with synchronous commands received from other thread-processor. */
   volatile XDRV_ITPC_CMD         *syncRecvCmd;

   /* Bit-field indicating which commands have been sent by this thread-processor,
   ** and are to be serviced by the other thread processor. */
   volatile XDRV_ITPC_STATUS_REG  *sendStatusReg;

   /* Bit-field indicating which commands have been sent by the other
   ** thread-processor, and are to be serviced by this thread processor. */
   volatile XDRV_ITPC_STATUS_REG  *recvStatusReg;

} XDRV_ITPC_SHARED_MEM_DATA;

/* Stack Pointer. */
typedef unsigned int XDRV_ITPC_DSP_STACK_PTR;

/*
** Shared memory data structures used to record DSP stack dump.
*/
typedef struct XDRV_ITPC_DSP_STACK_DUMP
{
   /* Length of stack dump */
   volatile unsigned int  stackTraceCount;

   /* Stack dump ptr */
   volatile XDRV_ITPC_DSP_STACK_PTR  *stackPtr;

} XDRV_ITPC_DSP_STACK_DUMP;

/*
** Parameter that is passed back to user registered command handler.
*/
typedef void* XDRV_ITPC_CMD_HANDLER_PARM;

/*
** Command handler prototype that may be registered for each command type.
**
** Return 0 for success, else error code.
*/
typedef unsigned int ( *XDRV_ITPC_CMD_HANDLER )
(
   volatile XDRV_ITPC_CMD       *cmd,
   XDRV_ITPC_CMD_HANDLER_PARM    parm
);


/*
** The inter-thread processor communication module supports synchronous commands
** that need to be serialized in order to prevent concurrent access to shared
** memory data structures.
**
** It is up to the caller to create the appropriate synchronization object, and
** provide callback functions that can be used to "acquire" and "release" the
** mutex. This is necessary since this module will be used on multiple
** processors, each of which may be running drastically different applications.
** We can't assume support for an operating system mutex primitive exists,
** for example.
**
** Some potential mutex synchronization primitives are:
**    BOS         - Use Broadcom Operating System mutex primitive.
**
**    Interrupts  - Use interrupt disable/enable. Useful on systems that
**                  don't have an operating system.
**
**    None        - Don't use any mutex primitive. This can be used on systems
**                  where it is known that all synchronous command requests
**                  are issued from the same (thread) context, or if all
**                  commands are issued from threads of the same priority
**                  (that can't preempt one another).
*/
typedef void* XDRV_ITPC_MUTEX_PARM;
typedef unsigned int ( *XDRV_ITPC_MUTEX_FUNCP )( XDRV_ITPC_MUTEX_PARM parm );

typedef struct XDRV_ITPC_MUTEX
{
   /* Callbacks used to acquire and release the mutex. Return 0 for success. */
   XDRV_ITPC_MUTEX_FUNCP   acquireFunc;
   XDRV_ITPC_MUTEX_FUNCP   releaseFunc;

   /* Mutex instance data. */
   XDRV_ITPC_MUTEX_PARM    data;
} XDRV_ITPC_MUTEX;



/*
** State for each ITPC driver instance.
*/
typedef struct XDRV_ITPC
{
   /*
   ** User registered command handlers.
   */
   XDRV_ITPC_CMD_HANDLER      commandHandler    [ XDRV_ITCP_MAX_CMD_ID + 1 ];
   XDRV_ITPC_CMD_HANDLER_PARM commandHandlerData[ XDRV_ITCP_MAX_CMD_ID + 1 ];


   /*
   ** Signalling software interrupt id used to notify thread processors. Setting this
   ** software interrupt on one thread processor will trigger an interrupt on the
   ** other thread processor.
   */
   unsigned int               itpcInterruptId;


   /*
   ** Shared memory locations used to send commands and responses.
   */
   XDRV_ITPC_SHARED_MEM_DATA *sharedMemData;


   /*
   ** Spin-lock synchronization primitive used to prevent concurrent accesses
   ** to shared data structures.
   */
   XDRV_SPIN_LOCK            *spinLock;


   /*
   ** Mutex primitive used to serialize synchronous command requests.
   */
   XDRV_ITPC_MUTEX           *mutex;

} XDRV_ITPC;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvItpcInit
**
** PURPOSE:    To initialize the inter-thread processor communcation module.
**
** PARAMETERS: itpcInterruptId   (in)  Software interrupt id used for thread
**                                     processor communication. Setting this
**                                     software interrupt on one thread processor
**                                     will trigger an interrupt on the other
**                                     thread processor.
**             sharedMemData     (in)  Data structures that shared between
**                                     thread-processors.
**             spinLock          (in)  Spin-lock used to provide concurrency
**                                     protection between shared data structs.
**             mutex             (in)  Serialization mutex for synchronous cmds.
**                                     Optionally NULL if no mutex required.
**             itpcDrv           (out) ITPC instance memory state.
**
** RETURNS:    Nothing.
**
** NOTE: It is assumed that the appropriate registers have been configured
**       to route the software interrupt from one thread processor to the
**       other thread processor prior to calling this function.
*****************************************************************************
*/
void xdrvItpcInit
(
   XDRV_INT_CTRL_INTERRUPT    itpcInterruptId,
   XDRV_ITPC_SHARED_MEM_DATA *sharedMemData,
   XDRV_SPIN_LOCK            *spinLock,
   XDRV_ITPC_MUTEX           *mutex,
   XDRV_ITPC                 *itpcDrv
);


/*
*****************************************************************************
** FUNCTION:   xdrvItpcDeinit
**
** PURPOSE:    To de-initialize the inter-thread communication module.
**
** PARAMETERS: itpcDrv   (mod) ITPC instance memory state.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvItpcDeinit( XDRV_ITPC *itpcDrv );


/*
*****************************************************************************
** FUNCTION:   xdrvItpcRegisterHandler
**
** PURPOSE:    Allows users to register a handler (i.e. ISR) for each ITPC
**             command type. The handler will be invoked when a command is
**             sent from one thread processor to another.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Install handler for this command.
**             handler     (in)  Handler to invoke when command is sent.
**             parm        (in)  Function parameter for handler.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvItpcRegisterHandler
(
   XDRV_ITPC                    *itpcDrv,
   XDRV_ITPC_CMD_ID              cmdId,
   XDRV_ITPC_CMD_HANDLER         handler,
   XDRV_ITPC_CMD_HANDLER_PARM    parm
);


/*
*****************************************************************************
** FUNCTION:   xdrvItpcUnregiserHandler
**
** PURPOSE:    To un-install a command handler.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Uninstall handler for this command.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvItpcUnregisterHandler
(
   XDRV_ITPC        *itpcDrv,
   XDRV_ITPC_CMD_ID  cmdId
);


/*
*****************************************************************************
** FUNCTION:   xdrvItpcSendSyncCommand
**
** PURPOSE:    To send a synchronous command to the other thread processor.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Command to send to other thread processor.
**             cmdData     (in)  Command specific data (optionally NULL).
**             rspData     (out) Response data (optionally NULL if no resp required).
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:       This function will block until the other thread processor
**             has acknowledged the command (and optionally provided a response).
**             This will result in a serialization of sync command requests.
*****************************************************************************
*/
XDRV_STATUS xdrvItpcSendSyncCommand
(
   XDRV_ITPC           *itpcDrv,
   XDRV_ITPC_CMD_ID     cmdId,
   XDRV_ITPC_DATA       cmdData,
   XDRV_ITPC_DATA       rspData
);



/*
*****************************************************************************
** FUNCTION:   xdrvItpcSendAsyncCommand
**
** PURPOSE:    To send an asynchronous command to the other thread processor.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Command to send to other thread processor.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:       This function returns immediately. It does NOT wait for
**             the other thread processor to ack the command.
*****************************************************************************
*/
XDRV_STATUS xdrvItpcSendAsyncCommand
(
   XDRV_ITPC        *itpcDrv,
   XDRV_ITPC_CMD_ID  cmdId
);





#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_ITPC_H  */
