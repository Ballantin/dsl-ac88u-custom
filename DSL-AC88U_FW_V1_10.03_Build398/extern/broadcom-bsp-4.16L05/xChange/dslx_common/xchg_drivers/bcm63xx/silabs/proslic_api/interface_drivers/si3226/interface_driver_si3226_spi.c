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

#include "interface_driver_si3226_spi.h"
#include "bcmChip.h"
#include <kernelMainWrapper.h>
#include "mspiDef.h"
#include "si_voice_datatypes.h"
#include <xdrvLog.h>
#include <bosSleep.h>
#include "si3226_reset.h"
#include "bcmSpiRes.h"
#include "interface_driver_si_common.h"

/* Spi Control Interface */
#define SI3226_SPI_BRDCST_CMD        0x80
#define SI3226_SPI_READ_CMD          0x40
#define SI3226_SPI_WRITE_CMD         0x00
#define SI3226_SPI_REG_CMD           0x20
#define SI3226_SPI_RAM_CMD           0x00
#define SI3226_SPI_CID_MSK           0x1F

/* Indirect register access */
#define SI3226_REG_UAM               126
#define SI3226_UAM_BYTE_0            0x02
#define SI3226_UAM_BYTE_1            0x08
#define SI3226_UAM_BYTE_2            0x0E
#define SI3226_UAM_BYTE_3            0x00
#define SI3226_REG_RAMSTAT              4
#define SI3226_REG_RAM_ADDR_HI          5
#define SI3226_RAM_ADDR_HI_SHFT         8
#define SI3226_RAM_ADDR_HI_MSK     0x0700
#define SI3226_RAM_ADDR_10_MSK       0x80
#define SI3226_REG_RAM_ADDR_LO         10
#define SI3226_RAM_ADDR_LO_MSK     0x00FF
#define SI3226_REG_RAM_DATA_B0          6
#define SI3226_RAM_DATA_B0_RD_MSK    0xF8
#define SI3226_REG_RAM_DATA_B1          7
#define SI3226_REG_RAM_DATA_B2          8
#define SI3226_REG_RAM_DATA_B3          9
#define SI3226_RAM_DATA_B0_WR_MSK       0x0000001F
#define SI3226_RAM_DATA_B1_WR_MSK       0x00001FE0
#define SI3226_RAM_DATA_B2_WR_MSK       0x001FE000
#define SI3226_RAM_DATA_B3_WR_MSK       0x1FE00000

#define CNUM_TO_CID_QUAD(channelNumber) (((channelNumber<<4)&0x10)|((channelNumber<<2)&0x8)|((channelNumber>>2)&0x2)|((channelNumber>>4)&0x1)|(channelNumber&0x4))

/* Internal Functions */
static XDRV_UINT8 MSPI_Read( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr );
static void MSPI_Write( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT8 addr, XDRV_UINT8 data );
static XDRV_UINT32 MSPI_Indirect_Read( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT16 addr );
static void MSPI_Indirect_Write( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT16 addr, XDRV_UINT32 data );
#if defined(CONFIG_BCM96816)  
static int spi_bus_num_special = 0;
#endif
/*
******************************************************************************
** FUNCTION:  SPI_3226_Init
**
** PURPOSE:   Initializes the SPI interface
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
int SPI_3226_Init (ctrl_S * hSpi)
{
	return 0;
}


/*
******************************************************************************
** FUNCTION:  ctrl_3226_ResetWrapper
**
** PURPOSE:   Sets reset pins of slic
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
int ctrl_3226_ResetWrapper (void * hSpi, int status)
{
	if (status)
	{
		Si3226Reset(1);
	}
	else
	{
		Si3226Reset(0);
	}
	return 0;
}

/*
******************************************************************************
** FUNCTION:  ctrl_3226_ReadRegisterWrapper
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
XDRV_UINT8 ctrl_3226_ReadRegisterWrapper (void * hSpi, XDRV_UINT8 channel,XDRV_UINT8 regAddr)
{
   XDRV_UINT8 readValue;
   
   bosMutexAcquire( &spiSiMutex );
   readValue =  MSPI_Read(((ctrl_S*)hSpi)->portID,channel,regAddr);			
   bosMutexRelease( &spiSiMutex );

   return readValue;			
}


/*
******************************************************************************
** FUNCTION:  ctrl_3226_WriteRegisterWrapper
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
int ctrl_3226_WriteRegisterWrapper (void * hSpi, XDRV_UINT8 channel,  XDRV_UINT8 regAddr, XDRV_UINT8 data)
{
   bosMutexAcquire( &spiSiMutex );   
   MSPI_Write(((ctrl_S*)hSpi)->portID, channel, regAddr, data);
   bosMutexRelease( &spiSiMutex );
	return 0;
}


/*
******************************************************************************
** FUNCTION:  ctrl_3226_ReadRAMWrapper
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
XDRV_UINT32 ctrl_3226_ReadRAMWrapper (void * hSpi, XDRV_UINT8 channel, XDRV_UINT16 ramAddr)
{
   XDRV_UINT32 readValue;
   
   bosMutexAcquire( &spiSiMutex );   
   readValue = MSPI_Indirect_Read( ((ctrl_S*)hSpi)->portID, channel, ramAddr );
   bosMutexRelease( &spiSiMutex );
   
   return readValue;
}

/*
******************************************************************************
** FUNCTION:  ctrl_3226_WriteRAMWrapper
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
int ctrl_3226_WriteRAMWrapper (void * hSpi, XDRV_UINT8 channel, XDRV_UINT16 ramAddr, XDRV_UINT32 data)
{
   bosMutexAcquire( &spiSiMutex ); 
   MSPI_Indirect_Write( ((ctrl_S*)hSpi)->portID, channel, ramAddr, data );
   bosMutexRelease( &spiSiMutex );
	return 0;
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
   XDRV_UINT8    SPICmdControlByte;
   XDRV_UINT8    txBuffer[3] = { 0, 0, 0 };
   XDRV_UINT8    rxBuffer[3] = { 0, 0, 0 };
#if 0
   XDRV_UINT8    temp_cid;
#endif
   XDRV_UINT8    chan_cid;
   int           i;
   
/* Debug */
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Read: mspiDevId %d, channel %d, addr 0x%08x ", mspiDevId,
//channel, addr));  

