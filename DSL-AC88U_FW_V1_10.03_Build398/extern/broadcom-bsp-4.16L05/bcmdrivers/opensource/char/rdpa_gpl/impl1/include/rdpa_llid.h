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


#ifndef _RDPA_LLID_H_
#define _RDPA_LLID_H_

/**
 * \defgroup llid LLID Management
 * \ingroup eponmanagement
 * Objects and functions in this group are used for LLID configuration
 * - Associating L1 channels with LLID group
 * - LLID-level operations
 * @{
 */

/** Max number of LLIDs */
#define RDPA_EPON_MAX_LLID      8

/** Max number of L1 data queues per LLID, excluding the management queue */
#define RDPA_EPON_LLID_QUEUES   8

/* llid object private data */
typedef struct {
    bdmf_index index;                           /* LLID index */
    bdmf_boolean control_enable;                /* enable control channel */
    bdmf_boolean data_enable;                   /* enable control channel */
    int16_t num_channels;                       /* number of data channels */
    int16_t channels[RDPA_EPON_LLID_QUEUES+1];  /* channels assigned to LLID: [0]-control, [1-N]-data */
    bdmf_object_handle data_egress_tm;          /* egress_tm object handle */
    bdmf_object_handle control_egress_tm;       /* egress_tm object handle */
    bdmf_index ds_def_flow;                     /* llid downstream default flow configuration index*/
} llid_drv_priv_t;

/** llid def flow per port action */
typedef struct {
    bdmf_object_handle vlan_action; /**< VLAN action object */
    bdmf_boolean drop; /**< Drop action - true/false */
} rdpa_llid_port_action_t;
/** @} end of llid Doxygen group */

typedef struct
    {
    uint8_t L1ChannelData;
    uint8_t L1ChannelControl;
    } EponL2L1Mapping;

typedef struct
    {
    EponL2L1Mapping eponL2L1Mapping[RDPA_EPON_LLID_QUEUES];
    } EponL2L1Allocation;

#endif /* _RDPA_LLID_H_ */
