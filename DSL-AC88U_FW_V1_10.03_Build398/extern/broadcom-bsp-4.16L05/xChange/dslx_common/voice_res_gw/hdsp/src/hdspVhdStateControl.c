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
*    Filename: hdspVhdStateControl.c
*
****************************************************************************
*    Description:
*
*     This file implements the API that controls the state of the VHDs.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <hdspCfg.h>
#include <hapi.h>
#include <hapi_rm.h>
#include <hdspRm.h>
#include <hdspVhdStateControl.h>
#include <bosError.h>
#include <bosMutex.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define HDSP_VHD_STATE_CONTROL_ENABLED 1

#if HDSP_VHD_STATE_CONTROL_ENABLED

/* Size of the VHD state control block table */
#define HDSP_VHD_STATE_CONTROL_BLOCK_MAX_SIZE 25

/* Identifier to represent empty VHD state control table entry */
#define HDSP_VHD_STATE_CONTROL_VHD_HANDLE_NONE 0xDEAD

/* Timer increment, in seconds, for each VHD in state control block table */
#define HDSP_VHD_STATE_CONTROL_TIME_INCREMENT 4

/* VHD enable/disable states */
#define HDSP_VHD_STATE_CONTROL_VHD_ENABLE  1
#define HDSP_VHD_STATE_CONTROL_VHD_DISABLE 0

/* Stucture that represents an entry in the VHD state control table */
typedef struct HDSP_VHD_STATE_CONTROL_BLOCK
{
   VRG_UINT16 vhdHandle;   /* VHD handle */
   VRG_BOOL   isEnabled;   /* Flag to indicate if VHD is in enabled state */
   VRG_UINT16 timer;       /* VHD timer. VHD must be disabled when timer is 0 */
   BOS_MUTEX  lock;        /* Lock handle for the control block */
} HDSP_VHD_STATE_CONTROL_BLOCK;

/* Structure holding information about the PSTN configuration of the system */
typedef struct HDSP_VHD_STATE_CONTROL_PSTN_INFO
{
   VRG_UINT16 pstnVhdHandle;   /* PSTN VHD handle (if any)*/
   VRG_UINT16 lineVhdHandle;   /* FXO Line VHD handle (if any) */
} HDSP_VHD_STATE_CONTROL_PSTN_INFO;

#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */

/* ---- Private Variables ------------------------------------------------ */

#if HDSP_VHD_STATE_CONTROL_ENABLED
/* A VHD state control information stored is stored in this table */
static HDSP_VHD_STATE_CONTROL_BLOCK hdspVhdStateCtlTable[HDSP_VHD_STATE_CONTROL_BLOCK_MAX_SIZE];

/* All the FXO related VHD handles stored in this structure */
static HDSP_VHD_STATE_CONTROL_PSTN_INFO pstnInfoBlock;
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */

/* ---- Private Function Prototypes -------------------------------------- */

#if HDSP_VHD_STATE_CONTROL_ENABLED
/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlGetStateCtlBlock
*
*  PURPOSE:    Given a VHD handle, returns the corresponding entry in the VHD
*              state control table.
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*
*  RETURNS:    Pointer to HDSP_VHD_STATE_CONTROL_BLOCK on success. NULL on failure.
*
*  NOTE:
*****************************************************************************/
static HDSP_VHD_STATE_CONTROL_BLOCK* hdspVhdStateControlGetStateCtlBlock( VRG_UINT16 vhdHandle )
{
   int i;

   for(i = 0; i < HDSP_VHD_STATE_CONTROL_BLOCK_MAX_SIZE; i++)
   {
      /* Found the entry in VHD state control table */
      if ( hdspVhdStateCtlTable[i].vhdHandle == vhdHandle )
      {
         return &hdspVhdStateCtlTable[i];
      }
   }
     
   return NULL;
}
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */

/* ---- Functions -------------------------------------------------------- */

