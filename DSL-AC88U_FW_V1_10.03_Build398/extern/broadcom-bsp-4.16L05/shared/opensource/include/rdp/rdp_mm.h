/*
   Copyright (c) 2014 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2014:DUAL/GPL:standard
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
    :>
*/

/******************************************************************************/
/*                                                                            */
/* File Description:                                                          */
/*                                                                            */
/* This file contains inline functions for runner host memory management      */
/*                                                                            */
/******************************************************************************/
#ifndef _RDP_MM_H_
#define _RDP_MM_H_

#ifdef CM3390
#define VIRT_TO_PHY(_a)   (_a)
#endif

#ifndef CM3390
#if !defined(RDP_SIM)
static inline void *rdp_mm_aligned_alloc(uint32_t size, uint32_t *phy_addr_p)
{
#if defined(__KERNEL__) && defined(CONFIG_ARM)
    dma_addr_t phy_addr;
    uint32_t size32 = (uint32_t)(size + (sizeof(void *)) + 3) & (uint32_t)(~3); /* must be multiple of 4B */
    /* memory allocation of NONCACHE_MALLOC for ARM is aligned to page size which is aligned to cache */
    uint32_t *mem = (uint32_t *)NONCACHED_MALLOC_ATOMIC(size32, &phy_addr);
    if (unlikely(mem == NULL))
        return NULL;
    /*	printk("\n\tsize %u, size32 %u, mem %p, &mem[size] %p, phy_addr 0x%08x\n\n", size, size32, mem, &mem[(size32-sizeof(void *))>>2], phy_addr); */
    mem[(size32-sizeof(void *))>>2] = phy_addr;
    *phy_addr_p = (uint32_t)phy_addr;
    return (void *)mem;
#else
    uint32_t cache_line = DMA_CACHE_LINE;
    void *mem = (void *)CACHED_MALLOC_ATOMIC(size + cache_line + (sizeof(void *)) - 1);
    void **ptr = (void **)((long)(mem + cache_line + (sizeof(void *))) & ~(cache_line - 1));
    ptr[-1] = mem;
    *phy_addr_p = (uint32_t)VIRT_TO_PHYS(ptr);
    INV_RANGE((uint32_t)ptr, size);
    return (void *)CACHE_TO_NONCACHE(ptr);
#endif
}

static inline void rdp_mm_aligned_free(void *ptr, uint32_t size)
{
#if defined(__KERNEL__) && defined(CONFIG_ARM)
    uint32_t size32 = (uint32_t)(size + (sizeof(void *)) + 3) & (uint32_t)(~3); /* must be multiple of 4B */
    NONCACHED_FREE(size32, ptr, ((uint32_t *)(ptr))[(size32-(sizeof(void *)))>>2]);
#else
    CACHED_FREE(((void**)ptr)[-1]);
#endif
}

#else /* for simulator */

static inline void *rdp_mm_aligned_alloc(uint32_t size, uint32_t *phy_addr_p)
{
	*phy_addr_p = (int32_t)malloc(size);
	return (void*)*phy_addr_p;
}

static inline void rdp_mm_aligned_free(void *ptr, uint32_t size)
{
	free(ptr);
}
#endif
#endif /* CM3390 */

static inline void rdp_mm_cpyl_context(void *__to, void *__from, unsigned int __n)
{
    unsigned int src = (unsigned int)__from;
    unsigned int dst = (unsigned int)__to;
    int i, n = __n / 4;

    for (i = 0; i < n; i++, src += 4, dst += 4)
    {
        if ((i & 0x3) == 3)
            continue;

        *(volatile unsigned int *)dst = swap4bytes(*(volatile unsigned int *)src);
    }
}

#endif
