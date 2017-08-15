/*
 <:copyright-BRCM:2011:DUAL/GPL:standard
 
    Copyright (c) 2011 Broadcom 
    All Rights Reserved
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation (the "GPL").
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 
 A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
 
 :>
*/
#define _BCMENET_LOCAL_
#include <board.h>
#include "bcm_OS_Deps.h"
#include "bcmmii.h"
#include "bcmsw.h"
#include "boardparms.h"
#include "bcmenet.h"
#include "bcmswaccess.h"
#include "bcm_map_part.h"
#include "ethsw_phy.h"

extern struct semaphore bcm_ethlock_switch_config;
extern uint8_t port_in_loopback_mode[TOTAL_SWITCH_PORTS];

#if defined(SWITCH_REG_SINGLE_SERDES_CNTRL)
#include "pmc/pmc_drv.h"

static u16 serdesSet1GFiber [] =
{
    0x8000, 0x0c2f,
    0x8182, 0x4000,     /* This and next lines are for yield rate improvement */
    0x8186, 0x003c,
    0x8300, 0x015d,     /* Force Auto Detect */
    0x8301, 0x7,
    0x0,    0x1140,
    0x8000, 0x2c2f
};

static u16 serdesSet100MFiber [] =
{
    0x8000, 0x0c2f,
    0x8182, 0x4000,     /* This and next lines are for yield rate improvement */
    0x8186, 0x003c,
    0x8300, 0xd,
    0x8400, 0x14b,
    0x8402, 0x880,
    0x8000, 0x2c2f,
};

static u16 serdesSetSGMII [] =
{
    0x8000, 0x0c2f,
    0x8182, 0x4000,     /* This and next lines are for yield rate improvement */
    0x8186, 0x003c,
    0x8300, 0x0100,
    0x0,    0x1140,
    0x8000, 0x2c2f
};

static void config_serdes(int phyId, u16 seq[], int seqSize)
{
    int i;

    seqSize /= sizeof(seq[0]);
    for(i=0; i<seqSize; i+=2)
    {
        ethsw_phy_exp_wreg(phyId, seq[i], &seq[i+1]);
    }
}

static void ethsw_phy_autong_restart(int phyId)
{
    u16 val16;
    ethsw_phy_rreg(phyId, MII_CONTROL, &val16);
    val16 |= MII_CONTROL_RESTART_AUTONEG;
    ethsw_phy_wreg(phyId, MII_CONTROL, &val16);
}

void ethsw_phy_reset(int phyId)
{
    u16 v16;

    /* Reset PHY to clear status first */
    v16= MII_CONTROL_RESET;
    ethsw_phy_exp_wreg(phyId, MII_CONTROL, &v16);
    for(ethsw_phy_exp_rreg(phyId, MII_CONTROL, &v16); v16 & MII_CONTROL_RESET;
            ethsw_phy_exp_rreg(phyId, MII_CONTROL, &v16));
}

void ethsw_config_serdes_1kx(int phyId)
{
    ethsw_phy_reset(phyId);
    config_serdes(phyId, serdesSet1GFiber, sizeof(serdesSet1GFiber));
    ethsw_phy_autong_restart(phyId);
    msleep(50);
}

void ethsw_config_serdes_100fx(int phyId)
{
    ethsw_phy_reset(phyId);
    config_serdes(phyId, serdesSet100MFiber, sizeof(serdesSet100MFiber));
    msleep(80);
}

void ethsw_config_serdes_sgmii(int phyId)
{
    config_serdes(phyId, serdesSetSGMII, sizeof(serdesSetSGMII));
    ethsw_phy_autong_restart(phyId);
    msleep(50);
}


