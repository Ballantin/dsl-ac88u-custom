
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
 * File Name  : rdpa_cmd_filter.c
 *
 * Description: This file contains the filter configuration API.
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
#include "rdpa_ag_filter.h"
#include "rdpa_drv.h"

#define __BDMF_LOG__

#define CMD_FILTER_LOG_ID_RDPA_CMD_DRV BCM_LOG_ID_RDPA_CMD_DRV

#if defined(__BDMF_LOG__)
#define CMD_FILTER_LOG_ERROR(fmt, args...) 										\
    do {                                                            				\
        if (bdmf_global_trace_level >= bdmf_trace_level_error)      				\
            bdmf_trace("ERR: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);	\
    } while(0)
#define CMD_FILTER_LOG_INFO(fmt, args...) 										\
    do {                                                            				\
        if (bdmf_global_trace_level >= bdmf_trace_level_info)      					\
            bdmf_trace("INF: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);	\
    } while(0)
#define CMD_FILTER_LOG_DEBUG(fmt, args...) 										\
    do {                                                            				\
        if (bdmf_global_trace_level >= bdmf_trace_level_debug)      					\
            bdmf_trace("DBG: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);	\
    } while(0)
#else
#define CMD_FILTER_LOG_ERROR(fmt, arg...) BCM_LOG_ERROR(fmt, arg...)
#define CMD_FILTER_LOG_INFO(fmt, arg...) BCM_LOG_INFO(fmt, arg...)
#define CMD_FILTER_LOG_DEBUG(fmt, arg...) BCM_LOG_DEBUG(fmt, arg...)
#endif


/*******************************************************************************/
/* global routines                                                             */
/*******************************************************************************/

/*******************************************************************************
 *
 * Function: rdpa_cmd_filter_ioctl
 *
 * IOCTL interface to the RDPA FILTER API.
 *
 *******************************************************************************/
int rdpa_cmd_filter_ioctl(unsigned long arg)
{
    rdpa_drv_ioctl_filter_t *userFilter_p = (rdpa_drv_ioctl_filter_t *)arg;
    rdpa_drv_ioctl_filter_t filter;
    rdpa_filter_tpid_vals_t tpid_curr_vals;
    bdmf_object_handle filter_obj = NULL;
    int rc = BDMF_ERR_OK;

    copy_from_user(&filter, userFilter_p, sizeof(rdpa_drv_ioctl_filter_t));
    CMD_FILTER_LOG_DEBUG("RDPA FILTER CMD(%d)", filter.cmd);

    bdmf_lock();
    rc = rdpa_filter_get( &filter_obj );

    if (rc)
    {
        bdmf_unlock();
        return rc;
    }

    switch(filter.cmd)
    {
        case RDPA_IOCTL_FILTER_CMD_ADD_ENTRY: 
        {
            rc = rdpa_filter_entry_set(filter_obj, &filter.param.key, &filter.param.ctrl);
            break;
        }

        case RDPA_IOCTL_FILTER_CMD_GLOBAL_CFG: 
        {
            rc = rdpa_filter_global_cfg_set(filter_obj, &filter.param.global_cfg);
            break;
        }

        case RDPA_IOCTL_FILTER_CMD_ETYPE_UDEF_CFG: 
        {            
            rc = rdpa_filter_etype_udef_set(filter_obj, filter.param.udef_inx, filter.param.udef_val);
            break;
        }

        case RDPA_IOCTL_FILTER_CMD_TPID_VALS_CFG: 
        {
            /* Change only the related value, read modify write */
            rc = rdpa_filter_tpid_vals_get(filter_obj, &tpid_curr_vals);
            if (rc)
            {
                /* If was not configured before configure with the default dont care value */
                tpid_curr_vals.val_ds = 0xffff;
                tpid_curr_vals.val_us = 0xffff;
            }

            /* US direction = 1 */
            if (filter.param.tpid_direction)
                filter.param.tpid_vals.val_ds = tpid_curr_vals.val_ds;
            else
                filter.param.tpid_vals.val_us = tpid_curr_vals.val_us;

            rc = rdpa_filter_tpid_vals_set(filter_obj, &filter.param.tpid_vals);
            break;
        }

        case RDPA_IOCTL_FILTER_CMD_OUI_CFG: 
        {
            rc = rdpa_filter_oui_val_set(filter_obj, &filter.param.oui_val_key, filter.param.oui_val);
            break;
        }

        case RDPA_IOCTL_FILTER_CMD_GET_STAT: 
        {
            rc = rdpa_filter_stats_get(filter_obj, &filter.param.stats_params, &filter.param.stats_val);
            if (!rc)
            {
                copy_to_user((rdpa_drv_ioctl_filter_t *)arg, &filter, sizeof(rdpa_drv_ioctl_filter_t));
            }
            break;
        }

        default:
            CMD_FILTER_LOG_ERROR("Invalid IOCTL cmd %d", filter.cmd);
            rc = RDPA_DRV_ERROR;
    }

    bdmf_put(filter_obj);
    if (rc) {
        CMD_FILTER_LOG_ERROR("rdpa_cmd_filter_ioctl() OUT: FAILED: rc(%d)", rc);
    }
    bdmf_unlock();

    return rc;
}