/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlInit
*
*  PURPOSE:    Initialize the HDSP VHD state control module.
*
*  PARAMETERS: None
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
void hdspVhdStateControlInit( void )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   int i;

   /* Clear the VHD state control table */
   for(i = 0; i < HDSP_VHD_STATE_CONTROL_BLOCK_MAX_SIZE; i++)
   {
      hdspVhdStateCtlTable[i].vhdHandle = HDSP_VHD_STATE_CONTROL_VHD_HANDLE_NONE;
      hdspVhdStateCtlTable[i].isEnabled = VRG_FALSE;
      hdspVhdStateCtlTable[i].timer = 0;
   }
   
   /* Clear the PSTN state information block */
   hdspVhdStateControlSetPstnInfoBlock( HDSP_VHD_STATE_CONTROL_VHD_HANDLE_NONE, 
                                        HDSP_VHD_STATE_CONTROL_VHD_HANDLE_NONE );
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlRegisterVhd
*
*  PURPOSE:    Register a VHD handle into the VHD state control block table.
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspVhdStateControlRegisterVhd( VRG_UINT16 vhdHandle )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   int i;
   BOS_STATUS status;

   for(i = 0; i < HDSP_VHD_STATE_CONTROL_BLOCK_MAX_SIZE; i++)
   {
      /* Found unused entry in VHD state control table */
      if ( hdspVhdStateCtlTable[i].vhdHandle == HDSP_VHD_STATE_CONTROL_VHD_HANDLE_NONE )
      {
         status = bosMutexCreate( "HdspStateCtlLck", &(hdspVhdStateCtlTable[i].lock) );
         if ( status != BOS_STATUS_OK )
         {
            return HAPIFAILURE;
         }
         
         hdspVhdStateCtlTable[i].vhdHandle = vhdHandle;
         hdspVhdStateCtlTable[i].isEnabled = VRG_TRUE;
         hdspVhdStateCtlTable[i].timer = HDSP_VHD_STATE_CONTROL_TIME_INCREMENT;   
         return HAPISUCCESS;
      }
   }
   
   /* Ran out of table entries so return failure */
   return HAPIFAILURE;
