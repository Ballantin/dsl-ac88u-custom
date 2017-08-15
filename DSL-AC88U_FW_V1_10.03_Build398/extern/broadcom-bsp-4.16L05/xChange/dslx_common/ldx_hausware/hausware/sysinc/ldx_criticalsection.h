/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************/
#ifndef LDX_CRITICALSECTION_H
#define LDX_CRITICALSECTION_H


#include <xcfg_criticalsection.h>
#include <lhapi.h>

/*
** Using the direct function call will produce a smaller code size.
** Calling the Lock and Unlock indirectly through the function pointers
** allows us to jump directly to the os function (e.g., intLock).  A two
** step call to intLock (i.e., lhapiBiosLock->intLock) would likely result
** in an extra cache miss.  Since these functions are called frequently
** we want to avoid cache missed.
*/
#if XCFG_CRITICALSECTION_USE_DIRECT_FUNCTION

/* Protect the globals in the Kernel layer
 */
#define ldxProtectKnlStart(context)      ((context)=lhapiBiosLock())
#define ldxProtectKnlEnd(context)        lhapiBiosUnlock((context))

/* Protect the globals in the Kernel layer. These are only used for non-distributed 
 * LDX configurations running on SMP systems. Defines are empty stubs since we never
 * use direct function calls in non-dist/SMP configurations.
 */
#define ldxProtectKnlStartSMP(context)   (void)(context)
#define ldxProtectKnlEndSMP(context)

/* Protect the globals in the HalShim layer
 */
#define ldxProtectHalShimStart(context)  ((context)=lhapiBiosLock())
#define ldxProtectHalShimEnd(context)    lhapiBiosUnlock((context))

/* Protect everything else.  Ideally, we should be able to classify every
 * protection, but until then we will use this macro.
 */
#define ldxProtectGenericStart(context)   ((context)=lhapiBiosLock())
#define ldxProtectGenericEnd(context)     lhapiBiosUnlock((context))

#else  /* XCFG_CRITICALSECTION_USE_DIRECT_FUNCTION */

#define ldxProtectKnlStart(context)      ((context)=LHAPI_gpFncLock())
#define ldxProtectKnlEnd(context)        LHAPI_gpFncUnlock((context))

/* Protect the globals in the Kernel layer. These are only used for non-distributed 
 * LDX configurations running on SMP systems.
 */ 
#define ldxProtectKnlStartSMP(context)     ((context)=LHAPI_gpFncLockSMP())
#define ldxProtectKnlEndSMP(context)       LHAPI_gpFncUnlockSMP((context))


#define ldxProtectHalShimStart(context)  ((context)=LHAPI_gpFncLock())
#define ldxProtectHalShimEnd(context)    LHAPI_gpFncUnlock((context))

#define ldxProtectGenericStart(context)          ((context)=LHAPI_gpFncLock())
#define ldxProtectGenericEnd(context)          LHAPI_gpFncUnlock((context))

#endif /* XCFG_CRITICALSECTION_USE_DIRECT_FUNCTION */

#endif /* LDX_CRITICALSECTION_H */

