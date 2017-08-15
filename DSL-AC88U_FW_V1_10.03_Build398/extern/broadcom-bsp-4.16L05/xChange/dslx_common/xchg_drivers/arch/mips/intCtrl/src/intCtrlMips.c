/****************************************************************************
*  
*  Copyright (c) 2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.  
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: intCtrlMips.c
*
****************************************************************************
*    Description:
*
*     Implements MIPS interrupt control module.
*
*     This module allows clients to register interrupt service routines, for
*     each interrupt source supported by a specific MIPS core, including
*     individual peripheral interrupts.
*
*     User registered interrupt service routines are high level (C language)
*     functions. Low-level, architecture specific, processor context saves
*     will be performed prior to invoking registered handlers. Like-wise,
*     a context restore will be performed after the registered handler returns.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvIntCtrl.h>
#include <intCtrlMips.h>
#include <exceptionMips.h>
#include <stddef.h>
#include <xdrvGlobalInterrupt.h>
#include <xchgAssert.h>
#include <string.h>
#include "intCtrlRegsMips.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Broadcom MIPS processors typically have 32 peripheral interrupt sources
** multiplexed onto a single "external" MIPS core interrupt. */
#define NUM_PERIPHERAL_INTERRUPTS   32


/*
** Map an interrupt id, to a 0-indexed external interrupt number. Interrupt
** id's 0 and 1 are software interrupts, and 7 is the timer interrupt. 2-6
** are external interrupts from the perspective of the MIPS core. One (or
** more) of the "external" MIPS core interrupts will be used as a internal
** peripheral interrupt (gPeripheralInterruptId); typically, interrupt id 2.
** The other interrupts are true, honest to god, external interrupts. For
** simplicity, it is assumed that all id's greater than the peripheral interrupt
** (and less than the timer interrupt) are true external interrupts.
*/
#define MAP_INT_ID_TO_EXTERNAL_INDEX( id ) ( (id) - gPeripheralInterruptId - 1 )


/*
** Indicates if the specified interrupt id is an external interrupt. External
** interrupts refers to real external interrupts (not internal peripheral
** interrupts that are "external" from the perspective of the MIPS core).
*/
#define IS_EXTERNAL_INT_ID( id ) \
   ( ((id) > gPeripheralInterruptId) && ((id) <= INT_CTRL_MIPS_CORE_EXT_INTERRUPT_4))



/*
** Registers that control interrupts, and are specific to Broadcom MIPS
** processors. These include:
**       - Peripheral interrupt mask register.
**       - Peripheral interrupt status register.
**       - External interrupt configuration register.
**
** I need to declare these as pointers because they are not contiguous in
** memory for some chips.
*/
typedef struct BRCM_MIPS_INTERRUPT_CONTROL
{
   volatile XDRV_UINT32   *intEnableMask;

   volatile XDRV_UINT32   *intStatus;

   /* Upper 8 bits RSVD. Next 4 bits enable level detection. Lower 4 bits set
   ** disable edge sensitivity */
   volatile XDRV_UINT16   *extEdgeInSense;

   /* Upper 4 bits enable corresponding ExtIrq, Lower 4 bits clr IRQ */
   volatile XDRV_UINT8    *extIrqMskandDetClr;

   /* Upper 4 bits are current status of input, lower bits define edge
    * sensitivity.  If high, IRQ on rising edge. If low, IRQ on falling edge. */
   volatile XDRV_UINT8    *extIrqStatEdgeConfig;

} BRCM_MIPS_INTERRUPT_CONTROL;



/* ---- Private Variables ------------------------------------------------ */

/* Standard MIPS core interrupt service routines. */
static XDRV_INT_CTRL_ISR         gInterruptHandler[ INT_CTRL_MIPS_NUM_INTERRUPTS ];
static XDRV_INT_CTRL_ISR_PARM    gInterruptData[ INT_CTRL_MIPS_NUM_INTERRUPTS ];


