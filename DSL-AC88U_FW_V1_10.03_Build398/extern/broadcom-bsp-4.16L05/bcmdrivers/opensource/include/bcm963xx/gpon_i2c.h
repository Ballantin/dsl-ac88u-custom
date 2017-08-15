/*
 
 Copyright (c) 2008 Broadcom Corporation 
* <:copyright-BRCM:2012:DUAL/GPL:standard 
* 
*    Copyright (c) 2012 Broadcom 
*    All Rights Reserved
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License, version 2, as published by
* the Free Software Foundation (the "GPL").
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* 
* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
* 
* :>
*/
/***********************************************************************/
/*                                                                     */
/*   MODULE   gpon_i2c.h                                               */
/*   DATE:    07/23/08                                                 */
/*   PURPOSE: GPON Transceiver reg access API                          */
/*                                                                     */
/***********************************************************************/
#ifndef __GPON_I2C_H
#define __GPON_I2C_H

#define MAX_TRANSACTION_SIZE  32



/* Global Comments */
/* client_num = 0 selects the first EEPROM at I2C address 0xA0 and non-zero */
/* client_num selects the second EEPROM at I2C address 0xA2.                */

/****************************************************************************/
/* Write gponPhy: Writes count number of bytes from buf on to the I2C bus   */
/* Returns:                                                                 */
/*   number of bytes written on success, negative value on failure.         */
/* Notes: 1. The count > 32 is not yet supported                            */
/*        2. The buf[0] should be the offset where write starts             */
/****************************************************************************/
ssize_t gponPhy_write(u8 client_num, char *buf, size_t count);

/****************************************************************************/
/* Read gponPhy: Reads count number of bytes from gponPhy                   */
/* Returns:                                                                 */
/*   number of bytes read on success, negative value on failure.            */
/* Notes: 1. The count > 32 is not yet supported                            */
/*        2. The buf[0] should be the offset where read starts              */
/****************************************************************************/
ssize_t gponPhy_read(u8 client_num, char *buf, size_t count);

/****************************************************************************/
/* Write Register: Writes the val into gponPhy register                     */
/* Returns:                                                                 */
/*   0 on success, negative value on failure.                               */
/* Notes: 1. The offset should be DWORD aligned                             */
/****************************************************************************/
int gponPhy_write_reg(u8 client_num, u8 offset, int val);

/****************************************************************************/
/* Read Register: Read the gponPhy register at given offset                 */
/* Returns:                                                                 */
/*   value on success, negative value on failure.                           */
/* Notes: 1. The offset should be DWORD aligned                             */
/****************************************************************************/
int gponPhy_read_reg(u8 client_num, u8 offset);

/****************************************************************************/
/* Write Word: Writes the val into LSB 2 bytes of Register                  */ 
/* Returns:                                                                 */
/*   0 on success, negative value on failure.                               */
/* Notes: 1. The offset should be WORD aligned                              */
/****************************************************************************/
int gponPhy_write_word(u8 client_num, u8 offset, u16 val);

/****************************************************************************/
/* Read Word: Reads the LSB 2 bytes of Register                             */ 
/* Returns:                                                                 */
/*   value on success, negative value on failure.                           */
/* Notes: 1. The offset should be WORD aligned                              */
/****************************************************************************/
u16 gponPhy_read_word(u8 client_num, u8 offset);

/****************************************************************************/
/* Write Byte: Writes the byte val into offset                              */ 
/* Returns:                                                                 */
/*   0 on success, negative value on failure.                               */
/****************************************************************************/
int gponPhy_write_byte(u8 client_num, u8 offset, u8 val);

/****************************************************************************/
/* Read Byte: Reads a byte from offset                                      */ 
/* Returns:                                                                 */
/*   value on success, negative value on failure.                           */
/****************************************************************************/
u8 gponPhy_read_byte(u8 client_num, u8 offset);

#endif
