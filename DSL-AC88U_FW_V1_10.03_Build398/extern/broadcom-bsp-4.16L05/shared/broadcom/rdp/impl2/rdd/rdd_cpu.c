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

#include "rdd.h"

#if defined(DSL_63138) || defined(DSL_63148)
#define _RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_SIZE  RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_SIZE2
#else
#define _RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_SIZE  RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_SIZE
#endif

/******************************************************************************/
/*                                                                            */
/*                            Global Variables                                */
/*                                                                            */
/******************************************************************************/

uint16_t  g_ingress_packet_descriptors_counter;

#if defined(FIRMWARE_INIT)
extern uint32_t  GenericClassifierTable;
#endif

#if !defined(FIRMWARE_INIT)
static uint8_t                                 g_dummy_read;
#endif
extern uint8_t                                 g_broadcom_switch_mode;
extern BL_LILAC_RDD_BRIDGE_PORT_DTE            g_broadcom_switch_physical_port;
extern BL_LILAC_RDD_WAN_PHYSICAL_PORT_DTE      g_wan_physical_port;

extern RDD_WAN_TX_POINTERS_TABLE_DTS           *wan_tx_pointers_table_ptr;

#ifdef FIRMWARE_INIT
extern unsigned int CpuRxRingBase;
#endif

#if defined(CONFIG_DHD_RUNNER)
extern bdmf_boolean is_dhd_enabled[];
#endif

#if !defined(RDD_BASIC)
#if !defined(FIRMWARE_INIT)
static inline int32_t f_rdd_bridge_port_to_bpm_src_port ( BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_bridge_port )
{
    switch ( xi_bridge_port )
    {
    case BL_LILAC_RDD_WAN0_BRIDGE_PORT: // DSL
    case BL_LILAC_RDD_WAN1_BRIDGE_PORT: // DSL

        return ( DRV_BPM_SP_GPON );

    case BL_LILAC_RDD_LAN0_BRIDGE_PORT:

        return ( DRV_BPM_SP_EMAC0 );

    case BL_LILAC_RDD_LAN1_BRIDGE_PORT:

        return ( DRV_BPM_SP_EMAC1 );

    case BL_LILAC_RDD_LAN2_BRIDGE_PORT:

        return ( DRV_BPM_SP_EMAC2 );

    case BL_LILAC_RDD_LAN3_BRIDGE_PORT:

        return ( DRV_BPM_SP_EMAC3 );

    case BL_LILAC_RDD_LAN4_BRIDGE_PORT:

        return ( DRV_BPM_SP_EMAC4 );

    case BL_LILAC_RDD_PCI_BRIDGE_PORT:

        return ( DRV_BPM_SP_PCI0 );

    default:

        return ( 0 );
    }

    return ( 0 );
}
#endif /* !defined(FIRMWARE_INIT) */
#endif /* !defined(RDD_BASIC) */

static inline rdpa_cpu_reason cpu_reason_to_cpu_per_port_reason_index(rdpa_cpu_reason xi_cpu_reason)
{
    switch ( xi_cpu_reason )
    {
    case rdpa_cpu_rx_reason_mcast:
        return 0;
    case rdpa_cpu_rx_reason_bcast:
        return 1;
    case rdpa_cpu_rx_reason_unknown_da:
        return 2;
    default:
        break;
    }
    return 3;
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_initialize ( void )
{
    RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS               *cpu_reason_to_cpu_rx_queue_table_ptr;
    RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_DTS               *cpu_reason_to_cpu_rx_queue_entry_ptr;
    RDD_DS_CPU_REASON_TO_METER_TABLE_DTS                      *cpu_reason_to_meter_table_ptr;
    RDD_CPU_REASON_TO_METER_ENTRY_DTS                      *cpu_reason_to_meter_entry_ptr;
#if !defined(DSL_63138) && !defined(DSL_63148)
    RDD_CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_DTS  *cpu_reason_and_src_bridge_port_to_meter_table_ptr;
    RDD_CPU_REASON_TO_METER_ENTRY_DTS                      *cpu_reason_and_src_bridge_port_to_meter_entry_ptr;
    BL_LILAC_RDD_BRIDGE_PORT_DTE                           bridge_port;
    uint32_t                                               cpu_reason_per_port_index;
#endif
#if defined(FIRMWARE_INIT)
    RDD_CPU_RX_DESCRIPTOR_DTS                              *cpu_rx_descriptor_ptr;
    uint32_t                                               host_buffer_address;
#endif
    uint16_t                                               *cpu_rx_ingress_queue_ptr;
    uint8_t                                                cpu_reason;
    uint8_t                                                cpu_queue;
#if defined(FIRMWARE_INIT)
    uint32_t                                               i;

    /* Init Rings */
    cpu_rx_descriptor_ptr = ( RDD_CPU_RX_DESCRIPTOR_DTS * )CpuRxRingBase;

    host_buffer_address = SIMULATOR_DDR_RING_OFFSET + RDD_RING_DESCRIPTORS_TABLE_SIZE * SIMULATOR_DDR_RING_NUM_OF_ENTRIES * sizeof ( RDD_CPU_RX_DESCRIPTOR_DTS );

    for ( i = 0; i < RDD_RING_DESCRIPTORS_TABLE_SIZE * 10; i++, cpu_rx_descriptor_ptr++, host_buffer_address += LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE )
    {
        RDD_CPU_RX_DESCRIPTOR_HOST_DATA_BUFFER_POINTER_WRITE ( host_buffer_address, cpu_rx_descriptor_ptr );
    }
#endif

    cpu_rx_ingress_queue_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_CPU_RX_FAST_INGRESS_QUEUE_PTR_ADDRESS );

    MWRITE_16( cpu_rx_ingress_queue_ptr, DS_CPU_RX_FAST_INGRESS_QUEUE_ADDRESS );

    cpu_rx_ingress_queue_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_CPU_RX_FAST_INGRESS_QUEUE_PTR_ADDRESS );

    MWRITE_16( cpu_rx_ingress_queue_ptr, US_CPU_RX_FAST_INGRESS_QUEUE_ADDRESS );


    cpu_rx_ingress_queue_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_CPU_RX_PICO_INGRESS_QUEUE_PTR_ADDRESS );

    MWRITE_16( cpu_rx_ingress_queue_ptr, DS_CPU_RX_PICO_INGRESS_QUEUE_ADDRESS );

    cpu_rx_ingress_queue_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_CPU_RX_PICO_INGRESS_QUEUE_PTR_ADDRESS );

    MWRITE_16( cpu_rx_ingress_queue_ptr, US_CPU_RX_PICO_INGRESS_QUEUE_ADDRESS );

    cpu_reason_to_cpu_rx_queue_table_ptr = ( RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + DS_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_ADDRESS );

    for ( cpu_reason = rdpa_cpu_rx_reason_direct_queue_0, cpu_queue = BL_LILAC_RDD_CPU_RX_QUEUE_0; cpu_reason <= rdpa_cpu_rx_reason_direct_queue_7; cpu_reason++, cpu_queue++)
    {
#if defined(DSL_63138) || defined(DSL_63148)
        cpu_reason_to_cpu_rx_queue_entry_ptr = &( cpu_reason_to_cpu_rx_queue_table_ptr->entry[ 0 ] [ cpu_reason ] );
        RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_CPU_RX_QUEUE_WRITE ( cpu_queue, cpu_reason_to_cpu_rx_queue_entry_ptr );
        cpu_reason_to_cpu_rx_queue_entry_ptr = &( cpu_reason_to_cpu_rx_queue_table_ptr->entry[ 1 ] [ cpu_reason ] );
        RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_CPU_RX_QUEUE_WRITE ( cpu_queue, cpu_reason_to_cpu_rx_queue_entry_ptr );
#else
        cpu_reason_to_cpu_rx_queue_entry_ptr = &( cpu_reason_to_cpu_rx_queue_table_ptr->entry[ cpu_reason ] );
        RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_CPU_RX_QUEUE_WRITE ( cpu_queue, cpu_reason_to_cpu_rx_queue_entry_ptr );
#endif
    }


    for ( cpu_reason = rdpa_cpu_rx_reason_oam; cpu_reason < rdpa_cpu_reason__num_of; cpu_reason++ )
    {
        cpu_reason_to_meter_table_ptr = ( RDD_DS_CPU_REASON_TO_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_CPU_REASON_TO_METER_TABLE_ADDRESS );

        cpu_reason_to_meter_entry_ptr = &( cpu_reason_to_meter_table_ptr->entry[ cpu_reason ] );

        RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_WRITE ( BL_LILAC_RDD_CPU_METER_DISABLE, cpu_reason_to_meter_entry_ptr );

        cpu_reason_to_meter_table_ptr = ( RDD_DS_CPU_REASON_TO_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_CPU_REASON_TO_METER_TABLE_ADDRESS );

        cpu_reason_to_meter_entry_ptr = &( cpu_reason_to_meter_table_ptr->entry[ cpu_reason ] );

        RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_WRITE ( BL_LILAC_RDD_CPU_METER_DISABLE, cpu_reason_to_meter_entry_ptr );

#if !defined(DSL_63138) && !defined(DSL_63148)
        if ( ( cpu_reason != rdpa_cpu_rx_reason_mcast ) && ( cpu_reason != rdpa_cpu_rx_reason_bcast ) && ( cpu_reason != rdpa_cpu_rx_reason_unknown_da ) )
        {
            continue;
        }

        cpu_reason_and_src_bridge_port_to_meter_table_ptr =
            ( RDD_CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_ADDRESS );

        cpu_reason_per_port_index = cpu_reason_to_cpu_per_port_reason_index(cpu_reason);

        for ( bridge_port = BL_LILAC_RDD_WAN0_BRIDGE_PORT; bridge_port <= BL_LILAC_RDD_PCI_BRIDGE_PORT; bridge_port++ ) // DSL
        {
            cpu_reason_and_src_bridge_port_to_meter_entry_ptr = &( cpu_reason_and_src_bridge_port_to_meter_table_ptr->entry[ cpu_reason_per_port_index ][ bridge_port ] );

            RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_WRITE ( BL_LILAC_RDD_CPU_METER_DISABLE, cpu_reason_and_src_bridge_port_to_meter_entry_ptr );
        }
#endif
    }

    return ( BL_LILAC_RDD_OK );
}

#if !defined(FIRMWARE_INIT)
#if defined(DSL_63138) || defined(DSL_63148)
extern cpu_tx_skb_free_indexes_cache_t g_cpu_tx_skb_free_indexes_cache;

static inline BL_LILAC_RDD_ERROR_DTE f_rdd_cpu_tx_skb_free_cache_initialize ( void )
{
    g_cpu_tx_skb_free_indexes_cache.write = 0;
    g_cpu_tx_skb_free_indexes_cache.read = 0;
    g_cpu_tx_skb_free_indexes_cache.count = 0;
    g_cpu_tx_skb_free_indexes_cache.data = (uint16_t *)CACHED_MALLOC_ATOMIC(sizeof(RDD_FREE_SKB_INDEXES_FIFO_ENTRY_DTS) * g_cpu_tx_abs_packet_limit);

    if (g_cpu_tx_skb_free_indexes_cache.data == NULL)
        return ( BL_LILAC_RDD_ERROR_MALLOC_FAILED );
    return ( BL_LILAC_RDD_OK );
}
#endif /* defined(DSL) */
#endif

BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_initialize ( void )
{
    uint32_t  *ih_header_descriptor_ptr;
    uint32_t  ih_header_descriptor[ 2 ];
#if !defined(DSL_63138) && !defined(DSL_63148)
    uint16_t  *cpu_tx_ingress_queue_ptr;
    uint8_t   *wan_physical_port_ptr;
#endif /* !DSL */
    uint32_t  *ih_buffer_bbh_ptr;
    int i;

    ih_header_descriptor_ptr = ( uint32_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RUNNER_FLOW_HEADER_DESCRIPTOR_ADDRESS );

    ih_header_descriptor[ 0 ] = ( LILAC_RDD_ON << 5 ) + ( CPU_TX_FAST_THREAD_NUMBER << 6 );

    ih_header_descriptor[ 1 ] = WAN_SRC_PORT + ( LILAC_RDD_IH_HEADER_LENGTH << 5 ) + ( LILAC_RDD_RUNNER_A_IH_BUFFER << 20 );

    MWRITE_32( ( ( uint8_t * )ih_header_descriptor_ptr + 0 ), ih_header_descriptor[ 0 ] );
    MWRITE_32( ( ( uint8_t * )ih_header_descriptor_ptr + 4 ), ih_header_descriptor[ 1 ] );

    ih_header_descriptor_ptr = ( uint32_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_RUNNER_FLOW_HEADER_DESCRIPTOR_ADDRESS );
    for (i = 0; i < 3; i++)
    {
        ih_header_descriptor[ 0 ] = LILAC_RDD_ON << 5;

        ih_header_descriptor[ 1 ] = ( LILAC_RDD_IH_HEADER_LENGTH << 5 ) + ( LILAC_RDD_RUNNER_B_IH_BUFFER << 20 );

        MWRITE_32( ( ( uint8_t * )ih_header_descriptor_ptr + 0 ), ih_header_descriptor[ 0 ] );
        MWRITE_32( ( ( uint8_t * )ih_header_descriptor_ptr + 4 ), ih_header_descriptor[ 1 ] );
        ih_header_descriptor_ptr += 2;
    }

    ih_buffer_bbh_ptr = ( uint32_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + IH_BUFFER_BBH_POINTER_ADDRESS );

    MWRITE_32( ih_buffer_bbh_ptr, ( ( BBH_PERIPHERAL_IH << 16 ) | ( LILAC_RDD_IH_BUFFER_BBH_ADDRESS + LILAC_RDD_RUNNER_B_IH_BUFFER_BBH_OFFSET ) ) );

    g_cpu_tx_queue_write_ptr[ FAST_RUNNER_A ] = CPU_TX_FAST_QUEUE_ADDRESS;
    g_cpu_tx_queue_write_ptr[ FAST_RUNNER_B ] = CPU_TX_FAST_QUEUE_ADDRESS;
    g_cpu_tx_queue_write_ptr[ PICO_RUNNER_A ] = CPU_TX_PICO_QUEUE_ADDRESS;
    g_cpu_tx_queue_write_ptr[ PICO_RUNNER_B ] = CPU_TX_PICO_QUEUE_ADDRESS;

    g_cpu_tx_queue_abs_data_ptr_write_ptr[ FAST_RUNNER_A ] = DS_FAST_CPU_TX_DESCRIPTOR_ABS_DATA_PTR_QUEUE_ADDRESS;
    g_cpu_tx_queue_abs_data_ptr_write_ptr[ FAST_RUNNER_B ] = US_FAST_CPU_TX_DESCRIPTOR_ABS_DATA_PTR_QUEUE_ADDRESS;
    g_cpu_tx_queue_abs_data_ptr_write_ptr[ PICO_RUNNER_A ] = DS_PICO_CPU_TX_DESCRIPTOR_ABS_DATA_PTR_QUEUE_ADDRESS;
    g_cpu_tx_queue_abs_data_ptr_write_ptr[ PICO_RUNNER_B ] = US_PICO_CPU_TX_DESCRIPTOR_ABS_DATA_PTR_QUEUE_ADDRESS;

#if defined(DSL_63138) || defined(DSL_63148)
    {
        uint16_t *gso_queue_ptr_ptr;

        gso_queue_ptr_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + GSO_PICO_QUEUE_PTR_ADDRESS );
        MWRITE_16( gso_queue_ptr_ptr, GSO_PICO_QUEUE_ADDRESS );
    }

