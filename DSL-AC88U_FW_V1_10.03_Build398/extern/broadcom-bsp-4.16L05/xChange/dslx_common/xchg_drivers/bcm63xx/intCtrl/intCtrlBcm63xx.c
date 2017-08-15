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
*    Filename: intCtrlBcm63xx.c
*
****************************************************************************
*    Description:
*
*     Implements an interface to the Cable Modem Board Support Package (BSP)
*     for registering interrupts.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stddef.h>
#include <xdrvIntCtrl.h>
#include <linux/interrupt.h>
#include <xchgAssert.h>
#include "intCtrlBcm63xx.h"
#include <bcm_intr.h>
#include <xdrvLog.h>
#include <xchgAssert.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */


#ifdef BRCM_NONDIST_BUILD
#define MAX_INTERRUPTS_ID  INTERRUPT_ID_LAST /* Use max interrupts from 6XXX_intr.h */
#else
#define MAX_INTERRUPTS_ID 72      /* Peripheral interrupts + 1 Timer + 2 SW Ints + 5 EXT Ints */
#endif /* BRCM_NONDIST_BUILD */

/* Table for storing the interrupt service routine for every interrupt supported
 * by the system. The common linux interrupt handler indexes this table to invoke
 * the corresponding ISR.
 */
static XDRV_INT_CTRL_ISR_INFO interruptHandlers[MAX_INTERRUPTS_ID];

/* ---- Private Variables ------------------------------------------------ */

/* ---- Private Function Prototypes -------------------------------------- */

static int linuxInterruptHandler(int irq, void *dev_id, struct pt_regs *ptregs);

/* ---- Functions -------------------------------------------------------- */


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
   XCHG_ASSERT(interruptId < MAX_INTERRUPTS_ID);

   interruptHandlers[interruptId].isrFunction = isrFunc;
   interruptHandlers[interruptId].isrParm     = isrParm;

#ifdef BRCM_NONDIST_BUILD
# if (defined(CONFIG_BCM96816) || defined(CONFIG_BCM96818))
   if ( interruptId == INTERRUPT_ID_APM_DMA_2 ||
        interruptId == INTERRUPT_ID_APM_DMA_4 )
# elif defined(CONFIG_BCM96838)
   if ( interruptId == INTERRUPT_ID_APM_IUDMA ||
        interruptId == INTERRUPT_ID_APM )
# elif defined(CONFIG_BCM96828)
   if ( interruptId == INTERRUPT_ID_APM_DMA2 ||
        interruptId == INTERRUPT_ID_APM_DMA4 )
# elif defined(CONFIG_BCM968500)
   if ( 0 )
# elif defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
   if ( interruptId == INTERRUPT_ID_PCM_0 ||
        interruptId == INTERRUPT_ID_PCM_1 )
# else
   if ( interruptId == INTERRUPT_ID_PCM_DMA_0 )
# endif /* defined(CONFIG_BCM96816) */
   {
      /*
      ** In the SMP voice configuration Hausware and the PCM/APM HAL must
      ** run on TP1
      */
      BcmHalMapInterruptEx( (FN_HANDLER)linuxInterruptHandler,
                            (unsigned int)isrParm,
                            interruptId,
                            "voip",
                            INTR_REARM_YES,
# if CONFIG_SMP
                            INTR_AFFINITY_TP1_ONLY
# else
                            INTR_AFFINITY_DEFAULT
# endif /* CONFIG_SMP */
                          );
   }
   else
   {
# ifndef CONFIG_BCM968500
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_INTCTRLBCM63xx: ERROR - Unknown Interrupt %d being configured !!!", interruptId));
      XCHG_ASSERT(0);
# endif /* CONFIG_BCM968500 */
   }

#else
   BcmHalMapInterrupt( (FN_HANDLER)linuxInterruptHandler, (unsigned int)isrParm, interruptId );
#endif /* BRCM_NONDIST_BUILD */
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
void xdrvIntCtrlUninstallInterrupt( XDRV_INT_CTRL_INTERRUPT interruptId )
{
   XCHG_ASSERT(interruptId < MAX_INTERRUPTS_ID);

   free_irq(interruptId, interruptHandlers[interruptId].isrParm);

   interruptHandlers[interruptId].isrFunction = NULL;
   interruptHandlers[interruptId].isrParm = NULL;
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
   XCHG_ASSERT(interruptId < MAX_INTERRUPTS_ID);

   BcmHalInterruptEnable( interruptId );
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
   XCHG_ASSERT(interruptId < MAX_INTERRUPTS_ID);

   BcmHalInterruptDisable( interruptId );
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
#if !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)
   XCHG_ASSERT(interruptId == INTERRUPT_ID_SOFTWARE_0 || interruptId == INTERRUPT_ID_SOFTWARE_1);

   BcmHalGenerateSoftInterrupt( interruptId );
#endif
}


/*
*****************************************************************************
** FUNCTION:   linuxInterruptHandler
**
** PURPOSE:    This interrupt handler is just a wrapper for invoking the
**             interrupt handler for the common code. The reason for the
**             existence of this wrapper is the different prototype for
**             the interrupt handler under Linux. Namely, the interrupt
**             handler for the common code requires only one argument
**             specifying the context for the interrupt. On the other hand
**             the interrupt handler that has to be registered with the
**             Linux kernel should contain three arguments.
**             The second argument 'dev_id' corresponds to the interrupt
**             context so dev_id is used to invoke the interrupt handler
**             for the common code.
**
** PARAMETERS: irq      (out)
**             dev_id   (out)
**             ptregs   (out)
**
** RETURNS:    Value returned by the common code interrupt handler.
**
*****************************************************************************
*/
static int linuxInterruptHandler(int irq, void *dev_id, struct pt_regs *ptregs)
{
   XDRV_INT_CTRL_ISR_PARM context = dev_id;

   XCHG_ASSERT(irq >= 0 && irq < MAX_INTERRUPTS_ID);

   /* Invoke ISR if one is registered */
   if ( interruptHandlers[irq].isrFunction )
   {
      interruptHandlers[irq].isrFunction( context );
      return IRQ_HANDLED;
   }

   return IRQ_NONE;
}
