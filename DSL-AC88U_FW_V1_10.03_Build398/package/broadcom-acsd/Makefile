#
# Copyright (C) 2011 David Tang <david_tang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-acsd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-acsd
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary WPS utility
  MAINTAINER:=David Tang <david_tang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm47xx||@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared
endef

define Package/broadcom-acsd/description
 This package contains the proprietary acsd application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	if [ -d $(TOPDIR)/extern/target-install/acsd ]; then \
		$(CP) $(TOPDIR)/extern/target-install/acsd/usr/sbin/acsd $(PKG_BUILD_DIR); \
		$(CP) $(TOPDIR)/extern/target-install/acsd/usr/sbin/acs_cli $(PKG_BUILD_DIR);\
	else \
		$(CP) $(TOPDIR)/extern/target-install/bin/acsd $(PKG_BUILD_DIR); \
		$(CP) $(TOPDIR)/extern/target-install/bin/acs_cli $(PKG_BUILD_DIR); \
	fi
endef

define Build/InstallDev
endef

define Package/broadcom-acsd/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/acsd $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/acs_cli $(1)/bin/
endef

$(eval $(call BuildPackage,broadcom-acsd))
