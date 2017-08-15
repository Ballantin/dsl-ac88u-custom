// <:copyright-BRCM:2013:DUAL/GPL:standard
// 
//    Copyright (c) 2013 Broadcom 
//    All Rights Reserved
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2, as published by
// the Free Software Foundation (the "GPL").
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// 
// A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
// writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
// 
// :>
/*
 * dhd_helper object header file.
 * This header file is generated automatically. Do not edit!
 */
#ifndef _RDPA_AG_DHD_HELPER_H_
#define _RDPA_AG_DHD_HELPER_H_

/** \addtogroup dhd_helper
 * @{
 */


/** Get dhd_helper type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a dhd_helper object.
 * \return dhd_helper type handle
 */
bdmf_type_handle rdpa_dhd_helper_drv(void);

/* dhd_helper: Attribute types */
typedef enum {
    rdpa_dhd_helper_attr_radio_idx = 0, /* radio_idx : KRI : number/4 : Radio Index */
    rdpa_dhd_helper_attr_init_cfg = 1, /* init_cfg : MRI : aggregate/52 dhd_init_config(rdpa_dhd_init_cfg_t) : Initial DHD Configuration */
    rdpa_dhd_helper_attr_flush = 2, /* flush : W : number/4 : Flush FlowRing */
    rdpa_dhd_helper_attr_flow_ring_enable = 3, /* flow_ring_enable : RWF : bool/1[542] : Enable/Disable FlowRing */
    rdpa_dhd_helper_attr_rx_post_init = 4, /* rx_post_init : W : bool/1 : RX Post init: allocate and push RX Post descriptors to Dongle */
    rdpa_dhd_helper_attr_ssid_tx_dropped_packets = 5, /* ssid_tx_dropped_packets : RF : number/4[16] : SSID Dropped Packets */
    rdpa_dhd_helper_attr_int_connect = 6, /* int_connect : W : bool/1 : Connect interrupts */
} rdpa_dhd_helper_attr_types;

extern int (*f_rdpa_dhd_helper_get)(bdmf_number radio_idx_, bdmf_object_handle *pmo);

/** Get dhd_helper object by key.

 * This function returns dhd_helper object instance by key.
 * \param[in] radio_idx_    Object key
 * \param[out] dhd_helper_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_dhd_helper_get(bdmf_number radio_idx_, bdmf_object_handle *dhd_helper_obj);

/** Get dhd_helper/radio_idx attribute.
 *
 * Get Radio Index.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[out]  radio_idx_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_radio_idx_get(bdmf_object_handle mo_, bdmf_number *radio_idx_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_dhd_helper_attr_radio_idx, &_nn_);
    *radio_idx_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set dhd_helper/radio_idx attribute.
 *
 * Set Radio Index.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[in]   radio_idx_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_radio_idx_set(bdmf_object_handle mo_, bdmf_number radio_idx_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_dhd_helper_attr_radio_idx, radio_idx_);
}


/** Get dhd_helper/init_cfg attribute.
 *
 * Get Initial DHD Configuration.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[out]  init_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_init_cfg_get(bdmf_object_handle mo_, rdpa_dhd_init_cfg_t * init_cfg_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_dhd_helper_attr_init_cfg, init_cfg_, sizeof(*init_cfg_));
}


/** Set dhd_helper/init_cfg attribute.
 *
 * Set Initial DHD Configuration.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[in]   init_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_init_cfg_set(bdmf_object_handle mo_, const rdpa_dhd_init_cfg_t * init_cfg_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_dhd_helper_attr_init_cfg, init_cfg_, sizeof(*init_cfg_));
}


/** Set dhd_helper/flush attribute.
 *
 * Set Flush FlowRing.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[in]   flush_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_flush_set(bdmf_object_handle mo_, bdmf_number flush_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_dhd_helper_attr_flush, flush_);
}


/** Get dhd_helper/flow_ring_enable attribute entry.
 *
 * Get Enable/Disable FlowRing.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  flow_ring_enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_flow_ring_enable_get(bdmf_object_handle mo_, bdmf_index ai_, bdmf_boolean *flow_ring_enable_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attrelem_get_as_num(mo_, rdpa_dhd_helper_attr_flow_ring_enable, (bdmf_index)ai_, &_nn_);
    *flow_ring_enable_ = (bdmf_boolean)_nn_;
    return _rc_;
}


/** Set dhd_helper/flow_ring_enable attribute entry.
 *
 * Set Enable/Disable FlowRing.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[in]   flow_ring_enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_flow_ring_enable_set(bdmf_object_handle mo_, bdmf_index ai_, bdmf_boolean flow_ring_enable_)
{
    return bdmf_attrelem_set_as_num(mo_, rdpa_dhd_helper_attr_flow_ring_enable, (bdmf_index)ai_, flow_ring_enable_);
}


/** Invoke dhd_helper/rx_post_init attribute.
 *
 * Invoke RX Post init: allocate and push RX Post descriptors to Dongle.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_dhd_helper_rx_post_init(bdmf_object_handle mo_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_dhd_helper_attr_rx_post_init, 1);
}


/** Get dhd_helper/ssid_tx_dropped_packets attribute entry.
 *
 * Get SSID Dropped Packets.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  ssid_tx_dropped_packets_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_ssid_tx_dropped_packets_get(bdmf_object_handle mo_, bdmf_index ai_, bdmf_number *ssid_tx_dropped_packets_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attrelem_get_as_num(mo_, rdpa_dhd_helper_attr_ssid_tx_dropped_packets, (bdmf_index)ai_, &_nn_);
    *ssid_tx_dropped_packets_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set dhd_helper/int_connect attribute.
 *
 * Set Connect interrupts.
 * \param[in]   mo_ dhd_helper object handle or mattr transaction handle
 * \param[in]   int_connect_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_dhd_helper_int_connect_set(bdmf_object_handle mo_, bdmf_boolean int_connect_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_dhd_helper_attr_int_connect, int_connect_);
}

/** @} end of dhd_helper Doxygen group */




#endif /* _RDPA_AG_DHD_HELPER_H_ */
