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
#ifndef _RDPA_CPU_HELPER_H
#define _RDPA_CPU_HELPER_H

#ifndef CM3390
#include "rdp_cpu_ring_defs.h"
#else
#include "packing.h"
#include "rdp_drv_bpm.h"
#include "rdd_data_structures_auto.h"
#include "rdd_runner_proj_defs.h"
#include "bdmf_system_common.h"
#include "rdpa_cpu.h"
#endif

#define SIZE_OF_RING_DESCRIPTOR sizeof(CPU_RX_DESCRIPTOR)

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
#define RUNNER_SOURCE_PORT_PCI  19
#else
#define RUNNER_SOURCE_PORT_PCI  8
#endif

extern rdpa_if map_rdd_to_rdpa_if[];

static inline rdpa_if rdpa_cpu_rx_srcport_to_rdpa_if(uint16_t rdd_srcport, int flow_id)
{
#ifndef BRCM_FTTDP
    /* Special case for wifi packets: if src_port is PCI then need to set
     * SSID */
    return (rdd_srcport == RUNNER_SOURCE_PORT_PCI) ? rdpa_if_ssid0 +
        flow_id : map_rdd_to_rdpa_if[rdd_srcport];
#else
    switch (rdd_srcport)
    {
    case 0:
        return rdpa_if_wan0;
    /* .. upto number-of-lan-ifs + 1 */
    case 1 ... rdpa_if_lan_max - rdpa_if_lan0 + 1 + 1:
        return rdpa_if_lan0 + rdd_srcport - 1;
    default:
        return rdpa_if_none;
    }
#endif
}

#ifdef CM3390

static inline rdpa_if switch_port_to_rdpa_if(u8 port)
{
    if (port == 7)
        return rdpa_if_lan5;
    else
        return rdpa_if_lan0 + port;
}
static inline u8 rdpa_if_to_switch_port(rdpa_if rif)
{
    if (rif == rdpa_if_lan5)
        return 7;
    else
        return rif - rdpa_if_lan0;
}

#pragma pack(push, 1)
typedef struct
{
    uint32_t offset:9         __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t rsv1:7           __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t priority:3       __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t src_subid:5      __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t src_port:5       __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t rsv2:2           __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t pd_size:1        __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t packet_size:12   __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t token_idx:16     __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t ddr:2            __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t rsv0:1           __PACKING_ATTRIBUTE_FIELD_LEVEL__;
    uint32_t valid:1          __PACKING_ATTRIBUTE_FIELD_LEVEL__;
} DOCSIS_PD;
#pragma pack(pop)

typedef void (*cpu_tx_dir_queue)(const rdpa_cpu_tx_info_t *info, void *raw_desc);

static inline int cpu_get_docsis_hdr_len(void)
{
    return 8;
}

static inline void cpu_tx_encode_docsis_hdr(void *docsis_hdr, uint32_t offset, uint32_t token, uint32_t wan_type, rdpa_if srcport)
{
    DOCSIS_PD *docsispd = (DOCSIS_PD *)docsis_hdr;
    uint32_t *swapptr = (uint32_t *)docsis_hdr;
    rdp_fpm_index fpm_idx = *(rdp_fpm_index *)&token;

    memset(docsispd, 0, sizeof(DOCSIS_PD));

    /* fill the docsis header */
    docsispd->pd_size = 0; /* always 8 */
    switch (srcport)
    {
        case rdpa_if_lan0:
            docsispd->src_port = RDD_LAN0_VPORT;
            break;
        case rdpa_if_lan1:
            docsispd->src_port = RDD_LAN1_VPORT;
            break;
        case rdpa_if_lan2:
            docsispd->src_port = RDD_LAN2_VPORT;
            break;
        case rdpa_if_lan3:
            docsispd->src_port = RDD_LAN3_VPORT;
            break;
        case rdpa_if_lan4:
            docsispd->src_port = RDD_LAN4_VPORT;
            break;
        case rdpa_if_lan5:
            docsispd->src_port = RDD_LAN5_VPORT;
            break;

        case rdpa_if_ssid0 ... rdpa_if_ssid7:
            docsispd->src_port = RDD_WIFI_VPORT;
            docsispd->src_subid = srcport - rdpa_if_ssid0;
            break;

        case rdpa_if_ssid8 ... rdpa_if_ssid15:
            docsispd->src_port = RDD_WIFI_VPORT;
            docsispd->src_subid = srcport - rdpa_if_ssid0;
            break;

        default:
        case rdpa_if_cpu:
            docsispd->src_port = RDD_CPU_VPORT;
            break;
    }
    docsispd->ddr = fpm_idx.ddr;
    docsispd->valid = wan_type;
    docsispd->offset = offset + sizeof(DOCSIS_PD);
    docsispd->token_idx = fpm_idx.token_index;
    docsispd->packet_size = fpm_idx.token_size;

    *swapptr = swap4bytes(*swapptr);
    swapptr++;
    *swapptr = swap4bytes(*swapptr);
}

