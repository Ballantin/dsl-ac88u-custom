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
/* This file contains the definitions for Broadcom's 6838 Data path		  	  */
/* initialization sequence											          */
/*                                                                            */
/******************************************************************************/

#ifndef __OREN_DATA_PATH_INIT_H
#define __OREN_DATA_PATH_INIT_H

#include <rdp_drv_bbh.h>

typedef enum
{
	DPI_RC_OK,
	DPI_RC_ERROR
} E_DPI_RC;

typedef struct
{
	uint32_t wan_bbh;
	uint32_t mtu_size;
	uint32_t headroom_size;
	uint32_t car_mode;
	uint32_t ip_class_method;
	uint32_t bridge_fc_mode;
	uint32_t runner_freq;
	uint32_t enabled_port_map;
	uint32_t g9991_debug_port;
    uint32_t us_ddr_queue_enable;
    uint32_t runner_tm_base_addr;
    uint32_t runner_mc_base_addr;
    uint32_t bpm_buffer_size;
    uint32_t bpm_buffers_number;
    uint32_t wan_phy_port_type;
} S_DPI_CFG;

uint32_t data_path_init(S_DPI_CFG *pCfg);
uint32_t data_path_init_fiber(DRV_BBH_PORT_INDEX wan_emac);
uint32_t data_path_go(void);
void bridge_port_sa_da_cfg(void);

#endif