/* Broadcom peripheral interrupt service routines. */
static XDRV_INT_CTRL_ISR         gPeripheralInterruptHandler[ NUM_PERIPHERAL_INTERRUPTS ];
static XDRV_INT_CTRL_ISR_PARM    gPeripheralInterruptData[ NUM_PERIPHERAL_INTERRUPTS ];


/* Broadcom specific interrupt control registers. */
static BRCM_MIPS_INTERRUPT_CONTROL  gBrcmMipsCtrl;


/* Interrupt id of peripheral interrupt. On most chips, this is
** INT_CTRL_MIPS_CORE_EXT_INTERRUPT_0. However, some chips (e.g. dual-cores) support
** redundant peripheral interrupt controllers; and therefore, multiple peripheral
** interrupt ids. */
static INT_CTRL_MIPS_INTERRUPT_ID   gPeripheralInterruptId;



/* ---- Private Function Prototypes -------------------------------------- */

static unsigned int SoftwareIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
);

static unsigned int ExtIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
);

static unsigned int TimerIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
);

static unsigned int PeriphIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
);

static void MapLowLevelHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            isrFunc,
   unsigned int            isrParm
);


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   intCtrlInitMips
**
** PURPOSE:    Initialize MIPS interrupt control driver.
**
** PARAMETERS: periphIntId    (in)  Interrupt id of peripheral interrupt. On most
**                                  chips, this is INT_CTRL_MIPS_CORE_EXT_INTERRUPT_0.
**             intEnableMask  (in)  Peripheral interrupt mask register.
**             intState       (in)  Peripheral interrupt status register.
**             extIntConfig   (in)  External interrupt configuration register.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that interrupts (IE bit of status register) are
**             initially disabled prior to calling this function. They will
**             be enabled as part of this function.
*****************************************************************************
*/
void intCtrlInitMips
(
   INT_CTRL_MIPS_INTERRUPT_ID    periphIntId,
   volatile XDRV_UINT32         *intEnableMask,
   volatile XDRV_UINT32         *intStatus,
   volatile XDRV_UINT32         *extIntConfig
)
{
   unsigned int   intId;

   /*
   ** Default user registered interrupt service routines.
   */
   memset( gInterruptHandler, 0, sizeof( gInterruptHandler ) );
   memset( gInterruptData,    0, sizeof( gInterruptData ) );

   memset( gPeripheralInterruptHandler, 0, sizeof( gPeripheralInterruptHandler ) );
   memset( gPeripheralInterruptData,    0, sizeof( gPeripheralInterruptData ) );


   /*
   ** Store chip specific state.
   */
   gPeripheralInterruptId              = periphIntId;
   gBrcmMipsCtrl.intEnableMask         = intEnableMask;
   gBrcmMipsCtrl.intStatus             = intStatus;
   gBrcmMipsCtrl.extEdgeInSense        = (volatile XDRV_UINT16 *) extIntConfig;
   gBrcmMipsCtrl.extIrqMskandDetClr    = (volatile XDRV_UINT8 *) &gBrcmMipsCtrl.extEdgeInSense[1];
   gBrcmMipsCtrl.extIrqStatEdgeConfig  = &gBrcmMipsCtrl.extIrqMskandDetClr[1];



   /*
   ** Install internal interrupt service routines. These are used to bridge
   ** the low-level interrupt handler with the user registered interrupt handler.
   */

   /* Timer interrupt (interrupt id 7). */
   MapLowLevelHandler( INT_CTRL_MIPS_TIMER_INTERRUPT, (unsigned int)TimerIntHandler,   0 );

   /* Real external interrupts. (Start at interrupt id 6, and work down to the
   ** peripheral interrupt. */
   for ( intId = INT_CTRL_MIPS_CORE_EXT_INTERRUPT_4; intId > gPeripheralInterruptId; intId-- )
   {
      MapLowLevelHandler( intId, (unsigned int)ExtIntHandler, 0 );
   }

   /* Peripheral interrupt. */
   MapLowLevelHandler( gPeripheralInterruptId, (unsigned int)PeriphIntHandler, 0 );

   /* Software interrupts (interrupt id's 0 and 1). */
   MapLowLevelHandler( INT_CTRL_MIPS_SW_INTERRUPT_0, (unsigned int)SoftwareIntHandler, 0 );
   MapLowLevelHandler( INT_CTRL_MIPS_SW_INTERRUPT_1, (unsigned int)SoftwareIntHandler, 0 );




   /*
   ** Enable the master peripheral interrupt (in status register). However,
   ** disable the individual peripheral interrupt sources. (Both must be enabled
   ** for an interrupt to be serviced). Individual peripheral interrupts need
   ** to be enabled at the peripheral source level via the
   ** xdrvIntCtrlEnableInterrupt() API. Similarly, other interrupts
   ** (external, software, timer) need to be explicitly enabled via
   ** xdrvIntCtrlEnableInterrupt().
   */
   *intEnableMask = 0;
   intCtrlUnmaskMips( gPeripheralInterruptId );


   /*
   ** Enable global interrupts.
   */
   xdrvGlobalInterruptEnable();
}


