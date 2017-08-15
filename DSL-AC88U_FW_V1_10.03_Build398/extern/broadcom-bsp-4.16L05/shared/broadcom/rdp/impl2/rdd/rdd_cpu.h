
/*
   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2013:DUAL/GPL:standard
    
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

#ifndef _BL_LILAC_DRV_RUNNER_CPU_H
#define _BL_LILAC_DRV_RUNNER_CPU_H
extern uint32_t  g_runner_ddr_base_addr;
extern uint32_t  g_ddr_headroom_size;
extern uint32_t  g_cpu_tx_queue_write_ptr[ 4 ];
extern uint32_t  g_cpu_tx_queue_free_counter[ 4 ];
extern uint32_t  g_cpu_tx_queue_abs_data_ptr_write_ptr[ 4 ];
#if !defined(DSL_63138) && !defined(DSL_63148)
extern uint32_t  g_cpu_tx_skb_free_indexes_head_ptr;
extern uint32_t  g_cpu_tx_skb_free_indexes_counter;
#endif
extern uint32_t  g_cpu_tx_skb_free_indexes_release_ptr;
extern uint32_t  g_cpu_tx_released_skb_counter;
extern uint32_t  g_cpu_tx_no_free_skb_counter;
extern uint32_t  g_cpu_tx_sent_abs_packets_counter;
extern uint32_t  g_cpu_tx_abs_packet_limit;

#if !defined(FIRMWARE_INIT)

#if defined(CONFIG_BCM_PKTRUNNER_GSO)
extern uint32_t  g_cpu_tx_no_free_gso_desc_counter;
extern uint32_t  g_cpu_tx_sent_abs_gso_packets_counter;
extern uint32_t  g_cpu_tx_sent_abs_gso_bytes_counter;
#endif

#if defined(DSL_63138) || defined(DSL_63148)
//#define CC_RDD_CPU_SPDSVC_DEBUG
extern cpu_tx_skb_free_indexes_cache_t g_cpu_tx_skb_free_indexes_cache;
extern uint16_t  *g_free_skb_indexes_fifo_table_physical_address;
extern uint32_t  g_cpu_tx_pending_free_indexes_counter;
#if defined(CC_RDD_CPU_SPDSVC_DEBUG)
extern bdmf_sysb  g_spdsvc_setup_sysb_ptr;
#endif
#endif

#define __rdd_cpu_trace(fmt, args...)                                   \
    printk("TRACE: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);

#endif /* !FIRMWARE_INIT */

extern uint16_t  *g_free_skb_indexes_fifo_table;
extern uint32_t  *g_cpu_tx_skb_pointers_reference_array;
extern uint32_t  *g_cpu_tx_data_pointers_reference_array;
extern RDD_WAN_TX_POINTERS_TABLE_DTS                    *wan_tx_pointers_table_ptr;
extern BL_LILAC_RDD_LOCK_CRITICAL_SECTION_FP_IRQ_DTE    f_rdd_lock_irq;
extern BL_LILAC_RDD_UNLOCK_CRITICAL_SECTION_FP_IRQ_DTE  f_rdd_unlock_irq;
extern bdmf_fastlock                                    int_lock_irq;


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_rx_queue_discard_get                                             */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   return the number of packets that were dropped in this queue due to      */
/*   maximum size overflow.                                                   */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_queue_id - CPU-RX queue index (0-7)                                   */
/*   xo_number_of_packets - the number of dropped packets.                    */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_ILLEGAL - CPU-RX queue is illegal.     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_queue_discard_get ( BL_LILAC_RDD_CPU_RX_QUEUE_DTE  xi_queue_id,
                                                      uint16_t                       *xo_number_of_packets );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_reason_to_cpu_rx_queue                                           */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   cpu trap reason to cpu rx queue conversion.                              */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_cpu_reason - the reason for sending the packet to the CPU             */
/*   xi_queue_id - CPU-RX queue index (0-7)                                   */
/*   xi_direction - upstream or downstream                                    */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_ILLEGAL - CPU-RX queue is illegal.     */
/*     BL_LILAC_RDD_ERROR_CPU_RX_REASON_ILLEGAL - CPU-RX reason is illegal.   */
/*                                                                            */
/******************************************************************************/
#if defined(DSL_63138) || defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE rdd_cpu_reason_to_cpu_rx_queue ( rdpa_cpu_reason  xi_cpu_reason,
                                                        BL_LILAC_RDD_CPU_RX_QUEUE_DTE   xi_queue_id,
                                                        rdpa_traffic_dir                xi_direction,
                                                        uint32_t                        xi_table_index );
#else
BL_LILAC_RDD_ERROR_DTE rdd_cpu_reason_to_cpu_rx_queue ( rdpa_cpu_reason  xi_cpu_reason,
                                                        BL_LILAC_RDD_CPU_RX_QUEUE_DTE   xi_queue_id,
                                                        rdpa_traffic_dir                xi_direction );
#endif


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_reason_to_cpu_rx_meter                                           */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   cpu trap reason to cpu rx meter conversion.                              */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_cpu_reason    - the reason for sending the packet to the CPU          */
/*   xi_cpu_meter     - the CPU-meter id (0-15)                               */
/*   xi_direction     - upstream or downstream                                */
/*   xi_src_port_mask - egress ports mask. this mask is relevant for the      */
/*                      the following cpu reasons:                            */
/*                      rdpa_cpu_rx_reason_mcast,                             */
/*                      rdpa_cpu_rx_reason_bcast and                          */
/*                      rdpa_cpu_rx_reason_unknown_da.                        */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_RX_REASON_ILLEGAL - CPU-RX reason is illegal.   */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_reason_to_cpu_rx_meter ( rdpa_cpu_reason       xi_cpu_reason,
                                                        BL_LILAC_RDD_CPU_METER_DTE           xi_cpu_meter,
                                                        rdpa_traffic_dir                     xi_direction,
                                                        BL_LILAC_RDD_BRIDGE_PORT_VECTOR_DTE  xi_src_port_mask );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_rx_meter_config                                                  */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   CPU-RX config cpu meter                                                  */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This function returns the status of the operation                        */
/*                                                                            */
/* Registers :                                                                */
/*                                                                            */
/*   none                                                                     */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_cpu_meter - the CPU-meter id (0-15)                                   */
/*   xi_direction - upstream or downstream                                    */
/*   xi_average_rate - the average rate of the meter,                         */
/*                     specified in granularity of 100 packets/sec            */
/*   xi_burst_size   - max size of accumulated budget of the meter            */
/*                     specified in granularity of 100 packets/sec            */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   none                                                                     */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_meter_config ( BL_LILAC_RDD_CPU_METER_DTE  xi_cpu_meter,
                                                 uint16_t                    xi_average_rate,
                                                 uint16_t                    xi_burst_size,
                                                 rdpa_traffic_dir            xi_direction );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_rx_meter_drop_counter_get                                        */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*                                                                            */
/* Registers :                                                                */
/*                                                                            */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_meter_drop_counter_get ( BL_LILAC_RDD_CPU_METER_DTE  xi_cpu_meter,
                                                           rdpa_traffic_dir            xi_direction,
                                                           uint16_t                    *xo_drop_counter );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_tx_write_eth_packet                                              */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This packet is directed to the EMAC TX queue.                            */
/*   check if the CPU TX queue is not full, if not then a DDR buffer is       */
/*   allocated from the BPM, then the packet data is copied to the allocated  */
/*   buffer and a new packet descriptor is written to the CPU TX queue.       */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_packet_ptr - pointer to a DDR buffer that hold the packet data        */
/*   xi_packet_size - packet size in bytes                                    */
/*   xi_emac_id - EMAC port index (ETH0 - ETH4, PCI)                          */
/*   xi_wifi_ssid - service set id for PCI with wifi multiple ssid support    */
/*   xi_queue_id - ETH TX queue index                                         */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_TX_NOT_ALLOWED - the runner is not enabled.     */
/*     BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL - the CPU TX has no place for new */
/*                                            packets.                        */
/*     BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL - unable to allocate a DDR buffer to */
/*                                         from the BPM.                      */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_write_eth_packet ( uint8_t                    *xi_packet_ptr,
                                                     uint32_t                   xi_packet_size,
                                                     BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                                     uint8_t                    xi_wifi_ssid,
                                                     BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id );


#if !defined(RDD_BASIC)
/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_tx_write_gpon_packet                                             */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This packet is directed to the GPON TX queue.                            */
/*   check if the CPU TX queue is not full, if not then a DDR buffer is       */
/*   allocated from the BPM, then the packet data is copied to the allocated  */
/*   buffer and a new packet descriptor is written to the CPU TX queue.       */
/*                                                                            */
/* Input:                                                                     */
/*         RDD_CPU_TX_PARAMS                                                  */
/*   xi_packet_ptr - pointer to a DDR buffer that hold the packet data        */
/*   xi_packet_size - packet size in bytes                                    */
/*   xi_upstream_gem_flow - upstream gem flow index for CRC and GEM port      */
/*   xi_tcont - TCONT index                                                   */
/*   xi_rate_controller - rate controller index                               */
/*   xi_queue - GPON TX queue index                                           */
/*   xi_exclusive_packet - contorl traffic                                    */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL - the CPU TX has no place for new */
/*                                            packets.                        */
/*     BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL - unable to allocate a DDR buffer to */
/*                                         from the BPM.                      */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_write_gpon_packet ( uint8_t                              *xi_packet_ptr,
                                                      uint32_t                             xi_packet_size,
                                                      uint32_t                             xi_upstream_gem_flow,
                                                      RDD_WAN_CHANNEL_ID                   xi_tcont,
                                                      BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller,
                                                      BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue,
                                                      uint8_t                              xi_exclusive_packet );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_tx_send_packet_to_wan_bridge                                     */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This packet is sent to the upstream full route.                          */
