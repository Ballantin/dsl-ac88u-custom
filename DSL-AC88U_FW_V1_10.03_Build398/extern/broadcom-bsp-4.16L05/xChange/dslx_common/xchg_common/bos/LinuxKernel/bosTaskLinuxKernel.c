/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosTaskLinuxKernel.c
*
*  @brief   Linux kernel implementation of the BOS Task Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */


#include <bosCfg.h>

#if BOS_CFG_TASK

#include <linux/version.h>
#include <linux/kernel.h>  /* Include this file as first kernel include file */
#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/param.h>
#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/nsproxy.h>
#include <linux/semaphore.h>
#include <linux/completion.h>

#include <string.h>
#include <bosTypes.h>
#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosTask.h>
#include <bosTaskPrivate.h>
#include <bosLog.h>

#if BOS_CFG_EVENT
#include <bosEventPrivate.h>
#endif

#include <str.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Kernel thread parameters */
typedef struct threadParms
{
   struct task_struct*  thread;
   struct completion    done;
   BOS_TASK_ARG         argument;
   void*                taskEntry;
   const char*          name;
} threadParms;

/* ---- Private Variables ------------------------------------------------ */

/**
 * Indicates that the Task module has been initialized.
 */

static   BOS_BOOL gBosTaskInitialized = BOS_FALSE;


/* ---- Private Function Prototypes -------------------------------------- */

static BOS_STATUS CheckTaskStatusLinux( int err, BOS_TASK_FUNC_ID funcId );
static int newThread (void* arg);
static int kernel_sched_setscheduler(pid_t pid, int policy, struct sched_param *param);

