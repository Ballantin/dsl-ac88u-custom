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
*    Filename: daaSi3050.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      SiLabs Si3050 Data Access Arrangement circuit (DAA).
*
****************************************************************************/


/* ---- Include Files ---------------------------------------- */

#include <xdrvTypes.h>
#include <kernelMainWrapper.h>
#include "mspiDef.h"
#include <xdrvLog.h>
#include <string.h>
#include <bosSleep.h>
#include <bcmChip.h>

#include "daaSi3050.h"
#include "daaSi3050Defs.h"
#include "daaSi3050Locale.h"

#include "bcmSpiRes.h"
#include "board.h"

/*
	Uncomment the following line for debug information.
*/
/* #define DAA_DEBUG */
               

/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */

#define Si3050_RWLOG_ENABLED          0
#define Si3050_MAX_POLL_COUNTER       100
#define MAX_SETUP_REGISTERS            200
#define DAATEST_ENABLED               0


/* ---- Private Variables ------------------------------------ */
/* Create a static array of each country's WinSLAC parameters */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) si3050##country##Archive,
static const SI3050_REGISTER_VALUE *gDaaSi3050Values[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

static int mspiId;

/* ---- Private Function Prototypes -------------------------- */

/* DAA "meta-driver" interface functions */
static void daaSi3050ModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode );
static void daaSi3050LedControl( XDRV_DAA *pDrv, int value );
static XDRV_BOOL daaSi3050IsOffhook( XDRV_DAA *pDrv );
static XDRV_BOOL daaSi3050IsRingActive( XDRV_DAA *pDrv );
static XDRV_DAA_LOOP daaSi3050GetLoopCurrent( XDRV_DAA *pDrv );
static void daaProcessEvents( XDRV_DAA *pDrv );
static XDRV_UINT32 daaProbe( XDRV_DAA *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                             XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                             XDRV_UINT8 set );

/* DAA "meta-driver" interface */
static const XDRV_DAA_FUNCS daaSi3050DrvFuncs =
{
   daaSi3050ModeControl,
   daaSi3050LedControl,
   daaSi3050IsOffhook,
   daaSi3050IsRingActive,
   daaSi3050GetLoopCurrent,
   daaProcessEvents,
   daaProbe
};

/* Private Functions */
static void OpenDaa( DAA_Si3050_DRV *pDev );
static void CloseDaa( DAA_Si3050_DRV *pDev );
static void StartDaa( DAA_Si3050_DRV *pDev );

static void DaaWriteField(XDRV_UINT16 reg, XDRV_UINT8 field, XDRV_UINT8 val);
       void DaaWriteReg  (XDRV_UINT16 reg, XDRV_UINT8 val);
static XDRV_UINT8 DaaReadField(XDRV_UINT16 reg, XDRV_UINT8 field);
       XDRV_UINT8 DaaReadReg  (XDRV_UINT16 reg);

#ifdef DAA_DEBUG
static void DaaDumpRegisters( void );
#endif /* DAA_DEBUG */
static SI3050_REGISTER_VALUE *DaaGetLocale( VRG_COUNTRY country );
#if defined(CONFIG_BCM96816)  
static int spi_bus_num_special = 0;
#endif
/*
*****************************************************************************
** FUNCTION:   daaSi3050Init
**
** PURPOSE:    Initialize and open the Si3050 DAA
**
** PARAMETERS: chan      - the line number ( 0 referenced )
**             pDev      - pointer to the Si3050 DAA info structure
**             mspiHdl   - MSPI handle
**             rstGpioPin-  GPIO pin used to reset the DAA
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaSi3050Init
(
   VRG_COUNTRY             locale,
   int                     chan,
   DAA_Si3050_DRV*         pDev,
   XDRV_UINT16             rstGpioPin
)
{
   int daaId   = chan;
   pDev->daaId = daaId;

   /* Initialize the DAA information for each channel */
   pDev->daaInfo.pDrvFuncs = &daaSi3050DrvFuncs;
   pDev->daaLocale = locale;
   daaSi3050Reset(rstGpioPin);
 
   mspiId = pDev->mspiId; // Assuming single 3050 FXO device

   /* Open the DAA */
   OpenDaa( pDev );

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"DAA Device Init completed. ")); 
   
}