#if !defined(FIRMWARE_INIT) && !defined(RDD_BASIC)
    f_rdd_cpu_tx_skb_free_cache_initialize();
    f_rdd_initialize_skb_free_indexes_cache();
    rdd_cpu_tx_free_skb_timer_config();
#endif
#endif /* DSL */

    {
        uint16_t *ipsec_queue_ptr_ptr;

        ipsec_queue_ptr_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + IPSEC_DS_QUEUE_PTR_ADDRESS );
        MWRITE_16( ipsec_queue_ptr_ptr, IPSEC_DS_QUEUE_ADDRESS );
    }

#if !defined(DSL_63138) && !defined(DSL_63148)
    cpu_tx_ingress_queue_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + CPU_TX_PICO_INGRESS_QUEUE_PTR_ADDRESS );

    MWRITE_16( cpu_tx_ingress_queue_ptr, CPU_TX_EMAC_LOOPBACK_QUEUE_ADDRESS );

    cpu_tx_ingress_queue_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + CPU_TX_PICO_US_FLOODING_QUEUE_PTR_ADDRESS - sizeof ( RUNNER_COMMON ) );

    MWRITE_16( cpu_tx_ingress_queue_ptr, CPU_TX_US_FLOODING_QUEUE_ADDRESS );

    ih_buffer_bbh_ptr = ( uint32_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + IH_BUFFER_BBH_POINTER_ADDRESS );

    MWRITE_32( ih_buffer_bbh_ptr, ( ( BBH_PERIPHERAL_IH << 16 ) | ( LILAC_RDD_IH_BUFFER_BBH_ADDRESS + LILAC_RDD_RUNNER_B_IH_BUFFER_BBH_OFFSET ) ) );

    wan_physical_port_ptr = ( uint8_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + WAN_PHYSICAL_PORT_ADDRESS );

    if ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH4 )
    {
        MWRITE_8( wan_physical_port_ptr, ETH4_SRC_PORT );
    }
    else if ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH0 )
    {
        MWRITE_8( wan_physical_port_ptr, ETH0_SRC_PORT );
    }
    else
    {
        MWRITE_8( wan_physical_port_ptr, WAN_SRC_PORT );
    }
#endif /* !DSL */

#if defined(CONFIG_BCM_PKTRUNNER_GSO)
    bdmf_gso_desc_pool_create(RUNNER_MAX_GSO_DESC );
#endif
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_TYPE  xi_msg_type,
                                                   LILAC_RDD_RUNNER_INDEX_DTS     xi_runner_index,
                                                   uint32_t                       xi_sram_base,
                                                   uint32_t                       xi_parameter_1,
                                                   uint32_t                       xi_parameter_2,
                                                   uint32_t                       xi_parameter_3,
                                                   BL_LILAC_RDD_CPU_WAIT_DTE      xi_wait )
{
    RUNNER_REGS_CFG_CPU_WAKEUP  runner_cpu_wakeup_register;
    RDD_CPU_TX_DESCRIPTOR_DTS   *cpu_tx_descriptor_ptr;
#if !defined(BDMF_SYSTEM_SIM)
    uint32_t                    cpu_tx_descriptor_valid;
#endif

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( xi_sram_base ) + g_cpu_tx_queue_write_ptr[ xi_runner_index ] );

    if( g_cpu_tx_queue_free_counter[ xi_runner_index ] == 0 )
    {
        f_rdd_get_tx_descriptor_free_count ( xi_runner_index, cpu_tx_descriptor_ptr );

        if( g_cpu_tx_queue_free_counter[ xi_runner_index ] == 0 )
            return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
    }

    RDD_CPU_TX_MESSAGE_DESCRIPTOR_COMMAND_WRITE ( LILAC_RDD_CPU_TX_COMMAND_MESSAGE, cpu_tx_descriptor_ptr );
    RDD_CPU_TX_MESSAGE_DESCRIPTOR_MESSAGE_TYPE_WRITE ( xi_msg_type, cpu_tx_descriptor_ptr );

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_FLUSH_GPON_QUEUE )
    {
        RDD_CPU_TX_DESCRIPTOR_TX_QUEUE_PTR_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_FLUSH_ETH_QUEUE )
    {
        RDD_CPU_TX_DESCRIPTOR_EMAC_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
        RDD_CPU_TX_DESCRIPTOR_QUEUE_WRITE ( xi_parameter_2, cpu_tx_descriptor_ptr );
    }

#ifdef CONFIG_DHD_RUNNER
    if ( is_dhd_enabled[xi_parameter_2 >> 14] && xi_runner_index == PICO_RUNNER_A && xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_DHD_MESSAGE )
    {
        RDD_CPU_TX_DHD_MESSAGE_DESCRIPTOR_DHD_MSG_TYPE_WRITE( xi_parameter_1, cpu_tx_descriptor_ptr );
        RDD_CPU_TX_DHD_MESSAGE_DESCRIPTOR_FLOW_RING_ID_WRITE( (xi_parameter_2 & ~0xc000), cpu_tx_descriptor_ptr );
        RDD_CPU_TX_DHD_MESSAGE_DESCRIPTOR_RADIO_IDX_WRITE( xi_parameter_2 >> 14, cpu_tx_descriptor_ptr );
        RDD_CPU_TX_DHD_MESSAGE_DESCRIPTOR_DISABLED_WRITE ( xi_parameter_3, cpu_tx_descriptor_ptr );
    }
#endif

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_RELEASE_SKB_BUFFERS )
    {
        RDD_CPU_TX_DESCRIPTOR_EMAC_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

    if ( ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_FLOW_PM_COUNTERS_GET ) || ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_RX_FLOW_PM_COUNTERS_GET ) || ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_TX_FLOW_PM_COUNTERS_GET ) )
    {
        RDD_CPU_TX_DESCRIPTOR_FLOW_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_BRIDGE_PORT_PM_COUNTERS_GET )
    {
        RDD_CPU_TX_DESCRIPTOR_SRC_BRIDGE_PORT_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_PM_COUNTER_GET )
    {
        RDD_CPU_TX_DESCRIPTOR_GROUP_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
        RDD_CPU_TX_DESCRIPTOR_COUNTER_WRITE ( xi_parameter_2, cpu_tx_descriptor_ptr );
        RDD_CPU_TX_DESCRIPTOR_COUNTER_4_BYTES_WRITE ( xi_parameter_3, cpu_tx_descriptor_ptr );
    }

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_IPTV_MAC_COUNTER_GET )
    {
        RDD_CPU_TX_DESCRIPTOR_IPTV_MAC_IDX_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

#if defined(DSL_63138) || defined(DSL_63148)
    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_UPDATE_PD_POOL_QUOTA )
    {
        RDD_CPU_TX_UPDATE_PD_POOL_QUOTA_MESSAGE_DESCRIPTOR_GUARANTEED_FREE_COUNT_INCR_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
        RDD_CPU_TX_UPDATE_PD_POOL_QUOTA_MESSAGE_DESCRIPTOR_GUARANTEED_FREE_COUNT_DELTA_WRITE ( xi_parameter_2, cpu_tx_descriptor_ptr );
    }
#else
    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_ACTIVATE_TCONT )
    {
        RDD_CPU_TX_DESCRIPTOR_TCONT_INDEX_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }
#endif

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_MIRRORING_MODE_CONFIG
#ifdef CONFIG_DHD_RUNNER
        && xi_runner_index == FAST_RUNNER_B
#endif
        )
    {
        RDD_CPU_TX_DESCRIPTOR_EMAC_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_INVALIDATE_CONTEXT_INDEX_CACHE_ENTRY )
    {
        RDD_CPU_TX_DESCRIPTOR_CONTEXT_INDEX_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

    if ( xi_msg_type == LILAC_RDD_CPU_TX_MESSAGE_RING_DESTROY )
    {
        RDD_CPU_TX_DESCRIPTOR_QUEUE_WRITE ( xi_parameter_1, cpu_tx_descriptor_ptr );
    }

    RDD_CPU_TX_MESSAGE_DESCRIPTOR_VALID_WRITE ( LILAC_RDD_TRUE, cpu_tx_descriptor_ptr );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ xi_runner_index ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ xi_runner_index ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;
    g_cpu_tx_queue_free_counter[ xi_runner_index ]--;

    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    if ( xi_runner_index == FAST_RUNNER_A || xi_runner_index == FAST_RUNNER_B )
    {
        runner_cpu_wakeup_register.req_trgt = CPU_TX_FAST_THREAD_NUMBER / 32;
        runner_cpu_wakeup_register.thread_num = CPU_TX_FAST_THREAD_NUMBER % 32;
        runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;
    }
    else
    {
        runner_cpu_wakeup_register.req_trgt = CPU_TX_PICO_THREAD_NUMBER / 32;
        runner_cpu_wakeup_register.thread_num = CPU_TX_PICO_THREAD_NUMBER % 32;
        runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;
    }

    if ( xi_runner_index == FAST_RUNNER_A || xi_runner_index == PICO_RUNNER_A )
    {
        RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
    }
    else
    {
        RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
    }

#if !defined(BDMF_SYSTEM_SIM)
    if ( xi_wait == BL_LILAC_RDD_WAIT )
    {
        /* wait for the cpu tx thread to finish the current message */
        do
        {
            RDD_CPU_TX_MESSAGE_DESCRIPTOR_VALID_READ ( cpu_tx_descriptor_valid, ( ( volatile RDD_CPU_TX_DESCRIPTOR_DTS * )cpu_tx_descriptor_ptr ) );
        }
        while ( cpu_tx_descriptor_valid == LILAC_RDD_TRUE );
    }
#endif

    return ( BL_LILAC_RDD_OK );
}
BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_queue_discard_get ( BL_LILAC_RDD_CPU_RX_QUEUE_DTE  xi_ring_id,
                                                      uint16_t                       *xo_number_of_packets )
{
    RDD_RING_DESCRIPTORS_TABLE_DTS  *ring_table_ptr;
    RDD_RING_DESCRIPTOR_DTS         *ring_descriptor_ptr;
    unsigned long                   flags;

    /* check the validity of the input parameters - CPU-RX queue index */
    if ( xi_ring_id >= RDD_RING_DESCRIPTORS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_ILLEGAL );
    }

    ring_table_ptr = ( RDD_RING_DESCRIPTORS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + RING_DESCRIPTORS_TABLE_ADDRESS );

    ring_descriptor_ptr = &(ring_table_ptr->entry[ xi_ring_id ]);

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    RDD_RING_DESCRIPTOR_DROP_COUNTER_READ ( *xo_number_of_packets, ring_descriptor_ptr );
    RDD_RING_DESCRIPTOR_DROP_COUNTER_WRITE ( 0, ring_descriptor_ptr );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


