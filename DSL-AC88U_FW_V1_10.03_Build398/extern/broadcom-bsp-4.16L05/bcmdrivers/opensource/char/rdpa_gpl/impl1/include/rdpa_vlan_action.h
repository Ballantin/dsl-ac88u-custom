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


#ifndef _RDPA_VLAN_ACTION_H_
#define _RDPA_VLAN_ACTION_H_

/** \defgroup vlan_action VLAN Actions
 * RDPA supports many different kinds of L2 header manipulations (VLAN actions).
 * Once VLAN action is configured, it can be applied for specific Ethernet flows,
 * ports, connections.
 * @{
 */

/* ToDo: check constants */
#define RDPA_MAX_VLAN_ACTION        128  /**< Max number of VLAN actions per direction */
#define RDPA_DROP_ACTION RDPA_MAX_VLAN_ACTION

/** Max number of tags supported by vlan_action */
#define RDPA_VLAN_MAX_TAGS          2

/** Outer tag index */
#define RDPA_VLAN_TAG_OUT   0 
/** Inner tag index */
#define RDPA_VLAN_TAG_IN    1 

#define RDPA_VLAN_ACTION_TPID_DONT_CARE (0xffff)

/* Per VLAN tag Command bits */
typedef enum
{
    RDPA_VLAN_CMD_BIT_PUSH,         /* Add VLAN tag */
    RDPA_VLAN_CMD_BIT_PUSH_ALWAYS,  /* Add VLAN tag always (even if packet is double-tagged) */
    RDPA_VLAN_CMD_BIT_POP,          /* Remove VLAN tag */
    RDPA_VLAN_CMD_BIT_REPLACE,      /* Replace VLAN tag */
    RDPA_VLAN_CMD_BIT_PUSH2,        /* Add 2 VLAN tags */
    RDPA_VLAN_CMD_BIT_PUSH2_ALWAYS, /* Add 2 VLAN tag always (even if packet is double-tagged) */
    RDPA_VLAN_CMD_BIT_POP2,         /* Remove 2 VLAN tags */
    RDPA_VLAN_CMD_BIT_REPLACE2,     /* Replace 2 VLAN tags */
    RDPA_VLAN_CMD_BIT_REMARK,       /* P-Bit re-marking in VLAN tag*/
    RDPA_VLAN_CMD_BIT_REMAP,        /* Re-mapping according to P-bit in VLAN tag */
    RDPA_VLAN_CMD_BIT_DSCP,         /* Remap DSCP -> P-bit in VLAN tag mapping */
    RDPA_VLAN_CMD_BIT_TPID,         /* Replace TPID in VLAN tag */
    RDPA_VLAN_CMD_BIT_TPID_REMARK   /* Remark TPID in VLAN tag */
} rdpa_vlan_cmd_bit;

/** VLAN command bits
 * VLAN command is a combination of RDPA_VLAN_CMD_XX bits
 */
typedef enum {
    /** Transparent action */
    RDPA_VLAN_CMD_TRANSPARENT = 0,
    /** Add VLAN tag */
    RDPA_VLAN_CMD_PUSH = (1 << RDPA_VLAN_CMD_BIT_PUSH),
    /** Add VLAN tag always (even if packet is double-tagged) */
    RDPA_VLAN_CMD_PUSH_ALWAYS = (1 << RDPA_VLAN_CMD_BIT_PUSH_ALWAYS),
    /** Remove VLAN tag */
    RDPA_VLAN_CMD_POP = (1 << RDPA_VLAN_CMD_BIT_POP),
    /** Replace VLAN tag */
    RDPA_VLAN_CMD_REPLACE = (1 << RDPA_VLAN_CMD_BIT_REPLACE),
    /** Add 2 VLAN tags */
    RDPA_VLAN_CMD_PUSH2 = (1 << RDPA_VLAN_CMD_BIT_PUSH2),
    /** Add 2 VLAN tag always (even if packet is double-tagged) */
    RDPA_VLAN_CMD_PUSH2_ALWAYS = (1 << RDPA_VLAN_CMD_BIT_PUSH2_ALWAYS),
    /** Remove 2 VLAN tags */
    RDPA_VLAN_CMD_POP2 = (1 << RDPA_VLAN_CMD_BIT_POP2),
    /** Replace 2 VLAN tags */
    RDPA_VLAN_CMD_REPLACE2 = (1 << RDPA_VLAN_CMD_BIT_REPLACE2),
    /** P-Bit re-marking in VLAN tag*/
    RDPA_VLAN_CMD_REMARK = (1 << RDPA_VLAN_CMD_BIT_REMARK),
    /** Re-mapping according to P-bit in VLAN tag */
    RDPA_VLAN_CMD_REMAP = (1 << RDPA_VLAN_CMD_BIT_REMAP),
    /** Remap DSCP -> P-bit in VLAN tag mapping */
    RDPA_VLAN_CMD_DSCP = (1 << RDPA_VLAN_CMD_BIT_DSCP),
    /** Replace TPID in VLAN tag */
    RDPA_VLAN_CMD_TPID = (1 << RDPA_VLAN_CMD_BIT_TPID),
    /** Remark TPID in VLAN tag */
    RDPA_VLAN_CMD_TPID_REMARK = (1 << RDPA_VLAN_CMD_BIT_TPID_REMARK),
} rdpa_vlan_command;

/** VLAN tag + pbit */
typedef struct {
    uint16_t vid;       /**< VID */
    rdpa_pbit pbit;     /**< PBIT */
    uint16_t tpid;      /**< TPID */
} rdpa_vtag_cmd_parm_t;

/** VLAN action parameters */
typedef struct {
    uint32_t cmd;       /**< Action command - combination of ::rdpa_vlan_command bits */
    rdpa_vtag_cmd_parm_t parm[RDPA_VLAN_MAX_TAGS];    /**< Command parameters */
} rdpa_vlan_action_cfg_t;

/** @} end of vlan_action Doxygen group */

#endif /* _RDPA_VLAN_ACTION_H_ */
