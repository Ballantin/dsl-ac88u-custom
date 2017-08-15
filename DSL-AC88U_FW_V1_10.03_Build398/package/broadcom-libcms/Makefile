#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-libcms
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-libcms
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary CMS (configuration management system) library
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +broadcom-libethswctl +libpthread
endef

define Package/broadcom-libcms/description
 This package contains the proprietary cms library on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libcms_core.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libcms_qdm.so   $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/public/libcms_util.so  $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/public/libcms_boardctl.so  $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/public/libcms_msg.so    $(PKG_BUILD_DIR)


	$(CP) $(TOPDIR)/extern/target-install/lib/public/libbcm_crc.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/public/libbcm_flashutil.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libatmctl.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/librdpactl.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libtmctl.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libvlanctl.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libxdslctl.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-libcms/install
	$(INSTALL_DIR) $(1)/lib
	#$(INSTALL_DIR) $(1)/etc/cms_entity_info.d
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/libcms_cli.so  $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libcms_core.so $(1)/lib/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/libcms_dal.so  $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libcms_qdm.so  $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libcms_util.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libcms_boardctl.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libcms_msg.so  $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libbcm_crc.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libbcm_flashutil.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libatmctl.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/librdpactl.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libtmctl.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libvlanctl.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libxdslctl.so $(1)/lib/

	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/eid_bcm_voip.txt     $(1)/etc/cms_entity_info.d/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/eid_bcm_base.txt     $(1)/etc/cms_entity_info.d/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/eid_bcm_kthreads.txt $(1)/etc/cms_entity_info.d/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/symbol_table.txt     $(1)/etc/cms_entity_info.d/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/eid_bcm_mgmt.txt     $(1)/etc/cms_entity_info.d/
endef

$(eval $(call BuildPackage,broadcom-libcms))
