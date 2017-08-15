
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

/*
 *******************************************************************************
 * File Name  : rdpa_cmd_sys.c
 *
 * Description: This file contains the FAP Traffic Manager configuration API.
 *
 *******************************************************************************
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/bcm_log.h>
#include "bcmenet.h"
#include "bcmtypes.h"
#include "bcmnet.h"
#include "rdpa_types.h"
#include "rdpa_api.h"
#include "rdpa_epon.h"
#include "rdpa_ag_epon.h"
#include "rdpa_ag_port.h"
#include "rdpa_drv.h"
#include "rdpa_cmd_sys.h"

#define __BDMF_LOG__

#define CMD_SYS_LOG_ID_RDPA_CMD_DRV BCM_LOG_ID_RDPA_CMD_DRV

#if defined(__BDMF_LOG__)
#define CMD_SYS_LOG_ERROR(fmt, args...) 										\
    do {                                                            				\
        if (bdmf_global_trace_level >= bdmf_trace_level_error)      				\
            bdmf_trace("ERR: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);	\
    } while(0)
#define CMD_SYS_LOG_INFO(fmt, args...) 										\
    do {                                                            				\
        if (bdmf_global_trace_level >= bdmf_trace_level_info)      					\
            bdmf_trace("INF: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);	\
    } while(0)
#define CMD_SYS_LOG_DEBUG(fmt, args...) 										\
    do {                                                            				\
        if (bdmf_global_trace_level >= bdmf_trace_level_debug)      					\
            bdmf_trace("DBG: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);	\
    } while(0)
#else
#define CMD_SYS_LOG_ERROR(fmt, arg...) BCM_LOG_ERROR(fmt, arg...)
#define CMD_SYS_LOG_INFO(fmt, arg...) BCM_LOG_INFO(fmt, arg...)
#define CMD_SYS_LOG_DEBUG(fmt, arg...) BCM_LOG_DEBUG(fmt, arg...)
#endif


/*******************************************************************************/
/* global routines                                                             */
/*******************************************************************************/

static bdmf_object_handle system_obj = NULL;

/*******************************************************************************
 *
 * Function: rdpa_cmd_sys_ioctl
 *
 * IOCTL interface to the RDPA INGRESS CLASSIFIER API.
 *
 *******************************************************************************/
