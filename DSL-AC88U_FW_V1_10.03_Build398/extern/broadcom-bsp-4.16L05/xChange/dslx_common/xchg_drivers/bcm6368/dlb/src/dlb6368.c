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
*    Filename: dlb6368.c
*
****************************************************************************
*    Description:
*
*     Implementation for 6368-specific dynamic load balancing between MIPS
*     thread-processors. The thread-processors have a number of shared hardware
*     resources (data-cache etc). If the "DSP" thread-processor approaches 100%
*     utilization (or passes 100%), the primary thread processor should be
*     throttled. This should prevent DSP overrun conditions which would
*     otherwise result in poor voice quality. The throttling mechanism
*     should prevent the primary thread processor from accessing shared
*     resources for a specific (and short) amount of time.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <bosTask.h>
#include <bosSem.h>
#include <itpcSharedCmds.h>
#include <dlb6368.h>
#include <dlb6368Cfg.h>
#include <bcm6368.h>
#include <xdrvLog.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/*
** Macros used to twiddle debug GPIOs.
*/
#define  SET_GPIO()                                   \
         do                                           \
         {                                            \
            if ( gDlbGpioEnabled )                    \
            {                                         \
               /* GPIOs can be enabled at run-time,   \
               ** we need to set the direction as     \
               ** output each time. */                \
               GPIO->gpio_hi     |= (1<<0);           \
               GPIO->gpio_dir_hi |= (1<<0);           \
            }                                         \
         }                                            \
         while ( 0 );


#define  CLEAR_GPIO()                                 \
         do                                           \
         {                                            \
            if ( gDlbGpioEnabled )                    \
            {                                         \
               GPIO->gpio_hi &= ~(1<<0);              \
            }                                         \
         }                                            \
         while ( 0 );




// These macros are coded for the GNU C compiler inline asm syntax, and for the
// MIPS CPU.  Since this is a 6368 specific file, and we only care about MIPS
// processors this should be relatively reasonable.
//
// Note the use of (), which groups the instructions into a single expression,
// the result of which is returned essentially as though it were a function call.
// In order to store and return the result, we need to introduce a new scope via
// {} so that we can have temporary variables.  This combination of ({...}) is
// not ANSI compliant, but works with the GNU compiler.  In order to keep the
// compiler from warning about this non-ANSI construct, the GNU boys allow us
// to use the __extension__ keywork, which suppresses the non-ANSI warning for
// this macro.
//
// This function reads the current value of the other thread processor's
// MIPS CP0 count register using a backdoor trick. This is required because
// this thread-processors's count register is used by the operating system,
// and it periodically clears it. We want a free-running count register.
// Also, all the peripheral timers are already in-use.
//
// This is more efficient than using the generic cp0GetRegMips() function
// because it executes the mtc0 instructions uncached. This macro is called
// in a tight loop, and we want to avoid uncached accesses which will derate
// the other thread processor.
#define GetTimerCount()                                                          \
__extension__                                                                    \
({                                                                               \
   /* These variables will be register-optimized by the compiler. */             \
   unsigned int __ticks;                                                         \
                                                                                 \
                                                                                 \
   /* This instruction reads the other thread-processor's count register.    */  \
   /* The volatile keyword keeps the compiler from reordering the statements */  \
   /* too much.                                                              */  \
   __asm__ volatile ("mfc0 %0, $9, 7" : "=d" (__ticks));                         \
                                                                                 \
   __asm__ ("nop; nop; nop");                                                    \
                                                                                 \
   /* This causes the value in __ticks to be returned from the macro as      */  \
   /* though it were the return value from a function.                       */  \
   __ticks;                                                                      \
})


/* ---- Private Variables ------------------------------------------------ */

/* Set to 1 to enable debug GPIO toggling. Use a variable instead of a #define
** so that this can be enabled at run-time. */
static unsigned int gDlbGpioEnabled;


/* ---- Private Function Prototypes -------------------------------------- */

static unsigned int LoadBalanceIsr
(
   volatile XDRV_ITPC_CMD       *data,
   XDRV_ITPC_CMD_HANDLER_PARM    parm
);

static void LoadBalanceTaskMain( BOS_TASK_ARG taskArg );
static void DisplayStats( XDRV_DLB *drv );
static void SetThrottleDuration( XDRV_DLB *drv, unsigned int durationMsec );
static void EnableLoadBalancing( XDRV_DLB *drv, XDRV_BOOL enable );
static XDRV_BOOL IsLoadBalancingEnabled( XDRV_DLB *drv );
static void EnableStaticLoadBalancing( XDRV_DLB *drv, XDRV_BOOL enable );

