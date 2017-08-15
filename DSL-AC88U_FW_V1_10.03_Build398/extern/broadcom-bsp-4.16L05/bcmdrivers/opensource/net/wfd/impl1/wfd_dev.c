/*
* <:copyright-BRCM:2012:DUAL/GPL:standard
* 
*    Copyright (c) 2012 Broadcom 
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

/****************************************************************************/
/**                                                                        **/
/** Software unit Wlan accelerator dev                                     **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   Wlan accelerator interface.                                          **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Mediation layer between the wifi / PCI interface and the Accelerator **/
/**  (Runner/FAP)                                                          **/
/**                                                                        **/
/** Allocated requirements:                                                **/
/**                                                                        **/
/** Allocated resources:                                                   **/
/**                                                                        **/
/**   A thread.                                                            **/
/**   An interrupt.                                                        **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/


/****************************************************************************/
/******************** Operating system include files ************************/
/****************************************************************************/
#include <linux/types.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <net/route.h>
#include <linux/moduleparam.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/etherdevice.h>
#include <linux/kthread.h>
#include "wfd_dev.h"

#if defined(PKTC) && defined(CONFIG_BCM_WFD_CHAIN_SUPPORT)
#include <linux_osl_dslcpe_pktc.h>
#include <linux/bcm_skb_defines.h>
#endif

/****************************************************************************/
/***************************** Module Version *******************************/
/****************************************************************************/
static const char *version = "Wifi Forwarding Driver";

#if defined(CONFIG_BCM96838) ||defined(CONFIG_BCM96848)
/* 6838/48 platforms support 2 radios  and 1 multi-cast stream */
#define WFD_MAX_OBJECTS   3
#elif defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
/* 63138/148 platforms support 3 radios */
#define WFD_MAX_OBJECTS   3
#else
#define WFD_MAX_OBJECTS   2
#endif
#define WFD_QUEUE_TO_WFD_IDX_MASK 0x1
#define WIFI_MW_MAX_NUM_IF    ( 16 )
#define WIFI_IF_NAME_STR_LEN  ( IFNAMSIZ )
#define WLAN_CHAINID_OFFSET 8
#define WFD_INTERRUPT_COALESCING_TIMEOUT_US 500
#define WFD_INTERRUPT_COALESCING_MAX_PKT_CNT 32
#define WFD_BRIDGED_OBJECT_IDX 2
#define WFD_BRIDGED_QUEUE_IDX 2

/****************************************************************************/
/*********************** Multiple SSID FUNCTIONALITY ************************/
/****************************************************************************/
static struct net_device __read_mostly *wifi_net_devices[WIFI_MW_MAX_NUM_IF]={NULL, } ;

static struct proc_dir_entry *proc_directory, *proc_file_conf;

extern void replace_upper_layer_packet_destination( void * cb, void * napi_cb );
extern void unreplace_upper_layer_packet_destination( void );

static int wfd_tasklet_handler(void  *context);

/****************************************************************************/
/***************************** Module parameters*****************************/
/****************************************************************************/
/* Number of packets to read in each tasklet iteration */
#define NUM_PACKETS_TO_READ_MAX 128
static int num_packets_to_read = NUM_PACKETS_TO_READ_MAX;
module_param (num_packets_to_read, int, 0);
/* wifi Broadcom prefix */
static char wifi_prefix [WIFI_IF_NAME_STR_LEN] = "wl";
module_param_string (wifi_prefix, wifi_prefix, WIFI_IF_NAME_STR_LEN, 0);

#if (defined(CONFIG_BCM_RDPA)||defined(CONFIG_BCM_RDPA_MODULE)) 
#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848)
#define WFD_NUM_QUEUES_PER_WFD_INST 1
#else
/* 2 queues(Priorities low & high) per WFD Instance. Even-Low, Odd-High
   WFD 0 - Queue 8 low, 9 high
   WFD 1 - Queue 10 low, 11 high 
   WFD 2 - Queue 12 low, 13 high */