/*   check if the CPU TX queue is not full, if not then a DDR buffer is       */
/*   allocated from the BPM, then the packet data is copied to the allocated  */
/*   buffer and a new packet descriptor is written to the CPU TX queue.       */
/*   The packet is parsed by the IH as a runner flow before concatentaing it  */
/*   to the upstream firmware.                                                */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_packet_ptr - pointer to a DDR buffer that hold the packet data        */
/*   xi_packet_size - packet size in bytes                                    */
/*   xi_src_bridge_port - the source port of packet actually it's origin.     */
/*   xi_wifi_ssid - service set id for PCI with wifi multiple ssid support    */
/*   xi_exclusive_packet - control traffic                                    */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL - the CPU TX has no place for new */
/*                                            packets.                        */
/*     BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL - unable to allocate a DDR buffer to */
/*                                         from the BPM.                      */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_wan_bridge ( uint8_t                       *xi_packet_ptr,
                                                              uint32_t                      xi_packet_size,
                                                              BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_src_bridge_port,
                                                              uint8_t                       xi_wifi_ssid,
                                                              uint8_t                       xi_exclusive_packet );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_tx_send_packet_to_lan_bridge                                     */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This packet is sent to the downstream full route.                        */
/*   check if the CPU TX queue is not full, if not then a DDR buffer is       */
/*   allocated from the BPM, then the packet data is copied to the allocated  */
/*   buffer and a new packet descriptor is written to the CPU TX queue.       */
/*   The packet is parsed by the IH as a runner flow before concatentaing it  */
/*   to the downstream firmware.                                              */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_packet_ptr - pointer to a DDR buffer that hold the packet data        */
/*   xi_packet_size - packet size in bytes                                    */
/*   xi_downstream_gem_flow - the GEM flow of packet actually it's origin.    */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL - the CPU TX has no place for new */
/*                                            packets.                        */
/*     BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL - unable to allocate a DDR buffer to */
/*                                         from the BPM.                      */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_lan_bridge ( uint8_t   *xi_packet_ptr,
                                                              uint32_t  xi_packet_size,
                                                              uint32_t  xi_downstream_gem_flow );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_tx_send_packet_to_wan_interworking                               */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This packet is sent to the upstream route for flow classification        */
/*   without filtering or forwarding desicion.                                */
/*   check if the CPU TX queue is not full, if not then a DDR buffer is       */
/*   allocated from the BPM, then the packet data is copied to the allocated  */
/*   buffer and a new packet descriptor is written to the CPU TX queue.       */
/*   The packet is parsed by the IH as a runner flow before concatentaing it  */
/*   to the upstream firmware.                                                */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_packet_ptr - pointer to a DDR buffer that hold the packet data        */
/*   xi_packet_size - packet size in bytes                                    */
/*   xi_src_bridge_port - the source port of packet actually it's origin.     */
/*   xi_wifi_ssid - service set id for PCI with wifi multiple ssid support    */
/*   xi_subnet_id - the subnet id is needed for 2nd flow classification.      */
/*   xi_exclusive_packet - control traffic                                    */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL - the CPU TX has no place for new */
/*                                            packets.                        */
/*     BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL - unable to allocate a DDR buffer to */
/*                                         from the BPM.                      */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_wan_interworking ( uint8_t                       *xi_packet_ptr,
                                                                    uint32_t                      xi_packet_size,
                                                                    BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_src_bridge_port,
                                                                    uint8_t                       xi_wifi_ssid,
                                                                    BL_LILAC_RDD_SUBNET_ID_DTE    xi_subnet_id,
                                                                    uint8_t                       xi_exclusive_packet );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   bl_lilac_rdd_cpu_tx_send_packet_to_lan_interworking                      */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This packet is sent to the downstream route for flow classification      */
/*   without filtering or forwarding desicion.                                */
/*   check if the CPU TX queue is not full, if not then a DDR buffer is       */
/*   allocated from the BPM, then the packet data is copied to the allocated  */
/*   buffer and a new packet descriptor is written to the CPU TX queue.       */
/*   The packet is parsed by the IH as a runner flow before concatentaing it  */
/*   to the downstream firmware.                                              */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_packet_ptr - pointer to a DDR buffer that hold the packet data        */
/*   xi_packet_size - packet size in bytes                                    */
/*   xi_downstream_gem_flow - the GEM flow of packet actually it's origin.    */
/*   xi_emac_id - the egress EMAC of the packet.                              */
/*   xi_wifi_ssid - service set id for PCI with wifi multiple ssid support    */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL - the CPU TX has no place for new */
/*                                            packets.                        */
/*     BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL - unable to allocate a DDR buffer to */
/*                                         from the BPM.                      */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_lan_interworking ( uint8_t                   *xi_packet_ptr,
                                                                    uint32_t                  xi_packet_size,
                                                                    uint32_t                  xi_downstream_gem_flow,
                                                                    BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id,
                                                                    uint8_t                   xi_wifi_ssid );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   bl_lilac_rdd_cpu_tx_get_released_skb_counter                             */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   Returns number of SKB released by RDD                                    */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   xo_skb_counter - released SKB counter         .                          */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE bl_lilac_rdd_cpu_tx_get_released_skb_counter ( uint32_t  *xo_skb_counter );


#endif /* !defined(RDD_BASIC)*/


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_flow_pm_counters_get                                                 */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Performance Monitoring - read bridge port statistics counters            */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This function returns the status of the operation                        */
/*                                                                            */
/* Registers :                                                                */
/*                                                                            */
/*   none                                                                     */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_flow_id - the ingress flow id to be read                              */
/*   xi_clear_counters - accumulate or clear counter                          */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   xo_pm_counters - a structure that hold all the counters of a single port */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_flow_pm_counters_get ( uint32_t                                xi_flow_id,
                                                  BL_LILAC_RDD_FLOW_PM_COUNTERS_TYPE_DTE  xi_flow_pm_counters_type,
                                                  bdmf_boolean                            xi_clear_counters,
                                                  BL_LILAC_RDD_FLOW_PM_COUNTERS_DTE       *xo_pm_counters );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_service_queue_pm_counters_get                                        */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   read service queue statistics counters                                   */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This function returns the status of the operation                        */
/*                                                                            */
/* Registers :                                                                */
/*                                                                            */
/*   none                                                                     */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_service_queue - the service queue to be read                          */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   xo_pm_counters - a structure that hold all the counters of a single port */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_service_queue_pm_counters_get ( uint32_t                           xi_service_queue,
                                                          RDD_SERVICE_QUEUE_PM_COUNTERS_DTE  *xo_pm_counters );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_bridge_port_pm_counters_get                                          */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Performance Monitoring - read bridge port statistics counters            */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This function returns the status of the operation                        */
/*                                                                            */
/* Registers :                                                                */
/*                                                                            */
/*   none                                                                     */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_bridge_port - the bridge port to be read                              */
/*   xi_clear_counters - accumulate or clear counter                          */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   xo_pm_counters - a structure that hold all the counters of a single port */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_bridge_port_pm_counters_get ( BL_LILAC_RDD_BRIDGE_PORT_DTE              xi_bridge_port,
                                                         bdmf_boolean                              xi_clear_counters,
                                                         BL_LILAC_RDD_BRIDGE_PORT_PM_COUNTERS_DTE  *xo_pm_counters );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_crc_error_counter_get                                                */
