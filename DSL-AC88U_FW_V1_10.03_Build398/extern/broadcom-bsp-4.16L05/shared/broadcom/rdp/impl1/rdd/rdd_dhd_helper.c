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

#include "rdd.h"
#include "rdd_runner_defs_auto.h"
#include "rdd_data_structures.h"
#include "rdd_dhd_helper.h"
#include "rdpa_dhd_helper_basic.h"
#if !defined(FIRMWARE_INIT)
#include <linux/nbuff.h>
#include "bcm_mm.h"
#include "rdp_mm.h"
#else
#include "rdp_drv_bpm.h"
#include "access_macros.h"
#endif
#ifdef LEGACY_RDP
#include "rdd_legacy_conv.h"
#endif

#ifdef CM3390
extern uint32_t g_runner_ddr0_base_addr;
#endif
#ifndef G9991
bdmf_boolean is_dhd_enabled[RDPA_MAX_RADIOS] = {0};
RDD_DHD_COMPLETE_RING_DESCRIPTOR_DTS g_dhd_complete_ring_desc[RDPA_MAX_RADIOS] = {{0, 0, 0, 0},};

extern uint32_t g_runner_ddr_base_addr;
extern bdmf_fastlock int_lock_irq;
extern rdpa_bpm_buffer_size_t g_bpm_buffer_size;

