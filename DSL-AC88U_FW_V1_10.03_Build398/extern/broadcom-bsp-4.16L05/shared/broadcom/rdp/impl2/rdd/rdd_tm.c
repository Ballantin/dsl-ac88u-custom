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


/******************************************************************************/
/*                                                                            */
/*                            Global Variables                                */
/*                                                                            */
/******************************************************************************/

#if defined(FIRMWARE_INIT)
extern uint32_t  DsConnectionTableBase;
extern uint32_t  UsConnectionTableBase;
extern uint32_t  ContextTableBase;
extern uint32_t  IPTVTableBase;
extern uint32_t  IPTVContextTableBase;
extern uint32_t  FirewallRulesMapTable;
extern uint32_t  FirewallRulesTable;
#endif

extern uint32_t                                     g_runner_ddr_base_addr;
extern uint32_t                                     g_runner_tables_ptr;
extern uint32_t                                     g_ddr_headroom_size;
extern uint32_t                                     g_rate_controllers_pool_idx;
extern uint32_t                                     g_downstream_rate_shaper[ RDD_DS_RATE_SHAPERS_TABLE_SIZE ];
extern uint32_t                                     g_bbh_peripheral_eth_rx[];
extern uint8_t                                      g_broadcom_switch_mode;
extern BL_LILAC_RDD_BRIDGE_PORT_DTE                 g_broadcom_switch_physical_port;
extern RDD_WAN_TX_POINTERS_TABLE_DTS                *wan_tx_pointers_table_ptr;
extern BL_LILAC_RDD_WAN_PHYSICAL_PORT_DTE           g_wan_physical_port;
extern BL_LILAC_RDD_LOCK_CRITICAL_SECTION_FP_IRQ_DTE    f_rdd_lock_irq;
extern BL_LILAC_RDD_UNLOCK_CRITICAL_SECTION_FP_IRQ_DTE  f_rdd_unlock_irq;
extern bdmf_fastlock                                    int_lock_irq;

static uint32_t                                     api_first_time_call_ds_rate_limiter = LILAC_RDD_TRUE;

#if !defined(RDD_BASIC) && (defined(DSL_63138) || defined(DSL_63148))
static uint16_t ds_free_packet_descriptors_pool_guaranteed_pool_size = DS_FREE_PACKET_DESCRIPTOR_POOL_MIN_GUARANTEED_POOL_SIZE;
static uint16_t ds_free_packet_descriptors_pool_non_guaranteed_pool_size = RDD_DS_FREE_PACKET_DESCRIPTORS_POOL_SIZE - DS_FREE_PACKET_DESCRIPTOR_POOL_MIN_GUARANTEED_POOL_SIZE;

static BL_LILAC_RDD_ERROR_DTE rdd_tm_ds_free_packet_descriptors_pool_size_update (void);
#endif

/******************************************************************************/
/*                                                                            */
/*   set the timer for upstream rate control budget allocation, in each       */
/*   iteration 16 rate controllers get budget, this API should be called only */
/*   when at least one wan channel is working at rate control scheduling.     */
/*                                                                            */
/******************************************************************************/
static BL_LILAC_RDD_ERROR_DTE f_rdd_us_rate_controller_timer_set ( void )
{
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET           runner_timer_target_register;
    RUNNER_REGS_CFG_CPU_WAKEUP             runner_cpu_wakeup_register;

    RUNNER_REGS_1_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

    runner_timer_target_register.timer_0_2 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_0_2_MAIN_CORE_VALUE;

    RUNNER_REGS_1_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );
#endif

    bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );
#if !defined(FIRMWARE_INIT)
    /* budget allocation is taking place every 500 micro second, 16 rate controllers each cycle allocation */
    RDD_BRIDGE_CONFIGURATION_REGISTER_US_RATE_CONTROLLER_TIMER_WRITE ( BL_LILAC_RDD_US_RATE_CONTROL_TIMER_INTERVAL / 8 - 1, bridge_cfg_register );

    runner_cpu_wakeup_register.req_trgt = RATE_CONTROLLER_BUDGET_ALLOCATOR_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = RATE_CONTROLLER_BUDGET_ALLOCATOR_THREAD_NUMBER % 32;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#else
    RDD_BRIDGE_CONFIGURATION_REGISTER_US_RATE_CONTROLLER_TIMER_WRITE ( 8, bridge_cfg_register );
#endif
    return ( BL_LILAC_RDD_OK );
}


static BL_LILAC_RDD_ERROR_DTE f_ds_rate_limiter_config ( RDD_RATE_LIMITER_ID_DTE  xi_rate_limiter_id,
                                                         RDD_RATE_LIMIT_PARAMS    *xi_budget )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET          runner_timer_target_register;
#endif
    RDD_DS_RATE_LIMITER_TABLE_DTS            *rate_limiter_table_ptr;
    RDD_RATE_LIMITER_REMAINDER_TABLE_DTS  *rate_limiter_remainder_table_ptr;
    RDD_RATE_LIMITER_REMAINDER_ENTRY_DTS  *rate_limiter_remainder_entry_ptr;
    RDD_RATE_LIMITER_ENTRY_DTS            *rate_limiter_entry_ptr;
    uint32_t                              exponent_index;
    uint32_t                              rate;
    uint32_t                              limit;
    uint8_t                               remainder;
    uint32_t                              exponent_list[] = { 0, EMAC_RATE_LIMITER_EXPONENT };

    rate_limiter_table_ptr = ( RDD_DS_RATE_LIMITER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_LIMITER_TABLE_ADDRESS );
    rate_limiter_remainder_table_ptr = ( RDD_RATE_LIMITER_REMAINDER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + RATE_LIMITER_REMAINDER_TABLE_ADDRESS );

    rate_limiter_entry_ptr = &( rate_limiter_table_ptr->entry[ xi_rate_limiter_id ] );
    rate_limiter_remainder_entry_ptr = &( rate_limiter_remainder_table_ptr->entry[ xi_rate_limiter_id ] );

    /* devise mantissa and exponent of budget limit and write them to memory */
    exponent_index = rdd_get_exponent ( xi_budget->limit, 15, 2, exponent_list );

    limit = xi_budget->limit >> exponent_list[ exponent_index ];

    RDD_RATE_LIMITER_ENTRY_BUDGET_LIMIT_WRITE ( limit, rate_limiter_entry_ptr );
    RDD_RATE_LIMITER_ENTRY_BUDGET_LIMIT_EXP_WRITE ( exponent_index, rate_limiter_entry_ptr );

    /* devise mantissa and exponent of allocated budget and write them to memory */
    rate = rdd_budget_to_alloc_unit ( xi_budget->rate, EMAC_RATE_LIMITER_TIMER_PERIOD * 4, 0 );

    /* take into account the remainder to improve accuracy */
    remainder = rate & 0x3; 
    RDD_RATE_LIMITER_REMAINDER_ENTRY_ALLOCATED_BUDGET_WRITE( remainder, rate_limiter_remainder_entry_ptr );
    RDD_RATE_LIMITER_REMAINDER_ENTRY_CURRENT_BUDGET_WRITE( 0, rate_limiter_remainder_entry_ptr );

    rate = rate >> 2;

    exponent_index = rdd_get_exponent ( rate, 15, 2, exponent_list );

    rate >>= exponent_list[ exponent_index ];

    RDD_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_WRITE ( rate, rate_limiter_entry_ptr );
    RDD_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_EXP_WRITE ( exponent_index, rate_limiter_entry_ptr );

    if ( api_first_time_call_ds_rate_limiter )
    {
#if !defined(FIRMWARE_INIT)
        RUNNER_REGS_0_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

        runner_timer_target_register.timer_4_6 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_4_6_PICO_CORE_VALUE;

        RUNNER_REGS_0_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

        rdd_timer_task_config ( rdpa_dir_ds, EMAC_RATE_LIMITER_TIMER_PERIOD, DOWNSTREAM_RATE_LIMITER_BUDGET_ALLOCATE_CODE_ID );
#else
        rdd_timer_task_config ( rdpa_dir_ds, 400, DOWNSTREAM_RATE_LIMITER_BUDGET_ALLOCATE_CODE_ID );
#endif

        api_first_time_call_ds_rate_limiter = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}


static BL_LILAC_RDD_ERROR_DTE f_rdd_wan_channel_0_7_config ( RDD_WAN_CHANNEL_ID           xi_wan_channel_id,
                                                             RDD_WAN_CHANNEL_SCHEDULE     xi_schedule,
                                                             RDD_US_PEAK_SCHEDULING_MODE  xi_peak_scheduling_mode )
{
#if defined(DSL_63138) || defined(DSL_63148)
    BL_LILAC_RDD_WAN_PHYSICAL_PORT_DTE  wan_physical_port = (xi_wan_channel_id == RDD_WAN1_CHANNEL_BASE)
        ? BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH0 : BL_LILAC_RDD_WAN_PHYSICAL_PORT_DSL;
#endif
    RDD_WAN_CHANNELS_0_7_TABLE_DTS      *wan_channels_table_ptr;
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS  *wan_channel_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS       *wan_tx_pointers_entry_ptr;
    uint32_t                            rate_controller_id;
    uint32_t                            queue_id;

    wan_channels_table_ptr = ( RDD_WAN_CHANNELS_0_7_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + WAN_CHANNELS_0_7_TABLE_ADDRESS );

    wan_channel_descriptor_ptr = &( wan_channels_table_ptr->entry[ xi_wan_channel_id ] );

    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_SCHEDULE_WRITE ( xi_schedule, wan_channel_descriptor_ptr );
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_PEAK_SCHEDULING_MODE_WRITE ( xi_peak_scheduling_mode, wan_channel_descriptor_ptr );
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_ACK_PENDING_WRITE ( LILAC_RDD_TRUE, wan_channel_descriptor_ptr );
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_ACK_PENDING_EPON_WRITE ( LILAC_RDD_TRUE, wan_channel_descriptor_ptr );

#if defined(DSL_63138) || defined(DSL_63148)
    switch ( wan_physical_port )
    {
    case BL_LILAC_RDD_WAN_PHYSICAL_PORT_DSL :

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_BBH_DESTINATION_WRITE ( ( BBH_PERIPHERAL_DSL_TX + ( (xi_wan_channel_id - RDD_WAN0_CHANNEL_BASE) << 7 ) ), wan_channel_descriptor_ptr );
        break;

	case BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH0:

	    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_BBH_DESTINATION_WRITE ( BBH_PERIPHERAL_ETH0_TX, wan_channel_descriptor_ptr );
		break;

    default:
        break;
    }
#else
    switch ( g_wan_physical_port )
    {
    case BL_LILAC_RDD_WAN_PHYSICAL_PORT_GPON:
    case BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH5:
    case BL_LILAC_RDD_WAN_PHYSICAL_PORT_EPON:

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_BBH_DESTINATION_WRITE ( ( BBH_PERIPHERAL_GPON_TX + ( xi_wan_channel_id << 7 ) ), wan_channel_descriptor_ptr );
        break;

    case BL_LILAC_RDD_WAN_PHYSICAL_PORT_ETH4:

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_BBH_DESTINATION_WRITE ( BBH_PERIPHERAL_ETH4_TX, wan_channel_descriptor_ptr );
        break;

    default:
        break;
	}
#endif

    for ( rate_controller_id = BL_LILAC_RDD_RATE_CONTROLLER_0; rate_controller_id <= BL_LILAC_RDD_RATE_CONTROLLER_31; rate_controller_id++ )
    {
        for ( queue_id = 0; queue_id < RDD_US_RATE_CONTROLLER_DESCRIPTOR_TX_QUEUE_ADDR_NUMBER; queue_id++  )
        {
            /* write the allocated wan channel descriptor address to the wan tx pointers table */
            wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ rate_controller_id ][ queue_id ] );

            wan_tx_pointers_entry_ptr->wan_channel_ptr = WAN_CHANNELS_0_7_TABLE_ADDRESS + xi_wan_channel_id * sizeof ( RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS );
        }
    }

    return ( BL_LILAC_RDD_OK );
}


