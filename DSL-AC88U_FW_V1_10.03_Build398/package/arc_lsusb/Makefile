# 
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

###
### ;;;; remember to change VID ;;;;;;

include $(TOPDIR)/rules.mk

PKG_NAME:=arc_lsusb
PKG_VERSION:=0.9.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SUBMENU:=Utilities
	SECTION:=utils    
	CATEGORY:=Arcadyan
  TITLE:=An utility for gathering attached USB device info.
  DEPENDS:=+libusb-1.0 +libarcgpl
endef

TARGET_CFLAGS += \
	-I$(STAGING_DIR)/usr/include \

TARGET_LDFLAGS += \
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib

	
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default)
	if [ -d ./src ] ; then \
		$(CP) ./src/* $(PKG_BUILD_DIR)/. ; \
	fi
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		TARGET="$(PKG_NAME)" \
		VID="0x1d6b"
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		TARGET=resetUsb
endef

define Build/InstallDev
endef

define Package/$(PKG_NAME)/install
	mkdir -p $(1)/usr/bin
	$(CP)	$(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin
	$(INSTALL_BIN)	$(PKG_BUILD_DIR)/resetUsb $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