static void DelayMSec( unsigned int mSec );



/* DLB "meta-driver" interface */
static const XDRV_DLB_FUNCS  gDlb6368DrvFuncs =
{
   EnableLoadBalancing,
   IsLoadBalancingEnabled,
   DisplayStats,
   EnableStaticLoadBalancing,
   SetThrottleDuration
};


/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   dlb6368Create
**
** PURPOSE:    Instantiate a 6368 dynamic load balancer (DLB) driver.
**
** PARAMETERS: throttleDurationMsec (in)  Duration to throttle access to shared
**                                        hardware resources.
**             itpcDrv              (in)  Inter-thread processor communication
**                                        driver used to communication with other
**                                        thread processor.
**             drv                  (out) Load balancer driver object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void dlb6368Create
(
   unsigned int   throttleDurationMsec,
   XDRV_ITPC     *itpcDrv,
   DLB_6368_DRV  *drv
)
{
   gDlbGpioEnabled = 0;

   memset( drv, 0, sizeof( DLB_6368_DRV ) );

   /* Set the "meta-driver" interface */
   drv->pDrvFuncs = &gDlb6368DrvFuncs;

   drv->itpcDrv                = itpcDrv;
   drv->throttleDurationMsec   = throttleDurationMsec;



   /*
   ** Create an interrupt service task for DSP assertions.
   */
   drv->loadBalanceTaskId = 0;
   if ( BOS_STATUS_OK != bosTaskCreateEx( "Load-balance",
                                          (8 * 1024),
                                          DLB_6368_CFG_TASK_PRIORITY,
                                          NULL,
                                          LoadBalanceTaskMain,
                                          NULL,
                                          drv,
                                          &drv->loadBalanceTaskId ) )
   {
      XCHG_ASSERT( 0 );
   }


   /*
   ** Install handler for signal from other thread-processor to perform
   ** dynamic load balancing.
   */
   xdrvItpcRegisterHandler( drv->itpcDrv,
                            ITPC_LOAD_BALANCE_TRIGGER_CMD_ID,
                            LoadBalanceIsr,
                            drv );


}


/*
*****************************************************************************
** FUNCTION:   dlb6368Destroy
**
** PURPOSE:    To destroy a previously created load balancer driver.
**
** PARAMETERS: drv   (mod) Load balancer driver object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void dlb6368Destroy( DLB_6368_DRV *drv )
{
   /*
   ** Uninstall handler for remote exception.
   */
   xdrvItpcUnregisterHandler( drv->itpcDrv, ITPC_LOAD_BALANCE_TRIGGER_CMD_ID );


   if ( drv->loadBalanceTaskId != 0 )
   {
      /* Currently, the interrupt service task should be blocked in the reset-
      ** acknowledgement state. Advance the task to the reset-active state.
      ** This will cause the task to resume operation. In this state, the task
      ** should perform any operations required for proper shutdown before
      ** exiting from it's main task function.
      */
      bosTaskProceedToShutdown( &drv->loadBalanceTaskId );

      /* Wait for the task to complete de-initialization */
      bosTaskWaitForDone( &drv->loadBalanceTaskId );
   }


   /*
   ** De-allocate sema4.
   */
   if ( bosSemDestroy( &drv->loadBalanceSema4 ) != BOS_STATUS_OK )
   {
      XCHG_ASSERT( 0 );
   }
}


/*
*****************************************************************************
** FUNCTION:   EnableLoadBalancing
**
** PURPOSE:    Enable/disable dynamic load balancing.
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**             enable   (in)
**
** RETURNS:    none
**
*****************************************************************************
*/
static void EnableLoadBalancing( XDRV_DLB *dlbDrv, XDRV_BOOL enable )
{
   DLB_6368_DRV                    *drv      = (DLB_6368_DRV *) dlbDrv;
   ITPC_SHARED_LOAD_BALANCE_CONFIG  config;


   /* Get the load balancing state from the DSP thread-processor. I don't keep
   ** a local copy of the state because I don't want them to get out of sync. */
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( drv->itpcDrv,
                                                   ITPC_GET_LOAD_BALANCE_CONFIG_CMD_ID,
                                                   NULL,
                                                   &config ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: can't get load balancing regs!" ));
      return;
   }




   /* Tell DSP thread-processor to enable or disable load balancing. */
   config.isEnabled = enable;
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( drv->itpcDrv,
                                                   ITPC_SET_LOAD_BALANCE_CONFIG_CMD_ID,
                                                   &config,
                                                   NULL ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV,  "ERROR: can't set load balancing regs!" ));
   }
}