#if defined(DSL_63138) || defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE rdd_cpu_reason_to_cpu_rx_queue ( rdpa_cpu_reason  xi_cpu_reason,
                                                        BL_LILAC_RDD_CPU_RX_QUEUE_DTE   xi_queue_id,
                                                        rdpa_traffic_dir                xi_direction,
                                                        uint32_t                        xi_table_index )
{
    RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS  *cpu_reason_to_cpu_rx_queue_table_ptr;
    RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_DTS  *cpu_reason_to_cpu_rx_queue_entry_ptr;
    uint8_t                                   cpu_queue;

    /* check the validity of the input parameters - CPU-RX reason */
    if ( xi_cpu_reason >= _RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_REASON_ILLEGAL );
    }

    if ( (xi_direction == rdpa_dir_ds && xi_table_index > CPU_REASON_WAN1_TABLE_INDEX) ||
         (xi_direction == rdpa_dir_us && xi_table_index > CPU_REASON_LAN_TABLE_INDEX) )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_REASON_ILLEGAL );
    }

    /* check the validity of the input parameters - CPU-RX queue-id */
    if ( xi_queue_id >= CPU_RX_NUMBER_OF_QUEUES )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_ILLEGAL );
    }

    if ( xi_direction == rdpa_dir_ds )
    {
        cpu_reason_to_cpu_rx_queue_table_ptr = ( RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + DS_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_ADDRESS );
    }
    else
    {
        cpu_reason_to_cpu_rx_queue_table_ptr = ( RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + US_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_ADDRESS - sizeof ( RUNNER_COMMON ) );
    }

    cpu_reason_to_cpu_rx_queue_entry_ptr = &( cpu_reason_to_cpu_rx_queue_table_ptr->entry[xi_table_index] [ xi_cpu_reason ] );

    cpu_queue = xi_queue_id;

    RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_CPU_RX_QUEUE_WRITE ( cpu_queue, cpu_reason_to_cpu_rx_queue_entry_ptr );

    return ( BL_LILAC_RDD_OK );
}
#else
BL_LILAC_RDD_ERROR_DTE rdd_cpu_reason_to_cpu_rx_queue ( rdpa_cpu_reason  xi_cpu_reason,
                                                        BL_LILAC_RDD_CPU_RX_QUEUE_DTE   xi_queue_id,
                                                        rdpa_traffic_dir                xi_direction )
{
    RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS  *cpu_reason_to_cpu_rx_queue_table_ptr;
    RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_DTS  *cpu_reason_to_cpu_rx_queue_entry_ptr;
    uint8_t                                   cpu_queue;

    /* check the validity of the input parameters - CPU-RX reason */
    if ( xi_cpu_reason >= RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_REASON_ILLEGAL );
    }

    /* check the validity of the input parameters - CPU-RX queue-id */
    if ( xi_queue_id >= CPU_RX_NUMBER_OF_QUEUES )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_ILLEGAL );
    }

    if ( xi_direction == rdpa_dir_ds )
    {
        cpu_reason_to_cpu_rx_queue_table_ptr = ( RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + DS_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_ADDRESS );
    }
    else
    {
        cpu_reason_to_cpu_rx_queue_table_ptr = ( RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + US_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_ADDRESS - sizeof ( RUNNER_COMMON ) );
    }

    cpu_reason_to_cpu_rx_queue_entry_ptr = &( cpu_reason_to_cpu_rx_queue_table_ptr->entry[ xi_cpu_reason ] );

    cpu_queue = xi_queue_id;

    RDD_CPU_REASON_TO_CPU_RX_QUEUE_ENTRY_CPU_RX_QUEUE_WRITE ( cpu_queue, cpu_reason_to_cpu_rx_queue_entry_ptr );

    return ( BL_LILAC_RDD_OK );
}
#endif


static BL_LILAC_RDD_ERROR_DTE f_bl_lilac_rdd_cpu_reason_and_src_bridge_port_to_cpu_rx_meter_cfg ( rdpa_cpu_reason  xi_cpu_reason,
                                                                                                  BL_LILAC_RDD_CPU_METER_DTE      xi_cpu_meter,
                                                                                                  BL_LILAC_RDD_BRIDGE_PORT_DTE    xi_src_port )
{
    RDD_CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_DTS  *cpu_reason_and_src_bridge_port_to_meter_table_ptr;
    RDD_CPU_REASON_TO_METER_ENTRY_DTS                      *cpu_reason_and_src_bridge_port_to_meter_entry_ptr;
    uint32_t                                               cpu_reason_per_port_index;

    cpu_reason_and_src_bridge_port_to_meter_table_ptr = ( RDD_CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_ADDRESS );

    cpu_reason_per_port_index = 0;

    cpu_reason_per_port_index = cpu_reason_to_cpu_per_port_reason_index(xi_cpu_reason);

    cpu_reason_and_src_bridge_port_to_meter_entry_ptr = &( cpu_reason_and_src_bridge_port_to_meter_table_ptr->entry[ cpu_reason_per_port_index ][ xi_src_port ] );

    RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_WRITE ( xi_cpu_meter, cpu_reason_and_src_bridge_port_to_meter_entry_ptr );

    return ( BL_LILAC_RDD_OK );
}


static BL_LILAC_RDD_ERROR_DTE f_bl_lilac_rdd_cpu_reason_and_src_bridge_port_to_cpu_rx_meter_clear ( rdpa_cpu_reason  xi_cpu_reason,
                                                                                                    BL_LILAC_RDD_CPU_METER_DTE      xi_cpu_meter,
                                                                                                    BL_LILAC_RDD_BRIDGE_PORT_DTE    xi_src_port )
{
    RDD_CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_DTS  *cpu_reason_and_src_bridge_port_to_meter_table_ptr;
    RDD_CPU_REASON_TO_METER_ENTRY_DTS                      *cpu_reason_and_src_bridge_port_to_meter_entry_ptr;
    BL_LILAC_RDD_CPU_METER_DTE                             curr_cpu_meter;
    uint32_t                                               cpu_reason_per_port_index;

    cpu_reason_and_src_bridge_port_to_meter_table_ptr = ( RDD_CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) +
                                                                                                                    CPU_REASON_AND_SRC_BRIDGE_PORT_TO_METER_TABLE_ADDRESS );

    cpu_reason_per_port_index = 0;

    cpu_reason_per_port_index = cpu_reason_to_cpu_per_port_reason_index(xi_cpu_reason);

    cpu_reason_and_src_bridge_port_to_meter_entry_ptr = &( cpu_reason_and_src_bridge_port_to_meter_table_ptr->entry[ cpu_reason_per_port_index ][ xi_src_port ] );

    RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_READ ( curr_cpu_meter, cpu_reason_and_src_bridge_port_to_meter_entry_ptr );

    if ( curr_cpu_meter == xi_cpu_meter )
    {
        RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_WRITE ( BL_LILAC_RDD_CPU_METER_DISABLE, cpu_reason_and_src_bridge_port_to_meter_entry_ptr );
    }

    return ( BL_LILAC_RDD_OK );
}


static BL_LILAC_RDD_ERROR_DTE cfg_cpu_reason_and_src_bridge_port_to_cpu_rx_meter ( rdpa_cpu_reason       xi_cpu_reason,
                                                                                              BL_LILAC_RDD_CPU_METER_DTE           xi_cpu_meter,
                                                                                              BL_LILAC_RDD_BRIDGE_PORT_VECTOR_DTE  xi_src_port_mask )
{
    BL_LILAC_RDD_BRIDGE_PORT_VECTOR_DTE  bridge_port_vector;
    BL_LILAC_RDD_BRIDGE_PORT_DTE         bridge_port;

    for ( bridge_port_vector = BL_LILAC_RDD_BRIDGE_PORT_VECTOR_PCI; bridge_port_vector <= BL_LILAC_RDD_BRIDGE_PORT_VECTOR_LAN4; bridge_port_vector <<= 1 )
    {
        bridge_port = rdd_bridge_port_vector_to_bridge_port ( bridge_port_vector );

        if ( xi_src_port_mask & bridge_port_vector )
        {
            f_bl_lilac_rdd_cpu_reason_and_src_bridge_port_to_cpu_rx_meter_cfg ( xi_cpu_reason, xi_cpu_meter, bridge_port );
        }
        else
        {
            f_bl_lilac_rdd_cpu_reason_and_src_bridge_port_to_cpu_rx_meter_clear ( xi_cpu_reason, xi_cpu_meter, bridge_port );
        }
    }

    return ( BL_LILAC_RDD_OK );
}


static BL_LILAC_RDD_ERROR_DTE f_bl_lilac_rdd_cpu_reason_to_cpu_rx_meter_ds ( rdpa_cpu_reason  xi_cpu_reason,
                                                                             BL_LILAC_RDD_CPU_METER_DTE      xi_cpu_meter )
{
    RDD_DS_CPU_REASON_TO_METER_TABLE_DTS  *cpu_reason_to_meter_table_ptr;
    RDD_CPU_REASON_TO_METER_ENTRY_DTS  *cpu_reason_to_meter_entry_ptr;

    cpu_reason_to_meter_table_ptr = ( RDD_DS_CPU_REASON_TO_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_CPU_REASON_TO_METER_TABLE_ADDRESS );

    cpu_reason_to_meter_entry_ptr = &( cpu_reason_to_meter_table_ptr->entry[ xi_cpu_reason ] );
    
    RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_WRITE ( xi_cpu_meter, cpu_reason_to_meter_entry_ptr );

    return ( BL_LILAC_RDD_OK );
}


