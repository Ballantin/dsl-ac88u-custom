include $(TOPDIR)/rules.mk

PKG_NAME:=qtn-QCSAPI-cmdline
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_SOURCE:=
PKG_SOURCE_URL:=

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Utilities
  SECTION:=utils
  CATEGORY:=Quantenna
  TITLE:=The Quantenna Configuration and Status APIs (Command Line Ver)
  URL:=
  DEPENDS:=
endef

define Package/$(PKG_NAME)/description
  The Quantenna Configuration and Status APIs offer the application to access WIFI module.
endef


define Build/Prepare
	@echo "++++++++++PKG_BUILD_DIR = $(PKG_BUILD_DIR) ++++++++++"
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
	
endef

define Build/Compile
$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include -I$(PKG_BUILD_DIR) -I$(LINUX_HEADERS_DIR)/include -Wp,-dD -save-temps -v -DPCIE_RPC_TYPE=RPC_TYPE_CALL_QCSAPI_PCIE"\
		LDFLAGS="$(TARGET_LDFLAGS)"	
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/call_qcsapi_sockrpc $(1)/usr/bin/	
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
