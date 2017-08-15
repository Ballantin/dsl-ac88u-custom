
libcurl: conditional_build 

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

export LINUX_VER_STR TOOLCHAIN_PREFIX


ifneq ($(strip $(BUILD_LIBCURL)),)
conditional_build: all
else
conditional_build:
	@echo "skipping libcurl (not configured)"
endif


libcurl/configure: curl-7.44.0.tar.gz
	mkdir -p libcurl
	cd libcurl ; tar --strip-components=1 -xzf ../curl-7.44.0.tar.gz
	touch -c libcurl/configure
	@echo "libcurl is untarred"

check_config: libcurl/Makefile

libcurl/Makefile: libcurl/configure
	cd libcurl ; ./configure --prefix=$(BCM_FSBUILD_DIR)/public/ --host=$(TOOLCHAIN_PREFIX)

all: check_config
	mkdir -p $(INSTALL_DIR)/lib/public/
	cd libcurl ; make
	cd libcurl ; make install
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libcurl*.so* $(INSTALL_DIR)/lib/public/

clean:
	-rm -f $(INSTALL_DIR)/lib/public/libcurl*.so*
	-rm -rf libcurl

bcm_dorel_distclean: distclean

distclean:
	-rm -rf libcurl

shell:
	bash -i

