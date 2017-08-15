/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: interface_driver_spi.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the spi interface for
*      proslic api
*
****************************************************************************/

#include "interface_driver_si321x_spi.h"
#include "bcmChip.h"
#include <kernelMainWrapper.h>
#include "mspiDef.h"
#include "si_voice_datatypes.h"
#include <xdrvLog.h>
#include "bcmSpiRes.h"
#include "interface_driver_si_common.h"

/* Spi commands */
#define SI_SPI_READ_CMD          0x80
#define SI_SPI_WRITE_CMD         0x00

/* Direct registers -- should be same for all siLab devices */
#define REG_DIRECT_SPIMODE             0
#define REG_DIRECT_PNI2                1

/* Indirect register access registers -- should be same for all siLab devices */
#define REG_INDIRECT_DATA_ACCESS_LOW      28 
#define REG_INDIRECT_DATA_ACCESS_HIGH     29
#define REG_INDIRECT_ADDRESS              30
#define REG_INDIRECT_ADDRESS_STATUS       31

/* Register constants */
#define SPI_DAISYCHAIN_MODE      0x80
#define SI3215_PNI2              0x80

/* Internal Functions */
static int Si3215orSi3216(XDRV_UINT16 mspiDevId ,XDRV_UINT8 channel);
static XDRV_UINT8 possibleAddressCorrect(XDRV_UINT16 mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 address);
static XDRV_UINT8 MSPI_Read( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr );
static void MSPI_Write( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr, XDRV_UINT8 data );
static XDRV_UINT16 MSPI_Indirect_Read( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr );
static void MSPI_Indirect_Write( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr, XDRV_UINT16 data );
#if defined(CONFIG_BCM96816)  
static int spi_bus_num_special = 0;
#endif
/* Local variables */
int daisyChain = 0;

/*
******************************************************************************
** FUNCTION:  si321xSPI_Init
**
** PURPOSE:   Initializes the SPI interface
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
int si321xSPI_Init (ctrl_S * hSpi)
{
	return 0;
}


/*
******************************************************************************
** FUNCTION:  ctrl_si321xResetWrapper
**
** PURPOSE:   Sets reset pins of slic
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
int ctrl_si321xResetWrapper (void * hSpi, int status)
{
	/*if (status)
		RSTNLow();
	else
		RSTNHigh();*/
	return 0;
}

/*
******************************************************************************
** FUNCTION:  ctrl_si321xReadRegisterWrapper
**
** PURPOSE:   Reads a single ProSLIC Register location
**
** PARAMETERS:
**            channel   - channel for multi channel slics
**            ramAddr   - address of the control register
**
** RETURNS:
**            Register data
**
*****************************************************************************
*/
XDRV_UINT8 ctrl_si321xReadRegisterWrapper (void * hSpi, XDRV_UINT8 channel,XDRV_UINT8 regAddr)
{
   XDRV_UINT8 readValue;
   
   bosMutexAcquire( &spiSiMutex );
   readValue =  MSPI_Read(((ctrl_S*)hSpi)->portID,channel,regAddr);			
   bosMutexRelease( &spiSiMutex );

   return readValue;			
}


/*
******************************************************************************
** FUNCTION:  ctrl_si321xWriteRegisterWrapper
**
** PURPOSE:   writes a single ProSLIC Register location
**
** PARAMETERS:
**            channel   - channel for multi channel slics
**            ramAddr   - address of the control register
**            data      - Data to write 
**
** RETURNS:
**            None
**
*****************************************************************************
*/
int ctrl_si321xWriteRegisterWrapper (void * hSpi, XDRV_UINT8 channel,  XDRV_UINT8 regAddr, XDRV_UINT8 data)
{
   bosMutexAcquire( &spiSiMutex );   
   MSPI_Write(((ctrl_S*)hSpi)->portID, channel, regAddr, data);
   bosMutexRelease( &spiSiMutex );
	return 0;
}


/*
******************************************************************************
** FUNCTION:  ctrl_si321xReadRAMWrapper
**
** PURPOSE:   Reads a single ProSLIC RAM location
**
** PARAMETERS:
**            channel   - channel for multi channel slics
**            ramAddr   - address of the control register
**
** RETURNS:
**            ram data
**
*****************************************************************************
*/
XDRV_UINT32 ctrl_si321xReadRAMWrapper (void * hSpi, XDRV_UINT8 channel, XDRV_UINT16 ramAddr)
{
   XDRV_UINT32 readValue;
   /* Check if ramAddr need to be modified */
   XDRV_UINT8 addr;
   
   bosMutexAcquire( &spiSiMutex );
   addr = possibleAddressCorrect(((ctrl_S*)hSpi)->portID,(XDRV_UINT8)channel,(XDRV_UINT8)ramAddr);
   readValue = MSPI_Indirect_Read( ((ctrl_S*)hSpi)->portID, channel, addr );
   bosMutexRelease( &spiSiMutex );
   
   return readValue;
}