static void sgmiiResCal(int phyId)
{
    int val;
    u16 v16;

    v16 = RX_AFE_CTRL2_DIV4 | RX_AFE_CTRL2_DIV10;
    ethsw_phy_exp_wreg(phyId, RX_AFE_CTRL2, &v16);

    if(GetRCalSetting(RCAL_1UM_VERT, &val) != kPMC_NO_ERROR)
    {
        printk("AVS is not turned on, leave SGMII termination resistor values as current default\n");
        ethsw_phy_exp_rreg(phyId, PLL_AFE_CTRL1, &v16);
        printk("    PLL_PON: 0x%04x; ", (v16 & PLL_AFE_PLL_PON_MASK) >> PLL_AFE_PLL_PON_SHIFT);
        ethsw_phy_exp_rreg(phyId, TX_AFE_CTRL2, &v16);
        printk("TX_PON: 0x%04x; ", (v16 & TX_AFE_TX_PON_MASK) >> TX_AFE_TX_PON_SHIFT);
        ethsw_phy_exp_rreg(phyId, RX_AFE_CTRL0, &v16);
        printk("RX_PON: 0x%04x\n", (v16 & RX_AFE_RX_PON_MASK) >> RX_AFE_RX_PON_SHIFT);
        return;
    }

    val &= 0xf;
    printk("Setting SGMII Calibration value to 0x%x\n", val);

    ethsw_phy_exp_rreg(phyId, PLL_AFE_CTRL1, &v16);
    v16 = (v16 & (~PLL_AFE_PLL_PON_MASK)) | (val << PLL_AFE_PLL_PON_SHIFT);
    ethsw_phy_exp_wreg(phyId, PLL_AFE_CTRL1, &v16);

    ethsw_phy_exp_rreg(phyId, TX_AFE_CTRL2, &v16);
    v16 = (v16 & (~TX_AFE_TX_PON_MASK)) | (val << TX_AFE_TX_PON_SHIFT);
    ethsw_phy_exp_wreg(phyId, TX_AFE_CTRL2, &v16);

    ethsw_phy_exp_rreg(phyId, RX_AFE_CTRL0, &v16);
    v16 = (v16 & (~RX_AFE_RX_PON_MASK)) | (val << RX_AFE_RX_PON_SHIFT);
    ethsw_phy_exp_wreg(phyId, RX_AFE_CTRL0, &v16);
}

void ethsw_powerup_serdes(int powerLevel, int phyId)
{
    static int curPwrLvl=SERDES_POWER_DOWN;
    u32 val32;
    u16 val16;

    if (powerLevel == curPwrLvl)
        return;

    val32 = *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL;
    switch(powerLevel)
    {
        case SERDES_POWER_ON:
            val32 |= SWITCH_REG_SERDES_RESETPLL|SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET;
            val32 &= ~(SWITCH_REG_SERDES_IQQD|SWITCH_REG_SERDES_PWRDWN);
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
            msleep(1);
            val32 &= ~(SWITCH_REG_SERDES_RESETPLL|SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET);
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;

            /* Do dummy MDIO read to work around ASIC problem */
            ethsw_phy_rreg(phyId, 0, &val16);
            break;
        case SERDES_POWER_STANDBY:
            if (val32 & SWITCH_REG_SERDES_IQQD) {
                val32 |= SWITCH_REG_SERDES_PWRDWN;
                val32 &= ~SWITCH_REG_SERDES_IQQD;
                *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
                msleep(1);
            }
            // note lack of break here
        case SERDES_POWER_DOWN:
            val32 |= SWITCH_REG_SERDES_PWRDWN|SWITCH_REG_SERDES_RESETPLL|
                SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET;
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
            break;
        case SERDES_POWER_DOWN_FORCE:
            val32 |= SWITCH_REG_SERDES_PWRDWN|SWITCH_REG_SERDES_RESETPLL|
                SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET| 
                SWITCH_REG_SERDES_IQQD;
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
            break;            
        default:
            printk("Wrong power level request to Serdes module\n");
            return;
    }
    curPwrLvl = powerLevel;
}

