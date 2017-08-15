/****************************************************************************
 * <:copyright-BRCM:2014:DUAL/GPL:standard
 * 
 *    Copyright (c) 2014 Broadcom 
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
***************************************************************************/
#ifndef __I2S_H
#define __I2S_H

/************************************************************************** 
 *  i2s raw data driver:
 *  --------------------
 *  Supported Sampling rates:   Supported data widths:
 *    16000Hz                       32-bit
 *    32000Hz                       24-bit
 *    44100Hz                       20-bit
 *    48000Hz                       18-bit
 *    96000Hz                       16-bit
 *    192000Hz
 *    384000Hz
 *
 *  Note: For all data widths less than 32-bit, each sample needs to be 
 *  aligned to the MSB of a 32-bit word
 **************************************************************************/

#define I2S_MAJOR       256
#define I2S_SAMPLING_FREQ_SET_IOCTL 1

#endif /* __I2S_H */