/*
******************************************************************************
** FUNCTION:  ctrl_si321xWriteRAMWrapper
**
** PURPOSE:   Writes a single ProSLIC RAM location
**
** PARAMETERS:
**            channel   - channel for multi channel slics
**            ramAddr   - address of the control register
**            data      : data to write to RAM location
**
** RETURNS:
**            None
**
*****************************************************************************
*/
int ctrl_si321xWriteRAMWrapper (void * hSpi, XDRV_UINT8 channel, XDRV_UINT16 ramAddr, XDRV_UINT32 data)
{
   /* Check if ramAddr need to be modified */
   XDRV_UINT8 addr;

   bosMutexAcquire( &spiSiMutex ); 
   addr = possibleAddressCorrect(((ctrl_S*)hSpi)->portID,(XDRV_UINT8)channel,(XDRV_UINT8)ramAddr);
   MSPI_Indirect_Write( ((ctrl_S*)hSpi)->portID, channel, addr, data );
   bosMutexRelease( &spiSiMutex );
   
	return 0;
}


/*
******************************************************************************
** FUNCTION:  Si3215orSi3216
**
** PURPOSE:   check whether chip is Si3215/6
**
** PARAMETERS:
**            mspiDevId  - mspi device id ( 0 referenced )
**            channel  - channel for multi channel slics
**            addr  - address of the control register
**
** RETURNS:
**            True if Si3215/6 found
**
*****************************************************************************
*/
static int Si3215orSi3216(XDRV_UINT16 mspiDevId ,XDRV_UINT8 channel)
{
   static int slicPreviouslyIdentified = 0;
   static int isSi3215 = 0;

   if(!slicPreviouslyIdentified)
   {
      bosMutexAcquire( &spiSiMutex );
	   isSi3215 = ((MSPI_Read( mspiDevId, channel, REG_DIRECT_PNI2 ) & SI3215_PNI2) == SI3215_PNI2);
	   slicPreviouslyIdentified = 1;
      bosMutexRelease( &spiSiMutex );
   }

	return (isSi3215);
}

/*
******************************************************************************
** FUNCTION:  possibleAddressCorrect
**
** PURPOSE:   Correct indirect addreses for Si3215/6 chips
**
** PARAMETERS:
**            mspiDevId  - mspi device id ( 0 referenced )
**            channel  - channel for multi channel slics
**            addr  - address of the control register
**
** RETURNS:
**            data  - 8-bit data for the control register
**
*****************************************************************************
*/
static XDRV_UINT8 possibleAddressCorrect(XDRV_UINT16 mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 address)
{
   if (!Si3215orSi3216(mspiDevId,channel)) 
		return (address);
		
	if ((address > 12) && (address < 41))
		return (address - 13);
	if ((address == 41) || (address == 43))
		return (address + 23);
	if ((address > 87)&&(address < 99))
		return (address - 13); 
	if ((address > 98) && (address < 105))
		return (address - 30);
	if (address >= 208)
		return address;

	return 0xFF;
}


/*
******************************************************************************
** FUNCTION:  MSPI_Read
**
** PURPOSE:   Read from a memory address via MSPI
**
** PARAMETERS:
**            mspiDevId  - the line number ( 0 referenced )
**            channel  - channel for multi channel slics
**            addr  - address of the control register
**
** RETURNS:
**            data  - 8-bit data for the control register
**
*****************************************************************************
*/
static XDRV_UINT8 MSPI_Read( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr )
{
   XDRV_UINT8    SPICmdAddressByte = SI_SPI_READ_CMD | addr;
   XDRV_UINT8    txBuffer[2];
   XDRV_UINT8    rxBuffer[2] = { 0, 0 };
   XDRV_UINT8    daisyChainDevSelect;
   int           i;
   XDRV_UINT8    dummy;
      
   /* Ignore the channel select byte for now since we only support single channel slics */
   (void) channel;

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

	/* Send device select only if in daisychain mode */
	if ( daisyChain )
	{
   	/* proslic api is operatin in daisychain mode. This mode requires a device  select *
      * Byte to be transmitted first. Our hardware is not wired this way therefore       *
      * our device  select byte is FF which would result in any device with its          *
      * chip select line asserted be able to communicate with the spi master             */
      daisyChainDevSelect = 0xFF;
      
      /* Send the device select byte */
      if( BcmSpiSyncTrans(&daisyChainDevSelect, &dummy, 0, 1, MSPI_BUS_ID, mspiDevId) )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "PROSlic api interface MSPI_Write Error\n"));
      }
   }
   else
   {
      /* To avoid compile warnings */
      (void) daisyChainDevSelect;
   }

   /* Place the command/address and data into the lower byte of each txBuffer */
   txBuffer[0] = SPICmdAddressByte;
   txBuffer[1] = 0xFA;

   /* Write to the MSPI */
   /* controller does not support one byte mode */
   for ( i = 0; i < 2; i++ )
   {
       if( BcmSpiSyncTrans(&txBuffer[i], &rxBuffer[i], 0, 1, MSPI_BUS_ID, mspiDevId) )
       {
          XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "PROSlic api interface MSPI_Read Error\n"));
       }
   }

   return( rxBuffer[1] & 0xFF );
}


