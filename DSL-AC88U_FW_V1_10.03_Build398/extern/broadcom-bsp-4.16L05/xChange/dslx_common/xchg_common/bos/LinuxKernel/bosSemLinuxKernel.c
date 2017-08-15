/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom Corporation
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
*  Filename: bosSemLinuxKernel.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosSemLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Semaphore Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <kernelMainWrapper.h>

/* linux/interrupt.h is included right after kernelMainWrapper.h
** in order to resolve warnings when included later when kernel printf is redefined. */
#include <linux/interrupt.h>  /* for in_softirq */

#include <string.h>
#include <bosCfg.h>

#if BOS_CFG_SEM

#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosSem.h>
#include <bosSemPrivate.h>
#include <bosLog.h>
#include <bosSignalLinuxKernel.h>

#define SEMFIX 0              /* Fix for bosSemTimedTake is disabled! */

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define SEM_NAME_STR_LEN   4

/* Semaphore structure */
typedef struct
{
   struct semaphore  sem;
   char              name[SEM_NAME_STR_LEN + 1];
   BOS_BOOL          bInUse;
   atomic_t          bosCount;
}BOS_SEMAPHORE, *PBOS_SEMAPHORE;

#if (SEMFIX == 1)
/* Data passed to the timer handler */
struct TIMER_DATA
{
    BOS_SEMAPHORE *sem;        /* Semaphore being used */
    struct task_struct *tsk;   /* Task structure of current process */
};
#endif /* SEMFIX */

/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosSemInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */

static BOS_STATUS CheckSemStatusLinuxKernel( int err, BOS_SEM_FUNC_ID funcId );
static void bcmOsSemTimeOutHdlr( BOS_UINT32 arg );

/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
 * Function Name: bosSemInit
 * Description  : Initializes the BOS semaphore
 * Returns      : BOS_STATUS_OK