static void cpu_tx_ds_forward(const rdpa_cpu_tx_info_t *info, void *raw_desc)
{
    RDD_CPU_TX_DS_FORWARD_DESCRIPTOR_DTS *rnr_desc = (RDD_CPU_TX_DS_FORWARD_DESCRIPTOR_DTS *)raw_desc;
    rdp_fpm_index fpm_idx = *(rdp_fpm_index *)&info->data;

    memset(rnr_desc, 0, sizeof(RDD_CPU_TX_DS_FORWARD_DESCRIPTOR_DTS));

    rnr_desc->src_bridge_port = info->port == rdpa_if_wan0 ? RDD_WAN0_VPORT : RDD_WAN1_VPORT;
    rnr_desc->payload_offset = info->data_offset;
    rnr_desc->packet_length = info->data_size;
    rnr_desc->ih_class = info->port == rdpa_if_wan0 ? 8 : 9;
    rnr_desc->buffer_number = fpm_idx.token_index;
    rnr_desc->ddr_id = fpm_idx.ddr;
    rnr_desc->valid = 1;
}

static void cpu_tx_us_forward(const rdpa_cpu_tx_info_t *info, void *raw_desc)
{
    RDD_CPU_TX_US_FORWARD_DESCRIPTOR_DTS *rnr_desc = (RDD_CPU_TX_US_FORWARD_DESCRIPTOR_DTS *)raw_desc;
    rdp_fpm_index fpm_idx = *(rdp_fpm_index *)&info->data;

    memset(rnr_desc, 0, sizeof(RDD_CPU_TX_US_FORWARD_DESCRIPTOR_DTS));
    /* TODO:Convert switch to map */
    if (info->port >= rdpa_if_ssid0  && info->port <= rdpa_if_ssid15)
    {
        rnr_desc->src_bridge_port = RDD_WIFI_VPORT;
        rnr_desc->src_ssid = info->port - rdpa_if_ssid0;
        rnr_desc->ih_class = 2;
    }
    else
    {
        rnr_desc->src_bridge_port = info->port - rdpa_if_lan0 + RDD_VPORT_ID_1;
        rnr_desc->ih_class = info->port - rdpa_if_lan0 + 10;
    }
    rnr_desc->payload_offset = info->data_offset;
    rnr_desc->packet_length = info->data_size + 4; /*must padd the packet for the runner*/
    rnr_desc->buffer_number = fpm_idx.token_index;
    rnr_desc->ddr_id = fpm_idx.ddr;
    rnr_desc->valid = 1;
}

static void cpu_tx_ds_egress(const rdpa_cpu_tx_info_t *info, void *raw_desc)
{
    RDD_CPU_TX_DS_EGRESS_DESCRIPTOR_DTS *rnr_desc = (RDD_CPU_TX_DS_EGRESS_DESCRIPTOR_DTS *)raw_desc;
    rdp_fpm_index fpm_idx = *(rdp_fpm_index *)&info->data;

    memset(rnr_desc , 0 , sizeof(RDD_CPU_TX_DS_EGRESS_DESCRIPTOR_DTS));
    if (info->port >= rdpa_if_ssid0  && info->port <= rdpa_if_ssid15)
    {
        rnr_desc->emac = RDD_WIFI_VPORT;
        rnr_desc->dst_ssid = info->port - rdpa_if_ssid0;
    }
    else
    {
        rnr_desc->emac = info->port - rdpa_if_lan0 + RDD_LAN0_VPORT;
    }
    rnr_desc->tx_queue = info->x.lan.queue_id;
    rnr_desc->src_bridge_port = RDD_CPU_VPORT;
    rnr_desc->payload_offset = info->data_offset;
    rnr_desc->packet_length = info->data_size; /*must padd the packet for the runner*/
    rnr_desc->buffer_number = fpm_idx.token_index;
    rnr_desc->ddr_id = fpm_idx.ddr;
    rnr_desc->valid = 1;

    pr_debug("cpu_tx_ds_egress: %08x %08x\n",
         ((unsigned *)raw_desc)[0],
         ((unsigned *)raw_desc)[1]);
    pr_debug("packet_length:   %d\n"
         "src_bridge_port: %d\n"
         "tx_queue:        %d\n"
         "emac:            %d\n"
         "valid:           %d\n"
         "buffer_number:   %d\n"
         "ddr_id:          %d\n"
         "payload_offset:  %d\n"
         "dst_ssid:        %d\n",
         rnr_desc->packet_length,
         rnr_desc->src_bridge_port,
         rnr_desc->tx_queue,
         rnr_desc->emac,
         rnr_desc->valid,
         rnr_desc->buffer_number,
         rnr_desc->ddr_id,
         rnr_desc->payload_offset,
         rnr_desc->dst_ssid);
}