/*
*****************************************************************************
** FUNCTION:  MSPI_Write
**
** PURPOSE:   Write to a memory address via MSPI
**
** PARAMETERS:
**            mspiDevId  - the line number ( 0 referenced )
**            channel  - channel for multi channel slics
**            addr  - address of the control register
**            data  - 8-bit data to be written
**
** RETURNS:   nothing
**
*****************************************************************************
*/
static void MSPI_Write( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr, XDRV_UINT8 data )
{
   XDRV_UINT8    SPICmdAddressByte = SI_SPI_WRITE_CMD | addr;
   XDRV_UINT8    txBuffer[2];
   XDRV_UINT8    daisyChainDevSelect;
   int           i;
   XDRV_UINT8    dummy;


   /* Ignore the channel select byte for now since we only support single channel slics */
   (void) channel;

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
   
   /* update diasychain flag, regaddr 0 amd data 0x80 indicates daisychain mode is established */   
	if (addr == REG_DIRECT_SPIMODE)
	{
		if (data & SPI_DAISYCHAIN_MODE)
		{
			daisyChain=1;
		}
		else
		{
			daisyChain=0;
		}
	}
	
	/* Send device select only if in daisychain mode */
	if ( daisyChain )
	{
   	/* proslic api is operatin in daisychain mode. This mode requires a device  select *
      * Byte to be transmitted first. Our hardware is not wired this way therefore       *
      * our device  select byte is FF which would result in any device with its          *
      * chip select line asserted be able to communicate with the spi master             */
      daisyChainDevSelect = 0xFF;
      
      /* Send the device select byte */
      if( BcmSpiSyncTrans(&daisyChainDevSelect, &dummy, 0, 1, MSPI_BUS_ID, mspiDevId) )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "PROSlic api interface MSPI_Write Error\n"));
      }
   }
   else
   {
      /* To avoid compile warnings */
      (void) daisyChainDevSelect;
   }
   
   /* Place the command/address and data into the lower byte of each txBuffer */
   txBuffer[0] = SPICmdAddressByte;
   txBuffer[1] = data;

   /* Write to the MSPI */
   /* controller does not support one byte mode */
   for ( i = 0; i < 2; i++ )
   {
       if( BcmSpiSyncTrans(&txBuffer[i], &dummy, 0, 1, MSPI_BUS_ID, mspiDevId) )
       {
          XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "PROSlic api interface MSPI_Read Error\n"));
       }
   }
}


/*
*****************************************************************************
** FUNCTION:  MSPI_Indirect_Read
**
** PURPOSE:   Reads indirect registers via MSPI
**
** PARAMETERS:
**            mspiDevId - mspi Device id
**            channel  - channel for multi channel slics
**            addr - address of the indirect register
**
** RETURNS:
**            data - 16-bit data from the indirect register
**
*****************************************************************************
*/
static XDRV_UINT16 MSPI_Indirect_Read( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr )
{
   XDRV_UINT8 low;
   XDRV_UINT8 high;

   /* write to IAA without writing to IDA is interpreted as a read request */
   MSPI_Write( mspiDevId, channel, REG_INDIRECT_ADDRESS, addr );

   /* when the transfer is complete, a read of IAS register will return 0 */
   while( MSPI_Read( mspiDevId, channel, REG_INDIRECT_ADDRESS_STATUS ));

   /* read from IDA register - lower byte */
   low = MSPI_Read( mspiDevId, channel, REG_INDIRECT_DATA_ACCESS_LOW );

   /* read from IDA register - upper byte */
   high = MSPI_Read( mspiDevId, channel, REG_INDIRECT_DATA_ACCESS_HIGH );

   return(( high << 8 ) | low );
}


/*
*****************************************************************************
** FUNCTION:  MSPI_Indirect_Write
**
** PURPOSE:   Writes to indirect registers via MSPI
**
** PARAMETERS:
**            mspiDevId - mspi device id
**            channel  - channel for multi channel slics
**            data - 16-bit data for the indirect register
**            addr - address of the indirect register
**
** RETURNS:    nothing
**
*****************************************************************************
*/
static void MSPI_Indirect_Write( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr, XDRV_UINT16 data )
{
   /* Write data to lower byte of IDA register */
   MSPI_Write( mspiDevId, channel, REG_INDIRECT_DATA_ACCESS_LOW, (XDRV_UINT8)(data & 0x00FF) );

   /* Write data to higher byte of IDA register */
   MSPI_Write( mspiDevId, channel, REG_INDIRECT_DATA_ACCESS_HIGH, (XDRV_UINT8)(data >> 8) );

   /* Write register address to IAA register */
   MSPI_Write( mspiDevId, channel, REG_INDIRECT_ADDRESS, addr );

   /* when the transfer is complete, a read of IAS register will return 0 */
   while( MSPI_Read( mspiDevId, channel, REG_INDIRECT_ADDRESS_STATUS )){} 
}