static BL_LILAC_RDD_ERROR_DTE f_bl_lilac_rdd_cpu_reason_to_cpu_rx_meter_us ( rdpa_cpu_reason       xi_cpu_reason,
                                                                             BL_LILAC_RDD_CPU_METER_DTE           xi_cpu_meter,
                                                                             BL_LILAC_RDD_BRIDGE_PORT_VECTOR_DTE  xi_src_port_mask )
{
    RDD_DS_CPU_REASON_TO_METER_TABLE_DTS  *cpu_reason_to_meter_table_ptr;
    RDD_CPU_REASON_TO_METER_ENTRY_DTS  *cpu_reason_to_meter_entry_ptr;

    if ( ( xi_cpu_reason == rdpa_cpu_rx_reason_mcast ) || ( xi_cpu_reason == rdpa_cpu_rx_reason_bcast ) || ( xi_cpu_reason == rdpa_cpu_rx_reason_unknown_da ) )
    {
        cfg_cpu_reason_and_src_bridge_port_to_cpu_rx_meter ( xi_cpu_reason, xi_cpu_meter, xi_src_port_mask );
    }
    else
    {
        cpu_reason_to_meter_table_ptr = ( RDD_DS_CPU_REASON_TO_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_CPU_REASON_TO_METER_TABLE_ADDRESS );

        cpu_reason_to_meter_entry_ptr = &( cpu_reason_to_meter_table_ptr->entry[ xi_cpu_reason ] );

        RDD_CPU_REASON_TO_METER_ENTRY_CPU_METER_WRITE ( xi_cpu_meter, cpu_reason_to_meter_entry_ptr );
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_reason_to_cpu_rx_meter ( rdpa_cpu_reason       xi_cpu_reason,
                                                        BL_LILAC_RDD_CPU_METER_DTE           xi_cpu_meter,
                                                        rdpa_traffic_dir                     xi_direction,
                                                        BL_LILAC_RDD_BRIDGE_PORT_VECTOR_DTE  xi_src_port_mask )
{
    /* check the validity of the input parameters - CPU-RX reason */
    if ( xi_cpu_reason >= _RDD_CPU_REASON_TO_CPU_RX_QUEUE_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_REASON_ILLEGAL );
    }

    if ( xi_direction == rdpa_dir_ds )
    {
        f_bl_lilac_rdd_cpu_reason_to_cpu_rx_meter_ds ( xi_cpu_reason, xi_cpu_meter );
    }
    else
    {
        f_bl_lilac_rdd_cpu_reason_to_cpu_rx_meter_us ( xi_cpu_reason, xi_cpu_meter, xi_src_port_mask );
    }
    
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_meter_config ( BL_LILAC_RDD_CPU_METER_DTE  xi_cpu_meter,
                                                 uint16_t                    xi_average_rate,
                                                 uint16_t                    xi_burst_size,
                                                 rdpa_traffic_dir            xi_direction )
{
    RDD_CPU_RX_METER_TABLE_DTS  *cpu_meter_table_ptr;
    RDD_CPU_RX_METER_ENTRY_DTS  *cpu_meter_entry_ptr;
    static uint32_t             api_first_time_call[ 2 ] = { LILAC_RDD_TRUE, LILAC_RDD_TRUE };

    if ( xi_direction == rdpa_dir_ds )
    {
        cpu_meter_table_ptr = ( RDD_CPU_RX_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_CPU_RX_METER_TABLE_ADDRESS );
    }
    else
    {
        cpu_meter_table_ptr = ( RDD_CPU_RX_METER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_CPU_RX_METER_TABLE_ADDRESS );
    }

    cpu_meter_entry_ptr = &( cpu_meter_table_ptr->entry[ xi_cpu_meter ] );

    xi_burst_size = rdd_budget_to_alloc_unit ( xi_burst_size, LILAC_RDD_CPU_RX_METER_TIMER_PERIOD, 0 );

    RDD_CPU_RX_METER_ENTRY_BUDGET_LIMIT_WRITE ( xi_burst_size, cpu_meter_entry_ptr );

    xi_average_rate = rdd_budget_to_alloc_unit ( xi_average_rate, LILAC_RDD_CPU_RX_METER_TIMER_PERIOD, 0 );

    RDD_CPU_RX_METER_ENTRY_ALLOCATED_BUDGET_WRITE ( xi_average_rate, cpu_meter_entry_ptr );

    if ( api_first_time_call[ xi_direction ] )
    {
        rdd_timer_task_config ( xi_direction, LILAC_RDD_CPU_RX_METER_TIMER_PERIOD, CPU_RX_METER_BUDGET_ALLOCATE_CODE_ID );
        api_first_time_call[ xi_direction ] = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_meter_drop_counter_get ( BL_LILAC_RDD_CPU_METER_DTE  xi_cpu_meter,
                                                           rdpa_traffic_dir            xi_direction,
                                                           uint16_t                    *xo_drop_counter )
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error;

    rdd_error = rdd_2_bytes_counter_get ( CPU_RX_METERS_DROPPED_PACKETS_GROUP, xi_direction * BL_LILAC_RDD_CPU_METER_DISABLE + xi_cpu_meter, xo_drop_counter );

    return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_write_eth_packet ( uint8_t                    *xi_packet_ptr,
                                                     uint32_t                   xi_packet_size,
                                                     BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                                     uint8_t                    xi_wifi_ssid,
                                                     BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_CPU_WAKEUP  runner_cpu_wakeup_register;
    uint32_t                    packet_ddr_ptr;
#endif
    RDD_CPU_TX_DESCRIPTOR_DTS   *cpu_tx_descriptor_ptr;
    uint32_t                    cpu_tx_descriptor;
    uint32_t                    bpm_buffer_number;
    uint8_t                     cpu_tx_descriptor_valid;
    unsigned long               flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + g_cpu_tx_queue_write_ptr[ 2 ] );

    /* if the descriptor is valid then the CPU-TX queue is full and the packet should be dropped */
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_READ ( cpu_tx_descriptor_valid, cpu_tx_descriptor_ptr );

    if ( cpu_tx_descriptor_valid )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
    }

#if !defined(FIRMWARE_INIT)
    if ( fi_bl_drv_bpm_req_buffer ( DRV_BPM_SP_SPARE_0, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
    }

    packet_ddr_ptr = g_runner_ddr_base_addr + bpm_buffer_number * LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE + g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET;

    /* copy the packet from the supplied DDR buffer */
    MWRITE_BLK_8 ( ( uint8_t * )packet_ddr_ptr, xi_packet_ptr, xi_packet_size );

    g_dummy_read = *( ( ( uint8_t * )packet_ddr_ptr ) + xi_packet_size - 1 );
#else
    bpm_buffer_number = 0;
#endif

    /* write CPU-TX descriptor and validate it */
    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_PAYLOAD_OFFSET_L_WRITE ( ( g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET ) / 2 ) |
                        RDD_CPU_TX_DESCRIPTOR_BUFFER_NUMBER_L_WRITE ( bpm_buffer_number ) |
                        RDD_CPU_TX_DESCRIPTOR_1588_INDICATION_L_WRITE ( 0 );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_COMMAND_L_WRITE ( LILAC_RDD_CPU_TX_COMMAND_EGRESS_PORT_PACKET ) |
                        RDD_CPU_TX_DESCRIPTOR_PACKET_LENGTH_L_WRITE ( xi_packet_size + 4 ) |
                        RDD_CPU_TX_DESCRIPTOR_SRC_BRIDGE_PORT_L_WRITE ( SPARE_0_SRC_PORT ) |
                        RDD_CPU_TX_DESCRIPTOR_EMAC_L_WRITE ( xi_emac_id ) |
                        RDD_CPU_TX_DESCRIPTOR_QUEUE_L_WRITE ( xi_queue_id ) |
                        RDD_CPU_TX_DESCRIPTOR_VALID_L_WRITE ( LILAC_RDD_TRUE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ 2 ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ 2 ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;

#if !defined(FIRMWARE_INIT)
    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt = CPU_TX_PICO_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = CPU_TX_PICO_THREAD_NUMBER % 32;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


#if !defined(RDD_BASIC)
BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_write_gpon_packet ( uint8_t                              *xi_packet_ptr,
                                                      uint32_t                             xi_packet_size,
                                                      uint32_t                             xi_upstream_gem_flow,
                                                      RDD_WAN_CHANNEL_ID                   xi_wan_channel,
                                                      BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller,
                                                      BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue,
                                                      uint8_t                              xi_exclusive_packet )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_CPU_WAKEUP     runner_cpu_wakeup_register;
    uint32_t                       packet_ddr_ptr;
#endif
    RDD_CPU_TX_DESCRIPTOR_DTS      *cpu_tx_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS  *wan_tx_pointers_entry_ptr;
    uint32_t                       cpu_tx_descriptor;
    uint32_t                       bpm_buffer_number;
    uint8_t                        cpu_tx_descriptor_valid;
    unsigned long                  flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel ][ xi_rate_controller ][ xi_queue ] );

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + g_cpu_tx_queue_write_ptr[ 1 ] );

    /* if the descriptor is valid then the CPU-TX queue is full and the packet should be dropped */
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_READ ( cpu_tx_descriptor_valid, cpu_tx_descriptor_ptr );

    if ( cpu_tx_descriptor_valid )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
    }

#if !defined(FIRMWARE_INIT)
    if ( fi_bl_drv_bpm_req_buffer ( DRV_BPM_SP_EMAC0, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
    {
        if ( xi_exclusive_packet ) 
        {
            if ( fi_bl_drv_bpm_req_buffer ( DRV_BPM_SP_SPARE_0, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
            {
                f_rdd_unlock_irq ( &int_lock_irq, flags );
                return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
            }
        }
        else
        {
            f_rdd_unlock_irq ( &int_lock_irq, flags );
            return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
        }
    }

    packet_ddr_ptr = g_runner_ddr_base_addr + bpm_buffer_number * LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE + g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET;

    /* copy the packet from the supplied DDR buffer */
    MWRITE_BLK_8 ( ( uint8_t * )packet_ddr_ptr, xi_packet_ptr, xi_packet_size );

    g_dummy_read = *( ( ( uint8_t * )packet_ddr_ptr ) + xi_packet_size - 1 );
#else
    bpm_buffer_number = 0;
#endif

    /* write CPU-TX descriptor and validate it */
    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_TX_QUEUE_PTR_L_WRITE ( ( ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - WAN_TX_QUEUES_TABLE_ADDRESS ) / sizeof ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS ) ) ) |
                        RDD_CPU_TX_DESCRIPTOR_BUFFER_NUMBER_L_WRITE ( bpm_buffer_number ) |
                        RDD_CPU_TX_DESCRIPTOR_PAYLOAD_OFFSET_L_WRITE ( ( g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET ) / 2 );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_COMMAND_L_WRITE ( LILAC_RDD_CPU_TX_COMMAND_EGRESS_PORT_PACKET ) |
                        RDD_CPU_TX_DESCRIPTOR_PACKET_LENGTH_L_WRITE ( xi_packet_size + 4 ) |
                        RDD_CPU_TX_DESCRIPTOR_SRC_BRIDGE_PORT_L_WRITE ( SPARE_0_SRC_PORT ) |
                        RDD_CPU_TX_DESCRIPTOR_US_GEM_FLOW_L_WRITE ( xi_upstream_gem_flow ) |
                        RDD_CPU_TX_DESCRIPTOR_VALID_L_WRITE ( LILAC_RDD_TRUE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ 1 ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ 1 ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;

#if !defined(FIRMWARE_INIT)
    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt = CPU_TX_FAST_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = CPU_TX_FAST_THREAD_NUMBER % 32;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_wan_bridge ( uint8_t                       *xi_packet_ptr,
                                                              uint32_t                      xi_packet_size,
                                                              BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_src_bridge_port,
                                                              uint8_t                       xi_wifi_ssid,
                                                              uint8_t                       xi_exclusive_packet )
{
    RDD_CPU_TX_DESCRIPTOR_DTS   *cpu_tx_descriptor_ptr;
    uint32_t                    cpu_tx_descriptor;
    uint32_t                    bpm_buffer_number;
    uint32_t                    ih_class;
    uint8_t                     cpu_tx_descriptor_valid;
#if !defined(FIRMWARE_INIT)
    DRV_BPM_SP_USR              bpm_src_port;
    RUNNER_REGS_CFG_CPU_WAKEUP  runner_cpu_wakeup_register;
    uint32_t                    packet_ddr_ptr;
#endif
    unsigned long               flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + g_cpu_tx_queue_write_ptr[ 3 ] );

    /* if the descriptor is valid then the CPU-TX queue is full and the packet should be dropped */
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_READ ( cpu_tx_descriptor_valid, cpu_tx_descriptor_ptr );

    if ( cpu_tx_descriptor_valid )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
    }

#if !defined(FIRMWARE_INIT)

    if ( g_broadcom_switch_mode && ( xi_src_bridge_port >= BL_LILAC_RDD_LAN0_BRIDGE_PORT ) && ( xi_src_bridge_port <= BL_LILAC_RDD_LAN4_BRIDGE_PORT ) )
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( g_broadcom_switch_physical_port );
    }
    else
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( xi_src_bridge_port );
    }

    if ( fi_bl_drv_bpm_req_buffer ( bpm_src_port, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
    {
        if ( xi_exclusive_packet )
        {
            if ( fi_bl_drv_bpm_req_buffer ( DRV_BPM_SP_SPARE_0, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
            {
                f_rdd_unlock_irq ( &int_lock_irq, flags );
                return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
            }
        }
        else
        {
            f_rdd_unlock_irq ( &int_lock_irq, flags );
            return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
        }
    }

    packet_ddr_ptr = g_runner_ddr_base_addr + bpm_buffer_number * LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE +
                     g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET;

    /* copy the packet from the supplied DDR buffer */
    MWRITE_BLK_8 ( ( uint8_t * )packet_ddr_ptr, xi_packet_ptr, xi_packet_size );

    g_dummy_read = *( ( ( uint8_t * )packet_ddr_ptr ) + xi_packet_size - 1 );
#else
    bpm_buffer_number = 0;
#endif

    ih_class = f_rdd_bridge_port_to_class_id ( xi_src_bridge_port );

    /* write CPU-TX descriptor */
    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_IH_CLASS_L_WRITE ( ih_class ) |
                        RDD_CPU_TX_DESCRIPTOR_BUFFER_NUMBER_L_WRITE ( bpm_buffer_number ) |
                        RDD_CPU_TX_DESCRIPTOR_SSID_L_WRITE ( xi_wifi_ssid ) |
                        RDD_CPU_TX_DESCRIPTOR_ABS_FLAG_L_WRITE ( LILAC_RDD_FALSE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_COMMAND_L_WRITE ( LILAC_RDD_CPU_TX_COMMAND_BRIDGE_PACKET ) |
                        RDD_CPU_TX_DESCRIPTOR_PACKET_LENGTH_L_WRITE ( xi_packet_size + 4 ) |
                        RDD_CPU_TX_DESCRIPTOR_SRC_BRIDGE_PORT_L_WRITE ( xi_src_bridge_port ) |
                        RDD_CPU_TX_DESCRIPTOR_VALID_L_WRITE ( LILAC_RDD_TRUE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ 3 ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ 3 ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;

#if !defined(FIRMWARE_INIT)
    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt = CPU_TX_PICO_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = CPU_TX_PICO_THREAD_NUMBER % 32;

    RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_lan_bridge ( uint8_t   *xi_packet_ptr,
                                                              uint32_t  xi_packet_size,
                                                              uint32_t  xi_downstream_gem_flow )
{
    RDD_CPU_TX_DESCRIPTOR_DTS   *cpu_tx_descriptor_ptr;
    uint32_t                    cpu_tx_descriptor;
    uint32_t                    bpm_buffer_number;
    uint32_t                    ih_class;
    uint8_t                     cpu_tx_descriptor_valid;
#if !defined(FIRMWARE_INIT)
    DRV_BPM_SP_USR              bpm_src_port;
    RUNNER_REGS_CFG_CPU_WAKEUP  runner_cpu_wakeup_register;
    uint32_t                    packet_ddr_ptr;
#endif
    unsigned long               flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + g_cpu_tx_queue_write_ptr[ 0 ] );

    /* if the descriptor is valid then the CPU-TX queue is full and the packet should be dropped */
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_READ ( cpu_tx_descriptor_valid, cpu_tx_descriptor_ptr );

    if ( cpu_tx_descriptor_valid )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
    }

#if !defined(FIRMWARE_INIT)

    if ( ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_GPON ) || ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_EPON ) || ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH5 ) )
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( BL_LILAC_RDD_WAN0_BRIDGE_PORT ); // DSL
    }
    else if ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH4 )
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( BL_LILAC_RDD_LAN4_BRIDGE_PORT );
    }
    else if ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH0 )
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( BL_LILAC_RDD_LAN0_BRIDGE_PORT );
    }
    else
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_NOT_ALLOWED );
    }

    if ( fi_bl_drv_bpm_req_buffer ( bpm_src_port, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
    }

    packet_ddr_ptr = g_runner_ddr_base_addr + bpm_buffer_number * LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE +
                     g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET;

    /* copy the packet from the supplied DDR buffer */
    MWRITE_BLK_8 ( ( uint8_t * )packet_ddr_ptr, xi_packet_ptr, xi_packet_size );

    g_dummy_read = *( ( ( uint8_t * )packet_ddr_ptr ) + xi_packet_size - 1 );
#else
    bpm_buffer_number = 0;
#endif

    ih_class = f_rdd_bridge_port_to_class_id ( BL_LILAC_RDD_WAN0_BRIDGE_PORT ); // DSL

    /* write CPU-TX descriptor */
    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_IH_CLASS_L_WRITE ( ih_class ) |
                        RDD_CPU_TX_DESCRIPTOR_BUFFER_NUMBER_L_WRITE ( bpm_buffer_number ) |
                        RDD_CPU_TX_DESCRIPTOR_ABS_FLAG_L_WRITE ( LILAC_RDD_FALSE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_COMMAND_L_WRITE ( LILAC_RDD_CPU_TX_COMMAND_BRIDGE_PACKET ) |
                        RDD_CPU_TX_DESCRIPTOR_PACKET_LENGTH_L_WRITE ( xi_packet_size + 4 ) |
                        RDD_CPU_TX_DESCRIPTOR_DS_GEM_FLOW_L_WRITE ( xi_downstream_gem_flow ) |
                        RDD_CPU_TX_DESCRIPTOR_VALID_L_WRITE ( LILAC_RDD_TRUE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ 0 ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ 0 ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;

#if !defined(FIRMWARE_INIT)
    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt = CPU_TX_FAST_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = CPU_TX_FAST_THREAD_NUMBER % 32;

    RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_wan_interworking ( uint8_t                       *xi_packet_ptr,
                                                                    uint32_t                      xi_packet_size,
                                                                    BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_src_bridge_port,
                                                                    uint8_t                       xi_wifi_ssid,
                                                                    BL_LILAC_RDD_SUBNET_ID_DTE    xi_subnet_id,
                                                                    uint8_t                       xi_exclusive_packet )
{
    RDD_CPU_TX_DESCRIPTOR_DTS   *cpu_tx_descriptor_ptr;
    uint32_t                    cpu_tx_descriptor;
    uint32_t                    bpm_buffer_number;
    uint32_t                    ih_class;
    uint8_t                     cpu_tx_descriptor_valid;
#if !defined(FIRMWARE_INIT)
    DRV_BPM_SP_USR              bpm_src_port;
    RUNNER_REGS_CFG_CPU_WAKEUP  runner_cpu_wakeup_register;
    uint32_t                    packet_ddr_ptr;
#endif
    unsigned long               flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + g_cpu_tx_queue_write_ptr[ 3 ] );

    /* if the descriptor is valid then the CPU-TX queue is full and the packet should be dropped */
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_READ ( cpu_tx_descriptor_valid, cpu_tx_descriptor_ptr );

    if ( cpu_tx_descriptor_valid )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
    }

#if !defined(FIRMWARE_INIT)

    bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( xi_src_bridge_port );

    if ( fi_bl_drv_bpm_req_buffer ( bpm_src_port, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
    {
        if ( xi_exclusive_packet )
        {
            if ( fi_bl_drv_bpm_req_buffer ( DRV_BPM_SP_SPARE_0, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
            {
                f_rdd_unlock_irq ( &int_lock_irq, flags );
                return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
            }
        }
        else
        {
            f_rdd_unlock_irq ( &int_lock_irq, flags );
            return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
        }
    }

    packet_ddr_ptr = g_runner_ddr_base_addr + bpm_buffer_number * LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE +
                     g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET;

    /* copy the packet from the supplied DDR buffer */
    MWRITE_BLK_8 ( ( uint8_t * )packet_ddr_ptr, xi_packet_ptr, xi_packet_size );

    g_dummy_read = *( ( ( uint8_t * )packet_ddr_ptr ) + xi_packet_size - 1 );
#else
    bpm_buffer_number = 0;
#endif

    ih_class = f_rdd_bridge_port_to_class_id ( xi_src_bridge_port );

    /* write CPU-TX descriptor */
    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_IH_CLASS_L_WRITE ( ih_class ) |
                        RDD_CPU_TX_DESCRIPTOR_BUFFER_NUMBER_L_WRITE ( bpm_buffer_number ) |
                        RDD_CPU_TX_DESCRIPTOR_SSID_L_WRITE ( xi_wifi_ssid ) |
                        RDD_CPU_TX_DESCRIPTOR_ABS_FLAG_L_WRITE ( LILAC_RDD_FALSE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_COMMAND_L_WRITE ( LILAC_RDD_CPU_TX_COMMAND_INTERWORKING_PACKET ) |
                        RDD_CPU_TX_DESCRIPTOR_PACKET_LENGTH_L_WRITE ( xi_packet_size + 4 ) |
                        RDD_CPU_TX_DESCRIPTOR_SRC_BRIDGE_PORT_L_WRITE ( xi_src_bridge_port ) |
                        RDD_CPU_TX_DESCRIPTOR_SUBNET_ID_L_WRITE ( xi_subnet_id ) |
                        RDD_CPU_TX_DESCRIPTOR_VALID_L_WRITE ( LILAC_RDD_TRUE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ 3 ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ 3 ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;

#if !defined(FIRMWARE_INIT)
    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt = CPU_TX_PICO_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = CPU_TX_PICO_THREAD_NUMBER % 32;

    RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_send_packet_to_lan_interworking ( uint8_t                   *xi_packet_ptr,
                                                                    uint32_t                  xi_packet_size,
                                                                    uint32_t                  xi_downstream_gem_flow,
                                                                    BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id,
                                                                    uint8_t                   xi_wifi_ssid )
{
    RDD_CPU_TX_DESCRIPTOR_DTS   *cpu_tx_descriptor_ptr;
    uint32_t                    cpu_tx_descriptor;
    uint32_t                    bpm_buffer_number;
    uint32_t                    ih_class;
    uint8_t                     cpu_tx_descriptor_valid;
#if !defined(FIRMWARE_INIT)
    DRV_BPM_SP_USR              bpm_src_port;
    RUNNER_REGS_CFG_CPU_WAKEUP  runner_cpu_wakeup_register;
    uint32_t                    packet_ddr_ptr;
#endif
    unsigned long               flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    cpu_tx_descriptor_ptr = ( RDD_CPU_TX_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + g_cpu_tx_queue_write_ptr[ 0 ] );

    /* if the descriptor is valid then the CPU-TX queue is full and the packet should be dropped */
    RDD_CPU_TX_DESCRIPTOR_CORE_VALID_READ ( cpu_tx_descriptor_valid, cpu_tx_descriptor_ptr );

    if ( cpu_tx_descriptor_valid )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_QUEUE_FULL );
    }

#if !defined(FIRMWARE_INIT)

    if ( ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_GPON ) || ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_EPON ) || ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH5 ) )
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( BL_LILAC_RDD_WAN0_BRIDGE_PORT ); // DSL
    }
    else if ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH4 )
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( BL_LILAC_RDD_LAN4_BRIDGE_PORT );
    }
    else if ( g_wan_physical_port == BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH0 )
    {
        bpm_src_port = f_rdd_bridge_port_to_bpm_src_port ( BL_LILAC_RDD_LAN0_BRIDGE_PORT );
    }
    else
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_CPU_TX_NOT_ALLOWED );
    }

    if ( fi_bl_drv_bpm_req_buffer ( bpm_src_port, ( uint32_t * )&bpm_buffer_number ) != DRV_BPM_ERROR_NO_ERROR )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_BPM_ALLOC_FAIL );
    }

    packet_ddr_ptr = g_runner_ddr_base_addr + bpm_buffer_number * LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE +
                     g_ddr_headroom_size + LILAC_RDD_PACKET_DDR_OFFSET;

    /* copy the packet from the supplied DDR buffer */
    MWRITE_BLK_8 ( ( uint8_t * )packet_ddr_ptr, xi_packet_ptr, xi_packet_size );

    g_dummy_read = *( ( ( uint8_t * )packet_ddr_ptr ) + xi_packet_size - 1 );