/*
*****************************************************************************
** FUNCTION:   IsLoadBalancingEnabled
**
** PURPOSE:    Indicates current state of DLB (enabled or disable).
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**
** RETURNS:    XDRV_TRUE or XDRV_FALSE.
**
*****************************************************************************
*/
static XDRV_BOOL IsLoadBalancingEnabled( XDRV_DLB *dlbDrv )
{
   DLB_6368_DRV                    *drv      = (DLB_6368_DRV *) dlbDrv;
   ITPC_SHARED_LOAD_BALANCE_CONFIG  config;


   /* Get the load balancing state from the DSP thread-processor. I don't keep
   ** a local copy of the state because I don't want them to get out of sync. */
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( drv->itpcDrv,
                                                   ITPC_GET_LOAD_BALANCE_CONFIG_CMD_ID,
                                                   NULL,
                                                   &config ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV,  "ERROR: can't get load balancing regs!" ));
      return ( XDRV_FALSE );
   }

   return ( config.isEnabled );
}



/*
*****************************************************************************
** FUNCTION:   EnableStaticLoadBalancing
**
** PURPOSE:    Enable/disable static load balancing, instead of dynamic.
**
** PARAMETERS: pDrv           (in)  Pointer to the DLB driver device structure.
**             enable         (in)
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
static void EnableStaticLoadBalancing( XDRV_DLB *dlbDrv, XDRV_BOOL enable )
{
   DLB_6368_DRV                    *drv      = (DLB_6368_DRV *) dlbDrv;
   ITPC_SHARED_LOAD_BALANCE_CONFIG  config;


   /* Get the load balancing state from the DSP thread-processor. I don't keep
   ** a local copy of the state because I don't want them to get out of sync. */
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( drv->itpcDrv,
                                                   ITPC_GET_LOAD_BALANCE_CONFIG_CMD_ID,
                                                   NULL,
                                                   &config ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: can't get load balancing regs!" ));
      return;
   }




   /* Tell DSP thread-processor to enable or disable static load balancing. */
   config.isStaticModeEnabled = enable;
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( drv->itpcDrv,
                                                   ITPC_SET_LOAD_BALANCE_CONFIG_CMD_ID,
                                                   &config,
                                                   NULL ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: can't set load balancing regs!" ));
   }
}


/*
*****************************************************************************
** FUNCTION:   SetThrottleDuration
**
** PURPOSE:    Set the throttleing duration used by the DLB algorithm. This
**             is really an implementation specific parameter, and probably
**             doesn't belong in the generic API.
**
** PARAMETERS: pDrv           (in)  Pointer to the DLB driver device structure.
**             durationMsec   (in)  Throttling duration used by DLB algorithm.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
static void SetThrottleDuration( XDRV_DLB *dlbDrv, unsigned int durationMsec )
{
   DLB_6368_DRV *drv = (DLB_6368_DRV *) dlbDrv;

   drv->throttleDurationMsec = durationMsec;
}


/*
*****************************************************************************
** FUNCTION:   DisplayStats
**
** PURPOSE:    Display load balancing statistics (for debug).
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
static void DisplayStats( XDRV_DLB *dlbDrv )
{
   DLB_6368_DRV                    *drv      = (DLB_6368_DRV *) dlbDrv;
   ITPC_SHARED_LOAD_BALANCE_CONFIG  config;

   /* Cache current count since it might change while we execute this function. */
   unsigned int currentCount = drv->currLoadBalanceCount;


   memset( &config, 0, sizeof( config ) );


   /* Get the load balancing state from the DSP thread-processor. I don't keep
   ** a local copy of the state because I don't want them to get out of sync. */
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( drv->itpcDrv,
                                                   ITPC_GET_LOAD_BALANCE_CONFIG_CMD_ID,
                                                   NULL,
                                                   &config ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: can't get load balancing regs!" ));
   }


   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Is enabled: %d", config.isEnabled ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Is static mode enabled: %d", config.isStaticModeEnabled ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Load balance count (since last query): %d",
              currentCount - drv->prevLoadBalanceCount ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Load balance count (total):            %d",
              currentCount ));

   drv->prevLoadBalanceCount = currentCount;
}



