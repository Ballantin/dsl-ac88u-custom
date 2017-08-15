#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-ieee1905
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-ieee1905
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary ieee1905
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +broadcom-libcms +broadcom-libethswctl +broadcom-libhomeplugctl +broadcom-libnanoxml +broadcom-libpwrctl +broadcom-libfcctl
endef

define Package/broadcom-ieee1905/description
 This package contains the proprietary ieee1905 application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/ieee1905/bin/ieee1905     $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/ieee1905/bin/i5ctl        $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/ieee1905/bin/ncap         $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/ieee1905/etc/init.d/ieee1905.sh $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-ieee1905/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/rc3.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ieee1905 $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/i5ctl $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ncap $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ieee1905.sh $(1)/etc/init.d/
	ln -sf ../init.d/ieee1905.sh $(1)/etc/rc3.d/S70ieee1905
	ln -sf i5ctl $(1)/bin/i5
endef

$(eval $(call BuildPackage,broadcom-ieee1905))
