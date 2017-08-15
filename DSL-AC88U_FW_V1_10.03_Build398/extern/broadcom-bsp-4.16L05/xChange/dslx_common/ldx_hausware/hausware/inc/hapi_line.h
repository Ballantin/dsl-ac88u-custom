/****************************************************************************
*
*     Copyright (c) 2003 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  hapi_line.h
*
*  PURPOSE:
*     This file contains the Line Service API definitions.
*
***************************************************************************/
#ifndef HAPI_LINE_H
#define HAPI_LINE_H

/* ---- Include Files ---------------------------------------------------- */
#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* HausWare environment definitions */
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* HausWare API Global definitions  */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* HausWare generic cmds/events     */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* HausWare API IDs                 */
#endif
/* ---- Constants and Types ---------------------------------------------- */

/*************************
* constants for equalizer 
*/
/*
** Maximum filter order - MUST BE EVEN
*/
#define HAPI_LINE_EQU_MAX_FILTER_ORDER         128

/*
** Identifiers for supported filter blocks
*/
#define HAPI_LINE_EQU_TX_FIR             0
#define HAPI_LINE_EQU_RX_FIR             1

// HACK - only support one direction for now
#if 0
#define HAPI_LINE_EQU_NUM_FILTER_BLOCKS  2
#endif
#define HAPI_LINE_EQU_NUM_FILTER_BLOCKS  1

#define HAPI_LINE_EQU_ENABLE_MASK              0x00FF
#define HAPI_LINE_EQU_CONFIG_MASK              0x0300
#define HAPI_LINE_EQU_CONFIG_WIDEBAND_MASK     0x0100
#define HAPI_LINE_EQU_CONFIG_NARROWBAND_MASK   0x0200

/****************************************************************************
* STRUCTURES AND ASSOCIATED DEFINITIONS                                     *
****************************************************************************/

/*
** Maximum number of filter coefficients that can be set using the
** HAPI_EQU_COEFF structure.
*/

#define HAPI_LINE_EQU_MAX_COEFF_NUM         40

/*
** Structure for passing filter coefficients to filter blocks.
**
** If we store coefficients as h[ 0, 1, .., HAPI_EQU_MAX_FILTER_ORDER ], then
** we write the new coefficients to h[] as follows:
**
**    h[ i + offset ] = coeff[ i ], for i = 0 to i = HAPI_EQU_MAX_COEFF_NUM-1
**
** The offset must be restricted as follows:
**
**  0 <= offset <= HAPI_EQU_MAX_FILTER_ORDER - HAPI_EQU_MAX_COEFF_NUM
**
*/

typedef struct HAPI_LINE_EQU_COEFF
{
   HAPI_UINT16   id;            /* filter block identifier                      */
   HAPI_UINT16   offset;        /* coefficient offset (defined above)           */
   HAPI_SINT16   coeff[ HAPI_LINE_EQU_MAX_COEFF_NUM ];
                           /* coefficients (Q15 format)                    */
} HAPI_LINE_EQU_COEFF;



/****************************************************************************
* COMMANDS                                                              *
****************************************************************************/
#define HAPI_LINE_HIGHPASS_ENABLE_CMD (HSF_MKCMD_STD(HAPI_LINE_SRV, 0))
   /* op1 : true to enable highpass filter, false to disable */
   /* op2 : reserved */

#define HAPI_LINE_EQU_CONFIG_FILTER_CMD (HSF_MKCMD_STD(HAPI_LINE_SRV, 1))
   /* OP1: filter block identifier */
   /* OP2: lower byte : TRUE to enable, FALSE to disable           */
   /*      upper byte : indication of wideband / narrowband filter */

#define HAPI_LINE_EQU_SET_FILTER_ORDER_CMD (HSF_MKCMD_STD(HAPI_LINE_SRV, 2))
   /* OP1: filter block identifier */
   /* OP2: filter order, n, where 0 <= n <= HAPI_EQU_MAX_FILTER_ORDER */
   /*      and n is even.                                             */

#define HAPI_LINE_EQU_SET_FILTER_COEFF_CMD (HSF_MKCMD_EXT(HAPI_LINE_SRV, 3))
   /* OP1: size of HAPI_EQU_COEFF structure in bytes. */
   /* OP2: pointer to a HAPI_EQU_COEFF structure      */
#define HAPI_LINE_BANDSPLIT_ENABLE_CMD (HSF_MKCMD_STD(HAPI_LINE_SRV, 4))
   /* op1 : true to enable highpass filter, false to disable */
   /* op2 : reserved */
#define HAPI_LINE_BANDCOMBINE_ENABLE_CMD (HSF_MKCMD_STD(HAPI_LINE_SRV, 5))
   /* op1 : true to enable highpass filter, false to disable */
   /* op2 : reserved */

#endif
