#
# Copyright (C) Arcadyan 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-bdmf_shell
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-bdmf_shell
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary bdmf_shell
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +libpthread
endef

define Package/broadcom-bdmf_shell/description
 This package contains the proprietary bdmf_shell application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/bdmf_shell   $(PKG_BUILD_DIR)
	#$(CP) $(TOPDIR)/extern/target-install/bin/bs           $(PKG_BUILD_DIR)
	#$(CP) $(TOPDIR)/extern/target-install/bin/runner       $(PKG_BUILD_DIR)
	#$(CP) $(TOPDIR)/extern/target-install/etc/rdpa_init.sh $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-bdmf_shell/install
	$(INSTALL_DIR) $(1)/bin
	#$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bdmf_shell   $(1)/bin/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/bs           $(1)/bin/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/runner       $(1)/bin/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/rdpa_init.sh $(1)/etc/
endef

$(eval $(call BuildPackage,broadcom-bdmf_shell))