void ethsw_init_serdes(void)
{
    ETHERNET_MAC_INFO *info = EnetGetEthernetMacInfo();
    int muxExtPort, phyId;
    u16 val16;

    for (muxExtPort = 0; muxExtPort < BCMENET_CROSSBAR_MAX_EXT_PORTS; muxExtPort++) {
        phyId = info->sw.crossbar[muxExtPort].phy_id;
        if((phyId & MAC_IFACE) != MAC_IF_SERDES)
        {
            continue;
        }

        ETHSW_POWERUP_SERDES(phyId);
        ethsw_sfp_restore_from_power_saving(phyId);

        /* Enable GPIO 36 Input */
        if (BpGetSgmiiGpios(&val16) != BP_SUCCESS)
        {
            printk("Error: GPIO pin for Serdes not defined\n");
            return;
        }

        if(val16 != 28 && val16 != 36)
        {
            printk("Error: GPIO Pin %d for Serdes is not supported, correct boardparams.c definition.\n", val16);
            return;
        }

        if (val16 == 28)
        {
            MISC->miscSGMIIFiberDetect = 0;
            printk("GPIO Pin 28 is assigned to Serdes Fiber Signal Detection.\n");
        }
        else
        {
            MISC->miscSGMIIFiberDetect = MISC_SGMII_FIBER_GPIO36;
            printk("GPIO 36 is assigned to Serdes Fiber Signal Detection.\n");
        }

        /* Enable AN mode */
#if 0
        ethsw_phy_rreg(phyId, MII_CONTROL, &val16);
        val16 |= MII_CONTROL_AN_ENABLE|MII_CONTROL_RESTART_AUTONEG;
        ethsw_phy_wreg(phyId, MII_CONTROL, &val16);
#endif

        /* read back for testing */
        ethsw_phy_rreg(phyId, MII_CONTROL, &val16);

        /* Calibrate SGMII termination resistors */
        sgmiiResCal(phyId);

        ethsw_sfp_init(phyId);
    }
}

#if defined(CONFIG_I2C)
static void ethsw_conf_copper_sfp(int speed);
#endif

enum
{
    SFP_NO_MODULE,
    SFP_FIBER,
    SFP_COPPER,
};

static PHY_STAT ethsw_serdes_stat(int phyId);
static int sfp_module_type = SFP_NO_MODULE;
static int serdes_speed = BMCR_ANENABLE;
static int16 sfpDetectionGpio = -1;
static int ethsw_sfp_link_status_changed;

enum {SFP_MODULE_OUT, SFP_MODULE_IN, SFP_LINK_UP};
static int sfp_status = SFP_MODULE_OUT;
static int serdes_power_mode = SERDES_BASIC_POWER_SAVING;

int ethsw_serdes_get_speed(int phyId)
{
    return serdes_speed;
}

/* Quick check for link up/down only without checking speed */
static inline int ethsw_phyid_sfp_link_only(int phyId)
{
    uint16 v16;

    ethsw_phy_rreg(phyId, MII_STATUS, &v16);
    return (v16 & MII_STATUS_LINK) != 0;
}

static PHY_STAT ethsw_phyid_sfp_stat(int phyId)
{
    PHY_STAT ps;
    uint16 v16;

    memset(&ps, 0, sizeof(ps));

    ethsw_phy_rreg(phyId, MII_STATUS, &v16);
    ps.lnk = (v16 & MII_STATUS_LINK) != 0;

    if(ps.lnk)
    {
        ethsw_phy_exp_rreg(phyId, MIIEX_DIGITAL_STATUS_1000X, &v16);
        ps.spd1000 = (v16 & MIIEX_SPEED) == MIIEX_SPD1000;
        ps.spd100 = (v16 & MIIEX_SPEED) == MIIEX_SPD100;
        ps.spd10 = (v16 & MIIEX_SPEED) == MIIEX_SPD10;
        ps.fdx = (v16 & MIIEX_DUPLEX) > 0;
    }

    return ps;
}

static inline void ethsw_serdes_try_1kx_from_100(int phyId)
{
    /* If link up at 100fx, the other end might just got out from POWER_SAVING mode
       now, try 1kx again to give both sides another chance to link at higher speed */
    ethsw_config_serdes_1kx(phyId);
    msleep(1500);    /* Sleep more than polling interval to get bigger chance to link at 1G */
    if (ethsw_phyid_sfp_link_only(phyId)) return;

    /* No luck, set it back to 100fx */
    ethsw_config_serdes_100fx(phyId);
}

