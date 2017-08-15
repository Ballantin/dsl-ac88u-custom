/*
* <:copyright-BRCM:2013:DUAL/GPL:standard
* 
*    Copyright (c) 2013 Broadcom 
*    All Rights Reserved
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License, version 2, as published by
* the Free Software Foundation (the "GPL").
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* 
* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
* 
* :>
*/

#include "bcm_OS_Deps.h"
#include <linux/bcm_log.h>
#include <rdpa_api.h>
#include <rdpa_epon.h>
#include <autogen/rdpa_ag_epon.h>
#include <linux/blog_rule.h>
#include "boardparms.h"
#include "board.h"
#include "clk_rst.h"
#include "wan_drv.h"
#ifdef CONFIG_BCM_TIME_SYNC_MODULE
#include "time_sync.h"
#endif

/* init system params */
#define EPON_SPEED_NORMAL "Normal"
#define EPON_SPEED_TURBO  "Turbo"
#define BP_NO_EXT_SW 30
#define RDPA_SWITCHING_MODE_PSP_KEY "SwitchingMode"
#define RDPA_IPCLASS_METHOD_PSP_KEY "IpClassMethod"
#define RDPA_US_DDR_QUEUE_ENABLE "UsDDRQueueEn"

static int emac_map = 0;
static int emacs_num = 0;
static int ext_sw_pid = BP_NO_EXT_SW;
#if !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)
/* scratchpad defaults */
char *wan_default_type = "GPON";
static char *wan_oe_default_emac = "EMAC0";
char *epon_default_speed = EPON_SPEED_NORMAL;

#define base(x) ((x >= '0' && x <= '9') ? '0' : \
    (x >= 'a' && x <= 'f') ? 'a' - 10 : \
    (x >= 'A' && x <= 'F') ? 'A' - 10 : \
    '\255')

#define TOHEX(x) (x - base(x))
#endif

static int rdpa_get_init_system_bp_params(void)
{
    int rc = 0;
    int i, cnt;
    const ETHERNET_MAC_INFO* EnetInfos;
    EnetInfos = BpGetEthernetMacInfoArrayPtr();
    if (EnetInfos == NULL)
        return rc;

    emac_map = EnetInfos[0].sw.port_map & 0xFF;
    for (i = 0; i < BP_MAX_SWITCH_PORTS; ++i)
    {
        if (((1<<i) & emac_map) && ((int)EnetInfos[0].sw.phy_id[i] & EXTSW_CONNECTED))
            ext_sw_pid = i;
    }
    if (EnetInfos[0].ucPhyType != BP_ENET_NO_PHY)
    {
        bitcount(cnt, EnetInfos[0].sw.port_map);
        emacs_num += cnt;
    }

    return rc;
}

#if !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)
static int scratchpad_get_or_init(char *sp_key, char *buf, int max_len,
    char *default_val, int init_only)
{
    int count = 0;

    if (!init_only)
    {
        count = kerSysScratchPadGet(sp_key, buf, max_len - 1);
        if (count > 0)
            buf[count] = '\0';
    }

    /* init_only or read from scratch pad failed */
    if (count <= 0)
    {
        count = kerSysScratchPadSet(sp_key, default_val, strlen(default_val));
        init_only = 1;
        if (count)
        {
            printk("Could not set PSP %s to %s, rc=%d", sp_key, default_val,
                    count);
            return count;
        }

        if (buf)
            strncpy(buf, default_val, max_len);
    }

    printk("scratchpad %s%s - %s \n", init_only ? "init " : "", sp_key, buf ? :
            default_val);

    return 0;
}

static int rdpa_pon_car_mode_cfg(void)
{
    int rc;
    rdpa_system_init_cfg_t init_cfg;
    rdpa_system_cfg_t sys_cfg;
    bdmf_object_handle system_obj;

    rc = rdpa_system_get(&system_obj);
    if (rc)
    {
        printk("%s %s Failed to get RDPA System object rc(%d)\n", __FILE__, __FUNCTION__, rc);
        return rc;
    }

    rc = rdpa_system_cfg_get(system_obj, &sys_cfg);
    if (rc)
    {
        printk("Failed to getting RDPA System cfg\n");
        goto Exit;
    }

    rc = rdpa_system_init_cfg_get(system_obj, &init_cfg);
    if (rc)
    {
        printk("Failed to getting RDPA System init cfg\n");
        goto Exit;
    }

    if (init_cfg.ip_class_method == rdpa_method_fc)
        sys_cfg.car_mode = 1;
    else
        sys_cfg.car_mode = 0;

    rc = rdpa_system_cfg_set(system_obj, &sys_cfg);
    if(rc)
        printk("%s %s Failed to set RDPA System car mode rc(%d)\n", __FILE__, __FUNCTION__, rc);

Exit:
    if (system_obj)
        bdmf_put(system_obj);

    return rc;
}

