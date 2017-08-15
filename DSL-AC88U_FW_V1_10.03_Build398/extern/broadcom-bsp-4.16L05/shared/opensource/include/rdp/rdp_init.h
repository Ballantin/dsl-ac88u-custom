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
#ifndef _RDP_INIT_H_
#define _RDP_INIT_H_

typedef struct
{
    uint32_t runner_tm_base_addr;           /* bpm pool base address for unicast packets */
    uint32_t runner_mc_base_addr;           /* bpm pool base address for multicast packets */
    uint32_t bpm_buffer_size;               /* bpm buffer size */
    uint32_t bpm_buffers_number;            /* amount of bpm buffers */
} rdp_init_params;

void rdp_get_init_params(rdp_init_params *init_params);

#endif