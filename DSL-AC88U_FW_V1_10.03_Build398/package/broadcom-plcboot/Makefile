#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-plcboot
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-plcboot
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary plcboot
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +broadcom-libcms +libpwrctl 
endef

define Package/broadcom-plcboot/description
 This package contains the proprietary plcboot application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/plcboot/bin/plcboot $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/fw_a0.bin $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/fw.bin $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/paramconfig_a0.bin $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/paramconfig_a0.csv $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/paramconfig.bin $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/paramconfig.csv $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/ucode_a0.bin $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/plcboot/etc/plcboot/ucode.bin $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-plcboot/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/etc/plcboot
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/plcboot $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fw_a0.bin $(1)/etc/plcboot/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fw.bin $(1)/etc/plcboot/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paramconfig_a0.bin $(1)/etc/plcboot/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paramconfig_a0.csv $(1)/etc/plcboot/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paramconfig.bin $(1)/etc/plcboot/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paramconfig.csv $(1)/etc/plcboot/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ucode_a0.bin $(1)/etc/plcboot/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ucode.bin $(1)/etc/plcboot/
endef

$(eval $(call BuildPackage,broadcom-plcboot))
