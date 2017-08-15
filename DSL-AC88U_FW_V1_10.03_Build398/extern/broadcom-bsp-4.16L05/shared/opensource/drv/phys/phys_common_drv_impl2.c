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

#include "phys_common_drv.h"
#include "egphy_drv_impl2.h"
#include "extphy_drv_impl2.h"
#include "mdio_drv_impl2.h"
#include "boardparms.h"

typedef struct
{
    uint32_t phy_addr;
    uint32_t if_flags;
    mdio_type_t mdio_type;
} phy_cache_t;

static phy_cache_t phy_cache[BP_MAX_SWITCH_PORTS] = { { 0 } };
static uint32_t	phy_cache_initialized = 0;
static const ETHERNET_MAC_INFO *emac_info;

static inline int phy_is_link_up(mdio_type_t mdio_type, uint32_t phy_addr)
{
    uint16_t stat_reg;

    if (mdio_read_c22_register(mdio_type, phy_addr, MII_BMSR, &stat_reg) != MDIO_OK)
        return 0;

    return stat_reg & BMSR_LSTATUS ? 1 :  0;
}

static inline void fill_phy_cache(void)
{
    uint32_t iter;

    if (phy_cache_initialized)
        return;

    if ((emac_info = BpGetEthernetMacInfoArrayPtr()) == NULL)
    {
        printk("Error reading Ethernet MAC info from board params\n");
        return;
    }

    for (iter = 0; iter < BP_MAX_SWITCH_PORTS; iter++)
    {
        if (emac_info->sw.port_map & (1<<iter))
        {
            uint32_t phy_id = emac_info->sw.phy_id[iter];

            phy_cache[iter].phy_addr = phy_id & BCM_PHY_ID_M;
            phy_cache[iter].if_flags = phy_id & MAC_IFACE;
            phy_cache[iter].mdio_type = phy_id & PHY_EXTERNAL ? MDIO_EXT : MDIO_INT;
        }
    }

    phy_cache_initialized  = 1;
}

static inline int port_exist(uint32_t port)
{
    fill_phy_cache();

    if (!phy_cache[port].phy_addr)
    {
        printk("Port %u is not declared\n", port);
        return 0;
    }

    return 1;
}

void phy_reset(uint32_t port_map)
{
    uint32_t phy_map = 0;
    uint32_t port;
    uint32_t phy_addr;

    fill_phy_cache();

    for (port = 0; port < 6; port++)
    {
        if ((phy_addr = phy_cache[port].phy_addr))
            phy_map |= (1 << (phy_addr -1));
    }

    extphy_reset();
    egphy_reset(phy_map);
}
EXPORT_SYMBOL(phy_reset);

void phy_auto_enable(uint32_t port)
{
    uint16_t reg_value;
    uint32_t phy_addr;
    mdio_type_t	mdio_type;
    uint32_t if_flags;

    if (!port_exist(port))
        return;

    phy_addr = phy_cache[port].phy_addr;
    mdio_type = phy_cache[port].mdio_type;
    if_flags = phy_cache[port].if_flags;

    /* Configure AEPCS */
    if (if_flags == MAC_IF_SERDES)
    {
        /* Set block address to zero */
        reg_value = 0x0000;
        if (mdio_write_c22_register(MDIO_INT, phy_addr, MII_PTEST1_RDBRW, reg_value) != MDIO_OK)
            return;

        /* Disable MII autoneg and force speed to 1000M full duplex */
        reg_value = BMCR_SPEED1000 | BMCR_FULLDPLX;
        if (mdio_write_c22_register(MDIO_INT, phy_addr, MII_BMCR, reg_value) != MDIO_OK)
            return;

        reg_value = 0x4101;
        if (mdio_write_c22_register(MDIO_INT, phy_addr, MII_XCTL, reg_value) != MDIO_OK)
            return;

        return;
    }

    /* Reset phy */
    if (mdio_write_c22_register(mdio_type, phy_addr, MII_BMCR, BMCR_RESET) != MDIO_OK)
        return;

    udelay(300);

    /* Write the autoneg bits */
    reg_value = ADVERTISE_CSMA | ADVERTISE_10HALF | ADVERTISE_10FULL
        | ADVERTISE_100HALF | ADVERTISE_100FULL | ADVERTISE_PAUSE_CAP;
    if (mdio_write_c22_register(mdio_type, phy_addr, MII_ADVERTISE, reg_value) != MDIO_OK)
        return;

    /* Favor clock master for better compatibility when in EEE */
    reg_value = ADVERTISE_REPEATER;

    /* If 1000 is supported also advertise 1000 capability */
    if (if_flags == MAC_IF_GMII || if_flags == MAC_IF_SGMII || if_flags == MAC_IF_HSGMII)
        reg_value |= ADVERTISE_1000FULL;

    if (mdio_write_c22_register(mdio_type, phy_addr, MII_CTRL1000, reg_value) != MDIO_OK)
        return;

    /* Initiate autonegotiation*/
    if (mdio_read_c22_register(mdio_type, phy_addr, MII_BMCR, &reg_value) != MDIO_OK)
        return;

    reg_value |= BMCR_ANENABLE | BMCR_ANRESTART;
    if (mdio_write_c22_register(mdio_type, phy_addr, MII_BMCR, reg_value) != MDIO_OK)
        return;
    
    /* Extra configuration according to phy type */
    switch (mdio_type)
    {
    case MDIO_INT:
        egphy_auto_enable_extra_config(if_flags == MAC_IF_HSGMII, phy_addr);
        break;
    case MDIO_EXT:
        extphy_auto_enable_extra_config();
        break;
    default:
        break;
    }
}
EXPORT_SYMBOL(phy_auto_enable);