/*                                                                            */
/* Description:                                                               */
/*                                                                            */
/*   The function returns the number of packets dropped because of CRC error  */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_bridge_port (IPTV bridge port returns CRC error counter for IPTV      */
/*   traffic; any other bridge port returns returns CRC error counter for all */
/*   non IPTV traffic.                                                        */
/*   xi_clear_counters - accumulate or clear counter                          */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   xo_crc_counter                                                           */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_crc_error_counter_get ( BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_bridge_port,
                                                   bdmf_boolean                  xi_clear_counters,
                                                   uint16_t                      *xo_crc_counter );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_subnet_counters_get                                                  */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - read and clear of subnet counters                      */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This function read all the subnet counters per subnet id, the counters   */
/*   are cleared after reading. Four counter are supported RX, TX, RX & TX    */
/*   dropped.                                                                 */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_subnet_id - Subnet index                                              */
/*   xo_subnet_counters - structure to hold the counters                      */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_subnet_counters_get ( BL_LILAC_RDD_SUBNET_ID_DTE           xi_subnet_id,
                                                 BL_LILAC_RDD_SUBNET_PM_COUNTERS_DTE  *xo_subnet_counters );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_various_counters_get                                                 */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - read and clear of various counters                     */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This function reads various counters. The counters are                   */
/*   cleared after reading. Selection of the counters to get                  */
/*   can be done using a mask ( bitmap ).                                     */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_direction - direction (upstream or downstream)                        */
/*   xi_various_counters_mask - counters selection                            */
/*   xi_clear_counters - accumulate or clear counter                          */
/*   xo_various_counters - structure to hold the counters                     */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*   BL_LILAC_RDD_OK - No error                                               */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_various_counters_get ( rdpa_traffic_dir                   xi_direction,
                                                  uint32_t                           xi_various_counters_mask,
                                                  bdmf_boolean                       xi_clear_counters,
                                                  BL_LILAC_RDD_VARIOUS_COUNTERS_DTE  *xo_various_counters );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_g9991_counters_get                                                   */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - read and clear of G9991 counters                       */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   This function read all the G9991 counters, the counters                  */
/*   are cleared after reading.                                               */
/*   dropped.                                                                 */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xo_g9991_counters - structure to hold the counters                       */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*   BL_LILAC_RDD_OK - No error                                               */
/*                                                                            */
/******************************************************************************/
#ifdef G9991
BL_LILAC_RDD_ERROR_DTE rdd_g9991_counters_get ( RDD_G9991_PM_COUNTERS_DTE *xo_g9991_counters);
#endif

/* local */
BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_initialize ( void );

BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_initialize ( void );

#if defined(DSL_63138) || defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE rdd_spdsvc_initialize( void );

BL_LILAC_RDD_ERROR_DTE rdd_spdsvc_config( uint32_t xi_kbps,
                                          uint32_t xi_mbs,
                                          uint32_t xi_copies,
                                          uint32_t xi_total_length );
BL_LILAC_RDD_ERROR_DTE rdd_spdsvc_get_tx_result( uint8_t *xo_running_p,
                                                 uint32_t *xo_tx_packets_p,
                                                 uint32_t *xo_tx_discards_p );
#endif

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_ring_init                                                            */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   configures CPU-RX ring, set the number packets that are pending to be    */
/*   read by the CPU for that queue, and the interrupt that will be set       */
/*   during packet enterance to the queue, the default interrupt is the queue */
/*   number.                                                                  */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_ring_id - CPU-RX ring index (0-7)                                     */
/*   xi_number_of_entries - queue maximum size in packets.                    */
/*   xi_ring_address - address of allocated ring                              */
/*   xi_size_of_entry - size of cpu-rx descriptor                             */
/*   xi_interrupt_id  - interrupt id to set                                   */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_ring_init ( uint32_t  xi_ring_id,
                                       uint32_t  *xi_ring_address,
                                       uint32_t  xi_number_of_entries,
                                       uint32_t  xi_size_of_entry,
                                       uint32_t  xi_interrupt_id );


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_ring_destroy                                                         */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   destroyes CPU-RX ring                                                    */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_ring_id - CPU-RX ring index (0-7)                                     */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*                                                                            */
/******************************************************************************/
BL_LILAC_RDD_ERROR_DTE rdd_ring_destroy ( uint32_t  xi_ring_id );

#if !defined(RDD_BASIC)
/* RDD Inline Functions */

static inline int32_t f_rdd_bridge_port_to_class_id ( BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_bridge_port )
{
    switch ( xi_bridge_port )
    {
    case BL_LILAC_RDD_WAN0_BRIDGE_PORT: // DSL
    case BL_LILAC_RDD_WAN1_BRIDGE_PORT: // DSL

        return ( LILAC_RDD_IH_WAN_BRIDGE_LOW_CLASS );

    case BL_LILAC_RDD_LAN0_BRIDGE_PORT:

        return ( LILAC_RDD_IH_LAN_EMAC0_CLASS );

    case BL_LILAC_RDD_LAN1_BRIDGE_PORT:

        return ( LILAC_RDD_IH_LAN_EMAC1_CLASS );

    case BL_LILAC_RDD_LAN2_BRIDGE_PORT:

        return ( LILAC_RDD_IH_LAN_EMAC2_CLASS );

    case BL_LILAC_RDD_LAN3_BRIDGE_PORT:

        return ( LILAC_RDD_IH_LAN_EMAC3_CLASS );

    case BL_LILAC_RDD_LAN4_BRIDGE_PORT:

        return ( LILAC_RDD_IH_LAN_EMAC4_CLASS );

    case BL_LILAC_RDD_PCI_BRIDGE_PORT:

        return ( LILAC_RDD_IH_PCI_CLASS );

    default:

        return ( 0 );
    }

    return ( 0 );
}

#endif /* !defined(RDD_BASIC) */


static inline void f_rdd_get_tx_descriptor_free_count(LILAC_RDD_RUNNER_INDEX_DTS runner_index, RDD_CPU_TX_DESCRIPTOR_DTS *cpu_tx_descriptor_ptr )
{
    RDD_CPU_TX_DESCRIPTOR_QUEUE_TAIL_TABLE_DTS *cpu_tx_queue_read_table_ptr;
    uint8_t                                    *cpu_tx_queue_read_entry_ptr;
    uint8_t                                     cpu_tx_queue_read_idx;

    cpu_tx_queue_read_table_ptr = RDD_CPU_TX_DESCRIPTOR_QUEUE_TAIL_TABLE_PTR();

    cpu_tx_queue_read_entry_ptr = (uint8_t *)&cpu_tx_queue_read_table_ptr->entry[ runner_index ];

    MREAD_8( cpu_tx_queue_read_entry_ptr, cpu_tx_queue_read_idx );

    {
        uint8_t read = cpu_tx_queue_read_idx;
        uint8_t write = ( g_cpu_tx_queue_write_ptr[ runner_index ] & ~CPU_TX_DESCRIPTOR_ADDRESS_MASK ) / LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
        uint32_t free;

        /* if(runner_index == FAST_RUNNER_A) */
        /* { */
        /*     printk("%s,%u: read %u, write %u, cpu_tx_queue_read_entry_ptr %p\n", */
        /*            __FUNCTION__, __LINE__, read, write, cpu_tx_queue_read_entry_ptr); */
        /* } */

        if(write >= read)
        {
            free = LILAC_RDD_CPU_TX_QUEUE_SIZE - write + read - 1;
        }
        else 
        {
            /* write wraparound */
            free = read - write - 1;
        }

        g_cpu_tx_queue_free_counter[ runner_index ] = free;
    }
}

#if defined(DSL_63138) || defined(DSL_63148)
#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
#define RDD_CPU_TX_MAX_PENDING_FREE_INDEXES  64

static inline uint32_t __skb_free_indexes_cache_nbr_of_entries(cpu_tx_skb_free_indexes_cache_t *cache_p)
{
    return (cache_p->count);
}

#define __is_skb_free_indexes_cache_not_empty( _cache_p )       \
    __skb_free_indexes_cache_nbr_of_entries( (_cache_p) )

static inline uint32_t __skb_free_indexes_cache_read(cpu_tx_skb_free_indexes_cache_t *cache_p)
{
    uint32_t data;

#if 0
    if(cache_p->count == 0)
    {
        printk("\n\tBUG: Queue Empty\n\n");

        return 0;
    }
#endif

    data = cache_p->data[cache_p->read];

//    printk("READ : count %u, write %u, read %u, data %u\n", cache_p->count, cache_p->write, cache_p->read, data);

    if(cache_p->read == (g_cpu_tx_abs_packet_limit-1))
    {
        cache_p->read = 0;
    }
    else
    {
        cache_p->read++;
    }
    
    cache_p->count--;

    return data;
}

static inline void __skb_free_indexes_cache_write(cpu_tx_skb_free_indexes_cache_t *cache_p, uint32_t data)
{
#if 0
    if(cache_p->count == g_cpu_tx_abs_packet_limit)
    {
        printk("\n\tBUG: Queue Full\n\n");

        return;
    }
#endif

//    printk("WRITE: count %u, write %u, read %u, data %u\n", cache_p->count, cache_p->write, cache_p->read, data);

    cache_p->data[cache_p->write] = data;

    if(cache_p->write == (g_cpu_tx_abs_packet_limit-1))
    {
        cache_p->write = 0;
    }
    else
    {
        cache_p->write++;
    }
    
    cache_p->count++;
}

static inline void f_rdd_initialize_skb_free_indexes_cache(void)
{
    uint32_t free_index = 0;

    for(free_index=0; free_index<g_cpu_tx_abs_packet_limit; ++free_index)
    {
        __skb_free_indexes_cache_write(&g_cpu_tx_skb_free_indexes_cache, free_index);
    }
}

static inline BL_LILAC_RDD_ERROR_DTE f_rdd_release_free_skb ( uint32_t max_pending_free_indexes )
{
#if !(defined(FIRMWARE_INIT) || defined(BDMF_SYSTEM_SIM)) /* DSL changed expression */

    /* BULK FREE: In order to reduce the number of Runner SRAM reads and increase CPU
       performance, SKB recycling is deferred until the number of buffers pending
       transmission is higher than the provided max_pending_free_indexes */

    if ( unlikely( g_cpu_tx_pending_free_indexes_counter > max_pending_free_indexes ) )
    {
        uint32_t prev_cpu_tx_pending_free_indexes_counter = g_cpu_tx_pending_free_indexes_counter;
        uint32_t *free_indexes_fifo_tail_ptr;
        uint32_t loaded = 0;
        uint32_t tx_skb_free_indexes_tail_address;
        uint64_t free_indexes_64 = 0;
	uint16_t *free_indexes_array = (uint16_t *)&free_indexes_64;

        /* read free indexes fifo tail ptr */
        free_indexes_fifo_tail_ptr = ( uint32_t * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + FREE_SKB_INDEXES_DDR_FIFO_TAIL_ADDRESS );

        MREAD_32( free_indexes_fifo_tail_ptr, tx_skb_free_indexes_tail_address );
        /*The free_indexes_64_addr read below is an uncached normal memory read, which on ARM can be 
         *speculatively issued out of order before the above fifo tail pointer read.
         *This creates a race condition where ARM can read from the tail of the FIFO before the Runner has
         *completed writing to it.
         *To guarantee correct ordering, we place a read barrier after the tail pointer read.
         */
        rmb();

        tx_skb_free_indexes_tail_address = (tx_skb_free_indexes_tail_address -
                (uint32_t)g_free_skb_indexes_fifo_table_physical_address) >> 1;

        while ( g_cpu_tx_skb_free_indexes_release_ptr != tx_skb_free_indexes_tail_address )
        {
            uint32_t release_ptr_index;
            uint16_t free_index;

            release_ptr_index = g_cpu_tx_skb_free_indexes_release_ptr & (uint32_t)(0x3);

            if (!release_ptr_index || !loaded)
            {
                volatile uint64_t *free_indexes_64_addr = (volatile uint64_t *)(&g_free_skb_indexes_fifo_table[
                        ( g_cpu_tx_skb_free_indexes_release_ptr & (uint32_t)(~0x3) ) ]);
		free_indexes_64 = (uint64_t)*free_indexes_64_addr;
                loaded = 1;
            }

	    free_index = (uint16_t)swap2bytes(free_indexes_array[release_ptr_index]) & ~LILAC_RDD_CPU_TX_SKB_INDEX_OWNERSHIP_BIT_MASK;

            bdmf_sysb_free ( ( bdmf_sysb )g_cpu_tx_skb_pointers_reference_array[ free_index ] );

//            printk("RDD FREE: free_index %u, skb %p\n", free_index, ( bdmf_sysb )g_cpu_tx_skb_pointers_reference_array[ free_index ] );

#if defined(CC_RDD_CPU_SPDSVC_DEBUG)
            if( ( bdmf_sysb )g_cpu_tx_skb_pointers_reference_array[ free_index ] == g_spdsvc_setup_sysb_ptr )
            {
                __rdd_cpu_trace("Speed Service Generator: OFF (%p, 0x%x)", g_spdsvc_setup_sysb_ptr, free_index);
                g_spdsvc_setup_sysb_ptr = NULL;
            }
#endif
            __skb_free_indexes_cache_write(&g_cpu_tx_skb_free_indexes_cache, free_index);

            g_cpu_tx_data_pointers_reference_array[ free_index ] = 0;

            g_cpu_tx_released_skb_counter++;
            g_cpu_tx_skb_free_indexes_release_ptr = ( g_cpu_tx_skb_free_indexes_release_ptr + 1 ) % g_cpu_tx_abs_packet_limit;
            g_cpu_tx_pending_free_indexes_counter--;
#if !defined(DSL_63138) && !defined(DSL_63148)
            g_cpu_tx_skb_free_indexes_counter++;
#endif
        }

        if( prev_cpu_tx_pending_free_indexes_counter != g_cpu_tx_pending_free_indexes_counter )
        {
            /* We have freed one or more SKBs: make sure the updates of the data pointers table reached the DDR */
            wmb();
        }
    }
#endif
    return ( BL_LILAC_RDD_OK );
}

static inline bdmf_sysb rdd_cpu_return_free_index(uint16_t free_index)
{
    unsigned long flags;
    bdmf_sysb sysb;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    sysb = ( bdmf_sysb )( g_cpu_tx_skb_pointers_reference_array[ free_index ] );

    __skb_free_indexes_cache_write(&g_cpu_tx_skb_free_indexes_cache, free_index);

    g_cpu_tx_data_pointers_reference_array[ free_index ] = 0;

    wmb();

    g_cpu_tx_released_skb_counter++;
    g_cpu_tx_pending_free_indexes_counter--;

    f_rdd_unlock_irq ( &int_lock_irq, flags );

//    printk("SKB GET: free_index %u, skb %p\n", free_index, sysb);

    return sysb;
}
#endif /* !FIRMWARE_INIT && !RDD_BASIC */
#endif /* DSL */

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   rdd_cpu_tx                                                               */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   Firmware Driver - CPU interface.                                         */
/*                                                                            */
/* Abstract:                                                                  */
/*                                                                            */
/*   all purpose function for cpu tx packet transmission to the FW.           */
/*   supports:                                                                */
/*   * both pre allocated BPM or absolute address                             */
/*   * both upstream and dowstream transmission                               */
/*   * transmission to bridge, interworking or Egress enqueue                 */
/*     phase in the FW flow                                                   */
/*   * 1588 packet transmission in case of ds bpm Egress enqueue              */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/*   xi_args - pointer the rdd_cpu_tx_args_t structure that includes all      */
/*             arguments                                                      */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/*   BL_LILAC_RDD_ERROR_DTE - Return status                                   */
/*     BL_LILAC_RDD_OK - No error                                             */
/*     BL_LILAC_RDD_ERROR_CPU_TX_NOT_ALLOWED - the runner is not enabled.     */
/*     BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL - the CPU TX has no place for new */
/*                                            packets.                        */
/*     BL_LILAC_RDD_ERROR_NO_FREE_SKB  - no free place in SKB indexes array   */
/*     BL_LILAC_RDD_ERROR_1588_TX - 1588 packet transmission failed           */
/*     SW should drop packet and free SKB or use standard send procedure      */
/*                                      .                                     */
/*                                                                            */
/******************************************************************************/
#if defined(CONFIG_DHD_RUNNER)
extern bdmf_boolean is_dhd_enabled[];
#endif
static inline BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx ( rdd_cpu_tx_args_t  *xi_args,
                                                  void               *buffer,
                                                  uint32_t           skb_pkt_length)
{
#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
    RUNNER_REGS_CFG_CPU_WAKEUP     runner_cpu_wakeup_register;
    RDD_CPU_TX_DESCRIPTOR_DTS      *cpu_tx_descriptor_ptr;
#if !defined(DSL_63138) && !defined(DSL_63148)
    RDD_FOUR_BYTES_DTS             *cpu_tx_abs_descriptor_ptr;
    uint8_t                        free_index = 0xFF;
#else
    uint16_t                       free_index = 0xFFFF;
#endif
    unsigned long                  flags;
    RDD_WAN_TX_POINTERS_ENTRY_DTS  *wan_tx_pointers_entry_ptr;
    LILAC_RDD_RUNNER_INDEX_DTS     cpu_tx_descriptor_queue_table_idx;
    uint8_t                        *thread_result_ptr;
    uint32_t                       descriptor_number;
    uint32_t                       cpu_tx_descriptor = 0;
    uint32_t                       src_port = xi_args->direction.ds.emac_id;

    if ( ( rdpa_dir_ds == xi_args->traffic_dir ) && ( xi_args->direction.ds.en_1588 ) )
    {
        thread_result_ptr = ( uint8_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + IP_SYNC_1588_TX_ENQUEUE_RESULT_ADDRESS );

        MWRITE_8( thread_result_ptr, RDD_1588_TX_THREAD_RETURN_NO_RESULT );
    }

    f_rdd_lock_irq ( &int_lock_irq, &flags );

#if defined(DSL_63138) || defined(DSL_63148)
    f_rdd_release_free_skb( RDD_CPU_TX_MAX_PENDING_FREE_INDEXES );
#endif

#if !defined(DSL_63138) && !defined(DSL_63148)
    if ( ( rdd_host_buffer == xi_args->buffer_type ) && ( g_cpu_tx_skb_free_indexes_counter == 0 ) )
    {
        g_cpu_tx_no_free_skb_counter++;
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return BL_LILAC_RDD_ERROR_NO_FREE_SKB;
    }
#endif

    if ( rdpa_dir_ds == xi_args->traffic_dir )
    {
        cpu_tx_descriptor_queue_table_idx = ( rdd_cpu_tx_mode_egress_enq  == xi_args->mode ) ? PICO_RUNNER_A : FAST_RUNNER_A;
    }
    else /* if ( rdpa_dir_us == xi_args->traffic_dir ) */
    {
        cpu_tx_descriptor_queue_table_idx = ( rdd_cpu_tx_mode_egress_enq == xi_args->mode ) ? FAST_RUNNER_B : PICO_RUNNER_B;
    }

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( ( rdpa_dir_ds == xi_args->traffic_dir ? RUNNER_PRIVATE_0_OFFSET : RUNNER_PRIVATE_1_OFFSET ) ) + ( g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] ) );

    if( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
    {
        f_rdd_get_tx_descriptor_free_count ( cpu_tx_descriptor_queue_table_idx, cpu_tx_descriptor_ptr );

        if( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
        {
            g_cpu_tx_released_skb_counter++;

            f_rdd_unlock_irq ( &int_lock_irq, flags );
            return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
        }
    }

    /* get skb pointer list free index (ABS only) */
    if ( rdd_host_buffer == xi_args->buffer_type )
    {
        /* get skb pointer list free index */
#if defined(DSL_63138) || defined(DSL_63148)
        if (__is_skb_free_indexes_cache_not_empty(&g_cpu_tx_skb_free_indexes_cache))
        {
            free_index = __skb_free_indexes_cache_read(&g_cpu_tx_skb_free_indexes_cache);

            g_cpu_tx_pending_free_indexes_counter++;
        }
        else
        {
            g_cpu_tx_no_free_skb_counter++;

            f_rdd_unlock_irq ( &int_lock_irq, flags );
            return ( BL_LILAC_RDD_ERROR_NO_FREE_SKB );
        }
#else
        free_index = g_free_skb_indexes_fifo_table[ g_cpu_tx_skb_free_indexes_head_ptr ];

        g_cpu_tx_skb_free_indexes_head_ptr = ( g_cpu_tx_skb_free_indexes_head_ptr + 1 ) % g_cpu_tx_abs_packet_limit;

        g_cpu_tx_skb_free_indexes_counter--;
#endif

        /* save buffer ptr and data ptr */
        g_cpu_tx_skb_pointers_reference_array[ free_index ] = ( uint32_t )buffer;

#if defined(DSL_63138) || defined(DSL_63148)
        g_cpu_tx_data_pointers_reference_array[ free_index ] = ( uint32_t )swap4bytes( VIRT_TO_PHYS( bdmf_sysb_data( buffer ) ) );
#else
        g_cpu_tx_data_pointers_reference_array[ free_index ] = ( uint32_t )bdmf_sysb_data( buffer ) & 0x1FFFFFFF;
#endif

        g_cpu_tx_sent_abs_packets_counter++;
    }

    /* write CPU-TX descriptor Word 1: bridge/interwork/Egress enqueue common */
    RDD_CPU_TX_DESCRIPTOR_BPM_BUFFER_NUMBER_L_WRITE ( cpu_tx_descriptor, rdd_runner_buffer == xi_args->buffer_type ? ( ( bdmf_pbuf_t * )buffer )->bpm_bn : ( free_index & 0x3FFF ) );

    /* Egress enqueue case */
    if ( rdd_cpu_tx_mode_egress_enq == xi_args->mode )
    {
        if ( rdpa_dir_ds == xi_args->traffic_dir )
        {
            if ( BL_LILAC_RDD_EMAC_ID_PCI == xi_args->direction.ds.emac_id )
            {
                src_port = PCI_0_SRC_PORT;
#if defined(CONFIG_DHD_RUNNER)
                if (is_dhd_enabled[xi_args->wan_flow >> 14])
                {
                    RDD_CPU_TX_DHD_DESCRIPTOR_FLOW_RING_ID_L_WRITE( cpu_tx_descriptor, xi_args->wan_flow & ~0xc000 );
                    RDD_CPU_TX_DHD_DESCRIPTOR_RADIO_IDX_L_WRITE( cpu_tx_descriptor, xi_args->wan_flow >> 14);
                    RDD_CPU_TX_DHD_DESCRIPTOR_SSID_L_WRITE ( cpu_tx_descriptor, xi_args->wifi_ssid );
                    RDD_CPU_TX_DHD_DESCRIPTOR_SSID_MULTICAST_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_FALSE ); /* expandable, it is 0 anyway */
                }
                else
#endif
                {
                    RDD_CPU_TX_DESCRIPTOR_CORE_PAYLOAD_OFFSET_L_WRITE ( cpu_tx_descriptor, rdd_runner_buffer == xi_args->buffer_type ? ( ( ( ( bdmf_pbuf_t * )buffer )->offset ) / 2 ) : ( ( g_ddr_headroom_size + BL_LILAC_RDD_DDR_PACKET_PAYLOAD_OFFSET ) / 2 ) );
                    RDD_CPU_TX_DESCRIPTOR_DS_PICO_WIFI_SSID_L_WRITE ( cpu_tx_descriptor, xi_args->wifi_ssid );
                    RDD_CPU_TX_DESCRIPTOR_DS_PICO_WIFI_SSID_MULTICAST_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_FALSE ); /* expandable, it is 0 anyway */
                }
            }
            else
            {
                RDD_CPU_TX_DESCRIPTOR_CORE_PAYLOAD_OFFSET_L_WRITE ( cpu_tx_descriptor, rdd_runner_buffer == xi_args->buffer_type ? ( ( ( ( bdmf_pbuf_t * )buffer )->offset ) / 2 ) : ( ( g_ddr_headroom_size + BL_LILAC_RDD_DDR_PACKET_PAYLOAD_OFFSET ) / 2 ) );
                RDD_CPU_TX_DESCRIPTOR_DS_PICO_EN_1588_L_WRITE ( cpu_tx_descriptor, xi_args->direction.ds.en_1588 );
            }
        }
        else /* if ( rdpa_dir_us == xi_args->traffic_dir ) */
        {
            RDD_CPU_TX_DESCRIPTOR_CORE_PAYLOAD_OFFSET_L_WRITE ( cpu_tx_descriptor, rdd_runner_buffer == xi_args->buffer_type ? ( ( ( ( bdmf_pbuf_t * )buffer )->offset ) / 2 ) : ( ( g_ddr_headroom_size + BL_LILAC_RDD_DDR_PACKET_PAYLOAD_OFFSET ) / 2 ) );
            wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_args->direction.us.wan_channel ][ xi_args->direction.us.rate_controller ][ xi_args->direction.us.queue ] );
            RDD_CPU_TX_DESCRIPTOR_US_FAST_TX_QUEUE_L_WRITE ( cpu_tx_descriptor, ( ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - WAN_TX_QUEUES_TABLE_ADDRESS ) / sizeof( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS ) ) );
        }
    }
    /* bridge/interwork case */
    else
    {
#if defined(DSL_63138) || defined(DSL_63148)
        RDD_CPU_TX_DESCRIPTOR_CORE_IH_CLASS_L_WRITE ( cpu_tx_descriptor, f_rdd_bridge_port_to_class_id ( rdpa_dir_ds == xi_args->traffic_dir ? BL_LILAC_RDD_WAN0_BRIDGE_PORT : xi_args->direction.us.src_bridge_port ) );
#else
        RDD_CPU_TX_DESCRIPTOR_CORE_IH_CLASS_L_WRITE ( cpu_tx_descriptor, f_rdd_bridge_port_to_class_id ( rdpa_dir_ds == xi_args->traffic_dir ? BL_LILAC_RDD_WAN_BRIDGE_PORT : xi_args->direction.us.src_bridge_port ) );
#endif
        RDD_CPU_TX_DESCRIPTOR_CORE_SSID_L_WRITE ( cpu_tx_descriptor, ( rdd_cpu_tx_mode_interworking == xi_args->mode ) || ( rdpa_dir_us == xi_args->traffic_dir ) ? xi_args->wifi_ssid : 0 );
        RDD_CPU_TX_DESCRIPTOR_CORE_ABS_FLAG_L_WRITE ( cpu_tx_descriptor, ( rdd_host_buffer == xi_args->buffer_type ) ? LILAC_RDD_TRUE : LILAC_RDD_FALSE );
    }

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = 0;

    /* bridge/interwork/Egress enqueue common part */
