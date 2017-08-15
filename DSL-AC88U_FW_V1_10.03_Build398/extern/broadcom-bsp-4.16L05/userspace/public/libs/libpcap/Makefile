
libpcap: conditional_build 

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

export LINUX_VER_STR TOOLCHAIN_PREFIX


ifneq ($(strip $(BUILD_LIBPCAP)),)
conditional_build: all
else
conditional_build:
	@echo "skipping libpcap (not configured)"
endif


libpcap/configure: libpcap.tar.gz
	mkdir -p libpcap
	cd libpcap ; tar --strip-components=1 -xzf ../libpcap.tar.gz
	touch -c libpcap/configure
	@echo "libpcap is untarred"

check_config: libpcap/Makefile

libpcap/Makefile: libpcap/configure
	cd libpcap ; ac_cv_linux_vers=$(LINUX_VER_STR)  ./configure --host=$(TOOLCHAIN_PREFIX) --with-pcap=linux --prefix=$(BCM_FSBUILD_DIR)/public/

all: check_config
	mkdir -p $(INSTALL_DIR)/lib/public/
	cd libpcap ; make
	cd libpcap ; make install
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libpcap.so* $(INSTALL_DIR)/lib/public/

clean:
	rm -f $(INSTALL_DIR)/lib/public/libpcap.so*
	-[ -e libpcap/Makefile ] && cd libpcap && make clean uninstall distclean
	rm -f libpcap/net

bcm_dorel_distclean: distclean

distclean: clean

shell:
	bash -i

