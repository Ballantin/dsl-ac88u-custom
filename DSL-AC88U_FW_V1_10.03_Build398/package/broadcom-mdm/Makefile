#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-mdm
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-mdm
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary mdm
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared
endef

define Package/broadcom-mdm/description
 This package contains the proprietary mdm application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/mdm/lib/private/libmdm.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-mdm/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libmdm.so $(1)/lib/
endef

$(eval $(call BuildPackage,broadcom-mdm))