static inline void ethsw_serdes_speed_detect(int phyId)
{
    if (sfp_module_type != SFP_FIBER || serdes_speed != BMCR_ANENABLE) return;

    /* First see if we can link at 1kx */
    ethsw_config_serdes_1kx(phyId);
    if (ethsw_phyid_sfp_link_only(phyId)) return;

    /* 2nd. see if we can link at 100Mbps */
    ethsw_config_serdes_100fx(phyId);
    if (ethsw_phyid_sfp_link_only(phyId))
    {
        ethsw_serdes_try_1kx_from_100(phyId);
        return;
    }
    else
    {
        /* Leave the speed at 1G */
        ethsw_config_serdes_1kx(phyId);
    }
}

void ethsw_sfp_restore_from_power_saving(int phyId)
{
    if (sfp_module_type == SFP_NO_MODULE)
        return;

#if defined(CONFIG_I2C)
    if(sfp_module_type == SFP_COPPER)
    {
        /* Configure Serdes into SGMII mode */
        ethsw_config_serdes_sgmii(phyId);
    }
    else
#endif
    {
        switch(serdes_speed)
        {
            case BMCR_ANENABLE:
                ethsw_serdes_speed_detect(phyId);
                break;
            case BMCR_SPEED1000|BMCR_FULLDPLX:
                ethsw_config_serdes_1kx(phyId);
                break;
            case BMCR_SPEED100|BMCR_FULLDPLX:
                ethsw_config_serdes_100fx(phyId);
                break;
            default:
                break;
        }
    }
}

void ethsw_serdes_set_speed(int phyId, int speed)
{
    speed &= ~BMCR_ANRESTART;
    if (speed != BMCR_ANENABLE &&
            speed != (BMCR_SPEED1000| BMCR_FULLDPLX) &&
            speed != (BMCR_SPEED100| BMCR_FULLDPLX))
    {
        printk("Not Supported Speed %x attempted to set to Serdes\n", speed);
        return;
    }

    if (speed == serdes_speed) return;;
    serdes_speed = speed;

    switch (sfp_module_type)
    {
        case SFP_NO_MODULE:
            break;

        case SFP_FIBER:
            ETHSW_POWERDOWN_SERDES(phyId);
            ETHSW_POWERUP_SERDES(phyId);
            ethsw_sfp_restore_from_power_saving(phyId);
            ethsw_serdes_stat(phyId);
            break;

#if defined(CONFIG_I2C)
        case SFP_COPPER:
            ethsw_conf_copper_sfp(speed);
            break;
#endif

    }
    ethsw_sfp_link_status_changed = 1;
    return;
}

#if defined(CONFIG_I2C)
static int sfp_i2c_module_detect(int phyId)
{
    u32 val32;

    /* If I2C read operation succeeds, I2C module is connected
       and which means it is a copper SFP module */
    if (sfp_i2c_phy_read(0, &val32))
    {
        sfp_module_type = SFP_COPPER;
        ethsw_conf_copper_sfp(serdes_speed);
        ethsw_config_serdes_sgmii(phyId);

        printk("Copper SFP Module Plugged in\n");
        return 1;
    }
    return 0;
}

static void ethsw_conf_copper_sfp(int speed)
{
    uint16 v16;

    sfp_i2c_phy_write(MII_CONTROL, MII_CONTROL_RESET);     /* Software reset */

    /* Configure SFP PHY into SGMII mode */
    sfp_i2c_phy_write(0x1b, 0x9084);    /* Enable SGMII mode */
    sfp_i2c_phy_write(MII_CONTROL, MII_CONTROL_RESET);     /* Software reset */

    if (speed & BMCR_ANENABLE)
    {
        sfp_i2c_phy_write(0x9, 0x0f00);     /* Advertise 1kBase-T Full/Half-Duplex */
        sfp_i2c_phy_write(0x0, 0x8140);     /* Software reset */
        sfp_i2c_phy_write(0x4, 0x0de1);     /* Adverstize 100/10Base-T Full/Half-Duplex */
        sfp_i2c_phy_write(0x0, 0x9140);     /* Software reset */

        return;
    }

    /* Fixed speed configuration */
    v16 = MII_CONTROL_RESET;
    if (speed & BMCR_SPEED1000)
    {
        v16 |= MII_CONTROL_SPEED_1000;
    }
    else if (speed & BMCR_SPEED100)
    {
        v16 |= MII_CONTROL_SPEED_100;
    }

    if (speed & BMCR_FULLDPLX)
    {
        v16 |= MII_CONTROL_DUPLEX_MODE;
    }
    sfp_i2c_phy_write(MII_CONTROL, v16);
}
#endif