#else
    bpm_buffer_number = 0;
#endif

    ih_class = f_rdd_bridge_port_to_class_id ( BL_LILAC_RDD_WAN0_BRIDGE_PORT ); // DSL

    /* write CPU-TX descriptor */
    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_IH_CLASS_L_WRITE ( ih_class ) |
                        RDD_CPU_TX_DESCRIPTOR_BUFFER_NUMBER_L_WRITE ( bpm_buffer_number ) |
                        RDD_CPU_TX_DESCRIPTOR_SSID_L_WRITE ( xi_wifi_ssid ) |
                        RDD_CPU_TX_DESCRIPTOR_ABS_FLAG_L_WRITE ( LILAC_RDD_FALSE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr + 4, cpu_tx_descriptor );

    cpu_tx_descriptor = RDD_CPU_TX_DESCRIPTOR_COMMAND_L_WRITE ( LILAC_RDD_CPU_TX_COMMAND_INTERWORKING_PACKET ) |
                        RDD_CPU_TX_DESCRIPTOR_PACKET_LENGTH_L_WRITE ( xi_packet_size + 4 ) |
                        RDD_CPU_TX_DESCRIPTOR_EMAC_L_WRITE ( xi_emac_id ) |
                        RDD_CPU_TX_DESCRIPTOR_DS_GEM_FLOW_L_WRITE ( xi_downstream_gem_flow ) |
                        RDD_CPU_TX_DESCRIPTOR_VALID_L_WRITE ( LILAC_RDD_TRUE );

    MWRITE_32( ( uint8_t * )cpu_tx_descriptor_ptr, cpu_tx_descriptor );

    /* increment and wrap around if needed the write pointer of the CPU-TX queue */
    g_cpu_tx_queue_write_ptr[ 0 ] += LILAC_RDD_CPU_TX_DESCRIPTOR_SIZE;
    g_cpu_tx_queue_write_ptr[ 0 ] &= LILAC_RDD_CPU_TX_QUEUE_SIZE_MASK;

#if !defined(FIRMWARE_INIT)
    /* send asynchronous wakeup command to the CPU-TX thread in the Runner */
    runner_cpu_wakeup_register.req_trgt = CPU_TX_FAST_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = CPU_TX_FAST_THREAD_NUMBER % 32;

    RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}

BL_LILAC_RDD_ERROR_DTE bl_lilac_rdd_cpu_tx_get_released_skb_counter ( uint32_t  *xo_skb_counter )
{
    *xo_skb_counter = g_cpu_tx_released_skb_counter;
    return ( BL_LILAC_RDD_OK );
}


#endif /* !defined(RDD_BASIC) */


BL_LILAC_RDD_ERROR_DTE rdd_service_queue_pm_counters_get ( uint32_t                           xi_service_queue,
                                                          RDD_SERVICE_QUEUE_PM_COUNTERS_DTE  *xo_pm_counters )
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error;
 
    if ( xi_service_queue > BL_LILAC_RDD_QUEUE_7 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }

    rdd_error = rdd_4_bytes_counter_get ( SERVICE_QUEUE_PACKET_GROUP, xi_service_queue, &xo_pm_counters->good_tx_packet );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( SERVICE_QUEUE_PACKET_GROUP, SERVICE_QUEUE_DROP_COUNTER_GROUP_OFFSET + xi_service_queue, &xo_pm_counters->error_tx_packets_discard );

    return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_flow_pm_counters_get ( uint32_t                                xi_flow_id,
                                                  BL_LILAC_RDD_FLOW_PM_COUNTERS_TYPE_DTE  xi_flow_pm_counters_type,
                                                  bdmf_boolean                            xi_clear_counters,
                                                  BL_LILAC_RDD_FLOW_PM_COUNTERS_DTE       *xo_pm_counters )
{
    BL_LILAC_RDD_FLOW_PM_COUNTERS_DTE  *pm_counters_buffer_ptr;
    BL_LILAC_RDD_ERROR_DTE             rdd_error;
    unsigned long                      flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    /* read pm counters of a single port and reset its value */
    rdd_error = f_rdd_cpu_tx_send_message ( xi_flow_pm_counters_type, FAST_RUNNER_A, RUNNER_PRIVATE_0_OFFSET, xi_flow_id, 0, 0, BL_LILAC_RDD_WAIT );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( rdd_error );
    }

    pm_counters_buffer_ptr = ( BL_LILAC_RDD_FLOW_PM_COUNTERS_DTE * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + PM_COUNTERS_BUFFER_ADDRESS );

#if defined(DSL_63138) || defined(DSL_63148)
    xo_pm_counters->good_rx_packet           = swap4bytes (pm_counters_buffer_ptr->good_rx_packet);
    xo_pm_counters->good_rx_bytes            = swap4bytes (pm_counters_buffer_ptr->good_rx_bytes);
    xo_pm_counters->good_tx_packet           = swap4bytes (pm_counters_buffer_ptr->good_tx_packet);
    xo_pm_counters->good_tx_bytes            = swap4bytes (pm_counters_buffer_ptr->good_tx_bytes);

    xo_pm_counters->error_rx_packets_discard = swap2bytes (pm_counters_buffer_ptr->error_rx_packets_discard);
    xo_pm_counters->error_tx_packets_discard = swap2bytes (pm_counters_buffer_ptr->error_tx_packets_discard);
#else /* !DSL */
    xo_pm_counters->good_rx_packet           = pm_counters_buffer_ptr->good_rx_packet;
    xo_pm_counters->good_rx_bytes            = pm_counters_buffer_ptr->good_rx_bytes;
    xo_pm_counters->good_tx_packet           = pm_counters_buffer_ptr->good_tx_packet;
    xo_pm_counters->good_tx_bytes            = pm_counters_buffer_ptr->good_tx_bytes;

    xo_pm_counters->error_rx_packets_discard = pm_counters_buffer_ptr->error_rx_packets_discard;
    xo_pm_counters->error_tx_packets_discard = pm_counters_buffer_ptr->error_tx_packets_discard;