#define WFD_NUM_QUEUES_PER_WFD_INST 2
#endif
#define WFD_NUM_QUEUE_SUPPORTED (WFD_MAX_OBJECTS * WFD_NUM_QUEUES_PER_WFD_INST)
#elif (defined(CONFIG_BCM_FAP) || defined(CONFIG_BCM_FAP_MODULE))
#include "fap_dqm.h"
#endif

/* Counters */
static unsigned int gs_count_rx_bridged_packets [WIFI_MW_MAX_NUM_IF] = {0, } ;
static unsigned int gs_count_tx_packets [WIFI_MW_MAX_NUM_IF] = {0, } ;
static unsigned int gs_count_no_buffers [WFD_NUM_QUEUE_SUPPORTED] = { } ;
static unsigned int gs_count_rx_pkt [WFD_NUM_QUEUE_SUPPORTED] = { } ;
static unsigned int gs_max_rx_pkt [WFD_NUM_QUEUE_SUPPORTED] = { } ;
static unsigned int gs_count_rx_invalid_ssid_vector[WFD_NUM_QUEUE_SUPPORTED] = {};
static unsigned int gs_count_rx_no_wifi_interface[WFD_NUM_QUEUE_SUPPORTED] = {} ;
typedef uint32_t (*wfd_bulk_get_t)(unsigned long qid, unsigned long budget, void *wfd_p, void **rx_pkts);

typedef struct
{
    int  isValid;
    struct net_device *wl_dev_p;
    enumWFD_WlFwdHookType eFwdHookType; 
    bool isTxChainingReqd;
    wfd_bulk_get_t wfd_bulk_get;
    HOOK4PARM wfd_fwdHook;
    HOOK32 wfd_completeHook;
    HOOK3PARM wfd_mcastHook;
    unsigned int wl_chained_packets;
    unsigned int wl_mcast_packets;
    unsigned int count_rx_queue_packets;
    void * wfd_acc_info_p;
    unsigned int wfd_idx;
    unsigned int wfd_rx_work_avail;
    wait_queue_head_t   wfd_rx_thread_wqh;
    struct task_struct *wfd_rx_thread;
    int wfd_queue_mask;
    int wl_radio_idx;
    struct net_device *wl_if_dev[WIFI_MW_MAX_NUM_IF];
} wfd_object_t;

static wfd_object_t wfd_objects[WFD_MAX_OBJECTS];
static int wfd_objects_num=0;
static spinlock_t wfd_irq_lock[WFD_MAX_OBJECTS];
/* first Cpu ring queue - Currently pci CPU ring queues must be sequential */
static const int first_pci_queue = 8;

#define WFD_IRQ_LOCK(wfdLockIdx, flags) spin_lock_irqsave(&wfd_irq_lock[wfdLockIdx], flags)
#define WFD_IRQ_UNLOCK(wfdLockIdx, flags) spin_unlock_irqrestore(&wfd_irq_lock[wfdLockIdx], flags)

#define WFD_WAKEUP_RXWORKER(wfdIdx) do { \
            wake_up_interruptible(&wfd_objects[wfdIdx].wfd_rx_thread_wqh); \
          } while (0)

int (*send_packet_to_upper_layer)(struct sk_buff *skb) = netif_rx;
EXPORT_SYMBOL(send_packet_to_upper_layer); 
int (*send_packet_to_upper_layer_napi)(struct sk_buff *skb) = netif_receive_skb;
EXPORT_SYMBOL(send_packet_to_upper_layer_napi);
int inject_to_fastpath = 0;
EXPORT_SYMBOL(inject_to_fastpath);

static void wfd_dump(void);

