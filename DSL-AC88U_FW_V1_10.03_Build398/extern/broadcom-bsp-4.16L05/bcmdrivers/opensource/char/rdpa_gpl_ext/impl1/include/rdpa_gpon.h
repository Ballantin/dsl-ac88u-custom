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

#ifndef _RDPA_GPON_H_
#define _RDPA_GPON_H_

#include "rdpa_types.h"
#include "bcm_intr.h"
#include "rdpa_gpon_cfg.h"

#define RDPA_IC_GPON_RX_IRQ   INTERRUPT_ID_WAN_GPON_RX   /**< GPON rx IRQ in IC */
#define RDPA_IC_GPON_TX_IRQ   INTERRUPT_ID_WAN_GPON_TX   /**< GPON tx IRQ in IC */

extern const bdmf_attr_enum_table_t rdpa_cpu_reason_enum_table;

/** \defgroup xgpon xGPON Management
 * Objects in this group control xGPON-related configuration
 */

/**
 * \defgroup gpon Link Management
 * \ingroup xgpon
 * Objects and functions in this group are used for configuration and
 * status monitoring on PON link and ONU level:
 * - PON link configuration
 * - PON link statistics
 * - ONU SN and password configuration
 * @{
 */

 #define RDPA_PLOAM_LENGTH 12


/** PON indications */
typedef enum
{
    rdpa_indication_lof,                            /**< LOF indication */
    rdpa_indication_err,                            /**< ERR indication */
    rdpa_indication_sf,                             /**< SF indication */
    rdpa_indication_sd,                             /**< SD indication */
    rdpa_indication_lcdg,                           /**< LCDG indication */
    rdpa_indication_dact,                           /**< DACT indication */
    rdpa_indication_dis,                            /**< DIS indication */
    rdpa_indication_pee,                            /**< PEE indication */
    rdpa_indication_mem,                            /**< MEM indication */ 
    rdpa_indication_pst_msg,                        /**< PST Message indication */
    rdpa_indication_ranging_start,                  /**< Ranging start indication */
    rdpa_indication_ranging_stop,                   /**< Ranging stop indication */
    rdpa_indication_assign_alloc_id_msg,            /**< Assign Alloc ID Message indication */
    rdpa_indication_cfg_port_id_msg,                /**< Configure Port ID Message indication */
    rdpa_indication_encrypted_port_id_msg,          /**< Encrypted Port ID Message indication */
    rdpa_indication_key_switched_out_of_sync,       /**< Key switched out of sync indication */
    rdpa_indication_key_switched,                   /**< Key switched indication */
    rdpa_indication_state_transition,               /**< State transition indication */
    rdpa_indication_assign_onu_id_msg,              /**< Assign ONU ID Message indication */
    rdpa_indication_link_state_transition,          /**< Link state transition indication */
    rdpa_indication_rouge_onu,                      /**< Rouge ONU indication */
    rdpa_indication_pmd_alarm,                      /**< PMD alarm indication */
    rdpa_indication_none = 0xffff
}rdpa_pon_indication;

/** PON link administrative state */
typedef enum {
    rdpa_pon_admin_state_not_ready,     /**< Not ready */
    rdpa_pon_admin_state_inactive,      /**< Inactive */
    rdpa_pon_admin_state_active         /**< Active */
} rdpa_pon_admin_state;

/** PON link sub state */
typedef enum {
    rdpa_pon_oper_state_none,               /**< Not set */
    rdpa_pon_oper_state_standby,            /**< Standby */
    rdpa_pon_oper_state_operational,        /**< Operational */
    rdpa_pon_oper_state_stop                /**< Stop */
}  rdpa_pon_sub_state;

/** PON link Operational state */
typedef enum {
    rdpa_pon_init_o1,          /**< o1 */
    rdpa_pon_standby_o2,       /**< standby_o2*/
    rdpa_pon_serial_num_o3,    /**< serial_num_o3*/
    rdpa_pon_ranging_o4,       /**< ranging_o4*/
    rdpa_pon_operational_o5,   /**< operational_o5*/
    rdpa_pon_popup_o6,         /**< popup_o6*/
    rdpa_pon_emergency_stop_o7,/**< emergency_stop_o7*/
}  rdpa_pon_link_operational_state;

