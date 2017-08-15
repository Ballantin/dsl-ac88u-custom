# 
# Copyright (C) 2011 Arcadyan
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=asus-apps
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

MKDIR:=mkdir -p
RM:=rm -rf

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SUBMENU:=Utilities
	SECTION:=utils
	CATEGORY:=Arcadyan
	TITLE:=Asus apps install layers.
	URL:=http://www.arcadyan.com/
	DEPENDS:=libarcgpl
endef

define Package/$(PKG_NAME)/description
	Middle layer for nvram.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/arc-release/usr/lib/libasusnvram.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/arc-release/usr/include/asus_nvram.h $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

#### Target Rules ###############################
define Build/Compile
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libasusnvram.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/asus_nvram.h $(1)/usr/include
endef	
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libasusnvram.so $(1)/usr/lib
endef

define Build/$(PKG_NAME)/clean
	@- [ -e $(PKG_BUILD_DIR)/src/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/src clean
	$(call Build/Clean/Default)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