#if defined(CONFIG_DHD_RUNNER)
    if ((is_dhd_enabled[xi_args->wan_flow >> 14]) && (rdd_cpu_tx_mode_egress_enq == xi_args->mode) && (BL_LILAC_RDD_EMAC_ID_PCI == xi_args->direction.ds.emac_id) && (rdpa_dir_ds == xi_args->traffic_dir))
        RDD_CPU_TX_DESCRIPTOR_CORE_PACKET_LENGTH_L_WRITE ( cpu_tx_descriptor, ( rdd_host_buffer == xi_args->buffer_type ) ? skb_pkt_length  : ( ( bdmf_pbuf_t * )buffer )->length );
    else
#endif
        RDD_CPU_TX_DESCRIPTOR_CORE_PACKET_LENGTH_L_WRITE ( cpu_tx_descriptor, ( rdd_host_buffer == xi_args->buffer_type ) ? skb_pkt_length + 4 : ( ( bdmf_pbuf_t * )buffer )->length + 4 );
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );

    if ( ( rdpa_dir_ds == xi_args->traffic_dir ) && ( rdd_cpu_tx_mode_full != xi_args->mode ) )
    {
        RDD_CPU_TX_DESCRIPTOR_CORE_EMAC_L_WRITE ( cpu_tx_descriptor, src_port );
    }

    /* bridge/interwork case */
    if ( rdd_cpu_tx_mode_egress_enq != xi_args->mode )
    {
        RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, ( rdd_cpu_tx_mode_full == xi_args->mode ) ? LILAC_RDD_CPU_TX_COMMAND_BRIDGE_PACKET : LILAC_RDD_CPU_TX_COMMAND_INTERWORKING_PACKET );

        if ( xi_args->traffic_dir == rdpa_dir_ds )
        {
            RDD_CPU_TX_DESCRIPTOR_DS_FAST_DOWNSTREAM_WAN_FLOW_L_WRITE ( cpu_tx_descriptor, xi_args->wan_flow );
        }
        else
        {
            RDD_CPU_TX_DESCRIPTOR_CORE_SRC_BRIDGE_PORT_L_WRITE ( cpu_tx_descriptor, xi_args->direction.us.src_bridge_port );
        }
    }

    /* Egress enqueue case */
    else
    {
#if defined(DSL_63138) || defined(DSL_63148)
        if(xi_args->is_spdsvc_setup_packet)
        {
#if defined(CC_RDD_CPU_SPDSVC_DEBUG)
            g_spdsvc_setup_sysb_ptr = buffer;
            __rdd_cpu_trace("Speed Service Generator ON (%p, 0x%x)", g_spdsvc_setup_sysb_ptr, free_index);
#endif
            RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_CPU_TX_COMMAND_SPDSVC_PACKET );
        }
        else
