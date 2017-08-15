/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom Corporation
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
*    Description:
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */
#include <mspi.h>
#include <mspi_Defs.h>

/* ---- Private Constants and Types -------------------------- */
static const BCM_MSPI_REGS mspiDefaultRegs =
{
   5,       /* SerialClk Divider setting - (default is 6.250 MHz) */
   0,       /* minimum time between subsequent transfers (in SCK clock cycles) */
   0,       /* sets SPI to transfer using little endian */
   0xFF,    /* data to be transferred during half duplex reads */
   0x0000,  /* default command register settings */
#if MSPI_USE_INTERRUPTS
   BCM_MSPI_CTRL_MODE_INTERRUPT  /* Interrupt mode */
#else
   BCM_MSPI_CTRL_MODE_POLLING /*Polling mode*/
#endif /* MSPI_USE_INTERRUPTS */
};

/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
static int setMspiRegs( BCM_MSPI_DRV *pMspiDrv, BCM_MSPI_REGS *pNewMspiRegs );
static int getMspiRegs( BCM_MSPI_DRV *pMspiDrv, BCM_MSPI_REGS *pNewMspiRegs );

/* ---- Functions -------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   mspiIOctl
**
** PURPOSE:    Perform IO control on the MSPI registers
**
** PARAMETERS: command  - IO command
**             cmdp     - IO structure pointer
**
** RETURNS:    MSPI_SUCCESS / MSPI_ERROR
**
*****************************************************************************
*/
int mspiIOctl( BCM_MSPI_CMD command, void *cmdp )
{
   int result;
   BCM_MSPI_DRV *pMspiDrv;

   /* Retrieve MSPI driver */
   pMspiDrv = &mspiDrv;

   switch ( command )
   {
      case BCM_MSPI_INITREGS_CMD:
      {
         result = setMspiRegs( pMspiDrv, (BCM_MSPI_REGS *)&mspiDefaultRegs );
      }
      break;

      case BCM_MSPI_SETREGS_CMD:
      {
         result = setMspiRegs( pMspiDrv, cmdp );
      }
      break;

      case BCM_MSPI_GETREGS_CMD:
      {
         result = getMspiRegs( pMspiDrv, cmdp );
      }
      break;

      case BCM_MSPI_RXCALLBACK_INSTALL_CMD:
      {
         pMspiDrv->rxCallbackFnc = cmdp;
         result = MSPI_SUCCESS;
      }
      break;

      case BCM_MSPI_TXCALLBACK_INSTALL_CMD:
      {
         pMspiDrv->txCallbackFnc = cmdp;
         result = MSPI_SUCCESS;
      }
      break;

      default:
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Unknown message received"));
         return ( MSPI_ERROR );
      }
      break;
   }

   return( result );
}


/*
*****************************************************************************
** FUNCTION:   setMspiRegs
**
** PURPOSE:    Set the MSPI registers
**
** PARAMETERS: pMspiDrv     - pointer to MSPI driver structure
**             pNewMspiRegs - pointer to the new MSPI register settings
**
** RETURNS:    MSPI_SUCCESS / MSPI_ERROR
**
*****************************************************************************
*/
static int setMspiRegs( BCM_MSPI_DRV *pMspiDrv, 
                        BCM_MSPI_REGS *pNewMspiRegs )
{
   volatile SpiControl *pMspiHwRegs = pMspiDrv->pSpiRegs;

   /* If MSPI is busy then abort */
   if (( pMspiHwRegs->spiStatus & MMR_MSPI_STATUS_SERBUSY ) || ( pMspiHwRegs->spiStatus & MMR_MSPI_STATUS_CMDBUSY ))
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: MSPI driver busy"));
      return ( MSPI_ERROR );
   }

   /* Set Clock Settings */
   if (( pNewMspiRegs->spiClkDiv < 1 ) || ( pNewMspiRegs->spiClkDiv > 6 ))
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Invalid register settings"));
      return ( MSPI_ERROR );
   }
   else
   {
      pMspiHwRegs->spiClkCfg &= ~SPI_CLK_MASK;
      pMspiHwRegs->spiClkCfg |= pNewMspiRegs->spiClkDiv;
   }

   if ( pNewMspiRegs->byteSwap == 0 )
   {
      pMspiHwRegs->spiClkCfg &= ~SPI_BYTE_SWAP;
   }
   else
   {
      pMspiHwRegs->spiClkCfg |= SPI_BYTE_SWAP;
   }

   if ( pNewMspiRegs->ssOffTime > 7 )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Invalid register settings"));
      return ( MSPI_ERROR );
   }
   else
   {
      pMspiHwRegs->spiClkCfg &= ~SPI_SSOFFTIME_MASK;
      pMspiHwRegs->spiClkCfg |= ( pNewMspiRegs->ssOffTime << 3 );
   }

   /* Set the fill byte */
   if ( pNewMspiRegs->fillByte > 0x000000FF )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Invalid register settings"));
      return ( MSPI_ERROR );
   }
   else
   {
      pMspiHwRegs->spiFillByte = (XDRV_UINT8)pNewMspiRegs->fillByte;
   }
   
   /* Set the command word register settings */
   if ( pNewMspiRegs->commandWord > 0x0000FFFF )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "ERROR: Invalid register settings"));
      return ( MSPI_ERROR );
   }
   else
   {
      pMspiHwRegs->spiCmd = (XDRV_UINT16)pNewMspiRegs->commandWord;
   }

   /*
   ** All interrupts are cleared and disabled for polling mode
   */
   if( pNewMspiRegs->mspiCtrlMode == BCM_MSPI_CTRL_MODE_POLLING )
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "INFO: MSPI POLLING MODE"));

      pMspiHwRegs->spiIntMask = 0x00;
      mspi63268intc_IrqDisable();
   }
