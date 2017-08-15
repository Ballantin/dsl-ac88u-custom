/*
 * <:copyright-BRCM:2013:DUAL/GPL:standard
 * 
 *    Copyright (c) 2013 Broadcom 
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
// BCMFORMAT: notabs reindent:uncrustify:bcm_minimal_i4.cfg

/*
  These are low level functions that can be called from CFE or from the Linux init code.
  If pinmux changes are needed after Linux init, support should be added to the board 
  driver including any necessary locking.
*/

#include "bcm_otp.h"

#ifdef _CFE_                                                
#include "lib_types.h"
#include "lib_string.h"
#include "bcm_map_part.h"
#define printk  printf
#else // Linux
#include <bcm_map_part.h>
#include <linux/string.h>
#include <linux/spinlock.h>

static DEFINE_SPINLOCK(otp_spinlock);
#endif

/**********************************************
** bcm_otp_get_row                           **
** This function reads otp row               **
** INPUT:                                    **
**     row  - The row number to read from    **
**     val  - The row val returned value     **
** OUTPUT:                                   **
**     0 on success, -1 on fail.             **
***********************************************/
int bcm_otp_get_row(int row, unsigned int* val)
{
    int      rval = 0;
    uint32_t cntr = BTRM_OTP_READ_TIMEOUT_CNT;

#ifndef _CFE_
    unsigned long flags;

    spin_lock_irqsave(&otp_spinlock, flags);
#endif

    /* turn on cpu mode, set up row addr, activate read word */
    JTAG_OTP->ctrl1 |= JTAG_OTP_CTRL_CPU_MODE;
    JTAG_OTP->ctrl3 = row;
    JTAG_OTP->ctrl0 = JTAG_OTP_CTRL_START | JTAG_OTP_CTRL_PROG_EN | JTAG_OTP_CTRL_ACCESS_MODE;

    /* Wait for low CMD_DONE (current operation has begun), reset countdown, wait for retrieval to complete */
    while(!((JTAG_OTP->status1) & JTAG_OTP_STATUS_1_CMD_DONE))
    {
        cntr--;
        if (cntr == 0)
        {
            rval = -1;
             break;
        }
    }

    /* If cntr nonzero, read was successful, retrieve data */
    if (cntr)
        *val = JTAG_OTP->status0;

    /* zero out the ctrl_0 reg, turn off cpu mode, return results */
    JTAG_OTP->ctrl0 = 0;
    JTAG_OTP->ctrl1 &= ~JTAG_OTP_CTRL_CPU_MODE;

#ifndef _CFE_
    spin_unlock_irqrestore(&otp_spinlock, flags);
#endif

    return rval;
}

int bcm_otp_is_btrm_boot(void)
{
    unsigned int row;
    int rval;

#if defined(_BCM963381_) || defined(CONFIG_BCM963381) || defined(CONFIG_BCM96848) || defined(_BCM96848_) || defined(_BCM963138_) || defined(CONFIG_BCM963138)
#if defined(_BCM963381_) || defined(CONFIG_BCM963381)
    if ((MISC->miscStrapBus & MISC_STRAP_BUS_SPI_NAND_DISABLE) == 0)
#elif defined (CONFIG_BCM96848) || defined(_BCM96848_)
    if ((MISC->miscStrapBus & MISC_STRAP_BUS_BOOT_SEL_MASK) == MISC_STRAP_BUS_BOOT_SPI_NAND)
#elif defined(_BCM963138_) || defined(CONFIG_BCM963138)
    if ((MISC->miscStrapBus & MISC_STRAP_BUS_SW_BOOT_NORMAL_MASK) == 0)
#endif
    {
        return 1;
    }
#endif

    rval = bcm_otp_get_row(OTP_BRCM_BTRM_BOOT_ENABLE_ROW, &row);
    if ( !rval && (row & OTP_BRCM_BTRM_BOOT_ENABLE_MASK) )
    {
        rval = bcm_otp_get_row( OTP_CUST_BTRM_BOOT_ENABLE_ROW, &row);
        if ( !rval && (row & OTP_CUST_BTRM_BOOT_ENABLE_MASK) )
            rval = 1;
        else
            rval = 0;
    }
    else
        rval = 0;

    return rval;
}

int bcm_otp_is_boot_secure(void)
{
    int rval;
    unsigned int bcmBtrmEn, cusBtrmEn, cusMtkid;

    rval = bcm_otp_get_row(OTP_BRCM_BTRM_BOOT_ENABLE_ROW, &bcmBtrmEn);
    rval |= bcm_otp_get_row( OTP_CUST_BTRM_BOOT_ENABLE_ROW, &cusBtrmEn);
    rval |= bcm_otp_get_row( OTP_CUST_MFG_MRKTID_ROW, &cusMtkid);
    
    if ( !rval && (bcmBtrmEn & OTP_BRCM_BTRM_BOOT_ENABLE_MASK) &&
          (cusBtrmEn & OTP_CUST_BTRM_BOOT_ENABLE_MASK) && 
          (cusMtkid & OTP_CUST_MFG_MRKTID_MASK) )
    {
        rval = 1;
    }
    else
        rval = 0;

    return rval;
}

#if defined (CONFIG_BCM96848) || defined(_BCM96848_)
int bcm_otp_get_revId(void)
{
    unsigned int row;
    int rval = bcm_otp_get_row(OTP_BRCM_CHIPID_ROW, &row);

    if (!rval)
        rval = (row & OTP_BRCM_CHIP_REVID_MASK) >> OTP_BRCM_CHIP_REVID_SHIFT;

    return rval;
}

int bcm_otp_get_max_clksel(void)
{
    unsigned int row;
    int rval = bcm_otp_get_row(OTP_BRCM_FEATURE_DISABLE_ROW, &row);

    if (!rval)
        rval = (row & OTP_BRCM_MAX_CLOCK_FREQ_MASK) >> OTP_BRCM_MAX_CLOCK_FREQ_SHIFT;

    return rval;
}
#endif