#endif
        {
            RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, rdd_host_buffer == xi_args->buffer_type ? LILAC_RDD_CPU_TX_COMMAND_ABSOLUTE_ADDRESS_PACKET : LILAC_RDD_CPU_TX_COMMAND_EGRESS_PORT_PACKET );
        }

        RDD_CPU_TX_DESCRIPTOR_CORE_TX_QUEUE_L_WRITE ( cpu_tx_descriptor, rdpa_dir_ds == xi_args->traffic_dir ? xi_args->direction.ds.queue_id : 0 );

        if ( rdd_runner_buffer == xi_args->buffer_type )
        {
            RDD_CPU_TX_DESCRIPTOR_CORE_SRC_BRIDGE_PORT_L_WRITE ( cpu_tx_descriptor, ( xi_args->buffer_type == rdd_runner_buffer ) ? SPARE_0_SRC_PORT : 0 );
        }

        if ( rdpa_dir_us == xi_args->traffic_dir )
        {
            RDD_CPU_TX_DESCRIPTOR_US_FAST_UPSTREAM_GEM_FLOW_L_WRITE ( cpu_tx_descriptor, xi_args->wan_flow );
        }
    }

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    descriptor_number = ( g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] & LILAC_RDD_CPU_TX_DESCRIPTOR_NUMBER_MASK ) / LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;
    g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ]--;

#if !defined(DSL_63138) && !defined(DSL_63148)
    /* in case of absolute address, write the data pointer into 2nd descriptor to avoid reading DDR in FW */
    if ( ( rdd_host_buffer == xi_args->buffer_type ) )
    {
        cpu_tx_abs_descriptor_ptr = ( RDD_FOUR_BYTES_DTS * )(DEVICE_ADDRESS( ( rdpa_dir_ds == xi_args->traffic_dir ? RUNNER_PRIVATE_0_OFFSET : RUNNER_PRIVATE_1_OFFSET ) )
            + g_cpu_tx_queue_abs_data_ptr_write_ptr[ cpu_tx_descriptor_queue_table_idx ] + ( descriptor_number * LILAC_RDD_CPU_TX_ABS_DATA_PTR_DESCRIPTOR_SIZE ) );

        MWRITE_32( ( uint8_t * )cpu_tx_abs_descriptor_ptr, g_cpu_tx_data_pointers_reference_array[ free_index ] );
    }
