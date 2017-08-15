/****************************************************************************
*
* Copyright © 2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
 ***************************************************************************
 * File Name  : boardHalNtr.h
 *
 *    Description:
 *
 *      This file contains a hardware-specific (6362 platform) interface for
 *      Network Timing Synchronization via software comparison of two hardware
 *      registers: one updated by the local clock, another by an NTR signal
 *      obtained from the physical layer, which get it from the network
 *
 *
 ***************************************************************************/

#if defined( NTR_SUPPORT )

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>         /* primitive types */

#include <boardHalNtr.h>
#include <ntr6362.h>          /* Implementation */

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalNtrGetCounts
**
** PURPOSE:    Get the lateset register values from the MIPS counter and NTR counter
**
** PARAMETERS: localCount OUT - The MIPS counter value
**             ntrCount OUT   - The NTR counter value
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalNtrGetCounts( VRG_UINT32 * localCount, VRG_UINT32 * ntrCount )
{
   ntr6362_getCounts( localCount, ntrCount );
   return;
}

/*
*****************************************************************************
** FUNCTION:   boardHalNtrSetFeedback
**
** PURPOSE:    Set a feedback value into the PCM feedback register, thus adjusting the 
**             PCM highway frequency
**
** PARAMETERS: pcmStepsAdjust IN - the signed number of fine-grain adjustments to make
**                                 to default (reset) value
**             isEnbl IN - The state of NTR task, whether it is enabled or disabled
**
** RETURNS:    none
**
** NOTE:       The PCM feedback register is either switched on / enable feedback (when
**             isEnbl != 0 ) or is switched off / disabled feedback (when isEnbl == 0)
**             In this way, this function can switch off the PCM feedback register.
**             In either case, the feedback register will be written to with the new
**             feedback value, regardless of whether or not it is actually used.
**
*****************************************************************************
*/
void boardHalNtrSetFeedback( VRG_SINT32 pcmStepsAdjust, VRG_BOOL isEnbl )
{
   ntr6362_setFeedback( pcmStepsAdjust, isEnbl );
   return;
}

/*
*****************************************************************************
** FUNCTION:   boardHalNtrPrint
**
** PURPOSE:    Output the NTR and MIPS register values several times over a period of time
**
** PARAMETERS: numSamples IN        - The number of samples desired
**             sampleIntervalMs IN  - The interval between samples
**
** RETURNS:    none
**
** NOTE:       The sleep interval is not precise and cannot be used as a benchmark
**
*****************************************************************************
*/
void boardHalNtrPrint( VRG_BOOL numSamples, VRG_SINT32 sampleIntervalMs )
{
   ntr6362_print( numSamples, sampleIntervalMs );   
   return;
}

/*
*****************************************************************************
** FUNCTION:   boardHalNtrGetNdiv
**
** PURPOSE:    Get the lateset register values from the NDIV_INT and NDIV_FRAC
**             registers in PLL.
**
** PARAMETERS: ndivInt  OUT - The NDIV_INT register value
**             ndivFrac OUT - The NDIV_FRAC register value
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalNtrGetNdiv( VRG_UINT32 * ndivInt, VRG_UINT32 * ndivFrac )
{
   ntr6362_getNdiv( ndivInt, ndivFrac ); 
   return; 
}

#endif /* NTR_SUPPORT */