void phy_shut_down(uint32_t port)
{
    uint32_t phy_addr;
    mdio_type_t	mdio_type;

    if (!port_exist(port))
        return;

    phy_addr = phy_cache[port].phy_addr;
    mdio_type = phy_cache[port].mdio_type;

    mdio_write_c22_register(mdio_type, phy_addr, MII_BMCR, BMCR_PDOWN);
}
EXPORT_SYMBOL(phy_shut_down);

#define CORE_SHD1C_08   0x18
#define MII_AUXSTAT     0x18

phy_rate_t phy_get_line_rate_and_duplex(uint32_t port)
{
    uint32_t phy_addr;
    mdio_type_t	mdio_type;
    uint16_t mii_reg;
    uint32_t ret = PHY_RATE_LINK_DOWN;
    uint32_t link;
    uint32_t speed;
    uint32_t fdx;

    if (!port_exist(port))
        return PHY_RATE_ERR;

    phy_addr = phy_cache[port].phy_addr;
    mdio_type = phy_cache[port].mdio_type;

    if (!phy_is_link_up(mdio_type, phy_addr))
        return PHY_RATE_LINK_DOWN;

    if (phy_addr == 1 || phy_addr == 2) /* EGPHY ports 0,1 */
    {
        mii_reg = egphy_read_register(phy_addr, RDB_REG_ACCESS | CORE_SHD1C_08);
        speed = ((mii_reg >> 3) & 0x3);
        fdx = !((mii_reg >> 7) & 0x1);
        if (speed == 0)
            ret = fdx ? PHY_RATE_1000_FULL : PHY_RATE_1000_HALF;
        else if (speed == 1)
            ret = fdx ? PHY_RATE_100_FULL : PHY_RATE_100_HALF;
        else if (speed == 2)
            ret = fdx ? PHY_RATE_10_FULL : PHY_RATE_10_HALF;
        else
            ret = PHY_RATE_LINK_DOWN;
    }
    else if (phy_addr == 3 || phy_addr == 4) /* EPHY ports 2,3 */
    {
        mii_reg = egphy_read_register(phy_addr, MII_AUXSTAT);
        link = ((mii_reg >> 2) & 0x1);
        speed = ((mii_reg >> 3) & 0x1);
        fdx = mii_reg & 0x1;
        if (link == 0)
            ret = PHY_RATE_LINK_DOWN;
        else if (speed == 1)
            ret = fdx ? PHY_RATE_100_FULL : PHY_RATE_100_HALF;
        else
            ret = fdx ? PHY_RATE_10_FULL : PHY_RATE_10_HALF;
    }
    else if (phy_addr == 5) /* AEPCS */
    {
        return PHY_RATE_1000_FULL;
    }
    else if (phy_addr == 6) /* SGMII */
    {
        /* Get actual link speed from SGMII xgxsStatus1 register */
        mii_reg = sgmii_read_register(phy_addr, 0x8122);
        link = ((mii_reg >> 8) & 0x1);
        speed = mii_reg & 0x3;
        fdx = 1; /* ??? */
        if (speed == 0)
            ret = fdx ? PHY_RATE_10_FULL : PHY_RATE_10_HALF;
        else if (speed == 1)
            ret = fdx ? PHY_RATE_100_FULL : PHY_RATE_100_HALF;
        else if (speed == 2)
            ret = fdx ? PHY_RATE_1000_FULL : PHY_RATE_1000_HALF;
        else if (speed == 3 && fdx)
            ret = PHY_RATE_2500_FULL;
        else
            ret = PHY_RATE_ERR;
    }
    else
    {
        ret = PHY_RATE_ERR;
    }

    return ret;
}
EXPORT_SYMBOL(phy_get_line_rate_and_duplex);

uint32_t phy_link_status(uint32_t port)
{
    phy_rate_t phy_rate = phy_get_line_rate_and_duplex(port);

    if (phy_rate == PHY_RATE_LINK_DOWN || phy_rate == PHY_RATE_ERR)
        return 0;
    else
        return 1;
}
EXPORT_SYMBOL(phy_link_status);

uint16_t phy_read_register(uint32_t phy_addr, uint16_t reg_offset)
{
    if (phy_addr & PHY_EXTERNAL)
        return extphy_read_register(phy_addr & BCM_PHY_ID_M, reg_offset);
    else if (phy_addr == 1 || phy_addr == 2) /* EGPHY ports 0,1 */
        return egphy_read_register(phy_addr & BCM_PHY_ID_M, reg_offset);
    else if (phy_addr == 6) /* SGMII */
        return sgmii_read_register(phy_addr & BCM_PHY_ID_M, reg_offset);
    else
        return egphy_read_register(phy_addr & BCM_PHY_ID_M, reg_offset & 0x1f);
}
EXPORT_SYMBOL(phy_read_register);

int32_t phy_write_register(uint32_t phy_addr, uint16_t reg_offset, uint16_t value)
{
    if (phy_addr & PHY_EXTERNAL)
        return extphy_write_register(phy_addr & BCM_PHY_ID_M, reg_offset, value);
    else
        return egphy_write_register(phy_addr & BCM_PHY_ID_M, reg_offset, value);
}
EXPORT_SYMBOL(phy_write_register);