#if 0
   temp_cid = channel & SI3226_SPI_CID_MSK;
   /* Bits in Si3226 SPI Control byte are reversed for CID field.  ie: CID[4:0] must be bit reversed */
   chan_cid = (temp_cid & 0x1) << 4;
   chan_cid |= (temp_cid & 0x2) << 2;
   chan_cid |= (temp_cid & 0x4);
   chan_cid |= (temp_cid & 0x8) >> 2;
   chan_cid |= (temp_cid & 0x10) >> 4;
#endif

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
   
   chan_cid = CNUM_TO_CID_QUAD(channel);
   
   SPICmdControlByte = SI3226_SPI_READ_CMD | SI3226_SPI_REG_CMD | chan_cid;
    
   txBuffer[0] = SPICmdControlByte;
   txBuffer[1] = addr;

   /* Write to the MSPI */
   /* controller does not support one byte mode */
   for ( i = 0; i < 3; i++ )
   {
       if( BcmSpiSyncTrans(&txBuffer[i], &rxBuffer[i], 0, 1, MSPI_BUS_ID, mspiDevId) )
       {
          XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "PROSlic api interface MSPI_Read Error"));
       }
   }

   return( rxBuffer[2] & 0xFF );
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
   XDRV_UINT8    SPICmdControlByte;
   XDRV_UINT8    txBuffer[3] = {0, 0, 0};
#if 0
   XDRV_UINT8    temp_cid;
#endif
   XDRV_UINT8    chan_cid;
   int           i;
   XDRV_UINT8    dummy;


/* Debug */
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"Proslic: MSPI_Write: mspiDevId %d, channel %d, addr 0x%08x, data 0x%08x ", mspiDevId,
//channel, addr, data)); 
#if 0
   temp_cid = channel & SI3226_SPI_CID_MSK;
   /* Bits in Si3226 SPI Control byte are reversed for CID field.  ie: CID[4:0] must be bit reversed */
   chan_cid = (temp_cid & 0x1) << 4;
   chan_cid |= (temp_cid & 0x2) << 2;
   chan_cid |= (temp_cid & 0x4);
   chan_cid |= (temp_cid & 0x8) >> 2;
   chan_cid |= (temp_cid & 0x10) >> 4;