/** PON Link state */
typedef struct
{
    rdpa_pon_admin_state admin_state;                   /**< Administrative state */
    rdpa_pon_sub_state sub_state;                       /**< Sub state */       
    rdpa_pon_link_operational_state link_oper_state;    /**< Link operational state */
}rdpa_pon_link_state;

/** Ranging stop reason */
typedef enum
{
    rdpa_ranging_stop_to1_timeout,              /** TO1  timeout */
    rdpa_ranging_stop_link_folt,                /** Link folt */
    rdpa_ranging_stop_deactivate_onu,           /** Deactivate ONU */
    rdpa_ranging_stop_disable_onu,              /** Disable ONU */
    rdpa_ranging_stop_successful_ranging,       /** Successful ranging */
    rdpa_ranging_stop_unmatch_sn,               /** Unmatched serial number */
    rdpa_ranging_stop_link_deactivate           /** Link deactivated */
}rdpa_ranging_stop_reason;

/** Pon status indication enable*/
typedef enum
{
    rdpa_indication_on,                         /** Indication enabled */
    rdpa_indication_off                         /** Indication disabled */
}rdpa_pon_status_indication;

/** ALLOC ID parameters */
typedef struct
{
    uint32_t alloc_id;
    rdpa_pon_status_indication status;
}rdpa_alloc_id_param;

/** Port ID paramerters */
typedef struct
{
    uint32_t port_id;
    rdpa_pon_status_indication status;
}rdpa_cfg_port_id_param;

/** Port Encryption */
typedef struct
{
    uint32_t port_id;
    bdmf_boolean encryption;
}rdpa_encrypted_port_id_param;

/** Key switching */
typedef struct
{
    uint32_t current_superframe ;
    uint32_t switch_over_superframe ;
}rdpa_key_switch_param;

/** PON sub state */
typedef struct
{
    rdpa_pon_sub_state old_state;
    rdpa_pon_sub_state new_state;
}rdpa_oper_state_param;

/** PON Link operational state */
typedef struct
{
    rdpa_pon_link_operational_state old_state;
    rdpa_pon_link_operational_state new_state;
}rdpa_link_state_param;

/** Rouge ONU mode */
typedef enum
{
    rdpa_monitor_rouge_mode,
    rdpa_fault_rouge_mode,
}rdpa_rouge_onu_mode;

/** Rouge ONU parameters */
typedef struct
{
    bdmf_boolean status;
    rdpa_rouge_onu_mode type;
}rdpa_rlouge_onu_param;

/** Rogue ONU Detection Mode */
typedef enum
{
    rdpa_rogue_tx_monitor_mode,
    rdpa_rogue_tx_fault_mode,
} rdpa_rogue_mode_t;

/** Rogue ONU Default Pin Configuration */
typedef struct
{
    uint32_t default_pin_mode_reg;  /**< Default pin mode cfg */
    uint32_t default_loop_sel_cfg;  /**< Default loop select reg configuration */
    uint32_t default_pin_mux_data;  /**< Default pin mux data */
} rdpa_rogue_default_pin_cfg_t;

/** Rogue ONU parameters */
typedef struct
{
    bdmf_boolean enable;    /**< Rogue State: disable / enable */
    rdpa_rogue_mode_t mode; /**< Rogue ONU Detection Mode: TX_MONITOR / TX_FAULT */
    uint32_t clock_cycle;   /**< Rogue ONU window_size in clock cycles */
    uint32_t gpio_pin;      /**< Rogue ONU the wanted GPIO for this feature,
                                this GPIO should be connected to the tx monitor/error transceiver pin */
    rdpa_rogue_default_pin_cfg_t default_pin_cfg; /**< Rogue ONU Default Pin Configuration */
} rdpa_rogue_onu_t;