static BL_LILAC_RDD_ERROR_DTE f_rdd_wan_channel_8_39_config ( RDD_WAN_CHANNEL_ID           xi_wan_channel_id,
                                                              RDD_WAN_CHANNEL_SCHEDULE     xi_schedule,
                                                              RDD_US_PEAK_SCHEDULING_MODE  xi_peak_scheduling_mode )
{
    RDD_WAN_CHANNELS_8_39_TABLE_DTS      *wan_channels_table_ptr;
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS  *wan_channel_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS        *wan_tx_pointers_entry_ptr;
    uint32_t                             rate_controller_id;
    uint32_t                             queue_id;

    wan_channels_table_ptr = ( RDD_WAN_CHANNELS_8_39_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + WAN_CHANNELS_8_39_TABLE_ADDRESS );

    wan_channel_descriptor_ptr = &( wan_channels_table_ptr->entry[ xi_wan_channel_id - RDD_WAN_CHANNEL_8 ] );

    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_SCHEDULE_WRITE ( xi_schedule, wan_channel_descriptor_ptr );
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_PEAK_SCHEDULING_MODE_WRITE ( xi_peak_scheduling_mode, wan_channel_descriptor_ptr );
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_ACK_PENDING_WRITE ( LILAC_RDD_TRUE, wan_channel_descriptor_ptr );
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_ACK_PENDING_EPON_WRITE ( LILAC_RDD_TRUE, wan_channel_descriptor_ptr );
#if defined(DSL_63138) || defined(DSL_63148)
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_BBH_DESTINATION_WRITE ( ( BBH_PERIPHERAL_DSL_TX + ( (xi_wan_channel_id - RDD_WAN0_CHANNEL_BASE) << 7 ) ), wan_channel_descriptor_ptr );
#else
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_BBH_DESTINATION_WRITE ( ( BBH_PERIPHERAL_GPON_TX + ( xi_wan_channel_id << 7 ) ), wan_channel_descriptor_ptr );
#endif

    for ( rate_controller_id = BL_LILAC_RDD_RATE_CONTROLLER_0; rate_controller_id <= BL_LILAC_RDD_RATE_CONTROLLER_3; rate_controller_id++ )
    {
        for ( queue_id = 0; queue_id < RDD_US_RATE_CONTROLLER_DESCRIPTOR_TX_QUEUE_ADDR_NUMBER; queue_id++  )
        {
            /* write the allocated wan channel descriptor address to the wan tx pointers table */
            wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ rate_controller_id ][ queue_id ] );

            wan_tx_pointers_entry_ptr->wan_channel_ptr = WAN_CHANNELS_8_39_TABLE_ADDRESS + ( xi_wan_channel_id - RDD_WAN_CHANNEL_8 ) * sizeof ( RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS );
        }
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE f_rdd_tm_service_queues_initialize ( void )
{
    RDD_DS_SERVICE_QUEUES_DESCRIPTORS_TABLE_DTS  *lan_enqueue_service_queue_descriptor_ptr;
    RDD_DS_SERVICE_QUEUE_DESCRIPTOR_DTS          *service_queue_descriptor_entry_ptr;
    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS       *ds_service_tm_descriptor_ptr;
    uint32_t                                     tx_queue;
    uint16_t                                     *cpu_rx_pd_ingress_queue_ptr;

    lan_enqueue_service_queue_descriptor_ptr = RDD_DS_SERVICE_QUEUES_DESCRIPTORS_TABLE_PTR ();

    for ( tx_queue = 0; tx_queue < RDD_DS_SERVICE_QUEUES_DESCRIPTORS_TABLE_SIZE; tx_queue++ )
    {
        service_queue_descriptor_entry_ptr = &( lan_enqueue_service_queue_descriptor_ptr->entry[ tx_queue ] );

        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_QUEUE_MASK_WRITE ( 1 << tx_queue , service_queue_descriptor_entry_ptr );
        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_PACKET_COUNTER_WRITE ( 0 , service_queue_descriptor_entry_ptr );
        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( 128 , service_queue_descriptor_entry_ptr );
        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_HEAD_PTR_WRITE ( 0, service_queue_descriptor_entry_ptr );
        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_TAIL_PTR_WRITE ( 0, service_queue_descriptor_entry_ptr );
        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_RATE_LIMITER_ID_WRITE ( RDD_RATE_LIMITER_IDLE, service_queue_descriptor_entry_ptr );
        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_PROFILE_PTR_WRITE ( 0, service_queue_descriptor_entry_ptr );
    }

    ds_service_tm_descriptor_ptr = ( RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_SERVICE_TM_DESCRIPTOR_ADDRESS );

    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_SERVICE_QUEUES_STATUS_WRITE ( 0, ds_service_tm_descriptor_ptr );
    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_RATE_LIMITER_STATUS_WRITE ( 0xff, ds_service_tm_descriptor_ptr );
    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_OVERALL_RATE_LIMITER_MODE_WRITE ( 0, ds_service_tm_descriptor_ptr );

    cpu_rx_pd_ingress_queue_ptr = ( uint16_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + CPU_RX_PD_INGRESS_QUEUE_PTR_ADDRESS );

    MWRITE_16( cpu_rx_pd_ingress_queue_ptr, CPU_RX_PD_INGRESS_QUEUE_ADDRESS );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_tm_service_queue_config ( BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id,
                                                        uint16_t                   xi_packet_threshold,
                                                        RDD_RATE_LIMITER_ID_DTE    xi_rate_limiter_id,
                                                        rdd_queue_profile          xi_profile_id )
{
#ifndef G9991
    RDD_DS_SERVICE_QUEUES_DESCRIPTORS_TABLE_DTS  *lan_enqueue_service_queue_descriptor_ptr;
    RDD_DS_SERVICE_QUEUE_DESCRIPTOR_DTS          *service_queue_descriptor_entry_ptr;
    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS       *ds_service_tm_descriptor_ptr;
    uint16_t                                     queue_profile_address;

    lan_enqueue_service_queue_descriptor_ptr = RDD_DS_SERVICE_QUEUES_DESCRIPTORS_TABLE_PTR ();

    service_queue_descriptor_entry_ptr = &( lan_enqueue_service_queue_descriptor_ptr->entry[ xi_queue_id ] );

    if ( xi_queue_id > BL_LILAC_RDD_QUEUE_7 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }

    if ( xi_profile_id == rdd_queue_profile_disabled )
    {
        queue_profile_address = 0;
    }
    else
    {
        queue_profile_address = DS_QUEUE_PROFILE_TABLE_ADDRESS + xi_profile_id * sizeof ( RDD_QUEUE_PROFILE_DTS );
    }

    RDD_DS_SERVICE_QUEUE_DESCRIPTOR_PROFILE_PTR_WRITE ( queue_profile_address, service_queue_descriptor_entry_ptr );
    RDD_DS_SERVICE_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_packet_threshold, service_queue_descriptor_entry_ptr );

    if ( xi_rate_limiter_id == RDD_RATE_LIMITER_IDLE )
    {
        ds_service_tm_descriptor_ptr = ( RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_SERVICE_TM_DESCRIPTOR_ADDRESS );

        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_RATE_LIMITER_ID_WRITE ( RDD_RATE_LIMITER_IDLE, service_queue_descriptor_entry_ptr );
        RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_RATE_LIMITER_STATUS_WRITE ( 0xff, ds_service_tm_descriptor_ptr );
    }
    else
    {
        RDD_DS_SERVICE_QUEUE_DESCRIPTOR_RATE_LIMITER_ID_WRITE ( xi_queue_id + RDD_RATE_LIMITER_SERVICE_QUEUE_0, service_queue_descriptor_entry_ptr );
    }
#endif
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_channel_set ( RDD_WAN_CHANNEL_ID           xi_wan_channel_id,
                                             RDD_WAN_CHANNEL_SCHEDULE     xi_schedule,
                                             RDD_US_PEAK_SCHEDULING_MODE  xi_peak_scheduling_mode )
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error;
    static uint32_t         api_first_time_call = LILAC_RDD_TRUE;

    /* check the validity of the input parameters - wan channel index */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 )
    {
        rdd_error = f_rdd_wan_channel_0_7_config ( xi_wan_channel_id, xi_schedule, xi_peak_scheduling_mode );
    }
    else
    {
        rdd_error = f_rdd_wan_channel_8_39_config ( xi_wan_channel_id, xi_schedule, xi_peak_scheduling_mode );
    }

    if ( ( xi_schedule == RDD_WAN_CHANNEL_SCHEDULE_RATE_CONTROL ) && ( api_first_time_call ) )
    {
        f_rdd_us_rate_controller_timer_set ();

        api_first_time_call = LILAC_RDD_FALSE;
    }

    return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_channel_byte_counter_read ( RDD_WAN_CHANNEL_ID  xi_wan_channel_id,
                                                           uint32_t            *xo_byte_counter )
{
    RDD_WAN_CHANNELS_0_7_TABLE_DTS       *wan_channels_0_7_table_ptr;
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS   *wan_channel_0_7_descriptor_ptr;
    RDD_WAN_CHANNELS_8_39_TABLE_DTS      *wan_channels_8_39_table_ptr;
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS  *wan_channel_8_39_descriptor_ptr;

    /* check the validity of the input parameters - wan channel indexe */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 )
    {
        wan_channels_0_7_table_ptr = ( RDD_WAN_CHANNELS_0_7_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + WAN_CHANNELS_0_7_TABLE_ADDRESS );

        wan_channel_0_7_descriptor_ptr = &( wan_channels_0_7_table_ptr->entry[ xi_wan_channel_id ] );

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_BYTE_COUNTER_READ ( *xo_byte_counter, wan_channel_0_7_descriptor_ptr );
    }
    else
    {
        wan_channels_8_39_table_ptr = ( RDD_WAN_CHANNELS_8_39_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + WAN_CHANNELS_8_39_TABLE_ADDRESS );

        wan_channel_8_39_descriptor_ptr = &( wan_channels_8_39_table_ptr->entry[ xi_wan_channel_id - RDD_WAN_CHANNEL_8 ] );

        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_BYTE_COUNTER_READ ( *xo_byte_counter, wan_channel_8_39_descriptor_ptr );
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_channel_rate_limiter_config ( RDD_WAN_CHANNEL_ID                      xi_wan_channel_id,
                                                             BL_LILAC_RDD_RATE_LIMITER_MODE_DTE      xi_rate_limiter_mode,
                                                             BL_LILAC_RDD_RATE_LIMITER_PRIORITY_DTE  xi_rate_limiter_priority )
{
    RDD_WAN_CHANNELS_0_7_TABLE_DTS                         *wan_channels_0_7_table_ptr;
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS                     *wan_channel_0_7_descriptor_ptr;
    RDD_WAN_CHANNELS_8_39_TABLE_DTS                        *wan_channels_8_39_table_ptr;
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS                    *wan_channel_8_39_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS                          *wan_tx_pointers_entry_ptr;
    RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_TABLE_DTS  *rate_limiter_wan_channel_ptr_table_ptr;
    RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_ENTRY_DTS  *rate_limiter_wan_channel_ptr_entry_ptr;
    BL_LILAC_RDD_RATE_LIMITER_MODE_DTE                     rate_limiter_mode;
    uint16_t                                               entry_wan_channel_descriptor_address;
    uint16_t                                               wan_channel_pointer_index;
    static uint32_t                                        policer_number_of_connected_wan_channels = 0;
    BL_LILAC_RDD_ERROR_DTE                                 rdd_error;
#if !defined(FIRMWARE_INIT)
    unsigned long                                          flags;
#endif

    if ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 )
    {
        wan_channels_0_7_table_ptr = ( RDD_WAN_CHANNELS_0_7_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + WAN_CHANNELS_0_7_TABLE_ADDRESS );

        wan_channel_0_7_descriptor_ptr = &( wan_channels_0_7_table_ptr->entry[ xi_wan_channel_id ] );

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_RATE_LIMITER_MODE_READ ( rate_limiter_mode, wan_channel_0_7_descriptor_ptr );

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_RATE_LIMITER_MODE_WRITE ( xi_rate_limiter_mode, wan_channel_0_7_descriptor_ptr );
        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_RATE_LIMITER_PRIORITY_WRITE ( ( xi_rate_limiter_priority & xi_rate_limiter_mode ), wan_channel_0_7_descriptor_ptr );
    }
    else
    {
        wan_channels_8_39_table_ptr = ( RDD_WAN_CHANNELS_8_39_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + WAN_CHANNELS_8_39_TABLE_ADDRESS );

        wan_channel_8_39_descriptor_ptr = &( wan_channels_8_39_table_ptr->entry[ xi_wan_channel_id - RDD_WAN_CHANNEL_8 ] );

        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_RATE_LIMITER_MODE_READ ( rate_limiter_mode, wan_channel_8_39_descriptor_ptr );

        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_RATE_LIMITER_MODE_WRITE ( xi_rate_limiter_mode, wan_channel_8_39_descriptor_ptr );
        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_RATE_LIMITER_PRIORITY_WRITE ( ( xi_rate_limiter_priority & xi_rate_limiter_mode ), wan_channel_8_39_descriptor_ptr );
    }

    rate_limiter_wan_channel_ptr_table_ptr = ( RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_TABLE_ADDRESS );

    rdd_error = BL_LILAC_RDD_OK;

    if ( xi_rate_limiter_mode == BL_LILAC_RDD_RATE_LIMITER_ENABLE )
    {
        if ( rate_limiter_mode == BL_LILAC_RDD_RATE_LIMITER_ENABLE )
        {
            return ( BL_LILAC_RDD_OK );
        }

        wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ 0 ][ 0 ] );

        rate_limiter_wan_channel_ptr_entry_ptr = &( rate_limiter_wan_channel_ptr_table_ptr->entry[ policer_number_of_connected_wan_channels ] );
        RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_ENTRY_WAN_CHANNEL_PTR_WRITE ( wan_tx_pointers_entry_ptr->wan_channel_ptr, rate_limiter_wan_channel_ptr_entry_ptr );

        policer_number_of_connected_wan_channels++;
    }
    else
    {
        wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ 0 ][ 0 ] );

        for ( wan_channel_pointer_index = RDD_WAN_CHANNEL_0; wan_channel_pointer_index <= RDD_WAN_CHANNEL_39; wan_channel_pointer_index++ )
        {
            rate_limiter_wan_channel_ptr_entry_ptr = &( rate_limiter_wan_channel_ptr_table_ptr->entry[ wan_channel_pointer_index ] );
            RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_ENTRY_WAN_CHANNEL_PTR_READ ( entry_wan_channel_descriptor_address, rate_limiter_wan_channel_ptr_entry_ptr );

            if ( entry_wan_channel_descriptor_address == wan_tx_pointers_entry_ptr->wan_channel_ptr )
            {
                policer_number_of_connected_wan_channels--;

                rate_limiter_wan_channel_ptr_entry_ptr = &( rate_limiter_wan_channel_ptr_table_ptr->entry[ policer_number_of_connected_wan_channels ] );
                RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_ENTRY_WAN_CHANNEL_PTR_READ ( entry_wan_channel_descriptor_address, rate_limiter_wan_channel_ptr_entry_ptr );

                rate_limiter_wan_channel_ptr_entry_ptr = &( rate_limiter_wan_channel_ptr_table_ptr->entry[ wan_channel_pointer_index ] );
                RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_ENTRY_WAN_CHANNEL_PTR_WRITE ( entry_wan_channel_descriptor_address, rate_limiter_wan_channel_ptr_entry_ptr );

                rate_limiter_wan_channel_ptr_entry_ptr = &( rate_limiter_wan_channel_ptr_table_ptr->entry[ policer_number_of_connected_wan_channels ] );
                RDD_US_OVERALL_RATE_LIMITER_WAN_CHANNEL_PTR_ENTRY_WAN_CHANNEL_PTR_WRITE ( 0, rate_limiter_wan_channel_ptr_entry_ptr );

                break;
            }
        }

#if !defined(FIRMWARE_INIT)

        if ( rate_limiter_mode == BL_LILAC_RDD_RATE_LIMITER_ENABLE )
        {
            f_rdd_lock_irq ( &int_lock_irq, &flags );

            rdd_error = f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_ACTIVATE_TCONT, FAST_RUNNER_B, RUNNER_PRIVATE_1_OFFSET, xi_wan_channel_id, 0, 0, BL_LILAC_RDD_WAIT );

            f_rdd_unlock_irq ( &int_lock_irq, flags );
        }
#endif

    }

    return ( rdd_error );
}


static void f_rdd_rate_controller_params_set ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS  *rate_controller_descriptor_ptr,
                                               RDD_RATE_CONTROLLER_PARAMS             *xi_params )
{
    RDD_RATE_CONTROLLER_EXPONENT_TABLE_DTS  *exponent_table_ptr;
    RDD_RATE_CONTROLLER_EXPONENT_ENTRY_DTS  *exponent_entry_ptr;
    uint32_t                                exponent_list[ LILAC_RDD_US_RATE_CONTROL_EXPONENT_NUM ];
    uint32_t                                exponent_table_index;
    uint32_t                                peak_budget_exponent;
    uint32_t                                peak_limit_exponent;
    uint32_t                                sustain_budget;
    uint32_t                                peak_budget;
    uint32_t                                peak_limit;

    /* read exponents table */
    exponent_table_ptr = ( RDD_RATE_CONTROLLER_EXPONENT_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_RATE_CONTROLLER_EXPONENT_TABLE_ADDRESS );

    for ( exponent_table_index = 0; exponent_table_index < LILAC_RDD_US_RATE_CONTROL_EXPONENT_NUM; exponent_table_index++ )
    {
        exponent_entry_ptr = &( exponent_table_ptr->entry[ exponent_table_index ] );
        RDD_RATE_CONTROLLER_EXPONENT_ENTRY_EXPONENT_READ ( exponent_list[ exponent_table_index ], exponent_entry_ptr );
    }

    /* convert sustain rate to allocation units */
    sustain_budget = rdd_budget_to_alloc_unit ( xi_params->sustain_budget, BL_LILAC_RDD_US_RATE_CONTROL_TIMER_INTERVAL, 0 );

    /* convert peak budget to allocation unit and divide to exponent and mantissa */
    peak_budget = rdd_budget_to_alloc_unit ( xi_params->peak_budget.rate, BL_LILAC_RDD_US_RATE_CONTROL_TIMER_INTERVAL, 0 );

    peak_budget_exponent = rdd_get_exponent ( xi_params->peak_budget.rate, 14, LILAC_RDD_US_RATE_CONTROL_EXPONENT_NUM, exponent_list );

    peak_budget = peak_budget >> exponent_list[ peak_budget_exponent ];

    /* convert peak limit to allocation unit and divide to exponent and mantissa */
    peak_limit_exponent = rdd_get_exponent ( xi_params->peak_budget.limit, 14, LILAC_RDD_US_RATE_CONTROL_EXPONENT_NUM, exponent_list );

    peak_limit = xi_params->peak_budget.limit >> exponent_list[ peak_limit_exponent ];

    /* write rate control parameters to descriptor */
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_ALLOCATED_PEAK_BUDGET_EXPONENT_WRITE ( peak_budget_exponent, rate_controller_descriptor_ptr );
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_ALLOCATED_PEAK_BUDGET_WRITE ( peak_budget, rate_controller_descriptor_ptr );
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_PEAK_BUDGET_LIMIT_EXPONENT_WRITE ( peak_limit_exponent, rate_controller_descriptor_ptr );
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_PEAK_BUDGET_LIMIT_WRITE ( peak_limit, rate_controller_descriptor_ptr );
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_ALLOCATED_SUSTAIN_BUDGET_WRITE ( sustain_budget, rate_controller_descriptor_ptr );
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_PEAK_WEIGHT_WRITE ( xi_params->peak_weight >> 8, rate_controller_descriptor_ptr );

    return;
}