/*
*****************************************************************************
** FUNCTION:   intCtrlDeinitMips
**
** PURPOSE:    Deinitialize MIPS interrupt control driver.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void intCtrlDeinitMips( void )
{
   XDRV_GLOBAL_INTERRUPT_LOCK   dummy;

   /*
   ** Disable global interrupts.
   */
   xdrvGlobalInterruptDisable( &dummy );


   /*
   ** Disable peripheral interrupts.
   */
   intCtrlMaskMips( gPeripheralInterruptId );


   /*
   ** Default user registered interrupt service routines.
   */
   memset( gInterruptHandler, 0, sizeof( gInterruptHandler ) );
   memset( gInterruptData,    0, sizeof( gInterruptData ) );

   memset( gPeripheralInterruptHandler, 0, sizeof( gPeripheralInterruptHandler ) );
   memset( gPeripheralInterruptData,    0, sizeof( gPeripheralInterruptData ) );
}


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlInstallInterrupt
**
** PURPOSE:    To register an interupt service routine for a specific interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to register. The value of 'interruptId'
**                               will be chip-specific, since each chip supports
**                               a different set of peripheral interrupt sources.
**             isrFunc     (in)  ISR that will be invoked when interrupt triggers.
**             isrParm     (in)  Parameter to pass to ISR when invoked.
**
** RETURNS:    Nothing.
**
** NOTE:       This function simply registers the interrupt service routine.
**             It does not enable it. xdrvIntCtrlEnableInterrupt() must be
**             called to enable the ISR.
*****************************************************************************
*/
void xdrvIntCtrlInstallInterrupt
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   XDRV_INT_CTRL_ISR       isrFunc,
   XDRV_INT_CTRL_ISR_PARM  isrParm
)
{
   XDRV_GLOBAL_INTERRUPT_LOCK   lock;


   xdrvGlobalInterruptDisable( &lock );
   {
      /*
      ** Register user interrupt service routine.
      */

      if ( INT_CTRL_MIPS_IS_PERIPH_ID( interruptId ) )
      {
         unsigned int periphIntIndex;

         /* Map interrupt id to peripheral interrupt index. */
         periphIntIndex = INT_CTRL_MIPS_PERIPH_ID_TO_INDEX( interruptId );

         XCHG_ASSERT( periphIntIndex < NUM_PERIPHERAL_INTERRUPTS );

         gPeripheralInterruptHandler[ periphIntIndex ] = isrFunc;
         gPeripheralInterruptData   [ periphIntIndex ] = isrParm;
      }
      else
      {
         XCHG_ASSERT( interruptId <  INT_CTRL_MIPS_NUM_INTERRUPTS );
         XCHG_ASSERT( interruptId != gPeripheralInterruptId       );

         if ( IS_EXTERNAL_INT_ID( interruptId ) )
         {
            unsigned int extInterruptIndex;

            /* Map interrupt id to external interrupt index. */
            extInterruptIndex = MAP_INT_ID_TO_EXTERNAL_INDEX( interruptId );

            /* Configure External Interrupt selected. */
            *gBrcmMipsCtrl.extEdgeInSense       &= ~(1 << extInterruptIndex ); /* one edge */
            *gBrcmMipsCtrl.extIrqMskandDetClr   |=  (1 << extInterruptIndex ); /* clr */
            *gBrcmMipsCtrl.extIrqStatEdgeConfig &= ~(1 << extInterruptIndex ); /* falling edge */
         }


         gInterruptHandler[ interruptId ] = isrFunc;
         gInterruptData   [ interruptId ] = isrParm;
      }
   }
   xdrvGlobalInterruptRestore( &lock );
}


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlUninstallInterrupt
**
** PURPOSE:    To de-register an interupt service routine for a specific interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to register. The value of 'interruptId'
**                               will be chip-specific, since each chip supports
**                               a different set of peripheral interrupt sources.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvIntCtrlUninstallInterrupt
(
   XDRV_INT_CTRL_INTERRUPT interruptId
)
{
   xdrvIntCtrlInstallInterrupt( interruptId, NULL, NULL );
}


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlEnableInterrupt
**
** PURPOSE:    Enable the specified interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to enable.
**
** RETURNS:    Nothing.
**
** NOTE:       xdrvIntCtrlInstallInterrupt() must be called to register the
**             actual interrupt service routine.
*****************************************************************************
*/
void xdrvIntCtrlEnableInterrupt( XDRV_INT_CTRL_INTERRUPT interruptId )
{
   XDRV_GLOBAL_INTERRUPT_LOCK   lock;


   xdrvGlobalInterruptDisable( &lock );
   {
      if ( INT_CTRL_MIPS_IS_PERIPH_ID( interruptId ) )
      {
         unsigned int periphIntIndex;

         /*
         ** Enable peripheral interrupt.
         */

         /* Map interrupt id to peripheral interrupt index. */
         periphIntIndex = INT_CTRL_MIPS_PERIPH_ID_TO_INDEX( interruptId );

         XCHG_ASSERT( periphIntIndex < NUM_PERIPHERAL_INTERRUPTS );

         *gBrcmMipsCtrl.intEnableMask |= (1 << periphIntIndex);

      }
      else
      {
         XCHG_ASSERT( interruptId <  INT_CTRL_MIPS_NUM_INTERRUPTS );
         XCHG_ASSERT( interruptId != gPeripheralInterruptId       );

         if ( IS_EXTERNAL_INT_ID( interruptId ) )
         {
            unsigned int extInterruptIndex;

            /* Map interrupt id to external interrupt index. */
            extInterruptIndex = MAP_INT_ID_TO_EXTERNAL_INDEX( interruptId );

            /* Enable the interrupt in the BRCM external config register. */
            *gBrcmMipsCtrl.extIrqMskandDetClr |= (1 << (extInterruptIndex + 4));
         }


         /*
         ** Enable regular MIPS core interrupt (CP0 status register).
         */
         intCtrlUnmaskMips( interruptId );
      }
   }
   xdrvGlobalInterruptRestore( &lock );
}