void ethsw_serdes_power_mode_set(int phy_id, int mode)
{
    if (serdes_power_mode == mode)
        return;

    serdes_power_mode = mode;

    if (sfp_status == SFP_LINK_UP)
        return;

    if(mode == SERDES_NO_POWER_SAVING)
    {
        ETHSW_POWERUP_SERDES(phy_id);
        ethsw_sfp_restore_from_power_saving(phy_id);
    }
    else if (mode == SERDES_FORCE_OFF)
        ETHSW_POWERDOWN_FORCE_OFF_SERDES(phy_id);
    else
        ETHSW_POWERDOWN_SERDES(phy_id);
}

void ethsw_serdes_power_mode_get(int phy_id, int *mode)
{
    *mode = serdes_power_mode;
}

static int ethsw_sfp_module_detected(void)
{
    if (sfpDetectionGpio != -1)
    {
        return kerSysGetGpioValue(sfpDetectionGpio) == 0;
    }
    else
    {
        return ((*(u32*)SWITCH_SINGLE_SERDES_STAT) & SWITCH_REG_SSER_RXSIG_DET) > 0;
    }
}

/*
   Module detection is not going through SGMII,
   so it can be down even under SGMII power down.
 */
static int ethsw_sfp_module_detect(int phyId)
{
#if defined(CONFIG_I2C)
#define I2CDetectDelay 8
#define I2CInitDetectDelay 8
    static int i2cDetectDelay = 0;
    static int i2cInitDetectDelay = 0;
    if (i2cInitDetectDelay++ < I2CInitDetectDelay) return 0;
#endif

    if ( ethsw_sfp_module_detected() == 0)
    {
        if(sfp_module_type != SFP_NO_MODULE)
        {
            sfp_module_type = SFP_NO_MODULE;
            printk("SFP module unplugged\n");
        }
        return 0;
    }

    if (sfp_module_type == SFP_NO_MODULE)
    {
#if defined(CONFIG_I2C)
        if (!sfp_i2c_module_detect(phyId))
#endif
        {
            /* Configure Serdes into 1000Base-X mode */
            sfp_module_type = SFP_FIBER;

#if defined(CONFIG_I2C)
            i2cDetectDelay = 0;
            printk("Fibre SFP Module Plugged in\n");
#else
            printk("SFP Module Plugged in\n");
#endif
        }
    }
#if defined(CONFIG_I2C)
    else    /* MODULE is plug in */
    {
        /* Work around for some I2C long initialization, do more I2C after Fiber type detected */
        if (sfp_module_type == SFP_FIBER && i2cDetectDelay < I2CDetectDelay)
        {
            if (sfp_i2c_module_detect(phyId))
            {
                ETHSW_POWERDOWN_SERDES(phyId);  /* Power down and up Serdes to reset Serdes Status */
                ETHSW_POWERUP_SERDES(phyId);
                i2cDetectDelay = I2CDetectDelay;
            }
            else
            {
                i2cDetectDelay++;
            }
        }
    }
#endif
    return 1;
}

void ethsw_sfp_init(int phyId)
{
#if defined(CONFIG_I2C)
    volatile u32 val32; 
    u32 v32;
    sfp_i2c_phy_read(0, &v32);    /* Dummy read to trigger I2C hardware prepared */
    val32 = v32;
    msleep(1);
#endif
    /* Call the function to init state machine without leaving
       SFP on without control during initialization */
    ethsw_serdes_stat(phyId);

    if (BpGetSfpDetectGpio(&sfpDetectionGpio) == BP_SUCCESS)
    {
        kerSysSetGpioDirInput(sfpDetectionGpio);
        printk("GPIO Pin %d is configured as SPF MOD_ABS for module insertion detection\n", sfpDetectionGpio);
    }
    else
    {
        sfpDetectionGpio = -1;
        printk("Energy detection is used as SFP module insertion detection\n");
    }
}