BL_LILAC_RDD_ERROR_DTE rdd_rate_controller_config ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                    BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                    RDD_RATE_CONTROLLER_PARAMS           *xi_params )
{
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS     *wan_channel_0_7_descriptor_ptr;
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS    *wan_channel_8_39_descriptor_ptr;
    RDD_US_RATE_CONTROLLERS_TABLE_DTS      *rate_controllers_table_ptr;
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS  *rate_controller_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS          *wan_tx_pointers_entry_ptr;
    RDD_BUDGET_ALLOCATOR_TABLE_DTS         *budget_allocator_table_ptr;
    RDD_BUDGET_ALLOCATOR_ENTRY_DTS         *budget_allocator_entry_ptr;
    uint32_t                               rate_controller_mask;
    uint32_t                               wan_channel_scheduling;
    uint16_t                               active_rate_controllers;
    uint32_t                               tx_queue_id;
    uint32_t                               idx;

    /* check the validity of the input parameters - wan channel and Rate controller indexes */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 ) )
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_31 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }
    else
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_3 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }

    rate_controllers_table_ptr = ( RDD_US_RATE_CONTROLLERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + US_RATE_CONTROLLERS_TABLE_ADDRESS - sizeof ( RUNNER_COMMON ) );

    /* find a free rate controller descriptor in the table */
    for ( idx = 0; idx < RDD_US_RATE_CONTROLLERS_TABLE_SIZE; idx++ )
    {
        rate_controller_descriptor_ptr =
            &( rate_controllers_table_ptr->entry[ ( idx + g_rate_controllers_pool_idx ) % RDD_US_RATE_CONTROLLERS_TABLE_SIZE ] );

        RDD_US_RATE_CONTROLLER_DESCRIPTOR_RATE_CONTROLLER_MASK_READ ( rate_controller_mask, rate_controller_descriptor_ptr );

        if ( rate_controller_mask == 0 )
        {
            break;
        }
    }

    /* all the rate controller descriptors are occupied */
    if ( idx == RDD_US_RATE_CONTROLLERS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_RATE_CONTROLLERS_POOL_OVERFLOW );
    }

    idx = ( idx + g_rate_controllers_pool_idx ) % RDD_US_RATE_CONTROLLERS_TABLE_SIZE;

    MEMSET ( rate_controller_descriptor_ptr, 0, sizeof ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS ) );

    for ( tx_queue_id = BL_LILAC_RDD_QUEUE_0; tx_queue_id <= BL_LILAC_RDD_QUEUE_7; tx_queue_id++ )
    {
        /* write the allocated rate controller descriptor address to the wan tx queues pointers table */
        wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ tx_queue_id ] );

        wan_tx_pointers_entry_ptr->rate_controller_ptr = US_RATE_CONTROLLERS_TABLE_ADDRESS + idx * sizeof ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS );

        /* priority queues are not initialized - therfore packets should be dropped */
        RDD_US_RATE_CONTROLLER_DESCRIPTOR_TX_QUEUE_ADDR_WRITE ( DUMMY_WAN_TX_QUEUE_DESCRIPTOR_ADDRESS, rate_controller_descriptor_ptr, tx_queue_id);
    }

    /* initialize the hardcoded parameters of the rate controller descriptor */
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_RATE_CONTROLLER_MASK_WRITE ( ( 1 << xi_rate_controller_id ), rate_controller_descriptor_ptr );

    f_rdd_rate_controller_params_set ( rate_controller_descriptor_ptr, xi_params );

    RDD_US_RATE_CONTROLLER_DESCRIPTOR_PEAK_BURST_FLAG_WRITE ( LILAC_RDD_ON, rate_controller_descriptor_ptr );

    /* configuration handler assumption: the wan channel was already configured */
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_WAN_CHANNEL_PTR_WRITE ( wan_tx_pointers_entry_ptr->wan_channel_ptr, rate_controller_descriptor_ptr );

    if ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 )
    {
        wan_channel_0_7_descriptor_ptr = ( RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_channel_ptr );

        /* set the inverse connection: wan channel to it's rate controller */
        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_RATE_CONTROLLER_ADDR_WRITE ( US_RATE_CONTROLLERS_TABLE_ADDRESS + idx * sizeof ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS ), wan_channel_0_7_descriptor_ptr, xi_rate_controller_id );

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_SCHEDULE_READ ( wan_channel_scheduling, wan_channel_0_7_descriptor_ptr );
    }
    else
    {
        wan_channel_8_39_descriptor_ptr = ( RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_channel_ptr );

        /* set the inverse connection: wan channel to it's rate controller */
        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_RATE_CONTROLLER_ADDR_WRITE ( US_RATE_CONTROLLERS_TABLE_ADDRESS + idx * sizeof( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS ), wan_channel_8_39_descriptor_ptr, xi_rate_controller_id );

        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_SCHEDULE_READ ( wan_channel_scheduling, wan_channel_8_39_descriptor_ptr );
    }

    /* budget allocator optimization - spread allocation between groups of 16 rate controller descriptors */
    g_rate_controllers_pool_idx = ( idx + LILAC_RDD_RATE_CONTROLLERS_BUDGET_SET_SIZE ) & ~( LILAC_RDD_RATE_CONTROLLERS_BUDGET_SET_SIZE - 1 );

    if ( wan_channel_scheduling )
    {
        budget_allocator_table_ptr = ( RDD_BUDGET_ALLOCATOR_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_RATE_CONTROL_BUDGET_ALLOCATOR_TABLE_ADDRESS );

        budget_allocator_entry_ptr = &( budget_allocator_table_ptr->entry[ ( idx / LILAC_RDD_RATE_CONTROLLERS_BUDGET_SET_SIZE ) ] );

        RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_READ ( active_rate_controllers, budget_allocator_entry_ptr );

        active_rate_controllers |= ( 1 << ( idx % LILAC_RDD_RATE_CONTROLLERS_BUDGET_SET_SIZE ) );

        RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_WRITE ( active_rate_controllers, budget_allocator_entry_ptr );
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_rate_controller_modify ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                    BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                    RDD_RATE_CONTROLLER_PARAMS           *xi_params )
{
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS     *wan_channel_0_7_descriptor_ptr;
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS    *wan_channel_8_39_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS          *wan_tx_pointers_entry_ptr;
    RDD_BUDGET_ALLOCATOR_TABLE_DTS         *budget_allocator_table_ptr;
    RDD_BUDGET_ALLOCATOR_ENTRY_DTS         *budget_allocator_entry_ptr;
    uint16_t                               active_rate_controllers;
    uint32_t                               wan_channel_scheduling;
    uint32_t                               idx;

    /* check the validity of the input parameters - wan channel and Rate controller indexes */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 ) )
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_31 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }
    else
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_3 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ 0 ] );

    /* verify that the rate controller was configured before */
    if ( wan_tx_pointers_entry_ptr->rate_controller_ptr == 0 )
    {
        return ( BL_LILAC_RDD_ERROR_RATE_CONTROLLER_NOT_CONFIGURED );
    }

    f_rdd_rate_controller_params_set ( ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) +
                                       wan_tx_pointers_entry_ptr->rate_controller_ptr - sizeof ( RUNNER_COMMON ) ),
                                       xi_params );

    /*get rate controller index*/
    idx = ( wan_tx_pointers_entry_ptr->rate_controller_ptr - US_RATE_CONTROLLERS_TABLE_ADDRESS ) / sizeof ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS );

    if ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 )
    {
        wan_channel_0_7_descriptor_ptr = ( RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_channel_ptr );

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_SCHEDULE_READ ( wan_channel_scheduling, wan_channel_0_7_descriptor_ptr );
    }
    else
    {
        wan_channel_8_39_descriptor_ptr = ( RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_channel_ptr );

        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_SCHEDULE_READ ( wan_channel_scheduling, wan_channel_8_39_descriptor_ptr );
    }

    /* budget allocator optimization - spread allocation between groups of 16 rate controller descriptors */
    budget_allocator_table_ptr = ( RDD_BUDGET_ALLOCATOR_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_RATE_CONTROL_BUDGET_ALLOCATOR_TABLE_ADDRESS );
    
    budget_allocator_entry_ptr = &( budget_allocator_table_ptr->entry[ ( idx / LILAC_RDD_RATE_CONTROLLERS_BUDGET_SET_SIZE ) ] );

    RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_READ ( active_rate_controllers, budget_allocator_entry_ptr );
    
    if ( wan_channel_scheduling )
    {
        active_rate_controllers |= ( 1 << ( idx % LILAC_RDD_RATE_CONTROLLERS_BUDGET_SET_SIZE ) );
    }
    else
    {
        active_rate_controllers &= ~( 1 << ( idx % LILAC_RDD_RATE_CONTROLLERS_BUDGET_SET_SIZE ) );
    }

    RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_WRITE ( active_rate_controllers, budget_allocator_entry_ptr );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_rate_controller_remove ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                    BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id )
{
    RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS     *wan_channel_0_7_descriptor_ptr;
    RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS    *wan_channel_8_39_descriptor_ptr;
    RDD_US_RATE_CONTROLLERS_TABLE_DTS      *rate_controllers_table_ptr;
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS  *rate_controller_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS          *wan_tx_pointers_entry_ptr;
    RDD_BUDGET_ALLOCATOR_TABLE_DTS         *budget_allocator_table_ptr;
    RDD_BUDGET_ALLOCATOR_ENTRY_DTS         *budget_allocator_entry_ptr;
    uint32_t                               wan_channel_scheduling;
    uint16_t                               active_rate_controllers;
    uint32_t                               queue_id;
    uint32_t                               idx;

    /* check the validity of the input parameters - wan channel and Rate controller indexes */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 ) )
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_31 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }
    else
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_3 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ 0 ] );

    /* verify that the rate controller was configured before */
    if ( wan_tx_pointers_entry_ptr->rate_controller_ptr == 0 )
    {
        return ( BL_LILAC_RDD_ERROR_RATE_CONTROLLER_NOT_CONFIGURED );
    }

    /* disconnect the queue from the rate controller */
    if ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 )
    {
        wan_channel_0_7_descriptor_ptr = ( RDD_WAN_CHANNEL_0_7_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_channel_ptr);

        /* set the inverse connection: wan channel to it's rate controller */
        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_RATE_CONTROLLER_ADDR_WRITE ( DUMMY_RATE_CONTROLLER_DESCRIPTOR_ADDRESS, wan_channel_0_7_descriptor_ptr, xi_rate_controller_id );

        RDD_WAN_CHANNEL_0_7_DESCRIPTOR_SCHEDULE_READ ( wan_channel_scheduling, wan_channel_0_7_descriptor_ptr );
    }
    else
    {
        wan_channel_8_39_descriptor_ptr = ( RDD_WAN_CHANNEL_8_39_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_channel_ptr );

        /* set the inverse connection: wan channel to it's rate controller */
        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_RATE_CONTROLLER_ADDR_WRITE ( DUMMY_RATE_CONTROLLER_DESCRIPTOR_ADDRESS, wan_channel_8_39_descriptor_ptr, xi_rate_controller_id );

        RDD_WAN_CHANNEL_8_39_DESCRIPTOR_SCHEDULE_READ ( wan_channel_scheduling, wan_channel_8_39_descriptor_ptr );
    }

    rate_controller_descriptor_ptr = ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + wan_tx_pointers_entry_ptr->rate_controller_ptr - sizeof ( RUNNER_COMMON ) );

    RDD_US_RATE_CONTROLLER_DESCRIPTOR_RATE_CONTROLLER_MASK_WRITE ( 0, rate_controller_descriptor_ptr );

    for ( queue_id = 0; queue_id < BL_LILAC_RDD_QUEUE_7; queue_id++ )
    {
        wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ queue_id ] );

        wan_tx_pointers_entry_ptr->rate_controller_ptr = 0;
    }

    rate_controllers_table_ptr = ( RDD_US_RATE_CONTROLLERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + US_RATE_CONTROLLERS_TABLE_ADDRESS - sizeof ( RUNNER_COMMON ) );

    /* budget allocator optimization */
    if ( wan_channel_scheduling )
    {
        budget_allocator_table_ptr = ( RDD_BUDGET_ALLOCATOR_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_RATE_CONTROL_BUDGET_ALLOCATOR_TABLE_ADDRESS );

        for ( idx = 0; idx < RDD_US_RATE_CONTROLLERS_TABLE_SIZE; idx++ )
        {
            if ( rate_controller_descriptor_ptr == &( rate_controllers_table_ptr->entry[ idx ] ) )
            {
                break;
            }
        }

        budget_allocator_entry_ptr = &( budget_allocator_table_ptr->entry[ ( idx / 16 ) ] );

        RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_READ ( active_rate_controllers, budget_allocator_entry_ptr );

        active_rate_controllers &= ~( 1 << ( idx % 16 ) );

        RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_WRITE ( active_rate_controllers, budget_allocator_entry_ptr );
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_tx_queue_config ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                 BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                 BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id,
                                                 uint16_t                             xi_packet_threshold,
                                                 rdd_queue_profile                    xi_profile_id,
                                                 uint8_t                              xi_counter_id )
{
    RDD_WAN_TX_QUEUES_TABLE_DTS            *wan_tx_queues_table_ptr;
    RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS        *wan_tx_queue_descriptor_ptr;
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS  *rate_controller_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS          *wan_tx_pointers_entry_ptr;
    uint16_t                               wan_channel_queue_mask;
    uint32_t                               idx;
    uint16_t                               queue_profile_address;

    /* check the validity of the input parameters - wan channel, Rate controller and queue indexes */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 ) )
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_31 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }
    else
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_3 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }

    if ( xi_queue_id > BL_LILAC_RDD_QUEUE_7 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }

    wan_tx_queues_table_ptr = ( RDD_WAN_TX_QUEUES_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + WAN_TX_QUEUES_TABLE_ADDRESS - sizeof ( RUNNER_COMMON ) );

    /* find a free wan tx queue descriptor in the wan tx queues table */
    for ( idx = 0; idx < RDD_WAN_TX_QUEUES_TABLE_SIZE; idx++ )
    {
        wan_tx_queue_descriptor_ptr = &( wan_tx_queues_table_ptr->entry[ idx ] );

        RDD_WAN_TX_QUEUE_DESCRIPTOR_QUEUE_MASK_READ ( wan_channel_queue_mask , wan_tx_queue_descriptor_ptr );

        if ( wan_channel_queue_mask == 0 )
        {
            break;
        }
    }

    /* all the wan tx queues descriptors are occupied */
    if ( idx == RDD_WAN_TX_QUEUES_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_GPON_TX_QUEUES_POOL_OVERFLOW );
    }

    MEMSET ( wan_tx_queue_descriptor_ptr, 0, sizeof ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS ) );

    /* write the allocated tx queue descriptor address to the wan tx queues pointers table */
    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ xi_queue_id ] );

    wan_tx_pointers_entry_ptr->wan_tx_queue_ptr = WAN_TX_QUEUES_TABLE_ADDRESS + idx * sizeof ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS );

    /* initialize the hardcoded parameters of the wan tx queue descriptor */
    RDD_WAN_TX_QUEUE_DESCRIPTOR_INDEX_WRITE ( idx, wan_tx_queue_descriptor_ptr );
    RDD_WAN_TX_QUEUE_DESCRIPTOR_QUEUE_MASK_WRITE ( ( 1 << xi_queue_id ), wan_tx_queue_descriptor_ptr );

    /* initialize the configured parameters of the wan tx queue descriptor */
    RDD_WAN_TX_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_packet_threshold, wan_tx_queue_descriptor_ptr );

    if ( xi_profile_id == rdd_queue_profile_disabled )
    {
        queue_profile_address = 0;
    }
    else
    {
        queue_profile_address = US_QUEUE_PROFILE_TABLE_ADDRESS + xi_profile_id * sizeof ( RDD_QUEUE_PROFILE_DTS );
    }

    RDD_WAN_TX_QUEUE_DESCRIPTOR_PROFILE_PTR_WRITE ( queue_profile_address, wan_tx_queue_descriptor_ptr );
    RDD_WAN_TX_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_packet_threshold, wan_tx_queue_descriptor_ptr );

    /* configuration handler assumption: the rate controller was already configured */
    RDD_WAN_TX_QUEUE_DESCRIPTOR_RATE_CONTROLLER_PTR_WRITE ( wan_tx_pointers_entry_ptr->rate_controller_ptr, wan_tx_queue_descriptor_ptr );

    rate_controller_descriptor_ptr = ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + wan_tx_pointers_entry_ptr->rate_controller_ptr - sizeof ( RUNNER_COMMON ) );

    /* set the inverse connection: rate controller to it's queue */
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_TX_QUEUE_ADDR_WRITE ( WAN_TX_QUEUES_TABLE_ADDRESS + idx * sizeof( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS ), rate_controller_descriptor_ptr, xi_queue_id);

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_tx_queue_modify ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                 BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                 BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id,
                                                 uint16_t                             xi_packet_threshold,
                                                 rdd_queue_profile                    xi_profile_id,
                                                 uint8_t                              xi_counter_id )
{
    RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS  *wan_tx_queue_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS    *wan_tx_pointers_entry_ptr;
    uint16_t                         queue_profile_address;

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ xi_queue_id ] );

    /* verify that the wan channel queue was configured before */
    if ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr == 0 )
    {
        return ( BL_LILAC_RDD_ERROR_GPON_TX_QUEUE_NOT_CONFIGURED );
    }

    wan_tx_queue_descriptor_ptr = ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - sizeof ( RUNNER_COMMON ) );

    /* modify the packet threshold and profile of the wan channel queue */
    if ( xi_profile_id == rdd_queue_profile_disabled )
    {
        queue_profile_address = 0;
    }
    else
    {
        queue_profile_address = US_QUEUE_PROFILE_TABLE_ADDRESS + xi_profile_id * sizeof ( RDD_QUEUE_PROFILE_DTS );
    }

    RDD_WAN_TX_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_packet_threshold, wan_tx_queue_descriptor_ptr );
    RDD_WAN_TX_QUEUE_DESCRIPTOR_PROFILE_PTR_WRITE ( queue_profile_address, wan_tx_queue_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_tx_queue_remove ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                 BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                 BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id )
{
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS  *rate_controller_descriptor_ptr;
    RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS        *wan_tx_queue_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS          *wan_tx_pointers_entry_ptr;

    /* check the validity of the input parameters - wan channel, Rate controller and queue indexes */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 ) )
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_31 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }
    else
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_3 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }

    if ( xi_queue_id > BL_LILAC_RDD_QUEUE_7 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ xi_queue_id ] );

    /* verify that the queue was configured before */
    if ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr == 0 )
    {
        return ( BL_LILAC_RDD_ERROR_GPON_TX_QUEUE_NOT_CONFIGURED );
    }

    /* disconnect the queue from the rate controller */
    rate_controller_descriptor_ptr = ( RDD_US_RATE_CONTROLLER_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + wan_tx_pointers_entry_ptr->rate_controller_ptr - sizeof ( RUNNER_COMMON ) );

    /* packets that will continue to arrive to this queue will be dropped */
    RDD_US_RATE_CONTROLLER_DESCRIPTOR_TX_QUEUE_ADDR_WRITE ( DUMMY_WAN_TX_QUEUE_DESCRIPTOR_ADDRESS, rate_controller_descriptor_ptr, xi_queue_id );

    wan_tx_queue_descriptor_ptr = ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - sizeof ( RUNNER_COMMON ) );

    RDD_WAN_TX_QUEUE_DESCRIPTOR_QUEUE_MASK_WRITE ( 0, wan_tx_queue_descriptor_ptr );

    wan_tx_pointers_entry_ptr->wan_tx_queue_ptr = 0;

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_tx_queue_flush ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id,
                                                BL_LILAC_RDD_CPU_WAIT_DTE            xi_wait )
{
    RDD_WAN_TX_POINTERS_ENTRY_DTS  *wan_tx_pointers_entry_ptr;
    BL_LILAC_RDD_ERROR_DTE         rdd_error;
    unsigned long                  flags;

    /* check the validity of the input parameters - wan channel and queue index */
    if ( xi_wan_channel_id > RDD_WAN_CHANNEL_39 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_WAN_CHANNEL_ID );
    }

    if ( ( xi_wan_channel_id <= RDD_WAN_CHANNEL_7 ) )
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_31 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }
    else
    {
        if ( xi_rate_controller_id > BL_LILAC_RDD_RATE_CONTROLLER_3 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_CONTROLLER_ID );
        }
    }

    if ( xi_queue_id > BL_LILAC_RDD_QUEUE_7 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ xi_queue_id ] );

    /* verify that the queue was configured before */
    if ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr == 0 )
    {
        return ( BL_LILAC_RDD_ERROR_GPON_TX_QUEUE_NOT_CONFIGURED );
    }

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    rdd_error = f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_FLUSH_GPON_QUEUE, FAST_RUNNER_B, RUNNER_PRIVATE_1_OFFSET,
                                            ( ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - WAN_TX_QUEUES_TABLE_ADDRESS ) / sizeof ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS ) ), 0, 0, xi_wait );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_tx_queue_get_status ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                     BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                     BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id,
                                                     rdpa_stat_1way_t                     *stat )
{
    BL_LILAC_RDD_ERROR_DTE rdd_error = BL_LILAC_RDD_OK;
    RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS  *wan_tx_queue_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS    *wan_tx_pointers_entry_ptr;

#if defined(DSL_63138) || defined(DSL_63148)
    uint8_t  tx_queue;
    uint16_t discarded_packets;
#endif

    /* find dynamically allocated queue and read its size */
    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ xi_queue_id ] );

    wan_tx_queue_descriptor_ptr = ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - sizeof ( RUNNER_COMMON ) );