int wan_scratchpad_get(rdpa_wan_type *wan_type, rdpa_epon_speed_mode *epon_speed, rdpa_emac *wan_emac)
{
    int rc;
    char buf[PSP_BUFLEN_16];
    
    *wan_type = rdpa_wan_none;
    if (wan_emac)
        *wan_emac = rdpa_emac_none;

    rc = scratchpad_get_or_init(RDPA_WAN_TYPE_PSP_KEY, buf, sizeof(buf), wan_default_type, 0);
    if (rc)
        return rc;

    if (!strcmp(buf ,"GBE"))
    {
        *wan_type = rdpa_wan_gbe;
        rc = scratchpad_get_or_init(RDPA_WAN_OEMAC_PSP_KEY, buf, sizeof(buf), wan_oe_default_emac, 0);
        if (rc)
            return rc;

        if (!strncmp(buf ,"EMAC",4) && (strlen(buf) == strlen("EMACX")))
        {
            if (wan_emac)
                *wan_emac = (rdpa_emac)(TOHEX(buf[4]));
        }
        else
        {
            printk("%s %s Wrong EMAC string in ScrachPad - ###(%s)###\n", __FILE__, __FUNCTION__, buf);
            return -1;
        }
    }
    /* saved for backward compatibility */
    else if (!strcmp(buf ,"AE"))
    {
        *wan_type = rdpa_wan_gbe;
        rc = scratchpad_get_or_init(RDPA_WAN_TYPE_PSP_KEY, NULL, 0, "GBE", 1);
        if (rc)
            return rc;
        rc = scratchpad_get_or_init(RDPA_WAN_OEMAC_PSP_KEY, NULL, 0, "EMAC5", 1);
        if (rc)
            return rc;

        if (wan_emac)
            *wan_emac = rdpa_emac5;
    }
    else if (!strcmp(buf ,"GPON"))
        *wan_type = rdpa_wan_gpon;
    else if (!strcmp(buf ,"EPON"))
    {
        *wan_type = rdpa_wan_epon;
        
        if (epon_speed)
        {
            rc = scratchpad_get_or_init(RDPA_EPON_SPEED_PSP_KEY, buf, sizeof(buf), epon_default_speed, 0);
            if (rc)
                return rc;

            if (!strcmp(buf, EPON_SPEED_TURBO))
                *epon_speed = rdpa_epon_speed_2g1g;
            else if (!strcmp(buf, EPON_SPEED_NORMAL))
                *epon_speed = rdpa_epon_speed_1g1g;
        }
    }
    else if (!strcmp(buf ,"AUTO"))
        return 0; /* returns wan_type = rdpa_wan_none */

    /* Scratchpad string not identified */
    if (*wan_type == rdpa_wan_none)
        return -1;

    return 0;
}

int switching_mode_get(rdpa_vlan_switching *switching_mode, rdpa_wan_type wan_type)
{
#ifndef BRCM_CMS_BUILD
    char buf[PSP_BUFLEN_16];
#endif
    char switchDefault[16];

    *switching_mode = rdpa_switching_none;
    strcpy(switchDefault, "NONE");

#if defined(CONFIG_EPON_SFU)
    if (wan_type == rdpa_wan_epon)
    {
        *switching_mode = rdpa_mac_based_switching;
        strcpy(switchDefault, "MAC");
    }
#endif

#ifndef BRCM_CMS_BUILD
    scratchpad_get_or_init(RDPA_SWITCHING_MODE_PSP_KEY, buf, sizeof(buf), switchDefault, 0);

    if (!strcmp(buf ,"NONE"))
    {
        *switching_mode = rdpa_switching_none;
    }
    else if (!strcmp(buf ,"MAC"))
    {
        *switching_mode = rdpa_mac_based_switching;
    }
    else if (!strcmp(buf ,"VLAN"))
    {
        *switching_mode = rdpa_vlan_aware_switching;
    }
#endif

    return 0;
}

