/*
<:copyright-BRCM:2013:DUAL/GPL:standard

   Copyright (c) 2013 Broadcom 
   All Rights Reserved

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

#ifndef __BCM63XX_PCIE_H
#define __BCM63XX_PCIE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <bcm_map_part.h>
#include <bcm_intr.h>
#include <board.h>
#include <pmc_pcie.h>
#include <pmc_drv.h>
#include <shared_utils.h>

#if 0
#define DPRINT(x...)                printk(x)
#define TRACE()                     DPRINT("%s\n",__FUNCTION__)
#define TRACE_READ(x...)            printk(x)
#define TRACE_WRITE(x...)           printk(x)
#else
#undef  DPRINT
#define DPRINT(x...)
#define TRACE()
#define TRACE_READ(x...)
#define TRACE_WRITE(x...)
#endif

/*PCI-E */
#define BCM_BUS_PCIE_ROOT           0
#if defined(PCIEH) && defined(PCIEH_1)
#define NUM_CORE                    2
#else
#define NUM_CORE                    1
#endif


/*
 * Per port control structure
 */
struct bcm63xx_pcie_port {
    unsigned char * __iomem regs;
    struct resource *owin_res;
    unsigned int irq;
    struct hw_pci hw_pci;

    bool enabled;                   // link-up
    bool link;                      // link-up
    bool saved;                     // pci-state saved
};

#ifdef __cplusplus
}
#endif

#endif /* __BCM63XX_PCIE_H */