/*****************************************************************************
*
* FUNCTION:   daaSi3050Reset
*
* PURPOSE:    Reset the DAAs
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void daaSi3050Reset
(
   XDRV_UINT16 rstGpioPin
)
{
   kerSysSetGpio( rstGpioPin, kGpioInactive );
   bosSleep( 10 ); 
   kerSysSetGpio( rstGpioPin, kGpioActive );
   bosSleep( 250 );
   kerSysSetGpio( rstGpioPin, kGpioInactive );
   bosSleep( 250 );
}

/*
*****************************************************************************
** FUNCTION:   daaSi3050Deinit
**
** PURPOSE:    Shutdown the Si3050 DAA
**
** PARAMETERS: pDev  - pointer to the Si3050 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaSi3050Deinit( DAA_Si3050_DRV *pDev )
{
   /* Close the DAA */
   CloseDaa( pDev );
}

#ifdef DYING_GASP_API
/*****************************************************************************
*
* FUNCTION:   daaSi3050Shutdown
*
* PURPOSE:    Shutdown the DAAs
*
* PARAMETERS: reset pin 
*
* RETURNS:    none
*
*****************************************************************************/
void daaSi3050Shutdown ( int rstGpioPin )
{
   kerSysSetGpio( rstGpioPin, kGpioActive );
}
#endif

/*
*****************************************************************************
** FUNCTION:   daaSi3050IsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL daaSi3050IsOffhook( XDRV_DAA *pDrv )
{
   XDRV_BOOL            isOffhook;
   XDRV_UINT8           loopCurrent=0;
   
   (void) pDrv;

   loopCurrent = ( DaaReadReg(REG_LINEDEVSTAT) & (FIELD_LSC) );
   
   if ( loopCurrent < OFFHLCMIN )
   {
      isOffhook = XDRV_FALSE;
   }
   else
   {
      isOffhook = XDRV_TRUE;
      if ( loopCurrent == OFFHLCMAX )
      {
         /* loop current > 127 mA--overload may exist */
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA_ERR: loop current > 127 mA--overload may exist"));
      }
   }

   return ( isOffhook );
}


/*
*****************************************************************************
** FUNCTION:   daaSi3050IsRingActive
**
** PURPOSE:    Determine if a channel's ring signal is active
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if ring active, XDRV_FALSE otherwise
**
**
*****************************************************************************
*/
static XDRV_BOOL daaSi3050IsRingActive( XDRV_DAA *pDrv )
{
   XDRV_UINT8	rdt, rdtp, rdtn;
   XDRV_BOOL rc = XDRV_FALSE;

   (void) pDrv;
   
   /* 
      We are using the second method available on the Si3050/Si3019 to detect ring,
      this method requires validation of the RDTP, RDTN, RDT and RDTI bits.
   */
   rdt  = DaaReadField( REG_DAACONTROL1, FIELD_RDT );
   rdtp = DaaReadField( REG_DAACONTROL1, FIELD_RDTP );
   rdtn = DaaReadField( REG_DAACONTROL1, FIELD_RDTN );

   if( rdt && ( rdtp || rdtn ) )
   {
      rc = XDRV_TRUE;
//      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Ring Detected: RDTI %d, RDT %d, RDTP %d, RDTN %d",
//                rdti, rdt, rdtp, rdtn));
   }

   return rc;
}

/*
*****************************************************************************
** FUNCTION:   daaSi3050GetLoopCurrent
**
** PURPOSE:    Determine the state of a channel's loop current
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_DAA_LOOP_OPEN if there is no loop current detected
**             XDRV_DAA_LOOP_CLOSED if loop current is detected
**
** NOTE:       No polarity reversal detection is implemented
**
*****************************************************************************
*/
static XDRV_DAA_LOOP daaSi3050GetLoopCurrent( XDRV_DAA *pDrv )
{
   XDRV_UINT8           loopCurrent=0;
   (void) pDrv;

   loopCurrent = ( DaaReadReg(REG_LINEDEVSTAT) & (FIELD_LSC) );
   
   if ( loopCurrent < OFFHLCMIN )
   {
      return XDRV_DAA_LOOP_OPEN;
   }
   else
   {
      if ( loopCurrent == OFFHLCMAX )
      {
         /* loop current > 127 mA--overload may exist */
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA_ERR: loop current > 127 mA--overload may exist"));
      }
      return XDRV_DAA_LOOP_CLOSED;
   }
}