#endif /* !DSL */

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_bridge_port_pm_counters_get ( BL_LILAC_RDD_BRIDGE_PORT_DTE              xi_bridge_port,
                                                         bdmf_boolean                              xi_clear_counters,
                                                         BL_LILAC_RDD_BRIDGE_PORT_PM_COUNTERS_DTE  *xo_pm_counters )
{
    BL_LILAC_RDD_BRIDGE_PORT_PM_COUNTERS_DTE  *pm_counters_buffer_ptr;
    BL_LILAC_RDD_ERROR_DTE                    rdd_error;
    unsigned long                             flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    /* read pm counters of a single port and reset its value */
    rdd_error = f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_BRIDGE_PORT_PM_COUNTERS_GET, FAST_RUNNER_A, RUNNER_PRIVATE_0_OFFSET, xi_bridge_port, 0, 0, BL_LILAC_RDD_WAIT );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( rdd_error );
    }

    pm_counters_buffer_ptr = ( BL_LILAC_RDD_BRIDGE_PORT_PM_COUNTERS_DTE * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + PM_COUNTERS_BUFFER_ADDRESS );

    if ( BL_LILAC_RDD_IS_WAN_BRIDGE_PORT(xi_bridge_port) ) // DSL
    {
        xo_pm_counters->rx_valid = pm_counters_buffer_ptr->rx_valid;
        xo_pm_counters->tx_valid = pm_counters_buffer_ptr->tx_valid;
    }
    else if ( xi_bridge_port == BL_LILAC_RDD_WAN_IPTV_BRIDGE_PORT )
    {
        xo_pm_counters->rx_valid = pm_counters_buffer_ptr->rx_valid;
        xo_pm_counters->tx_valid = 0;
    }
    else
    {
        xo_pm_counters->rx_valid = 0;
        xo_pm_counters->tx_valid = 0;
    }
    xo_pm_counters->error_rx_bpm_congestion   = pm_counters_buffer_ptr->error_rx_bpm_congestion;
    xo_pm_counters->bridge_filtered_packets   = pm_counters_buffer_ptr->bridge_filtered_packets;
    xo_pm_counters->bridge_tx_packets_discard = pm_counters_buffer_ptr->bridge_tx_packets_discard;

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_crc_error_counter_get ( BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_bridge_port,
                                                   bdmf_boolean                  xi_clear_counters,
                                                   uint16_t                      *xo_crc_counter )
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error;

    if ( xi_bridge_port == BL_LILAC_RDD_WAN_IPTV_BRIDGE_PORT )
    {
        rdd_error = rdd_2_bytes_counter_get ( WAN_BRIDGE_PORT_GROUP, WAN_CRC_ERROR_IPTV_COUNTER_OFFSET, xo_crc_counter );
    }
    else
    {
        rdd_error = rdd_2_bytes_counter_get ( WAN_BRIDGE_PORT_GROUP, WAN_CRC_ERROR_NORMAL_COUNTER_OFFSET, xo_crc_counter );
    }

    return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_subnet_counters_get ( BL_LILAC_RDD_SUBNET_ID_DTE           xi_subnet_id,
                                                 BL_LILAC_RDD_SUBNET_PM_COUNTERS_DTE  *xo_subnet_counters )
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error;

    if ( xi_subnet_id == BL_LILAC_RDD_SUBNET_BRIDGE )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_SUBNET_ID );
    }

    rdd_error = rdd_4_bytes_counter_get ( SUBNET_RX_GROUP, xi_subnet_id, &xo_subnet_counters->good_rx_packet );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_4_bytes_counter_get ( SUBNET_TX_GROUP, xi_subnet_id, &xo_subnet_counters->good_tx_packet );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_4_bytes_counter_get ( SUBNET_RX_BYTES_GROUP, xi_subnet_id, &xo_subnet_counters->good_rx_bytes );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_4_bytes_counter_get ( SUBNET_TX_BYTES_GROUP, xi_subnet_id, &xo_subnet_counters->good_tx_bytes );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( SUBNET_RX_GROUP, SUBNET_DROPPED_PACKETS_SUB_GROUP_OFFSET + xi_subnet_id, &xo_subnet_counters->rx_dropped_packet );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( SUBNET_TX_GROUP, SUBNET_DROPPED_PACKETS_SUB_GROUP_OFFSET + xi_subnet_id, &xo_subnet_counters->tx_dropped_packet );

    return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_various_counters_get ( rdpa_traffic_dir                   xi_direction,
                                                  uint32_t                           xi_various_counters_mask,
                                                  bdmf_boolean                       xi_clear_counters,
                                                  BL_LILAC_RDD_VARIOUS_COUNTERS_DTE  *xo_various_counters )
{
    uint32_t                ingress_filter_idx;
    uint32_t                l4_filter_idx;
    uint32_t                counters_group;
    BL_LILAC_RDD_ERROR_DTE  rdd_error;

    if ( xi_direction == rdpa_dir_ds )
    {
        counters_group = DOWNSTREAM_VARIOUS_PACKETS_GROUP;
    }
    else
    {
        counters_group = UPSTREAM_VARIOUS_PACKETS_GROUP;
    }

    if ( xi_various_counters_mask & ETHERNET_FLOW_ACTION_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, ETHERNET_FLOW_DROP_ACTION_COUNTER_OFFSET, &xo_various_counters->eth_flow_action_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & VLAN_SWITCHING_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, VLAN_SWITCHING_DROP_COUNTER_OFFSET, &xo_various_counters->vlan_switching_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & SA_LOOKUP_FAILURE_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, SA_LOOKUP_FAILURE_DROP_COUNTER_OFFSET, &xo_various_counters->sa_lookup_failure_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & DA_LOOKUP_FAILURE_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, DA_LOOKUP_FAILURE_DROP_COUNTER_OFFSET, &xo_various_counters->da_lookup_failure_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & SA_ACTION_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, SA_ACTION_DROP_COUNTER_OFFSET, &xo_various_counters->sa_action_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & DA_ACTION_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, DA_ACTION_DROP_COUNTER_OFFSET, &xo_various_counters->da_action_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & FORWARDING_MATRIX_DISABLED_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, FORWARDING_MATRIX_DISABLED_DROP_COUNTER_OFFSET, &xo_various_counters->forwarding_matrix_disabled_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & CONNECTION_ACTION_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, CONNECTION_ACTION_DROP_COUNTER_OFFSET, &xo_various_counters->connection_action_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & TPID_DETECT_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, TPID_DETECT_DROP_COUNTER_OFFSET, &xo_various_counters->tpid_detect_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & INVALID_SUBNET_IP_DROP_COUNTER_MASK )
    {
        rdd_error = rdd_2_bytes_counter_get ( counters_group, INVALID_SUBNET_IP_DROP_COUNTER_OFFSET, &xo_various_counters->invalid_subnet_ip_drop );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }
    }

    if ( xi_various_counters_mask & INGRESS_FILTERS_DROP_COUNTER_MASK )
    {
        for ( ingress_filter_idx = 0; ingress_filter_idx < BL_LILAC_RDD_STOP_FILTER_NUMBER; ingress_filter_idx++ )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, INGRESS_FILTER_DROP_SUB_GROUP_OFFSET + ingress_filter_idx, &xo_various_counters->ingress_filters_drop[ ingress_filter_idx ] );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }
    }

    if ( xi_various_counters_mask & IP_VALIDATION_FILTER_DROP_COUNTER_MASK )
    {
        for ( ingress_filter_idx = 0; ingress_filter_idx < 2; ingress_filter_idx++ )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, INGRESS_FILTER_IP_VALIDATIOH_GROUP_OFFSET + ingress_filter_idx, &xo_various_counters->ip_validation_filter_drop[ ingress_filter_idx ] );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }
    }

    if ( xi_various_counters_mask & LAYER4_FILTERS_DROP_COUNTER_MASK )
    {
        for ( l4_filter_idx = 0; l4_filter_idx <= RDD_LAYER4_FILTER_UNKNOWN; l4_filter_idx++ )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, LAYER4_FILTER_DROP_SUB_GROUP_OFFSET + l4_filter_idx, &xo_various_counters->layer4_filters_drop[ l4_filter_idx ] );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }
    }

    if ( xi_direction == rdpa_dir_ds )
    {
        if ( xi_various_counters_mask & INVALID_LAYER2_PROTOCOL_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, INVALID_LAYER2_PROTOCOL_DROP_COUNTER_OFFSET, &xo_various_counters->invalid_layer2_protocol_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & FIREWALL_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, FIREWALL_DROP_COUNTER_OFFSET, &xo_various_counters->firewall_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & DST_MAC_NON_ROUTER_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, DST_MAC_NON_ROUTER_COUNTER_OFFSET, &xo_various_counters->dst_mac_non_router_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & EMAC_LOOPBACK_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, EMAC_LOOPBACK_DROP_COUNTER, &xo_various_counters->emac_loopback_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & IPTV_LAYER3_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, IPTV_LAYER3_DROP_COUNTER_OFFSET, &xo_various_counters->iptv_layer3_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & DOWNSTREAM_POLICERS_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, DOWNSTREAM_POLICERS_DROP_COUNTER_OFFSET, &xo_various_counters->downstream_policers_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & DUAL_STACK_LITE_CONGESTION_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, DUAL_STACK_LITE_CONGESTION_DROP_COUNTER_OFFSET, &xo_various_counters->dual_stack_lite_congestion_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        rdd_error = rdd_2_bytes_counter_get ( counters_group, DOWNSTREAM_PARALLEL_PROCESSING_NO_SLAVE_WAIT_OFFSET, &xo_various_counters->ds_parallel_processing_no_avialable_slave );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }

        rdd_error = rdd_2_bytes_counter_get ( counters_group, DOWNSTREAM_PARALLEL_PROCESSING_REORDER_WAIT_OFFSET, &xo_various_counters->ds_parallel_processing_reorder_slaves );

        if ( rdd_error != BL_LILAC_RDD_OK )
        {
            return ( rdd_error );
        }

        if ( xi_various_counters_mask & ABSOLUTE_ADDRESS_LIST_OVERFLOW_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, ABSOLUTE_ADDRESS_LIST_OVERFLOW_OFFSET, &xo_various_counters->absolute_address_list_overflow_drop );
            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }
    }
    else
    {
        if ( xi_various_counters_mask & ACL_OUI_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, ACL_OUI_DROP_COUNTER_OFFSET, &xo_various_counters->acl_oui_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & ACL_L2_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, ACL_LAYER2_DROP_COUNTER_OFFSET, &xo_various_counters->acl_l2_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & ACL_L3_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, ACL_LAYER3_DROP_COUNTER_OFFSET, &xo_various_counters->acl_l3_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & LOCAL_SWITCHING_CONGESTION_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, LAN_ENQUEUE_CONGESTION_COUNTER_OFFSET, &xo_various_counters->local_switching_congestion );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }

        if ( xi_various_counters_mask & EPON_DDR_QUEUEU_DROP_COUNTER_MASK )
        {
            rdd_error = rdd_2_bytes_counter_get ( counters_group, EPON_DDR_QUEUES_COUNTER_OFFSET, &xo_various_counters->us_ddr_queue_drop );

            if ( rdd_error != BL_LILAC_RDD_OK )
            {
                return ( rdd_error );
            }
        }
    }

    return ( BL_LILAC_RDD_OK );
}


#ifdef G9991
BL_LILAC_RDD_ERROR_DTE rdd_g9991_counters_get ( RDD_G9991_PM_COUNTERS_DTE *xo_g9991_counters)
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error;

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_DFC_FRAME_ERROR_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->dfc_frame_error_counter );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_DFC_FRAME_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->dfc_frame_counter );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_DATA_FRAME_ERROR_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->data_frame_error_counter );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_ILLEGAL_SID_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->illegal_sid_error_counter );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_LENGTH_ERROR_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->length_error_counter );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_REASSEMBLY_ERROR_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->reassembly_error_counter );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_BBH_ERROR_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->bbh_error_counter );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }

    rdd_error = rdd_2_bytes_counter_get ( G9991_GLOBAL_GROUP, US_CONSEQUENT_DROP_G9991_GLOBAL_GROUP_OFFSET, &xo_g9991_counters->consequent_drop );

    if ( rdd_error != BL_LILAC_RDD_OK )
    {
        return ( rdd_error );
    }


    return ( BL_LILAC_RDD_OK );

}
#endif


BL_LILAC_RDD_ERROR_DTE rdd_ring_init ( uint32_t  xi_ring_id,
                                       uint32_t  *xi_ring_address,
                                       uint32_t  xi_number_of_entries,
                                       uint32_t  xi_size_of_entry,
                                       uint32_t  xi_interrupt_id )
{
    RDD_RING_DESCRIPTORS_TABLE_DTS  *ring_table_ptr;
    RDD_RING_DESCRIPTOR_DTS         *ring_descriptor_ptr;


    ring_table_ptr = ( RDD_RING_DESCRIPTORS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + RING_DESCRIPTORS_TABLE_ADDRESS );

    ring_descriptor_ptr = &( ring_table_ptr->entry[ xi_ring_id ] );

    RDD_RING_DESCRIPTOR_ENTRIES_COUNTER_WRITE ( 0, ring_descriptor_ptr );
    RDD_RING_DESCRIPTOR_SIZE_OF_ENTRY_WRITE ( xi_size_of_entry, ring_descriptor_ptr );
    RDD_RING_DESCRIPTOR_NUMBER_OF_ENTRIES_WRITE ( xi_number_of_entries, ring_descriptor_ptr );
    RDD_RING_DESCRIPTOR_INTERRUPT_ID_WRITE ( 1 << xi_interrupt_id, ring_descriptor_ptr );
    RDD_RING_DESCRIPTOR_RING_POINTER_WRITE ( xi_ring_address, ring_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ring_destroy ( uint32_t  xi_ring_id )
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error = BL_LILAC_RDD_OK;
    unsigned long           flags;

    f_rdd_lock_irq ( &int_lock_irq, &flags );

#if !defined(FIRMWARE_INIT)
    rdd_error = f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_RING_DESTROY, PICO_RUNNER_A, RUNNER_PRIVATE_0_OFFSET, xi_ring_id, 0, 0, BL_LILAC_RDD_WAIT );
#endif
    f_rdd_unlock_irq ( &int_lock_irq, flags );

    return ( rdd_error );
}