#if !defined(DSL_63138) && !defined(DSL_63148)

    RDD_WAN_TX_QUEUE_DESCRIPTOR_PACKET_COUNTER_READ( (stat->passed.packets), wan_tx_queue_descriptor_ptr );

#else

    RDD_WAN_TX_QUEUE_DESCRIPTOR_INDEX_READ ( tx_queue, wan_tx_queue_descriptor_ptr );

    if (xi_wan_channel_id == 0)
    {        
       rdd_error = rdd_4_bytes_counter_get ( WAN_TX_QUEUES_PACKETS_GROUP, tx_queue, &stat->passed.packets );
       if ( rdd_error != BL_LILAC_RDD_OK )
           return ( rdd_error );

       rdd_error = rdd_4_bytes_counter_get ( WAN_TX_QUEUES_BYTES_GROUP, tx_queue, &stat->passed.bytes );
       if ( rdd_error != BL_LILAC_RDD_OK )
           return ( rdd_error );

       rdd_error = rdd_2_bytes_counter_get ( WAN_TX_QUEUES_DROPPED_PACKETS_GROUP, tx_queue, &discarded_packets );
       stat->discarded.packets = discarded_packets;
       if ( rdd_error != BL_LILAC_RDD_OK )
           return ( rdd_error );

       rdd_error = rdd_4_bytes_counter_get ( WAN_TX_QUEUES_DROPPED_BYTES_GROUP, tx_queue, &stat->discarded.bytes );
    }
    else
    {
       rdd_error = rdd_4_bytes_counter_get ( WAN0_TX_QUEUES_PACKETS_GROUP, tx_queue, &stat->passed.packets );
       if ( rdd_error != BL_LILAC_RDD_OK )
           return ( rdd_error );

       rdd_error = rdd_4_bytes_counter_get ( WAN0_TX_QUEUES_BYTES_GROUP, tx_queue, &stat->passed.bytes );
       if ( rdd_error != BL_LILAC_RDD_OK )
           return ( rdd_error );

       rdd_error = rdd_2_bytes_counter_get ( WAN0_TX_QUEUES_DROPPED_PACKETS_GROUP, tx_queue, &discarded_packets );
       stat->discarded.packets = discarded_packets;
       if ( rdd_error != BL_LILAC_RDD_OK )
           return ( rdd_error );

       rdd_error = rdd_4_bytes_counter_get ( WAN0_TX_QUEUES_DROPPED_BYTES_GROUP, tx_queue, &stat->discarded.bytes );
    }
#endif

    return ( rdd_error );
}

BL_LILAC_RDD_ERROR_DTE rdd_wan_tx_flow_control_config ( BL_LILAC_RDD_BRIDGE_PORT_VECTOR_DTE xi_bridge_port)
{
    return ( BL_LILAC_RDD_OK );
}

#if !defined(DSL_63138) && !defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE f_epon_tx_post_scheduling_ddr_queue_initialize ( uint16_t  xi_queue_size )
{
    RDD_EPON_TX_POST_SCHEDULING_QUEUES_TABLE_DTS          *epon_tx_post_scheduling_queues_table_ptr;
    RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_DTS      *epon_tx_post_scheduling_queue_descriptor_ptr;
    RDD_EPON_TX_POST_SCHEDULING_DDR_QUEUES_TABLE_DTS      *epon_tx_post_scheduling_ddr_queues_table_ptr;
    RDD_EPON_TX_POST_SCHEDULING_DDR_QUEUE_DESCRIPTOR_DTS  *epon_tx_post_scheduling_ddr_queue_descriptor_ptr;
    uint32_t                                              ddr_queue_head_address;
    uint16_t                                              wan_channel_id;

    epon_tx_post_scheduling_queues_table_ptr = ( RDD_EPON_TX_POST_SCHEDULING_QUEUES_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) +
                                                                                                  EPON_TX_POST_SCHEDULING_QUEUES_TABLE_ADDRESS - sizeof ( RUNNER_COMMON ) );

    epon_tx_post_scheduling_ddr_queues_table_ptr = ( RDD_EPON_TX_POST_SCHEDULING_DDR_QUEUES_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) +
                                                                                                          EPON_TX_POST_SCHEDULING_DDR_QUEUES_TABLE_ADDRESS - sizeof ( RUNNER_COMMON ) );

    for( wan_channel_id = 0; wan_channel_id <= RDD_WAN_CHANNEL_15; wan_channel_id++ )
    {
        epon_tx_post_scheduling_queue_descriptor_ptr = &( epon_tx_post_scheduling_queues_table_ptr->entry[ wan_channel_id ] );

        MEMSET ( epon_tx_post_scheduling_queue_descriptor_ptr, 0, sizeof ( RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_DTS ) );

        /* initialize the hardcoded parameters of the GPON tx queue descriptor */
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_HEAD_PTR_WRITE ( 0, epon_tx_post_scheduling_queue_descriptor_ptr );
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_TAIL_PTR_WRITE ( 0, epon_tx_post_scheduling_queue_descriptor_ptr );
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_TAIL_ENTRY_WRITE ( 0, epon_tx_post_scheduling_queue_descriptor_ptr );
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_HEAD_ENTRY_WRITE ( 0, epon_tx_post_scheduling_queue_descriptor_ptr );
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_TAIL_BASE_ENTRY_WRITE ( 0x8, epon_tx_post_scheduling_queue_descriptor_ptr );
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_HEAD_BASE_ENTRY_WRITE ( 0, epon_tx_post_scheduling_queue_descriptor_ptr );
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_QUEUE_STATE_WRITE ( 0, epon_tx_post_scheduling_queue_descriptor_ptr );

        /* initialize the configured parameters of the GPON queue descriptor */
        RDD_EPON_TX_POST_SCHEDULING_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_queue_size, epon_tx_post_scheduling_queue_descriptor_ptr );

        /* set DDR q base address for each queue */
        epon_tx_post_scheduling_ddr_queue_descriptor_ptr = &( epon_tx_post_scheduling_ddr_queues_table_ptr->entry[ wan_channel_id ] );

        ddr_queue_head_address = ( EponPostSchedulingDdrQueuesBase + ( wan_channel_id * xi_queue_size * CACHE_ENTRY_BYTE_SIZE ) ) & 0x1FFFFFFF;

        RDD_EPON_TX_POST_SCHEDULING_DDR_QUEUE_DESCRIPTOR_BASE_ADDRESS_WRITE ( ddr_queue_head_address, epon_tx_post_scheduling_ddr_queue_descriptor_ptr );
    }

    return ( BL_LILAC_RDD_OK );
}
#endif /* !DSL */


BL_LILAC_RDD_ERROR_DTE rdd_emac_config ( BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id,
                                         RDD_RATE_LIMITER_ID_DTE   xi_rate_limiter_id,
                                         uint16_t                  xi_rate_shaper_max_burst )
{
    RDD_ETH_TX_MAC_TABLE_DTS       *eth_tx_mac_table_ptr;
    RDD_ETH_TX_MAC_DESCRIPTOR_DTS  *eth_tx_mac_descriptor_ptr;

    /* check the validity of the input parameters - emac id */
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    eth_tx_mac_table_ptr = ( RDD_ETH_TX_MAC_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_MAC_TABLE_ADDRESS );

    eth_tx_mac_descriptor_ptr = &( eth_tx_mac_table_ptr->entry[ xi_emac_id ] );

    RDD_ETH_TX_MAC_DESCRIPTOR_RATE_LIMITER_ID_WRITE ( xi_rate_limiter_id, eth_tx_mac_descriptor_ptr );
    RDD_ETH_TX_MAC_DESCRIPTOR_RATE_SHAPER_MAX_BURST_WRITE ( xi_rate_shaper_max_burst, eth_tx_mac_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}

#ifdef G9991
BL_LILAC_RDD_ERROR_DTE rdd_G9991_virtual_to_physical_port_mapping_config ( BL_LILAC_RDD_EMAC_ID_DTE  xi_virtual_port,
                                                                           BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id )
{
    RDD_ETH_TX_MAC_TABLE_DTS                    *eth_tx_mac_table_ptr;
    RDD_ETH_TX_MAC_DESCRIPTOR_DTS               *eth_tx_mac_descriptor_ptr;
    RDD_G9991_VIRTUAL_PORT_STATUS_PER_EMAC_DTS  *virtual_port_status_per_emac_table_ptr;
    RDD_ETH_TX_LOCAL_REGISTERS_DTS              *eth_tx_local_registers_ptr;
    RDD_ETH_TX_LOCAL_REGISTERS_ENTRY_DTS        *eth_tx_local_registers_entry_ptr;
    uint32_t                                    bbh_peripheral_eth_tx;
    uint32_t                                    virtual_port_status;


    eth_tx_mac_table_ptr = RDD_ETH_TX_MAC_TABLE_PTR();
    eth_tx_mac_descriptor_ptr = &( eth_tx_mac_table_ptr->entry[ xi_virtual_port ] );

    RDD_ETH_TX_MAC_DESCRIPTOR_TX_TASK_NUMBER_WRITE ( ETH_TX_THREAD_NUMBER, eth_tx_mac_descriptor_ptr );
    RDD_ETH_TX_MAC_DESCRIPTOR_EMAC_MASK_WRITE ( ( 1 << xi_virtual_port ), eth_tx_mac_descriptor_ptr );
    RDD_ETH_TX_MAC_DESCRIPTOR_PHYISCAL_PORT_WRITE ( xi_emac_id, eth_tx_mac_descriptor_ptr );
    RDD_ETH_TX_MAC_DESCRIPTOR_EOF_FLAG_WRITE ( 1, eth_tx_mac_descriptor_ptr );

    switch ( xi_emac_id )
    {
        case BL_LILAC_RDD_EMAC_ID_0:
            bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH0_TX;
            break;
        case BL_LILAC_RDD_EMAC_ID_1:
            bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH1_TX;
            break;
        case BL_LILAC_RDD_EMAC_ID_2:
            bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH2_TX;
            break;
        case BL_LILAC_RDD_EMAC_ID_3:
            bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH3_TX;
            break;
        case BL_LILAC_RDD_EMAC_ID_4:
            bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH4_TX;
            break;
        default:
            bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH0_TX;
            break;
    }

    eth_tx_local_registers_ptr = RDD_ETH_TX_LOCAL_REGISTERS_PTR();

    eth_tx_local_registers_entry_ptr = &(eth_tx_local_registers_ptr->entry[ xi_virtual_port ]);

    RDD_ETH_TX_LOCAL_REGISTERS_ENTRY_EMAC_DESCRIPTOR_PTR_WRITE ( ( ETH_TX_MAC_TABLE_ADDRESS + xi_virtual_port * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ), eth_tx_local_registers_entry_ptr );
    RDD_ETH_TX_LOCAL_REGISTERS_ENTRY_ETH_TX_QUEUES_POINTERS_TABLE_PTR_WRITE ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + xi_virtual_port * LILAC_RDD_EMAC_NUMBER_OF_QUEUES * sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ), eth_tx_local_registers_entry_ptr );
    RDD_ETH_TX_LOCAL_REGISTERS_ENTRY_BBH_DESTINATION_WRITE ( bbh_peripheral_eth_tx, eth_tx_local_registers_entry_ptr );

    virtual_port_status_per_emac_table_ptr = RDD_G9991_VIRTUAL_PORT_STATUS_PER_EMAC_PTR();
    MREAD_32( &( virtual_port_status_per_emac_table_ptr->entry[ xi_emac_id ]), virtual_port_status );

    virtual_port_status = virtual_port_status | ( 1 << xi_virtual_port );
    MWRITE_32( &( virtual_port_status_per_emac_table_ptr->entry[ xi_emac_id ]), virtual_port_status );

    return ( BL_LILAC_RDD_OK );
}
#endif

BL_LILAC_RDD_ERROR_DTE rdd_inter_lan_scheduling_mode_config ( BL_LILAC_RDD_INTER_LAN_SCHEDULING_MODE_DTE  xi_inter_lan_scheduling_mode,
                                                              BL_LILAC_RDD_EMAC_ID_DTE                    xi_emac_id )
{
    RDD_ETH_TX_MAC_TABLE_DTS               *eth_tx_mac_table_ptr;
    RDD_ETH_TX_MAC_DESCRIPTOR_DTS          *eth_tx_mac_descriptor_ptr;
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;
    uint32_t                               *eth_tx_local_registers_ptr;
    uint32_t                               emac_id;
    uint32_t                               bbh_peripheral_eth_tx;

    eth_tx_mac_table_ptr = ( RDD_ETH_TX_MAC_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_MAC_TABLE_ADDRESS );

    for ( emac_id = BL_LILAC_RDD_EMAC_ID_0; emac_id < BL_LILAC_RDD_EMAC_ID_COUNT; emac_id++ )
    {
        eth_tx_mac_descriptor_ptr = &( eth_tx_mac_table_ptr->entry[ emac_id ] );

        if ( xi_inter_lan_scheduling_mode == BL_LILAC_RDD_INTER_LAN_SCHEDULING_MODE_NORMAL )
        {
            RDD_ETH_TX_MAC_DESCRIPTOR_TX_TASK_NUMBER_WRITE ( ( ETH0_TX_THREAD_NUMBER + ( emac_id - BL_LILAC_RDD_EMAC_ID_0 ) ), eth_tx_mac_descriptor_ptr );
            RDD_ETH_TX_MAC_DESCRIPTOR_EMAC_MASK_WRITE ( 0, eth_tx_mac_descriptor_ptr );
            RDD_ETH_TX_MAC_DESCRIPTOR_PACKET_COUNTERS_PTR_WRITE ( ETH_TX_MAC_TABLE_ADDRESS + emac_id * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) + LILAC_RDD_EMAC_EGRESS_COUNTER_OFFSET, eth_tx_mac_descriptor_ptr );
        }
        else
        {
            RDD_ETH_TX_MAC_DESCRIPTOR_TX_TASK_NUMBER_WRITE ( ETH_TX_THREAD_NUMBER, eth_tx_mac_descriptor_ptr );
            RDD_ETH_TX_MAC_DESCRIPTOR_EMAC_MASK_WRITE ( ( 1 << emac_id ), eth_tx_mac_descriptor_ptr );
            RDD_ETH_TX_MAC_DESCRIPTOR_PACKET_COUNTERS_PTR_WRITE ( ETH_TX_MAC_TABLE_ADDRESS + emac_id * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) + LILAC_RDD_EMAC_EGRESS_COUNTER_OFFSET, eth_tx_mac_descriptor_ptr );

            if ( xi_inter_lan_scheduling_mode == BL_LILAC_RDD_INTER_LAN_SCHEDULING_MODE_ROUND_ROBIN )
            {
                RDD_ETH_TX_MAC_DESCRIPTOR_PACKET_COUNTERS_PTR_WRITE ( ETH_TX_MAC_TABLE_ADDRESS + xi_emac_id * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) + LILAC_RDD_EMAC_EGRESS_COUNTER_OFFSET, eth_tx_mac_descriptor_ptr );
            }
        }
    }


    eth_tx_local_registers_ptr = ( uint32_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_LOCAL_REGISTERS_ADDRESS );

    if ( ( xi_inter_lan_scheduling_mode == BL_LILAC_RDD_INTER_LAN_SCHEDULING_MODE_STRICT_PRIORITY ) || ( xi_inter_lan_scheduling_mode == BL_LILAC_RDD_INTER_LAN_SCHEDULING_MODE_ROUND_ROBIN ) )
    {
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_0 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 1 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_1 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 2 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_2 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 3 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_3 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 4 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_4 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 5 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
#if defined(DSL_63138) || defined(DSL_63148)
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_5 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 6 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_6 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 7 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
        MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_7 * 2 + 0, ( ETH_TX_MAC_TABLE_ADDRESS + 8 * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) ) << 16 );