static void cpu_tx_us_egress(const rdpa_cpu_tx_info_t *info, void *raw_desc)
{
    RDD_CPU_TX_US_EGRESS_DESCRIPTOR_DTS *rnr_desc = (RDD_CPU_TX_US_EGRESS_DESCRIPTOR_DTS *)raw_desc;
    rdp_fpm_index fpm_idx = *(rdp_fpm_index *)&info->data;

    memset(rnr_desc, 0, sizeof(RDD_CPU_TX_US_EGRESS_DESCRIPTOR_DTS));
    rnr_desc->wan_flow = info->port;
    rnr_desc->tx_queue = info->x.wan.queue_id;
    rnr_desc->payload_offset = info->data_offset;
    rnr_desc->packet_length = info->data_size + 4; /*must padd the packet for the runner*/
    rnr_desc->buffer_number = fpm_idx.token_index;
    rnr_desc->ddr_id = fpm_idx.ddr;
    rnr_desc->valid = 1;
}

static cpu_tx_dir_queue cpu_tx_array[2][2] =
{
    {cpu_tx_ds_egress, cpu_tx_us_egress},
    {cpu_tx_us_forward, cpu_tx_ds_forward}
};

inline int rdpa_cpu_tx_pd_set(const rdpa_cpu_tx_info_t *info, void *raw_desc)
{
    int dir = (info->port == rdpa_if_wan0) | (info->port == rdpa_if_wan1);
    cpu_tx_array[info->method][dir](info, raw_desc);

    return 0;
}

static inline rdpa_if rdpa_cmim_to_rdpa_if(uint16_t cmim)
{
    switch (cmim & 0xF)
    {
    case 1:
        return rdpa_if_lan0;
    case 2:
        return rdpa_if_lan1;
    case 3:
        return rdpa_if_lan2;
    case 4:
        return rdpa_if_lan3;
    case 5:
        return rdpa_if_lan5;
    case 6:
        return rdpa_if_lan7;
    case 8:
        return rdpa_if_ssid0 + (cmim >> 4);
    case 9:
        return rdpa_if_ssid8 + (cmim >> 4);
    default:
        return rdpa_if_none;
    }
}

/* in 3390 Cpu RX descriptor is different than Legacy RDP */
inline int rdpa_cpu_rx_pd_get(void *raw_desc /* Input */, rdpa_cpu_rx_info_t *rx_pd/* Output */)
{
    RDD_CPU_RX_DESCRIPTOR_DTS *p_desc = (RDD_CPU_RX_DESCRIPTOR_DTS *)raw_desc;
    RDD_CPU_RX_DESCRIPTOR_DTS rx_desc;
    rdp_fpm_index fpm_idx = {};

    /* copy descriptor to local cached memory, assuming arm memcpy is efficiany */
    memcpy(&rx_desc, p_desc, sizeof(RDD_CPU_RX_DESCRIPTOR_DTS));

    fpm_idx.token_size = rx_desc.packet_length;
    fpm_idx.ddr = rx_desc.ddr_id;
    fpm_idx.token_index = rx_desc.buffer_number;
    fpm_idx.token_valid = 1;

    rx_pd->data = *((uint32_t *)&fpm_idx);
    rx_pd->size = rx_desc.packet_length;
    rx_pd->src_port = rdpa_cpu_rx_srcport_to_rdpa_if(rx_desc.src_bridge_port, rx_desc.src_ssid);
    rx_pd->reason = (rdpa_cpu_reason)rx_desc.reason;
    rx_pd->data_offset = rx_desc.payload_offset;
    rx_pd->dest_ssid = rx_desc.dst_ssid;
    rx_pd->cmim_priority = rx_desc.cmim_priority;
    rx_pd->cmim_inc_bm = rx_desc.cmim_inc_bm;

    if (rx_pd->cmim_inc_bm)
        rx_pd->cmim_if = rdpa_cmim_to_rdpa_if(rx_desc.cmim_if);
    else
        rx_pd->cmim_if = 0;

    return 0;
}
#endif