/*
*****************************************************************************
** FUNCTION:   daaSi3050ModeControl
**
** PURPOSE:    Sets the DAA into one of the defined modes
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**             mode  - the mode to set the DAA into.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void daaSi3050ModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode )
{
   (void) pDrv;

   switch( mode )
   {
      /* Put device into on-hook - release line */
      case XDRV_DAA_MODE_ONHOOK:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Going OnHook"));
         DaaWriteField(REG_DAACONTROL1, FIELD_OH, 0);
      }
      /* Pass through so that it will be ready to detect CLID RX for next incoming call */
   
      /*  Caller ID receive mode, enable onhook data reception  */
      case XDRV_DAA_MODE_ONHOOK_DATA:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Enable on-hook Caller ID receive. "));
         DaaWriteField(REG_DAACONTROL1, FIELD_ONHM, 1 );
      }
      break;

      /* Put device into off-hook - seize line */
      case XDRV_DAA_MODE_OFFHOOK:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"DAA: Going OffHook"));
         DaaWriteField(REG_DAACONTROL1, FIELD_OH, 1);
      }
      break;
  
      /* Enable ring validation mode */
      case XDRV_DAA_MODE_RING_VALID_ON:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Enable ring validation"));
         DaaWriteField( REG_RINGVAL3, FIELD_RNGV, 1 );
      }
      break;
      
      /* Disables ring validation mode */
      case XDRV_DAA_MODE_RING_VALID_OFF:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Disables ring validation"));
         DaaWriteField( REG_RINGVAL3, FIELD_RNGV, 0 );
      }
      break;

      default:
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"DAA: unrecognized DAA mode"));
      }
      break;
   }
}

/*
*****************************************************************************
** FUNCTION:   daaSi3050LedControl
**
** PURPOSE:    This function controls the on board Hook State LED
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
static void daaSi3050LedControl( XDRV_DAA *pDrv, int value )
{
    (void) pDrv;
    (void) value;
    /* Currently not applicable */
}

/*
*****************************************************************************
** FUNCTION:   OpenDaa
**
** PURPOSE:    Open the dual DAA
**
** PARAMETERS: pDev  - pointer to the Si3050 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void OpenDaa( DAA_Si3050_DRV *pDev )
{
   XDRV_UINT16 resetCount=0;
      
   (void) pDev;

   bosSleep( 10 );

   /* 
   		Reset the chips internal registers (bit gets 0 again automatically, wait for this) 
   */
   DaaWriteReg( REG_CONTROL1, 0x80 );

   /* 
		Wait for at least 1 msec (per specification) 
   */
   bosSleep(10);

   while( DaaReadReg(REG_CONTROL1) && resetCount<MAX_RESET_WAIT_PERIOD )
   {         
        resetCount++;        
        bosSleep(10);
   }
 
   if ( resetCount >= MAX_RESET_WAIT_PERIOD) 
   {
     XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "     DAA DBG: DAA read failed!!! "));
     
   }
   else
   {  
     XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"     DAA DBG: Successful READ!!!! count = %d ",resetCount ));
   }

   StartDaa( pDev );
}