static PHY_STAT ethsw_serdes_stat(int phyId)
{
    PHY_STAT ps;

    memset(&ps, 0, sizeof(ps));
    if (serdes_power_mode == SERDES_FORCE_OFF) {
        //should already be powered down -- just exit
        goto sfp_end;
    }

    if (serdes_power_mode > SERDES_NO_POWER_SAVING && sfp_status < SFP_LINK_UP) 
        ETHSW_POWERSTANDBY_SERDES(phyId);

    switch (sfp_status)
    {
        case SFP_MODULE_OUT:
sfp_module_out:
            if(sfp_status == SFP_MODULE_OUT && ethsw_sfp_module_detect(phyId))
                goto sfp_module_in;

            sfp_status = SFP_MODULE_OUT;
            goto sfp_end;

        case SFP_MODULE_IN:
sfp_module_in:
            if(sfp_status >= SFP_MODULE_IN && !ethsw_sfp_module_detect(phyId))
            {
                sfp_status = SFP_MODULE_IN;
                goto sfp_module_out;
            }

            if(sfp_status <= SFP_MODULE_IN)
            {
                if(serdes_power_mode == SERDES_BASIC_POWER_SAVING)
                {
                    ETHSW_POWERUP_SERDES(phyId);
                    ethsw_sfp_restore_from_power_saving(phyId);
                }
                else if (serdes_power_mode == SERDES_NO_POWER_SAVING)
                {
                    ethsw_serdes_speed_detect(phyId);
                }

                ps = ethsw_phyid_sfp_stat(phyId);
                if(ps.lnk)
                {
                    sfp_status = SFP_MODULE_IN;
                    goto sfp_link_up;
                }
            }
            sfp_status = SFP_MODULE_IN;
            goto sfp_end;

        case SFP_LINK_UP:
sfp_link_up:
            if(sfp_status == SFP_LINK_UP)
            {
                if(!ethsw_sfp_module_detect(phyId)) goto sfp_module_out;
                ps = ethsw_phyid_sfp_stat(phyId);
                if(!ps.lnk)
                {
                    goto sfp_module_in;
                }
            }
            sfp_status = SFP_LINK_UP;
            goto sfp_end;
    }

sfp_end:
    if( serdes_power_mode > SERDES_NO_POWER_SAVING && sfp_status != SFP_LINK_UP)
        ETHSW_POWERDOWN_SERDES(phyId);
    return ps;
}

#endif

