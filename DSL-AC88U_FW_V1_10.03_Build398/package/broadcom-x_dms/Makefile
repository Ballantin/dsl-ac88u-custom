#
# Copyright (C) Arcadyan 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-x_dms
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-x_dms
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary x_dms
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +libpthread
endef

define Package/broadcom-x_dms/description
 This package contains the proprietary x_dms application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)/html_root/css
	mkdir -p $(PKG_BUILD_DIR)/html_root/js
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/bcmmserver                   $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libb_playback_ip.so  $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libbmedia.so         $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libbdlna.so          $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libbdlna-dms.so      $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libbdlna-dms-aal.so  $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/public/libssp.so             $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/public/libsqlite3.so.0.8.6   $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/libjpeg.so.9                 $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/webs/html_root/icon*             $(PKG_BUILD_DIR)/html_root
	$(CP) $(TOPDIR)/extern/target-install/webs/html_root/index.html        $(PKG_BUILD_DIR)/html_root
	$(CP) $(TOPDIR)/extern/target-install/webs/html_root/css/*             $(PKG_BUILD_DIR)/html_root/css
	$(CP) $(TOPDIR)/extern/target-install/webs/html_root/js/*              $(PKG_BUILD_DIR)/html_root/js
endef

define Build/InstallDev
endef

define Package/broadcom-x_dms/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_DIR) $(1)/www/html_root
	$(INSTALL_DIR) $(1)/www/html_root/js
	$(INSTALL_DIR) $(1)/www/html_root/css
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bcmmserver            $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libb_playback_ip.so   $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libbmedia.so          $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libbdlna.so           $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libbdlna-dms.so       $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libbdlna-dms-aal.so   $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libsqlite3.so.0.8.6   $(1)/lib/libsqlite3.so.0
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libjpeg.so.9          $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libssp.so             $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/html_root/icon*       $(1)/www/html_root
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/html_root/index.html  $(1)/www/html_root
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/html_root/css/*       $(1)/www/html_root/css
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/html_root/js/*        $(1)/www/html_root/js
endef

$(eval $(call BuildPackage,broadcom-x_dms))
