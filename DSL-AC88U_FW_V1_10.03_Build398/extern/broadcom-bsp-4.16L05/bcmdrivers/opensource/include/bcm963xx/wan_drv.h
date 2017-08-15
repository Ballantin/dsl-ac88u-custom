/*
    Copyright 2000-2010 Broadcom Corporation

    <:label-BRCM:2011:DUAL/GPL:standard
    
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

#ifndef _WAN_DRV_H_
#define _WAN_DRV_H_

#include <bdmf_data_types.h>

typedef enum
{
    SERDES_WAN_TYPE_NONE,
    SERDES_WAN_TYPE_GPON,
    SERDES_WAN_TYPE_EPON_1G,
    SERDES_WAN_TYPE_EPON_2G,
    SERDES_WAN_TYPE_AE,
} serdes_wan_type_t;

typedef enum
{
    DRV_SERDES_NO_ERROR, 
    DRV_SERDES_AMPLITUDE_IS_OUT_OF_RANGE,
}
serdes_error_t;

int wan_serdes_config(serdes_wan_type_t wan_type);
serdes_wan_type_t wan_serdes_type_get(void);
void wan_prbs_gen(uint32_t enable, int enable_host_tracking, int mode, serdes_wan_type_t wan_type, bdmf_boolean *valid);
void wan_prbs_status(bdmf_boolean *valid, uint32_t *errors);

typedef void (*PMD_DEV_ENABLE_PRBS)(uint16_t enable, uint8_t prbs_mode);
void wan_register_pmd_prbs_callback(PMD_DEV_ENABLE_PRBS callback);

#ifdef CONFIG_BCM96848
void onu2g_read(uint32_t addr, uint16_t *data);
void onu2g_write(uint32_t addr, uint16_t data);
#endif

#ifdef CONFIG_BCM96838
serdes_error_t wan_serdes_amplitude_set(uint16_t val);
serdes_error_t wan_serdes_amplitude_get(uint16_t *val);
#endif

#endif
