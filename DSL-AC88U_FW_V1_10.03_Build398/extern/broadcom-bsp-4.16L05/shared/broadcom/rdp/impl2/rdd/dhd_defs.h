/*
   Copyright (c) 2015 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2015:DUAL/GPL:standard
    
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

#ifndef _DHD_DEFS_H_
#define _DHD_DEFS_H_

#define DHD_DOORBELL_IRQ_NUM                        2
#define DHD_MSG_TYPE_TX_POST                        0xF
#define DHD_MSG_TYPE_RX_POST                        0x11
#define DHD_TX_POST_FLOW_RING_DESCRIPTOR_SIZE       48
#define DHD_RX_POST_FLOW_RING_SIZE                  1024
#define DHD_TX_COMPLETE_FLOW_RING_SIZE              1024
#define DHD_RX_COMPLETE_FLOW_RING_SIZE              1024
#define DHD_DATA_LEN                                2048
#define DHD_RX_POST_RING_NUMBER                     1  
#define DHD_TX_COMPLETE_RING_NUMBER                 3
#define DHD_RX_COMPLETE_RING_NUMBER                 4
#define DHD_DATA_OFFSET                             60

/* The host DHD driver sets the Tx Post descriptor request_id field with the 32 bit
 * aligned SKB/FKB address.  Therefore, the least significant two bits are always 0.
 * These bits are used to indicate packet type.  Value 0 is SKB and value 3 is FKB.
 * The address and packet type are stored in little endian byte ordering.  This
 * causes the packet type to be in bits [25:24] instead of [1:0] on the big endian
 * Runner processor.  DHD offload firmware will use the packet type field to indicate
 * RDD host buffer (1) and Runner BPM buffer (2).
 */
#define DHD_TX_POST_BUFFER_TYPE_WIDTH               2
#define DHD_TX_POST_BUFFER_TYPE_OFFSET              24
#define DHD_TX_POST_HOST_BUFFER_BIT_OFFSET          DHD_TX_POST_BUFFER_TYPE_OFFSET
#define DHD_TX_POST_SKB_BUFFER_VALUE                0   /* 00: possible value in tx complete only */
#define DHD_TX_POST_HOST_BUFFER_VALUE               1   /* 01: possible value in tx post and tx complete */
#define DHD_TX_POST_BPM_BUFFER_VALUE                2   /* 10: possible value in tx post and tx complete */
#define DHD_TX_POST_FKB_BUFFER_VALUE                3   /* 11: possible value in tx complete only */
#define DHD_COMPLETE_OWNERSHIP_RUNNER               1   /* 01: queued on DHD Complete CPU ring */

#define DHD_TX_POST_BPM_BUFFER_CLONE_OFFSET         26  /* When set, indicates that the BPM Buffer has been cloned */
#define DHD_TX_BPM_REF_COUNTER_TAIL_OFFSET          16
#define DHD_TX_POST_EXCLUSIVE_OFFSET                27

#define DHD_MSG_TYPE_FLOW_RING_FLUSH                0
#define DHD_MSG_TYPE_FLOW_RING_SET_DISABLED         1

#define DHD_TX_POST_FLOW_RING_CACHE_SIZE            16
#define DHD_FLOW_RING_CACHE_LKP_DEPTH               CAM_SEARCH_DEPTH_16

#define DHD_FLOW_RING_DISABLED_BIT                  1 /* (1 << 1) */

#define DHD_RADIO_OFFSET_COMMON_A(index)            (DHD_RADIO_INSTANCE_COMMON_A_DATA_ADDRESS + (index * sizeof(RDD_DHD_RADIO_INSTANCE_COMMON_A_ENTRY_DTS)))
#define DHD_RADIO_OFFSET_COMMON_B(index)            (DHD_RADIO_INSTANCE_COMMON_B_DATA_ADDRESS + (index * sizeof(RDD_DHD_RADIO_INSTANCE_COMMON_B_ENTRY_DTS)))

#endif

