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
*    Filename: boardHalGpio6328.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent SLIC interface for
*     the 6328.
*     
*     The terms SLIC and SLAC are used interchangeably in the comments
*     or variable names in this file. For the Legerity SLIC/SLAC chipset,
*     the SLIC control is performed indirectly through the SLAC.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bcmChip.h>
#include <boardparms.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include "boardHal6328.h"
#include <boardHalInit.h>
#include <boardHalProv.h>
#include "boardHal6328vcm.h"

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

typedef struct GpioPinMuxSelMap
{
   int      gpioNum;
#define NTR2_CLK              16
#define NTR_CLK               29
#define NTR_PULSE             31

   unsigned long long pinMuxSelMask;
#define  PINMUX_NTR2_CLK_MASK      ((uint64)3 << (NTR2_CLK << 1))
#define  PINMUX_NTR_CLK_MASK       ((uint64)3 << (NTR_CLK << 1))
#define  PINMUX_NTR_PULSE_MASK     ((uint64)3 << (NTR_PULSE << 1))

   unsigned long long pinMuxSelMode;
#define PINMUX_NTR2_CLK     ((uint64)3 << (NTR2_CLK << 1))
#define PINMUX_NTR_CLK      ((uint64)3 << (NTR_CLK << 1))
#define PINMUX_NTR_PULSE    ((uint64)1 << (NTR_PULSE << 1))

} GPIO_PINMUX_SEL_MAP;


#if defined(NTR_SUPPORT_6328) || defined(NTR_SUPPORT_6328_COMP) || defined(NTR_SUPPORT_6328_INT)
static GPIO_PINMUX_SEL_MAP  Slic6328PinMuxMap[] =
{
   { NTR2_CLK,  PINMUX_NTR2_CLK_MASK,  PINMUX_NTR2_CLK },
   { NTR_CLK,   PINMUX_NTR_CLK_MASK,   PINMUX_NTR_CLK },
   { NTR_PULSE, PINMUX_NTR_PULSE_MASK, PINMUX_NTR_PULSE },
   { BP_NOT_DEFINED, 0, 0 },
};
#endif

/* ---- Private Function Prototypes -------------------------------------- */

#if defined(NTR_SUPPORT_6328) || defined(NTR_SUPPORT_6328_COMP) || defined(NTR_SUPPORT_6328_INT)
static int setGpioPinMuxSel( int );
#endif

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalGpioInit
**
** PURPOSE:    Initialize the Le88276 SLIC
**
** PARAMETERS: 
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalGpioInit( void )
{
#if 0
   int            i;
   int deviceId = 0;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms(); 

   while ( voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE )
   {
      if ( voiceParams->voiceDevice[deviceId].requiresReset && (voiceParams->voiceDevice[deviceId].resetGpio != BP_NOT_DEFINED) )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Device %d using Reset GPIO 0x%08X ", deviceId, voiceParams->voiceDevice[deviceId].resetGpio ));
         /* Set GPIO PINMUX_SEL register */
         setGpioPinMuxSel((int) (voiceParams->voiceDevice[deviceId].resetGpio & BP_GPIO_NUM_MASK) );
      }
      deviceId++;
   }

   /* Set GPIOs for faiolver relays */
   for ( i = 0; i < voiceParams->numFailoverRelayPins; i++ )
   {
      if ( voiceParams->pstnRelayCtrl.relayGpio[i] == BP_NOT_DEFINED )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "GPIO for relay %d not set !!!", i ));
      }
      else
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Relay %d GPIO is 0x%08X !!!", i, voiceParams->pstnRelayCtrl.relayGpio[i] ));
         /* Set GPIO PINMUX_SEL register */
         setGpioPinMuxSel((int) (voiceParams->pstnRelayCtrl.relayGpio[i] & BP_GPIO_NUM_MASK) );
      }
   }
#endif

   /* NTR */
#if defined(NTR_SUPPORT_6328)
   setGpioPinMuxSel((int) NTR2_CLK );  /* NTR2_CLK */
#endif   
#if defined(NTR_SUPPORT_6328_COMP)
   setGpioPinMuxSel((int) NTR_PULSE );  /* NTR_PULSE */
#endif
#if defined(NTR_SUPPORT_6328_INT)
   setGpioPinMuxSel((int) NTR_CLK );  /* NTR_CLK */
   GPIO->GPIODir |= (1 << NTR_CLK); /* Set as OUTPUT */
#endif

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalGpioDeinit
**
** PURPOSE:    Shutdown the Le88276 SLIC
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalGpioDeinit( void )
{
   return ( 0 );
}

#if defined(NTR_SUPPORT_6328) || defined(NTR_SUPPORT_6328_COMP) || defined(NTR_SUPPORT_6328_INT)
static int setGpioPinMuxSel( int gpioNum )
{
   int  i = 0;

   for(i = 0; ; i++)
   {
      if( Slic6328PinMuxMap[i].gpioNum == BP_NOT_DEFINED )
      {
         return 0;
      }
      else if(Slic6328PinMuxMap[i].gpioNum == gpioNum )
      {
         unsigned long long   mask;
         unsigned long long   mode;

         mode = Slic6328PinMuxMap[i].pinMuxSelMode;
         mask = Slic6328PinMuxMap[i].pinMuxSelMask;
         
         GPIO->PinMuxSel &= ~mask;
         GPIO->PinMuxSel |= mode;
         VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "Set GPIO pin %u mux sel register value 0x%10x", gpioNum, (unsigned int)GPIO->PinMuxSel ));
      }
   }

   return 0;
}
#endif