/*
*****************************************************************************
** FUNCTION:   StartDaa
**
** PURPOSE:    Starts the dual DAA
**
** PARAMETERS: pDev  - pointer to the Si3050 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void StartDaa( DAA_Si3050_DRV *pDev )
{
   XDRV_UINT16 lockCount=0;
   XDRV_UINT8 lineDevType;
   XDRV_UINT8 sysDevRev;
   XDRV_UINT8 lineDevRev;
   XDRV_UINT8 lineDevSta1;
   XDRV_UINT8 lineDevSta2;
   SI3050_REGISTER_VALUE *pLocaleData = NULL;
   int i = 0;

   // Setup the PCM/SPI mode & timing 
   DaaWriteReg(REG_PCMTRANSSTARTLOW,  pDev->txTimeSlot);
   DaaWriteReg(REG_PCMRECVSTARTLOW,   pDev->rxTimeSlot);
   DaaWriteReg(REG_PCMSPIMODE      ,  pDev->pcmMode); 

   // Write Register 6 (00h) -- Normal operation for the line side device
   // 							activates the ISOCap.
   DaaWriteField(REG_DAACONTROL2, FIELD_PDL, 0);

   bosSleep(10);
   /* Wait before ISOCap link acquires lock */
   while (!DaaReadField(REG_LINEDEVSTAT, FIELD_FDT) && lockCount<MAX_LOCK_WAIT_PERIOD)
   {
       lockCount++;
       bosSleep(10);
   }
   
   if (lockCount == MAX_LOCK_WAIT_PERIOD)
   {
        XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, " DAA DBG: slacSI3050Init: ISOCAP lock failed "));
   }
   else
   {
       XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, " DAA DBG: ISOCAP lock count = %d ",lockCount));
   }
   
   /* 
      Other needed control setup for FXO interface.
   */
   DaaWriteField( REG_DCTERMCONTROL, FIELD_DCV, 3 );
   DaaWriteField( REG_CONTROL2, FIELD_RDI, 1 );
   DaaWriteReg( REG_RINGVAL2, REG_RINGVAL2_DEFAULT );
   DaaWriteField( REG_IRQMASK, FIELD_RDTM, 1 );

   /*
   *   Set needed locale information.
   */
   pLocaleData = DaaGetLocale( pDev->daaLocale );
   while( (pLocaleData != NULL) && (pLocaleData[i].reg != REG_INVALID) )
   {
      DaaWriteField( pLocaleData[i].reg,
                     pLocaleData[i].field,
                     pLocaleData[i].val );
      i++;
   }

    /* Read system/line side device revision */
   lineDevType = DaaReadField(REG_SYSDEVREV, FIELD_LSID);
   sysDevRev   = DaaReadField(REG_SYSDEVREV, FIELD_REVA);
   lineDevRev  = DaaReadField(REG_LINEDEVREV, FIELD_REVB);
   lineDevSta1 = DaaReadField(REG_LINEDEVSTAT, FIELD_FDT);
   lineDevSta2 = DaaReadField(REG_LINEDEVSTAT, FIELD_LSC);

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Si3050 SLAC Initialised, Line side device = Si: %s (0x%x)",
   	(lineDevType == LSID_SI3018) ? "3018" : ((lineDevType == LSID_SI3019) ? "3019" : "Unknown"),
   	lineDevType));
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "System dev rev: 0x%x, Line dev rev: 0x%x", sysDevRev, lineDevRev));
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Line dev status: FDT:0x%x, LCS: 0x%x", lineDevSta1, lineDevSta2));
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA initialized with country code %d", pDev->daaLocale));

   /* Reset all interrupts on start to prevent false positive. */
   DaaWriteReg( REG_IRQSOURCE, 0 );

#ifdef DAA_DEBUG
   DaaDumpRegisters();
#endif /* DAA_DEBUG */
}

/*
*****************************************************************************
** FUNCTION:   CloseDaa
**
** PURPOSE:    Close the dual DAA
**
** PARAMETERS: pDev  - pointer to the Si3050 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void CloseDaa( DAA_Si3050_DRV *pDev )
{
   (void) pDev;
}



/************************************************************************
** Function    : DaaWriteReg
** Description :
**    Write a register to the DAA
**    Note: reg also contains the reserved_mask (used to set reserved bits to 0)!!
************************************************************************/
void DaaWriteReg(XDRV_UINT16 reg, XDRV_UINT8 val)
{
    XDRV_UINT8   writeData[3];  
    XDRV_UINT8   regAddr  = GET_REG_ADDR(reg);
    XDRV_UINT8   resvMask = GET_RESV_MASK(reg);
    int wr_len, ix;
    XDRV_UINT8   dummy;
    
//    XDRV_UINT16  writeData16;
//    XDRV_UINT16  realData16;

#if defined(CONFIG_BCM96816)   
   /* Set bus number if on BCM6816 */
   if( (MISC->miscStrapBus & MISC_STRAP_BUS_LS_SPIM_ENABLED) == MISC_STRAP_BUS_LS_SPIM_ENABLED )
   {
      /* LS mode */
      spi_bus_num_special = LEG_SPI_BUS_NUM;
   }
   else
   {
      /* HS mode */
      spi_bus_num_special = HS_SPI_BUS_NUM;
   }
#endif

#if 1
    /* 
    	The SPI is configured in 8 bit/transfer mode.  The successive data bytes need
       	to be stored in the LSB bytes of the writeData array 
	*/
    writeData[0] = SPI_CONTROL_WRITE;		/* Control 				*/
    writeData[1] = regAddr;					/* Register address 	*/
    writeData[2] = val & resvMask;			/* Value to set			*/

	/* 
		Write one byte at a time to the MSPI 
	*/
	wr_len = 3;
	for ( ix = 0 ; ix < wr_len ; ix++ )
	{
        if( BcmSpiSyncTrans(&writeData[ix], &dummy, 0, 1, MSPI_BUS_ID, mspiId) )
		{
	   		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaWriteReg Reg 0x%x Val 0x%x - FAILED (%d)", 
	   			reg, val, ix));
		}
	}
