#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_



#ifdef	linux
/*
 * If this is either a Linux hybrid build or the per-port code of a hybrid build
 * then use the Linux header files to get some of the typedefs.  Otherwise, define
 * them entirely in this file.  We can't always define the types because we get
 * a duplicate typedef error; there is no way to "undefine" a typedef.
 * We know when it's per-port code because each file defines LINUX_PORT at the top.
 */
#if !defined(LINUX_HYBRID) || defined(LINUX_PORT)
#define TYPEDEF_UINT
#define TYPEDEF_USHORT
#define TYPEDEF_ULONG
#ifdef __KERNEL__
#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 19))
#define TYPEDEF_BOOL
#endif	/* >= 2.6.19 */
#endif	/* __KERNEL__ */
#endif  /* !defined(LINUX_HYBRID) || defined(LINUX_PORT) */
#endif	/* linux */


/* pick up ushort & uint from standard types.h */
#if defined(linux) && defined(__KERNEL__)

/* See note above */
#if !defined(LINUX_HYBRID) || defined(LINUX_PORT)
#ifdef USER_MODE
#include <sys/types.h>
#else
#include <linux/types.h>	/* sys/types.h and linux/types.h are oil and water */
#endif /* USER_MODE */
#endif /* !defined(LINUX_HYBRID) || defined(LINUX_PORT) */

#else

#include <sys/types.h>

#endif /* linux && __KERNEL__ */

/* define uchar, ushort, uint, ulong */
typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;

/* define [u]int8/16/32/64, uintptr */
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;
typedef unsigned long long uint64;
typedef unsigned int	uintptr;
typedef signed char	int8;
typedef signed short	int16;
typedef signed int	int32;
typedef signed long long int64;

/* define float32/64, float_t */
typedef float		float32;
typedef double		float64;

#endif /* __TYPEDEFS_H__ */