int ipclass_method_get(rdpa_ip_class_method *ipclass_method)
{

#ifndef BRCM_CMS_BUILD
    char buf[PSP_BUFLEN_16];
#endif
    char ipClassDefault[16];

#if defined(CONFIG_GPON_SFU) || defined(CONFIG_EPON_SFU)
    *ipclass_method = rdpa_method_none;
    strcpy(ipClassDefault, "NONE");
#else
    *ipclass_method = rdpa_method_fc;
    strcpy(ipClassDefault, "FC");
#endif

#ifndef BRCM_CMS_BUILD
    scratchpad_get_or_init(RDPA_IPCLASS_METHOD_PSP_KEY, buf, sizeof(buf), ipClassDefault, 0);

    if (!strcmp(buf ,"NONE"))
    {
        *ipclass_method = rdpa_method_none;
    }
    else if (!strcmp(buf ,"FC"))
    {
        *ipclass_method = rdpa_method_fc;
    }
    else if (!strcmp(buf ,"MIXED"))
    {
        *ipclass_method = rdpa_method_mixed;
    }
#endif

    return 0;
}

#ifndef BRCM_CMS_BUILD
static int us_ddr_queue_enable_get(bdmf_boolean * us_ddr_queue_enable)
{
    int rc = 0;
    char buf[6];
    char usDDRQueueEnableDefault[6];

    *us_ddr_queue_enable = BDMF_FALSE;

    strcpy(usDDRQueueEnableDefault, "NO");
    rc = scratchpad_get_or_init(RDPA_US_DDR_QUEUE_ENABLE, buf, sizeof(buf), usDDRQueueEnableDefault, 0);   
    if (rc)
        return rc;

    if (!strcmp(buf ,"YES"))
    {
        *us_ddr_queue_enable = BDMF_TRUE;
    }

    return rc;
}
#endif