#endif

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

   chan_cid = CNUM_TO_CID_QUAD(channel);

   if( channel == 0xFF )
   {
      SPICmdControlByte = SI3226_SPI_BRDCST_CMD | SI3226_SPI_WRITE_CMD | SI3226_SPI_REG_CMD | chan_cid;
   }
   else
   {
      SPICmdControlByte = SI3226_SPI_WRITE_CMD | SI3226_SPI_REG_CMD | chan_cid;
   }
   /* Place the command/address and data into the lower byte of each txBuffer */
   txBuffer[0] = SPICmdControlByte;
   txBuffer[1] = addr;
   txBuffer[2] = data;

   /* Write to the MSPI */
   /* controller does not support one byte mode */
   for ( i = 0; i < 3; i++ )
   {
       if( BcmSpiSyncTrans(&txBuffer[i], &dummy, 0, 1, MSPI_BUS_ID, mspiDevId) )
       {
          XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "PROSlic api interface MSPI_Read Error"));
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
static XDRV_UINT32 MSPI_Indirect_Read( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT16 addr )
{
#if 0
   XDRV_UINT8   uam_status=0;
#endif
   XDRV_UINT32  data=0;
   XDRV_UINT8   ram_addr_hi_10_required=0;
   XDRV_UINT8   hi_addr, lo_addr;
   XDRV_UINT8   b0_data=0, b1_data=0, b2_data=0, b3_data=0;

/* Debug */
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Read: mspiDevId %d, channel %d, addr %d ", mspiDevId,
//channel, addr));   

   /* Save high address bits */
   hi_addr = ((addr & SI3226_RAM_ADDR_HI_MSK) >> 8);   /* grab bits 10, 9, 8 and store in UINT8 */
   hi_addr <<= 5;                                      /* shift back to position 7, 6, 5 */ 
   lo_addr = (addr & SI3226_RAM_ADDR_LO_MSK);
   
   /* Check RAM_ADDR_HI[10] bit to see if it needs to be used.  If not, we can save byte transfers by not
      setting User Access Mode */
   if( hi_addr & SI3226_RAM_ADDR_10_MSK )
   {
      ram_addr_hi_10_required = 1;
   }   

#if 0 /* Already done in ProSlic driver */ 
   if( ram_addr_hi_10_required )
   {
      /* Check User Access Mode state */
      uam_status = (MSPI_Read( mspiDevId, channel, SI3226_REG_UAM) & 0x1);
   
      /* If not in UAM mode, then set to UAM to be able to use RAM_ADDR_HI[10] bit. */
      if( uam_status != 0x1 )
      {
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_0 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_1 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_2 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_3 );
         uam_status = 0x1;  /* set uam status */
      }
   }
#endif
   
   /* Program RAM_ADDR_HI register */
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_ADDR_HI, hi_addr );
   
   /* Program RAM_ADDR_LO register */
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_ADDR_LO, lo_addr );
   
   /* Wait for RAMSTAT bit */
   while( MSPI_Read( mspiDevId, channel, SI3226_REG_RAMSTAT ) & 0x1 );
  
   if( ram_addr_hi_10_required )
   {
      /* Clear RAM_ADDR_HI [10] bit */
      MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_ADDR_HI, ~SI3226_RAM_ADDR_10_MSK&hi_addr );
 #if 0 /* Already done in ProSlic driver */ 
      /* If we are in UAM mode, then clear UAM by sending UAM sequence again. */
      if( uam_status == 0x1 )
      {
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_0 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_1 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_2 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_3 );
         uam_status = 0x0;  /* unset uam status */
      }
#endif
   }
   
   /* Read RAM_DATA_B0 */
   b0_data = MSPI_Read( mspiDevId, channel, SI3226_REG_RAM_DATA_B0 );
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Read: b0_data 0x%08x", b0_data)); 
   data = (b0_data & SI3226_RAM_DATA_B0_RD_MSK) >> 3;

   /* Read RAM_DATA_B1 */ 
   b1_data = MSPI_Read( mspiDevId, channel, SI3226_REG_RAM_DATA_B1 );
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Read: b1_data 0x%08x", b1_data));
   data |= b1_data << 5;    

   /* Read RAM_DATA_B2 */ 
   b2_data = MSPI_Read( mspiDevId, channel, SI3226_REG_RAM_DATA_B2 );
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Read: b2_data 0x%08x", b2_data));   
   data |= b2_data << 13;

   /* Read RAM_DATA_B3 */
   b3_data = MSPI_Read( mspiDevId, channel, SI3226_REG_RAM_DATA_B3 );
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"Proslic: MSPI_Indirect_Read: b3_data 0x%08x", b3_data));   
   data |= b3_data << 21; 