int rdpa_cmd_sys_ioctl(unsigned long arg)
{
    rdpa_drv_ioctl_sys_t *userSys_p = (rdpa_drv_ioctl_sys_t *)arg;
    rdpa_drv_ioctl_sys_t sys;
    int rc = BDMF_ERR_OK;

    copy_from_user(&sys, userSys_p, sizeof(rdpa_drv_ioctl_sys_t));

    CMD_SYS_LOG_DEBUG("RDPA SYS CMD(%d)", sys.cmd);

    bdmf_lock();
	
    switch(sys.cmd)
    {
        case RDPA_IOCTL_SYS_CMD_WANTYPE_GET: 
        {
            sys.param.wan_type = rdpa_wan_type_get();
            copy_to_user((rdpa_drv_ioctl_sys_t *)arg, &sys, sizeof(rdpa_drv_ioctl_sys_t));
            break;
        }
		
        case RDPA_IOCTL_SYS_CMD_IN_TPID_GET: 
        {
            rdpa_tpid_detect_cfg_t entry = {.val_udef=0, .otag_en=0, .itag_en=1};
            rdpa_tpid_detect_t tpid = rdpa_tpid_detect_udef_2;
            rdpa_system_get(&system_obj);
            rdpa_system_tpid_detect_get(system_obj, tpid, &entry);
            bdmf_put(system_obj);
            sys.param.inner_tpid = entry.val_udef;
            copy_to_user((rdpa_drv_ioctl_sys_t *)arg, &sys, sizeof(rdpa_drv_ioctl_sys_t));
            break;
        }
		
    	case RDPA_IOCTL_SYS_CMD_IN_TPID_SET: 
        {
            rdpa_tpid_detect_cfg_t entry = {.val_udef=0, .otag_en=0, .itag_en=1};
            rdpa_tpid_detect_t tpid = rdpa_tpid_detect_udef_2;
            rdpa_system_get(&system_obj);
            entry.val_udef = sys.param.inner_tpid;
            rc = rdpa_system_tpid_detect_set(system_obj, tpid, &entry);
            bdmf_put(system_obj);
            break;
        }
		
        case RDPA_IOCTL_SYS_CMD_OUT_TPID_GET: 
        {
            rdpa_tpid_detect_cfg_t entry = {.val_udef=0, .otag_en=1, .itag_en=0};
            rdpa_tpid_detect_t tpid = rdpa_tpid_detect_udef_1;
            rdpa_system_get(&system_obj);
            rdpa_system_tpid_detect_get(system_obj, tpid, &entry);
            bdmf_put(system_obj);
            sys.param.outer_tpid= entry.val_udef;
            copy_to_user((rdpa_drv_ioctl_sys_t *)arg, &sys, sizeof(rdpa_drv_ioctl_sys_t));
            break;
        }
		
        case RDPA_IOCTL_SYS_CMD_OUT_TPID_SET: 
        {
            rdpa_tpid_detect_cfg_t entry = {.val_udef=0, .otag_en=1, .itag_en=0};
            rdpa_tpid_detect_t tpid = rdpa_tpid_detect_udef_1;
            rdpa_system_get(&system_obj);
            entry.val_udef = sys.param.outer_tpid;
            rc = rdpa_system_tpid_detect_set(system_obj, tpid, &entry);
            bdmf_put(system_obj);
            break;
        }

        case RDPA_IOCTL_SYS_CMD_EPON_MODE_SET:
        {
            /* temporary. this will move to the rdpa_cmd_epon 
            */
            rdpa_epon_mode epon_mode = sys.param.epon_mode;
            if (!rdpa_epon_get(&system_obj))
            {
                rc = rdpa_epon_mode_set(system_obj, epon_mode);
                bdmf_put(system_obj);
            }
            break;
        }

        case RDPA_IOCTL_SYS_CMD_ALWAYS_TPID_SET:
        {
            rdpa_system_cfg_t system_cfg;
            rdpa_system_get(&system_obj);
            rc = rdpa_system_cfg_get(system_obj, &system_cfg);
            system_cfg.add_always_tpid = sys.param.always_tpid;
            rc = rdpa_system_cfg_set(system_obj, &system_cfg);
            bdmf_put(system_obj);
            break;
        }

        case RDPA_IOCTL_SYS_CMD_FORCE_DSCP_GET:
        {
            rdpa_system_cfg_t system_cfg;
            rdpa_system_get(&system_obj);
            rdpa_system_cfg_get(system_obj, &system_cfg);
            bdmf_put(system_obj);

            if (sys.param.force_dscp.dir == rdpa_dir_ds)
                sys.param.force_dscp.enable = system_cfg.force_dscp_to_pbit_ds;
            else
                sys.param.force_dscp.enable = system_cfg.force_dscp_to_pbit_us;
            
            copy_to_user((rdpa_drv_ioctl_sys_t *)arg, 
                &sys, sizeof(rdpa_drv_ioctl_sys_t));
            
            break;
        }

        case RDPA_IOCTL_SYS_CMD_FORCE_DSCP_SET:
        {
            rdpa_system_cfg_t system_cfg;
            rdpa_system_get(&system_obj);
            rc = rdpa_system_cfg_get(system_obj, &system_cfg);

            if (sys.param.force_dscp.dir == rdpa_dir_ds)
                system_cfg.force_dscp_to_pbit_ds = sys.param.force_dscp.enable;
            else
                system_cfg.force_dscp_to_pbit_us = sys.param.force_dscp.enable;
            
            rc = rdpa_system_cfg_set(system_obj, &system_cfg);
            bdmf_put(system_obj);
            break;
        }

        default:
            CMD_SYS_LOG_ERROR("Invalid IOCTL cmd %d", sys.cmd);
            rc = RDPA_DRV_ERROR;
    }

    if (rc) {
    CMD_SYS_LOG_ERROR("rdpa_cmd_sys_ioctl() OUT: FAILED: rc(%d)", rc);
    }
    
    bdmf_unlock();
    return rc;
}

/*******************************************************************************
 *
 * Function: rdpa_cmd_SYS_init
 *
 * Initializes the RDPA IC API.
 *
 *******************************************************************************/
void rdpa_cmd_sys_init(void)
{
    CMD_SYS_LOG_DEBUG("RDPA SYS INIT");
}