/** MISC transmit parameters */
typedef struct
{
    bdmf_boolean enable;    /**< Generate tx burst depended data State: disable / enable */
    uint8_t prodc[16];      /**< Generate tx burst depended data Preamble overhead content */
    uint8_t prcl;           /**< Generate tx burst depended data configured length */
    uint8_t brc;            /**< Generate tx burst depended data Byte repetition content */
    uint8_t prl;            /**< Generate tx burst depended data Preamble repetition length */
    uint16_t msstart;       /**< Generate tx burst depended data frame start */
    uint16_t msstop;        /**< Generate tx burst depended data frame stop */
} rdpa_misc_tx_t;

/** PON tarnsnit */
typedef enum
{
    rdpa_pon_tx,
    rdpa_pon_rx,
    rdpa_pon_none
} rdpa_pon_transmit;

/** PMD Alarm parameters */
typedef struct
{
    bdmf_boolean esc_be                ;
    bdmf_boolean esc_rogue             ;
    bdmf_boolean esc_mod_over_current  ;
    bdmf_boolean esc_bias_over_current ;
    bdmf_boolean esc_mpd_fault         ;
    bdmf_boolean esc_eye_safety        ;
    bdmf_boolean fault_alarm;
    bdmf_boolean esc_alarm;
}rdpa_pmd_alarm_param;


/** GPON Callback Indication */
typedef union
{
    rdpa_pon_status_indication lof_status;                      /* LOF status */
    uint32_t bip8_errors;                                       /* ERR parameters */
    rdpa_pon_status_indication sf_status;                       /* SF status */
    rdpa_pon_status_indication sd_status;                       /* SD status */
    rdpa_pon_status_indication lcdg_status;                     /* LCDG status */
    rdpa_pon_status_indication dis_status;                      /* DIS status */
    uint8_t unkown_ploam_mem[RDPA_PLOAM_LENGTH];                /* MEM parameters */
    uint8_t pst_ploam[RDPA_PLOAM_LENGTH];                       /* PST parameters */
    rdpa_ranging_stop_reason ranging_stop;                      /* Ranging stop parameters */
    rdpa_alloc_id_param assign_alloc_id_parameters;             /* Assign alloc ID parameters */
    uint8_t onu_id;                                             /* Assign ONU ID parameters */
    rdpa_cfg_port_id_param configure_port_id_parameters;        /* Configure port ID parameters */
    rdpa_encrypted_port_id_param encrypted_port_id_parameters;  /* Encrypted port ID parameters */
    rdpa_key_switch_param key_switch_parameters;                /* Key switch parameters */
    rdpa_oper_state_param state_transition_parameters;          /* State transition parameters */
    rdpa_link_state_param link_state_transition_parameters;     /* Link State transition parameters */
    rdpa_rlouge_onu_param rogue_onu ;                           /* Rogue ONU parameters */
    rdpa_pmd_alarm_param pmd_alarm;                             /* PMD Alarm parameters */
} rdpa_callback_indication;

/** PON-level counters.
 * Underlying type for gpon_stat aggregate
 */
typedef struct 
{
    uint32_t bip_errors;        /**< BIP error counter */
    uint32_t crc_errors;        /**< PLOAM CRC errors */
    uint32_t rx_onu_id;         /**< Valid ONU id PLOAM counter */
    uint32_t rx_broadcast;      /**< Broadcast PLOAM counter */
    uint32_t rx_unknown;        /**< Rx unknown PLOAM counter */
    uint32_t tx_illegal_access; /**< Tx illegal access PLOAM counter */
    uint32_t tx_idle;           /**< Tx idle PLOAM counter */
    uint32_t tx_ploam;         /**< Tx PLOAM counter */
} rdpa_gpon_stat_t;

/** ONU serial number.
 * Underlying type for onu_sn aggregate
 */
typedef struct 
{
    uint8_t vendor_id[4];       /**< Vendor id */
    uint8_t vendor_specific[4]; /**< Vendor-specific number */
} rdpa_onu_sn_t;

/** ONU password */
typedef struct 
{
    uint8_t password[10];
} rdpa_onu_password_t;