/****************************************************************************/
/******************* Other software units include files *********************/
/****************************************************************************/
#if (defined(CONFIG_BCM_RDPA)||defined(CONFIG_BCM_RDPA_MODULE)) 
#include "runner_wfd_inline.h"
void (*wfd_dump_fn)(void) = 0;
#elif (defined(CONFIG_BCM_FAP) || defined(CONFIG_BCM_FAP_MODULE))
#include "fap_wfd_inline.h"
extern void (*wfd_dump_fn)(void);
#endif

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_tasklet_handler.                                                 **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wlan accelerator - tasklet handler                                   **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Reads all the packets from the Rx queue and send it to the wifi      **/
/**   interface.                                                           **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wfd_tasklet_handler(void *context)
{
    int wfdIdx = (int)context;
    int rx_pktcnt = 0;
    int qid, qidx = 0;
    wfd_object_t * wfd_p = &wfd_objects[wfdIdx];
    unsigned long flags;
    void *rx_pkts[NUM_PACKETS_TO_READ_MAX];
    uint32_t qMask = 0;

    printk("Instantiating WFD %d thread\n", wfdIdx);
    while (1)
    {
        wait_event_interruptible(wfd_p->wfd_rx_thread_wqh, wfd_p->wfd_rx_work_avail || kthread_should_stop());
        if (kthread_should_stop())
        {
            printk(KERN_INFO "kthread_should_stop detected in wfd\n");
            break;
        }

        qMask = wfd_p->wfd_queue_mask;
        /* Read from High priority queues first
           Odd bits correspond to high priority queues
           Even bits correspond to low priority queues
           Hence get the last bit set */
        qidx = __fls(qMask);
        while (qMask)
        {
            if ((qMask & (1 << qidx)) == 0)
            {
               qidx--;
               continue;
            }

            qid = wfd_get_qid(qidx);

            local_bh_disable();

            rx_pktcnt = wfd_p->wfd_bulk_get(qid, num_packets_to_read, wfd_p, rx_pkts);

            gs_count_rx_pkt[qidx] += rx_pktcnt;
            gs_max_rx_pkt[qidx] = gs_max_rx_pkt[qidx] > rx_pktcnt ? gs_max_rx_pkt[qidx] : rx_pktcnt;

            local_bh_enable();

            /* NOTE: wfd_queue_not_empty() function clears the hw dqm interrupt for FAP platforms and
               must be always executed on every iteration */
            if (wfd_queue_not_empty(qid, qidx))
            {
                if (rx_pktcnt >= num_packets_to_read)
                {
                    schedule();
                }
                /* else do nothing. Queue is not empty. Do not clear work avail flag. 
                   Let the thread complete processing the rest of the packets. */
            }
            else
            {
                /* Queue is empty: no more packets, enable interrupts */
                WFD_IRQ_LOCK(wfdIdx, flags);
                wfd_p->wfd_rx_work_avail &= (~(1 << qidx));
                WFD_IRQ_UNLOCK(wfdIdx, flags);
                wfd_int_enable(qid, qidx);
            }

            qMask &= ~(1 << qidx);
            qidx--;
        } /*for pci queue*/

#if 0
        WFD_IRQ_LOCK(flags);
        if (wfd_rx_work_avail == 0)
        {
            for (qidx = 0; qidx < number_of_queues; qidx++)
            {
               qid = wfd_get_qid(qidx);

               //Enable interrupts
               wfd_int_enable(qid);
            }
        }
