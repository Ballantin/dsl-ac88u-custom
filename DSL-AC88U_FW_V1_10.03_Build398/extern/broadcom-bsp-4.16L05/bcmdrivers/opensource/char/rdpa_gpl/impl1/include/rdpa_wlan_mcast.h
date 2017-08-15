/*
* <:copyright-BRCM:2015:DUAL/GPL:standard
* 
*    Copyright (c) 2015 Broadcom 
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

#ifndef _RDPA_WLAN_MCAST_H_
#define _RDPA_WLAN_MCAST_H_

#include "rdpa_cmd_list.h"

/** \defgroup mcast Multicast Flow Management
 * @{
 */

#define RDPA_WLAN_MCAST_MAX_FLOWS                  64
#define RDPA_WLAN_MCAST_MAX_DHD_STATIONS           64
#define RDPA_WLAN_MCAST_DHD_STATION_INDEX_INVALID  RDPA_WLAN_MCAST_MAX_DHD_STATIONS
#define RDPA_WLAN_MCAST_FWD_TABLE_INDEX_INVALID    RDPA_WLAN_MCAST_MAX_FLOWS
#define RDPA_WLAN_MCAST_MAX_SSID_MAC_ADDRESSES     48
#define RDPA_WLAN_MCAST_MAX_SSID_STATS             48

#define RDPA_WLAN_MCAST_SSID_MAC_ADDRESS_ENTRY_INDEX(_radio_index, _ssid) \
    ((((_radio_index) & 0x3) << 4) | ((_ssid) & 0xF))

#define RDPA_WLAN_MCAST_SSID_STATS_ENTRY_INDEX(_radio_index, _ssid) \
    ((((_radio_index) & 0x3) << 4) | ((_ssid) & 0xF))


/** DHD Station Table.\n
 */
typedef struct {
    bdmf_mac_t mac_address;          /**<  */
    uint8_t radio_index;             /**<  */
    uint8_t ssid;                    /**<  */
    uint16_t flowring_index;         /**<  */
    uint8_t tx_priority;             /**<  */
    uint8_t reference_count;         /**< (READ ONLY) */
} rdpa_wlan_mcast_dhd_station_t;

/** WLAN Multicast Forwarding Table.\n
 */
typedef struct {
    uint8_t is_proxy_enabled;        /**<  */
    uint8_t wfd_tx_priority;         /**<  */
    uint8_t wfd_0_priority;          /**<  */
    uint8_t wfd_1_priority;          /**<  */
    uint8_t wfd_2_priority;          /**<  */
    uint16_t wfd_0_ssid_vector;      /**<  */
    uint16_t wfd_1_ssid_vector;      /**<  */
    uint16_t wfd_2_ssid_vector;      /**<  */
    bdmf_index dhd_station_index;    /**<  */
    uint8_t dhd_station_count;       /**<  */
    uint8_t dhd_station_list_size;   /**<  */
    uint8_t dhd_station_list[RDPA_WLAN_MCAST_MAX_DHD_STATIONS];  /**< (READ ONLY) */
} rdpa_wlan_mcast_fwd_table_t;

/** SSID MAC Address Table.\n
 */
typedef struct {
    uint8_t radio_index;             /**<  */
    uint8_t ssid;                    /**<  */
    bdmf_mac_t mac_address;          /**<  */
    uint8_t reference_count;         /**< (READ ONLY) */
} rdpa_wlan_mcast_ssid_mac_address_t;

/** SSID Statistics Table.\n
 */
typedef struct {
    uint8_t radio_index;             /**<  */
    uint8_t ssid;                    /**<  */
    uint32_t packets;                /**< (READ ONLY) */
    uint32_t bytes;                  /**< (READ ONLY) */
} rdpa_wlan_mcast_ssid_stats_t;

/** @} end of mcast Doxygen group. */

#endif /* _RDPA_WLAN_MCAST_H_ */