#endif

#if 0  /* 16-bit mode */ 
   writeData16 = ((SPI_CONTROL_WRITE << 8) | (regAddr));
   if( BcmSpiSyncTrans(&writeData16, &dummy, 0, 2, MSPI_BUS_ID, mspiId) )
	{
		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaWriteReg Reg 0x%x Val 0x%x - FAILED wrt(0)", 
		   reg, writeData16));
		return 0xFF;
	}
	realData16 = ((val & resvMask) << 8);
   if( BcmSpiSyncTrans(&realData16, &dummy, 0, 2, MSPI_BUS_ID, mspiId) )
	{
		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaWriteReg Reg 0x%x Val 0x%x - FAILED wrt(2)", 
			reg, realData16));
		return 0xFF;
	}
#endif
 
}

/************************************************************************
** Function    : DaaReadReg
** Description :
**    Reads a register to the DAA
**    Note: reg also contains the reserved_mask !!
************************************************************************/
XDRV_UINT8 DaaReadReg(XDRV_UINT16 reg)
{
    XDRV_UINT8   writeData[3]; 
    XDRV_UINT8   readData;  
    XDRV_UINT8   regAddr  = GET_REG_ADDR(reg);
    XDRV_UINT8 dummy;
    
//    XDRV_UINT16  writeData16;
//    XDRV_UINT16  readData16;

#if defined(CONFIG_BCM96816)   
   /* Set bus number if on BCM6816 */
   if( (MISC->miscStrapBus & MISC_STRAP_BUS_LS_SPIM_ENABLED) == MISC_STRAP_BUS_LS_SPIM_ENABLED )
   {
      /* LS mode */
      spi_bus_num_special = LEG_SPI_BUS_NUM;
   }
   else
   {
      /* HS mode */
      spi_bus_num_special = HS_SPI_BUS_NUM;
   }
#endif

    writeData[0] = SPI_CONTROL_READ;	/* Control 				*/
    writeData[1] = regAddr;				/* Register address 	*/
    writeData[2] = 0xFF;				/* Dummy 				*/

#if 1
	/* 
		Write first byte (control) 
	*/
	if( BcmSpiSyncTrans(&writeData[0], &dummy, 0, 1, MSPI_BUS_ID, mspiId) )
	{
		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaReadReg Reg 0x%x Val 0x%x - FAILED wrt(0)", 
		   reg, writeData[0]));
		return 0xFF;
	}

	/* 
		Write second byte (register address) 
	*/
	if( BcmSpiSyncTrans(&writeData[1], &dummy, 0, 1, MSPI_BUS_ID, mspiId) )
	{
		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaReadReg Reg 0x%x Val 0x%x - FAILED wrt(1)", 
			reg, writeData[1]));
		return 0xFF;
	}

	/* 
		Write third byte (dummy) 
	*/
	if( BcmSpiSyncTrans(&writeData[2], &readData, 0, 1, MSPI_BUS_ID, mspiId) )
	{
		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaReadReg Reg 0x%x Val 0x%x - FAILED wrt(2)", 
			reg, writeData[2]));
		return 0xFF;
	}

   return (readData & 0xff);
#endif

#if 0 /* 16-bit mode */
	writeData16 = ((SPI_CONTROL_READ << 8 ) | regAddr); 
	
	if( BcmSpiSyncTrans(&writeData16, &dummy, 0, 2, MSPI_BUS_ID, mspiId) )
	{
		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaReadReg Reg 0x%x Val 0x%x - FAILED wrt(0)", 
		   reg, writeData[0]));
		return 0xFF;
	}
	
	if( BcmSpiSyncTrans(&writeData16, &readData16, 0, 2, MSPI_BUS_ID, mspiId) )
	{
		XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaReadReg Reg 0x%x Val 0x%x - FAILED wrt(2)", 
			reg, writeData[2]));
		return 0xFF;
	}
    return (readData16 & 0xff);
