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

#ifndef __EGPHY_DRV_IMPL2_H
#define __EGPHY_DRV_IMPL2_H

#include "bl_os_wraper.h"

#define RDB_REG_ACCESS                  0x8000

void egphy_reset(uint32_t phy_map);
void egphy_auto_enable_extra_config(uint32_t is_hsgmii, uint32_t phy_addr);
uint16_t egphy_read_register(uint32_t phy_addr, uint16_t reg_addr);
int32_t egphy_write_register(uint32_t phy_addr, uint16_t reg_addr, uint16_t value);
uint16_t sgmii_read_register(uint32_t phy_addr, uint16_t reg_addr);

#endif