#endif

    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt = (cpu_tx_descriptor_queue_table_idx <= FAST_RUNNER_B ? CPU_TX_FAST_THREAD_NUMBER : CPU_TX_PICO_THREAD_NUMBER) >> 5;
    runner_cpu_wakeup_register.thread_num = ( cpu_tx_descriptor_queue_table_idx <= FAST_RUNNER_B ? CPU_TX_FAST_THREAD_NUMBER : CPU_TX_PICO_THREAD_NUMBER) & 0x1f;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    if ( rdpa_dir_ds == xi_args->traffic_dir )
    {
        RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
    }
    else /* if ( rdpa_dir_us == xi_args->traffic_dir ) */
    {
        RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
    }

#if !defined(DSL_63138) && !defined(DSL_63148)
    /* release buffers with ownership bit on starting from SW release pointer */
    while ( g_free_skb_indexes_fifo_table[ g_cpu_tx_skb_free_indexes_release_ptr ] & LILAC_RDD_CPU_TX_SKB_INDEX_OWNERSHIP_BIT_MASK )
    {
        /* clear ownership bit */
        g_free_skb_indexes_fifo_table[ g_cpu_tx_skb_free_indexes_release_ptr ] &= LILAC_RDD_CPU_TX_SKB_INDEX_MASK;

        /* free pointer */
        bdmf_sysb_free ( ( bdmf_sysb )g_cpu_tx_skb_pointers_reference_array[ g_free_skb_indexes_fifo_table[ g_cpu_tx_skb_free_indexes_release_ptr ] ] );
    
        /* increment counters */
        g_cpu_tx_released_skb_counter++;
        g_cpu_tx_skb_free_indexes_release_ptr = ( g_cpu_tx_skb_free_indexes_release_ptr + 1 ) % g_cpu_tx_abs_packet_limit;
        g_cpu_tx_skb_free_indexes_counter++;

    }
#endif

    if ( ( rdpa_dir_ds == xi_args->traffic_dir ) && ( xi_args->direction.ds.en_1588 ) )
    {
        /* waiting for an answer from the thread */
        while ( RDD_1588_TX_THREAD_RETURN_NO_RESULT == *thread_result_ptr ) {};

        f_rdd_unlock_irq ( &int_lock_irq, flags );

        if ( RDD_1588_TX_THREAD_RETURN_SUCCESS == *thread_result_ptr )
        {
            return BL_LILAC_RDD_OK;
        }
        else
        {
            return BL_LILAC_RDD_ERROR_1588_TX;
        }
    }

    f_rdd_unlock_irq ( &int_lock_irq, flags );
#endif
    return BL_LILAC_RDD_OK;
}

#if defined(DSL_63138) || defined(DSL_63148)

/*
 * NOTE: This function must be called with int_lock_irq locked.
 */
static inline BL_LILAC_RDD_ERROR_DTE alloc_cpu_tx_descriptor(LILAC_RDD_RUNNER_INDEX_DTS descriptor_queue_table_idx,
                                                             RDD_CPU_TX_DESCRIPTOR_DTS  **cpu_tx_descriptor_ptr,
                                                             uint32_t                   *descriptor_number,
                                                             uint16_t                   *skb_free_index)
{
#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)

    if (descriptor_queue_table_idx == FAST_RUNNER_A || descriptor_queue_table_idx == PICO_RUNNER_A)
        *cpu_tx_descriptor_ptr = (RDD_CPU_TX_DESCRIPTOR_DTS *)(DEVICE_ADDRESS(RUNNER_PRIVATE_0_OFFSET +
                                                                              g_cpu_tx_queue_write_ptr[descriptor_queue_table_idx]));
    else
        *cpu_tx_descriptor_ptr = (RDD_CPU_TX_DESCRIPTOR_DTS *)(DEVICE_ADDRESS(RUNNER_PRIVATE_1_OFFSET +
                                                                              g_cpu_tx_queue_write_ptr[descriptor_queue_table_idx]));

    if (g_cpu_tx_queue_free_counter[descriptor_queue_table_idx] == 0)
    {
        f_rdd_get_tx_descriptor_free_count(descriptor_queue_table_idx, *cpu_tx_descriptor_ptr);

        if (g_cpu_tx_queue_free_counter[descriptor_queue_table_idx] == 0)
        {
            g_cpu_tx_released_skb_counter++;
            /* skb_free() will be called by caller upon error */
            return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
        }
    }

    /* get skb pointer list free index */
    if (__is_skb_free_indexes_cache_not_empty(&g_cpu_tx_skb_free_indexes_cache))
    {
        *skb_free_index = __skb_free_indexes_cache_read(&g_cpu_tx_skb_free_indexes_cache);
        g_cpu_tx_pending_free_indexes_counter++;
    }
    else
    {
        g_cpu_tx_no_free_skb_counter++;
        /* skb_free() will be called by caller upon error */
        return ( BL_LILAC_RDD_ERROR_NO_FREE_SKB );
    }

    *descriptor_number = (g_cpu_tx_queue_write_ptr[descriptor_queue_table_idx] &
                          LILAC_RDD_CPU_TX_DESCRIPTOR_NUMBER_MASK) / LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    
    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[descriptor_queue_table_idx] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[descriptor_queue_table_idx] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;
    g_cpu_tx_queue_free_counter[descriptor_queue_table_idx]--;
#endif
    return BL_LILAC_RDD_OK;
}


static inline BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_write_gpon_packet_from_abs_address ( bdmf_sysb                            xi_packet_ptr,
                                                                                     uint32_t                             xi_packet_size,
                                                                                     uint32_t                             xi_upstream_gem_flow,
                                                                                     RDD_WAN_CHANNEL_ID                   xi_wan_channel,
                                                                                     BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller,
                                                                                     BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue,
                                                                                     int                                  xi_is_spdsvc_setup_packet )
{
#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
    RUNNER_REGS_CFG_CPU_WAKEUP     runner_cpu_wakeup_register;
    RDD_CPU_TX_DESCRIPTOR_DTS      *cpu_tx_descriptor_ptr;
    uint16_t                       free_index = 0xFFFF;
    unsigned long                  flags;
    RDD_WAN_TX_POINTERS_ENTRY_DTS  *wan_tx_pointers_entry_ptr;
    LILAC_RDD_RUNNER_INDEX_DTS     cpu_tx_descriptor_queue_table_idx;
    uint32_t                       descriptor_number;
    uint32_t                       cpu_tx_descriptor = 0;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_queue_table_idx = FAST_RUNNER_B;
    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET + g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] ));

    if ( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
    {
        f_rdd_get_tx_descriptor_free_count ( cpu_tx_descriptor_queue_table_idx, cpu_tx_descriptor_ptr );

        if( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
        {
            g_cpu_tx_released_skb_counter++;

            f_rdd_unlock_irq ( &int_lock_irq, flags );
            /* skb_free() will be called by caller upon error */
            return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
        }
    }

    /* get skb pointer list free index */
    if (__is_skb_free_indexes_cache_not_empty(&g_cpu_tx_skb_free_indexes_cache))
    {
        free_index = __skb_free_indexes_cache_read(&g_cpu_tx_skb_free_indexes_cache);

        g_cpu_tx_pending_free_indexes_counter++;
    }
    else
    {
        g_cpu_tx_no_free_skb_counter++;

        f_rdd_unlock_irq ( &int_lock_irq, flags );
        /* skb_free() will be called by caller upon error */
        return ( BL_LILAC_RDD_ERROR_NO_FREE_SKB );
    }

    /* save xi_packet_ptr ptr and data ptr */
    g_cpu_tx_skb_pointers_reference_array[ free_index ] = ( uint32_t )xi_packet_ptr;

    g_cpu_tx_data_pointers_reference_array[ free_index ] = ( uint32_t )swap4bytes( VIRT_TO_PHYS( bdmf_sysb_data(xi_packet_ptr) ) );
    g_cpu_tx_sent_abs_packets_counter++;

    /* write CPU-TX descriptor Word 1: Egress enqueue */
    RDD_CPU_TX_DESCRIPTOR_BPM_BUFFER_NUMBER_L_WRITE ( cpu_tx_descriptor, free_index & 0x3FFF );

    RDD_CPU_TX_DESCRIPTOR_CORE_PAYLOAD_OFFSET_L_WRITE ( cpu_tx_descriptor, ( g_ddr_headroom_size + BL_LILAC_RDD_DDR_PACKET_PAYLOAD_OFFSET ) / 2 );

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel ][ xi_rate_controller ][ xi_queue ] );
    RDD_CPU_TX_DESCRIPTOR_US_FAST_TX_QUEUE_L_WRITE ( cpu_tx_descriptor, ( ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - WAN_TX_QUEUES_TABLE_ADDRESS ) / sizeof( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS ) ) );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = 0;

    RDD_CPU_TX_DESCRIPTOR_CORE_PACKET_LENGTH_L_WRITE ( cpu_tx_descriptor, xi_packet_size + 4 );
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );

    if(xi_is_spdsvc_setup_packet)
    {
#if defined(CC_RDD_CPU_SPDSVC_DEBUG)
        g_spdsvc_setup_sysb_ptr = xi_packet_ptr;
        __rdd_cpu_trace("Speed Service Generator ON (%p, 0x%x)", g_spdsvc_setup_sysb_ptr, free_index);
#endif
        RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_CPU_TX_COMMAND_SPDSVC_PACKET );
    }
    else
    {
        RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_CPU_TX_COMMAND_ABSOLUTE_ADDRESS_PACKET );
    }
    RDD_CPU_TX_DESCRIPTOR_CORE_TX_QUEUE_L_WRITE ( cpu_tx_descriptor, 0 );
    RDD_CPU_TX_DESCRIPTOR_US_FAST_UPSTREAM_GEM_FLOW_L_WRITE ( cpu_tx_descriptor, xi_upstream_gem_flow );


    descriptor_number = ( g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] & LILAC_RDD_CPU_TX_DESCRIPTOR_NUMBER_MASK ) / LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;
    g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ]--;

    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt =  CPU_TX_FAST_THREAD_NUMBER >> 5;
    runner_cpu_wakeup_register.thread_num =  CPU_TX_FAST_THREAD_NUMBER & 0x1f;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    f_rdd_release_free_skb( RDD_CPU_TX_MAX_PENDING_FREE_INDEXES );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