/*
*****************************************************************************
** FUNCTION:   LoadBalanceIsr
**
** PURPOSE:    ISR invoked when load balancing is to be performed. This will
**             be signalled by the other thread processor via a software
**             interrupt, when an overrun condition is detected.
**
** PARAMETERS: data  (in)  Not used.
**             parm  (in)  User registered data (pointer to DLB driver instance).
**
** RETURNS:    0
**
** NOTE:
*****************************************************************************
*/
static unsigned int LoadBalanceIsr
(
   volatile XDRV_ITPC_CMD       *data,
   XDRV_ITPC_CMD_HANDLER_PARM    parm
)
{
   DLB_6368_DRV *drv = (DLB_6368_DRV *) parm;

   /* Not used. */
   (void) data;

   /* Signal the dynamic load balancing task. */
   bosSemGive( &drv->loadBalanceSema4 );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   LoadBalanceTaskMain
**
** PURPOSE:    Interrupt service task that is signalled by the load balancing
**             ISR. The ISR will trigger when the other thread-processor
**             detects an overrun condition, and signals a software interrupt.
**             The load balancer should force this thread processor to throttle
**             access to shared hardware resources for a brief period of time.
**
**
** PARAMETERS: taskArg  (in)  User registered argument (pointer to DLB driver instance).
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void LoadBalanceTaskMain( BOS_TASK_ARG taskArg )
{

   DLB_6368_DRV *drv = (DLB_6368_DRV *) taskArg;

   while ( 1 )
   {
      /*
      ** Wait for an event from the other thread processor that load-balancing
      ** is required.
      */
      if ( BOS_STATUS_RESET == bosSemTake( &drv->loadBalanceSema4 ) )
      {
         /*
         ** This task has been notified that a reset is pending. Acknowledge
         ** the notification and then block until the we are resumed.
         */
         bosTaskResetAck();


         /* The task should stop running by exiting from this main function */
         return;
      }



      /* Assert debug GPIOs. */
      SET_GPIO();


      /*
      ** Perform load balancing - force this thread processor to throttle
      ** access to shared hardware resources for a brief period of time
      ** by spinning. This is done with interrupts enabled, so ISRs will
      ** continue to be serviced. Also, any tasks with higher priority than
      ** the load balancer can preempt us.
      */
      DelayMSec( drv->throttleDurationMsec );


      /*
      ** Update stats.
      */
      drv->currLoadBalanceCount++;


      /* Clear debug GPIOs. */
      CLEAR_GPIO();

   }
}


/*
*****************************************************************************
** FUNCTION:   DelayMSec
**
** PURPOSE:    Spin for the specified amount of milli-seconds with interrupts
**             enabled.
**
** PARAMETERS: mSec  (in) Number of milli-seconds to spin for.
**
** RETURNS:    Nothing.
**
** NOTE:       An overflow error can occur if this is called with a sufficiently
**             large delay value on a high speed processor. Currently, the
**             the fastest processor that we support is 300MHz, and would
**             require a requested delay value of 28 seconds. This isn't
**             a reasonable amount of time to spin for. If this assumption
**             proves to be wrong, then we will need to change the
**             implementation to spin in small chunks of the requested delay.
*****************************************************************************
*/
static void DelayMSec( unsigned int mSec )
{
   unsigned int            ticksToWait;
   unsigned int            currentTick;
   unsigned int            expiryTick;
   int                     diff;
   unsigned int            i;


   if ( mSec == 0 )
   {
      return;
   }


   /*
   ** We use the MIPS CP0 'count' register to implement the timer.
   ** The 'count' register increments at half the MIPS core frequency. So,
   ** divide the CPU freq by 2, and then multiple this by the delay in
   ** seconds to calculate the number of clock cycles that need to expire for
   ** the requested delay.
   */
   ticksToWait = MIPS_CLOCK_FREQ_HZ / 2;
   ticksToWait = ( ticksToWait / 1000 ) * mSec;


   /* Calculate the expiration tick value. */
   currentTick = GetTimerCount();
   expiryTick  = currentTick + ticksToWait;


   /* Keep spinning until the counter has reached the desired value. */
   do
   {
      // This for loop should have 6 instructions, or roughly 1/50 of a usec.
      // With 5000 iterations it should waste about 0.1 msec.  I think this will
      // reduce register polling by a factor of at least 5000.  We could try
      // a larger loop value if this isn't enough.
      for ( i = 5000; i > 0; i-- )
      {
          asm( "nop; nop; nop; " );
      }

      currentTick = GetTimerCount();
      diff = expiryTick - currentTick;

   }
   while( diff >= 0 );

}
