/*
 <:copyright-BRCM:2011:DUAL/GPL:standard
 
    Copyright (c) 2011 Broadcom 
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
#include "bcm_OS_Deps.h"
#include "bcmtypes.h"
#include "bcmmii.h"
#include "ethsw_phy.h"
#include "bcmsw.h"
#include "boardparms.h"
#include "bcmswaccess.h"

extern spinlock_t bcm_ethlock_phy_access;
static DEFINE_MUTEX(bcm_phy_mutex);

/*      
    Note: PHY register access need to wait 50us to complete.
    Hardware does not allow another MDIO R/W to be requested before last one
    finishes, or it will screw up forever. So we need to completely serialize
    all access passing through here.
*/
void ethsw_phy_rw_reg(int phy_id, int reg, uint16 *data, int ext_bit, int rd)
{
    uint32 reg_value, read_back, flags;
    static int needIsrDelay = 0;
    int atomic;

    phy_id &= BCM_PHY_ID_M;
    atomic = preempt_count() || irqs_disabled();

    if (atomic == 0)
    {
        mutex_lock(&bcm_phy_mutex);
    }

    spin_lock_irqsave(&bcm_ethlock_phy_access, flags);

    /* If this is an ISR and a task initiated access then went to sleep, 
        we have to wait here until outstanding access finishes */
    if(atomic && needIsrDelay) 
    {
        udelay(60);
    }

taskRetry:
    reg_value = 0;
    ethsw_wreg(PAGE_CONTROL, REG_MDIO_CTRL_ADDR, (uint8 *)&reg_value, 4);

    reg_value = (ext_bit? REG_MDIO_CTRL_EXT: 0) |
        ((phy_id << REG_MDIO_CTRL_ID_SHIFT) & REG_MDIO_CTRL_ID_MASK) |
        (reg  << REG_MDIO_CTRL_ADDR_SHIFT)|
        (rd? (REG_MDIO_CTRL_READ) : (REG_MDIO_CTRL_WRITE | *data));
    ethsw_wreg(PAGE_CONTROL, REG_MDIO_CTRL_ADDR, (uint8 *)&reg_value, 4);

    if(atomic == 0)
    {
        needIsrDelay = 1;
        spin_unlock_irqrestore(&bcm_ethlock_phy_access, flags);
        msleep(1);
        spin_lock_irqsave(&bcm_ethlock_phy_access, flags);

        /* Now compare the R/W control register contents,
            if it changed, IRS has cut in and changed the register,
            we need to retry the operation. If the register content
            is the same, either no IRS cut in, or even IRS cut in it did the
            same R/W operation, so we can use the result directly in either case. */
        ethsw_rreg(PAGE_CONTROL, REG_MDIO_CTRL_ADDR, (uint8 *)&read_back, 4);
        if (read_back != reg_value)
        {
            goto taskRetry;
        }
    }
    else
    {
        udelay(60);
    }

    if (rd)
    {
        ethsw_rreg(PAGE_CONTROL, REG_MDIO_DATA_ADDR, (uint8 *)data, 2);
    }

    needIsrDelay = 0;

    spin_unlock_irqrestore(&bcm_ethlock_phy_access, flags);
    if(atomic == 0)
    {
        mutex_unlock(&bcm_phy_mutex);
    }

}

extern spinlock_t bcm_ethlock_phy_shadow;
void ethsw_ephy_shadow_rw(int phy_id, int bank, uint16 reg, uint16 *data, int write)
{
    uint32 flags;

    spin_lock_irqsave(&bcm_ethlock_phy_shadow, flags);
    _ethsw_ephy_shadow_rw(phy_id, bank, reg, data, write);
    spin_unlock_irqrestore(&bcm_ethlock_phy_shadow, flags);
}

MODULE_LICENSE("GPL");