#endif
    return BL_LILAC_RDD_OK;
}


static inline BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_write_eth_packet_from_abs_address ( bdmf_sysb                  xi_packet_ptr,
                                                                                    uint32_t                   xi_packet_size,
                                                                                    BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                                                                    uint8_t                    xi_wifi_ssid,
                                                                                    BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id,
                                                                                    int                        xi_is_spdsvc_setup_packet )
{

#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
    RUNNER_REGS_CFG_CPU_WAKEUP     runner_cpu_wakeup_register;
    RDD_CPU_TX_DESCRIPTOR_DTS      *cpu_tx_descriptor_ptr;
    uint16_t                       free_index = 0xFFFF;
    unsigned long                  flags;
    LILAC_RDD_RUNNER_INDEX_DTS     cpu_tx_descriptor_queue_table_idx;
    uint32_t                       descriptor_number;
    uint32_t                       cpu_tx_descriptor = 0;
    void                           *xi_packet_data_ptr;
#if defined(CONFIG_BCM_PKTRUNNER_GSO)
    uint32_t                       is_gso_pkt=0;
#endif

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_queue_table_idx = PICO_RUNNER_A;
    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET + g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] ));

    if( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
    {
        f_rdd_get_tx_descriptor_free_count ( cpu_tx_descriptor_queue_table_idx, cpu_tx_descriptor_ptr );

        if( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
        {
            g_cpu_tx_released_skb_counter++;

            f_rdd_unlock_irq ( &int_lock_irq, flags );
            /* skb_free() will be called by caller upon error */
            return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
        }
    }

    /* get skb pointer list free index */
    if (__is_skb_free_indexes_cache_not_empty(&g_cpu_tx_skb_free_indexes_cache))
    {
        free_index = __skb_free_indexes_cache_read(&g_cpu_tx_skb_free_indexes_cache);

        g_cpu_tx_pending_free_indexes_counter++;
    }
    else
    {
        g_cpu_tx_no_free_skb_counter++;

        f_rdd_unlock_irq ( &int_lock_irq, flags );
        /* skb_free() will be called by caller upon error */
        return ( BL_LILAC_RDD_ERROR_NO_FREE_SKB );
    }

#if defined(CONFIG_BCM_PKTRUNNER_GSO)
    /* if needed create a GSO Descriptor */
    xi_packet_data_ptr = bdmf_sysb_gso_data(xi_packet_ptr, &is_gso_pkt); 

    if(unlikely(!xi_packet_data_ptr))
    {
        g_cpu_tx_no_free_gso_desc_counter++;

        /*release the skbfree index */
        __skb_free_indexes_cache_write(&g_cpu_tx_skb_free_indexes_cache, free_index);
        g_cpu_tx_pending_free_indexes_counter--;

        f_rdd_unlock_irq ( &int_lock_irq, flags );
        /* skb_free() will be called by caller upon error */
        return ( BL_LILAC_RDD_ERROR_NO_FREE_GSODESC);
    }
#else
    xi_packet_data_ptr = bdmf_sysb_data(xi_packet_ptr);
#endif

    /* save xi_packet_ptr ptr and data ptr */
    g_cpu_tx_skb_pointers_reference_array[ free_index ] = ( uint32_t )xi_packet_ptr;

    g_cpu_tx_data_pointers_reference_array[ free_index ] = ( uint32_t )swap4bytes( VIRT_TO_PHYS( xi_packet_data_ptr ) );

    g_cpu_tx_sent_abs_packets_counter++;

    /* write CPU-TX descriptor Word 1: Egress enqueue */
    RDD_CPU_TX_DESCRIPTOR_BPM_BUFFER_NUMBER_L_WRITE ( cpu_tx_descriptor, free_index & 0x3FFF );

    RDD_CPU_TX_DESCRIPTOR_CORE_PAYLOAD_OFFSET_L_WRITE ( cpu_tx_descriptor, ( g_ddr_headroom_size + BL_LILAC_RDD_DDR_PACKET_PAYLOAD_OFFSET ) / 2 );

#if 0 // TAKE OUT defined(CONFIG_BCM_PKTRUNNER_GSO)
    if(is_gso_pkt)
    {
        /* update is_gso_pkt information in TX Descriptor */
        cpu_tx_descriptor = cpu_tx_descriptor | RDD_CPU_TX_DESCRIPTOR_GSO_PKT_L_WRITE ( is_gso_pkt );

        /* update gso packet count & byte count */
        g_cpu_tx_sent_abs_gso_packets_counter++;
        g_cpu_tx_sent_abs_gso_bytes_counter += xi_packet_size;

        /*update xi_packet_size*/
        xi_packet_size = sizeof(runner_gso_desc_t);
    }
#endif
    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = 0;

    RDD_CPU_TX_DESCRIPTOR_CORE_PACKET_LENGTH_L_WRITE ( cpu_tx_descriptor, xi_packet_size + 4 );
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );
    RDD_CPU_TX_DESCRIPTOR_CORE_EMAC_L_WRITE ( cpu_tx_descriptor, xi_emac_id );

    if( xi_is_spdsvc_setup_packet )
    {
#if defined(CC_RDD_CPU_SPDSVC_DEBUG)
        g_spdsvc_setup_sysb_ptr = xi_packet_ptr;
        __rdd_cpu_trace("Speed Service Generator ON (%p, 0x%x)", g_spdsvc_setup_sysb_ptr, free_index);
#endif
        RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_CPU_TX_COMMAND_SPDSVC_PACKET );
    }
    else
    {
        RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_CPU_TX_COMMAND_ABSOLUTE_ADDRESS_PACKET );
    }

    RDD_CPU_TX_DESCRIPTOR_CORE_TX_QUEUE_L_WRITE ( cpu_tx_descriptor,  xi_queue_id );

    descriptor_number = ( g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] & LILAC_RDD_CPU_TX_DESCRIPTOR_NUMBER_MASK ) / LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;
    g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ]--;

    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt =  CPU_TX_PICO_THREAD_NUMBER >> 5;
    runner_cpu_wakeup_register.thread_num =  CPU_TX_PICO_THREAD_NUMBER & 0x1f;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    f_rdd_release_free_skb( RDD_CPU_TX_MAX_PENDING_FREE_INDEXES );

#if defined(CONFIG_BCM_PKTRUNNER_GSO)
    if(is_gso_pkt)
    {
        /* update is_gso_pkt information in TX Descriptor */
        cpu_tx_descriptor = cpu_tx_descriptor | RDD_CPU_TX_DESCRIPTOR_GSO_PKT_L_WRITE ( is_gso_pkt );

        /* update gso packet count & byte count */
        g_cpu_tx_sent_abs_gso_packets_counter++;
        g_cpu_tx_sent_abs_gso_bytes_counter += xi_packet_size;

        /*update xi_packet_size*/
        xi_packet_size = sizeof(runner_gso_desc_t);
    }
#endif
    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
#endif
    return BL_LILAC_RDD_OK;
}

#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
//static uint32_t offload_counter = 0;
#endif

static inline BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_write_offload_packet_from_abs_address ( bdmf_sysb                  xi_packet_ptr,
                                                                                        uint32_t                   xi_packet_size,
                                                                                        BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id )
{

#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
    RUNNER_REGS_CFG_CPU_WAKEUP     runner_cpu_wakeup_register;
    RDD_CPU_TX_DESCRIPTOR_DTS      *cpu_tx_descriptor_ptr;
    uint16_t                       free_index = 0xFFFF;
    unsigned long                  flags;
    LILAC_RDD_RUNNER_INDEX_DTS     cpu_tx_descriptor_queue_table_idx;
    uint32_t                       descriptor_number;
    uint32_t                       cpu_tx_descriptor;
    void                           *xi_packet_data_ptr;
    RDD_FOUR_BYTES_DTS             *cpu_tx_abs_descriptor_ptr;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_queue_table_idx = FAST_RUNNER_A;
    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET + g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] ));

    if( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
    {
        f_rdd_get_tx_descriptor_free_count ( cpu_tx_descriptor_queue_table_idx, cpu_tx_descriptor_ptr );

        if( g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ] == 0 )
        {
            g_cpu_tx_released_skb_counter++;

            f_rdd_unlock_irq ( &int_lock_irq, flags );
            /* skb_free() will be called by caller upon error */
            return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
        }
    }

    /* get skb pointer list free index */
    if (__is_skb_free_indexes_cache_not_empty(&g_cpu_tx_skb_free_indexes_cache))
    {
        free_index = __skb_free_indexes_cache_read(&g_cpu_tx_skb_free_indexes_cache);

        g_cpu_tx_pending_free_indexes_counter++;
    }
    else
    {
        g_cpu_tx_no_free_skb_counter++;

        f_rdd_unlock_irq ( &int_lock_irq, flags );
        /* skb_free() will be called by caller upon error */
        return ( BL_LILAC_RDD_ERROR_NO_FREE_SKB );
    }

    xi_packet_data_ptr = bdmf_sysb_data(xi_packet_ptr);

    /* save xi_packet_ptr ptr and data ptr */
    g_cpu_tx_skb_pointers_reference_array[ free_index ] = ( uint32_t )xi_packet_ptr;

    g_cpu_tx_data_pointers_reference_array[ free_index ] = ( uint32_t )swap4bytes( VIRT_TO_PHYS( xi_packet_data_ptr ) );

    /* save data pointer also in CPU Tx queue */
    descriptor_number = ( g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] & LILAC_RDD_CPU_TX_DESCRIPTOR_NUMBER_MASK ) / LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;

    cpu_tx_abs_descriptor_ptr = ( RDD_FOUR_BYTES_DTS * )( DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + g_cpu_tx_queue_abs_data_ptr_write_ptr[ cpu_tx_descriptor_queue_table_idx ] + ( descriptor_number * LILAC_RDD_CPU_TX_ABS_DATA_PTR_DESCRIPTOR_SIZE ) );

    MWRITE_32( ( uint8_t * )cpu_tx_abs_descriptor_ptr,  ( uint32_t )( VIRT_TO_PHYS( xi_packet_data_ptr ) ) );

