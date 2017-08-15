/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    time.h
*
*  @brief   Contains the time interface.
*
****************************************************************************/
/**
*  @defgroup   time.h
*
*  @brief      Wrapper for C run-time time functions
*
****************************************************************************/

#if !defined( CRT_TIME_H )
#define CRT_TIME_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include <linux/version.h>
#include <kernelMainWrapper.h>
#include <linux/time.h>

/* The structure tm is not defined in the Linux kernel
** so we have to define it here. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};
#endif
#endif /* CRT_TIME_H */