*****************************************************************************
*/
BOS_STATUS bosSemInit( void )
{
   BOS_ASSERT( !gBosSemInitialized );

   bosSemInitFunctionNames();

   gBosSemInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosSemInit */


/*
*****************************************************************************
 * Function Name: bosSemTerm
 * Description  : Uninitializes the BOS semaphore
 * Returns      : BOS_STATUS_OK
*****************************************************************************
*/
BOS_STATUS bosSemTerm( void )
{
   BOS_ASSERT( gBosSemInitialized );

   gBosSemInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosSemTerm */


/*
*****************************************************************************
 * Function Name: bosSemCreate
 * Description  : Creates the BOS semaphore. The memory for the semaphore
 *                is requested from the kernel and the fields of the
 *                semaphore structure are assigned values.
 * Returns      : BOS_STATUS_OK  = successful
 *                BOS_STATUS_ERR = failed(memory allocation failed)
*****************************************************************************
*/
BOS_STATUS bosSemCreate
(
   const char *name,
   int         initCount,
   int         maxCount,
   BOS_SEM    *sem
)
{

   BOS_SEMAPHORE* pSem;
   (void)maxCount;   /* maxCount is not used for LinuxKernel */

   BOS_ASSERT( gBosSemInitialized );

   if(( pSem = kmalloc(sizeof(BOS_SEMAPHORE), GFP_KERNEL) ))
   {
      memset( pSem, 0, sizeof( *pSem ));
      if (name)
      {
         strncpy(pSem->name, name, SEM_NAME_STR_LEN);
      }
      sema_init( &pSem->sem, initCount );
      pSem->bInUse = BOS_TRUE;
      atomic_set( &pSem->bosCount, 0 );

      *sem = &pSem->sem;
   }
   else
   {
      blogStrErr(( gBosBlog, "bosSemCreate() - memory cannot be allocated"));
      return BOS_STATUS_ERR;
   }

   return ( BOS_STATUS_OK );

} /* bosSemCreate */


/*
*****************************************************************************
 * Function Name: bosSemDestroy
 * Description  : Destroys the BOS semaphore. All the tasks waiting on the
 *                semaphore are released and the memory occupied by the
 *                semaphore is returned to the kernel.
 * Returns      : BOS_STATUS_OK  = successful
*****************************************************************************
*/
BOS_STATUS bosSemDestroy( BOS_SEM *sem )
{
   BOS_SEMAPHORE*    pSem;
   wait_queue_head_t wait;

   BOS_ASSERT( gBosSemInitialized );
   BOS_ASSERT( sem != NULL );
   BOS_ASSERT( *sem != NULL );

   pSem = (BOS_SEMAPHORE*)(*sem);
   if( pSem->bInUse == BOS_TRUE)
   {
      pSem->bInUse = BOS_FALSE;

      /*
       * WARNING: For now we will not bother unblocking any task that is waiting
       * on the semaphore. This is rather dangerous but the following code is also
       * dangerous and incorrect for the case when no task is blocking on the semaphore
       */

      /* before de-allocationg the semaphore structure, we need release
         all the tasks waiting on it.
         pSem->bosCount is the number of waiting tasks (it icrements every time
         a task is trying to take the semaphore, and decrements every time it
         takes it the semaphore).
         pSem->bInUse = FALSE indicates that the semaphore is being deleted. */
      while( atomic_read( &pSem->bosCount ) > 0 )
      {
         up( *sem );

         /* Sleep for 1 tick */
         init_waitqueue_head(&wait);
         interruptible_sleep_on_timeout(&wait, 1 );
      }

      memset( pSem, 0, sizeof(*pSem) );
      kfree(pSem);
   }

   return ( BOS_STATUS_OK );

} /* bosSemDestroy */


/*
*****************************************************************************
 * Function Name: bosSemTake
 * Description  : This function is used by tasks to take a BOS semaphore.
 *                The task could end up being blocked waiting for the semaphore.
 * Returns      : BOS_STATUS_OK  = successful
 *                BOS_STATUS_ERR = failed
*****************************************************************************
*/
BOS_STATUS bosSemTake( BOS_SEM *sem )
{
   BOS_SEMAPHORE* pSem;
   BOS_STATUS     status;

   BOS_ASSERT( gBosSemInitialized );
   BOS_ASSERT( sem != NULL );
   BOS_ASSERT( *sem != NULL );

   pSem     = (BOS_SEMAPHORE*)(*sem);
   status   = BOS_STATUS_OK;

   if ( !in_softirq() )
   {
      /* Cannot take semaphore if running in a bottom half */

      if ( pSem->bInUse == BOS_TRUE )
      {
         atomic_inc( &pSem->bosCount );
         if ( down_interruptible( *sem ))
         {
            atomic_dec( &pSem->bosCount );
            return bosSignalMapSigToStatus();  /* interrupted */
         }
         atomic_dec( &pSem->bosCount );
      }
      else
      {
         status = BOS_STATUS_ERR;   /* semaphore deleted */
      }
   }
   else
   {
      /* Unable to take semaphore, because we are running in a bottom half */
      status = BOS_STATUS_ERR;
   }

   return ( status );

} /* bosSemTake */


/*
*****************************************************************************
 * Function Name: bosSemTimedTake
 * Description  : This function is an enhanced version of bosSemTake. This
 *                function allows a task to take a BOS semaphore, however,
 *                the task need not wait for the semaphore indefinitely. The
 *                task blocks for a semaphore for the amount of time specified
 *                as an argument (timeoutMsec). The task gets unblocked as soon
 *                as the time expires.
 * Returns      : BOS_STATUS_OK  = successful
 *                BOS_STATUS_ERR = failed
*****************************************************************************
*/
#if (SEMFIX == 0)
BOS_STATUS bosSemTimedTake( BOS_SEM *sem, BOS_TIME_MS timeoutMsec )
{
   int            ticks;
   BOS_SEMAPHORE* pSem;
   BOS_STATUS     status;

   BOS_ASSERT( gBosSemInitialized );
   BOS_ASSERT( sem != NULL );
   BOS_ASSERT( *sem != NULL );

   pSem     = (BOS_SEMAPHORE*)(*sem);
   status   = BOS_STATUS_OK;

   if ( timeoutMsec == 0 )
   {
      ticks = 0;
   }
   else if ( timeoutMsec == BOS_WAIT_FOREVER )
   {
      ticks = BOS_WAIT_FOREVER;
   }
   else
   {
      ticks = BOS_MS_TO_TICKS( timeoutMsec );
   }


   if( !in_softirq() )
   {
      if( pSem->bInUse == BOS_TRUE )
      {
         /* If the timeout is big, no need to start timer. */
         if( ticks == BOS_WAIT_FOREVER )
         {
            atomic_inc( &pSem->bosCount );
            while( 1 )
            {
               if ( down_interruptible( *sem ))
               {
                  status = bosSignalMapSigToStatus();
                  if( status == BOS_STATUS_IO )
                  {
                     /* Handle the case where we were interrupted due to an IO
                      * event */
                     continue;
                  }
                  atomic_dec( &pSem->bosCount );
                  printk("bosSemTimedTake [%d]: interrupted (%d) (%s)\n", __LINE__, (unsigned int)status, pSem->name);
                  return bosSignalMapSigToStatus();  /* interrupted */
               }
               /* If the down succeeds, continue */
               status = BOS_STATUS_OK;
               break;
            }

            if ( pSem->bInUse == BOS_FALSE )
            {
               status = BOS_STATUS_ERR;
            }

            atomic_dec( &pSem->bosCount );
         }
         else  /* not BOS_WAIT_FOREVER */
         {
            struct             timer_list Timer;
            int                timerRunning;

            if (ticks == 0)
            {
               ticks = 1;
            }

            init_timer( &Timer );
            Timer.function = bcmOsSemTimeOutHdlr;
            Timer.data = (unsigned long)(*sem);
            Timer.expires = jiffies + ticks;
            add_timer( &Timer );

            atomic_inc( &pSem->bosCount );
            if ( down_interruptible( *sem ) )
            {
               status = bosSignalMapSigToStatus();
               atomic_dec( &pSem->bosCount );
               del_timer_sync( &Timer );
               printk("bosSemTimedTake [%d]: interrupted (%d) (%s)\n", __LINE__, (unsigned int)status, pSem->name);
               return bosSignalMapSigToStatus();  /* interrupted */
            }
            status = BOS_STATUS_OK;

            if ( pSem->bInUse == BOS_FALSE )
            {
               status = BOS_STATUS_ERR;
            }

            atomic_dec( &pSem->bosCount );

            timerRunning  = del_timer_sync( &Timer );
            /* del_timer returns: 0= expired, 1= active */

            if ( !timerRunning )
            {
               status = BOS_STATUS_TIMEOUT; /* Timed out. */
            }
         }
      }
      else
      {
         status = BOS_STATUS_ERR; /* semaphore deleted */
      }
    }
   else
   {
      printk("bosSemTimedTake BOS_STATUS_ERR (%d) (%s)\n", __LINE__, pSem->name);
      /* Unable to take semaphore, because we are running in a bottom half */
      status = BOS_STATUS_ERR;
   }

   return( status );

} /* bosSemTimedTake */

#else

BOS_STATUS bosSemTimedTake( BOS_SEM *sem, BOS_TIME_MS timeoutMsec )
{
   int            ticks;
   BOS_SEMAPHORE* pSem;
   BOS_STATUS     status;

   BOS_ASSERT( gBosSemInitialized );
   BOS_ASSERT( sem != NULL );
   BOS_ASSERT( *sem != NULL );

   pSem     = (BOS_SEMAPHORE*)(*sem);
   status   = BOS_STATUS_OK;

   if ( timeoutMsec == BOS_WAIT_FOREVER )
   {
      ticks = BOS_WAIT_FOREVER;
   }
   else
   {
      ticks = BOS_MS_TO_TICKS( timeoutMsec );
   }


   if( !in_softirq() )
   {
      if( pSem->bInUse == BOS_TRUE )
      {
         /* If the timeout is big, no need to start timer. */
         if( ticks == BOS_WAIT_FOREVER  )
         {
            atomic_inc( &pSem->bosCount );
            if ( down_interruptible( *sem ))
            {
               atomic_dec( &pSem->bosCount );
               printk("bosSemTimedTake interrupted (%d) (%s)\n", __LINE__, pSem->name);
               return bosSignalMapSigToStatus();  /* interrupted */
            }
            atomic_dec( &pSem->bosCount );
         }
         else  /* not BOS_WAIT_FOREVER */
         {
            struct             timer_list Timer;
            int                timerRunning;
            int                flag = 0;                 /* Dlag set when signal is received */
            struct TIMER_DATA  data = { *sem, current }; /* Data to be passed to timer */

            if (ticks == 0)
            {
               ticks = 1;
            }

            init_timer( &Timer );
            Timer.function = bcmOsSemTimeOutHdlr;
            Timer.data = (unsigned long)(&data);
            Timer.expires = jiffies + ticks;
            add_timer( &Timer );

            atomic_inc( &pSem->bosCount );
            if ( bos_down_interruptible( *sem, &flag ) && !flag ) /* Interrupted by another signal? */
            {
               del_timer_sync( &Timer );
               atomic_dec( &pSem->bosCount );
               printk("bosSemTimedTake interrupted (%d) (%s)\n", __LINE__, pSem->name);
               return bosSignalMapSigToStatus();  /* interrupted */
            }
            else
               flush_signals(current);

            atomic_dec( &pSem->bosCount );

            timerRunning  = del_timer_sync( &Timer );
            /* del_timer returns: 0= expired, 1= active */

            if ( !timerRunning )
            {
               status = BOS_STATUS_TIMEOUT; /* Timed out. */
            }
         }
      }
      else
      {
         status = BOS_STATUS_ERR; /* semaphore deleted */
      }
   }
   else
   {
      printk("bosSemTimedTake BOS_STATUS_ERR (%d) (%s)\n", __LINE__, pSem->name);
      /* Unable to take semaphore, because we are running in a bottom half */
      status = BOS_STATUS_ERR;
   }

   return( status );
} /* bosSemTimedTake */

#endif /* SEMFIX */

/*
*****************************************************************************
 * Function Name: bosSemTakeBlock
 * Description  : This function is used by tasks to take a BOS semaphore.
 *                The task could end up being blocked waiting for the semaphore.
 * Returns      : BOS_STATUS_OK  = successful
 *                BOS_STATUS_ERR = failed
*****************************************************************************
*/
BOS_STATUS bosSemTakeBlock( BOS_SEM *sem )
{
   BOS_SEMAPHORE* pSem;
   BOS_STATUS     status;

   BOS_ASSERT( gBosSemInitialized );
   BOS_ASSERT( sem != NULL );
   BOS_ASSERT( *sem != NULL );

   pSem     = (BOS_SEMAPHORE*)(*sem);
   status   = BOS_STATUS_OK;

   /* Cannot take semaphore if running in a bottom half */
   if ( !in_softirq() )
   {
      if ( pSem->bInUse == BOS_TRUE )
      {
         atomic_inc( &pSem->bosCount );
         if( down_killable( *sem ) )
         {
            atomic_dec( &pSem->bosCount );
            printk("bosSemTakeBlock [%d]: killed (%s)\n", __LINE__, pSem->name);
            return bosSignalMapSigToStatus();  /* interrupted */
         }
         atomic_dec( &pSem->bosCount );
      }
      else
      {
         status = BOS_STATUS_ERR;   /* semaphore deleted */
      }
   }
   else
   {
      /* Unable to take semaphore, because we are running in a bottom half */
      status = BOS_STATUS_ERR;
   }

   return ( status );
} /* bosSemTakeBlock */

/*
*****************************************************************************
* Function Name: bosSemGive
* Description  : This function is used by tasks to release a semaphore once
*                they are done with it. It gives the other tasks (waiting
*                for the semaphore) an opportunity to take it.
* Returns      : BOS_STATUS_OK  = successful
*                BOS_STATUS_ERR = failed
*****************************************************************************
*/
BOS_STATUS bosSemGive( BOS_SEM *sem )
{
   BOS_STATUS     status   = BOS_STATUS_OK;
   BOS_SEMAPHORE* pSem     = (BOS_SEMAPHORE*)(*sem);

   if( pSem->bInUse == BOS_TRUE )
   {
      up( *sem );
   }
   else
   {
      status = BOS_STATUS_ERR;
   }

   if ( status != BOS_STATUS_OK )
   {
      return CheckSemStatusLinuxKernel( status, BOS_SEM_GIVE_FUNC_ID );
   }

   return ( BOS_STATUS_OK );

} /* bosSemGive */

/**
 * @addtogroup bosSemInternal
 * @{
 */

/***************************************************************************/
/**
*  Translate a LinuxKernel error code into a BOS error code and report any
*  errors encountered.
*
*  @param   err      (in)  LinuxKernel Error code.
*  @param   funcId   (in)  ID of the function that we're checking for.
*/

static BOS_STATUS CheckSemStatusLinuxKernel( int err, BOS_SEM_FUNC_ID funcId )
{
   return bosErrorCheckStatusLinuxKernel( err,
                                      BOS_MAKE_ERROR_SOURCE( BOS_SEM_MODULE, funcId ));

} /* CheckSemStatusLinuxKernel */

/***************************************************************************/
/**
*  Timeout handler for bcmOsSemTimedTake.
*
*/
#if (SEMFIX == 0)
static void bcmOsSemTimeOutHdlr( BOS_UINT32 arg )
{
   BOS_SEMAPHORE *pSem = (BOS_SEMAPHORE*)arg;

   if(pSem && pSem->bInUse == BOS_TRUE)
        up(&pSem->sem);
} /* bcmOsSemTimeOutHdlr */

/** @} */

#else

static void bcmOsSemTimeOutHdlr( BOS_UINT32 arg )
{
   struct TIMER_DATA *data = (struct TIMER_DATA *)arg;

   if ( data && data->sem && (data->sem->bInUse == BOS_TRUE) )
   {
      force_sig(SIGUSR1, data->tsk/*, 1*/); /* Send signal to the task */
   }
} /* bcmOsSemTimeOutHdlr */

/** @} */

#endif /* SEMFIX */

#endif   /* BOS_CFG_SEM */
