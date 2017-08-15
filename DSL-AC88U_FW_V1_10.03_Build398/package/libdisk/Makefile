# 
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# libdisk is copied from ausu rt project.

include $(TOPDIR)/rules.mk

PKG_NAME:=libdisk
PKG_VERSION:=0.9.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Utilities
  SECTION:=utils    
  CATEGORY:=Arcadyan
  TITLE:=An utility for gathering attached USB device info.xxx
  DEPENDS:=+libarcgpl +asus-apps
endef

#  DEPENDS:=+librt +arcadyan-utility +lib-arc-com +lib-arc-cfg +arc-middle-mod-ccfgsal +arc-middle-mod-common-net +arc-middle-mod-qcsapi

#TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include \

#TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

	
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	if [ -d ./src ] ; then \
		$(CP) ./src/* $(PKG_BUILD_DIR)/. ; \
	fi
endef

#define Build/Compile
#	$(MAKE) -C $(PKG_BUILD_DIR) \
#		CC="$(TARGET_CC)" \
#		CFLAGS="$(TARGET_CFLAGS)" \
#		LDFLAGS="$(TARGET_LDFLAGS)" \
#		TARGET="$(PKG_NAME)"
#endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/disk_io_tools.h $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/usb_info.h $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/disk_share.h $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/disk_initial.h $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/shared.h $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/shutils.h $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libdisk.so $(1)/usr/lib/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libdisk.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test_share $(1)/usr/sbin
	ln -s test_share $(1)/usr/sbin/test_of_var_files
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