#endif /* DSL */

        if ( xi_emac_id == 0 )
        {
            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_0 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 1 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | BBH_PERIPHERAL_ETH0_TX );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_1 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 2 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | BBH_PERIPHERAL_ETH1_TX );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_2 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 3 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | BBH_PERIPHERAL_ETH2_TX );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_3 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 4 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | BBH_PERIPHERAL_ETH3_TX );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_4 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 5 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | BBH_PERIPHERAL_ETH4_TX );
        }
        else
        {
            switch ( xi_emac_id )
            {
            case BL_LILAC_RDD_EMAC_ID_0:
                bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH0_TX;
                break;
            case BL_LILAC_RDD_EMAC_ID_1:
                bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH1_TX;
                break;
            case BL_LILAC_RDD_EMAC_ID_2:
                bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH2_TX;
                break;
            case BL_LILAC_RDD_EMAC_ID_3:
                bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH3_TX;
                break;
            case BL_LILAC_RDD_EMAC_ID_4:
                bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH4_TX;
                break;
            default:
                bbh_peripheral_eth_tx = BBH_PERIPHERAL_ETH0_TX;
                break;
            }

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_0 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 1 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_1 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 2 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_2 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 3 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_3 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 4 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_4 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 5 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );
#if defined(DSL_63138) || defined(DSL_63148)
            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_5 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 6 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_6 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 7 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );

            MWRITE_32( eth_tx_local_registers_ptr + BL_LILAC_RDD_EMAC_ID_7 * 2 + 1, ( ( ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS + 8 * LILAC_RDD_EMAC_NUMBER_OF_QUEUES *
                                                                                        sizeof ( RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS ) ) << 16 ) | bbh_peripheral_eth_tx );
#endif /* DSL */
        }
    }

    bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

    RDD_BRIDGE_CONFIGURATION_REGISTER_INTER_LAN_SCHEDULING_MODE_WRITE ( xi_inter_lan_scheduling_mode, bridge_cfg_register );

    return ( BL_LILAC_RDD_OK );
}


/* stub function. used for G9991 */
void rdd_eth_tx_ddr_queue_addr_config ( BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                        BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id,
                                        uint32_t                   xi_ddr_address,
                                        uint16_t                   xi_queue_size,
                                        uint8_t                    xi_counter_id )
{
}										


BL_LILAC_RDD_ERROR_DTE rdd_eth_tx_queue_config ( BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                                 BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id,
                                                 uint16_t                   xi_packet_threshold,
                                                 rdd_queue_profile          xi_profile_id,
                                                 uint8_t                    xi_counter_id )
{
    RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS       *eth_tx_queue_descriptor_ptr;
    RDD_ETH_TX_QUEUES_POINTERS_TABLE_DTS  *eth_tx_queues_pointers_table_ptr;
    RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS   *eth_tx_queue_pointers_entry_ptr;
    uint16_t                              eth_tx_queue_descriptor_offset;
    uint16_t                              queue_profile_address;

    /* check the validity of the input parameters - emac id */
#ifndef G9991
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_START ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    /* check the validity of the input parameters - emac tx queue id */
    if ( xi_queue_id > BL_LILAC_RDD_QUEUE_LAST )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }

	if ( xi_emac_id == BL_LILAC_RDD_EMAC_ID_PCI )
    {
        /* PCI TX has 4 queues */
        if (  xi_queue_id > BL_LILAC_RDD_QUEUE_3 )
        {
            return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
        }

        if ( ( xi_profile_id == rdd_queue_profile_disabled ) && ( xi_packet_threshold > 0 ) )
        {
            /* PCI TX fifo imposes queue configuration larger than fifo size */
            if ( xi_packet_threshold <= LILAC_RDD_PCI_TX_FIFO_SIZE )
            {
                return ( BL_LILAC_RDD_ERROR_PCI_QUEUE_THRESHOLD_TOO_SMALL );
            }

            xi_packet_threshold -= LILAC_RDD_PCI_TX_FIFO_SIZE;
        }
    }
#else
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }
    /* check the validity of the input parameters - emac tx queue id */
    if ( xi_queue_id > BL_LILAC_RDD_QUEUE_LAST )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }
#endif

    eth_tx_queues_pointers_table_ptr = ( RDD_ETH_TX_QUEUES_POINTERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS );

    eth_tx_queue_pointers_entry_ptr = &( eth_tx_queues_pointers_table_ptr->entry[ xi_emac_id * LILAC_RDD_EMAC_NUMBER_OF_QUEUES + xi_queue_id ] );

    RDD_ETH_TX_QUEUE_POINTERS_ENTRY_TX_QUEUE_POINTER_READ ( eth_tx_queue_descriptor_offset, eth_tx_queue_pointers_entry_ptr );

    eth_tx_queue_descriptor_ptr = ( RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + eth_tx_queue_descriptor_offset );

    if ( xi_profile_id == rdd_queue_profile_disabled )
    {
        queue_profile_address = 0;
    }
    else
    {
        queue_profile_address = DS_QUEUE_PROFILE_TABLE_ADDRESS + xi_profile_id * sizeof ( RDD_QUEUE_PROFILE_DTS );
    }

    RDD_ETH_TX_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_packet_threshold, eth_tx_queue_descriptor_ptr );
    RDD_ETH_TX_QUEUE_DESCRIPTOR_PROFILE_PTR_WRITE ( queue_profile_address, eth_tx_queue_descriptor_ptr );

#if !defined(RDD_BASIC) && (defined(DSL_63138) || defined(DSL_63148))
    /*Don't update packet descriptors pool sizes in case of RDD_BASIC (CFE). In RDD_BASIC
     *case, the current function is invoked before the Runner cores are woken up, and the
     *rdd_tm_ds_free_packet_descriptors_pool_size_update() function below sends a CPU message
     *to apply the pool size changes.*/
    return rdd_tm_ds_free_packet_descriptors_pool_size_update();
#else
    return ( BL_LILAC_RDD_OK );
#endif
}


BL_LILAC_RDD_ERROR_DTE rdd_eth_tx_queue_flush ( BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                                BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id,
                                                BL_LILAC_RDD_CPU_WAIT_DTE  xi_wait )
{
    BL_LILAC_RDD_ERROR_DTE  rdd_error;
    unsigned long           flags;

    /* check the validity of the input parameters - emac id */
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    /* check the validity of the input parameters - emac tx queue id */
    if ( xi_queue_id > BL_LILAC_RDD_QUEUE_LAST )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_QUEUE_ID );
    }

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    rdd_error = f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_FLUSH_ETH_QUEUE, PICO_RUNNER_A, RUNNER_PRIVATE_0_OFFSET, xi_emac_id, xi_queue_id, 0, xi_wait );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_eth_tx_queue_get_status ( BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                                     BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id,
                                                     rdpa_stat_1way_t           *stat )
{
    BL_LILAC_RDD_ERROR_DTE rdd_error = BL_LILAC_RDD_OK;
    RDD_ETH_TX_QUEUES_TABLE_DTS      *eth_tx_queue_descriptor_table;
    RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS  *eth_tx_queue_descriptor_entry;

    eth_tx_queue_descriptor_table = ( RDD_ETH_TX_QUEUES_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_QUEUES_TABLE_ADDRESS );
    eth_tx_queue_descriptor_entry = &( eth_tx_queue_descriptor_table->entry[ xi_emac_id * LILAC_RDD_EMAC_NUMBER_OF_QUEUES + xi_queue_id ] );

#if !defined(DSL_63138) && !defined(DSL_63148)
    RDD_ETH_TX_QUEUE_DESCRIPTOR_INGRESS_PACKET_COUNTER_READ ( (stat->passed.packets), eth_tx_queue_descriptor_entry );
#else
    {    
      uint8_t  tx_queue;
      uint16_t discarded_packets;
    
      RDD_ETH_TX_QUEUE_DESCRIPTOR_INDEX_READ ( tx_queue, eth_tx_queue_descriptor_entry );
    
      rdd_error = rdd_4_bytes_counter_get ( LAN_TX_QUEUES_PACKETS_GROUP, tx_queue, &stat->passed.packets );
      if ( rdd_error != BL_LILAC_RDD_OK )
        return ( rdd_error );

      rdd_error = rdd_4_bytes_counter_get ( LAN_TX_QUEUES_BYTES_GROUP, tx_queue, &stat->passed.bytes );
      if ( rdd_error != BL_LILAC_RDD_OK )
        return ( rdd_error );

      rdd_error = rdd_2_bytes_counter_get ( LAN_TX_QUEUES_DROPPED_PACKETS_GROUP, tx_queue, &discarded_packets );
      stat->discarded.packets = discarded_packets;

      if ( rdd_error != BL_LILAC_RDD_OK )
        return ( rdd_error );

      rdd_error = rdd_4_bytes_counter_get ( LAN_TX_QUEUES_DROPPED_BYTES_GROUP, tx_queue, &stat->discarded.bytes );
    }
#endif

    return ( rdd_error );
}


/******************************************************************************/
/*                                                                            */
/*   set the timer for upstream rate control budget allocation, in each       */
/*   iteration 16 rate controllers get budget, this API should be called only */
/*   when at least one wan channel is working at rate control scheduling.     */
/*                                                                            */
/******************************************************************************/
static BL_LILAC_RDD_ERROR_DTE f_rdd_ds_rate_shaper_timer_set ( void )
{
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET           runner_timer_target_register;
    RUNNER_REGS_CFG_CPU_WAKEUP             runner_cpu_wakeup_register;

    RUNNER_REGS_0_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

    runner_timer_target_register.timer_0_2 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_0_2_PICO_CORE_VALUE;

    RUNNER_REGS_0_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );
#endif

    bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );
#if !defined(FIRMWARE_INIT)
    /* budget allocation is taking place every 512 micro second, 16 rate shapers each cycle allocation */
    RDD_BRIDGE_CONFIGURATION_REGISTER_DS_RATE_SHAPER_TIMER_WRITE ( BL_LILAC_RDD_DS_RATE_SHAPER_TIMER_INTERVAL / 8 - 1, bridge_cfg_register );

    runner_cpu_wakeup_register.req_trgt = RATE_SHAPER_BUDGET_ALLOCATOR_THREAD_NUMBER / 32;
    runner_cpu_wakeup_register.thread_num = RATE_SHAPER_BUDGET_ALLOCATOR_THREAD_NUMBER % 32;
    runner_cpu_wakeup_register.urgent_req = LILAC_RDD_FALSE;

    RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
#else
    RDD_BRIDGE_CONFIGURATION_REGISTER_DS_RATE_SHAPER_TIMER_WRITE ( 8, bridge_cfg_register );
#endif
    return ( BL_LILAC_RDD_OK );
}


