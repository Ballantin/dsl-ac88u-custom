/*
 * Bsafe uHSM driver device name.
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_devname.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#ifndef _BSAFE_DEVNAME_H_
#define _BSAFE_DEVNAME_H_

#ifdef BSAFE_SOCKET_SIM
#define BSAFE_DEVICE_NAME "10.18.10.92"
#else
#define BSAFE_DEVICE_NAME "/dev/bsafe_uhsm"
#endif

#endif /* _BSAFE_DEVNAME_H_ */