#endif

}

/************************************************************************
** Function    : DaaWriteField
** Description :
**    Write a field of a register to the DAA (RMW)
**    Note: reg also contains the reserved_mask !!
************************************************************************/
static void DaaWriteField(XDRV_UINT16 reg, XDRV_UINT8 field, XDRV_UINT8 val)
{
    XDRV_UINT8 oldVal;
    XDRV_UINT8 newVal;
    XDRV_UINT8 temp = field;

    /* Move val to correct bit position */
    while( !(temp & 0x01) ) 
    {
       temp = temp >> 1;
       val = val << 1;
    }
    /* Trunctate val */
    val &= field;
    
    oldVal  = DaaReadReg(reg);
    newVal  = oldVal & ~field;
    newVal |= val;
    
    DaaWriteReg(reg, newVal);
}

/************************************************************************
** Function    : DaaReadField
** Description :
**    Write a field of a register to the slac (RMW)
**    Note: reg also contains the reserved_mask !!
************************************************************************/
XDRV_UINT8 DaaReadField(XDRV_UINT16 reg, XDRV_UINT8 field)
{
    XDRV_UINT8 val;

    val =  DaaReadReg(reg);
    
    
    val &= field;

    /* shift to lsb */
    while (!(field & 0x01)) 
    {
       field = field >> 1;
       val = val >> 1;
    }
    
    return val;
}