#if defined(DSL_63138) || defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE rdd_cpu_rx_interrupt_coalescing_config( uint32_t xi_ring_id,
                                                               uint32_t xi_timeout_us,
                                                               uint32_t xi_max_packet_count )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET           runner_timer_target_register;
    RUNNER_REGS_CFG_CPU_WAKEUP             runner_cpu_wakeup_register;
#endif
    RDD_INTERRUPT_COALESCING_CONFIG_TABLE_DTS *ic_table_ptr;
    RDD_INTERRUPT_COALESCING_CONFIG_DTS       *ic_entry_ptr;
    uint16_t                                  *ic_timer_period;
    static uint32_t                           api_first_time_call = LILAC_RDD_TRUE;

    if( xi_ring_id > RDD_INTERRUPT_COALESCING_CONFIG_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_CPU_RX_REASON_ILLEGAL );
    }

    ic_table_ptr = ( RDD_INTERRUPT_COALESCING_CONFIG_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + INTERRUPT_COALESCING_CONFIG_TABLE_ADDRESS );
    ic_entry_ptr =  &( ic_table_ptr->entry[ xi_ring_id ] );

    RDD_INTERRUPT_COALESCING_CONFIG_CONFIGURED_TIMEOUT_WRITE( xi_timeout_us, ic_entry_ptr );
    RDD_INTERRUPT_COALESCING_CONFIG_CONFIGURED_MAX_PACKET_COUNT_WRITE( xi_max_packet_count, ic_entry_ptr );

    if ( api_first_time_call )
    {
        ic_timer_period = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_COMMON_0_OFFSET ) + INTERRUPT_COALESCING_TIMER_PERIOD_ADDRESS );
#if defined(FIRMWARE_INIT)
        MWRITE_16( ic_timer_period, 100 );
#else
        MWRITE_16( ic_timer_period, INTERRUPT_COALESCING_TIMER_PERIOD );

        RUNNER_REGS_0_CFG_TIMER_TARGET_READ ( runner_timer_target_register );
        runner_timer_target_register.timer_5_7 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_5_7_PICO_CORE_VALUE;
        RUNNER_REGS_0_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

        /* activate the interrupt coalescing task */
        runner_cpu_wakeup_register.req_trgt = CPU_RX_INTERRUPT_COALESCING_THREAD_NUMBER / 32;
        runner_cpu_wakeup_register.thread_num = CPU_RX_INTERRUPT_COALESCING_THREAD_NUMBER % 32;
        RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif
        api_first_time_call = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}
#endif

#if defined(DSL_63138) || defined(DSL_63148)

#if defined(CC_RDD_CPU_SPDSVC_DEBUG)
bdmf_sysb  g_spdsvc_setup_sysb_ptr = NULL;
#endif

static inline uint32_t f_rdd_spdsvc_kbps_to_tokens(uint32_t xi_kbps)
{
    return ( uint32_t )( ( (1000/8) * xi_kbps ) / SPDSVC_TIMER_HZ );
}

static inline uint32_t f_rdd_spdsvc_mbs_to_bucket_size(uint32_t xi_mbs)
{
    uint32_t bucket_size = xi_mbs;

    if(bucket_size < SPDSVC_BUCKET_SIZE_MIN)
    {
        bucket_size = SPDSVC_BUCKET_SIZE_MIN;
    }

    return bucket_size;
}

static inline RDD_SPDSVC_CONTEXT_ENTRY_DTS *f_rdd_spdsvc_get_context_ptr( rdpa_traffic_dir xi_direction )
{
    if( xi_direction == rdpa_dir_us )
    {
        return ( RDD_SPDSVC_CONTEXT_ENTRY_DTS * )
            ( DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_SPDSVC_CONTEXT_TABLE_ADDRESS );
    }
    else
    {
        return ( RDD_SPDSVC_CONTEXT_ENTRY_DTS * )
            ( DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_SPDSVC_CONTEXT_TABLE_ADDRESS );
    }
}

