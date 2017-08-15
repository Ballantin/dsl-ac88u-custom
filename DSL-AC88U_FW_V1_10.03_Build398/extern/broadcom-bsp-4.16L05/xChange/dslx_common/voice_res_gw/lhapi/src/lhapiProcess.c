/****************************************************************************
*  
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.  
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: lhapiProcess.c
*
****************************************************************************
*    Description:
*
*     Implements the "Low-level HAPI" API that is required to provide
*     common application supports (such as semas, error logging) to LDX.
*
*     This file contains implementations for the ingress and egress sample
*     processing functions. Note that these will only be invoked on a
*     non-distributed, host-only configuration.
*
*     These functions have been isolated in this file to prevent them from
*     being included by the linker in the final image for distributed
*     configurations. No other functions should be added to this file.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <lhapi.h>
#include <boardHalDsp.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


#ifndef BRCM_NONDIST_BUILD
/*
*****************************************************************************
** FUNCTION:   lhapiProcessIngress
**
** PURPOSE:    This function is invoked by hausware in response to application
**             calls to lhapiProcessEntry(), which is the function used to
**             run hausware at a periodic rate.
**
**             It is expected that this function will move digital samples
**             from the HAL to the hausware ingress buffers.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiProcessIngress( void )
{
   boardHalDspIngressProcess();
}


/*
*****************************************************************************
** FUNCTION:   lhapiProcessEgress
**
** PURPOSE:    This function is invoked by hausware in response to application
**             calls to lhapiProcessEntry(), which is the function used to
**             run hausware at a periodic rate.
**
**             It is expected that this function will take digital samples
**             from hausware egress buffers and move them to the HAL.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiProcessEgress( void )
{
   boardHalDspEgressProcess();
}
#endif