#if MSPI_USE_INTERRUPTS
   else
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "INFO: MSPI INTERRUPT MODE"));
      
      /* Clear MSPI interrupt status to prevent initial false interrupt */
      mspi63268intc_IrqDisable();
      pMspiHwRegs->spiIntStatus |= MMR_MSPI_INTSTATUS_MASK;

      /* Enable MSPI interrupts in the MSPI and MSPI interrupt control */
      pMspiHwRegs->spiIntMask = 0x07;
      mspi63268intc_IrqEnable();
   }
#endif /* MSPI_USE_INTERRUPTS */

   return ( MSPI_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   getMspiRegs
**
** PURPOSE:    Get the MSPI registers
**
** PARAMETERS: pMspiDrv     - pointer to MSPI driver structure
**             pNewMspiRegs - pointer to the location where the retrieved
**                            MSPI register settings are saved
**
** RETURNS:    MSPI_SUCCESS / MSPI_ERROR
**
*****************************************************************************
*/
static int getMspiRegs( BCM_MSPI_DRV *pMspiDrv, 
                        BCM_MSPI_REGS *pNewMspiRegs )
{
   volatile SpiControl *pMspiHwRegs = pMspiDrv->pSpiRegs;

   /* If MSPI is busy then abort */
   if (( pMspiHwRegs->spiStatus & MMR_MSPI_STATUS_SERBUSY ) || ( pMspiHwRegs->spiStatus & MMR_MSPI_STATUS_CMDBUSY ))
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "ERROR: MSPI driver busy"));
      return ( MSPI_ERROR );
   }

   /* Get Clock Settings */
   pNewMspiRegs->spiClkDiv = ( pMspiHwRegs->spiClkCfg & SPI_CLK_MASK );
   pNewMspiRegs->byteSwap = (( pMspiHwRegs->spiClkCfg & SPI_BYTE_SWAP ) >> 7 ); 
   pNewMspiRegs->ssOffTime = (( pMspiHwRegs->spiClkCfg & SPI_SSOFFTIME_MASK ) >> 3 ); 

   /* Get the fill byte setting */
   pNewMspiRegs->fillByte = pMspiHwRegs->spiFillByte;

   /* Get the command word register settings */
   pNewMspiRegs->commandWord = ( pMspiHwRegs->spiCmd & ~SPI_CMD_COMMAND_MASK );

   /* Get the control mode */
   if( pMspiHwRegs->spiIntMask != 0x00 )
   {
      pNewMspiRegs->mspiCtrlMode = BCM_MSPI_CTRL_MODE_INTERRUPT;
   }
   else
   {
      pNewMspiRegs->mspiCtrlMode = BCM_MSPI_CTRL_MODE_POLLING;
   }

   return ( MSPI_SUCCESS );
}
