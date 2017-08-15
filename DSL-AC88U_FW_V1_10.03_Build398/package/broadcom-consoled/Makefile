#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-consoled
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-consoled
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary consoled
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared +broadcom-brcmssp_util +broadcom-libcms +broadcom-libnanoxml +broadcom-libpwrctl +broadcom-smd
endef

define Package/broadcom-consoled/description
 This package contains the proprietary consoled application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/bin
	mkdir -p $(PKG_BUILD_DIR)/lib
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/consoled/bin/consoled $(PKG_BUILD_DIR)/bin/
endef
	#$(CP) $(TOPDIR)/extern/target-install/consoled/bin/* $(PKG_BUILD_DIR)/bin/
	#$(CP) $(TOPDIR)/extern/target-install/consoled/lib/* $(PKG_BUILD_DIR)/lib/

define Build/InstallDev
endef

define Package/broadcom-consoled/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/bin/* $(1)/bin/
endef
	#$(INSTALL_DIR) $(1)/lib
	#$(CP) $(PKG_BUILD_DIR)/lib/* $(1)/lib/

$(eval $(call BuildPackage,broadcom-consoled))