#ifndef CM3390
inline int rdpa_cpu_rx_pd_get(void *raw_desc, rdpa_cpu_rx_info_t *rx_pd)
{
    CPU_RX_DESCRIPTOR rx_desc;
#if defined(__ARMEL__)
    register uint32_t w0 __asm__("r8");
    register uint32_t w1 __asm__("r9");
    register uint32_t w2 __asm__("r10");

    READ_RX_DESC(raw_desc);
#else
    CPU_RX_DESCRIPTOR *p_desc = (CPU_RX_DESCRIPTOR *)raw_desc;
#endif

    /* p_desc is in uncached mem so reading 32bits at a time into
     cached mem improves performance will be change to BurstBank read later*/
#if defined(__ARMEL__)
    rx_desc.word2 = swap4bytes(w2);
#else
    rx_desc.word2 = p_desc->word2;
#endif
    if (rx_desc.word2 & 0x80000000)
    {
        rx_desc.word2 &= ~0x80000000;
        rx_pd->data = (uint32_t)PHYS_TO_CACHED(rx_desc.word2);

#if defined(__ARMEL__)
        rx_desc.word0 = swap4bytes(w0);
#else
        rx_desc.word0 = p_desc->word0;
#endif
        rx_pd->size = rx_desc.packet_length;
        cache_invalidate_len_outer_first((void *)rx_pd->data, rx_pd->size);

#if !defined(CONFIG_BCM963138) && !defined(_BCM963138_) && !defined(CONFIG_BCM963148) && !defined(_BCM963148_)
        rx_pd->reason_data = rx_desc.flow_id;
        rx_desc.word1 = p_desc->word1;
        rx_desc.word1 = swap4bytes(rx_desc.word1);
        rx_pd->reason = (rdpa_cpu_reason)rx_desc.reason;
        rx_pd->dest_ssid = rx_desc.dst_ssid;
        rx_desc.word3 = p_desc->word3;
        rx_desc.word3 = swap4bytes(rx_desc.word3);
        rx_pd->wl_metadata = rx_desc.wl_metadata;
        rx_pd->ptp_index = p_desc->ip_sync_1588_idx;
#else
        rx_pd->reason = rdpa_cpu_rx_reason_ip_flow_miss;
#endif

#if defined(CONFIG_BCM_PKTRUNNER_CSUM_OFFLOAD)
        rx_pd->rx_csum_verified = rx_desc.is_chksum_verified;
#endif
        rx_pd->src_port = rdpa_cpu_rx_srcport_to_rdpa_if(rx_desc.source_port, rx_desc.flow_id);
        return 0;
    }

    return BDMF_ERR_NO_MORE;
}

/** Resets the descriptor with a new data pointer and sets descriptor ownership to the \RUNNER.
 *
 * \param[in]   raw_desc           Raw packet descriptor.
 * \param[out]  data               New data pointer.
 */

inline void rdpa_cpu_ring_rest_desc(volatile void *__restrict__ raw_desc, void *__restrict__ data)
{
    volatile CPU_RX_DESCRIPTOR *p_desc = (volatile CPU_RX_DESCRIPTOR *)raw_desc;

    p_desc->word2 = swap4bytes(VIRT_TO_PHYS(data)) & 0x7fffffff;
}

/**
 *
 * \param[in]   raw_desc            Raw packet descriptor.
 * \return true if there is a valid packet in descriptor.
 */

const inline int rdpa_cpu_ring_not_empty(const void *raw_desc)
{
   CPU_RX_DESCRIPTOR *p_desc = (CPU_RX_DESCRIPTOR *)raw_desc;

#if defined(__ARMEL__)
    return p_desc->word2 & 0x80;
#else
    return p_desc->word2 & 0x80000000;
#endif
}
/** @} end of add to cpu Doxygen group */

#endif
#endif
