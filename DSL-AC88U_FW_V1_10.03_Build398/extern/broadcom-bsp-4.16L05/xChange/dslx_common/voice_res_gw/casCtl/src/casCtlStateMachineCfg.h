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
*    Filename: casCtlStateMachineCfg.h
*
****************************************************************************
*    Description:
*
*      Config values for CAS control state machine.
*
****************************************************************************/

#ifndef CAS_CTL_STATE_MACHINE_CFG_H
#define CAS_CTL_STATE_MACHINE_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#define CAS_STANBDY_THRESH    8000  /* Leave the CAS in low processing mode
                                    ** if no state change for 8s */

#define CAS_MONITOR_RATE      20    /* Run the CAS Monitor state machine every 64 ms */

/* FXS Ring Detect parameters */
#define FXS_RINGON           20     /* Ring may be noisy, so make this short */
#define FXS_RINGOFF         450     /* Just less than the 500ms delay for CLID,
                                    ** but long enough to skip some custom ring
                                    ** gaps */
#define FXS_CLIDRXEND      3500     /* 3.5 seconds from ring off to end of 
                                    ** CLIDRX interval */ 
#define FXS_RINGEND        6000     /* Six seconds without ring detected */

/* FXS Loop current detection parameters */
#define FXS_LOOPDEGLITCH    500     /* Time to allow loop current to settle */

#define FXO_RINGGLITCHREMOVE   100  /* Some SLICs cause an OFF HOOK glitch while ringing.
                                    ** These can be up to 100ms. Since we should connect
                                    ** if > 350ms, let's remove all glitches that are <200ms */

#ifdef __cplusplus
}
#endif

#endif   /* CAS_CTL_STATE_MACHINE_CFG_H */