#endif
    }

    return 0;
}

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wifi_mw_proc_read_func_conf                                          **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi mw - proc read                                                  **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Procfs callback method.                                              **/
/**      Called when someone reads proc command                            **/
/**   using: cat /proc/wifi_mw                                             **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/**   page  -  Buffer where we should write                                **/
/**   start -  Never used in the kernel.                                   **/
/**   off   -  Where we should start to write                              **/
/**   count -  How many character we could write.                          **/
/**   eof   -  Used to signal the end of file.                             **/
/**   data  -  Only used if we have defined our own buffer                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wifi_mw_proc_read_func_conf(char* page, char** start, off_t off, int count, int* eof, void* data)
{
    int wifi_index ;
    unsigned int count_rx_queue_packets_total=0 ;
    unsigned int count_tx_bridge_packets_total=0 ;
    int len = 0 ;

    page+=off;
    page[0]=0;

    for(wifi_index=0;wifi_index < WIFI_MW_MAX_NUM_IF;wifi_index++)
    {
        if( wifi_net_devices[wifi_index] != NULL )
        {
            len += sprintf((page+len),"WFD Registered Interface %d:%s\n",
                            wifi_index,wifi_net_devices[wifi_index]->name);
        }
    }

    /*RX-MW from WiFi queues*/
    for (wifi_index=0; wifi_index<WIFI_MW_MAX_NUM_IF; wifi_index++)
    {
        int head = 0;
        if (gs_count_rx_bridged_packets[wifi_index]!=0)
        {
            count_rx_queue_packets_total += gs_count_rx_bridged_packets[wifi_index];
            if (head == 0)
            {
                len += sprintf((page+len), "RX Bridged traffic from WiFi queues:\n");
                head = 1;
            }

            len += sprintf((page +len), "                            [WiFi %d] briged = %d\n", 
                wifi_index, gs_count_rx_bridged_packets[wifi_index]) ;
        }
    }

    /*TX-MW to bridge*/
    for (wifi_index=0; wifi_index<WIFI_MW_MAX_NUM_IF; wifi_index++)
    {
        int head = 0;
        if ( gs_count_tx_packets[wifi_index]!=0)
        {
            count_tx_bridge_packets_total += gs_count_tx_packets[wifi_index] ;
            if (head == 0)
            {
                len += sprintf((page+len), "TX to bridge:\n");
                head = 1;
            }
            len += sprintf((page+len ), "                            [WiFi %d] = %d\n",      
                wifi_index, gs_count_tx_packets[wifi_index]) ;
        }
    }

    for (wifi_index = 0 ; wifi_index < WFD_MAX_OBJECTS ;wifi_index++ )
    {
        if (wfd_objects[wifi_index].wfd_bulk_get)
        {
            len += sprintf((page+len),"\nWFD Object %d",wifi_index);
            len += sprintf((page+len), "\nwl_chained_counters       =%d", wfd_objects[wifi_index].wl_chained_packets) ;
            len += sprintf((page+len), "\nwl_mcast_counters         =%d", wfd_objects[wifi_index].wl_mcast_packets) ;
            len += sprintf((page+len), "\ncount_rx_queue_packets    =%d", wfd_objects[wifi_index].count_rx_queue_packets) ;
            len += sprintf((page+len), "\ncount_rx_pkt              =%d", gs_count_rx_pkt[wifi_index]) ;
            len += sprintf((page+len), "\nmax_rx_pkt                =%d", gs_max_rx_pkt[wifi_index]) ;
            count_rx_queue_packets_total += gs_count_rx_pkt[wifi_index];
            len += sprintf((page+len), "\nno_bpm_buffers_error      =%d", gs_count_no_buffers[wifi_index]) ;
            len += sprintf((page+len), "\nInvalid SSID vector       =%d",
                gs_count_rx_invalid_ssid_vector[wifi_index]);
            len += sprintf((page+len), "\nNo WIFI interface         =%d", gs_count_rx_no_wifi_interface[wifi_index]);
#if (defined(CONFIG_BCM_RDPA)||defined(CONFIG_BCM_RDPA_MODULE))
            len += sprintf((page+len), "\nRing Information:\n");
            len += sprintf((page+len), "\tRing Base address = 0x%pK\n",wfd_rings[wifi_index].base );
            len += sprintf((page+len), "\tRing Head address = 0x%pK\n",wfd_rings[wifi_index].head );
            len += sprintf((page+len), "\tRing Head position = %d\n",wfd_rings[wifi_index].head - wfd_rings[wifi_index].base);
#endif
            wfd_objects[wifi_index].wl_chained_packets = 0;
            wfd_objects[wifi_index].wl_mcast_packets = 0;
            wfd_objects[wifi_index].count_rx_queue_packets = 0;
        }
    }

    len += sprintf((page+len), "\nRX from WiFi queues      [SUM] = %d\n", count_rx_queue_packets_total) ;
    len += sprintf((page+len),   "TX to bridge             [SUM] = %d\n", count_tx_bridge_packets_total) ;

    memset(gs_count_rx_bridged_packets, 0, sizeof(gs_count_rx_bridged_packets));
    memset(gs_count_tx_packets, 0, sizeof(gs_count_tx_packets));
    memset(gs_count_no_buffers, 0, sizeof(gs_count_no_buffers));
    memset(gs_count_rx_pkt, 0, sizeof(gs_count_rx_pkt));
    memset(gs_max_rx_pkt, 0, sizeof(gs_max_rx_pkt));
    memset(gs_count_rx_invalid_ssid_vector, 0, sizeof(gs_count_rx_invalid_ssid_vector));
    memset(gs_count_rx_no_wifi_interface, 0, sizeof(gs_count_rx_no_wifi_interface));

    *eof = 1;
    return len;
}


