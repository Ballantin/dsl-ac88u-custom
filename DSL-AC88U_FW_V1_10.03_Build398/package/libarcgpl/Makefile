# 
# Copyright (C) 2011 Arcadyan
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libarcgpl
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DEPENDS:=

MKDIR:=mkdir -p
RM:=rm -rf

include $(INCLUDE_DIR)/package.mk

ifeq ($(__target_inc),)
  include $(INCLUDE_DIR)/target.mk
endif

define Package/libarcgpl
  SUBMENU:=Libs
  SECTION:=libs
  CATEGORY:=Arcadyan
  TITLE:=Arcadyan middle layer for non-arcadyan packages.
  URL:=http://www.arcadyan.com/
  DEPENDS:=
endef

define Package/libarcgpl/description
 Middle layer for non-arcadyan packages from Arcadyan Corporation for embedded Linux.
endef

define Package/$(PKG_NAME)/config
	config $(PKG_NAME)_TEST
		depends PACKAGE_$(PKG_NAME)
		bool "Build test API"
		default n
endef

define Build/Configure
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/arc-release/usr/lib/libarcgpl.so* $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Build/InstallDev
endef

define Package/libarcgpl/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libarcgpl.so* $(1)/usr/lib
endef

define Build/libarcgpl/clean
        @- [ -e $(PKG_BUILD_DIR)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR) clean
        $(call Build/Clean/Default)
endef

$(eval $(call BuildPackage,libarcgpl))
