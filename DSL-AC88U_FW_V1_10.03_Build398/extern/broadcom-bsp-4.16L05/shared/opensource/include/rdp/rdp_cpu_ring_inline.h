/*
   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2013:DUAL/GPL:standard
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
    :>
*/

/******************************************************************************/
/*                                                                            */
/* File Description:                                                          */
/*                                                                            */
/* This file contains the implementation of the Runner CPU ring interface     */
/*                                                                            */
/******************************************************************************/

#ifndef _RDP_CPU_RING_INLINE_H_
#define _RDP_CPU_RING_INLINE_H_

#ifndef RDP_SIM

#include "rdp_cpu_ring.h"

#if defined(__KERNEL__)
#include "linux/prefetch.h"
#include "linux/bcm_skb_defines.h"
#include "wfd_dev.h"
#endif

#define D_WLAN_CHAINID_OFFSET 8

#if defined(__KERNEL__) || defined(_CFE_)

static inline void AssignPacketBuffertoRing(RING_DESCTIPTOR *pDescriptor, volatile CPU_RX_DESCRIPTOR *pTravel, void *pBuf)
{
   /* assign the buffer address to ring and set the ownership to runner
    * by clearing  bit 31 which is used as ownership flag */

   pTravel->word2 = swap4bytes(((VIRT_TO_PHYS(pBuf)) & 0x7fffffff));

   /* advance the head ptr, wrap around if needed*/
   if (++pDescriptor->head == pDescriptor->end) 
      pDescriptor->head = pDescriptor->base;
}

#if defined(CONFIG_BCM963138) || defined(_BCM963138_) || defined(CONFIG_BCM963148) || defined(_BCM963148_)
static inline int ReadPacketFromRing(RING_DESCTIPTOR *pDescriptor, volatile CPU_RX_DESCRIPTOR *pTravel, CPU_RX_PARAMS *rxParams)
{
   /* pTravel is in uncached mem so reading 32bits at a time into
      cached mem improves performance*/
   CPU_RX_DESCRIPTOR	    rxDesc;

   rxDesc.word2 = pTravel->word2;
   //printk("ReadPacketFromRing addr=%p ddr= %x\n",pTravel, rxDesc.word2);
   rxDesc.word2 = swap4bytes(rxDesc.word2);

   //printk("ReadPacketFromRing swapped bufaddr= %x\n", rxDesc.word2);
   if ((rxDesc.word2 & 0x80000000))
   {
      rxDesc.ownership = 0; /*clear the ownership bit */
      rxParams->data_ptr = (uint8_t *)PHYS_TO_CACHED(rxDesc.word2);

      rxDesc.word0 = pTravel->word0;
      rxDesc.word0 = swap4bytes(rxDesc.word0);

      rxParams->packet_size = rxDesc.packet_length;
      rxParams->src_bridge_port = (BL_LILAC_RDD_BRIDGE_PORT_DTE)rxDesc.source_port;
      rxParams->flow_id = rxDesc.flow_id;

#if defined(__KERNEL__)
      cache_invalidate_len_outer_first(rxParams->data_ptr, rxParams->packet_size);
#endif

      rxDesc.word1 = pTravel->word1;
      rxDesc.word1 = swap4bytes(rxDesc.word1);

      rxParams->dst_ssid = rxDesc.dst_ssid;

      rxDesc.word3 = pTravel->word3;
      rxDesc.word3 = swap4bytes(rxDesc.word3);

      if (rxDesc.is_rx_offload)
      {
          rxParams->reason            = rdpa_cpu_rx_reason_direct_queue_2; /* hardcoded */
          rxParams->free_index        = rxDesc.free_index;
          rxParams->is_rx_offload     = rxDesc.is_rx_offload;
          rxParams->is_ipsec_upstream = rxDesc.is_ipsec_upstream;
      }
      else
      {
          rxParams->reason      = (rdpa_cpu_reason)rxDesc.reason;
          rxParams->wl_metadata = rxDesc.wl_metadata;
          rxParams->ptp_index   = pTravel->ip_sync_1588_idx;
#if defined(_CFE_)
          rxParams->wl_metadata = 0;
#endif
      }

      return 0;
   }

   return BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_EMPTY;
}

#else
static inline int ReadPacketFromRing(RING_DESCTIPTOR *pDescriptor, volatile CPU_RX_DESCRIPTOR *pTravel, CPU_RX_PARAMS *rxParams)
{
   /* pTravel is in uncached mem so reading 32bits at a time into
      cached mem improves performance*/
   CPU_RX_DESCRIPTOR	    rxDesc;

   rxDesc.word2 = pTravel->word2;
   if ((rxDesc.ownership == OWNERSHIP_HOST))
   {
      rxParams->data_ptr = (uint8_t *)PHYS_TO_CACHED(rxDesc.word2);

      rxDesc.word0 = pTravel->word0;
      rxParams->packet_size = rxDesc.packet_length;
      rxParams->src_bridge_port = (BL_LILAC_RDD_BRIDGE_PORT_DTE)rxDesc.source_port;
      rxParams->flow_id = rxDesc.flow_id;

      rxDesc.word1 = pTravel->word1 ;
      rxParams->reason = (rdpa_cpu_reason)rxDesc.reason;
      rxParams->dst_ssid = rxDesc.dst_ssid;
      rxDesc.word3 = pTravel->word3 ;
      rxParams->wl_metadata = rxDesc.wl_metadata;
      rxParams->ptp_index = pTravel->ip_sync_1588_idx;

      return 0;
   }

   return BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_EMPTY;
}
#endif

#endif /* defined(__KERNEL__) || defined(_CFE_) */
#endif /* RDP_SIM */
#endif /* _RDP_CPU_RING_INLINE_H_ */