/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wifi_proc_init.                                                      **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi mw - proc init                                                  **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function initialize the proc entry                               **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wifi_proc_init(void)
{
    /* make a directory in /proc */
    proc_directory = proc_mkdir("wfd", NULL) ;

    if (proc_directory==NULL) goto fail_dir ;

    /* make conf file */
    proc_file_conf = create_proc_entry("stats", 0444, proc_directory) ;

    if (proc_file_conf==NULL ) goto fail_entry ;

    /* set callback function on file */
    proc_file_conf->read_proc = wifi_mw_proc_read_func_conf ;

    return (0) ;

fail_entry:
    printk("%s %s: Failed to create proc entry in wifi_mw\n", __FILE__, __FUNCTION__);
    remove_proc_entry("wfd" ,NULL); /* remove already registered directory */

fail_dir:
    printk("%s %s: Failed to create directory wifi_mw\n", __FILE__, __FUNCTION__) ;
    return (-1) ;
}



/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   release_wfd_os_resources                                             **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function releases the OS resources                               **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**   bool - Error code:                                                   **/
/**             true - No error                                            **/
/**             false - Error                                              **/
/**                                                                        **/
/****************************************************************************/
static int release_wfd_os_resources(void)
{
    int idx;
    /* stop kernel thread */
    for (idx = 0; idx < WFD_MAX_OBJECTS; idx++)
    {
        if (wfd_objects[idx].wfd_rx_thread)
            kthread_stop(wfd_objects[idx].wfd_rx_thread);
    }

#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
    bdmf_destroy(rdpa_cpu_obj);
#endif
    return (0) ;
}



/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_dev_close                                                        **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi accelerator - close                                             **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function closes all the driver resources.                        **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static void wfd_dev_close(void)
{
    int wfdIdx;

    /* Disable the interrupt */
    for (wfdIdx = 0; wfdIdx < WFD_MAX_OBJECTS; wfdIdx++)
    {
        if (wfd_objects[wfdIdx].isValid)
        {
            wfd_unbind(wfdIdx, wfd_objects[wfdIdx].eFwdHookType);
        }
    }
     
    /* Release the OS driver resources */
    release_wfd_os_resources();

    remove_proc_entry("stats", proc_directory);

    remove_proc_entry("wfd", NULL);

#if defined(CONFIG_BCM96838) ||defined(CONFIG_BCM96848)
    /*Free PCI resources*/
    release_wfd_interfaces();
#endif
}


