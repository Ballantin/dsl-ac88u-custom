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

#ifdef CONFIG_BRCM_PCIE_PLATFORM

/* current linux kernel doesn't support pci bus rescan if we
 * power-down then power-up pcie.
 *
 * work-around by saving pci configuration after initial scan and
 * restoring it every time we repower pcie (implemented by module
 * init routine)
 *
 * module exit function powers down pcie
 */
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <bcm_map_part.h>
#include <pmc_pcie.h>

extern void bcm63xx_pcie_aloha(int hello);

static __init int bcm_mod_init(void)
{

	/* first invocation: save pci configuration
	 * subsequent: repower and restore configuration
	 */
	bcm63xx_pcie_aloha(1);

	return 0;
}

static void bcm_mod_exit(void)
{
	/* power down pcie */
	bcm63xx_pcie_aloha(0);
}

module_init(bcm_mod_init);
module_exit(bcm_mod_exit);

MODULE_LICENSE("GPL");

#endif