/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlDisableInterrupt
**
** PURPOSE:    Disable the specified interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to enable.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvIntCtrlDisableInterrupt( XDRV_INT_CTRL_INTERRUPT interruptId )
{
   XDRV_GLOBAL_INTERRUPT_LOCK   lock;

   xdrvGlobalInterruptDisable( &lock );
   {
      if ( INT_CTRL_MIPS_IS_PERIPH_ID( interruptId ) )
      {
         unsigned int periphIntIndex;

         /*
         ** Disable peripheral interrupt.
         */

         /* Map interrupt id to peripheral interrupt index. */
         periphIntIndex = INT_CTRL_MIPS_PERIPH_ID_TO_INDEX( interruptId );

         XCHG_ASSERT( periphIntIndex < NUM_PERIPHERAL_INTERRUPTS );

         *gBrcmMipsCtrl.intEnableMask &= ~(1 << periphIntIndex);
      }
      else
      {

         XCHG_ASSERT( interruptId <  INT_CTRL_MIPS_NUM_INTERRUPTS );
         XCHG_ASSERT( interruptId != gPeripheralInterruptId       );

         if ( IS_EXTERNAL_INT_ID( interruptId ) )
         {
            unsigned int extInterruptIndex;

            /* Map interrupt id to external interrupt index. */
            extInterruptIndex = MAP_INT_ID_TO_EXTERNAL_INDEX( interruptId );

            /* Enable the interrupt in the BRCM external config register. */
            *gBrcmMipsCtrl.extIrqMskandDetClr &= ~(1 << (extInterruptIndex + 4));
         }


         /*
         ** Disable regular MIPS core interrupt (CP0 status register).
         */
         intCtrlMaskMips( interruptId );
      }
   }
   xdrvGlobalInterruptRestore( &lock );
}


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlGenerateSoftInterrupt
**
** PURPOSE:    Generate the specified software interrupt.
**
** PARAMETERS: interruptId (in)  Software interrupt to generate.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvIntCtrlGenerateSoftInterrupt( XDRV_INT_CTRL_INTERRUPT interruptId )
{
   XDRV_GLOBAL_INTERRUPT_LOCK    lock;

   if ( ( interruptId != INT_CTRL_MIPS_SW_INTERRUPT_0 ) &&
        ( interruptId != INT_CTRL_MIPS_SW_INTERRUPT_1 ) )
   {
      /* Error. MIPS software interrupt id should be 0 or 1. */
      return;
   }


   /* Generate software interrupt. Make sure to disable interrupts while
   ** modifying CP0 registers. */
   xdrvGlobalInterruptDisable( &lock );
   {
      intCtrlGenerateSoftInterruptMips( interruptId );
   }
   xdrvGlobalInterruptRestore( &lock );
}