int rdpa_init_system_fiber(void)
{
    rdpa_wan_type wan_type;
    rdpa_epon_speed_mode epon_speed;
    int rc;
    rdpa_emac wan_emac;
    bdmf_object_handle rdpa_system=NULL;
    rdpa_system_init_cfg_t sys_init_cfg={};

    rc = wan_scratchpad_get(&wan_type, &epon_speed, &wan_emac);
    if (rc)
        return rc;

    switch (wan_type)
    {
    case rdpa_wan_gpon:
        rc = wan_serdes_config(SERDES_WAN_TYPE_GPON);
        break;
    case rdpa_wan_epon:
        rc = wan_serdes_config(epon_speed == rdpa_epon_speed_1g1g ? SERDES_WAN_TYPE_EPON_1G : SERDES_WAN_TYPE_EPON_2G);
        break;
    case rdpa_wan_gbe:
        if (wan_emac != rdpa_emac5)
            return 0;

        rc = wan_serdes_config(SERDES_WAN_TYPE_AE);
        break;
    default:
        printk("%s:%d wan_type not fiber\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (rc)
        return rc;
    
#ifdef CONFIG_BCM_TIME_SYNC_MODULE
    rc = time_sync_init();
    if (rc)
        return rc;
#endif

    if (wan_type == rdpa_wan_gpon || wan_type == rdpa_wan_epon)
    {
        rc = rdpa_pon_car_mode_cfg();
        if (rc)
            printk("%s %s Failed to configure rdpa pon car mode rc(%d)\n", __FILE__, __FUNCTION__, rc);
    }

    /* CMS MCPD is expected to work with group_ip_src_ip only in GPON SFU */
    if (rdpa_system_get(&rdpa_system)) {
       printk("%s:%d failed to get BDMF system object rc=%d\n", __FUNCTION__, __LINE__, rc);
       return -1;
    }
    rdpa_system_init_cfg_get(rdpa_system, &sys_init_cfg);
    bdmf_put(rdpa_system);
    if (wan_type == rdpa_wan_gpon && sys_init_cfg.ip_class_method == rdpa_method_none)
    {
        bdmf_object_handle iptv;

        rc = rdpa_iptv_get(&iptv);
        /* Do nothing if there is no iptv system object. */
        if (rc) {
           printk("%s:%d No IPTV object to configure rc=%d\n", __FUNCTION__, __LINE__, rc);
           return 0;
        } 
        rdpa_iptv_lookup_method_set(iptv, iptv_lookup_method_group_ip_src_ip);
        bdmf_put(iptv);
    }
    
    return rc;
}
EXPORT_SYMBOL(rdpa_init_system_fiber);

int rdpa_init_system(void)
{
    BDMF_MATTR(rdpa_system_attrs, rdpa_system_drv());
    bdmf_object_handle rdpa_system_obj = NULL;
    bdmf_object_handle rdpa_filter_obj = NULL;
    BDMF_MATTR(rdpa_filter_attrs, rdpa_filter_drv());
#if defined(G9991)
    uint32_t fttdp_addr, fttdp_val;
#endif
    int rc;
    rdpa_system_init_cfg_t sys_init_cfg = {};
    rdpa_wan_type wan_type;
    rdpa_system_cfg_t sys_cfg = {};
    bdmf_object_handle cpu =  NULL;

    rc = rdpa_get_init_system_bp_params();
    rc = rc ? : wan_scratchpad_get(&wan_type, NULL, &sys_init_cfg.gbe_wan_emac);
    if (rc)
        goto exit;

    sys_cfg.mtu_size = RDPA_MTU;
    sys_cfg.inner_tpid = 0x8100;
    sys_cfg.outer_tpid = 0x88a8;

    /* GBE EMAC5 cannot be configured in system init, must be configed later via port object! */
    if (sys_init_cfg.gbe_wan_emac == rdpa_emac5)
        sys_init_cfg.gbe_wan_emac = rdpa_emac_none;

    sys_init_cfg.enabled_emac = emac_map;

    sys_init_cfg.runner_ext_sw_cfg.emac_id = (ext_sw_pid == BP_NO_EXT_SW) ? rdpa_emac_none:(rdpa_emac)ext_sw_pid;
    sys_init_cfg.runner_ext_sw_cfg.enabled = (ext_sw_pid == BP_NO_EXT_SW) ? 0 : 1;
#if defined(G9991)
    sys_init_cfg.runner_ext_sw_cfg.enabled = 1;
    sys_init_cfg.runner_ext_sw_cfg.type = rdpa_brcm_fttdp;
#else
    sys_init_cfg.runner_ext_sw_cfg.type = rdpa_brcm_hdr_opcode_0;
#endif

    rc = switching_mode_get(&sys_init_cfg.switching_mode, wan_type);
    if (rc)
        goto exit;

    rc = ipclass_method_get(&sys_init_cfg.ip_class_method);
    if (rc)
        goto exit;
        
#ifndef BRCM_CMS_BUILD
    rc = us_ddr_queue_enable_get(&sys_init_cfg.us_ddr_queue_enable);
    if (rc)
        goto exit;
#endif

    rdpa_system_cfg_set(rdpa_system_attrs, &sys_cfg);
    rdpa_system_init_cfg_set(rdpa_system_attrs, &sys_init_cfg);
    rc = bdmf_new_and_set(rdpa_system_drv(), NULL, rdpa_system_attrs, &rdpa_system_obj);
    if (rc)
    {
        printk("%s %s Failed to create rdpa system object rc(%d)\n", __FILE__, __FUNCTION__, rc);
        goto exit;
    }

    rc = rdpa_cpu_get(rdpa_cpu_host, &cpu);

    /*CPU Interrupts are no longer automatically connected as part of CPU object
     *creation. They need to be explicitly using below rdpa call.*/
    rc = rc ? rc : rdpa_cpu_int_connect_set(cpu, 1 /*true*/);
    if (rc < 0)
    {
        BDMF_TRACE_RET(BDMF_ERR_INTERNAL, "Failed to connect cpu interrupts\n");
        goto exit;
    }

    rc = bdmf_new_and_set(rdpa_filter_drv(), NULL, rdpa_filter_attrs, &rdpa_filter_obj);
    if (rc)
        printk("%s %s Failed to create rdpa filter object rc(%d)\n", __FILE__, __FUNCTION__, rc);

exit:
    if (rc && rdpa_system_obj)
        bdmf_destroy(rdpa_system_obj);

//TODO:Remove the FTTD runner code when ready
#if defined(G9991)
    fttdp_addr = 0xb30d1818;
    fttdp_val = 0x19070019;
    WRITE_32(fttdp_addr, fttdp_val);
    fttdp_addr = 0xb30e1018;
    fttdp_val = 0x00002c2c;
    WRITE_32(fttdp_addr, fttdp_val);
    fttdp_addr = 0xb30e101c;
    fttdp_val = 0x00001013;
    WRITE_32(fttdp_addr, fttdp_val);
    fttdp_addr = 0xb30e1020;
    fttdp_val = 0x00001919;
    WRITE_32(fttdp_addr, fttdp_val);
#endif

    return rc;
}

#else /* CONFIG_BCM963138 || CONFIG_BCM963148 */

int rdpa_init_system(void)
{
    BDMF_MATTR(rdpa_system_attrs, rdpa_system_drv());
    bdmf_object_handle rdpa_system_obj = NULL;
    int rc;
    rdpa_system_init_cfg_t sys_init_cfg = {};
    bdmf_object_handle rdpa_port_object = NULL;
    BDMF_MATTR(rdpa_port_attrs, rdpa_port_drv());
    rdpa_port_dp_cfg_t port_cfg = {};
    bdmf_object_handle cpu =  NULL;

    rc = rdpa_get_init_system_bp_params();
    if (rc)
        goto exit;

    /* wan_type is set to the rdpa_if_wan0 type but it is not used to identify the WAN */
    sys_init_cfg.gbe_wan_emac = rdpa_emac0;
    sys_init_cfg.enabled_emac = emac_map;
    sys_init_cfg.runner_ext_sw_cfg.emac_id = (ext_sw_pid == BP_NO_EXT_SW) ? rdpa_emac_none:(rdpa_emac)ext_sw_pid;
    sys_init_cfg.runner_ext_sw_cfg.enabled = (ext_sw_pid == BP_NO_EXT_SW) ? 0 : 1;
    sys_init_cfg.runner_ext_sw_cfg.type = rdpa_brcm_hdr_opcode_0;
    sys_init_cfg.switching_mode = rdpa_switching_none;
    sys_init_cfg.ip_class_method = rdpa_method_fc;

    rdpa_system_init_cfg_set(rdpa_system_attrs, &sys_init_cfg);

    rc = bdmf_new_and_set(rdpa_system_drv(), NULL, rdpa_system_attrs, &rdpa_system_obj);
    if (rc)
    {
        printk("%s %s Failed to create rdpa system object rc(%d)\n", __FILE__, __FUNCTION__, rc);
        goto exit;
    }

    rc = rdpa_cpu_get(rdpa_cpu_host, &cpu);

    /*CPU Interrupts are no longer automatically connected as part of CPU object
     *creation. They need to be explicitly using below rdpa call.*/
    rc = rc ? rc : rdpa_cpu_int_connect_set(cpu, 1 /*true*/);
    if (rc < 0)
    {
        BDMF_TRACE_RET(BDMF_ERR_INTERNAL, "Failed to connect cpu interrupts\n");
        goto exit;
    }

    if (emac_map & (1<<rdpa_emac0)) /* Only create the Ethernet WAN port if specified in board parameters */
    {
        rdpa_port_index_set(rdpa_port_attrs, rdpa_if_wan0);
        rdpa_port_wan_type_set(rdpa_port_attrs, rdpa_wan_gbe);
        port_cfg.emac = rdpa_emac0;
        rdpa_port_cfg_set(rdpa_port_attrs, &port_cfg);
        rc = bdmf_new_and_set(rdpa_port_drv(), NULL, rdpa_port_attrs, &rdpa_port_object);
        if (rc)
        {
            printk("%s %s Failed to create rdpa wan port rc(%d)\n", __FILE__, __FUNCTION__, rc);
            goto exit;
        }
    }

exit:
    if (rc && rdpa_system_obj)
        bdmf_destroy(rdpa_system_obj);
    return rc;
}

#endif

