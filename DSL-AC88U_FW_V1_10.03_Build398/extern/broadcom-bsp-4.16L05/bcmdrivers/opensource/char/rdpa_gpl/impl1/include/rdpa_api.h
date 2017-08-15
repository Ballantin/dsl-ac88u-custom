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


#ifndef _RDPA_API_H_
#define _RDPA_API_H_

/*
 * Forward declarations of commonly-used types
 */

#include <bdmf_interface.h>
#include <rdpa_types.h>
#include <rdpa_config.h>
#include <rdpa_system.h>

#include <rdpa_bridge.h>
#include <rdpa_vlan.h>
#include <rdpa_qos_mapper.h>
#include <rdpa_tm.h>
#include <rdpa_vlan_action.h>
#ifndef CM3390
#include <rdpa_spdsvc.h>
#endif
#include <rdpa_wlan_mcast.h>
#if defined(DSL_63138) || defined(DSL_63148)
#include <rdpa_ipsec.h>
#include <rdpa_ucast.h>
#include <rdpa_l2_ucast.h>
#include <rdpa_mcast.h>
#include <rdpa_xtm.h>
#else
#include <rdpa_ip_class.h>
#include <rdpa_iptv.h>
#endif /* DSL_138 */

#include <rdpa_llid.h>
#include <rdpa_port.h>
#include <rdpa_cpu_basic.h>
#include <rdpa_cpu.h>
#include <rdpa_filter.h>
#include <rdpa_common.h>
#include <rdpa_ingress_class.h>
#include <rdpa_gem.h>
#include <rdpa_tcont.h>
#ifdef CONFIG_DHD_RUNNER
#include <rdpa_dhd_helper.h>
#endif

#include <autogen/rdpa_ag_bridge.h>
#include <autogen/rdpa_ag_cpu.h>
#include <autogen/rdpa_ag_llid.h>
#ifndef CM3390
#include <autogen/rdpa_ag_spdsvc.h>
#endif
#include <autogen/rdpa_ag_wlan_mcast.h>
#if defined(DSL_63138) || defined(DSL_63148)
#include <autogen/rdpa_ag_ipsec.h>
#include <autogen/rdpa_ag_ucast.h>
#include <autogen/rdpa_ag_l2_ucast.h>
#include <autogen/rdpa_ag_mcast.h>
#include <autogen/rdpa_ag_xtm.h>
#else
#include <autogen/rdpa_ag_ip_class.h>
#include <autogen/rdpa_ag_iptv.h>
#endif /* DSL_138 */
#include <autogen/rdpa_ag_policer.h>
#include <autogen/rdpa_ag_port.h>
#include <autogen/rdpa_ag_dscp_to_pbit.h>
#include <autogen/rdpa_ag_pbit_to_queue.h>
#include <autogen/rdpa_ag_tc_to_queue.h>
#include <autogen/rdpa_ag_egress_tm.h>
#include <autogen/rdpa_ag_system.h>
#include <autogen/rdpa_ag_vlan.h>
#include <autogen/rdpa_ag_vlan_action.h>
#include <autogen/rdpa_ag_filter.h>
#include <autogen/rdpa_ag_ingress_class.h>
#include <autogen/rdpa_ag_pbit_to_gem.h>
#include <autogen/rdpa_ag_gem.h>
#include <autogen/rdpa_ag_tcont.h>
#ifdef CONFIG_DHD_RUNNER
#include <autogen/rdpa_ag_dhd_helper.h>
#endif

#endif /* _RDPA_API_H_ */
