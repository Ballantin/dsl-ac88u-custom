/*
   Copyright (c) 2015 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2015:DUAL/GPL:standard
    
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

#include "extphy_drv_impl2.h"
#include "mdio_drv_impl2.h"
#include "boardparms.h"

void extphy_reset(void)
{
    /* External phy initialization */
}

void extphy_auto_enable_extra_config(void)
{
    /* Extra configuration for the external phy when it becomes enabled */
}

uint16_t extphy_read_register(uint32_t phy_addr, uint16_t reg_addr)
{
    uint16_t value = 0;

    if (mdio_read_c22_register(MDIO_EXT, phy_addr, reg_addr, &value) != MDIO_OK)
        printk("failed to read external phy register: phy_addr=%ul reg_addr=%ul\n", phy_addr, reg_addr);

    return value;
}
EXPORT_SYMBOL(extphy_read_register);

int32_t extphy_write_register(uint32_t phy_addr, uint16_t reg_addr, uint16_t value)
{
    int32_t ret;

    if ((ret = mdio_write_c22_register(MDIO_EXT, phy_addr, reg_addr, value)) != MDIO_OK)
        printk("failed to write external phy register: phy_addr=%ul reg_addr=%ul\n", phy_addr, reg_addr);

    return ret;
}
EXPORT_SYMBOL(extphy_write_register);
