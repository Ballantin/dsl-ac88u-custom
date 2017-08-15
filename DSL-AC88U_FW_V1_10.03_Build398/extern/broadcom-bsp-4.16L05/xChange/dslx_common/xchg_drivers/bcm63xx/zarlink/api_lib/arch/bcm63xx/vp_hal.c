/** \file vp_hal.c
 * vp_hal.c
 *
 * This file contains the platform dependent code for the Hardware Abstraction
 * Layer (HAL). This is example code only to be used by the customer to help
 * clarify HAL requirements.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 */
#include "vp_api_types.h"
#include "mspiDef.h"
#include "sys_service.h"
#include "hbi_hal.h"
#include "mpi_hal.h"

#include "bcmSpiRes.h"
#include <stdio.h>
#include <bosSleep.h>

#include "vp_zarlink_common.h"
/*
 * The API header is needed to define the Device Types used by the API to know
 * how to implement VpMpiReset
 */
#include "vp_api_dev_term.h"

#if defined(BRCM_68500)
#include <soc/bl_lilac_soc.h>
#include "spi_driver.h"
#endif /* defined(BRCM_68500) */

#define WRITE_COMMAND 0
#define READ_COMMAND 1

#define ZMPI_PCM_CLK 2048000
#define ZMPI_SPI_CLK MSPI_CLK_FREQUENCY

#if defined(CONFIG_BCM96816)
static int spi_bus_num_special = 0;
#endif

/*****************************************************************************
 * HAL functions for CSLAC devices. Not necessary for VCP
 ****************************************************************************/

/*****************************************************************************
 * HAL functions to read from Le88004/5 devices. Not necessary for VCP and VPP
 ****************************************************************************/