PHY_STAT ethsw_phyid_stat(int phyId)
{
    PHY_STAT ps;
    uint16 v16, misc;
    uint16 ctrl;
    uint16 mii_esr = 0;
    uint16 mii_stat = 0, mii_adv = 0, mii_lpa = 0;
    uint16 mii_gb_ctrl = 0, mii_gb_stat = 0;

    memset(&ps, 0, sizeof(ps));
    if (!IsPhyConnected(phyId))
    {
        ps.lnk = 1;
        ps.fdx = 1;
        if (IsMII(phyId))
            ps.spd100 = 1;
        else
            ps.spd1000 = 1;
        return ps;
    }

    down(&bcm_ethlock_switch_config);

#if defined(SWITCH_REG_SINGLE_SERDES_CNTRL)
    if((phyId & MAC_IFACE) == MAC_IF_SERDES)
    {
        /* Create a link down event to notify MAC,
           so that MAC layer don't need to remember previous speed etc. */
        if (ethsw_sfp_link_status_changed)
        {
            ethsw_sfp_link_status_changed = 0;
            goto end;
        }

        ps = ethsw_serdes_stat(phyId);
        goto end;
    }
#endif  /* SWITCH_REG_SINGLE_SERDES_CNTRL */

    ethsw_phy_rreg(phyId, MII_INTERRUPT, &v16);
    ethsw_phy_rreg(phyId, MII_ASR, &v16);

    if (!MII_ASR_LINK(v16)) {
        goto end;
    }

    ps.lnk = 1;

    ethsw_phy_rreg(phyId, MII_BMCR, &ctrl);

    if (!MII_ASR_DONE(v16)) {
        ethsw_phy_rreg(phyId, MII_BMCR, &ctrl);
        if (ctrl & BMCR_ANENABLE) {
            up(&bcm_ethlock_switch_config);
            return ps;
        }
        // auto-negotiation disabled
        ps.fdx = (ctrl & BMCR_FULLDPLX) ? 1 : 0;
        if((ctrl & BMCR_SPEED100) && !(ctrl & BMCR_SPEED1000))
            ps.spd100 = 1;
        else if(!(ctrl & BMCR_SPEED100) && (ctrl & BMCR_SPEED1000))
            ps.spd1000 = 1;

        goto end;
    }

    if (ps.spd1000 == 1) { // only GE phy support ethernet@wirespeed
        // check if ethernet@wirespeed is enabled, reg 0x18, shodow 0b'111, bit4
        misc = 0x7007;
        ethsw_phy_wreg(phyId, 0x18, &misc);
        ethsw_phy_rreg(phyId, 0x18, &misc);
        if(misc & 0x0010) {
            // get link speed from ASR if ethernet@wirespeed is enabled
            if (MII_ASR_1000(v16) && MII_ASR_FDX(v16)) {
                ps.spd1000 = 1;
                ps.fdx = 1;
            } else if (MII_ASR_1000(v16) && !MII_ASR_FDX(v16)) {
                ps.spd1000 = 1;
                ps.fdx = 0;            
            } else if (MII_ASR_100(v16) && MII_ASR_FDX(v16)) {
                ps.spd100 = 1;
                ps.fdx = 1;
            } else if (MII_ASR_100(v16) && !MII_ASR_FDX(v16)) {
                ps.spd100 = 1;
                ps.fdx = 0;
            } else if (MII_ASR_10(v16) && MII_ASR_FDX(v16)) {
                ps.spd10 = 1;
                ps.fdx = 1;
            } else if (MII_ASR_10(v16) && !MII_ASR_FDX(v16)) {
                ps.spd10 = 1;
                ps.fdx = 0;
            }
            goto end;
        }
    }

    //Auto neg enabled (this end) cases
    ethsw_phy_rreg(phyId, MII_ADVERTISE, &mii_adv);
    ethsw_phy_rreg(phyId, MII_LPA, &mii_lpa);
    ethsw_phy_rreg(phyId, MII_BMSR, &mii_stat);

    // read 1000mb Phy  registers if supported
    if (mii_stat & BMSR_ESTATEN) {

        ethsw_phy_rreg(phyId, MII_ESTATUS, &mii_esr);
        if (mii_esr & (1 << 15 | 1 << 14 |
                    ESTATUS_1000_TFULL | ESTATUS_1000_THALF))
            ethsw_phy_rreg(phyId, MII_CTRL1000, &mii_gb_ctrl);
        ethsw_phy_rreg(phyId, MII_STAT1000, &mii_gb_stat);
    }

    mii_adv &= mii_lpa;

    if ((mii_gb_ctrl & ADVERTISE_1000FULL) &&  // 1000mb Adv
            (mii_gb_stat & LPA_1000FULL))
    {
        ps.spd1000 = 1;
        ps.fdx = 1;
    } else if ((mii_gb_ctrl & ADVERTISE_1000HALF) &&
            (mii_gb_stat & LPA_1000HALF))
    {
        ps.spd1000 = 1;
        ps.fdx = 0;
    } else if (mii_adv & ADVERTISE_100FULL) {  // 100mb adv
        ps.spd100 = 1;
        ps.fdx = 1;
    } else if (mii_adv & ADVERTISE_100BASE4) {
        ps.spd100 = 1;
        ps.fdx = 0;
    } else if (mii_adv & ADVERTISE_100HALF) {
        ps.spd100 = 1;
        ps.fdx = 0;
    } else if (mii_adv & ADVERTISE_10FULL) {
        ps.fdx = 1;
    }
end:
    up(&bcm_ethlock_switch_config);
    return ps;
}

