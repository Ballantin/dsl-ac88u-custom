/*
   Copyright (c) 2014 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2014:DUAL/GPL:standard
    
    Unless you and Broadcom execute a separate written software license
    agreement governing use of this software, this software is licensed
    to you under the terms of the GNU General Public License version 2
    (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
    with the following added to such license:
    
       As a special exception, the copyright holders of this software give
       you permission to link this software with independent modules, and
       to copy and distribute the resulting executable under terms of your
       choice, provided that you also meet, for each linked independent
       module, the terms and conditions of the license of that module.
       An independent module is a module which is not derived from this
       software.  The special exception does not apply to any modifications
       of the software.
    
    Not withstanding the above, under no circumstances may you combine
    this software in any way with any other Broadcom software provided
    under a license other than the GPL, without Broadcom's express prior
    written consent.
    
:>
*/

#ifndef _RDD_DHD_HELPER_H
#define _RDD_DHD_HELPER_H

#include "rdd.h"
#include "rdd_data_structures_auto.h"
#include "dhd_defs.h"
#include "rdpa_types.h"
#include "rdpa_dhd_helper_basic.h"
#include "bdmf_shell.h"

#define FLOW_RING_RX_COMPLETE_IDX 4

extern RDD_DHD_COMPLETE_RING_DESCRIPTOR_DTS g_dhd_complete_ring_desc[];

/* Init Flow Rings array, including feeding the RX_post array with BPMs */
int rdd_dhd_hlp_cfg(uint32_t radio_idx, rdpa_dhd_init_cfg_t *init_cfg, int enable);
bdmf_boolean rdd_dhd_helper_flow_ring_is_enabled(uint32_t flow_ring_idx);
int rdd_dhd_rx_post_init(uint32_t radio_idx, rdpa_dhd_init_cfg_t *init_cfg);
int rdd_dhd_rx_post_uninit(rdpa_dhd_init_cfg_t *init_cfg);

void rdd_dhd_helper_flow_ring_flush(uint32_t radio_idx, uint32_t flow_ring_idx);
void rdd_dhd_helper_flow_ring_disable(uint32_t radio_idx, uint32_t flow_ring_idx);
void rdd_dhd_helper_shell_cmds_init(bdmfmon_handle_t rdd_dir);
void rdd_dhd_helper_wakeup_information_get(rdpa_dhd_wakeup_info_t *wakeup_info);
int rdd_dhd_helper_dhd_complete_ring_create(uint32_t radio_idx, uint32_t ring_size);
int rdd_dhd_helper_dhd_complete_ring_destroy(uint32_t radio_idx, uint32_t ring_size);
uint16_t rdd_dhd_helper_ssid_tx_dropped_packets_get(uint32_t radio_idx, uint32_t ssid);

static inline void rdd_dhd_helper_wakeup(uint32_t radio_idx, bdmf_boolean is_tx_complete)
{
    RUNNER_REGS_CFG_CPU_WAKEUP cpu_wakeup_reg = {};
#ifdef CM3390
    uint32_t dhd_tx_complete_thread = DHD0_TX_COMPLETE_THREAD_NUMBER + radio_idx;
#else
    uint32_t dhd_tx_complete_thread = DHD_TX_COMPLETE_FAST_A_THREAD_NUMBER + radio_idx;
#endif
    uint32_t dhd_rx_complete_thread = DHD_RX_THREAD_NUMBER + radio_idx;

    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    cpu_wakeup_reg.req_trgt = (is_tx_complete ? dhd_tx_complete_thread : dhd_rx_complete_thread) / 32;
    cpu_wakeup_reg.thread_num = (is_tx_complete ? dhd_tx_complete_thread : dhd_rx_complete_thread) % 32;
    cpu_wakeup_reg.urgent_req = 0;
   
    if (is_tx_complete)
        RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE(cpu_wakeup_reg);
    else
        RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE(cpu_wakeup_reg);
}


static inline int rdd_dhd_helper_dhd_complete_message_get(rdpa_dhd_complete_data_t *dhd_complete_info)
{
    RDD_DHD_COMPLETE_RING_DESCRIPTOR_DTS *pdesc = &g_dhd_complete_ring_desc[dhd_complete_info->radio_idx];
    uint32_t request_id_buffer_type;
    int rc = 0;

    RDD_DHD_COMPLETE_RING_ENTRY_REQUEST_ID_READ(request_id_buffer_type, pdesc->ring_ptr);

    if (RDD_DHD_COMPLETE_RING_ENTRY_OWNERSHIP_L_READ(request_id_buffer_type) != DHD_COMPLETE_OWNERSHIP_RUNNER)
    {
        /* Set the return parameters. */
        dhd_complete_info->request_id = request_id_buffer_type;
        RDD_DHD_COMPLETE_RING_ENTRY_STATUS_READ(dhd_complete_info->status, pdesc->ring_ptr);
        RDD_DHD_COMPLETE_RING_ENTRY_FLOW_RING_ID_READ(dhd_complete_info->flow_ring_id, pdesc->ring_ptr);

        /* Set the ring element to be owned by Runner */
        RDD_DHD_COMPLETE_RING_ENTRY_REQUEST_ID_WRITE(0, pdesc->ring_ptr);
        RDD_DHD_COMPLETE_RING_ENTRY_OWNERSHIP_WRITE(DHD_COMPLETE_OWNERSHIP_RUNNER, pdesc->ring_ptr);

        /* Update the ring pointer to the next element. */
        if (pdesc->ring_ptr == pdesc->ring_end)
            pdesc->ring_ptr = pdesc->ring_base;
        else
            pdesc->ring_ptr += sizeof(RDD_DHD_COMPLETE_RING_ENTRY_DTS);
    }
    else
        rc = BDMF_ERR_ALREADY;

    return rc;
}

/* Definitions taken from DHD driver (cannot include it's header) */
typedef union {
    struct {
        uint32_t low;
        uint32_t high;
    };
    struct {
        uint32_t low_addr;
        uint32_t high_addr;
    };
    uint64_t u64;
} addr64_t;

typedef struct 
{
    /* message type */
    uint8_t msg_type;
    /* interface index this is valid for */
    uint8_t if_id;
    /* flags */
    uint8_t flags;
    /* alignment */
    uint8_t reserved;
    /* packet Identifier for the associated host buffer */
    uint32_t request_id;
} cmn_msg_hdr_t;

typedef struct 
{
    /* common message header */
    cmn_msg_hdr_t cmn_hdr;
    /* provided meta data buffer len */
    uint16_t metadata_buf_len;
    /* provided data buffer len to receive data */
    uint16_t data_buf_len;
    /* alignment to make the host buffers start on 8 byte boundary */
    uint32_t rsvd;
    /* provided meta data buffer */
    addr64_t metadata_buf_addr;
    /* provided data buffer to receive data */
    addr64_t data_buf_addr;
} host_rxbuf_post_t;

#endif /* _RDD_DHD_HELPER_H */

