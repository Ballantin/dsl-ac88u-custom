#ifndef __FAP4KE_PERF_H_INCLUDED__
#define __FAP4KE_PERF_H_INCLUDED__

/*

 Copyright (c) 2007 Broadcom Corporation
 All Rights Reserved

<:label-BRCM:2011:DUAL/GPL:standard

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

/*
 *******************************************************************************
 *
 * File Name  : fap4ke_perf.h
 *
 * Description: This file contains the FAP Performance Tool definitions.
 *
 *******************************************************************************
 */

#include "fap4ke_tm.h"
#if (defined(CONFIG_BCM_BPM) || defined(CONFIG_BCM_BPM_MODULE)) && defined(CC_FAP4KE_TM)
#define CC_FAP4KE_PERF
#endif

#if defined(CC_FAP4KE_PERF)

#define p4kePerf (&p4keDspramGbl->perfCtrl)
#define p4kePerfResults (&p4kePsmGbl->perfResults)

typedef struct {
    uint32 ipSa;
    uint32 ipDa;
    union {
        struct {
            uint16 sPort;  /* UDP source port */
            uint16 dPort;  /* UDP dest port */
        };
        uint32 ports;
    };
} fap4kePerf_tuple_t;

typedef struct {
    uint8 *pBuf;
    uint16 len;
    uint16 dmaStatus;
    uint8 enable;
    struct {
        uint8 virtDestPort : 4;
        uint8 destQueue    : 4;
    };
    uint8 extSwTagLen;
    uint8 txChannel;
    uint8 isEnetTx;
    uint8 enetRxChannel;
    uint16 totalLen;
    uint32 copies;
    uint32 sequenceNbr;
    fap4keTm_shaper_t shaper;
    fap4kePerf_tuple_t rxTuple;
} fap4kePerf_ctrl_t;

typedef struct {
    uint32 packets;
    uint32 bytes;
    uint32 start_cycle_count;
    uint32 end_cycle_count;
} fap4kePerf_rxResults_t;

typedef struct {
    uint32 dropped;
} fap4kePerf_txResults_t;

typedef struct {
    uint8 running;
    uint8 overhead;
    fap4kePerf_rxResults_t rx;
    fap4kePerf_txResults_t tx;
} fap4kePerf_results_t;

int fap4kePerf_enetSetup(fapDqm_EthTx_t *tx);
#if defined(CONFIG_BCM_XTMCFG) || defined(CONFIG_BCM_XTMCFG_MODULE)
int fap4kePerf_xtmSetup(fapDqm_XtmTx_t *tx);
#endif
int fap4kePerf_receive(uint32 ipSa, uint32 ipDa, uint32 ports, int length, uint8 *payload_p);
void fap4kePerf_transmit(void);
void fap4kePerf_init(void);

/* User API */
void fap4kePerf_enable(void);
void fap4kePerf_disable(void);
void fap4kePerf_setAnalyzer(uint32 ipSa, uint32 ipDa, uint16 sPort, uint16 dPort);
int fap4kePerf_setGenerator(uint32 kbps, uint16 mbs, uint32 copies, uint16 totalLen);
void fap4kePerf_setEnetRxChannel(uint8 channel);

#else /* CC_FAP4KE_PERF */

#define fap4kePerf_setEnetRxChannel(_channel)

#endif /* CC_FAP4KE_PERF */

#endif  /* defined(__FAP4KE_PERF_H_INCLUDED__) */
