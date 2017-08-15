/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: interface_driver_si3226x_spi.h
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the spi interface for
*      proslic api
*
****************************************************************************/

#ifndef SPI_3226x_TYPE_H
#define SPI_3226x_TYPE_H

#include <xdrvTypes.h>
/*
** SPI/GCI structure
*/
typedef struct{
	int portID;
} ctrl_S;

/*
******************************************************************************
** FUNCTION:  SPI_3226x_Init
**
** PURPOSE:   Initializes the SPI interface
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
int SPI_3226x_Init (ctrl_S * hSpi);


/*
******************************************************************************
** FUNCTION:  ctrl_3226x_ResetWrapper
**
** PURPOSE:   Sets reset pins of slic
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
int ctrl_3226x_ResetWrapper (void * hSpi, int status);

/*
******************************************************************************
** FUNCTION:  ctrl_3226x_ReadRegisterWrapper
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
XDRV_UINT8 ctrl_3226x_ReadRegisterWrapper (void * hSpi, XDRV_UINT8 channel,XDRV_UINT8 regAddr);


/*
******************************************************************************
** FUNCTION:  ctrl_3226x_WriteRegisterWrapper
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
int ctrl_3226x_WriteRegisterWrapper (void * hSpi, XDRV_UINT8 channel,  XDRV_UINT8 regAddr, XDRV_UINT8 data);


/*
******************************************************************************
** FUNCTION:  ctrl_3226x_ReadRAMWrapper
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
XDRV_UINT32 ctrl_3226x_ReadRAMWrapper (void * hSpi, XDRV_UINT8 channel, XDRV_UINT16 ramAddr);

/*
******************************************************************************
** FUNCTION:  ctrl_3226x_WriteRAMWrapper
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
int ctrl_3226x_WriteRAMWrapper (void * hSpi, XDRV_UINT8 channel, XDRV_UINT16 ramAddr, XDRV_UINT32 data);


#endif


