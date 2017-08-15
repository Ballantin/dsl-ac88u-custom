/*
   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2013:DUAL/GPL:standard
    
    Unless you and Broadcom execute a separate written software license
    agreement governing use of this software, this software is licensed
    to you under the terms of the GNU General Public License version 2
    (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
    with the following added to such license:
    
       As a special exception, the copyright holders of this software give
       you permission to link this software with independent modules, and
       to copy and distribute the resulting executable under terms of your
       choice, provided that you also meet, for each linked independent
       module, the terms and conditions of the license of that module.
       An independent module is a module which is not derived from this
       software.  The special exception does not apply to any modifications
       of the software.
    
    Not withstanding the above, under no circumstances may you combine
    this software in any way with any other Broadcom software provided
    under a license other than the GPL, without Broadcom's express prior
    written consent.
    
:>
*/

#ifndef _BL_LILAC_DRV_RUNNER_H
#define _BL_LILAC_DRV_RUNNER_H

#if defined(FIRMWARE_INIT)

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "netinet/in.h"

#define CACHED_MALLOC(x) ((uint32_t) x)

#else

#include "bl_os_wraper.h"

#ifdef LINUX_KERNEL

#include <access_macros.h>
#include <linux/string.h>

#else

#include "access_macros.h"

#endif /* LINUX_KERNEL */

#endif /* defined(FIRMWARE_INIT) */


#if defined(DSL_63138) || defined(DSL_63148)
#if defined(_CFE_)
#include "bcm_mm.h"
#define RDP_CPU_RING_MAX_QUEUES             1
#define RDP_WLAN_MAX_QUEUES                 0

#elif defined(__KERNEL__)
#include "bcm_mm.h"
#define RDP_CPU_RING_MAX_QUEUES             RDPA_CPU_MAX_QUEUES
#define RDP_WLAN_MAX_QUEUES	                RDPA_WLAN_MAX_QUEUES
#if !defined(DSL_63138) && !defined(DSL_63148)
extern const bdmf_attr_enum_table_t rdpa_cpu_reason_enum_table;
#endif

#else
#define VIRT_TO_PHYS(_addr)				(((uint32_t)_addr)&0x1FFFFFFF)
#endif
#endif /* DSL */

#ifndef __PACKING_ATTRIBUTE_STRUCT_END__
#include "packing.h"
#endif

#ifdef RDD_BASIC
#include "rdp_runner.h"
#include "rdpa_types.h"
#else
#if !defined(FIRMWARE_INIT) || defined(BDMF_SYSTEM_SIM)
#include "bdmf_system.h"
#else
#define bdmf_sysb void *
static inline void *bdmf_sysb_data(const bdmf_sysb sysb)
{
    return ((void *)sysb);
}
#endif
#include "rdp_runner.h"
#include "rdpa_types.h"
#include "rdpa_ip_class_basic.h"
#include "rdpa_ingress_class_basic.h"
#include "rdpa_cpu_basic.h"
#include "rdpa_mcast.h" /* DSL */
#if defined(DSL_63138) || defined(DSL_63148)
#include "rdpa_l2_ucast.h"
#endif /* DSL */
#include "rdpa_ipsec.h"
#endif

#if !defined(FIRMWARE_INIT) || defined(BDMF_SYSTEM_SIM)

#include "rdp_drv_bpm.h"

/* temporary until complete stratosphere removal */

#ifndef RDD_BASIC

#undef malloc
/* Full RDD as part of RDPA */
#define malloc(s) bdmf_alloc(s)

#else

#define bdmf_fastlock int
#define DEFINE_BDMF_FASTLOCK(lock) int lock = 0

#if !defined _CFE_

#undef malloc
#undef printf
/* Minimal RDD for builtr-in kernel drivers */
#define malloc(s) kmalloc(s, GFP_KERNEL)
#define printf    printk

#endif /* !defined _CFE_ */

#endif /* RDD_BASIC */

#else

#define bdmf_fastlock int
#define DEFINE_BDMF_FASTLOCK(lock) int lock = 0

#endif /* !defined(FIRMWARE_INIT) */

#if !defined(BDMF_SYSTEM_SIM)
#define rdd_print(fmt, args...) printk(fmt, ##args)
#else
#define rdd_print(fmt, args...) printf(fmt, ##args)
#endif

/* task addresses labels from fw compilation */
#include "rdd_runner_a_labels.h"
#include "rdd_runner_b_labels.h"
#include "rdd_runner_c_labels.h"
#include "rdd_runner_d_labels.h"

#include "rdd_defs.h"
#include "rdd_runner_defs_auto.h"
#include "rdd_runner_defs.h"
#include "rdd_data_structures_auto.h"
#include "rdd_data_structures.h"
#include "rdd_common.h"
#include "rdd_ipsec.h"
#include "rdd_cpu.h"
#include "rdd_init.h"
#include "rdd_wlan_mcast.h"
#ifndef RDD_BASIC
#include "rdd_router.h"
#if defined(DSL_63138) || defined(DSL_63148)
#include "rdd_l2_ucast.h"
#endif /* DSL */
#include "rdd_bridge.h"
#include "rdd_interworking.h"
#endif
#include "rdd_lookup_engine.h"
#include "rdd_tm.h"
#endif /* _BL_LILAC_DRV_RUNNER_H */

