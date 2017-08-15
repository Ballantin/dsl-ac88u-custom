#
# Copyright (C) 2009 Arcadyan Corporation
# All Rights Reserved.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rp-l2tp
PKG_RELEASE:=1
PKG_VERSION:=0.5-rtn


PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=rp-l2tp opensource 
  DEPENDS:=
endef

define Package/$(PKG_NAME)/description
	rp-l2tp opensource
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)"
endef


define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/l2tpd $(1)/usr/sbin

	$(INSTALL_DIR) $(1)/usr/lib/l2tp/plugins
	$(CP) $(PKG_BUILD_DIR)/handlers/l2tp-control $(1)/usr/sbin 
	$(CP) $(PKG_BUILD_DIR)/handlers/*.so* $(1)/usr/lib/l2tp/plugins

endef

define Build/$(PKG_NAME)/clean
	@- [ -e $(PKG_BUILD_DIR)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR) clean
	$(call Build/Clean/Default)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