static BL_LILAC_RDD_ERROR_DTE f_rdd_spdsvc_config( uint32_t xi_kbps,
                                                   uint32_t xi_mbs,
                                                   uint32_t xi_copies,
                                                   uint32_t xi_total_length,
                                                   rdpa_traffic_dir xi_direction )
{
    RDD_SPDSVC_CONTEXT_ENTRY_DTS *spdsvc_context_ptr = f_rdd_spdsvc_get_context_ptr( xi_direction );
    RDD_SPDSVC_CONTEXT_ENTRY_DTS spdsvc_context;

    RDD_SPDSVC_CONTEXT_ENTRY_BBH_DESCRIPTOR_0_WRITE( 0, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_BBH_DESCRIPTOR_1_WRITE( 0, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_SKB_FREE_INDEX_WRITE( 0xFFFF, spdsvc_context_ptr );

    spdsvc_context.tokens = f_rdd_spdsvc_kbps_to_tokens( xi_kbps );
    spdsvc_context.bucket_size = f_rdd_spdsvc_mbs_to_bucket_size( spdsvc_context.tokens + xi_mbs );

    RDD_SPDSVC_CONTEXT_ENTRY_COPIES_IN_TRANSIT_WRITE( 0, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TOTAL_COPIES_WRITE( xi_copies, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TOTAL_LENGTH_WRITE( xi_total_length, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TOKENS_WRITE( spdsvc_context.tokens, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_BUCKET_SIZE_WRITE( spdsvc_context.bucket_size, spdsvc_context_ptr );

    RDD_SPDSVC_CONTEXT_ENTRY_TX_QUEUE_DISCARDS_WRITE( 0, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TX_QUEUE_WRITES_WRITE( 0, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TX_QUEUE_READS_WRITE( 0, spdsvc_context_ptr );

#if defined(CC_RDD_CPU_SPDSVC_DEBUG) && !defined(FIRMWARE_INIT)
    __rdd_cpu_trace("\n%s: kbps %u (tokens %u), mbs %u (bucket_size %u), copies %u, total_length %u, direction %s\n\n",
                    __FUNCTION__, xi_kbps, spdsvc_context.tokens, xi_mbs, spdsvc_context.bucket_size, xi_copies, xi_total_length,
                    ( xi_direction == rdpa_dir_us ) ? "US" : "DS");
#endif
    return ( BL_LILAC_RDD_OK );
}

#define RDD_CPU_SPDSVC_IS_RUNNING(__copies_in_transit, __total_copies)  \
    ( ((__copies_in_transit) > 4) || (__total_copies) )

BL_LILAC_RDD_ERROR_DTE rdd_spdsvc_config( uint32_t xi_kbps,
                                          uint32_t xi_mbs,
                                          uint32_t xi_copies,
                                          uint32_t xi_total_length )
{
    RDD_SPDSVC_CONTEXT_ENTRY_DTS *spdsvc_context_ptr;
    uint32_t copies_in_transit;
    uint32_t total_copies;
    int ret;

    spdsvc_context_ptr = f_rdd_spdsvc_get_context_ptr( rdpa_dir_us );
    RDD_SPDSVC_CONTEXT_ENTRY_COPIES_IN_TRANSIT_READ( copies_in_transit, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TOTAL_COPIES_READ( total_copies, spdsvc_context_ptr );

    if( RDD_CPU_SPDSVC_IS_RUNNING(copies_in_transit, total_copies) )
    {
        spdsvc_context_ptr = f_rdd_spdsvc_get_context_ptr( rdpa_dir_ds );
        RDD_SPDSVC_CONTEXT_ENTRY_COPIES_IN_TRANSIT_READ( copies_in_transit, spdsvc_context_ptr );
        RDD_SPDSVC_CONTEXT_ENTRY_TOTAL_COPIES_READ( total_copies, spdsvc_context_ptr );

        if( RDD_CPU_SPDSVC_IS_RUNNING(copies_in_transit, total_copies) )
        {
            return ( BL_LILAC_RDD_ERROR_SPDSVC_RESOURCE_BUSY );
        }
    }

    ret = f_rdd_spdsvc_config( xi_kbps, xi_mbs, xi_copies, xi_total_length, rdpa_dir_us );
    if( ret == BL_LILAC_RDD_OK )
    {
        ret = f_rdd_spdsvc_config( xi_kbps, xi_mbs, xi_copies, xi_total_length, rdpa_dir_ds );
    }

    return ret;
}

static BL_LILAC_RDD_ERROR_DTE f_rdd_spdsvc_get_tx_result( uint8_t *xo_running_p,
                                                          uint32_t *xo_tx_packets_p,
                                                          uint32_t *xo_tx_discards_p,
                                                          rdpa_traffic_dir xi_direction )
{
    RDD_SPDSVC_CONTEXT_ENTRY_DTS *spdsvc_context_ptr = f_rdd_spdsvc_get_context_ptr( xi_direction );
    RDD_SPDSVC_CONTEXT_ENTRY_DTS spdsvc_context;

    RDD_SPDSVC_CONTEXT_ENTRY_COPIES_IN_TRANSIT_READ( spdsvc_context.copies_in_transit, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TOTAL_COPIES_READ( spdsvc_context.total_copies, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TX_QUEUE_READS_READ( spdsvc_context.tx_queue_reads, spdsvc_context_ptr );
    RDD_SPDSVC_CONTEXT_ENTRY_TX_QUEUE_DISCARDS_READ( spdsvc_context.tx_queue_discards, spdsvc_context_ptr );

    *xo_running_p &= RDD_CPU_SPDSVC_IS_RUNNING(spdsvc_context.copies_in_transit, spdsvc_context.total_copies) ? 1 : 0;
    *xo_tx_packets_p += spdsvc_context.tx_queue_reads;
    *xo_tx_discards_p += spdsvc_context.tx_queue_discards;

    return ( BL_LILAC_RDD_OK );
}

BL_LILAC_RDD_ERROR_DTE rdd_spdsvc_get_tx_result( uint8_t *xo_running_p,
                                                 uint32_t *xo_tx_packets_p,
                                                 uint32_t *xo_tx_discards_p )
{
    int ret;

    *xo_running_p = 1;
    *xo_tx_packets_p = 0;
    *xo_tx_discards_p = 0;

    ret = f_rdd_spdsvc_get_tx_result( xo_running_p, xo_tx_packets_p, xo_tx_discards_p, rdpa_dir_us );
    if( ret == BL_LILAC_RDD_OK )
    {
        ret = f_rdd_spdsvc_get_tx_result( xo_running_p, xo_tx_packets_p, xo_tx_discards_p, rdpa_dir_ds );
    }

    return ret;
}

#if defined(CONFIG_BCM_SPDSVC_SUPPORT) && !defined(RDD_BASIC)
static void f_rdd_spdsvc_initialize_structs( rdpa_traffic_dir xi_direction )
{
    RDD_SPDSVC_CONTEXT_ENTRY_DTS *spdsvc_context_ptr = f_rdd_spdsvc_get_context_ptr( xi_direction );

    memset( spdsvc_context_ptr, 0, sizeof( RDD_SPDSVC_CONTEXT_ENTRY_DTS ) );

#if defined(FIRMWARE_INIT)
    RDD_SPDSVC_CONTEXT_ENTRY_TIMER_PERIOD_WRITE( 100, spdsvc_context_ptr );

    rdd_spdsvc_config( 100000, 2000, 10, 1514 );
#else
#if defined(CC_RDD_CPU_SPDSVC_DEBUG)
    __rdd_cpu_trace("\n\tspdsvc_context_ptr %p\n\n", spdsvc_context_ptr);
#endif
    RDD_SPDSVC_CONTEXT_ENTRY_TIMER_PERIOD_WRITE( SPDSVC_TIMER_PERIOD, spdsvc_context_ptr );

    RDD_SPDSVC_CONTEXT_ENTRY_SKB_FREE_INDEX_WRITE( 0xFFFF, spdsvc_context_ptr );
#endif
}

BL_LILAC_RDD_ERROR_DTE rdd_spdsvc_initialize( void )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET           runner_timer_target_register;
    RUNNER_REGS_CFG_CPU_WAKEUP             runner_cpu_wakeup_register;
#endif
    static uint32_t                        api_first_time_call = LILAC_RDD_TRUE;

    if ( api_first_time_call )
    {
        f_rdd_spdsvc_initialize_structs( rdpa_dir_us );
        f_rdd_spdsvc_initialize_structs( rdpa_dir_ds );

#if !defined(FIRMWARE_INIT)
        RUNNER_REGS_1_CFG_TIMER_TARGET_READ ( runner_timer_target_register );
        runner_timer_target_register.timer_5_7 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_5_7_MAIN_CORE_VALUE;
        RUNNER_REGS_1_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

        /* activate the speed service tasks */
        runner_cpu_wakeup_register.req_trgt = US_SPDSVC_THREAD_NUMBER / 32;
        runner_cpu_wakeup_register.thread_num = US_SPDSVC_THREAD_NUMBER % 32;
        RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );

        runner_cpu_wakeup_register.req_trgt = DS_SPDSVC_THREAD_NUMBER / 32;
        runner_cpu_wakeup_register.thread_num = DS_SPDSVC_THREAD_NUMBER % 32;
        RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#endif
        api_first_time_call = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}
#endif

#endif /* DSL */

#if defined(CONFIG_BCM_PKTRUNNER_GSO)
BL_LILAC_RDD_ERROR_DTE rdd_gso_counters_get ( RDD_GSO_COUNTERS_ENTRY_DTS *xo_gso_counters_ptr )
{
    RDD_GSO_CONTEXT_ENTRY_DTS   *gso_context_ptr;
    unsigned long               flags;

    gso_context_ptr = ( RDD_GSO_CONTEXT_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_GSO_CONTEXT_TABLE_ADDRESS );

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    RDD_GSO_CONTEXT_ENTRY_RX_PACKETS_READ( xo_gso_counters_ptr->rx_packets, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_RX_OCTETS_READ( xo_gso_counters_ptr->rx_octets, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TX_PACKETS_READ( xo_gso_counters_ptr->tx_packets, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TX_OCTETS_READ( xo_gso_counters_ptr->tx_octets, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_DROPPED_PACKETS_READ( xo_gso_counters_ptr->dropped_packets, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_DROPPED_NO_BPM_BUFFER_READ( xo_gso_counters_ptr->dropped_no_bpm_buffer, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_DROPPED_PARSE_FAILED_READ( xo_gso_counters_ptr->dropped_parse_failed, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_DROPPED_LINEAR_LENGTH_INVALID_READ( xo_gso_counters_ptr->dropped_linear_length_invalid, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_QUEUE_FULL_READ( xo_gso_counters_ptr->queue_full, gso_context_ptr );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}

BL_LILAC_RDD_ERROR_DTE rdd_gso_context_get ( RDD_GSO_CONTEXT_ENTRY_DTS *xo_gso_context_ptr )
{
    RDD_GSO_CONTEXT_ENTRY_DTS   *gso_context_ptr;
    unsigned long               flags;

    gso_context_ptr = ( RDD_GSO_CONTEXT_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_GSO_CONTEXT_TABLE_ADDRESS );

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    RDD_GSO_CONTEXT_ENTRY_RX_BBH_DESCRIPTOR_0_READ( xo_gso_context_ptr->rx_bbh_descriptor_0, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_RX_BBH_DESCRIPTOR_1_READ( xo_gso_context_ptr->rx_bbh_descriptor_1, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TX_BBH_DESCRIPTOR_0_READ( xo_gso_context_ptr->tx_bbh_descriptor_0, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TX_BBH_DESCRIPTOR_1_READ( xo_gso_context_ptr->tx_bbh_descriptor_1, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_SUMMARY_READ( xo_gso_context_ptr->summary, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IP_HEADER_OFFSET_READ( xo_gso_context_ptr->ip_header_offset, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IP_HEADER_LENGTH_READ( xo_gso_context_ptr->ip_header_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IP_TOTAL_LENGTH_READ( xo_gso_context_ptr->ip_total_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IP_ID_READ( xo_gso_context_ptr->ip_id, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IP_FRAGMENT_OFFSET_READ( xo_gso_context_ptr->ip_fragment_offset, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IP_FLAGS_READ( xo_gso_context_ptr->ip_flags, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IP_PROTOCOL_READ( xo_gso_context_ptr->ip_protocol, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IPV4_CSUM_READ( xo_gso_context_ptr->ipv4_csum, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_PACKET_HEADER_LENGTH_READ( xo_gso_context_ptr->packet_header_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_SEG_COUNT_READ( xo_gso_context_ptr->seg_count, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_NR_FRAGS_READ( xo_gso_context_ptr->nr_frags, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_FRAG_INDEX_READ( xo_gso_context_ptr->frag_index, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TCP_UDP_HEADER_OFFSET_READ( xo_gso_context_ptr->tcp_udp_header_offset, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TCP_UDP_HEADER_LENGTH_READ( xo_gso_context_ptr->tcp_udp_header_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TCP_UDP_TOTAL_LENGTH_READ( xo_gso_context_ptr->tcp_udp_total_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TCP_SEQUENCE_READ( xo_gso_context_ptr->tcp_sequence, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TCP_FLAGS_READ( xo_gso_context_ptr->tcp_flags, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_VERSION_READ( xo_gso_context_ptr->version, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TCP_UDP_CSUM_READ( xo_gso_context_ptr->tcp_udp_csum, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_MSS_READ( xo_gso_context_ptr->mss, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_MSS_ADJUST_READ( xo_gso_context_ptr->mss_adjust, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_SEG_LENGTH_READ( xo_gso_context_ptr->seg_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_SEG_BYTES_LEFT_READ( xo_gso_context_ptr->seg_bytes_left, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_MAX_CHUNK_LENGTH_READ( xo_gso_context_ptr->max_chunk_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_CHUNK_BYTES_LEFT_READ( xo_gso_context_ptr->chunk_bytes_left, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_PAYLOAD_BYTES_LEFT_READ( xo_gso_context_ptr->payload_bytes_left, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_PAYLOAD_PTR_READ( xo_gso_context_ptr->payload_ptr, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_PAYLOAD_LENGTH_READ( xo_gso_context_ptr->payload_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_LINEAR_LENGTH_READ( xo_gso_context_ptr->linear_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TX_PACKET_PTR_READ( xo_gso_context_ptr->tx_packet_ptr, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_TX_PACKET_LENGTH_READ( xo_gso_context_ptr->tx_packet_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_UDP_FIRST_PACKET_LENGTH_READ( xo_gso_context_ptr->udp_first_packet_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_UDP_FIRST_PACKET_PTR_READ( xo_gso_context_ptr->udp_first_packet_ptr, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_UDP_FIRST_PACKET_BUFFER_NUMBER_READ( xo_gso_context_ptr->udp_first_packet_buffer_number, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_BPM_BUFFER_NUMBER_READ( xo_gso_context_ptr->bpm_buffer_number, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_PACKET_LENGTH_READ( xo_gso_context_ptr->packet_length, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_IPV6_IP_ID_READ( xo_gso_context_ptr->ipv6_ip_id, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_AUTH_STATE_3_READ( xo_gso_context_ptr->auth_state_3, gso_context_ptr );
    RDD_GSO_CONTEXT_ENTRY_DEBUG_0_READ( xo_gso_context_ptr->debug_0, gso_context_ptr );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}

BL_LILAC_RDD_ERROR_DTE rdd_gso_desc_get ( RDD_GSO_DESC_ENTRY_DTS *xo_gso_desc_ptr )
{
    RDD_GSO_DESC_ENTRY_DTS  *gso_desc_ptr;
    unsigned long           flags;
#if 0
    int                     nr_frags;
#endif

    gso_desc_ptr = ( RDD_GSO_DESC_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_GSO_DESC_TABLE_ADDRESS );

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    RDD_GSO_DESC_ENTRY_DATA_READ( xo_gso_desc_ptr->data, gso_desc_ptr );
    RDD_GSO_DESC_ENTRY_LEN_READ( xo_gso_desc_ptr->len, gso_desc_ptr );
    RDD_GSO_DESC_ENTRY_LINEAR_LEN_READ( xo_gso_desc_ptr->linear_len, gso_desc_ptr );
    RDD_GSO_DESC_ENTRY_MSS_READ( xo_gso_desc_ptr->mss, gso_desc_ptr );
    RDD_GSO_DESC_ENTRY_IS_ALLOCATED_READ( xo_gso_desc_ptr->is_allocated, gso_desc_ptr );
    RDD_GSO_DESC_ENTRY_NR_FRAGS_READ( xo_gso_desc_ptr->nr_frags, gso_desc_ptr );

#if 0
    for ( nr_frags=0; nr_frags < xo_gso_desc_ptr->nr_frags; nr_frags++ )
    {
        RDD_GSO_DESC_ENTRY_FRAG_DATA_READ( xo_gso_desc_ptr->frag_data[nr_frags], gso_desc_ptr, nr_frags );
        RDD_GSO_DESC_ENTRY_FRAG_LEN_READ( xo_gso_desc_ptr->frag_len[nr_frags], gso_desc_ptr, nr_frags );
    }
#endif

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


#endif /* CONFIG_BCM_PKTRUNNER_GSO */

BL_LILAC_RDD_ERROR_DTE rdd_cpu_tx_free_skb_timer_config ( void )
{
    static uint32_t               api_first_time_call = LILAC_RDD_TRUE;

    if ( api_first_time_call )
    {
#if !defined(FIRMWARE_INIT)

        rdd_timer_task_config ( rdpa_dir_us, FREE_SKB_INDEX_TIMER_PERIOD, FREE_SKB_INDEX_ALLOCATE_CODE_ID );
        rdd_timer_task_config ( rdpa_dir_ds, FREE_SKB_INDEX_TIMER_PERIOD, FREE_SKB_INDEX_ALLOCATE_CODE_ID );
#else
        rdd_timer_task_config ( rdpa_dir_us, 100, FREE_SKB_INDEX_ALLOCATE_CODE_ID );
        rdd_timer_task_config ( rdpa_dir_ds, 100, FREE_SKB_INDEX_ALLOCATE_CODE_ID );
#endif

        api_first_time_call = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}

#if defined(DSL_63138) || defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE rdd_cso_counters_get ( RDD_CSO_COUNTERS_ENTRY_DTS *xo_cso_counters_ptr )
{
    RDD_CSO_CONTEXT_ENTRY_DTS   *cso_context_ptr;
    unsigned long               flags;

    cso_context_ptr = ( RDD_CSO_CONTEXT_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_CSO_CONTEXT_TABLE_ADDRESS );

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    RDD_CSO_CONTEXT_ENTRY_GOOD_CSUM_PACKETS_READ( xo_cso_counters_ptr->good_csum_packets, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_NO_CSUM_PACKETS_READ( xo_cso_counters_ptr->no_csum_packets, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_BAD_IPV4_HDR_CSUM_PACKETS_READ( xo_cso_counters_ptr->bad_ipv4_hdr_csum_packets, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_BAD_TCP_UDP_CSUM_PACKETS_READ( xo_cso_counters_ptr->bad_tcp_udp_csum_packets, cso_context_ptr );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}

BL_LILAC_RDD_ERROR_DTE rdd_cso_context_get ( RDD_CSO_CONTEXT_ENTRY_DTS *xo_cso_context_ptr )
{
    RDD_CSO_CONTEXT_ENTRY_DTS   *cso_context_ptr;
    unsigned long               flags;

    cso_context_ptr = ( RDD_CSO_CONTEXT_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_CSO_CONTEXT_TABLE_ADDRESS );

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    RDD_CSO_CONTEXT_ENTRY_SUMMARY_READ( xo_cso_context_ptr->summary, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_PACKET_LENGTH_READ( xo_cso_context_ptr->packet_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_LINEAR_LENGTH_READ( xo_cso_context_ptr->linear_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_PACKET_HEADER_LENGTH_READ( xo_cso_context_ptr->packet_header_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_IP_PROTOCOL_READ( xo_cso_context_ptr->ip_protocol, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_IP_HEADER_OFFSET_READ( xo_cso_context_ptr->ip_header_offset, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_IP_HEADER_LENGTH_READ( xo_cso_context_ptr->ip_header_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_IP_TOTAL_LENGTH_READ( xo_cso_context_ptr->ip_total_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_IPV4_CSUM_READ( xo_cso_context_ptr->ipv4_csum, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_TCP_UDP_HEADER_OFFSET_READ( xo_cso_context_ptr->tcp_udp_header_offset, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_TCP_UDP_HEADER_LENGTH_READ( xo_cso_context_ptr->tcp_udp_header_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_TCP_UDP_TOTAL_LENGTH_READ( xo_cso_context_ptr->tcp_udp_total_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_TCP_UDP_CSUM_READ( xo_cso_context_ptr->tcp_udp_csum, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_MAX_CHUNK_LENGTH_READ( xo_cso_context_ptr->max_chunk_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_CHUNK_BYTES_LEFT_READ( xo_cso_context_ptr->chunk_bytes_left, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_NR_FRAGS_READ( xo_cso_context_ptr->nr_frags, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_FRAG_INDEX_READ( xo_cso_context_ptr->frag_index, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_FRAG_LEN_READ( xo_cso_context_ptr->frag_len, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_FRAG_DATA_READ( xo_cso_context_ptr->frag_data, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_GOOD_CSUM_PACKETS_READ( xo_cso_context_ptr->good_csum_packets, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_NO_CSUM_PACKETS_READ( xo_cso_context_ptr->no_csum_packets, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_BAD_IPV4_HDR_CSUM_PACKETS_READ( xo_cso_context_ptr->bad_ipv4_hdr_csum_packets, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_BAD_TCP_UDP_CSUM_PACKETS_READ( xo_cso_context_ptr->bad_tcp_udp_csum_packets, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_FAIL_CODE_READ( xo_cso_context_ptr->fail_code, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_DMA_SYNC_READ( xo_cso_context_ptr->dma_sync, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_SEG_LENGTH_READ( xo_cso_context_ptr->seg_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_SEG_BYTES_LEFT_READ( xo_cso_context_ptr->seg_bytes_left, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_PAYLOAD_LENGTH_READ( xo_cso_context_ptr->payload_length, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_PAYLOAD_BYTES_LEFT_READ( xo_cso_context_ptr->payload_bytes_left, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_PAYLOAD_PTR_READ( xo_cso_context_ptr->payload_ptr, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_DDR_PAYLOAD_OFFSET_READ( xo_cso_context_ptr->ddr_payload_offset, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_DDR_SRC_ADDRESS_READ( xo_cso_context_ptr->ddr_src_address, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_SAVED_IH_BUFFER_NUMBER_READ( xo_cso_context_ptr->saved_ih_buffer_number, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_SAVED_CSUM32_RET_ADDR_READ( xo_cso_context_ptr->saved_csum32_ret_addr, cso_context_ptr );
    RDD_CSO_CONTEXT_ENTRY_SAVED_R16_READ( xo_cso_context_ptr->saved_r16, cso_context_ptr );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}
#endif