//    printk("XMIT: free_index %u, skb %p, descriptor_number %u, cpu_tx_abs_descriptor_ptr %p, xi_queue_id %u, count %08x\n", free_index, xi_packet_ptr, descriptor_number, cpu_tx_abs_descriptor_ptr, xi_queue_id, ++offload_counter);

    g_cpu_tx_sent_abs_packets_counter++;

    /* write CPU-TX descriptor Word 1: Egress enqueue */
    cpu_tx_descriptor = 0;

    RDD_CPU_TX_DESCRIPTOR_BPM_BUFFER_NUMBER_L_WRITE ( cpu_tx_descriptor, free_index & 0x3FFF );

    RDD_CPU_TX_DESCRIPTOR_CORE_PAYLOAD_OFFSET_L_WRITE ( cpu_tx_descriptor, ( g_ddr_headroom_size + BL_LILAC_RDD_DDR_PACKET_PAYLOAD_OFFSET ) / 2 );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    /* write CPU-TX descriptor Word 0: Egress enqueue */
    cpu_tx_descriptor = 0;

    RDD_CPU_TX_DESCRIPTOR_CORE_PACKET_LENGTH_L_WRITE ( cpu_tx_descriptor, xi_packet_size + 4 );
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );
    RDD_CPU_TX_DESCRIPTOR_CORE_COMMAND_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_CPU_TX_COMMAND_ABSOLUTE_ADDRESS_PACKET );
    RDD_CPU_TX_DESCRIPTOR_CORE_TX_QUEUE_L_WRITE ( cpu_tx_descriptor,  xi_queue_id );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ cpu_tx_descriptor_queue_table_idx ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;
    g_cpu_tx_queue_free_counter[ cpu_tx_descriptor_queue_table_idx ]--;

    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt =  CPU_TX_FAST_THREAD_NUMBER >> 5;
    runner_cpu_wakeup_register.thread_num =  CPU_TX_FAST_THREAD_NUMBER & 0x1f;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    f_rdd_release_free_skb( RDD_CPU_TX_MAX_PENDING_FREE_INDEXES );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
#endif
    return BL_LILAC_RDD_OK;
}

BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_interrupt_coalescing_config( uint32_t xi_ring_id, uint32_t xi_timeout_us, uint32_t xi_max_packet_count );
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_free_skb_timer_config ( void );
#endif /* DSL */

#if defined(CONFIG_BCM_PKTRUNNER_GSO)
BL_LILAC_RDD_ERROR_DTE rdd_gso_counters_get ( RDD_GSO_COUNTERS_ENTRY_DTS *xo_gso_counters_ptr );
BL_LILAC_RDD_ERROR_DTE rdd_gso_context_get ( RDD_GSO_CONTEXT_ENTRY_DTS *xo_gso_context_ptr );
BL_LILAC_RDD_ERROR_DTE rdd_gso_desc_get ( RDD_GSO_DESC_ENTRY_DTS *xo_gso_desc_ptr );
#endif /* CONFIG_BCM_PKTRUNNER_GSO */

#if defined(DSL_63138) || defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE rdd_cso_counters_get ( RDD_CSO_COUNTERS_ENTRY_DTS *xo_cso_counters_ptr );
BL_LILAC_RDD_ERROR_DTE rdd_cso_context_get ( RDD_CSO_CONTEXT_ENTRY_DTS *xo_cso_context_ptr );
#endif

static inline BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_ipsec_offload_from_abs_address(bdmf_sysb         xi_packet_ptr,
                                                                               rdpa_traffic_dir  xi_dir,
                                                                               uint8_t           xi_esphdr_offset,
                                                                               uint8_t           xi_sa_index,
                                                                               uint8_t           xi_sa_update,
                                                                               uint8_t           xi_cpu_qid

)
{
#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
    RUNNER_REGS_CFG_CPU_WAKEUP     runner_cpu_wakeup_register;
    RDD_CPU_TX_DESCRIPTOR_DTS      *cpu_tx_descriptor_ptr;
    uint16_t                       free_index = 0xFFFF;
    unsigned long                  flags;
    uint32_t                       descriptor_number;
    uint32_t                       cpu_tx_descriptor;
    uint32_t                       packet_size;
    void                           *packet_data_ptr;
    RDD_FOUR_BYTES_DTS             *cpu_tx_abs_descriptor_ptr;
    BL_LILAC_RDD_ERROR_DTE         rc;

    f_rdd_lock_irq(&int_lock_irq, &flags);

    rc = alloc_cpu_tx_descriptor(FAST_RUNNER_A, &cpu_tx_descriptor_ptr, &descriptor_number, &free_index);
    if (rc != BL_LILAC_RDD_OK)
    {
        f_rdd_unlock_irq(&int_lock_irq, flags);
        return rc;
    }
                                     
    packet_data_ptr = bdmf_sysb_data(xi_packet_ptr);
    packet_size     = bdmf_sysb_length(xi_packet_ptr);

    /* save xi_packet_ptr and data ptr */
    g_cpu_tx_skb_pointers_reference_array[free_index]  = (uint32_t)xi_packet_ptr;
    g_cpu_tx_data_pointers_reference_array[free_index] = (uint32_t)swap4bytes(VIRT_TO_PHYS(packet_data_ptr));

    /* save data pointer also in CPU Tx queue */
    cpu_tx_abs_descriptor_ptr = (RDD_FOUR_BYTES_DTS *)(DEVICE_ADDRESS(RUNNER_PRIVATE_0_OFFSET) +
                                                       g_cpu_tx_queue_abs_data_ptr_write_ptr[FAST_RUNNER_A] +
                                                       (descriptor_number * LILAC_RDD_CPU_TX_ABS_DATA_PTR_DESCRIPTOR_SIZE));

    MWRITE_32 ((uint8_t *)cpu_tx_abs_descriptor_ptr, (uint32_t)(VIRT_TO_PHYS(packet_data_ptr)));

    g_cpu_tx_sent_abs_packets_counter++;

    /* write CPU-TX descriptor Word 1: Egress enqueue */
    cpu_tx_descriptor = 0;

    RDD_CPU_TX_DESCRIPTOR_IPSEC_ABS_ADDRESS_INDEX_L_WRITE ( cpu_tx_descriptor, free_index & 0x3FFF );
    RDD_CPU_TX_DESCRIPTOR_IPSEC_ESPHDR_OFFSET_L_WRITE ( cpu_tx_descriptor, xi_esphdr_offset );
    RDD_CPU_TX_DESCRIPTOR_IPSEC_IPSEC_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );
    RDD_CPU_TX_DESCRIPTOR_IPSEC_ABS_FLAG_L_WRITE ( cpu_tx_descriptor,  LILAC_RDD_TRUE );
    
    MWRITE_32 ((uint8_t *)cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor);

    /* write CPU-TX descriptor Word 0: Egress enqueue */
    cpu_tx_descriptor = 0;

    RDD_CPU_TX_DESCRIPTOR_IPSEC_PACKET_LENGTH_L_WRITE ( cpu_tx_descriptor, packet_size );
    RDD_CPU_TX_DESCRIPTOR_IPSEC_SA_INDEX_L_WRITE ( cpu_tx_descriptor, xi_sa_index );
    if (xi_sa_update)
        RDD_CPU_TX_DESCRIPTOR_IPSEC_SA_UPDATE_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );
    if (xi_dir == rdpa_dir_us)
        RDD_CPU_TX_DESCRIPTOR_IPSEC_UPSTREAM_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );
        
    RDD_CPU_TX_DESCRIPTOR_IPSEC_CPU_RX_QUEUE_L_WRITE ( cpu_tx_descriptor,  xi_cpu_qid );
    RDD_CPU_TX_DESCRIPTOR_IPSEC_COMMAND_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_CPU_TX_COMMAND_ABSOLUTE_ADDRESS_PACKET );
    RDD_CPU_TX_DESCRIPTOR_IPSEC_VALID_L_WRITE ( cpu_tx_descriptor, LILAC_RDD_TRUE );

    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt   = CPU_TX_FAST_THREAD_NUMBER >> 5;
    runner_cpu_wakeup_register.thread_num = CPU_TX_FAST_THREAD_NUMBER & 0x1f;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    f_rdd_release_free_skb(RDD_CPU_TX_MAX_PENDING_FREE_INDEXES);

    MWRITE_32 ((uint8_t *)cpu_tx_descriptor_ptr, cpu_tx_descriptor);

    RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE (runner_cpu_wakeup_register);

    f_rdd_unlock_irq(&int_lock_irq, flags);
#endif
    return BL_LILAC_RDD_OK;
}

#endif /* _BL_LILAC_DRV_RUNNER_CPU_H */