static void rdd_ds_rate_shaper_params_set ( uint32_t               xi_rate_shaper_id,                                                                    
                                            RDD_RATE_LIMIT_PARAMS  *xi_budget )
{
    RDD_DS_RATE_SHAPERS_TABLE_DTS      *rate_shapers_table_ptr;
    RDD_DS_RATE_SHAPER_DESCRIPTOR_DTS  *rate_shaper_descriptor_ptr;
    uint32_t                           rate;
    uint32_t                           limit;
    uint32_t                           exponent_index;
    uint32_t                           exponent_list[] = { 0, DS_RATE_SHAPER_EXPONENT };

    rate_shapers_table_ptr = ( RDD_DS_RATE_SHAPERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_SHAPERS_TABLE_ADDRESS );

    rate_shaper_descriptor_ptr = &( rate_shapers_table_ptr->entry[ xi_rate_shaper_id ] );

    /* clear budget */
    RDD_DS_RATE_SHAPER_DESCRIPTOR_CURRENT_SUSTAIN_BUDGET_WRITE ( 0, rate_shaper_descriptor_ptr );

    /* devise mantissa and exponent of maximum budget and write them to memory */
    exponent_index = rdd_get_exponent ( xi_budget->limit, 15, 2, exponent_list );

    limit = xi_budget->limit >> exponent_list[ exponent_index ];

    RDD_DS_RATE_SHAPER_DESCRIPTOR_MAXIMUM_BUDGET_WRITE ( limit, rate_shaper_descriptor_ptr );
    RDD_DS_RATE_SHAPER_DESCRIPTOR_MAXIMUM_BUDGET_EXPONENT_WRITE ( exponent_index, rate_shaper_descriptor_ptr );

    /* devise mantissa and exponent of sustain budget and write them to memory */
    rate = rdd_budget_to_alloc_unit ( xi_budget->rate, BL_LILAC_RDD_DS_RATE_SHAPER_TIMER_INTERVAL, 0 );

    exponent_index = rdd_get_exponent ( rate, 15, 2, exponent_list );

    rate >>= exponent_list[ exponent_index ];

    RDD_DS_RATE_SHAPER_DESCRIPTOR_ALLOCATED_SUSTAIN_BUDGET_WRITE ( rate, rate_shaper_descriptor_ptr );
    RDD_DS_RATE_SHAPER_DESCRIPTOR_ALLOCATED_SUSTAIN_BUDGET_EXPONENT_WRITE ( exponent_index, rate_shaper_descriptor_ptr );

    return;
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_rate_shaper_config ( uint32_t                  xi_rate_shaper_id,                                                                    
                                                   RDD_RATE_LIMIT_PARAMS     *xi_budget,
                                                   BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id )
{
    RDD_DS_RATE_SHAPERS_TABLE_DTS      *rate_shapers_table_ptr;
    RDD_DS_RATE_SHAPER_DESCRIPTOR_DTS  *rate_shaper_descriptor_ptr;
    RDD_BUDGET_ALLOCATOR_TABLE_DTS     *budget_allocator_table_ptr;
    RDD_BUDGET_ALLOCATOR_ENTRY_DTS     *budget_allocator_entry_ptr;
    uint32_t                           mac_descriptor_address;
    uint16_t                           active_rate_shapers;
    static uint32_t                    api_first_time_call = LILAC_RDD_TRUE;

    /* check the validity of the input parameters - emac and rate shaper id */
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    if ( xi_rate_shaper_id >= RDD_DS_RATE_SHAPERS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    rate_shapers_table_ptr = ( RDD_DS_RATE_SHAPERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_SHAPERS_TABLE_ADDRESS );

    rate_shaper_descriptor_ptr = &( rate_shapers_table_ptr->entry[ xi_rate_shaper_id ] );

    /* pointer to emac descriptor */
    mac_descriptor_address = ETH_TX_MAC_TABLE_ADDRESS + xi_emac_id * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS );

    RDD_DS_RATE_SHAPER_DESCRIPTOR_EMAC_DESCRIPTOR_PTR_WRITE ( mac_descriptor_address, rate_shaper_descriptor_ptr );

    rdd_ds_rate_shaper_params_set ( xi_rate_shaper_id, xi_budget );

    /* budget allocator optimization - spread allocation between 8 allocation groups */
    /* (consecutive rate shapers are mapped to different allocation groups)          */
    budget_allocator_table_ptr = ( RDD_BUDGET_ALLOCATOR_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_SHAPER_BUDGET_ALLOCATOR_TABLE_ADDRESS );

    budget_allocator_entry_ptr = &( budget_allocator_table_ptr->entry[ xi_rate_shaper_id % 8 ] );

    RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_READ ( active_rate_shapers, budget_allocator_entry_ptr );

    active_rate_shapers |= ( 1 << ( xi_rate_shaper_id / 8 ) );

    RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_WRITE ( active_rate_shapers, budget_allocator_entry_ptr );

    if ( api_first_time_call )
    {
        f_rdd_ds_rate_shaper_timer_set ();

        api_first_time_call = LILAC_RDD_FALSE;
    }

    /* mark the rate controller as configured */
    g_downstream_rate_shaper[ xi_rate_shaper_id ] = LILAC_RDD_TRUE;
    
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_rate_shaper_modify ( uint32_t               xi_rate_shaper_id,                                                                    
                                                   RDD_RATE_LIMIT_PARAMS  *xi_budget )
{
    /* check the validity of the input parameters - rate shaper id */
    if ( xi_rate_shaper_id >= RDD_DS_RATE_SHAPERS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    /* verify that the rate shaper was configured before */
    if ( g_downstream_rate_shaper[ xi_rate_shaper_id ] == LILAC_RDD_FALSE )
    {
        return ( BL_LILAC_RDD_ERROR_RATE_SHAPER_NOT_CONFIGURED );
    }

    rdd_ds_rate_shaper_params_set ( xi_rate_shaper_id, xi_budget );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_rate_shaper_remove ( uint32_t                  xi_rate_shaper_id,
                                                   BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id )
{
    RDD_DS_RATE_SHAPERS_TABLE_DTS      *rate_shapers_table_ptr;
    RDD_DS_RATE_SHAPER_DESCRIPTOR_DTS  *rate_shaper_descriptor_ptr;
    RDD_BUDGET_ALLOCATOR_TABLE_DTS     *budget_allocator_table_ptr;
    RDD_BUDGET_ALLOCATOR_ENTRY_DTS     *budget_allocator_entry_ptr;
    uint16_t                           active_rate_shapers;

    /* check the validity of the input parameters - emac and rate shaper id */
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    if ( xi_rate_shaper_id >= RDD_DS_RATE_SHAPERS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    /* verify that the rate shaper was configured before */
    if ( g_downstream_rate_shaper[ xi_rate_shaper_id ] == LILAC_RDD_FALSE )
    {
        return ( BL_LILAC_RDD_ERROR_RATE_SHAPER_NOT_CONFIGURED );
    }

    /* disable budget allocation */
    budget_allocator_table_ptr = ( RDD_BUDGET_ALLOCATOR_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_SHAPER_BUDGET_ALLOCATOR_TABLE_ADDRESS );

    budget_allocator_entry_ptr = &( budget_allocator_table_ptr->entry[ xi_rate_shaper_id % 8 ] );

    RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_READ ( active_rate_shapers, budget_allocator_entry_ptr );

    active_rate_shapers &= ~( 1 << ( xi_rate_shaper_id / 8 ) );

    RDD_BUDGET_ALLOCATOR_ENTRY_ACTIVE_RATE_CONTROLLERS_WRITE ( active_rate_shapers, budget_allocator_entry_ptr );

    /* clear sustain rate and maximum budget (for debug) */
    rate_shapers_table_ptr = ( RDD_DS_RATE_SHAPERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_SHAPERS_TABLE_ADDRESS );

    rate_shaper_descriptor_ptr = &( rate_shapers_table_ptr->entry[ xi_rate_shaper_id ] );

    RDD_DS_RATE_SHAPER_DESCRIPTOR_ALLOCATED_SUSTAIN_BUDGET_WRITE ( 0, rate_shaper_descriptor_ptr );
    RDD_DS_RATE_SHAPER_DESCRIPTOR_ALLOCATED_SUSTAIN_BUDGET_EXPONENT_WRITE ( 0, rate_shaper_descriptor_ptr );
    RDD_DS_RATE_SHAPER_DESCRIPTOR_MAXIMUM_BUDGET_WRITE ( 0, rate_shaper_descriptor_ptr );
    RDD_DS_RATE_SHAPER_DESCRIPTOR_MAXIMUM_BUDGET_EXPONENT_WRITE ( 0, rate_shaper_descriptor_ptr );

    /* mark the rate shaper as disabled */
    g_downstream_rate_shaper[ xi_rate_shaper_id ] = LILAC_RDD_FALSE;

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_rate_shaper_queue_config ( uint32_t  xi_rate_shaper_id,
                                                         uint32_t  xi_packet_threshold )
{
    RDD_DS_RATE_SHAPERS_TABLE_DTS      *rate_shapers_table_ptr;
    RDD_DS_RATE_SHAPER_DESCRIPTOR_DTS  *rate_shaper_descriptor_ptr;
    RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS    *eth_tx_queue_descriptor_ptr;
    uint32_t                           eth_tx_queue_address;

    if ( xi_rate_shaper_id >= RDD_DS_RATE_SHAPERS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    rate_shapers_table_ptr = ( RDD_DS_RATE_SHAPERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_SHAPERS_TABLE_ADDRESS );

    rate_shaper_descriptor_ptr = &( rate_shapers_table_ptr->entry[ xi_rate_shaper_id ] );

    eth_tx_queue_address = ETH_TX_RS_QUEUE_DESCRIPTOR_TABLE_ADDRESS + xi_rate_shaper_id * sizeof ( RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS );

    eth_tx_queue_descriptor_ptr = ( RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + eth_tx_queue_address );

    RDD_ETH_TX_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_packet_threshold, eth_tx_queue_descriptor_ptr );

    /* connect the queue to the rate shaper */
    RDD_DS_RATE_SHAPER_DESCRIPTOR_TX_QUEUE_PTR_WRITE ( eth_tx_queue_address, rate_shaper_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_rate_shaper_queue_modify ( uint32_t  xi_rate_shaper_id,
                                                         uint32_t  xi_packet_threshold )
{
    RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS  *eth_tx_queue_descriptor_ptr;
    uint32_t                         eth_tx_queue_address;

    if ( xi_rate_shaper_id >= RDD_DS_RATE_SHAPERS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    eth_tx_queue_address = ETH_TX_RS_QUEUE_DESCRIPTOR_TABLE_ADDRESS + xi_rate_shaper_id * sizeof ( RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS );

    eth_tx_queue_descriptor_ptr = ( RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + eth_tx_queue_address );

    RDD_ETH_TX_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_WRITE ( xi_packet_threshold, eth_tx_queue_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_rate_shaper_queue_remove ( uint32_t  xi_rate_shaper_id )
{
    RDD_DS_RATE_SHAPERS_TABLE_DTS      *rate_shapers_table_ptr;
    RDD_DS_RATE_SHAPER_DESCRIPTOR_DTS  *rate_shaper_descriptor_ptr;

    if ( xi_rate_shaper_id >= RDD_DS_RATE_SHAPERS_TABLE_SIZE )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    rate_shapers_table_ptr = ( RDD_DS_RATE_SHAPERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_RATE_SHAPERS_TABLE_ADDRESS );

    rate_shaper_descriptor_ptr = &( rate_shapers_table_ptr->entry[ xi_rate_shaper_id ] );

    RDD_DS_RATE_SHAPER_DESCRIPTOR_TX_QUEUE_PTR_WRITE ( ETH_TX_QUEUE_DUMMY_DESCRIPTOR_ADDRESS, rate_shaper_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_queue_profile_config ( rdpa_traffic_dir   xi_direction,
                                                  rdd_queue_profile  xi_profile_id,
                                                  RDD_QUEUE_PROFILE  *xi_queue_profile )
{
    RDD_DS_QUEUE_PROFILE_TABLE_DTS  *queue_profile_table_ptr;
    RDD_QUEUE_PROFILE_DTS        *queue_profile_ptr;
    uint32_t                     max_drop_probability;
    uint32_t                     low_red_interval_length;
    uint32_t                     high_red_interval_length;

    if ( xi_direction == rdpa_dir_ds )
    {
        queue_profile_table_ptr = ( RDD_DS_QUEUE_PROFILE_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_QUEUE_PROFILE_TABLE_ADDRESS );
    }
    else
    {
        queue_profile_table_ptr = ( RDD_DS_QUEUE_PROFILE_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_QUEUE_PROFILE_TABLE_ADDRESS );
    }

    queue_profile_ptr = &( queue_profile_table_ptr->entry[ xi_profile_id ] );

    RDD_QUEUE_PROFILE_MAX_LOW_THRESHOLD_WRITE ( xi_queue_profile->low_priority_class.max_threshold, queue_profile_ptr );
    RDD_QUEUE_PROFILE_MIN_HIGH_THRESHOLD_WRITE ( xi_queue_profile->high_priority_class.min_threshold, queue_profile_ptr );
    RDD_QUEUE_PROFILE_MAX_HIGH_THRESHOLD_WRITE ( xi_queue_profile->high_priority_class.max_threshold, queue_profile_ptr );

    max_drop_probability = xi_queue_profile->max_drop_probability * 65535 / 100;
    low_red_interval_length = xi_queue_profile->low_priority_class.max_threshold - xi_queue_profile->low_priority_class.min_threshold;
    high_red_interval_length = xi_queue_profile->high_priority_class.max_threshold - xi_queue_profile->high_priority_class.min_threshold;

    if ( low_red_interval_length > 0 )
    {
        RDD_QUEUE_PROFILE_LOW_DROP_CONSTANT_WRITE ( max_drop_probability / low_red_interval_length, queue_profile_ptr );
        RDD_QUEUE_PROFILE_LOW_LARGE_INTERVAL_FLAG_WRITE ( ( ( low_red_interval_length > 0xFF ) ? LILAC_RDD_TRUE : LILAC_RDD_FALSE ), queue_profile_ptr );
    }

    if ( high_red_interval_length > 0 )
    {
        RDD_QUEUE_PROFILE_HIGH_DROP_CONSTANT_WRITE ( max_drop_probability / high_red_interval_length, queue_profile_ptr );
        RDD_QUEUE_PROFILE_HIGH_LARGE_INTERVAL_FLAG_WRITE ( ( ( high_red_interval_length > 0xFF ) ? LILAC_RDD_TRUE : LILAC_RDD_FALSE ), queue_profile_ptr );
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_us_quasi_policer_config ( BL_LILAC_RDD_BRIDGE_PORT_DTE  xi_bridge_port,
                                                     uint32_t                      xi_allocated_budget )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET  runner_timer_target_register;
#endif
    RDD_US_RATE_LIMITER_TABLE_DTS    *rate_limiter_table_ptr;
    RDD_RATE_LIMITER_ENTRY_DTS    *rate_limiter_entry_ptr;
    uint32_t                      bridge_port_index;
    static uint32_t               api_first_time_call = LILAC_RDD_TRUE;
    uint32_t                      exponent_index;
    uint32_t                      exponent_list[] = { 0, UPSTREAM_QUASI_POLICER_EXPONENT };

    rate_limiter_table_ptr = ( RDD_US_RATE_LIMITER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_RATE_LIMITER_TABLE_ADDRESS );

    bridge_port_index = rdd_bridge_port_to_port_index ( xi_bridge_port, 0 );

    rate_limiter_entry_ptr = &( rate_limiter_table_ptr->entry[ bridge_port_index ] );

    /* devise mantissa and exponent of allocated budget and write them to memory */
    xi_allocated_budget = rdd_budget_to_alloc_unit ( xi_allocated_budget, UPSTREAM_QUASI_POLICER_TIMER_PERIOD, 0 );

    exponent_index = rdd_get_exponent ( xi_allocated_budget, 15, 2, exponent_list );

    xi_allocated_budget = xi_allocated_budget >> exponent_list[ exponent_index ];

    RDD_RATE_LIMITER_ENTRY_BUDGET_LIMIT_WRITE ( xi_allocated_budget, rate_limiter_entry_ptr );
    RDD_RATE_LIMITER_ENTRY_BUDGET_LIMIT_EXP_WRITE ( exponent_index, rate_limiter_entry_ptr );

    RDD_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_WRITE ( xi_allocated_budget, rate_limiter_entry_ptr );
    RDD_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_EXP_WRITE ( exponent_index, rate_limiter_entry_ptr );

    if ( api_first_time_call )
    {
#if !defined(FIRMWARE_INIT)
        RUNNER_REGS_1_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

        runner_timer_target_register.timer_4_6 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_4_6_PICO_CORE_VALUE;

        RUNNER_REGS_1_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

        rdd_timer_task_config ( rdpa_dir_us, UPSTREAM_QUASI_POLICER_TIMER_PERIOD, UPSTREAM_QUASI_BUDGET_ALLOCATE_CODE_ID );
#else
        rdd_timer_task_config ( rdpa_dir_us, 100, UPSTREAM_QUASI_BUDGET_ALLOCATE_CODE_ID );
#endif

        api_first_time_call = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_emac_rate_limiter_config ( RDD_RATE_LIMITER_ID_DTE  xi_rate_limiter_id,
                                                      RDD_RATE_LIMIT_PARAMS    *xi_budget )
{
    if ( xi_rate_limiter_id > RDD_RATE_LIMITER_EMAC_LAST )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    f_ds_rate_limiter_config ( xi_rate_limiter_id, xi_budget );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_emac_queues_0_3_rate_limit_mode_config ( BL_LILAC_RDD_FILTER_MODE_DTE  xi_emac_queues_0_3_mode )
{
#if !defined(DSL_63138) && !defined(DSL_63148)
    RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS       *eth_tx_queue_descriptor_ptr;
    RDD_ETH_TX_QUEUES_POINTERS_TABLE_DTS  *eth_tx_queues_pointers_table_ptr;
    RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS   *eth_tx_queue_pointers_entry_ptr;
    BL_LILAC_RDD_EMAC_ID_DTE              emac_id;
    BL_LILAC_RDD_QUEUE_ID_DTE             tx_queue_id;
    uint16_t                              eth_tx_queue_descriptor_offset;

    eth_tx_queues_pointers_table_ptr = ( RDD_ETH_TX_QUEUES_POINTERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS );

    for ( emac_id = BL_LILAC_RDD_EMAC_ID_0 ; emac_id < BL_LILAC_RDD_EMAC_ID_COUNT; emac_id++ )
    {
        for ( tx_queue_id = BL_LILAC_RDD_QUEUE_0; tx_queue_id <= BL_LILAC_RDD_QUEUE_3; tx_queue_id++ )
        {
            eth_tx_queue_pointers_entry_ptr = &( eth_tx_queues_pointers_table_ptr->entry[ emac_id * LILAC_RDD_EMAC_NUMBER_OF_QUEUES + tx_queue_id ] );

            RDD_ETH_TX_QUEUE_POINTERS_ENTRY_TX_QUEUE_POINTER_READ ( eth_tx_queue_descriptor_offset, eth_tx_queue_pointers_entry_ptr );

            eth_tx_queue_descriptor_ptr = ( RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + eth_tx_queue_descriptor_offset );

            RDD_ETH_TX_QUEUE_DESCRIPTOR_RATE_LIMIT_OVERRIDE_WRITE ( xi_emac_queues_0_3_mode, eth_tx_queue_descriptor_ptr );
        }
    }
#endif

    return ( BL_LILAC_RDD_OK );
}

BL_LILAC_RDD_ERROR_DTE rdd_us_overall_rate_limiter_config ( RDD_RATE_LIMIT_PARAMS  *xi_budget )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET  runner_timer_target_register;
#endif
    RDD_US_RATE_LIMITER_TABLE_DTS    *rate_limiter_table_ptr;
    RDD_RATE_LIMITER_ENTRY_DTS    *rate_limiter_entry_ptr;
    static uint32_t               api_first_time_call = LILAC_RDD_TRUE;
    uint32_t                      rate;
    uint32_t                      limit;
    uint32_t                      exponent_index;
    uint32_t                      exponent_list[] = { 0, UPSTREAM_RATE_LIMITER_EXPONENT };
    
    rate_limiter_table_ptr = ( RDD_US_RATE_LIMITER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_RATE_LIMITER_TABLE_ADDRESS );

    rate_limiter_entry_ptr = &( rate_limiter_table_ptr->entry[ UPSTREAM_RATE_LIMITER_ID ] );

    if ( xi_budget->rate == 0 )
    {
        /* stop limiting immediately */
        RDD_RATE_LIMITER_ENTRY_CURRENT_BUDGET_WRITE ( 0xFFFFFFFF, rate_limiter_entry_ptr );
    }

    /* devise mantissa and exponent of budget limit and write them to memory */
    exponent_index = rdd_get_exponent ( xi_budget->limit, 15, 2, exponent_list );

    limit = xi_budget->limit >> exponent_list[ exponent_index ];

    RDD_RATE_LIMITER_ENTRY_BUDGET_LIMIT_WRITE ( limit, rate_limiter_entry_ptr );
    RDD_RATE_LIMITER_ENTRY_BUDGET_LIMIT_EXP_WRITE ( exponent_index, rate_limiter_entry_ptr );

    /* devise mantissa and exponent of allocated budget and write them to memory */
    rate = rdd_budget_to_alloc_unit ( xi_budget->rate, UPSTREAM_RATE_LIMITER_TIMER_PERIOD, 0 );

    exponent_index = rdd_get_exponent ( rate, 15, 2, exponent_list );

    rate >>= exponent_list[ exponent_index ];

    RDD_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_WRITE ( rate, rate_limiter_entry_ptr );
    RDD_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_EXP_WRITE ( exponent_index, rate_limiter_entry_ptr );
	
    if ( api_first_time_call )
    {
#if !defined(FIRMWARE_INIT)
        RUNNER_REGS_1_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

        runner_timer_target_register.timer_4_6 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_4_6_PICO_CORE_VALUE;

        RUNNER_REGS_1_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

        rdd_timer_task_config ( rdpa_dir_us, UPSTREAM_RATE_LIMITER_TIMER_PERIOD, UPSTREAM_RATE_LIMITER_BUDGET_ALLOCATE_CODE_ID );
#else
        rdd_timer_task_config ( rdpa_dir_us, 400, UPSTREAM_RATE_LIMITER_BUDGET_ALLOCATE_CODE_ID );
#endif

        api_first_time_call = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_mirroring_config ( rdpa_traffic_dir              xi_direction,
                                                  BL_LILAC_RDD_FILTER_MODE_DTE  xi_filter_mode,
                                                  BL_LILAC_RDD_EMAC_ID_DTE      xi_emac_id )
{
    BL_LILAC_RDD_ERROR_DTE                 rdd_error;
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;
#if defined(FIRMWARE_INIT)
    uint32_t                               *global_register_init_ptr;
    uint32_t                               global_register;
#else
    unsigned long                          flags;
#endif
    uint8_t                                global_ingress_config;

    rdd_error = BL_LILAC_RDD_OK;

    if ( xi_direction == rdpa_dir_ds )
    {
        bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

        RDD_BRIDGE_CONFIGURATION_REGISTER_GLOBAL_INGRESS_CONFIG_READ ( global_ingress_config, bridge_cfg_register );

        if ( xi_filter_mode )
        {
            global_ingress_config |= ( BL_LILAC_RDD_FILTER_ENABLE << 0 );
        }
        else
        {
            global_ingress_config &= ~( BL_LILAC_RDD_FILTER_ENABLE << 0 );
        }

        RDD_BRIDGE_CONFIGURATION_REGISTER_GLOBAL_INGRESS_CONFIG_WRITE ( global_ingress_config, bridge_cfg_register );
    }
    else
    {
        bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

#if defined(FIRMWARE_INIT)
        global_register_init_ptr = ( uint32_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_FAST_RUNNER_GLOBAL_REGISTERS_INIT_ADDRESS + 7 * sizeof ( uint32_t ) );
#else
        f_rdd_lock_irq ( &int_lock_irq, &flags );

        rdd_error = f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_MIRRORING_MODE_CONFIG, FAST_RUNNER_B, RUNNER_PRIVATE_1_OFFSET, xi_filter_mode, 0, 0, BL_LILAC_RDD_WAIT );

        f_rdd_unlock_irq ( &int_lock_irq, flags );
#endif
    }

    RDD_BRIDGE_CONFIGURATION_REGISTER_MIRRORING_PORT_WRITE ( xi_emac_id, bridge_cfg_register );

#if defined(FIRMWARE_INIT)
    if ( xi_direction == rdpa_dir_us )
    {
        MREAD_32( global_register_init_ptr, global_register );

        global_register |= ( xi_filter_mode << GLOBAL_CFG_MIRRORING_MODE_BIT_OFFSET );

        MWRITE_32( global_register_init_ptr, global_register );
    }
#endif

   return ( rdd_error );
}


BL_LILAC_RDD_ERROR_DTE rdd_emac_loopback_config ( BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id,
                                                  uint8_t                   xi_loopback_mode )
{
    RDD_ETH_TX_MAC_TABLE_DTS       *eth_tx_mac_table_ptr;
    RDD_ETH_TX_MAC_DESCRIPTOR_DTS  *eth_tx_mac_descriptor_ptr;

    /* check the validity of the input parameters - emac id */
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    eth_tx_mac_table_ptr = ( RDD_ETH_TX_MAC_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_MAC_TABLE_ADDRESS );

    eth_tx_mac_descriptor_ptr = &( eth_tx_mac_table_ptr->entry[ xi_emac_id ] );

    RDD_ETH_TX_MAC_DESCRIPTOR_LOOPBACK_MODE_WRITE ( xi_loopback_mode, eth_tx_mac_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}


#if !defined(DSL_63138) && !defined(DSL_63148)
BL_LILAC_RDD_ERROR_DTE rdd_sc_get ( uint8_t  *xo_buffer,
                                    uint8_t  *xo_flag )
{
    uint8_t  *sc_buffer_ptr;

    sc_buffer_ptr = ( uint8_t * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + SC_BUFFER_ADDRESS );

    memcpy ( xo_buffer, sc_buffer_ptr, 8 );

    *xo_flag = *( uint8_t * )( sc_buffer_ptr + 9 );

    return ( BL_LILAC_RDD_OK );
}
#endif /* !DSL */


BL_LILAC_RDD_ERROR_DTE rdd_drop_precedence_config ( rdpa_traffic_dir  xi_direction,
		                                            uint16_t          xi_eligibility_vector )
{
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;

    if ( xi_direction == rdpa_dir_ds )
    {
        /* downstream and local switching */
        bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

        RDD_BRIDGE_CONFIGURATION_REGISTER_DROP_PRECEDENCE_ELIGIBILITY_VECTOR_WRITE ( xi_eligibility_vector, bridge_cfg_register );


        bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

        RDD_BRIDGE_CONFIGURATION_REGISTER_PCI_LS_DP_ELIGIBILITY_VECTOR_WRITE ( xi_eligibility_vector, bridge_cfg_register );
    }
    else
    {
        /* upstream */
        bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

        RDD_BRIDGE_CONFIGURATION_REGISTER_DROP_PRECEDENCE_ELIGIBILITY_VECTOR_WRITE ( xi_eligibility_vector, bridge_cfg_register );
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_us_ingress_rate_limiter_config ( BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id,
                                                            uint32_t                  xi_rate,
                                                            uint32_t                  xi_drop_threshold,
                                                            uint32_t                  xi_flow_control_threshold )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET        runner_timer_target_register;
#endif
    RDD_US_INGRESS_RATE_LIMITER_TABLE_DTS  *rate_limiter_table_ptr;
    RDD_INGRESS_RATE_LIMITER_ENTRY_DTS  *rate_limiter_entry_ptr;
    uint32_t                            drain_per_cycle;
    uint32_t                            mantissa;
    uint32_t                            exponent;
    uint32_t                            exponent_list[] = { 0, 1, 2, 3 };
    static uint32_t                     api_first_time_call = LILAC_RDD_TRUE;
    BL_LILAC_RDD_ERROR_DTE              rdd_status;

    /* check the validity of the input parameters - emac id */
#ifdef G9991
    /* in G999.1 case, only the first 16 SID's/Virtual ports there will be ingress rate limiting*/
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_16 ) )
#else
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
#endif
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    /* translate xi_emac_id */
    xi_emac_id = xi_emac_id - BL_LILAC_RDD_EMAC_ID_0;

    rate_limiter_table_ptr = ( RDD_US_INGRESS_RATE_LIMITER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_INGRESS_RATE_LIMITER_TABLE_ADDRESS );

    rate_limiter_entry_ptr = &( rate_limiter_table_ptr->entry[ xi_emac_id ] );

    /* if rate is zero disable the rate limiter */
    if ( xi_rate == 0 )
    {
        RDD_INGRESS_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_MANTISA_WRITE ( 0, rate_limiter_entry_ptr );
        RDD_INGRESS_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_EXPONENT_WRITE ( 0, rate_limiter_entry_ptr );

	    return ( BL_LILAC_RDD_OK );
    }

    /* derive drain budget per cycle */
    drain_per_cycle = rdd_budget_to_alloc_unit ( xi_rate, UPSTREAM_INGRESS_RATE_LIMITER_BUDGET_ALLOCATOR_TIMER_INTERVAL, 0 );

    if ( drain_per_cycle > LILAC_RDD_INGRESS_RATE_LIMITER_MAX_ALLOCATED_BUDGET )
    {
        return ( BL_LILAC_RDD_ERROR_INGRESS_RATE_LIMITER_BUDGET_TOO_LARGE );
    }

    if ( xi_drop_threshold < xi_flow_control_threshold )
    {
        return ( BL_LILAC_RDD_ERROR_INGRESS_RATE_LIMITER_FLOW_CONTROL_THRESHOLD_TOO_LARGE );
    }

    /* calculate allocated budget (drain per cycle) - mantissa and exponent */
    exponent = rdd_get_exponent ( drain_per_cycle, 14, 4, exponent_list );

    mantissa = drain_per_cycle >> exponent;

    /* configure policer parameters - drained tokens per cycle */
    RDD_INGRESS_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_MANTISA_WRITE ( mantissa, rate_limiter_entry_ptr );
    RDD_INGRESS_RATE_LIMITER_ENTRY_ALLOCATED_BUDGET_EXPONENT_WRITE ( exponent, rate_limiter_entry_ptr );

    /* drop threshold (maximum burst size) */
    RDD_INGRESS_RATE_LIMITER_ENTRY_DROP_THRESHOLD_WRITE ( xi_drop_threshold, rate_limiter_entry_ptr );

    /* pause-xoff threshold */
    if ( xi_flow_control_threshold > 0 )
    {
        RDD_INGRESS_RATE_LIMITER_ENTRY_XOFF_THRESHOLD_WRITE ( ( xi_drop_threshold - xi_flow_control_threshold ), rate_limiter_entry_ptr );
    }
    else
    {
        RDD_INGRESS_RATE_LIMITER_ENTRY_XOFF_THRESHOLD_WRITE ( ( xi_drop_threshold + 2048 ), rate_limiter_entry_ptr );
    }

    /* BBH-RX address */
    RDD_INGRESS_RATE_LIMITER_ENTRY_BBH_RX_ADDRESS_WRITE ( g_bbh_peripheral_eth_rx[ xi_emac_id ], rate_limiter_entry_ptr );

    /* activate timer task */
    if ( api_first_time_call )
    {
#if !defined(FIRMWARE_INIT)
        RUNNER_REGS_1_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

        runner_timer_target_register.timer_4_6 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_4_6_PICO_CORE_VALUE;

        RUNNER_REGS_1_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

        rdd_status = rdd_timer_task_config ( rdpa_dir_us, UPSTREAM_INGRESS_RATE_LIMITER_BUDGET_ALLOCATOR_TIMER_INTERVAL, UPSTREAM_INGRESS_RATE_LIMITER_BUDGET_ALLOCATE_CODE_ID );
#else
        rdd_status = rdd_timer_task_config ( rdpa_dir_us, 200, UPSTREAM_INGRESS_RATE_LIMITER_BUDGET_ALLOCATE_CODE_ID );
#endif

        if ( rdd_status != BL_LILAC_RDD_OK )
        {
            return ( rdd_status );
        }

        api_first_time_call = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_policer_config ( rdpa_traffic_dir             xi_direction,
                                            BL_LILAC_RDD_POLICER_ID_DTE  xi_policer_id,
                                            RDD_RATE_LIMIT_PARAMS        *xi_budget )
{
#if !defined(FIRMWARE_INIT)
    RUNNER_REGS_CFG_TIMER_TARGET           runner_timer_target_register;
    RUNNER_REGS_CFG_CPU_WAKEUP             runner_cpu_wakeup_register;
#endif
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;
    RDD_POLICER_TABLE_DTS                  *policer_table_ptr;
    RDD_POLICER_ENTRY_DTS                  *policer_entry_ptr;
    uint16_t                               active_policers_vector;
    uint32_t                               commited_rate;
    uint32_t                               exponent_index;
    uint32_t                               exponent_list[] = { 0, POLICER_EXPONENT };
    static uint32_t                        api_first_time_call[ 2 ] = { LILAC_RDD_TRUE, LILAC_RDD_TRUE };

    if ( xi_policer_id > BL_LILAC_RDD_POLICER_15 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_POLICER_ID );
    }

    if ( xi_direction == rdpa_dir_ds )
    {
        bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

        policer_table_ptr = ( RDD_POLICER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_POLICER_TABLE_ADDRESS );
 	}
    else
    {
        bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

        policer_table_ptr = ( RDD_POLICER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_POLICER_TABLE_ADDRESS );
    }

    policer_entry_ptr = &( policer_table_ptr->entry[ xi_policer_id ] );

    /* if bucket size is larger than 16 bits in alloc units, use exponent */
    commited_rate = rdd_budget_to_alloc_unit ( xi_budget->rate, POLICER_TIMER_PERIOD, 0 );
    exponent_index = rdd_get_exponent( commited_rate, 15, 2, exponent_list );

    RDD_POLICER_ENTRY_EXPONENT_WRITE ( exponent_list[ exponent_index ], policer_entry_ptr );
    RDD_POLICER_ENTRY_COMMITED_RATE_WRITE ( commited_rate >> exponent_list[ exponent_index ], policer_entry_ptr );
    RDD_POLICER_ENTRY_BUCKET_SIZE_WRITE ( xi_budget->limit >> POLICER_EXPONENT, policer_entry_ptr );
    RDD_POLICER_ENTRY_CURRENT_BUDGET_WRITE ( xi_budget->limit >> POLICER_EXPONENT, policer_entry_ptr );

    /* configure active policers vector */
    RDD_BRIDGE_CONFIGURATION_REGISTER_ACTIVE_POLICERS_VECTOR_READ ( active_policers_vector, bridge_cfg_register );

    if ( xi_budget->rate == 0 )
    {
        active_policers_vector &= ~( 1 << xi_policer_id );
    }
    else
    {
        active_policers_vector |= ( 1 << xi_policer_id );
    }

    RDD_BRIDGE_CONFIGURATION_REGISTER_ACTIVE_POLICERS_VECTOR_WRITE ( active_policers_vector, bridge_cfg_register );

    if ( api_first_time_call [ xi_direction ])
    {
#if defined(FIRMWARE_INIT)
        RDD_BRIDGE_CONFIGURATION_REGISTER_POLICERS_PERIOD_WRITE ( 100, bridge_cfg_register );
#else
        RDD_BRIDGE_CONFIGURATION_REGISTER_POLICERS_PERIOD_WRITE ( POLICER_TIMER_PERIOD - 1, bridge_cfg_register );

        if ( xi_direction == rdpa_dir_ds )
        {
            RUNNER_REGS_0_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

            runner_timer_target_register.timer_1_3 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_1_3_MAIN_CORE_VALUE;

            RUNNER_REGS_0_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

            /* activate allocator task */
            runner_cpu_wakeup_register.req_trgt = POLICER_BUDGET_ALLOCATOR_THREAD_NUMBER / 32;
            runner_cpu_wakeup_register.thread_num = POLICER_BUDGET_ALLOCATOR_THREAD_NUMBER % 32;
            RUNNER_REGS_0_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
        }
        else
        {
            RUNNER_REGS_1_CFG_TIMER_TARGET_READ ( runner_timer_target_register );

            runner_timer_target_register.timer_1_3 = RUNNER_REGS_CFG_TIMER_TARGET_TIMER_1_3_MAIN_CORE_VALUE;

            RUNNER_REGS_1_CFG_TIMER_TARGET_WRITE ( runner_timer_target_register );

            /* activate allocator task */
            runner_cpu_wakeup_register.req_trgt = POLICER_BUDGET_ALLOCATOR_THREAD_NUMBER / 32;
            runner_cpu_wakeup_register.thread_num = POLICER_BUDGET_ALLOCATOR_THREAD_NUMBER % 32;
            RUNNER_REGS_1_CFG_CPU_WAKEUP_WRITE ( runner_cpu_wakeup_register );
        }
#endif
        api_first_time_call [ xi_direction ] = LILAC_RDD_FALSE;
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_policer_drop_counter_get ( rdpa_traffic_dir             xi_direction,
                                                      BL_LILAC_RDD_POLICER_ID_DTE  xi_policer_id,
                                                      uint16_t                     *xo_drop_counter )
{
    RDD_POLICER_TABLE_DTS  *policer_table_ptr;
    RDD_POLICER_ENTRY_DTS  *policer_entry_ptr;

    if ( xi_policer_id > BL_LILAC_RDD_POLICER_15 )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_POLICER_ID );
    }

    if ( xi_direction == rdpa_dir_ds )
    {
        policer_table_ptr = ( RDD_POLICER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_POLICER_TABLE_ADDRESS );
    }
    else
    {
        policer_table_ptr = ( RDD_POLICER_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_POLICER_TABLE_ADDRESS );
    }

    policer_entry_ptr = &( policer_table_ptr->entry[ xi_policer_id ] );

    RDD_POLICER_ENTRY_DROP_COUNTER_READ ( *xo_drop_counter, policer_entry_ptr );
    RDD_POLICER_ENTRY_DROP_COUNTER_WRITE ( 0, policer_entry_ptr );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_free_packet_descriptors_pool_size_get ( uint32_t  *xo_downstream_size,
                                                                   uint32_t  *xo_upstream_size )
{
    *xo_downstream_size = RDD_DS_FREE_PACKET_DESCRIPTORS_POOL_SIZE;
    *xo_upstream_size = RDD_US_FREE_PACKET_DESCRIPTORS_POOL_SIZE;

    return ( BL_LILAC_RDD_OK );
}

#if !defined(RDD_BASIC) && (defined(DSL_63138) || defined(DSL_63148))
/*This function retrieves the number of DS queues configured by RDPA*/
static uint32_t rdd_tm_calc_num_ds_queues(void)
{
    uint32_t num_ds_queues = 0;

    RDD_ETH_TX_QUEUES_POINTERS_TABLE_DTS *eth_tx_queues_pointers_table_ptr = 
        ( RDD_ETH_TX_QUEUES_POINTERS_TABLE_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + ETH_TX_QUEUES_POINTERS_TABLE_ADDRESS );
    BL_LILAC_RDD_EMAC_ID_DTE emac_id;
    BL_LILAC_RDD_QUEUE_ID_DTE queue_id;

    /*It would be faster to maintain this info in a local data structure instead of retrieving it from RDP SRAM, but chose to avoid
     *data redundancy. This function is not performance critical.*/
    for (emac_id = BL_LILAC_RDD_EMAC_ID_START; emac_id < BL_LILAC_RDD_EMAC_ID_COUNT; emac_id++)
    {
        for (queue_id = 0; queue_id <= BL_LILAC_RDD_QUEUE_LAST; queue_id ++)
        {
            RDD_ETH_TX_QUEUE_POINTERS_ENTRY_DTS *eth_tx_queue_pointers_entry_ptr = 
                &( eth_tx_queues_pointers_table_ptr->entry[ emac_id * LILAC_RDD_EMAC_NUMBER_OF_QUEUES + queue_id ] );
            uint16_t eth_tx_queue_descriptor_offset;
            RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS *eth_tx_queue_descriptor_ptr;
            uint16_t packet_threshold;

            RDD_ETH_TX_QUEUE_POINTERS_ENTRY_TX_QUEUE_POINTER_READ ( eth_tx_queue_descriptor_offset, eth_tx_queue_pointers_entry_ptr );
            eth_tx_queue_descriptor_ptr = 
                ( RDD_ETH_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + eth_tx_queue_descriptor_offset );
            RDD_ETH_TX_QUEUE_DESCRIPTOR_PACKET_THRESHOLD_READ ( packet_threshold, eth_tx_queue_descriptor_ptr );
 
            /*Queues that are configured by RDPA have a greater-than-zero packet_threshold.
             *Unconfigured queues have a packet_threshold value of zero*/
            if (packet_threshold != 0)
            {
                ++num_ds_queues;
            }
        }
    }

    return num_ds_queues;
}

/*The downstream packet descriptor pool is divided into a guaranteed and a non-guaranteed pool. The guaranteed pool guarantees a minimal 
 *amount (DS_FREE_PACKET_DESCRIPTOR_POOL_GUARANTEED_QUEUE_THRESHOLD) of packet descriptors to each DS queue. If a queue fills up beyond this
 *guaranteed amount it gets a packet descriptor from the non-guaranteed pool, if descriptors are available (not guaranteed).
 *The guaranteed pool size is a function of the number of DS queues used in the system. All remaining resource go into the non-guaranteed pool.
 *This function updates the guaranteed and non-guaranteed pool size based on the number of DS queues used.*/
static BL_LILAC_RDD_ERROR_DTE rdd_tm_ds_free_packet_descriptors_pool_size_update (void)
{
    BL_LILAC_RDD_ERROR_DTE rdd_error = BL_LILAC_RDD_OK;
    uint32_t num_ds_queues = rdd_tm_calc_num_ds_queues();
    uint16_t new_guaranteed_pool_size, new_non_guaranteed_pool_size;
    uint32_t incr=0, delta=0;

    new_guaranteed_pool_size = (uint16_t)num_ds_queues*DS_FREE_PACKET_DESCRIPTOR_POOL_GUARANTEED_QUEUE_THRESHOLD;
    /*To ensure reasonable behavior in corner cases, the guaranteed pool size can not go below DS_FREE_PACKET_DESCRIPTOR_POOL_MIN_GUARANTEED_POOL_SIZE.*/
    if (new_guaranteed_pool_size < DS_FREE_PACKET_DESCRIPTOR_POOL_MIN_GUARANTEED_POOL_SIZE)
    {
        new_guaranteed_pool_size = DS_FREE_PACKET_DESCRIPTOR_POOL_MIN_GUARANTEED_POOL_SIZE;
    }

    new_non_guaranteed_pool_size = RDD_DS_FREE_PACKET_DESCRIPTORS_POOL_SIZE - new_guaranteed_pool_size;

    /*Runner FW works with a guaranteed/non-guaranteed pool free count which is initialized to the pool size at boot time.
     *To implement pool size changes on a live system, the new pool sizes have to be expressed as delta relative to the old pool sizes
     *so that the current free count can be incremented/decremented with that delta.
     *An alternative would have been to have the Runner work with an allocation count (instead of a free count) and a pool size threshold.
     *This method would simplify implementation of pool size updates, but would also require additional Runner cycles when enqueuing packets.
     *Since this is in the critical path chose the approach that was more efficient in terms of Runner cycles.*/
    if (new_guaranteed_pool_size > ds_free_packet_descriptors_pool_guaranteed_pool_size)
    {
        incr = 1;
        delta = new_guaranteed_pool_size - ds_free_packet_descriptors_pool_guaranteed_pool_size;
    }
    else if (new_guaranteed_pool_size < ds_free_packet_descriptors_pool_guaranteed_pool_size)
    {
        incr = 0;
        delta = ds_free_packet_descriptors_pool_guaranteed_pool_size - new_guaranteed_pool_size;
    }
  
    /*Only update if there's actually a change*/
    if (delta!=0)
    {
        unsigned long flags;

        /*Pool size increments/decrements are non-atomic operations. Doing this from the host CPU would create a race condition with
         *the Runner A Pico core operating on the same variables. To avoid this issue we send a message to the cpu_tx_downstream_pico thread.
         *This Pico thread then applies the pool size increment/decrement.*/

        f_rdd_lock_irq ( &int_lock_irq, &flags );

        rdd_error = f_rdd_cpu_tx_send_message ( LILAC_RDD_CPU_TX_MESSAGE_UPDATE_PD_POOL_QUOTA, PICO_RUNNER_A, RUNNER_PRIVATE_0_OFFSET, 
                                                incr, delta, 0, BL_LILAC_RDD_WAIT );

        f_rdd_unlock_irq ( &int_lock_irq, flags );

        if (rdd_error == BL_LILAC_RDD_OK)
        {
            ds_free_packet_descriptors_pool_guaranteed_pool_size = new_guaranteed_pool_size;
            ds_free_packet_descriptors_pool_non_guaranteed_pool_size = new_non_guaranteed_pool_size;
        }
    }

    return rdd_error;
}
#endif

BL_LILAC_RDD_ERROR_DTE rdd_mdu_mode_pointer_get ( BL_LILAC_RDD_EMAC_ID_DTE  xi_emac_id,
                                                  uint16_t                  *xo_mdu_mode_pointer )
{
#if defined(DSL_63138) || defined(DSL_63148)
    BL_LILAC_RDD_EMAC_ID_DTE broadcom_switch_emac_id = broadcom_switch_emac_id = BL_LILAC_RDD_EMAC_ID_1;

    switch(g_broadcom_switch_physical_port)
    {
    case BL_LILAC_RDD_LAN0_BRIDGE_PORT:
        broadcom_switch_emac_id = BL_LILAC_RDD_EMAC_ID_0;
        break;
 
    case BL_LILAC_RDD_LAN1_BRIDGE_PORT:
        broadcom_switch_emac_id = BL_LILAC_RDD_EMAC_ID_1;
        break;
 
    case BL_LILAC_RDD_LAN2_BRIDGE_PORT:
        broadcom_switch_emac_id = BL_LILAC_RDD_EMAC_ID_2;
        break;
 
    case BL_LILAC_RDD_LAN3_BRIDGE_PORT:
        broadcom_switch_emac_id = BL_LILAC_RDD_EMAC_ID_3;
        break;
 
    case BL_LILAC_RDD_LAN4_BRIDGE_PORT:
        broadcom_switch_emac_id = BL_LILAC_RDD_EMAC_ID_4;
        break;
 
    default:
        break;
    }
#endif /* !DSL */

    /* check the validity of the input parameters - emac id */
    if ( ( xi_emac_id < BL_LILAC_RDD_EMAC_ID_0 ) || ( xi_emac_id >= BL_LILAC_RDD_EMAC_ID_COUNT ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_EMAC_ID );
    }

    if ( g_broadcom_switch_mode )
    {
#if defined(DSL_63138) || defined(DSL_63148)
        *xo_mdu_mode_pointer = ETH_TX_MAC_TABLE_ADDRESS + broadcom_switch_emac_id * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) + LILAC_RDD_EMAC_EGRESS_COUNTER_OFFSET;
#else /* !DSL */
        *xo_mdu_mode_pointer = ETH_TX_MAC_TABLE_ADDRESS + g_broadcom_switch_physical_port * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) + LILAC_RDD_EMAC_EGRESS_COUNTER_OFFSET;
#endif /* !DSL */
    }
    else
    {
        *xo_mdu_mode_pointer = ETH_TX_MAC_TABLE_ADDRESS + xi_emac_id * sizeof ( RDD_ETH_TX_MAC_DESCRIPTOR_DTS ) + LILAC_RDD_EMAC_EGRESS_COUNTER_OFFSET;
    }

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_channel_sniffer_get_head_ptr ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                              BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                              BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id,
                                                              uint32_t                             *xo_tx_queue_head_ptr )
{
    RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS  *wan_tx_queue_descriptor_ptr;
    RDD_WAN_TX_POINTERS_ENTRY_DTS    *wan_tx_pointers_entry_ptr;
    uint16_t                         tx_queue_packet_counter;
    uint32_t                         tx_queue_head_ptr;
    unsigned long                    flags;

    wan_tx_pointers_entry_ptr = &( wan_tx_pointers_table_ptr->entry[ xi_wan_channel_id ][ xi_rate_controller_id ][ xi_queue_id ] );

    /* verify that the wan channel queue was configured before */
    if ( wan_tx_pointers_entry_ptr->wan_tx_queue_ptr == 0 )
    {
         return ( BL_LILAC_RDD_ERROR_GPON_TX_QUEUE_NOT_CONFIGURED );
    }

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    wan_tx_queue_descriptor_ptr = ( RDD_WAN_TX_QUEUE_DESCRIPTOR_DTS * )(DEVICE_ADDRESS( RUNNER_COMMON_1_OFFSET ) + wan_tx_pointers_entry_ptr->wan_tx_queue_ptr - sizeof ( RUNNER_COMMON ) );

    RDD_WAN_TX_QUEUE_DESCRIPTOR_PACKET_COUNTER_READ ( tx_queue_packet_counter, wan_tx_queue_descriptor_ptr );

    if ( tx_queue_packet_counter == 0 )
    {
        *xo_tx_queue_head_ptr = 0;

        f_rdd_unlock_irq ( &int_lock_irq, flags );
        return ( BL_LILAC_RDD_ERROR_GPON_TX_QUEUE_EMPTY );
    }

    RDD_WAN_TX_QUEUE_DESCRIPTOR_HEAD_PTR_READ ( tx_queue_head_ptr, wan_tx_queue_descriptor_ptr );

    *xo_tx_queue_head_ptr = ( uint32_t )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + tx_queue_head_ptr );

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_wan_channel_sniffer_copy_packet ( uint32_t    xi_packet_descriptor_ptr,
                                                             uint8_t     *xi_packet_buffer_ptr,
                                                             uint32_t    *xo_packet_size,
                                                             uint32_t    *xo_next_packet_descriptor_ptr )
{
    uint16_t                         tx_descriptor_skb_index;
    uint16_t                         tx_descriptor_ddr_offset;
    uint16_t                         tx_descriptor_buffer_number;
    uint16_t                         tx_descriptor_abs_flag  = 0;
    uint32_t                         packet_ddr_ptr = 0;
    unsigned long                    flags;

    if ( xi_packet_descriptor_ptr == 0 )
    {
        return ( BL_LILAC_RDD_ERROR_GPON_SNIFFER_NULL_PD_PTR );
    }

    f_rdd_lock_irq ( &int_lock_irq, &flags );

    RDD_BBH_TX_DESCRIPTOR_ABSOLUTE_NORMAL_READ ( tx_descriptor_abs_flag, xi_packet_descriptor_ptr );

    if ( tx_descriptor_abs_flag == 0 )
    {
        RDD_BBH_TX_DESCRIPTOR_BUFFER_NUMBER_READ ( tx_descriptor_buffer_number, xi_packet_descriptor_ptr );
        RDD_BBH_TX_DESCRIPTOR_PAYLOAD_OFFSET_READ ( tx_descriptor_ddr_offset, xi_packet_descriptor_ptr );

        packet_ddr_ptr = g_runner_ddr_base_addr + tx_descriptor_buffer_number * LILAC_RDD_RUNNER_PACKET_BUFFER_SIZE + tx_descriptor_ddr_offset * 2;
    }
    else
    {
        RDD_BBH_TX_DESCRIPTOR_ABS_ADDRESS_INDEX_READ ( tx_descriptor_skb_index, xi_packet_descriptor_ptr );

        packet_ddr_ptr = g_cpu_tx_data_pointers_reference_array[tx_descriptor_skb_index];
    }

    RDD_BBH_TX_DESCRIPTOR_PACKET_LENGTH_READ ( *xo_packet_size, xi_packet_descriptor_ptr );

    ( *xo_packet_size ) -= 4;

    MREAD_BLK_8( xi_packet_buffer_ptr, ( uint8_t * )packet_ddr_ptr, *xo_packet_size );

    MREAD_16( xi_packet_descriptor_ptr, *xo_next_packet_descriptor_ptr );

    if ( *xo_next_packet_descriptor_ptr != 0 )
    {
        *xo_next_packet_descriptor_ptr = ( ( uint32_t )( DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + ( *xo_next_packet_descriptor_ptr & 0xFFF8 ) ) );
    }

    f_rdd_unlock_irq ( &int_lock_irq, flags );
    return ( BL_LILAC_RDD_OK );
}


void rdd_ds_policers_mode_config ( rdd_policer_mode  xi_policer_mode )
{
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;

    bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

    RDD_BRIDGE_CONFIGURATION_REGISTER_DS_INGRESS_POLICERS_MODE_WRITE ( xi_policer_mode, bridge_cfg_register );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_service_queue_overall_rate_limiter_set ( BL_LILAC_RDD_RATE_LIMITER_MODE_DTE  xi_overall_rate_limiter_mode )
{
    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS  *ds_service_tm_descriptor_ptr;

    ds_service_tm_descriptor_ptr = ( RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_SERVICE_TM_DESCRIPTOR_ADDRESS );

    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_OVERALL_RATE_LIMITER_MODE_WRITE ( xi_overall_rate_limiter_mode, ds_service_tm_descriptor_ptr );

    return ( BL_LILAC_RDD_OK );
}


BL_LILAC_RDD_ERROR_DTE rdd_ds_service_queue_rate_limiter_config ( RDD_RATE_LIMITER_ID_DTE  xi_rate_limiter_id,
                                                                  RDD_RATE_LIMIT_PARAMS    *xi_budget )
{
#ifndef G9991
    RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS       *ds_service_tm_descriptor_ptr;
    uint32_t                                     rate_limiters_offset_bit;
    uint32_t                                     rate_limiters_status;

    if ( ( xi_rate_limiter_id < RDD_RATE_LIMITER_SERVICE_QUEUE_0 ) || ( xi_rate_limiter_id > RDD_RATE_LIMITER_SERVICE_QUEUE_OVERALL ) )
    {
        return ( BL_LILAC_RDD_ERROR_ILLEGAL_RATE_SHAPER_ID );
    }

    ds_service_tm_descriptor_ptr = ( RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_0_OFFSET ) + DS_SERVICE_TM_DESCRIPTOR_ADDRESS );

    if ( xi_rate_limiter_id != RDD_RATE_LIMITER_SERVICE_QUEUE_OVERALL )
    {
        rate_limiters_offset_bit =  ( xi_rate_limiter_id - RDD_RATE_LIMITER_SERVICE_QUEUE_0 ) << 2;
        RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_RATE_LIMITER_STATUS_READ ( rate_limiters_status, ds_service_tm_descriptor_ptr );
        RDD_DS_SERVICE_TM_DESCRIPTOR_ENTRY_RATE_LIMITER_STATUS_WRITE ( rate_limiters_status | rate_limiters_offset_bit, ds_service_tm_descriptor_ptr );
    }

    f_ds_rate_limiter_config ( xi_rate_limiter_id, xi_budget );
#endif
    return ( BL_LILAC_RDD_OK );
}


void rdd_rate_controller_sustain_budget_limit_config ( uint32_t  xi_sustain_budget_limit )
{
    RDD_BRIDGE_CONFIGURATION_REGISTER_DTS  *bridge_cfg_register;

    bridge_cfg_register = ( RDD_BRIDGE_CONFIGURATION_REGISTER_DTS * )(DEVICE_ADDRESS( RUNNER_PRIVATE_1_OFFSET ) + US_BRIDGE_CONFIGURATION_REGISTER_ADDRESS );

    RDD_BRIDGE_CONFIGURATION_REGISTER_US_RATE_LIMIT_SUSTAIN_BUDGET_LIMIT_WRITE ( xi_sustain_budget_limit >> 8, bridge_cfg_register );
}

void rdd_wan_tx_ddr_queue_addr_config ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                        BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                        BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id,
                                        uint32_t                             xi_ddr_address,
                                        uint16_t                             xi_queue_size,
                                        uint8_t                              xi_counter_id )
{
}


BL_LILAC_RDD_ERROR_DTE rdd_eth_tx_queue_clear_stat ( BL_LILAC_RDD_EMAC_ID_DTE   xi_emac_id,
                                                     BL_LILAC_RDD_QUEUE_ID_DTE  xi_queue_id )
{
     return ( BL_LILAC_RDD_OK );
}
BL_LILAC_RDD_ERROR_DTE rdd_wan_tx_queue_clear_stat ( RDD_WAN_CHANNEL_ID                   xi_wan_channel_id,
                                                     BL_LILAC_RDD_RATE_CONTROLLER_ID_DTE  xi_rate_controller_id,
                                                     BL_LILAC_RDD_QUEUE_ID_DTE            xi_queue_id )
{
     return ( BL_LILAC_RDD_OK );
}
