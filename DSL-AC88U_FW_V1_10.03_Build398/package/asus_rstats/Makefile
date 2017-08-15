include $(TOPDIR)/rules.mk

PKG_NAME:=asus-rstats
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
	TITLE:=A bandwidth monitoring tool.
	DEPENDS:=+libarcgpl +asus-apps
endef

define Package/$(PKG_NAME)/description
	Support Traffic Monitor for daily and last 24 hours record.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	grep "qtn" $(TOPDIR)/.config > $(PKG_BUILD_DIR)/.config
endef

define Build/Configure
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
endef
	
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rstats $(1)/sbin
	$(INSTALL_DIR) $(1)/usr/bin
	ln -s /sbin/rstats $(1)/usr/bin/rstats 
endef

define Build/$(PKG_NAME)/clean
	@- [ -e $(PKG_BUILD_DIR)/src/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/src clean
	$(call Build/Clean/Default)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
