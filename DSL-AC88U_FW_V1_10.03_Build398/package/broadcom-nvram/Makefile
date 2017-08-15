#
# Copyright (C) 2009-2010 Jo-Philipp Wich <xm@subsignal.org>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=nvram
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/nvram
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Userspace port of the Broadcom NVRAM manipulation tool
  MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
  DEPENDS:=@TARGET_brcm47xx||@TARGET_ar71xx||@TARGET_brcm63xx +broadcom-libcms +broadcom-libwlcsm +bcm_crc +bcm_flashutil
endef

define Package/nvram/description
 This package contains the proprietary utility to manipulate NVRAM on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/nvram $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/bin/nvramUpdate $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/libnvram.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libnvram.so $(1)/usr/lib/
endef

define Package/nvram/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nvram $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nvramUpdate $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libnvram.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,nvram))