/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_dev_init                                                         **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi accelerator - init                                              **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function initialize all the driver resources.                    **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wfd_dev_init(void)
{
    int idx;
    if (num_packets_to_read > NUM_PACKETS_TO_READ_MAX)
    {
        printk("%s %s Invalid num_packets_to_read %d\n",__FILE__, __FUNCTION__, num_packets_to_read);    
        return -1;
    }

    /* Initialize WFD objects */
    memset((uint8_t *)wfd_objects, 0, sizeof(wfd_objects));

    for (idx = 0; idx < WFD_MAX_OBJECTS; idx++)
    {
       spin_lock_init(&wfd_irq_lock[idx]);
    }

    /* Initialize the proc interface for debugging information */
    if (wifi_proc_init()!=0)
    {
        printk("\n%s %s: wifi_proc_init() failed\n", __FILE__, __FUNCTION__) ;
        goto proc_release;
    }

    /* Initialize accelerator(Runner/FAP) specific data structures, Queues */
    if (wfd_accelerator_init() != 0)
    {
        printk("%s %s: wfd_platform_init() failed\n", __FILE__, __FUNCTION__) ;
        goto proc_release;    
    }

    wfd_dump_fn = wfd_dump;
#if defined(CONFIG_BCM_WFD_BRIDGE_SUPPORT) && (defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848))
    /*Bind instance for bridged traffic */
    if (wfd_bind(NULL, WFD_WL_FWD_HOOKTYPE_SKB, false, NULL, 0, NULL, -1) == -1)
    {
        printk("%s %s: wfd_bind() failed to bind bridged queue\n", __FILE__, __FUNCTION__) ;
        goto cleanup;    
    }

    {
        bdmf_object_handle tc_to_q_obj = NULL;
        BDMF_MATTR(qos_mattrs, rdpa_tc_to_queue_drv());
        rdpa_tc_to_queue_table_set(qos_mattrs, 0);
        rdpa_tc_to_queue_dir_set(qos_mattrs, rdpa_dir_ds);
        if (bdmf_new_and_set(rdpa_tc_to_queue_drv(), NULL, qos_mattrs, &tc_to_q_obj))
        {
            printk("%s %s: bdmf_new_and_set tc_to_queue obj failed\n", __FILE__, __FUNCTION__);
            goto cleanup;
        }

        if (rdpa_tc_to_queue_tc_map_set(tc_to_q_obj, 0, WFD_BRIDGED_QUEUE_IDX))
        {
            printk("%s %s: rdpa_tc_to_queue_tc_map_set failed, q(0)\n", __FILE__, __FUNCTION__);
            goto cleanup; 
        }
    }

#endif

    printk("\033[1m\033[34m%s is initialized!\033[0m\n", version);
        
    return 0;

#if defined(CONFIG_BCM_WFD_BRIDGE_SUPPORT) && (defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848))
cleanup:
    wfd_dump_fn = NULL;
#endif

proc_release:
    remove_proc_entry("stats", proc_directory);
    remove_proc_entry("wfd", NULL);

    return -1;
}

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_bind                                                             **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Bind the function hooks and other attributes that are needed by      **/
/**   wfd to forward packets to WLAN.                                      **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
int wfd_bind(struct net_device *wl_dev_p, 
             enumWFD_WlFwdHookType eFwdHookType, 
             bool isTxChainingReqd,
             HOOK4PARM wfd_fwdHook, 
             HOOK32 wfd_completeHook,
             HOOK3PARM wfd_mcastHook,
             int wl_radio_idx)
{
    int rc=0;
    int qid;
    int qidx;
    char threadname[15]={0};
    int tmp_idx;
    int minQIdx;
    int maxQIdx;
    int wfd_max_objects;


#if defined(CONFIG_BCM96838) ||defined(CONFIG_BCM96848)
    wfd_max_objects = WFD_MAX_OBJECTS - 1;
#else
    wfd_max_objects = WFD_MAX_OBJECTS;
#endif
    if (wfd_objects_num > wfd_max_objects)
    {
        printk("%s ERROR. WFD_MAX_OBJECTS(%d) limit reached\n", __FUNCTION__, WFD_MAX_OBJECTS);
        return rc;
    }

#if defined(CONFIG_BCM96838) ||defined(CONFIG_BCM96848)
    if (!wl_dev_p)
    {
        /*This bind is for bridged traffic use the dummy wfd instance */
        tmp_idx = WFD_BRIDGED_OBJECT_IDX;
        minQIdx = maxQIdx = WFD_BRIDGED_QUEUE_IDX;
    }
    else
