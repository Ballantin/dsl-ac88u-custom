#
# Copyright (C) Arcadyan 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-speedsvc
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-speedsvc
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary speedsvc
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +libpthread +broadcom-libcms +broadcom-libpwrctl +broadcom-libnanoxml +librt
endef

define Package/broadcom-speedsvc/description
 This package contains the proprietary speedsvc application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	make -C $(TOPDIR)/extern/broadcom-bsp/userspace/private/apps/speedsvc install
	$(CP) $(TOPDIR)/extern/broadcom-bsp/targets/WG9517AWAC23-ST/fs.install/bin/speedsvc $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-speedsvc/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/speedsvc            $(1)/bin/
endef

$(eval $(call BuildPackage,broadcom-speedsvc))
