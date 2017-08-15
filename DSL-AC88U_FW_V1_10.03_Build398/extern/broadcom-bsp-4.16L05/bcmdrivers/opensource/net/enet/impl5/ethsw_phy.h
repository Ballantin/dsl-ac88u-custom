/*
 Copyright 2004-2012 Broadcom Corp. All Rights Reserved.

<:label-BRCM:2012:GPL/GPL:standard

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
#ifndef ETHSW_PHY_H
#define ETHSW_PHY_H

#define ETHSW_PHY_GET_PHYID(port) enet_sw_port_to_phyid(0, port)
#define BCMSW_PHY_GET_PHYID(unit, port) enet_sw_port_to_phyid(unit, port)

void ethsw_phy_rw_reg(int phy_id, int reg, uint16 *data, int ext_bit, int rd);
#define ethsw_phy_read_reg(phy_id, reg, data, ext_bit) ethsw_phy_rw_reg(phy_id, reg, data, ext_bit, 1)
#define ethsw_phy_write_reg(phy_id, reg, data, ext_bit) ethsw_phy_rw_reg(phy_id, reg, data, ext_bit, 0)
#define ethsw_phy_rreg(phy_id, reg, data_ptr) ethsw_phy_read_reg(phy_id, reg, (data_ptr), IsExtPhyId(phy_id))
#define ethsw_phy_wreg(phy_id, reg, data_ptr) ethsw_phy_write_reg(phy_id, reg, (data_ptr), IsExtPhyId(phy_id))

/* Broadcom Expanded PHY Register R/W driver */
int ethsw_serdes_get_speed(int phyId);
PHY_STAT ethsw_phyid_stat(int phyId);
PHY_STAT ethsw_phy_stat(int unit, int port, int cb_port);
#define SUPPORT_NO_RX_LOS /* By default, we support NO LOS module */

#if defined(SWITCH_REG_SINGLE_SERDES_CNTRL)
#define SERDES_PHY_BASE_ID 6
#define SERDES_PHY_ID SERDES_PHY_BASE_ID
void ethsw_config_serdes_sgmii(int phyId);
void ethsw_config_serdes_1kx(int phyId);
void ethsw_config_serdes_100fx(int phyId);
void ethsw_init_serdes(void);
void ethsw_serdes_set_speed(int phyId, int speed);
enum SERDES_POWER_LEVEL {SERDES_POWER_DOWN, SERDES_POWER_STANDBY, SERDES_POWER_ON, SERDES_POWER_DOWN_FORCE};
void ethsw_powerup_serdes(int powerLevel, int phyId);
#define ETHSW_POWERUP_SERDES(phyId) ethsw_powerup_serdes(SERDES_POWER_ON, phyId)
#define ETHSW_POWERSTANDBY_SERDES(phyId) ethsw_powerup_serdes(SERDES_POWER_STANDBY, phyId)
#define ETHSW_POWERDOWN_SERDES(phyId) ethsw_powerup_serdes(SERDES_POWER_DOWN, phyId)
#define ETHSW_POWERDOWN_FORCE_OFF_SERDES(phyId) ethsw_powerup_serdes(SERDES_POWER_DOWN_FORCE, phyId)
void ethsw_serdes_power_mode_set(int phy_id, int mode);
void ethsw_serdes_power_mode_get(int phy_id, int *mode);
void ethsw_sfp_restore_from_power_saving(int phyId);
void ethsw_sfp_init(int phy_id);
#else
#define config_serdes() {}
#endif

void ethsw_phy_exp_rw_reg_flag(int phy_id, int reg, uint16 *data, int ext, int write);
#define ethsw_phy_exp_rreg(phy_id, reg, data_ptr) ethsw_phy_exp_rw_reg_flag(phy_id, reg, data_ptr, IsExtPhyId(phy_id), 0)
#define ethsw_phy_exp_wreg(phy_id, reg, data_ptr) ethsw_phy_exp_rw_reg_flag(phy_id, reg, data_ptr, IsExtPhyId(phy_id), 1)
#define ethsw_phy_exp_rreg_flag(phy_id, reg, data_ptr, flag) ethsw_phy_exp_rw_reg_flag(phy_id, reg, data_ptr, flag, 0)
#define ethsw_phy_exp_wreg_flag(phy_id, reg, data_ptr, flag) ethsw_phy_exp_rw_reg_flag(phy_id, reg, data_ptr, flag, 1)

#define ethsw_ephy_shadow_read(phy_id, bannk, reg, data) ethsw_ephy_shadow_rw(phy_id, bannk, reg, data, 0)
#define ethsw_ephy_shadow_write(phy_id, bannk, reg, data) ethsw_ephy_shadow_rw(phy_id, bannk, reg, data, 1)

void ethsw_ephy_shadow_rw(int phy_id, int bank, uint16 reg, uint16 *data, int write);
void _ethsw_ephy_shadow_rw(int phy_id, int bank, uint16 reg, uint16 *data, int write); /* Do not use */
void ethsw_ephy_write_bank1_reg(int phy_id, uint16 reg, uint16 val);
void ethsw_ephy_write_bank2_reg(int phy_id, uint16 reg, uint16 val);
void ethsw_ephy_write_bank3_reg(int phy_id, uint16 reg, uint16 val);

#endif /*ETHSW_PHY_H*/