#endif
    {
        /* Find available slot. */
        for (tmp_idx = 0; tmp_idx < wfd_max_objects; tmp_idx++)
        {
            if (!wfd_objects[tmp_idx].wfd_bulk_get) /* This callback must be set for registered object */
                break;
        }

        /* Get Min & Max Q Indices for this WFD Instance */
        minQIdx = wfd_get_minQIdx(tmp_idx);
        maxQIdx = wfd_get_maxQIdx(tmp_idx);
    }

    memset(&wfd_objects[tmp_idx], 0, sizeof(wfd_objects[tmp_idx]));

    wfd_objects[tmp_idx].isValid = 1;
    wfd_objects[tmp_idx].wl_dev_p         = wl_dev_p;
    wfd_objects[tmp_idx].eFwdHookType     = eFwdHookType;
    wfd_objects[tmp_idx].isTxChainingReqd = isTxChainingReqd;

    if (eFwdHookType == WFD_WL_FWD_HOOKTYPE_SKB)
        wfd_objects[tmp_idx].wfd_bulk_get = wfd_bulk_skb_get;
    else
        wfd_objects[tmp_idx].wfd_bulk_get = wfd_bulk_fkb_get;

    wfd_objects[tmp_idx].wfd_fwdHook      = wfd_fwdHook;
    wfd_objects[tmp_idx].wfd_completeHook = wfd_completeHook;
    wfd_objects[tmp_idx].wl_chained_packets = 0;
    wfd_objects[tmp_idx].wl_mcast_packets = 0;
    wfd_objects[tmp_idx].wfd_acc_info_p  = wfd_acc_info_get();
    wfd_objects[tmp_idx].wfd_idx  = tmp_idx;
    wfd_objects[tmp_idx].wfd_rx_work_avail  = 0;
    wfd_objects[tmp_idx].wfd_mcastHook = wfd_mcastHook;
    wfd_objects[tmp_idx].wl_radio_idx = wl_radio_idx;

    sprintf(threadname,"wfd%d-thrd", tmp_idx);

    /* Configure WFD RX queue */
    if (maxQIdx < WFD_NUM_QUEUE_SUPPORTED)
    {
        for (qidx = minQIdx; qidx <= maxQIdx; qidx++)
        {
           qid = wfd_get_qid(qidx);

           if ((rc = wfd_config_rx_queue(qid, WFD_WLAN_QUEUE_MAX_SIZE, eFwdHookType)) != 0)
           {
               printk("%s %s: Cannot configure WFD CPU Rx queue (%d), status (%d)\n",
                      __FILE__, __FUNCTION__, qid, rc);
               return rc;
           }

           wfd_int_enable (qid, qidx); 

           wfd_objects[tmp_idx].wfd_queue_mask |= (1 << qidx);
        }

        /* Create WFD Thread */
        init_waitqueue_head(&wfd_objects[tmp_idx].wfd_rx_thread_wqh);
        wfd_objects[tmp_idx].wfd_rx_thread = kthread_create(wfd_tasklet_handler, (void *)tmp_idx, threadname);
        /* wlmngr manages the logic to bind the WFD threads to specific CPUs depending on platform
           Look at function wlmngr_setupTPs() for more details */
        //kthread_bind(wfd_objects[tmp_idx].wfd_rx_thread, tmp_idx);
        wake_up_process(wfd_objects[tmp_idx].wfd_rx_thread);

        printk("\033[1m\033[34m %s: Dev %s wfd_idx %d wl_radio_idx %d Type %s configured WFD thread %s "
            "minQId/maxQId (%d/%d), status (%d) qmask 0x%x\033[0m\n",
            __FUNCTION__, wl_dev_p->name, tmp_idx, wl_radio_idx,
            ((eFwdHookType == WFD_WL_FWD_HOOKTYPE_SKB) ? "skb" : "fkb"), threadname,
            wfd_get_qid(minQIdx), wfd_get_qid(maxQIdx), rc, wfd_objects[tmp_idx].wfd_queue_mask);
    }
    else
    {
        printk("%s: ERROR qidx %d maxq %d\n", __FUNCTION__, 
            (int)maxQIdx, (int)WFD_NUM_QUEUE_SUPPORTED);
    }

#if defined(CONFIG_BCM96838) ||defined(CONFIG_BCM96848)
    if (tmp_idx != WFD_BRIDGED_OBJECT_IDX)
#endif
        wfd_objects_num++;
    return (tmp_idx);
}
EXPORT_SYMBOL(wfd_bind);