#if !defined(CONFIG_BCM96838) && !defined(CONFIG_BCM96848)
PHY_STAT ethsw_phy_stat(int unit, int port, int cb_port)
{
    PHY_STAT phys = {0};
    int      phyId;

    if ((unit == 0) && port_in_loopback_mode[port]) {
        return phys;
    }

    if ( cb_port == BP_CROSSBAR_NOT_DEFINED )
    {
        phyId = enet_sw_port_to_phyid(unit, port);
    }
    else
    {
        phyId = enet_cb_port_to_phyid(unit, cb_port);
    }
    
    if ( !IsPhyConnected(phyId) )
    {
        phys = enet_get_ext_phy_stat(unit, port, cb_port);
        /* is phys valid */
        if ( (phys.spd10 | phys.spd100 | phys.spd1000) > 0 )
        {
            return phys;
        }
    }
    return ethsw_phyid_stat(phyId);
}
#endif

/* Broadcom MII Extended Register Access Driver */
/* Do not invoke this directly, use ethsw_ephy_shadow_rw instead */
void _ethsw_ephy_shadow_rw(int phy_id, int bank, uint16 reg, uint16 *data, int write)
{
    uint16 v16;
    
    switch (bank)
    {
        case 1: 
            v16 = 0x8b;
            ethsw_phy_wreg(phy_id, BRCM_MIIEXT_BANK, &v16);
            break;
        case 2:
        case 3:
            v16 = 0xf;
            ethsw_phy_wreg(phy_id, BRCM_MIIEXT_BANK, &v16);
            break;
    }

    if (bank == 3)
    {
        if (write)
        {
            ethsw_phy_wreg(phy_id, 0xe, &reg);
            ethsw_phy_wreg(phy_id, 0xf, data);
        }
        else
        {
            ethsw_phy_wreg(phy_id, 0xe, &reg);
            ethsw_phy_rreg(phy_id, 0xf, data);
        }
    }
    else
    {
        if (write)
        {
            ethsw_phy_wreg(phy_id, reg, data);
        }
        else 
        {
            ethsw_phy_rreg(phy_id, reg, data);
        }
    }
    v16 = 0xb;
    ethsw_phy_wreg(phy_id, BRCM_MIIEXT_BANK, &v16);

    return;
}

void ethsw_ephy_write_bank1_reg(int phy_id, uint16 reg, uint16 val)
{
    ethsw_ephy_shadow_write(phy_id, 1, reg, &val);
}

void ethsw_ephy_write_bank2_reg(int phy_id, uint16 reg, uint16 val)
{
    ethsw_ephy_shadow_write(phy_id, 2, reg, &val);
}

void ethsw_ephy_write_bank3_reg(int phy_id, uint16 reg, uint16 val)
{
    ethsw_ephy_shadow_write(phy_id, 3, reg, &val);
}

DEFINE_MUTEX(bcm_phy_exp_mutex);
void ethsw_phy_exp_rw_reg_flag(int phy_id, int reg, uint16 *data, int ext, int write)
{
    u16 bank, v16, offset;

    /* If the register falls within standard MII address */
    if ((reg & BRCM_MIIEXT_ADDR_RANGE) == 0)
    {
        if(!write)
            ethsw_phy_read_reg(phy_id, reg, data, ext);
        else
            ethsw_phy_write_reg(phy_id, reg, data, ext);
        return;
    }

    bank = reg & BRCM_MIIEXT_BANK_MASK;
    offset = (reg & BRCM_MIIEXT_OFF_MASK) + BRCM_MIIEXT_OFFSET;
    mutex_lock(&bcm_phy_exp_mutex);
    /* Set Bank Address */
    ethsw_phy_wreg(phy_id, BRCM_MIIEXT_BANK, &bank);

    /* Set offset address */
    if (!write)
        ethsw_phy_read_reg(phy_id, offset, data, ext);
    else
        ethsw_phy_write_reg(phy_id, offset, data, ext);

    /* Set Bank back to default for standard access */
    if(bank != BRCM_MIIEXT_DEF_BANK || offset == BRCM_MIIEXT_OFFSET)
    {
        v16 = BRCM_MIIEXT_DEF_BANK;
        ethsw_phy_wreg(phy_id, BRCM_MIIEXT_BANK, &v16);
    }
    mutex_unlock(&bcm_phy_exp_mutex);
}
MODULE_LICENSE("GPL");

