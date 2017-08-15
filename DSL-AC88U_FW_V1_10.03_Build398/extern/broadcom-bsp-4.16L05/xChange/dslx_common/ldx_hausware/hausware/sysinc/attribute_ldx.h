/****************************************************************************
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
****************************************************************************/
#ifndef ATTRIBUTE_LDX_H
#define ATTRIBUTE_LDX_H

#ifndef XCFG_ATTRIBUTE_H
#  include <xcfg_attribute.h>
#endif

/*
** Globally disable attribute usage
*/
#if (XCFG_ATTRIBUTE_SUPPORT == 0)

/* empty macros to disable attribute support */

#define ATTR_SECT_PROG(name)
#define ATTR_SECT_GENERIC(name)
#define ATTR_ALIGN32
#define ATTR_SECT_INIT(name)

#else

#include <haus_cpu.h>

/*
** To keep the status quo the default to these attributes
** will be derived from the cpu type.  Doing this is not totally correct
** but it will do for now
**
** ARM builds for Linux Kernel are built as partially linked executables.
** I am not totally sure if such objects can handle program/data section
** other than .text, .data, .bss, etc. Theoretically it should be possible.
** For for simplicity, custom sections will be disabled.
*/
#if defined(HAUS_CPU_MIPS) || defined(HAUS_CPU_ARM)

#if !defined(ATTR_SECT_PROG)
#define ATTR_SECT_PROG(name)
#endif

#if !defined(ATTR_SECT_INIT)
#define ATTR_SECT_INIT(name)     __attribute__ ((section ("INITDATA")))
#endif

#if !defined(ATTR_SECT_GENERIC)
#define ATTR_SECT_GENERIC(name)
#endif

#if !defined(ATTR_ALIGN32)
#define ATTR_ALIGN32             __attribute__((aligned (4)))
#endif

/* Deprecated definition */
#if !defined(ATTRIBUTE_SECTION)
#define ATTRIBUTE_SECTION(name)
#endif

#endif

/*
**
*/
#if defined(HAUS_CPU_ZSP) && HAUS_CPU_ZSP

#if !defined(ATTR_SECT_PROG)
#define ATTR_SECT_PROG(name)     __attribute__((section(name)))
#endif

#if !defined(ATTR_SECT_GENERIC)
#define ATTR_SECT_GENERIC(name)  __attribute__((section(name)))
#endif

#if !defined(ATTR_SECT_INIT)
#define ATTR_SECT_INIT(name)     __attribute__ ((section (name)))
#endif

#if !defined(ATTR_ALIGN32)
#define ATTR_ALIGN32
#endif
#endif

#if defined(HAUS_CPU_X86) && HAUS_CPU_X86

#if !defined(ATTR_SECT_PROG)
#define ATTR_SECT_PROG(name)
#endif

#if !defined(ATTR_SECT_GENERIC)
#define ATTR_SECT_GENERIC(name)
#endif

#if !defined(ATTR_SECT_INIT)
#define ATTR_SECT_INIT(name)
#endif

#if !defined(ATTR_ALIGN32)
#define ATTR_ALIGN32
#endif
#endif

#endif /* XCFG_ATTRIBUTE_SUPPORT == 0 */

#endif /* ATTRIBUTE_LDX_H */