void wfd_unbind(int wfdIdx, enumWFD_WlFwdHookType hook_type)
{
    int qidx, qid;
    int minQIdx;
    int maxQIdx;

    // simple reclaim iff idx of last bind
    if (wfdIdx >= WFD_MAX_OBJECTS)
    {
        printk("%s wfd_idx %d out of bounds %d\n", __func__, wfdIdx, WFD_MAX_OBJECTS);
        return;
    }

#if defined(CONFIG_BCM96838) ||defined(CONFIG_BCM96848)
    if (wfdIdx == WFD_BRIDGED_OBJECT_IDX)
    {
        /*This unbind is for bridged traffic use the bridged qidx */
        minQIdx = maxQIdx = WFD_BRIDGED_QUEUE_IDX;
    }
    else
#endif
    {
        /* Get Min & Max Q Indices for this WFD Instance */
        minQIdx = wfd_get_minQIdx(wfdIdx);
        maxQIdx = wfd_get_maxQIdx(wfdIdx);
    }


    /* free the pci rx queue(s); disable the interrupt(s) */
    for (qidx = minQIdx; qidx <= maxQIdx; qidx++) 
    {
        /* Deconfigure PCI RX queue(s) */
        qid = wfd_get_qid(qidx);
        wfd_config_rx_queue(qid, 0, hook_type);
        wfd_objects[wfdIdx].wfd_queue_mask &= ~(1 << qidx);
        wfd_int_disable(qid, qidx);
    }

    memset(&wfd_objects[wfdIdx], 0, sizeof wfd_objects[wfdIdx]);
    wfd_objects_num--;
}
EXPORT_SYMBOL(wfd_unbind);

int wfd_registerdevice(uint32_t wfd_idx, int ifidx, struct net_device *dev)
{
   if (wfd_idx > WFD_MAX_OBJECTS )
   {
      printk("%s Error Incorrect wfd_idx %d passed\n", __FUNCTION__, wfd_idx);
      return -1;
   }
   
   if (ifidx > WIFI_MW_MAX_NUM_IF)
   {
       printk("%s Error ifidx %d out of bounds(%d)\n", 
              __FUNCTION__, ifidx, WIFI_MW_MAX_NUM_IF);
   }

   if (wfd_objects[wfd_idx].wl_if_dev[ifidx] != NULL)
   {
      printk("%s Device already registered for wfd_idx %d ifidx %d\n", 
              __FUNCTION__, wfd_idx, ifidx);
   }

   wfd_objects[wfd_idx].wl_if_dev[ifidx] = dev;

   printk("%s Successfully registered dev %s ifidx %d wfd_idx %d\n", 
          __FUNCTION__, dev->name, ifidx, wfd_idx);

   return 0;
}
EXPORT_SYMBOL(wfd_registerdevice);


int wfd_unregisterdevice(uint32_t wfd_idx, int ifidx)
{
   if (wfd_idx > WFD_MAX_OBJECTS )
   {
      printk("%s Error Incorrect wfd_idx %d passed\n", __FUNCTION__, wfd_idx);
      return -1;
   }
   
   if (ifidx > WIFI_MW_MAX_NUM_IF)
   {
       printk("%s Error ifidx %d out of bounds(%d)\n", 
              __FUNCTION__, ifidx, WIFI_MW_MAX_NUM_IF);
   }

   wfd_objects[wfd_idx].wl_if_dev[ifidx] = NULL;

   printk("%s Successfully unregistered ifidx %d wfd_idx %d\n", 
          __FUNCTION__, ifidx, wfd_idx);

   return 0;
}
EXPORT_SYMBOL(wfd_unregisterdevice);


static void wfd_dump(void)
{
    unsigned long flags;
    int idx;

    for (idx = 0; idx < WFD_MAX_OBJECTS; idx++)
    {
       WFD_IRQ_LOCK(idx, flags);
       printk("wfd_rx_work_avail 0x%x qmask 0x%x\n", 
              wfd_objects[idx].wfd_rx_work_avail, wfd_objects[idx].wfd_queue_mask);
       WFD_IRQ_UNLOCK(idx, flags);
    }
}

MODULE_DESCRIPTION("WLAN Forwarding Driver");
MODULE_AUTHOR("Broadcom");
MODULE_LICENSE("GPL");

module_init(wfd_dev_init);
module_exit(wfd_dev_close);