/**
 * ZmpiReadBasic8b(): Reads data over using the ZMPI protocol in basic 8 bits mode.
 *
 * This function is called by VpZmpiCmd().
 * For a pass-through command (SLACx) the basic mode function will compute
 * a delay based on SpiClk and PClk. A function "ndelay" included in
 * "<asm/delay.h>" will be used to wait this computed time (required to
 * synchronize SpiClk and PClk as well as the chip select off-time).
 *
 * Params:
 *   VpDeviceIdType deviceId: Device Id (chip select ID)
 *   uint8 subSlac: Identifier of the device to read (0: Le88004/5, 1: SLAC1, 2: SLAC2, 3: both SLAC)
 *   uint8 ecVal: Sub-SLAC channel identifier
 *   uint8 cmd: Register to read
 *   uint8 *dataPtr: Pointer to returned data
 *   uint8 dataLen: Number of bytes to read
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
static bool ZmpiReadBasic8b(
    VpDeviceIdType deviceId,
    uint8 subSlac,
    uint8 ecVal,
    uint8 cmd,
    uint8 dataLen,
    uint8 *dataPtr)
{
    uint8 dummy;
    unsigned long spiClkkHz = ZMPI_SPI_CLK/1000;
    unsigned long pClkkHz = ZMPI_PCM_CLK/1000;
    unsigned long spiDelay;
    unsigned long zsiDelay;
    unsigned long waitTimeNs;

    /* waitTime = 8bits / pClkHz + CS_off_time - 8bits / spiClkHz */
    spiDelay = (8000000 / spiClkkHz);
    zsiDelay = (8000000 / pClkkHz) + 2500;

    if (zsiDelay < spiDelay)
    {
      waitTimeNs = 0;
    }
    else
    {
      waitTimeNs = zsiDelay - spiDelay;
    }

    if (subSlac == 0) {
        uint8 byteCnt;
        /* Don't delay the transmission for a direct communication with Le88004/5 */
        BcmSpiSyncTrans(&cmd, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);

        if (dataLen != 0) {
            for(byteCnt=0; byteCnt < dataLen; byteCnt++)
            {
               BcmSpiSyncTrans(&dummy, &dataPtr[byteCnt], 0, 1, MSPI_BUS_ID, deviceId&0xFF);
            }
        }
    } else {
        uint8 tempBuf[2];
        uint8 byteCnt;
        uint8 prefix = (0xC0 | (subSlac << 4));

        if (subSlac > 3) {
            return FALSE;
        }

        /* Specify the EC value */
        tempBuf[0] = prefix;
        tempBuf[1] = 0x4A;  /* EC value register */
        BcmSpiSyncTrans(&tempBuf[0], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        BcmSpiSyncTrans(&tempBuf[1], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        bosSleepNs(waitTimeNs);

        tempBuf[0] = prefix;
        tempBuf[1] = ecVal;
        BcmSpiSyncTrans(&tempBuf[0], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        BcmSpiSyncTrans(&tempBuf[1], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        bosSleepNs(waitTimeNs);

        /* Specify the number of bytes to read in the prefix, then the opCode */
        prefix |= dataLen;
        tempBuf[0] = prefix;
        tempBuf[1] = cmd;
        BcmSpiSyncTrans(&tempBuf[0], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        BcmSpiSyncTrans(&tempBuf[1], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        bosSleepNs(waitTimeNs * 2);  /* Double delay required for transition from 'write' to 'read' */

        for (byteCnt = 0; byteCnt < dataLen; byteCnt++) {
            bosSleepNs(waitTimeNs);
            BcmSpiSyncTrans(&dummy, &dataPtr[byteCnt], 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        }
    }

    return TRUE;
} /* End ZmpiReadBasic8b */

/*****************************************************************************
 * HAL functions to write to Le88004/5 devices. Not necessary for VCP and VPP
 ****************************************************************************/
/**
 * ZmpiWriteBasic8b(): Write data over using the ZMPI protocol in basic 8 bits mode.
 *
 * This function is called by VpZmpiCmd().
 * For a pass-through command (sub-SLACx) the basic mode function will compute
 * a delay based on SpiClk and PClk. A function "ndelay" included in
 * "<asm/delay.h>" will be used to wait this computed time (required to
 * synchronize SpiClk and PClk as well as the chip select off-time).
 *
 * Params:
 *   VpDeviceIdType deviceId: Device Id (chip select ID)
 *   uint8 subSlac: Identifier of the device to read (0: Le88004/5, 1: SLAC1, 2: SLAC2, 3: both SLAC)
 *   uint8 ecVal: Sub-SLAC channel identifier
 *   uint8 cmd: Register to write
 *   uint8 *dataPtr: Pointer to returned data
 *   uint8 dataLen: Number of bytes to read
 *
 * Returns:
 *   TRUE on success, FALSE on failure
 */
static bool ZmpiWriteBasic8b(
    VpDeviceIdType deviceId,
    uint8 subSlac,
    uint8 ecVal,
    uint8 cmd,
    uint8 dataLen,
    uint8 *dataPtr)
{
    uint8 dummy;
#if 0
    unsigned long spiClkkHz = ZMPI_SPI_CLK/1000;
    unsigned long pClkkHz = ZMPI_PCM_CLK/1000;
    unsigned long spiDelay;
    unsigned long zsiDelay;
    unsigned long waitTimeNs;

    /* WaitTime = 8bits / pClkHz + CS_off_time - 8bits / spiClkHz */
    spiDelay = (8000000 / spiClkkHz);
    zsiDelay = (8000000 / pClkkHz) + 2500;

    if (zsiDelay < spiDelay)
    {
      waitTimeNs = 0;
    }
    else
    {
      waitTimeNs = zsiDelay - spiDelay;
    }
#endif

    if (subSlac == 0) {
        uint8 byteCnt;
        /* Don't delay the transmission for a direct communication with Le88004/5 */
        BcmSpiSyncTrans(&cmd, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        if (dataLen != 0) {
            for (byteCnt = 0; byteCnt < dataLen; byteCnt++) {
                BcmSpiSyncTrans(&dataPtr[byteCnt], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
            }
        }
    } else {
        uint8 byteCnt;
        uint8 prefix = 0xC0;
        uint8 data;

        if (subSlac > 3) {
            return FALSE;
        }
        prefix |= (subSlac << 4) & 0xF0;

        /* Send the EC value */
        data = 0x4A;
        BcmSpiSyncTrans(&prefix, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        BcmSpiSyncTrans(&data, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        //bosSleepNs(waitTimeNs);

        data = ecVal;
        BcmSpiSyncTrans(&prefix, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        BcmSpiSyncTrans(&data, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        //bosSleepNs(waitTimeNs);

        /* Send the command */
        data = cmd;
        BcmSpiSyncTrans(&prefix, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        BcmSpiSyncTrans(&data, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
        //bosSleepNs(waitTimeNs);

        for (byteCnt = 0 ; byteCnt < dataLen ; byteCnt++) {
            /* Send the Prefix and the data */
            BcmSpiSyncTrans(&prefix, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
            BcmSpiSyncTrans(&dataPtr[byteCnt], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
            //bosSleepNs(waitTimeNs);
        }
    }

    return TRUE;
} /* End ZmpiWriteBasic8b */

/**
 * VpZmpiCmd()
 * This function executes a Device ZMPI command from VpMpiCmd(). It
 * executes both read and write commands. The read or write operation is
 * determined by the "cmd" argument (odd = read, even = write). The caller must
 * ensure that the data array is large enough to hold the data being collected.
 * Because this command used hardware resources, this procedure is not
 * re-entrant.
 *
 * Note: For API-II to support multi-threading, this function has to write to
 * the EC register of the device to set the line being controlled, in addition
 * to the command being passed. The EC register write/read command is the same
 * for every CSLAC device and added to this function. The only exception is
 * if the calling function is accessing the EC register (read), in which case
 * the EC write cannot occur.

 * Preconditions:
 *  The device must be initialized.
 *
 * Postconditions:
 *   The data pointed to by dataPtr, using the command "cmd", with length
 * "cmdLen" has been sent to the ZMPI bus via the chip select associated with
 * deviceId.
 */
static void VpZmpiCmd(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 cmd,
    uint8 dataLen,
    uint8 *dataPtr)
{
    bool readCmd = TRUE;
    uint8 subSlac;

    /* Determine the sub-slac from the device id */
    subSlac = (deviceId & 0x0F00) >> 8;

    /* Determine if we are reading or writing */
    if (subSlac == 0) {
        /* "Hunt", "System Reset" and "SLAC Reset" are write commands */
        if((cmd == 0x00) || (cmd == 0x02) || (cmd == 0x03)) {
            readCmd = FALSE;
        } else {
            readCmd = !(cmd & 0x80);
        }
    } else {
        readCmd = (cmd & 0x01);
    }

    /* issue the read or write command */
    if (readCmd) {
        ZmpiReadBasic8b(deviceId, subSlac, ecVal, cmd, dataLen, dataPtr);
    } else {
        ZmpiWriteBasic8b(deviceId, subSlac, ecVal, cmd, dataLen, dataPtr);
    }
} /* End VpZmpiCmd */

/***********************/
/* Broadcom's VpMpiCmd */
/***********************/
static void brcmVpMpiCmd(
    VpDeviceIdType deviceId,    /**< Chip select, connector and 3 or 4 wire
                                 * interface for command
                                 */
    uint8 ecVal,        /**< Value to write to the EC register */
    uint8 cmd,          /**< Command number */
    uint8 cmdLen,       /**< Number of bytes used by command (cmd) */
    uint8 *dataPtr)     /**< Pointer to the data location */
{
    uint8 isRead = (cmd & READ_COMMAND);

#if !defined(BRCM_68500)
    uint8 byteCnt;
    uint8 dummy;
#endif

    /* Break out if Thalassa device */
    if (deviceId & 0x8000) {
       VpZmpiCmd(deviceId, ecVal, cmd, cmdLen, dataPtr);
       return;
    }
#define CSLAC_EC_REG_RD    0x4B   /* Same for all CSLAC devices */
#define CSLAC_EC_REG_WRT   0x4A   /* Same for all CSLAC devices */
#define CSLAC_EC_REG_LEN   0x01   /* Same for all CSLAC devices */
    /* Configure glue logic as necessary to talk to the device */
    /* Start critical section for MPI access */
#if defined(BRCM_68500)
    VpSysEnterCritical(deviceId, VP_MPI_CRITICAL_SEC);
    /* If a EC read is being preformed don't set the EC register */
    if (CSLAC_EC_REG_RD != cmd) {
       SPI_bytes_write( deviceId, CSLAC_EC_REG_WRT, &ecVal, 1 );
    }

    /* Write the command byte to MPI. */
    /* MpiWriteByte(deviceId, cmd); */
    if (isRead) {
       SPI_bytes_read( deviceId, cmd, dataPtr, cmdLen );
    } else {
       SPI_bytes_write( deviceId, cmd, dataPtr, cmdLen );
    }
    VpSysExitCritical( deviceId, VP_MPI_CRITICAL_SEC );
#else
    VpSysEnterCritical(deviceId&0xFF, VP_MPI_CRITICAL_SEC);
    /* If a EC read is being preformed don't set the EC register */
    if (CSLAC_EC_REG_RD == cmd) {
    } else {
       uint8 data = CSLAC_EC_REG_WRT;
       /* Write the EC register value passed to the device */
       /* MpiWriteByte(deviceId, CSLAC_EC_REG_WRT); */
       BcmSpiSyncTrans(&data, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);

       /* MpiWriteByte(deviceId, ecVal); */
       BcmSpiSyncTrans(&ecVal, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
    }
    /* Write the command byte to MPI. */
    /* MpiWriteByte(deviceId, cmd); */
    BcmSpiSyncTrans(&cmd, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);

    if (isRead) {
       if (deviceId & 0x4000) {
          uint8 nopData = 0x6; /* NOP */
          /* This needs to be handled as a special case for direct ZSI-only devices where 1st byte is throw-away. */
          /* Read first throw-away byte */
          BcmSpiSyncTrans(&nopData, &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);

          for(byteCnt=0; byteCnt < cmdLen; byteCnt++)
          {
             BcmSpiSyncTrans(&nopData, &dataPtr[byteCnt], 0, 1, MSPI_BUS_ID, deviceId&0xFF);
          }
       }
       else
       {
          // Write a dummy out, to assert the chip select and allow the SLAC to output the data
          for(byteCnt=0; byteCnt < cmdLen; byteCnt++)
          {
             BcmSpiSyncTrans(&dummy, &dataPtr[byteCnt], 0, 1, MSPI_BUS_ID, deviceId&0xFF);
          }
       }
    } else {
       /* If writing, access data from buffer passed and write to MPI */
       for (byteCnt = 0; byteCnt < cmdLen; byteCnt++) {
          /* MpiWriteByte(deviceId, dataPtr[byteCnt]); */
          BcmSpiSyncTrans(&dataPtr[byteCnt], &dummy, 0, 1, MSPI_BUS_ID, deviceId&0xFF);
       }
    }
    VpSysExitCritical(deviceId&0xFF, VP_MPI_CRITICAL_SEC);
#endif /* BRCM_68500 */
    return;
} /* End brcmVpMpiCmd */


/**
 * VpMpiCmd()
 *  This function executes a Device MPI command through the MPI port. It
 * executes both read and write commands. The read or write operation is
 * determined by the "cmd" argument (odd = read, even = write). The caller must
 * ensure that the data array is large enough to hold the data being collected.
 * Because this command used hardware resources, this procedure is not
 * re-entrant.
 *
 * Note: For API-II to support multi-threading, this function has to write to
 * the EC register of the device to set the line being controlled, in addition
 * to the command being passed. The EC register write/read command is the same
 * for every CSLAC device and added to this function. The only exception is
 * if the calling function is accessing the EC register (read), in which case
 * the EC write cannot occur.
 *
 * This example assumes the implementation of two byte level commands:
 *
 *    MpiReadByte(VpDeviceIdType deviceId, uint8 *data);
 *    MpiWriteByte(VpDeviceIdType deviceId, uint8 data);
 *
 * Preconditions:
 *  The device must be initialized.
 *
 * Postconditions:
 *   The data pointed to by dataPtr, using the command "cmd", with length
 * "cmdLen" has been sent to the MPI bus via the chip select associated with
 * deviceId.
 */
void
VpMpiCmd(
    VpDeviceIdType deviceId,    /**< Chip select, connector and 3 or 4 wire
                                 * interface for command
                                 */
    uint8 ecVal,        /**< Value to write to the EC register */
    uint8 cmd,          /**< Command number */
    uint8 cmdLen,       /**< Number of bytes used by command (cmd) */
    uint8 *dataPtr)     /**< Pointer to the data location */
{
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
   bosMutexAcquire( &spiZarMutex );
   brcmVpMpiCmd( deviceId, ecVal, cmd, cmdLen, dataPtr );
   bosMutexRelease( &spiZarMutex );
}
/* End VpMpiCmd */