/** PON FEC statistics */
typedef struct
{
    uint32_t corrected_bytes;          /**< FEC corrected bytes */
    uint32_t corrected_codewords;      /**< FEC corrected codewords */
    uint32_t uncorrectable_codewords;  /**< FEC uncorrectable codewords */
} rdpa_fec_stat_t;

/** Send PLOAM type */
typedef enum
{
    gpon_ploam_type_dgasp, /**< Dying Gasp */
    gpon_ploam_type_pee, /**< PEE */
    gpon_ploam_type_pst, /**< PST */
} rdpa_ploam_type_t;

/** PST parameters */
typedef struct
{
    uint32_t k1_value; /**< K1 */
    uint32_t k2_value; /**< K2 */
    uint32_t line_number; /**< Line number */
} rdpa_pst_params_t;

/** Send PLOAM parameters */
typedef struct
{
    rdpa_ploam_type_t ploam_type;  /**< PLOAM Type: Dying Gasp / PEE / PST */
    rdpa_pst_params_t pst_params; /**< PST parameters: K1, K2, Line number */
} rdpa_send_ploam_params_t;

/** DBA status report */
typedef enum
{
    gpon_dba_disable, /**< Disable  */
    gpon_dba_interval_1_msec, /**< 1 milisec */
    gpon_dba_interval_2_msec, /**< 2 milisec */
    gpon_dba_interval_3_msec, /**< 3 milisec */
    gpon_dba_interval_4_msec, /**< 4 milisec */
    gpon_dba_interval_5_msec, /**< 5 milisec */
    gpon_dba_interval_6_msec, /**< 6 milisec */
    gpon_dba_interval_7_msec, /**< 7 milisec */
    gpon_dba_interval_8_msec, /**< 8 milisec */
    gpon_dba_interval_9_msec, /**< 9 milisec */
    gpon_dba_interval_10_msec, /**< 10 milisec */
    gpon_dba_interval_125_usec = 125, /**< 125 microsec */
    gpon_dba_interval_250_usec = 250, /**< 250 microsec */
    gpon_dba_interval_500_usec = 500, /**< 500 microsec */
} rdpa_dba_interval_t;

/** Link activate / deactivate */
typedef enum
{
    rdpa_link_deactivate, /**< Deactivate */
    rdpa_link_activate_O1, /**< Activate: init at initial state */
    rdpa_link_activate_O7, /**< Activate: init at Emergency stop state */
} rdpa_link_activate_t;

void _ploam_isr_callback(void);
void _pmd_gpon_isr_wrapper(void);

/** GEM flow DS configuration.
 * Underlying type for rdpa_gpon_gem_ds_cfg_t aggregate
 */
typedef struct 
{
    uint16_t port; /**< GEM port */
    rdpa_discard_prty discard_prty; /**< Discard priority */
    bdmf_boolean encryption; /**< Encryption */
} rdpa_gpon_gem_ds_cfg_t;

/** TCONT statistics.
 * Underlying type for tcont_stat aggregate
 */
typedef struct 
{
    uint32_t idle_gem_counter; /**< Idle GEM Counter */
    uint32_t non_idle_gem_counter; /**< Non-Idle GEM Counter */
    uint32_t packet_counter; /**< Packet Counter - number of transmitted packets from TCONT*/
    uint32_t valid_access_counter; /**< Valid accesses for TCONT */
} rdpa_tcont_stat_t;

/** PON ID PLOAM paramters.
 * Underlying type for pon_id_param aggregate
 */
typedef struct 
{
    uint8_t pon_id_type; /**< PON-ID type: an indication of the ODN architecture, 
                              the source of the reported launch power and the ODN class 
                              as read from PON-ID ploam */
    uint8_t pon_identifier[7]; /**< PON Identifier as read from PON-ID ploam. */
    uint8_t tx_optical_level[2]; /**< Transmit Optical level [TOL], an indication of the current 
                                      transceiver launch power of the appropriate network element 
                                      as read from PON-ID ploam */
} rdpa_gpon_pon_id_param_t;

/** @} end of pon Doxygen group */

#endif /* _RDPA_GPON_H_ */