/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup bosTask
 * @{
 */

/***************************************************************************/

BOS_STATUS bosTaskInit( void )
{
   BOS_ASSERT( !gBosTaskInitialized );

   gBosTaskInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosTaskInit */

/***************************************************************************/

BOS_STATUS bosTaskTerm( void )
{
   BOS_ASSERT( gBosTaskInitialized );

   gBosTaskInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosTaskTerm */

/***************************************************************************/

BOS_STATUS bosTaskCreate
(
   const char            *name,
   int                   stackSize,
   BOS_TASK_CLASS        taskClass,
   BOS_TASK_ENTRY        taskEntry,
   BOS_TASK_EXTENDED_ARG *argument,
   BOS_TASK_ID           *taskId
)
{
   int rc;
   unsigned long      rt_priority;
   struct threadParms tp;
   mm_segment_t old_fs;
   struct sched_param param;

   BOS_ASSERT( gBosTaskInitialized );

   memset(&tp, 0, sizeof(threadParms));
   tp.taskEntry = taskEntry;
   tp.argument = (argument != NULL) ? argument->argument: 0;
   tp.name     = name;

   init_completion(&tp.done);

   rt_priority = bosTaskCalcNativePriority( taskClass );
   if ( rt_priority > 99 )
   {
      return BOS_STATUS_ERR;
   }

   /* Clone FS, files, and signal handler */
   tp.thread = kthread_run( newThread, (void*)&tp, "%s%s", BOS_TASK_NAME_PREFIX, name );
   if(IS_ERR_OR_NULL(tp.thread))
   {
      return CheckTaskStatusLinux( (unsigned int)tp.thread, BOS_TASK_CREATE_FUNC_ID );
   }
   wait_for_completion(&tp.done);

   /* Set the scheduling parameters for the new task */
   memset (&param, 0, sizeof(param));
   param.sched_priority = rt_priority;

   old_fs = get_fs();
   set_fs(get_ds());
   rc = kernel_sched_setscheduler(tp.thread->pid, (rt_priority > 0)? SCHED_FIFO: SCHED_NORMAL, &param);
   if (rc < 0)
   {
      printk ("Failed to set priority. err=%d\n", rc);
   }
   set_fs(old_fs);

#if CONFIG_SMP
   /* Set the task affinity if it cannot run on both TPs */
   if ( (argument != NULL) && (argument->affinity != BOS_TASK_AFFINITY_ALL_TP)  )
   {
      printk("Binding 0x%08x\n", (unsigned int)tp.thread);
      kthread_bind(tp.thread, argument->affinity);
   }
#endif /* CONFIG_SMP */

   /* Restart thread */
   wake_up_process(tp.thread);
   wait_for_completion(&tp.done);

   *taskId = (BOS_TASK_ID)tp.thread;
   blogStrUsr(( gBosBlog, BOS_LOG_PARAMS, "bcmOsTaskCreate: \n"));
   blogStrUsr(( gBosBlog, BOS_LOG_PARAMS, " TASK NAME      = %s \n", name));
   blogStrUsr(( gBosBlog, BOS_LOG_PARAMS, " TASK_PRIORITY  = %lu \n", rt_priority));

#if BOS_CFG_EVENT
   /*
    * Register the task with the Event module.
    */
   bosEventRegisterTask( taskId );
#endif

   return ( BOS_STATUS_OK );
} /* bosTaskCreate */

/***************************************************************************/

BOS_STATUS bosTaskDestroy ( BOS_TASK_ID *taskId )
{
   BOS_STATUS status;

#if BOS_CFG_EVENT
   /*
    * Un-register the task with the Event module. This needs to be performed
    * first since taskDelete will not return if the caller is committing suicide.
    */
   bosEventUnRegisterTask( taskId );
#endif

   if( current == (struct task_struct*)(*taskId) )
   {
      /* Daemonize voice kernel thread before exiting sine
      ** we don't want to send SIGCHLD to the parent,
      ** which might be another kernel thread.*/
      daemonize("VKD");

      /* Nothing else to do here. The process will exit from the main thread */
      status = BOS_STATUS_OK;
   }
   else
   {
      force_sig(SIGKILL, (struct task_struct*)(*taskId));

      status = BOS_STATUS_OK;
   }

   if ( status != BOS_STATUS_OK )
   {
      return CheckTaskStatusLinux( status, BOS_TASK_DESTROY_FUNC_ID );
   }

   return ( BOS_STATUS_OK );

} /* bosTaskDestroy */

/***************************************************************************/

BOS_STATUS bosTaskSuspend( BOS_TASK_ID *taskId )
{
   return CheckTaskStatusLinux( BOS_STATUS_ERR, BOS_TASK_SUSPEND_FUNC_ID );

} /* bosTaskSuspend */

/***************************************************************************/

BOS_STATUS bosTaskResume( BOS_TASK_ID *taskId )
{
   return CheckTaskStatusLinux( BOS_STATUS_ERR, BOS_TASK_SUSPEND_FUNC_ID );

} /* bosTaskResume */

/***************************************************************************/

BOS_STATUS bosTaskExists( const BOS_TASK_ID *taskId )
{
   return CheckTaskStatusLinux( BOS_STATUS_ERR, BOS_TASK_EXISTS_FUNC_ID );

} /* bosTaskExists */

/***************************************************************************/

BOS_STATUS bosTaskGetPriority( const BOS_TASK_ID *taskId, BOS_TASK_CLASS *taskClass )
{
   return CheckTaskStatusLinux( BOS_STATUS_ERR, BOS_TASK_GET_PRIORITY_FUNC_ID );

} /* bosTaskGetPriority */

/***************************************************************************/

BOS_STATUS bosTaskSetPriority( BOS_TASK_ID *taskId, BOS_TASK_CLASS taskClass )
{
    return CheckTaskStatusLinux( BOS_STATUS_ERR, BOS_TASK_SET_PRIORITY_FUNC_ID );

} /* bosTaskSetPriority */

/***************************************************************************/

BOS_STATUS bosTaskGetMyTaskId( BOS_TASK_ID *taskId )
{
   *taskId = (BOS_TASK_ID)(current);
   return ( BOS_STATUS_OK );
} /* bosTaskGetMyTaskId */

/***************************************************************************/

BOS_STATUS bosTaskGetIdFromName( const char *tName, BOS_TASK_ID *taskId )
{
   return CheckTaskStatusLinux( BOS_STATUS_ERR, BOS_TASK_GET_ID_FROM_NAME_FUNC_ID );

} /* bosTaskGetIdFromName */

/***************************************************************************/

BOS_STATUS bosTaskGetNameFromId
(
   const BOS_TASK_ID   *taskId,
   char                *dstTaskName,
   int                  maxLen
)
{
   return CheckTaskStatusLinux( BOS_STATUS_ERR, BOS_TASK_GET_ID_FROM_NAME_FUNC_ID );

} /* bosTaskGetNameFromId */

/** @} */

/**
 * @addtogroup bosTaskInternal
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

BOS_STATUS CheckTaskStatusLinux( int err, BOS_TASK_FUNC_ID funcId )
{
   return bosErrorCheckStatusLinuxKernel( err,
                                      BOS_MAKE_ERROR_SOURCE( BOS_TASK_MODULE, funcId ));

} /* CheckTaskStatusLinux */


/***************************************************************************/
/**
*  Create new kernel thread.
*
*  @param   arg      (in)  kernel thread parameters
*/
static int newThread(void* arg)
{
   threadParms localThreadParms;
   unsigned long state = 0;

   threadParms* pTp  = (threadParms*)arg;
   pTp->thread       = current;

   /* Stop thread to set its scheduling parameters and bind it to a CPU if needed */
   set_current_state(TASK_UNINTERRUPTIBLE);
   complete(&pTp->done);
   schedule();

   /* Copy the passed-in thread parameters to a local structure and
   ** after the semaphore give ( 'up' call below ) do not use the
   ** pointer to the passed-in thread parameters anymore because
   ** the pointed data might be deallocated in other thread.
   ** Use only the local copy of thread parameters after the 'up' call. */
   memcpy( &localThreadParms, pTp, sizeof(localThreadParms) );

   /* set signal mask to what we want to respond */
   spin_lock_irqsave(&current->sighand->siglock, state);
   siginitsetinv(&current->blocked, sigmask(SIGKILL)|sigmask(SIGINT)|sigmask(SIGTERM)|sigmask(SIGHUP));
   recalc_sigpending();
   spin_unlock_irqrestore(&current->sighand->siglock, state);

   complete(&pTp->done);

   /* call user task entry function */
   if ( localThreadParms.taskEntry )
      (*(BOS_TASK_ENTRY)localThreadParms.taskEntry)(localThreadParms.argument);

   return 0;
}

/**
*  Invoke sched_setscheduler syscall
*
*/
#if defined(CONFIG_MIPS)
static int kernel_sched_setscheduler(pid_t pid, int policy, struct sched_param *param)
{
    register unsigned long __a0 asm("$4") = (unsigned long) pid;
    register unsigned long __a1 asm("$5") = (unsigned long) policy;
    register unsigned long __a2 asm("$6") = (unsigned long) param;
    register unsigned long __a3 asm("$7");
    unsigned long __v0;

    __asm__ volatile ("					\n"
    "	.set	noreorder				\n"
    "	li	$2, %5		# __NR_sched_setscheduler		\n"
    "	syscall						\n"
    "	move	%0, $2					\n"
    "	.set	reorder					\n"
    : "=&r" (__v0), "=r" (__a3)
    : "r" (__a0), "r" (__a1), "r" (__a2), "i" (__NR_sched_setscheduler)
    : "$2", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24",
      "memory");

    if (__a3 == 0)
        return __v0;

    return -__v0;
}
#elif defined(CONFIG_ARM)
#  ifdef CONFIG_OABI_COMPAT
#  define __NR_OABI_sched_setscheduler (__NR_sched_setscheduler | __NR_OABI_SYSCALL_BASE)
static int kernel_sched_setscheduler(pid_t pid, int policy, struct sched_param *param)
{
   unsigned int _sys_result;
   {
      register int _a1 asm ("a1") = (int) (pid);
      register int _a2 asm ("a2") = (int) (policy);
      register int _a3 asm ("a3") = (int) (param);

      asm volatile ("swi	%1	@ syscall "
           : "=r" (_a1)
           : "i" (__NR_OABI_sched_setscheduler), "r" (_a1), "r" (_a2), "r" (_a3)
           : "memory");
      _sys_result = _a1;
   }
   return (int) _sys_result;
}
#  else
static int kernel_sched_setscheduler(pid_t pid, int policy, struct sched_param *param)
{
   unsigned int _sys_result;
   {
      register int _a1 asm ("a1") = (int) (pid);
      register int _a2 asm ("a2") = (int) (policy);
      register int _a3 asm ("a3") = (int) (param);
      register int _scno asm ("r7") = (int) (__NR_sched_setscheduler);

      asm volatile ("swi	%1	@ syscall "
           : "=r" (_a1)
           : "i" (__NR_sched_setscheduler), "r" (_a1), "r" (_a2), "r" (_a3), "r" (_scno)
           : "memory");
      _sys_result = _a1;
   }
   return (int) _sys_result;
}
#  endif /* CONFIG_OABI_COMPAT */
#else
#error "kernel_sched_setscheduler is not implemented for this CPU architecture!"
#endif

/** @} */

#endif /* BOS_CFG_TASK */