#ifdef DAA_DEBUG
/************************************************************************
** Function    : DaaDumpRegisters
** Description : Dumps the registers of the DAA
************************************************************************/
void DaaDumpRegisters( void )
{
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Si3050 Registers Values" ));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Control 1  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_CONTROL1), 
		DaaReadReg(REG_CONTROL1), 
		GET_RESV_MASK(REG_CONTROL1)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Control 2  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_CONTROL2), 
		DaaReadReg(REG_CONTROL2), 
		GET_RESV_MASK(REG_CONTROL2)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Int Mask   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_IRQMASK), 
		DaaReadReg(REG_IRQMASK), 
		GET_RESV_MASK(REG_IRQMASK)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Int Source - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_IRQSOURCE), 
		DaaReadReg(REG_IRQSOURCE), 
		GET_RESV_MASK(REG_IRQSOURCE)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. DAA Ctrl 1 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_DAACONTROL1), 
		DaaReadReg(REG_DAACONTROL1), 
		GET_RESV_MASK(REG_DAACONTROL1)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. DAA Ctrl 2 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_DAACONTROL2), 
		DaaReadReg(REG_DAACONTROL2), 
		GET_RESV_MASK(REG_DAACONTROL2)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Smp Rate   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_SAMPRATE), 
		DaaReadReg(REG_SAMPRATE), 
		GET_RESV_MASK(REG_SAMPRATE)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x08),
		DaaReadReg(0x0008), 
		GET_RESV_MASK(0x08)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x09), 
		DaaReadReg(0x0009), 
		GET_RESV_MASK(0x09)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. DAA Ctrl 3 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_DAACONTROL3), 
		DaaReadReg(REG_DAACONTROL3), 
		GET_RESV_MASK(REG_DAACONTROL3)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. S/LSD Rev  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_SYSDEVREV), 
		DaaReadReg(REG_SYSDEVREV), 
		GET_RESV_MASK(REG_SYSDEVREV)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. LSD Status - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_LINEDEVSTAT), 
		DaaReadReg(REG_LINEDEVSTAT), 
		GET_RESV_MASK(REG_LINEDEVSTAT)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. LSD Rev    - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_LINEDEVREV), 
		DaaReadReg(REG_LINEDEVREV), 
		GET_RESV_MASK(REG_LINEDEVREV)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. DAA Ctrl 4 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_DAACONTROL4), 
		DaaReadReg(REG_DAACONTROL4), 
		GET_RESV_MASK(REG_DAACONTROL4)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. T/R Gain 1 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_TXRXGAIN1), 
		DaaReadReg(REG_TXRXGAIN1), 
		GET_RESV_MASK(REG_TXRXGAIN1)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Int Ctrl 1 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_INTERNAT1), 
		DaaReadReg(REG_INTERNAT1), 
		GET_RESV_MASK(REG_INTERNAT1)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Int Ctrl 2 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_INTERNAT2), 
		DaaReadReg(REG_INTERNAT2), 
		GET_RESV_MASK(REG_INTERNAT2)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Int Ctrl 3 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_INTERNAT3), 
		DaaReadReg(REG_INTERNAT3), 
		GET_RESV_MASK(REG_INTERNAT3)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Int Ctrl 4 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_INTERNAT4), 
		DaaReadReg(REG_INTERNAT4), 
		GET_RESV_MASK(REG_INTERNAT4)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. CP Rx Atte - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_CALLPROGRESSRXATT), 
		DaaReadReg(REG_CALLPROGRESSRXATT), 
		GET_RESV_MASK(REG_CALLPROGRESSRXATT)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. CP Tx Atte - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_CALLPROGRESSTXATT), 
		DaaReadReg(REG_CALLPROGRESSTXATT), 
		GET_RESV_MASK(REG_CALLPROGRESSTXATT)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Rg Ctrl 1  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_RINGVAL1), 
		DaaReadReg(REG_RINGVAL1), 
		GET_RESV_MASK(REG_RINGVAL1)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Rg Ctrl 2  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_RINGVAL2), 
		DaaReadReg(REG_RINGVAL2), 
		GET_RESV_MASK(REG_RINGVAL2)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Rg Ctrl 3  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_RINGVAL3), 
		DaaReadReg(REG_RINGVAL3), 
		GET_RESV_MASK(REG_RINGVAL3)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Res Calib  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_RESCALIBRATION), 
		DaaReadReg(REG_RESCALIBRATION), 
		GET_RESV_MASK(REG_RESCALIBRATION)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. DC Ctrl    - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_DCTERMCONTROL), 
		DaaReadReg(REG_DCTERMCONTROL), 
		GET_RESV_MASK(REG_DCTERMCONTROL)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x1B), 
		DaaReadReg(0x001B), 
		GET_RESV_MASK(0x1B)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Loop Sta   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_LOOPCURRENTSTATUS), 
		DaaReadReg(REG_LOOPCURRENTSTATUS), 
		GET_RESV_MASK(REG_LOOPCURRENTSTATUS)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Line V Sta - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_LINEVOLTAGESTATUS), 
		DaaReadReg(REG_LINEVOLTAGESTATUS), 
		GET_RESV_MASK(REG_LINEVOLTAGESTATUS)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. AC Ctrl    - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_ACTERMCONTROL), 
		DaaReadReg(REG_ACTERMCONTROL), 
		GET_RESV_MASK(REG_ACTERMCONTROL)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. DAA Ctrl 5 - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_DAACONTROL5), 
		DaaReadReg(REG_DAACONTROL5), 
		GET_RESV_MASK(REG_DAACONTROL5)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Gd Sta Ctl - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_GROUNDSTARTCONTROL), 
		DaaReadReg(REG_GROUNDSTARTCONTROL), 
		GET_RESV_MASK(REG_GROUNDSTARTCONTROL)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. PCM/SPI Md - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PCMSPIMODE), 
		DaaReadReg(REG_PCMSPIMODE), 
		GET_RESV_MASK(REG_PCMSPIMODE)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. PCM T St L - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PCMTRANSSTARTLOW), 
		DaaReadReg(REG_PCMTRANSSTARTLOW), 
		GET_RESV_MASK(REG_PCMTRANSSTARTLOW)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. PCM T St H - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PCMTRANSSTARTHIGH), 
		DaaReadReg(REG_PCMTRANSSTARTHIGH), 
		GET_RESV_MASK(REG_PCMTRANSSTARTHIGH)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. PCM R St L - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PCMRECVSTARTLOW), 
		DaaReadReg(REG_PCMRECVSTARTLOW), 
		GET_RESV_MASK(REG_PCMRECVSTARTLOW)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. PCM R St H - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PCMRECVSTARTHIGH), 
		DaaReadReg(REG_PCMRECVSTARTHIGH), 
		GET_RESV_MASK(REG_PCMRECVSTARTHIGH)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. T Gain 2   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_TXGAIN2), 
		DaaReadReg(REG_TXGAIN2), 
		GET_RESV_MASK(REG_TXGAIN2)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. R Gain 2   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_RXGAIN2), 
		DaaReadReg(REG_RXGAIN2), 
		GET_RESV_MASK(REG_RXGAIN2)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. T Gain 3   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_TXGAIN3), 
		DaaReadReg(REG_TXGAIN3), 
		GET_RESV_MASK(REG_TXGAIN3)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. R Gain 3   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_RXGAIN3), 
		DaaReadReg(REG_RXGAIN3), 
		GET_RESV_MASK(REG_RXGAIN3)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. GCI Ctrl   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_GCICONTROL), 
		DaaReadReg(REG_GCICONTROL), 
		GET_RESV_MASK(REG_GCICONTROL)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. LV T Int   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_LCVTH), 
		DaaReadReg(REG_LCVTH), 
		GET_RESV_MASK(REG_LCVTH)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. LV T IntC  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_LCVTHCONTROL), 
		DaaReadReg(REG_LCVTHCONTROL), 
		GET_RESV_MASK(REG_LCVTHCONTROL)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 1  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID1), 
		DaaReadReg(REG_PROGHYBRID1), 
		GET_RESV_MASK(REG_PROGHYBRID1)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 2  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID2), 
		DaaReadReg(REG_PROGHYBRID2), 
		GET_RESV_MASK(REG_PROGHYBRID2)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 3  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID3), 
		DaaReadReg(REG_PROGHYBRID3), 
		GET_RESV_MASK(REG_PROGHYBRID3)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 4  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID4), 
		DaaReadReg(REG_PROGHYBRID4), 
		GET_RESV_MASK(REG_PROGHYBRID4)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 5  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID5), 
		DaaReadReg(REG_PROGHYBRID5), 
		GET_RESV_MASK(REG_PROGHYBRID5)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 6  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID6), 
		DaaReadReg(REG_PROGHYBRID6), 
		GET_RESV_MASK(REG_PROGHYBRID6)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 7  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID7), 
		DaaReadReg(REG_PROGHYBRID7), 
		GET_RESV_MASK(REG_PROGHYBRID7)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Prg Hyb 8  - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_PROGHYBRID8), 
		DaaReadReg(REG_PROGHYBRID8), 
		GET_RESV_MASK(REG_PROGHYBRID8)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x35), 
		DaaReadReg(0x0035), 
		GET_RESV_MASK(0x35)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x36), 
		DaaReadReg(0x0036),
		GET_RESV_MASK(0x36)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x37), 
		DaaReadReg(0x0037), 
		GET_RESV_MASK(0x37)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x38), 
		DaaReadReg(0x0038), 
		GET_RESV_MASK(0x38)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x39), 
		DaaReadReg(0x0039), 
		GET_RESV_MASK(0x39)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. Reserved   - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(0x3A), 
		DaaReadReg(0x003A), 
		GET_RESV_MASK(0x3A)));	
	XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "\tReg 0x%x. SQC        - 0x%x (Res 0x%x)", 
		GET_REG_ADDR(REG_SPARKQUENCHING), 
		DaaReadReg(REG_SPARKQUENCHING), 
		GET_RESV_MASK(REG_SPARKQUENCHING)));	
}
#endif /* DAA_DEBUG */


/*
*****************************************************************************
** FUNCTION:   DaaGetLocale
**
** PURPOSE:    This function gets the locale coefficients values for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**
** RETURNS:    Pointer to the correct register data.
**
*****************************************************************************
*/
SI3050_REGISTER_VALUE *DaaGetLocale( VRG_COUNTRY country )
{
   SI3050_REGISTER_VALUE *pLocaleData = NULL;
   
   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DaaGetLocale: ERROR! Unsupported locale %d", 
                country));
   }
   else
   {
      pLocaleData = (SI3050_REGISTER_VALUE *)gDaaSi3050Values[country];
   }
   
   return pLocaleData;
}

/*
*****************************************************************************
** FUNCTION:   daaProcessEvents
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
void daaProcessEvents( XDRV_DAA *pDrv )
{
   (void) pDrv;
   return;
}

/*
*****************************************************************************
** FUNCTION:   daaProbe
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
XDRV_UINT32 daaProbe( XDRV_DAA *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                      XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                      XDRV_UINT8 set )
{
   (void) pDrv;
   return 0;
}