/*
*****************************************************************************
** FUNCTION:   MapLowLevelHandler
**
** PURPOSE:    Register an interrupt service routine with the low-level
**             MIPS assembly interrupt handler. This should be used to register
**             internal interrupt handlers that bridge the low-level interrupt
**             code with the user registered interrupt service routine.
**
** PARAMETERS: interruptId (in)  Interrupt to register. The value of 'interruptId'
**                               will be chip-specific, since each chip supports
**                               a different set of peripheral interrupt sources.
**             isrFunc     (in)  ISR that will be invoked when interrupt triggers.
**             isrParm     (in)  Parameter to pass to ISR when invoked.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void MapLowLevelHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            isrFunc,
   unsigned int            isrParm
)
{
   XDRV_GLOBAL_INTERRUPT_LOCK   lock;

   XCHG_ASSERT( interruptId <  INT_CTRL_MIPS_NUM_INTERRUPTS );


   xdrvGlobalInterruptDisable( &lock );
   {
      exc_interrupt_handlers[interruptId] = isrFunc;
      exc_interrupt_data    [interruptId] = isrParm;
   }
   xdrvGlobalInterruptRestore( &lock );

}


/*
*****************************************************************************
** FUNCTION:   SoftwareIntHandler
**
** PURPOSE:    Internal interrupt handler that bridges the low-level MIPS
**             assembly interrupt code with the user registered interrupt
**             service routine.
**
** PARAMETERS: interruptId (in)  Identifier for interrupt.
**             data        (in)  User registered data.
**
** RETURNS:    0
**
** NOTE:
*****************************************************************************
*/
static unsigned int SoftwareIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
)
{
   XDRV_INT_CTRL_ISR handler = gInterruptHandler[ interruptId ];

   /* Mask the interrupt. It is the user's responsibility to unmask it. */
   intCtrlMaskMips( interruptId );


   /* Ack the interrupt. */
   intCtrlClearMips( interruptId );


   /* Invoke the user interrupt service routine. */
   if ( handler != NULL )
   {
      handler( gInterruptData[ interruptId ] );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   ExtIntHandler
**
** PURPOSE:    Internal interrupt handler that bridges the low-level MIPS
**             assembly interrupt code with the user registered interrupt
**             service routine.
**
** PARAMETERS: interruptId (in)  Identifier for interrupt.
**             data        (in)  User registered data.
**
** RETURNS:    0
**
** NOTE:
*****************************************************************************
*/
static unsigned int ExtIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
)
{
   unsigned int      extInterruptIndex;
   XDRV_INT_CTRL_ISR handler = gInterruptHandler[ interruptId ];


   /* Map interrupt id to external interrupt index. */
   extInterruptIndex = MAP_INT_ID_TO_EXTERNAL_INDEX( interruptId );

   /* Mask the interrupt. It is the user's responsibility to unmask it. */
   *gBrcmMipsCtrl.extIrqMskandDetClr &= ~(1 << (extInterruptIndex + 4));


   /* Ack the interrupt. */
   *gBrcmMipsCtrl.extIrqMskandDetClr |= (1 << extInterruptIndex);


   /* Invoke the user interrupt service routine. */
   if ( handler != NULL )
   {
      handler( gInterruptData[ interruptId ] );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   TimerIntHandler
**
** PURPOSE:    Internal interrupt handler that bridges the low-level MIPS
**             assembly interrupt code with the user registered interrupt
**             service routine.
**
** PARAMETERS: interruptId (in)  Identifier for interrupt.
**             data        (in)  User registered data.
**
** RETURNS:    0
**
** NOTE:
*****************************************************************************
*/
static unsigned int TimerIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
)
{
   XDRV_INT_CTRL_ISR handler = gInterruptHandler[ INT_CTRL_MIPS_TIMER_INTERRUPT ];


   /* Mask the interrupt. It is the user's responsibility to unmask it.
   ** Also, the user needs to clear the interrupt by writing to the CP0 'compare'
   ** register. I can't do it for them because I don't know what the new value
   ** of the compare register should be. */
   intCtrlMaskMips( interruptId );


   /* Invoke the user interrupt service routine. */
   if ( handler != NULL )
   {
      handler( gInterruptData[ INT_CTRL_MIPS_TIMER_INTERRUPT ] );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   PeriphIntHandler
**
** PURPOSE:    Internal interrupt handler that bridges the low-level MIPS
**             assembly interrupt code with the user registered interrupt
**             service routine.
**
** PARAMETERS: interruptId (in)  Identifier for interrupt.
**             data        (in)  User registered data.
**
** RETURNS:    0
**
** NOTE:
*****************************************************************************
*/
static unsigned int PeriphIntHandler
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   unsigned int            data
)
{
   unsigned int  activeInterrupts;
   unsigned int  currentInterrupt;
   int           i;

   /* Mask the asserted interrupts with the enable mask to find which are active. */
   while (( activeInterrupts = *gBrcmMipsCtrl.intStatus & *gBrcmMipsCtrl.intEnableMask ) != 0 )
   {
      currentInterrupt = 1 << 31;

      /* Test all interrupt bits starting with bit 31.  If an interrupt is active
      ** and there is an interrupt handler registered, call the handler. */
      for ( i = 31; i >= 0; i-- )
      {
         if (( activeInterrupts & currentInterrupt ) != 0)
         {
            XDRV_INT_CTRL_ISR handler = gPeripheralInterruptHandler[i];

            /* Mask the interrupt. It is the user's responsibility to unmask it.
            ** The interrupt must be cleared at the peripheral source! */
            *gBrcmMipsCtrl.intEnableMask &= (~currentInterrupt);


            /* Invoke the user interrupt service routine. */
            if ( handler != NULL )
            {
               handler( gPeripheralInterruptData[i] );
            }
         }

         currentInterrupt >>= 1;
      }
   }

   return ( 0 );
}

void xdrvIntCtrlSoftwareInterrupt( int number )
{
   xdrvIntCtrlDisableInterrupt(number);
}
