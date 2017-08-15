/*
* <:copyright-BRCM:2015:DUAL/GPL:standard
* 
*    Copyright (c) 2015 Broadcom 
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

#if !defined(DSL_63138) && !defined(DSL_63148)
#include "bcm_OS_Deps.h"
#include "bcmenet_common.h"
#include "rdpa_mw_qos.h"


static BOOL pkt_based_qos_en[2][RDPA_MW_QOS_TYPE_MAX];

int rdpa_mw_pkt_based_qos_get(rdpa_traffic_dir dir, 
                            rdpa_mw_qos_type type, 
                            BOOL *enable)
{
    if (dir > rdpa_dir_us || type >= RDPA_MW_QOS_TYPE_MAX)
    {
        return -1;
    }
    else
    {
        *enable = pkt_based_qos_en[dir][type];
        return 0;
    }
}

EXPORT_SYMBOL(rdpa_mw_pkt_based_qos_get);

int rdpa_mw_pkt_based_qos_set(rdpa_traffic_dir dir, 
                            rdpa_mw_qos_type type, 
                            BOOL *enable)
{
    if (dir > rdpa_dir_us || type >= RDPA_MW_QOS_TYPE_MAX)
    {
        return -1;
    }
    else
    {
        pkt_based_qos_en[dir][type] = *enable;
        return 0;
    }
}

EXPORT_SYMBOL(rdpa_mw_pkt_based_qos_set);


#endif