#else
   /* Force function to return success */
   return  HAPISUCCESS;
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */   
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlUnregisterVhd
*
*  PURPOSE:    Unregister a VHD handle into the VHD state control block table.
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspVhdStateControlUnregisterVhd( VRG_UINT16 vhdHandle )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   HDSP_VHD_STATE_CONTROL_BLOCK *vhdStateCtlBlock;
   BOS_STATUS status;
   
   vhdStateCtlBlock = hdspVhdStateControlGetStateCtlBlock( vhdHandle );
   if ( vhdStateCtlBlock == NULL )
   {
      /* No entry in state control table. Return failure */
      return HAPIFAILURE;
   }

   /* Found the entry in VHD state control table */
   status = bosMutexDestroy( &vhdStateCtlBlock->lock );
   if ( status != BOS_STATUS_OK )
   {
      return HAPIFAILURE;
   }
         
   vhdStateCtlBlock->vhdHandle = HDSP_VHD_STATE_CONTROL_VHD_HANDLE_NONE;
   vhdStateCtlBlock->isEnabled = VRG_FALSE;
   vhdStateCtlBlock->timer = 0;
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */
 
   return HAPISUCCESS;
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlEnableVhd
*
*  PURPOSE:    Enables the specified VHD is all necessary conditions are met.
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspVhdStateControlEnableVhd( VRG_UINT16 vhdHandle )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   VRG_SINT16 result;
   HDSP_VHD_STATE_CONTROL_BLOCK *vhdStateCtlBlock;

   if ( vhdHandle == HAPIRESMAN                  || /* Resource manager handle */
        vhdHandle == HAPI_HOST_RESMAN            || /* HOST resource manager handle */
        vhdHandle == pstnInfoBlock.pstnVhdHandle || /* PSTN VHD handle */
        vhdHandle == pstnInfoBlock.lineVhdHandle )  /* FXO line VHD handle */
   {
      /* Do nothing, simply return success */
      return HAPISUCCESS;
   }

   vhdStateCtlBlock = hdspVhdStateControlGetStateCtlBlock( vhdHandle );
   if ( vhdStateCtlBlock == NULL )
   {
      /* No entry in state control table. Return failure */
      return HAPIFAILURE;
   }

   /* Acquire lock */
   bosMutexAcquire( &vhdStateCtlBlock->lock );

   /* Issue command to enable VHD only if VHD is disabled */
   if ( vhdStateCtlBlock->isEnabled == VRG_FALSE )
   {
      result = hdspRmSetVhdProcessing( vhdHandle, 
                  (HAPI_RM_DIRECTION_INGRESS | HAPI_RM_DIRECTION_EGRESS),
                   HDSP_VHD_STATE_CONTROL_VHD_ENABLE );
      if ( result != HAPISUCCESS )
      {
         /* Release lock */
         bosMutexRelease( &vhdStateCtlBlock->lock );
         return result;
      }                

      /* Set VHD enable flag */
      vhdStateCtlBlock->isEnabled = VRG_TRUE; 
   }
   
   /* Reset the VHD timer increment */
   vhdStateCtlBlock->timer = HDSP_VHD_STATE_CONTROL_TIME_INCREMENT;

   /* Release lock */
   bosMutexRelease( &vhdStateCtlBlock->lock );
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */

   return HAPISUCCESS;     
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlDisableVhd
*
*  PURPOSE:    Disables the specified VHD is all necessary conditions are met.
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspVhdStateControlDisableVhd( VRG_UINT16 vhdHandle )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   VRG_SINT16 result;
   HDSP_VHD_STATE_CONTROL_BLOCK *vhdStateCtlBlock;

   if ( vhdHandle == HAPIRESMAN                  || /* Resource manager handle */
        vhdHandle == HAPI_HOST_RESMAN            || /* HOST resource manager handle */
        vhdHandle == pstnInfoBlock.pstnVhdHandle || /* PSTN VHD handle */
        vhdHandle == pstnInfoBlock.lineVhdHandle )  /* FXO line VHD handle */
   {
      /* Do nothing, simply return success */
      return HAPISUCCESS;
   }

   vhdStateCtlBlock = hdspVhdStateControlGetStateCtlBlock( vhdHandle );
   if ( vhdStateCtlBlock == NULL )
   {
      /* No entry in state control table. Return failure */
      return HAPIFAILURE;
   }

   /* Acquire lock */
   bosMutexAcquire( &vhdStateCtlBlock->lock );

   if ( vhdStateCtlBlock->isEnabled == VRG_TRUE )
   {
      /* Issue command to disable VHD only if VHD is enabled */
      result = hdspRmSetVhdProcessing( vhdHandle, 
                  (HAPI_RM_DIRECTION_INGRESS | HAPI_RM_DIRECTION_EGRESS),
                   HDSP_VHD_STATE_CONTROL_VHD_DISABLE );
      if ( result != HAPISUCCESS )
      {
         /* Release lock */
         bosMutexRelease( &vhdStateCtlBlock->lock );
         return result;
      }                

      /* Clear VHD enable flag */
      vhdStateCtlBlock->isEnabled = VRG_FALSE;
 
      /* Clear the VHD timer */
      vhdStateCtlBlock->timer = 0;
   }
   
   /* Release lock */
   bosMutexRelease( &vhdStateCtlBlock->lock );
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */
 
   return HAPISUCCESS;     
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlSetPstnInfoBlock
*
*  PURPOSE:    Populates the PSTN information block.
*
*  PARAMETERS: pstnVhdHandle      (in)  PSTN VHD handle.
*              lineVhdHandle      (out) FXO Line VHD handle.
*
*  RETURNS:    None
*
*  NOTE:
*****************************************************************************/
void hdspVhdStateControlSetPstnInfoBlock( VRG_UINT16 pstnVhdHandle, VRG_UINT16 lineVhdHandle )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   pstnInfoBlock.pstnVhdHandle = pstnVhdHandle;
   pstnInfoBlock.lineVhdHandle = lineVhdHandle;
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlIsVhdEnabled
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*
*  RETURNS:    VRG_TRUE if VHD is enabled and VRG_FALSE otherwise
*
*  NOTE:
*****************************************************************************/
VRG_BOOL hdspVhdStateControlIsVhdEnabled( VRG_UINT16 vhdHandle )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   HDSP_VHD_STATE_CONTROL_BLOCK *vhdStateCtlBlock;
   VRG_BOOL vhdStatus;

   if ( vhdHandle == HAPIRESMAN                  || /* Resource manager handle */
        vhdHandle == HAPI_HOST_RESMAN            || /* HOST resource manager handle */
        vhdHandle == pstnInfoBlock.pstnVhdHandle || /* PSTN VHD handle */
        vhdHandle == pstnInfoBlock.lineVhdHandle )  /* FXO line VHD handle */
   {
      /* 
      ** We do not want to disable these VHD. Return VRG_FALSE so the caller
      ** thinks the VHDs are already disabled and never tries to disable them.
      */
      return VRG_FALSE;
   }
   
   vhdStateCtlBlock = hdspVhdStateControlGetStateCtlBlock( vhdHandle );
   if ( vhdStateCtlBlock == NULL )
   {
      /* No entry in state control table. Assert */
      XCHG_ASSERT(0);
   }
   
   /* Acquire lock */
   bosMutexAcquire( &vhdStateCtlBlock->lock );
   
   vhdStatus = vhdStateCtlBlock->isEnabled;

   /* Release lock */
   bosMutexRelease( &vhdStateCtlBlock->lock );
   
   return vhdStatus;