static void rdd_rx_post_descr_init(uint32_t radio_idx, uint32_t descr_ptr, uint32_t bpm_buffer_number,
    bdmf_boolean valid_bpm)
{
    uint32_t req_id, data_buf_ptr_low;
    uint16_t len;

    RDD_DHD_RX_POST_DESCRIPTOR_MSG_TYPE_WRITE(DHD_MSG_TYPE_RX_POST, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_IF_ID_WRITE(radio_idx, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_COMMON_HDR_FLAGS_WRITE(0, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_EPOCH_WRITE(0, descr_ptr);

#if defined(DSL_63138)
    req_id = bpm_buffer_number | (DRV_BPM_SP_SPARE_1 << 15);
    len = __swap2bytes(DHD_DATA_LEN);
#elif defined(DSL_63148)
    req_id = bpm_buffer_number | (DRV_BPM_SP_SPARE_1 << 14);
    len = __swap2bytes(DHD_DATA_LEN);
#else
    req_id = bpm_buffer_number | (DRV_BPM_SP_SPARE_1 << 14);
    len = cpu_to_le16(DHD_DATA_LEN);
#endif
#ifdef CM3390
    data_buf_ptr_low = valid_bpm ? g_runner_ddr0_base_addr + bpm_buffer_number * 512 + DHD_DATA_OFFSET : 0;
#else
    data_buf_ptr_low = valid_bpm ? g_runner_ddr_base_addr + bpm_buffer_number * g_bpm_buffer_size +
        DHD_DATA_OFFSET : 0;
#endif
#if !defined(FIRMWARE_INIT)
    data_buf_ptr_low = VIRT_TO_PHYS(data_buf_ptr_low);
#endif
#if defined(DSL_63138) || defined(DSL_63148)
    data_buf_ptr_low = __swap4bytes(data_buf_ptr_low);
#else
    data_buf_ptr_low = cpu_to_le32(data_buf_ptr_low);
#endif

    RDD_DHD_RX_POST_DESCRIPTOR_REQUEST_ID_WRITE(req_id, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_META_BUF_LEN_WRITE(0, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_DATA_LEN_WRITE(len, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_METADATA_BUF_ADDR_HI_WRITE(0, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_METADATA_BUF_ADDR_LOW_WRITE(0, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_DATA_BUF_ADDR_HI_WRITE(0, descr_ptr);
    RDD_DHD_RX_POST_DESCRIPTOR_DATA_BUF_ADDR_LOW_WRITE(data_buf_ptr_low, descr_ptr); 

#if !defined(FIRMWARE_INIT)
    if (valid_bpm)
        cache_flush_len((void *)descr_ptr, 32);
#endif
}

#if 0
static void rdd_rx_post_descr_dump(uint32_t desc)
{
    uint32_t req_id, buf_addr;
    uint16_t data_len;

    RDD_DHD_RX_POST_DESCRIPTOR_REQUEST_ID_READ(req_id, desc);
    RDD_DHD_RX_POST_DESCRIPTOR_DATA_LEN_READ(data_len, desc);
    RDD_DHD_RX_POST_DESCRIPTOR_DATA_BUF_ADDR_LOW_READ(buf_addr, desc);
}
#endif

int rdd_dhd_rx_post_init(uint32_t radio_idx, rdpa_dhd_init_cfg_t *init_cfg)
{
#if !defined(FIRMWARE_INIT)
    uint32_t bpm_buffer_number, write_ptr;
    int i, rc;

    for (i = 0, write_ptr = (uint32_t)init_cfg->rx_post_flow_ring_base_addr; i < DHD_RX_POST_FLOW_RING_SIZE - 1; i++)
    {
        rc = fi_bl_drv_bpm_req_buffer(DRV_BPM_SP_SPARE_1, (uint32_t *)&bpm_buffer_number);
        if (rc != DRV_BPM_ERROR_NO_ERROR)
            return rc;
        rdd_rx_post_descr_init(radio_idx, write_ptr, bpm_buffer_number, 1);
        write_ptr += sizeof(RDD_DHD_RX_POST_DESCRIPTOR_DTS);
    }
#endif
    return 0;
}

int rdd_dhd_rx_post_uninit(rdpa_dhd_init_cfg_t *init_cfg)
{
#if !defined(FIRMWARE_INIT)
    uint32_t bpm_buffer_number, descr_ptr;
    int i, rc;

    for (i = 0, descr_ptr = (uint32_t)init_cfg->rx_post_flow_ring_base_addr; i < DHD_RX_POST_FLOW_RING_SIZE - 1; i++)
    {
        RDD_DHD_RX_POST_DESCRIPTOR_REQUEST_ID_READ(bpm_buffer_number, descr_ptr);
#if defined(DSL_63138)
        bpm_buffer_number &= ~(DRV_BPM_SP_SPARE_1 << 15);
        rc = fi_bl_drv_bpm_free_buffer(DRV_BPM_SP_SPARE_1, bpm_buffer_number);
#elif defined(DSL_63148)
        bpm_buffer_number &= ~(DRV_BPM_SP_SPARE_1 << 14);
        rc = fi_bl_drv_bpm_free_buffer(DRV_BPM_SP_SPARE_1, bpm_buffer_number);
#else
        bpm_buffer_number &= ~(DRV_BPM_SP_SPARE_1 << 14);
        rc = fi_bl_drv_bpm_free_buffer(DRV_BPM_SP_SPARE_1, bpm_buffer_number);
#endif

        if (rc)
            bdmf_trace("Error releasing BPM num %d, rc = %d\n", bpm_buffer_number, rc);
        descr_ptr += sizeof(RDD_DHD_RX_POST_DESCRIPTOR_DTS);
    }
#endif
    return 0;
}

void rdd_dhd_mode_enable(uint32_t radio_idx, bdmf_boolean enable)
{
    is_dhd_enabled[radio_idx] = enable;
}

void rdd_dhd_mode_enable_init(void)
{
    uint32_t radio_idx;
    for (radio_idx = 0; radio_idx < RDPA_MAX_RADIOS; radio_idx++)
    {        
        is_dhd_enabled[radio_idx] = (bdmf_boolean) 0;
    }
}

static void rdd_dhd_tx_post_descr_init(uint32_t *descr_addr)
{
    RDD_DHD_TX_POST_DESCRIPTOR_DTS *descr = (RDD_DHD_TX_POST_DESCRIPTOR_DTS *)descr_addr;

    RDD_DHD_TX_POST_DESCRIPTOR_MSG_TYPE_WRITE(DHD_MSG_TYPE_TX_POST, descr);
    RDD_DHD_TX_POST_DESCRIPTOR_COMMON_HDR_FLAGS_WRITE(0, descr);
    RDD_DHD_TX_POST_DESCRIPTOR_EPOCH_WRITE(0, descr);
    RDD_DHD_TX_POST_DESCRIPTOR_SEG_CNT_WRITE(1, descr);
    RDD_DHD_TX_POST_DESCRIPTOR_METADATA_BUF_ADDR_HI_WRITE(0, descr);
    RDD_DHD_TX_POST_DESCRIPTOR_METADATA_BUF_ADDR_LOW_WRITE(0, descr);
    RDD_DHD_TX_POST_DESCRIPTOR_META_BUF_LEN_WRITE(0, descr);
    RDD_DHD_TX_POST_DESCRIPTOR_DATA_BUF_ADDR_HI_WRITE(0, descr);
}

static void rdd_dhd_helper_tx_post_buffers_threshold_set(void)
{
#ifndef U_LINUX
    BPM_MODULE_REGS_BPM_GL_TRSH global_configuration;
    uint16_t *threshold_ptr = (uint16_t *)(DEVICE_ADDRESS(RUNNER_COMMON_0_OFFSET) + DHD_TX_POST_BUFFERS_THRESHOLD_ADDRESS);
    uint16_t threshold;

    BPM_MODULE_REGS_BPM_GL_TRSH_READ(global_configuration);

    /* Get total number of BPM buffers. */
    if (global_configuration.gl_bat <= DRV_BPM_GLOBAL_THRESHOLD_30K)
        threshold = (global_configuration.gl_bat + 1) * 2560;
    else
        threshold = 2560;

    /* Set tx post buffers threshold to half of the total number of BPM buffers. */
    threshold /= 2;

    /* The tx post buffers threshold is global for all radios. */
    MWRITE_16((uint8_t *)threshold_ptr, threshold);
#endif
}

#if !defined(FIRMWARE_INIT)
#define DHD_VTOP(val) (uint32_t)(VIRT_TO_PHYS(val))
#else
#define DHD_VTOP(val) (uint32_t)(val)
#endif

int rdd_dhd_hlp_cfg(uint32_t radio_idx, rdpa_dhd_init_cfg_t *init_cfg, int enable)
{
    RDD_DHD_RADIO_INSTANCE_COMMON_A_DATA_DTS *radio_instance_data_a_ptr;
    RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_DTS *radio_instance_entry_a_ptr;
    RDD_DHD_RADIO_INSTANCE_COMMON_B_DATA_DTS *radio_instance_data_ptr;
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_DTS *radio_instance_entry_ptr;
    RDD_DHD_RX_POST_FLOW_RING_BUFFER_DTS *rxp_flring_buffer_ptr;
    RDD_DHD_RX_POST_DESCRIPTOR_DTS *rxp_flring_entry_ptr;
    uint32_t i, rx_post_ptr, rx_post_last_wr_idx;

    radio_instance_data_ptr = (RDD_DHD_RADIO_INSTANCE_COMMON_B_DATA_DTS *)(DEVICE_ADDRESS(RUNNER_COMMON_1_OFFSET) +
        DHD_RADIO_INSTANCE_COMMON_B_DATA_ADDRESS - sizeof(RUNNER_COMMON));
    radio_instance_entry_ptr = &radio_instance_data_ptr->entry[radio_idx];

    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_RX_POST_FR_BASE_PTR_WRITE(DHD_VTOP(init_cfg->rx_post_flow_ring_base_addr), radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_RX_COMPLETE_FR_BASE_PTR_WRITE(DHD_VTOP(init_cfg->rx_complete_flow_ring_base_addr), radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_TX_COMPLETE_FR_BASE_PTR_WRITE(DHD_VTOP(init_cfg->tx_complete_flow_ring_base_addr), radio_instance_entry_ptr);

#if defined(DSL_63138) || defined(DSL_63148)
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_R2D_WR_FR_DESC_BASE_PTR_WRITE(init_cfg->r2d_wr_arr_base_phys_addr, radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_D2R_RD_FR_DESC_BASE_PTR_WRITE(init_cfg->d2r_rd_arr_base_phys_addr, radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_R2D_RD_FR_DESC_BASE_PTR_WRITE(init_cfg->r2d_rd_arr_base_phys_addr, radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_D2R_WR_FR_DESC_BASE_PTR_WRITE(init_cfg->d2r_wr_arr_base_phys_addr, radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_TX_POST_MGMT_FR_BASE_PTR_WRITE(init_cfg->tx_post_mgmt_arr_base_phys_addr, radio_instance_entry_ptr);

    /* Used by rdd_dhd_helper_shell.c. */
    radio_instance_entry_ptr->tx_post_mgmt_fr_base_ptr_rdd_only = (uint32_t) init_cfg->tx_post_mgmt_arr_base_addr;
#else
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_R2D_WR_FR_DESC_BASE_PTR_WRITE(DHD_VTOP(init_cfg->r2d_wr_arr_base_addr), radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_D2R_RD_FR_DESC_BASE_PTR_WRITE(DHD_VTOP(init_cfg->d2r_rd_arr_base_addr), radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_R2D_RD_FR_DESC_BASE_PTR_WRITE(DHD_VTOP(init_cfg->r2d_rd_arr_base_addr), radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_D2R_WR_FR_DESC_BASE_PTR_WRITE(DHD_VTOP(init_cfg->d2r_wr_arr_base_addr), radio_instance_entry_ptr);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_TX_POST_MGMT_FR_BASE_PTR_WRITE(DHD_VTOP(init_cfg->tx_post_mgmt_arr_base_addr), radio_instance_entry_ptr);
#endif

    rdd_dhd_tx_post_descr_init((uint32_t *)(DEVICE_ADDRESS(RUNNER_PRIVATE_0_OFFSET) + DS_DHD_TX_POST_FLOW_RING_BUFFER_ADDRESS));
    rdd_dhd_tx_post_descr_init((uint32_t *)(DEVICE_ADDRESS(RUNNER_PRIVATE_1_OFFSET) + US_DHD_TX_POST_FLOW_RING_BUFFER_ADDRESS));

    rxp_flring_buffer_ptr = (RDD_DHD_RX_POST_FLOW_RING_BUFFER_DTS *)(DEVICE_ADDRESS(RUNNER_PRIVATE_1_OFFSET) + DHD_RX_POST_FLOW_RING_BUFFER_ADDRESS);
    rxp_flring_entry_ptr =  &rxp_flring_buffer_ptr->entry[radio_idx].dhd_rx_post_descriptor;
    rdd_rx_post_descr_init(radio_idx, (uint32_t)rxp_flring_entry_ptr, 0, 0);

    rdd_dhd_mode_enable(radio_idx, enable);

    /* Update RX_post WR index both in SRAM and DDR. No need doorbell DHD that there are buffers available in RX Post.*/
#if defined(DSL_63138) || defined(DSL_63148)
    rx_post_last_wr_idx = __swap2bytes(DHD_RX_POST_FLOW_RING_SIZE - 1);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_RX_POST_R2D_INDEX_WRITE(rx_post_last_wr_idx, radio_instance_entry_ptr);

#if !defined(FIRMWARE_INIT)
    if (enable)
        *((uint16_t *)init_cfg->r2d_wr_arr_base_addr + 1) = DHD_RX_POST_FLOW_RING_SIZE - 1;
#endif
#else
    rx_post_last_wr_idx = cpu_to_le16(DHD_RX_POST_FLOW_RING_SIZE - 1);
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_RX_POST_R2D_INDEX_WRITE(rx_post_last_wr_idx, radio_instance_entry_ptr);
#if !defined(FIRMWARE_INIT)
    if (enable)
        *((uint16_t *)init_cfg->r2d_wr_arr_base_addr + 1) = rx_post_last_wr_idx;
#endif
#endif
    
    radio_instance_data_a_ptr = (RDD_DHD_RADIO_INSTANCE_COMMON_A_DATA_DTS *)(DEVICE_ADDRESS(RUNNER_COMMON_0_OFFSET) + DHD_RADIO_INSTANCE_COMMON_A_DATA_ADDRESS);
    radio_instance_entry_a_ptr = &radio_instance_data_a_ptr->entry[radio_idx];

    RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_DS_DHD_DOORBELL_WRITE(init_cfg->dongle_wakeup_register, radio_instance_entry_a_ptr);
    if (!enable)
    {
        /* Reset the rest of the fields */
        RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_DS_RD_FR_R2D_INDEXES_WRITE(0, radio_instance_entry_a_ptr);
        RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_DS_WR_FR_R2D_INDEXES_WRITE(0, radio_instance_entry_a_ptr);
        RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_TX_RD_FR_D2R_INDEXES_WRITE(0, radio_instance_entry_a_ptr);
        RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_TX_WR_FR_D2R_INDEXES_WRITE(0, radio_instance_entry_a_ptr);
        RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_TX_COMPLETE_PACKET_COUNTER_WRITE(0, radio_instance_entry_a_ptr);
        RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_RX_RD_FR_D2R_INDEXES_WRITE(0, radio_instance_entry_ptr);
        RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_RX_WR_FR_D2R_INDEXES_WRITE(0, radio_instance_entry_ptr);
    }
    RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_US_DHD_DOORBELL_WRITE(init_cfg->dongle_wakeup_register, radio_instance_entry_ptr);


    for (i = 0, rx_post_ptr = (uint32_t)init_cfg->rx_post_flow_ring_base_addr; enable && i < DHD_RX_POST_FLOW_RING_SIZE; i++)
    {
#if 0
        rdd_rx_post_descr_dump(rx_post_ptr);
#endif
        rx_post_ptr += sizeof(RDD_DHD_RX_POST_DESCRIPTOR_DTS);
    }

    rdd_dhd_helper_tx_post_buffers_threshold_set();

    return 0;
}

void rdd_dhd_helper_flow_ring_flush(uint32_t radio_idx, uint32_t flow_ring_idx)
{
    unsigned long flags;

    bdmf_fastlock_lock_irq(&int_lock_irq, flags);
#ifndef CM3390
    rdd_cpu_tx_send_message(RDD_CPU_TX_MESSAGE_DHD_MESSAGE, PICO_RUNNER_A, RUNNER_PRIVATE_0_OFFSET,
        DHD_MSG_TYPE_FLOW_RING_FLUSH, flow_ring_idx | (radio_idx << 14), 0, 1);
#endif
    bdmf_fastlock_unlock_irq(&int_lock_irq, flags);   
}

void rdd_dhd_helper_flow_ring_disable(uint32_t radio_idx, uint32_t flow_ring_idx)
{
    unsigned long flags;

    bdmf_fastlock_lock_irq(&int_lock_irq, flags);
#ifndef CM3390
    rdd_cpu_tx_send_message(RDD_CPU_TX_MESSAGE_DHD_MESSAGE, PICO_RUNNER_A, RUNNER_PRIVATE_0_OFFSET,
        DHD_MSG_TYPE_FLOW_RING_SET_DISABLED, flow_ring_idx | (radio_idx << 14), 0, 0);
#endif
    bdmf_fastlock_unlock_irq(&int_lock_irq, flags);   
}

void rdd_dhd_helper_wakeup_information_get(rdpa_dhd_wakeup_info_t *wakeup_info)
{
    RUNNER_REGS_CFG_CPU_WAKEUP runner_cpu_wakeup_register;

#if (defined(DSL_63138) || defined(DSL_63148)) && !defined(FIRMWARE_INIT)
    wakeup_info->tx_complete_wakeup_register = RUNNER_REGS_0_CFG_CPU_WAKEUP_ADDRESS - RDP_BASE + RDP_PHYS_BASE; 
#else
    wakeup_info->tx_complete_wakeup_register = RUNNER_REGS_0_CFG_CPU_WAKEUP_ADDRESS; 
#endif

#ifdef CM3390
    runner_cpu_wakeup_register.req_trgt = (DHD0_TX_COMPLETE_THREAD_NUMBER + wakeup_info->radio_idx) >> 5;
    runner_cpu_wakeup_register.thread_num = (DHD0_TX_COMPLETE_THREAD_NUMBER + wakeup_info->radio_idx) & 0x1f;
#else
    runner_cpu_wakeup_register.req_trgt = (DHD_TX_COMPLETE_FAST_A_THREAD_NUMBER + wakeup_info->radio_idx) >> 5;
    runner_cpu_wakeup_register.thread_num = (DHD_TX_COMPLETE_FAST_A_THREAD_NUMBER + wakeup_info->radio_idx) & 0x1f;
#endif
    runner_cpu_wakeup_register.urgent_req = 0;
    runner_cpu_wakeup_register.reserved0 = 0;
    MWRITE_32(&wakeup_info->tx_complete_wakeup_value, *(uint32_t *)&runner_cpu_wakeup_register);

#if (defined(DSL_63138) || defined(DSL_63148)) && !defined(FIRMWARE_INIT)
    wakeup_info->rx_complete_wakeup_register = RUNNER_REGS_1_CFG_CPU_WAKEUP_ADDRESS - RDP_BASE + RDP_PHYS_BASE;
#else
    wakeup_info->rx_complete_wakeup_register = RUNNER_REGS_1_CFG_CPU_WAKEUP_ADDRESS;
#endif

    runner_cpu_wakeup_register.req_trgt = (DHD_RX_THREAD_NUMBER + wakeup_info->radio_idx) >> 5;
    runner_cpu_wakeup_register.thread_num = (DHD_RX_THREAD_NUMBER + wakeup_info->radio_idx) & 0x1f;
    runner_cpu_wakeup_register.urgent_req = 0;
    runner_cpu_wakeup_register.reserved0 = 0;
    MWRITE_32(&wakeup_info->rx_complete_wakeup_value, *(uint32_t *)&runner_cpu_wakeup_register);
}

int rdd_dhd_helper_dhd_complete_ring_create(uint32_t radio_idx, uint32_t ring_size)
{
    int rc = 0;
#if !defined(FIRMWARE_INIT)
    RDD_DHD_COMPLETE_RING_DESCRIPTOR_BUFFER_DTS *dhd_complete_desc_buffer_ptr;
    RDD_DHD_COMPLETE_RING_DESCRIPTOR_DTS *dhd_complete_desc_ptr;
    RDD_DHD_COMPLETE_RING_DESCRIPTOR_DTS *pdesc = &g_dhd_complete_ring_desc[radio_idx];
    uint32_t i, phy_addr, ring_ptr;

    if (ring_size)
    {
        /* create an array of ring elements */
        if (pdesc->ring_base == 0)
        {
            pdesc->ring_base = (uint32_t)rdp_mm_aligned_alloc(sizeof(RDD_DHD_COMPLETE_RING_ENTRY_DTS) * ring_size, &phy_addr);
            if (pdesc->ring_base)
            {
                /* Initialize RDD descriptor */
                pdesc->ring_size = ring_size;
                pdesc->ring_end = (uint32_t) pdesc->ring_base + ((ring_size - 1) * sizeof(RDD_DHD_COMPLETE_RING_ENTRY_DTS));
                pdesc->ring_ptr = (uint32_t) pdesc->ring_base;

                /* Initialize firmware descriptor */
                dhd_complete_desc_buffer_ptr = RDD_DHD_COMPLETE_RING_DESCRIPTOR_BUFFER_PTR();
                dhd_complete_desc_ptr = &dhd_complete_desc_buffer_ptr->entry[radio_idx];

                RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_SIZE_WRITE(ring_size, dhd_complete_desc_ptr);
                RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_BASE_WRITE(phy_addr, dhd_complete_desc_ptr);
                RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_END_WRITE(phy_addr + ((ring_size - 1) * sizeof(RDD_DHD_COMPLETE_RING_ENTRY_DTS)), dhd_complete_desc_ptr);
                RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_PTR_WRITE(phy_addr, dhd_complete_desc_ptr);

                /* Initialize the ring elements to be owned by Runner */
                for (i = 0, ring_ptr = pdesc->ring_ptr; i < ring_size; i++, ring_ptr += sizeof(RDD_DHD_COMPLETE_RING_ENTRY_DTS))
                {
                    RDD_DHD_COMPLETE_RING_ENTRY_RING_VALUE_WRITE(0, ring_ptr);
                    RDD_DHD_COMPLETE_RING_ENTRY_OWNERSHIP_WRITE(DHD_COMPLETE_OWNERSHIP_RUNNER, ring_ptr);
                }
            }
            else
                rc = BDMF_ERR_NOMEM;
        }
        else
            rc = BDMF_ERR_ALREADY;
    }
#endif

    return rc;
}

int rdd_dhd_helper_dhd_complete_ring_destroy(uint32_t radio_idx, uint32_t ring_size)
{
    int rc = 0;
#if !defined(FIRMWARE_INIT)
    RDD_DHD_COMPLETE_RING_DESCRIPTOR_BUFFER_DTS *dhd_complete_desc_buffer_ptr;
    RDD_DHD_COMPLETE_RING_DESCRIPTOR_DTS *dhd_complete_desc_ptr;
    RDD_DHD_COMPLETE_RING_DESCRIPTOR_DTS *pdesc = &g_dhd_complete_ring_desc[radio_idx];

    if (ring_size)
    {
        /* create an array of ring elements */
        if (pdesc->ring_base)
        {
            rdp_mm_aligned_free((void *)pdesc->ring_base, sizeof(RDD_DHD_COMPLETE_RING_ENTRY_DTS) * ring_size);
            pdesc->ring_size = 0;
            pdesc->ring_base = 0;
            pdesc->ring_end = 0;
            pdesc->ring_ptr = 0;
 
            dhd_complete_desc_buffer_ptr = RDD_DHD_COMPLETE_RING_DESCRIPTOR_BUFFER_PTR();
            dhd_complete_desc_ptr = &dhd_complete_desc_buffer_ptr->entry[radio_idx];

            RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_SIZE_WRITE(0, dhd_complete_desc_ptr);
            RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_BASE_WRITE(0, dhd_complete_desc_ptr);
            RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_END_WRITE(0, dhd_complete_desc_ptr);
            RDD_DHD_COMPLETE_RING_DESCRIPTOR_RING_PTR_WRITE(0, dhd_complete_desc_ptr);
        }
        else
            rc = BDMF_ERR_ALREADY;
    }
#endif

    return rc;
}

uint16_t rdd_dhd_helper_ssid_tx_dropped_packets_get(uint32_t radio_idx, uint32_t ssid)
{
    uint16_t counter;

#if defined DSL_63138 || defined DSL_63148 || defined CM3390
    rdd_2_bytes_counter_get(DHD_SSID_DROP_PACKET_GROUP + radio_idx, ssid, &counter);
#else
    rdd_2_bytes_counter_get(DHD_SSID_DROP_PACKET_GROUP + radio_idx, ssid, 0, &counter);
#endif

    return counter;
}

#endif