/* Debug */
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Read: mspiDevId %d, channel %d, addr 0x%08x, data: %d ", mspiDevId,
//channel, addr, data));   
   return data;
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
static void MSPI_Indirect_Write( int mspiDevId, XDRV_UINT8 channel, XDRV_UINT16 addr, XDRV_UINT32 data )
{
#if 0
   XDRV_UINT8   uam_status=0;
#endif
   XDRV_UINT8   b0_data=0, b1_data=0, b2_data=0, b3_data=0;
   XDRV_UINT8   ram_addr_hi_10_required=0;
   XDRV_UINT8   hi_addr, lo_addr;

/* Debug */
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Write: mspiDevId %d, channel %d, addr %d, data 0x%08x", mspiDevId,
//channel, addr, data));
   
   /* Save high address bits */
   hi_addr = ((addr & SI3226_RAM_ADDR_HI_MSK) >> 8);   /* grab bits 10, 9, 8 and store in UINT8 */
   hi_addr <<= 5;                                      /* shift back to position 7, 6, 5 */ 
   lo_addr = (addr & SI3226_RAM_ADDR_LO_MSK);
   

   /* Check RAM_ADDR_HI[10] bit to see if it needs to be used.  If not, we can save byte transfers by not
      setting User Access Mode */
   if( hi_addr & SI3226_RAM_ADDR_10_MSK )
   {
      ram_addr_hi_10_required = 1;
   }
#if 0 /* Already done in ProSlic driver */       
   if( ram_addr_hi_10_required )
   {
      /* Check User Access Mode state */
      uam_status = (MSPI_Read( mspiDevId, channel, SI3226_REG_UAM) & 0x1);

      /* If not in UAM mode, then set to UAM to be able to use RAM_ADDR_HI[10] bit. */
      if( uam_status == 0x0 )
      {
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_0 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_1 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_2 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_3 );
         uam_status = 0x1;  /* set uam status */
      }
   }
#endif
   
   /* Program RAM_ADDR_HI register */
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_ADDR_HI, hi_addr );
   
   /* Write RAM_DATA_B0 */ 
   b0_data = ((data & SI3226_RAM_DATA_B0_WR_MSK) << 3);
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Write: b0_data 0x%08x", b0_data));   
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_DATA_B0, b0_data );
   /* Write RAM_DATA_B1 */
   b1_data = ((data & SI3226_RAM_DATA_B1_WR_MSK) >> 5); 
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Write: b1_data 0x%08x", b1_data));   
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_DATA_B1, b1_data );
   /* Write RAM_DATA_B2 */ 
   b2_data = ((data & SI3226_RAM_DATA_B2_WR_MSK) >> 13); 
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Write: b2_data 0x%08x", b2_data));   
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_DATA_B2, b2_data );
   /* Write RAM_DATA_B3 */ 
   b3_data = ((data & SI3226_RAM_DATA_B3_WR_MSK) >> 21);
//XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Proslic: MSPI_Indirect_Write: b3_data 0x%08x", b3_data));    
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_DATA_B3, b3_data ); 
   
   /* Program RAM_ADDR_LO register */
   MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_ADDR_LO, lo_addr );
   

   /* Wait for RAMSTAT bit */
   while( MSPI_Read( mspiDevId, channel, SI3226_REG_RAMSTAT ) & 0x1);


   if( ram_addr_hi_10_required )
   {
      /* Clear RAM_ADDR_HI [10] bit */
      MSPI_Write( mspiDevId, channel, SI3226_REG_RAM_ADDR_HI, ~SI3226_RAM_ADDR_10_MSK&hi_addr );
#if 0 /* Already done in ProSlic driver */ 
      /* If we are in UAM mode, then clear UAM by sending UAM sequence again. */
      if( uam_status == 0x1 )
      {
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_0 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_1 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_2 );
         MSPI_Write( mspiDevId, channel, SI3226_REG_UAM, SI3226_UAM_BYTE_3 );
         uam_status = 0x0;  /* unset uam status */
      }
#endif
   }
}

void Si3226_MSPI_dump_all(void)
{
   int i;
   XDRV_UINT8 data;
   XDRV_UINT32 ramdata;
   
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"MSPI REGISTER DUMP:")); 
   for( i=0; i < 99; i++)
   {  
      data = MSPI_Read( 2, 0, i );
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"MSPI REG Read Channel 0  Reg: %d Value: 0x%08x", i, data )); 
      data = MSPI_Read( 2, 1, i );
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"MSPI REG Read Channel 1  Reg: %d Value: 0x%08x", i, data )); 
   }
   
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "MSPI RAM DUMP:")); 
   for( i=0; i < 1024; i++)
   {
      ramdata = MSPI_Indirect_Read( 2, 0, i );
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "MSPI RAM Read Channel 0  RAM: %d Value: 0x%08x", i, (unsigned int)ramdata )); 
      ramdata = MSPI_Indirect_Read( 2, 1, i );
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "MSPI RAM Read Channel 1  RAM: %d Value: 0x%08x", i, (unsigned int)ramdata ));
   } 
}   
   