#else
   /* Force function to return false */
   return VRG_FALSE;
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */   
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlAdjustTimeIncrement
*
*  PURPOSE:    Adjusts the time increment of the specified VHD's entry in the
*              control table.
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*              value          (in)  value by which to adjust the increment.
*              updatedTimer   (out) updated timer value after adjustment.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspVhdStateControlAdjustTimeIncrement( VRG_UINT16 vhdHandle, VRG_UINT16 value, VRG_UINT16 *updatedTimer )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   HDSP_VHD_STATE_CONTROL_BLOCK *vhdStateCtlBlock;
   (void)value;

   if ( vhdHandle == HAPIRESMAN                  || /* Resource manager handle */
        vhdHandle == HAPI_HOST_RESMAN            || /* HOST resource manager handle */
        vhdHandle == pstnInfoBlock.pstnVhdHandle || /* PSTN VHD handle */
        vhdHandle == pstnInfoBlock.lineVhdHandle )  /* FXO line VHD handle */
   {
      /* Do nothing, simply return success */
      return HAPISUCCESS;
   }

   vhdStateCtlBlock = hdspVhdStateControlGetStateCtlBlock( vhdHandle );
   if ( vhdStateCtlBlock == NULL )
   {
      /* No entry in state control table. Return failure */
      return HAPIFAILURE;
   }

   /* Acquire lock */
   bosMutexAcquire( &vhdStateCtlBlock->lock );

   /* Adjust time increment only if the value is non-zero */
   if ( vhdStateCtlBlock->timer > 0 )
   {
      if ( vhdStateCtlBlock->timer >= value )
      {
         vhdStateCtlBlock->timer -= value;
      }
      else
      {
         vhdStateCtlBlock->timer = 0;
      }               
   }

   *updatedTimer = vhdStateCtlBlock->timer;

   /* Release lock */
   bosMutexRelease( &vhdStateCtlBlock->lock );
#endif /* HDSP_VHD_STATE_CONTROL_ENABLED */
 
   return HAPISUCCESS;     
}


/*****************************************************************************
*  FUNCTION:   hdspVhdStateControlIsVhdRegistered
*
*  PURPOSE:    Given a VHD handle, checks if a VHD is registered in the state 
*              control table.             
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*
*  RETURNS:    VRG_TRUE if VHD is registered and VRG_FALSE otherwise
*
*  NOTE:
*****************************************************************************/
VRG_BOOL hdspVhdStateControlIsVhdRegistered( VRG_UINT16 vhdHandle )
{
#if HDSP_VHD_STATE_CONTROL_ENABLED
   return ( hdspVhdStateControlGetStateCtlBlock( vhdHandle ) == NULL ) ? VRG_FALSE : VRG_TRUE;
#else
   /* Force function to return false */
   return VRG_FALSE;
#endif
}

