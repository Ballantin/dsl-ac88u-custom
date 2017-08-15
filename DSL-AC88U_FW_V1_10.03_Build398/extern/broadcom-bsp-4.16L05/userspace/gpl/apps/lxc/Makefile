# Makefile to build LXC

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

all dynamic install:

include $(BUILD_DIR)/make.common

APP = lxc-1.0.6

ifneq ($(strip $(BUILD_LXC)),)

all dynamic install: conditional_build

clean:
	@if [ -e $(APP)/Makefile ]; then \
		cd $(APP); $(MAKE) clean ; \
	fi
	@rm -f $(INSTALL_DIR)/etc/init.d/lxc.sh
	@rm -f $(INSTALL_DIR)/etc/rc3.d/S99lxc
	@rm -f $(INSTALL_DIR)/run

distclean: clean
	rm -rf $(APP)

check_untar_patch_configure: sanity_check
	if [ ! -e $(APP)/Makefile.in ]; then \
		echo "Untarring source and overrides..." ; \
		(tar xfz $(APP).tar.gz 2> /dev/null || true) ; \
		echo "Applying patches to $(APP)" ; \
		patch -p1 -b -N -s -d$(APP) < $(APP).patch ; \
		cd $(APP); \
		./autogen.sh ; \
		./configure --host=${TOOLCHAIN_PREFIX} --prefix= --enable-doc=no --enable-api-docs=no --enable-examples=no --enable-bash=no ; \
	fi

conditional_build: check_untar_patch_configure
	@echo "Making $(APP)"
	@cd $(APP); \
	make; \
	make install DESTDIR=$(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
	rm -f $(INSTALL_DIR)/run
	ln -s /var/run $(INSTALL_DIR)/run
	install -m 0755 lxc.sh $(INSTALL_DIR)/etc/init.d
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f S99lxc; ln -s ../init.d/lxc.sh S99lxc)

bcm_dorel_distclean: distclean

else

all dynamic install clean distclean:
	@echo "LXC not configured. Skipping."

endif
