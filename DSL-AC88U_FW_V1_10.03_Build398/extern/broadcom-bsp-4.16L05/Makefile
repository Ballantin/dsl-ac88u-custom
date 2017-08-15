#***********************************************************************
#
#  Copyright (c) 2004  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

# Top-level Makefile

###########################################
#
# This is the most important target: make all
# This is the first target in the Makefile, so it is also the default target.
# All the other targets are later in this file.
#
############################################

all: mkenv prebuild_checks all_postcheck1

all_postcheck1: profile_saved_check sanity_check rdplink \
     create_install pinmuxcheck kernelbuild modbuild kernelbuildlite \
     userspace gdbserver vodsl dectd hosttools nvram_3k_kernelbuild buildimage

mkenv:
	@echo "############### parallel build environment start ################";
	@echo  "brcm_max_jobs: "$(BRCM_MAX_JOBS)
	@echo  "actual_max_jobs: "$(ACTUAL_MAX_JOBS)
	@echo -n "hostname: "; hostname
	@echo -n "uname: "; uname -a
	@which nproc &> /dev/null && (echo -n "processors: "; nproc) || echo "nproc not available"
	@which vmstat &> /dev/null && vmstat -SM || echo "vmstat is not available"
	@which lscpu &> /dev/null && lscpu || echo "lscpu is not available"
	@echo "################ parallel build environment end ##################"

############################################################################
#
# A lot of the stuff in the original Makefile has been moved over
# to make.common.
#
############################################################################
BUILD_DIR = $(shell pwd)
include $(BUILD_DIR)/make.common

############################################################################
#
# Make info for voice has been moved over to make.voice
#
############################################################################
ifneq ($(strip $(BUILD_VODSL)),)
include $(BUILD_DIR)/make.voice
# ARCADYAN
else
export XCHANGE_DIR=$(BUILD_DIR)/xChange
# ARCADYAN
endif

############################################################################
#
# Make info for RDPA simulator and auto-generated code
#
############################################################################
-include $(BUILD_DIR)/make.rdpa_sim
ifneq ($(wildcard $(BUILD_DIR)/make.rdplink),)
-include $(BUILD_DIR)/make.rdplink
else

rdplink:

rdplink_clean:

endif

###########################################################################
#
# dsl, kernel defines
#
############################################################################
ifeq ($(strip $(BUILD_NOR_KERNEL_LZ4)),y)
KERNEL_COMPRESSION=lz4
else
KERNEL_COMPRESSION=lzma
endif 

ifeq ($(strip $(BRCM_KERNEL_DEBUG)),y) 
KERNEL_DEBUG=1
endif

ifeq ($(strip $(BRCM_KERNEL_KALLSYMS)),y) 
KERNEL_KALLSYMS=1
endif

# ARCADYAN
KCFLAGS += -DARCADYAN
export KCFLAGS
# ARCADYAN

#Set up ADSL standard
export ADSL=$(BRCM_ADSL_STANDARD)

#Set up ADSL_PHY_MODE  {file | obj}
export ADSL_PHY_MODE=file

#Set up ADSL_SELF_TEST
export ADSL_SELF_TEST=$(BRCM_ADSL_SELF_TEST)

#Set up ADSL_PLN_TEST
export ADSL_PLN_TEST=$(BUILD_TR69_XBRCM)

#WL command
ifneq ($(strip $(WL)),)
build_nop:=$(shell cd $(BUILD_DIR)/userspace/private/apps/wlan; make clean; cd $(BUILD_DIR))
export WL

SVN_IMPL:=$(patsubst IMPL%,%,$(WL))
export SVN_IMPL
#SVNTAG command
ifneq ($(strip $(SVNTAG)),)
WL_BASE := $(BUILD_DIR)/bcmdrivers/broadcom/net/wl
SVNTAG_DIR := $(shell if [ -d $(WL_BASE)/$(SVNTAG)/src ]; then echo 1; else echo 0; fi)
ifeq ($(strip $(SVNTAG_DIR)),1)
$(shell ln -sf $(WL_BASE)/$(SVNTAG)/src $(WL_BASE)/impl$(SVN_IMPL))
else
$(error There is no directory $(WL_BASE)/$(SVNTAG)/src)
endif
endif

endif

ifneq ($(strip $(BRCM_DRIVER_WIRELESS_USBAP)),)
    WLBUS ?= "usbpci"
endif
#default WLBUS for wlan pci driver
WLBUS ?="pci"
export WLBUS                                                                              

#IMAGE_VERSION:=$(BRCM_VERSION)$(BRCM_RELEASE)$(shell echo $(BRCM_EXTRAVERSION) | sed -e "s/\(0\)\([1-9]\)/\2/")$(shell echo $(PROFILE) | sed -e "s/^[0-9]*//")$(shell date '+%j%H%M')

ifneq ($(IMAGE_VERSION_STRING),)
    IMAGE_VERSION:=$(IMAGE_VERSION_STRING)
else
    IMAGE_VERSION:=$(BRCM_VERSION)$(BRCM_RELEASE)$(shell echo $(BRCM_EXTRAVERSION) | sed -e "s/\(0\)\([1-9]\)/\2/")$(shell echo $(PROFILE) | sed -e "s/^[0-9]*//")$(shell date '+%j%H%M')
endif




############################################################################
#
# When there is a directory name with the same name as a Make target,
# make gets confused.  PHONY tells Make to ignore the directory when
# trying to make these targets.
#
############################################################################
.PHONY: userspace unittests data-model hostTools kernellinks



############################################################################
#
# Other Targets. The default target is "all", defined at the top of the file.
#
############################################################################

#
# create a bcm_relversion.h which has our release version number, e.g.
# 4 10 02.  This allows device drivers which support multiple releases
# with a single driver image to test for version numbers.
#
BCM_SWVERSION_FILE := $(KERNEL_DIR)/include/linux/bcm_swversion.h
BCM_VERSION_LEVEL := $(strip $(BRCM_VERSION))
BCM_RELEASE_LEVEL := $(strip $(BRCM_RELEASE))
BCM_RELEASE_LEVEL := $(shell echo $(BCM_RELEASE_LEVEL) | sed -e 's/^0*//')
BCM_PATCH_LEVEL := $(strip $(shell echo $(BRCM_EXTRAVERSION) | cut -c1-2))
BCM_PATCH_LEVEL := $(shell echo $(BCM_PATCH_LEVEL) | sed -e 's/^0*//')

$(BCM_SWVERSION_FILE): $(BUILD_DIR)/version.make
ifneq ($(RELEASE_BUILD),)
	@if egrep -q '^BRCM_(VER|RE|EXTRA).*=.*[^a-zA-Z0-9]' version.make ; then \
		echo "error ... illegal character detected within version in version.mak" ; \
		exit 1 ; \
	fi
endif
	@echo "creating bcm release version header file"
	@echo "/* this file is automatically generated from top level Makefile */" > $(BCM_SWVERSION_FILE)
	@echo "#ifndef __BCM_SWVERSION_H__" >> $(BCM_SWVERSION_FILE)
	@echo "#define __BCM_SWVERSION_H__" >> $(BCM_SWVERSION_FILE)
	@echo "#define BCM_REL_VERSION $(BCM_VERSION_LEVEL)" >> $(BCM_SWVERSION_FILE)
	@echo "#define BCM_REL_RELEASE $(BCM_RELEASE_LEVEL)" >> $(BCM_SWVERSION_FILE)
	@echo "#define BCM_REL_PATCH $(BCM_PATCH_LEVEL)" >> $(BCM_SWVERSION_FILE)
	@echo "#define BCM_SW_VERSIONCODE ($(BCM_VERSION_LEVEL)*65536+$(BCM_RELEASE_LEVEL)*256+$(BCM_PATCH_LEVEL))" >> $(BCM_SWVERSION_FILE)
	@echo "#define BCM_SW_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))" >> $(BCM_SWVERSION_FILE)
	@echo "#endif" >> $(BCM_SWVERSION_FILE)

ifneq ($(PROFILE_KERNEL_VER),LINUX_2_6_30_RT)
BCM_KF_TXT_FILE := $(BUILD_DIR)/kernel/BcmKernelFeatures.txt
BCM_KF_KCONFIG_FILE := $(KERNEL_DIR)/Kconfig.bcm_kf
MAKEFNOTES_PL := $(HOSTTOOLS_DIR)/makefpatch/makefnotes.pl

havefeatures := $(wildcard $(BCM_KF_TXT_FILE))

ifneq ($(strip $(havefeatures)),)
$(BCM_KF_KCONFIG_FILE) : $(BCM_KF_TXT_FILE)
	perl $(MAKEFNOTES_PL) -kconfig -fl $(BCM_KF_TXT_FILE) > $(BCM_KF_KCONFIG_FILE)
endif

kernelbuild: $(BCM_KF_KCONFIG_FILE)

endif


userspace: sanity_check create_install data-model $(BCM_SWVERSION_FILE) kernellinks
	@echo "MAKING USERSPACE"
	$(MAKE) -j $(ACTUAL_MAX_JOBS) -C userspace

#
# Always run Make in the libcreduction directory.  In most non-voice configs,
# mklibs.py will be invoked to analyze user applications
# and libraries to eliminate unused functions thereby reducing image size.
# However, for voice configs, gdb server, oprofile and maybe some other
# special cases, the libcreduction makefile will just copy unstripped
# system libraries to fs.install for inclusion in the image.
#
libcreduction:
	$(MAKE) -C hostTools/libcreduction install



menuconfig:
	@cd $(INC_KERNEL_BASE); \
	$(MAKE) -C $(HOSTTOOLS_DIR)/scripts/lxdialog HOSTCC=gcc
	$(CONFIG_SHELL) $(HOSTTOOLS_DIR)/scripts/Menuconfig $(TARGETS_DIR)/config.in $(PROFILE)


#
# the userspace apps and libs make their own directories before
# they install, so they don't depend on this target to make the
# directory for them anymore.
#
create_install:
		mkdir -p $(PROFILE_DIR)/fs.install/etc
		mkdir -p $(INSTALL_DIR)/bin
		mkdir -p $(INSTALL_DIR)/lib
		mkdir -p $(INSTALL_DIR)/etc/snmp
		mkdir -p $(INSTALL_DIR)/etc/iproute2
		mkdir -p $(INSTALL_DIR)/opt/bin
		mkdir -p $(INSTALL_DIR)/opt/modules
		mkdir -p $(INSTALL_DIR)/opt/scripts


# By default, let make spawn 1 job per core.
# To set max jobs, specify on command line, BRCM_MAX_JOBS=8
# To also specify a max load, BRCM_MAX_JOBS="6 --max-load=3.0"
# To specify max load without max jobs, BRCM_MAX_JOBS=" --max-load=3.5"
ifneq ($(strip $(BRCM_MAX_JOBS)),)
ACTUAL_MAX_JOBS := $(BRCM_MAX_JOBS)
else
NUM_CORES := $(shell grep processor /proc/cpuinfo | wc -l)
ACTUAL_MAX_JOBS := $(NUM_CORES)
endif
# Since tms driver is called with -j1 and will call its sub-make with -j, 
# We want it to use this value. Although the jobserver is disabled for tms,
# at least tms is compiled with no more than this variable value jobs.
export ACTUAL_MAX_JOBS

# ARCADYAN
#BillChan, some Arcadyan kernel module will require to use mach-bcm963xx since from BRCM SDK 4.16L03
kernellinks: $(KERNEL_INCLUDE_LINK) $(KERNEL_MIPS_INCLUDE_LINK) $(KERNEL_ARM_INCLUDE_LINK) ARCADYAN_INCLUDE_LINK
#	ln -sf $(KERNEL_DIR)/arch/arm/mach-bcm963xx/include/mach $(KERNEL_ARM_INCLUDE_LINK)/generated/mach;
# ARCADYAN

$(KERNEL_INCLUDE_LINK):
	ln -s -f $(KERNEL_DIR)/include $(KERNEL_INCLUDE_LINK);

$(KERNEL_MIPS_INCLUDE_LINK):
	ln -s -f $(KERNEL_DIR)/arch/mips/include $(KERNEL_MIPS_INCLUDE_LINK);

$(KERNEL_ARM_INCLUDE_LINK):
	ln -s -f $(KERNEL_DIR)/arch/arm/include $(KERNEL_ARM_INCLUDE_LINK);

ARCADYAN_INCLUDE_LINK:
	[ -d $(KERNEL_ARM_INCLUDE_LINK)/generated ] || mkdir  $(KERNEL_ARM_INCLUDE_LINK)/generated
	ln -sf $(KERNEL_DIR)/arch/arm/mach-bcm963xx/include/mach $(KERNEL_ARM_INCLUDE_LINK)/generated/mach;

define android_kernel_merge_cfg
cd $(KERNEL_DIR); \
ARCH=${ARCH} scripts/kconfig/merge_config.sh -m arch/$(ARCH)/defconfig android/configs/android-base.cfg android/configs/android-recommended.cfg android/configs/android-bcm-recommended.cfg ;
endef

define pre_kernelbuild
	@echo
	@echo -------------------------------------------
	@echo ... starting kernel build at $(KERNEL_DIR)
        @echo KERNEL_CHECK_FILE is $(KERNEL_CHECK_FILE)
        @echo PROFILE_KERNEL_VER is $(PROFILE_KERNEL_VER)
	@cd $(INC_KERNEL_BASE); \
	if [ ! -e $(KERNEL_DIR)/$(KERNEL_CHECK_FILE) ]; then \
	  echo "Untarring original Linux kernel source: $(LINUX_ZIP_FILE)"; \
	  (tar xkfpj $(LINUX_ZIP_FILE) 2> /dev/null || true); \
	fi
	$(GENDEFCONFIG_CMD) $(PROFILE_PATH) ${MAKEFLAGS}
	cd $(KERNEL_DIR); \
	cp -f $(KERNEL_DIR)/arch/$(ARCH)/defconfig $(KERNEL_DIR)/.config;
	$(if $(strip $(BRCM_ANDROID)), $(call android_kernel_merge_cfg))
	cd $(KERNEL_DIR); \
	$(MAKE) oldconfig;
endef

kernelbuild: kernellinks $(BCM_SWVERSION_FILE)
	$(call pre_kernelbuild)
	cd $(KERNEL_DIR); $(MAKE) -j $(ACTUAL_MAX_JOBS)


ifeq ($(strip $(BRCM_CHIP)),63268)
ifneq ($(strip $(BUILD_SECURE_BOOT)),)
export BUILD_NVRAM_3K=n
nvram_3k_kernelbuild: 
	@mv $(KERNEL_DIR)/vmlinux $(KERNEL_DIR)/vmlinux.restore
	cd $(KERNEL_DIR); $(MAKE) nvram_3k -j $(ACTUAL_MAX_JOBS) BUILD_NVRAM_3K=y
nvram_3k_kernelclean:
	@rm -f $(KERNEL_DIR)/vmlinux_secureboot
	@rm -f $(KERNEL_DIR)/vmlinux.restore
else
nvram_3k_kernelbuild: 
	@echo "No 3k nvram build required... "
nvram_3k_kernelclean:
endif
else
nvram_3k_kernelbuild: 
	@echo "No 3k nvram build required... "
nvram_3k_kernelclean:
endif


kernel_config_test:
	@echo
	@echo "Building $(DIR)/config_$(PROFILE)";
	$(call pre_kernelbuild)
	-@mkdir $(DIR) 2> /dev/null || true
	sort $(KERNEL_DIR)/.config | grep -v "^\#.*$$" | grep -v "^[[:space:]]*$$" > $(DIR)/config_$(PROFILE)
	@echo "  ... done building $(DIR)/config_$(PROFILE)";

.PHONY: kernel_config_test

kernel: sanity_check create_install kernelbuild hosttools buildimage

ifeq ($(strip $(BRCM_DRIVER_BCMDSP)),m)
kernelbuildlite:
	@echo "******************** Kernel Build Lite ********************";
	$(BRCMDRIVERS_DIR)/broadcom/char/dspapp/impl1/getDspModSizes.sh  $(BRCMDRIVERS_DIR)/broadcom/char/dspapp/impl1/dspdd.ko $(SHARED_DIR) $(CROSS_COMPILE) $(KERNEL_DEBUG) $(KERNEL_KALLSYMS);
	cd $(KERNEL_DIR); $(MAKE)
	@echo "******************** Kernel Build Lite DONE ********************";
else
kernelbuildlite:
	@echo "******************** Kernel Build Lite ********************"
	@echo "Nothing to do... "
	@echo "******************** Kernel Build Lite DONE ********************"
endif

ifeq ($(strip $(VOXXXLOAD)),1)
modbuild: touch_voice_files
	@echo "******************** Starting modbuild (VOXXXLOAD) ********************";
	cd $(KERNEL_DIR); $(MAKE) modules && $(MAKE) modules_install
	@echo "******************** DONE modbuild (VOXXXLOAD) ********************";
else
modbuild:
	@echo "******************** Starting modbuild ********************";
	cd $(KERNEL_DIR); $(MAKE) -j $(ACTUAL_MAX_JOBS) modules && $(MAKE) modules_install
	@echo "******************** DONE modbuild ********************";
endif

mocamodbuild:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_MOCACFGDRV_PATH) modules 
mocamodclean:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_MOCACFGDRV_PATH) clean

adslmodbuild:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_ADSLDRV_PATH) modules 
adslmodbuildclean:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_ADSLDRV_PATH) clean

spumodbuild:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_SPUDRV_PATH) modules
spumodbuildclean:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_SPUDRV_PATH) clean

pwrmngtmodbuild:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_PWRMNGTDRV_PATH) modules
pwrmngtmodclean:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_PWRMNGTDRV_PATH) clean

enetmodbuild:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_ENETDRV_PATH) modules
enetmodclean:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_ENETDRV_PATH) clean

eponmodbuild:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_EPONDRV_PATH) modules
eponmodclean:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_EPONDRV_PATH) clean

gponmodbuild:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_GPON_PATH) modules
gponmodclean:
	cd $(KERNEL_DIR); $(MAKE) M=$(INC_GPON_PATH) clean

modules: sanity_check create_install modbuild kernelbuildlite hosttools buildimage

adslmodule: adslmodbuild
adslmoduleclean: adslmodbuildclean

spumodule: spumodbuild
spumoduleclean: spumodbuildclean

pwrmngtmodule: pwrmngtmodbuild
pwrmngtmoduleclean: pwrmngtmodclean

data-model:
	$(MAKE) -C data-model

unittests:
	$(MAKE) -C unittests

unittests_run:
	$(MAKE) -C unittests unittests_run

doxygen_build:
	$(MAKE) -C hostTools build_doxygen

doxygen_docs: doxygen_build
	rm -rf $(BUILD_DIR)/docs/doxygen;
	mkdir $(BUILD_DIR)/docs/doxygen;
	cd hostTools/doxygen/bin; ./doxygen

doxygen_clean:
	-$(MAKE) -C hostTools clean_doxygen


# touch_voice_files doesn't clean up voice, just enables incremental build of voice code
touch_voice_files:
	find bcmdrivers/broadcom/char/endpoint/ \( -name '*.o' -o -name '*.a' -o -name '*.lib' -o -name '*.ko' -o -name '*.cmd' -o -name '.*.cmd' -o -name '*.c' -o -name '*.mod' \) -print -exec rm -f "{}" ";"
	rm -rf $(KERNEL_DIR)/.tmp_versions/endpointdd.mod
	rm -rf $(KERNEL_DIR)/arch/mips/defconfig
	rm -rf $(KERNEL_DIR)/include/config/bcm/endpoint/
	find $(KERNEL_DIR)/lib/ -name '*.o' -print -exec rm -f "{}" ";"
	find $(KERNEL_DIR)/lib/ -name '*.lib' -print -exec rm -f "{}" ";"



############################################################################
#
# Build user applications depending on if they are
# specified in the profile.  Most of these BUILD_ checks should eventually get
# moved down to the userspace directory.
#
############################################################################

ifneq ($(strip $(BUILD_VCONFIG)),)
export BUILD_VCONFIG=y
endif


ifneq ($(strip $(BUILD_GDBSERVER)),)
gdbserver:
	install -m 755 $(TOOLCHAIN_TOP)/usr/$(TOOLCHAIN_PREFIX)/target_utils/gdbserver $(INSTALL_DIR)/bin
else
gdbserver:
endif

ifneq ($(strip $(BUILD_ETHWAN)),)
export BUILD_ETHWAN=y
endif

ifneq ($(strip $(BUILD_SNMP)),)

ifneq ($(strip $(BUILD_SNMP_SET)),)
export SNMP_SET=1
else
export SNMP_SET=0
endif

ifneq ($(strip $(BUILD_SNMP_ADSL_MIB)),)
export SNMP_ADSL_MIB=1
else
export SNMP_ADSL_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_ATM_MIB)),)
export SNMP_ATM_MIB=1
else
export SNMP_ATM_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_BRIDGE_MIB)),)
export SNMP_BRIDGE_MIB=1
else
export SNMP_BRIDGE_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_AT_MIB)),)
export SNMP_AT_MIB=1
else
export SNMP_AT_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_SYSOR_MIB)),)
export SNMP_SYSOR_MIB=1
else
export SNMP_SYSOR_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_TCP_MIB)),)
export SNMP_TCP_MIB=1
else
export SNMP_TCP_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_UDP_MIB)),)
export SNMP_UDP_MIB=1
else
export SNMP_UDP_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_IP_MIB)),)
export SNMP_IP_MIB=1
else
export SNMP_IP_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_ICMP_MIB)),)
export SNMP_ICMP_MIB=1
else
export SNMP_ICMP_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_SNMP_MIB)),)
export SNMP_SNMP_MIB=1
else
export SNMP_SNMP_MIB=0
endif

ifneq ($(strip $(BUILD_SNMP_ATMFORUM_MIB)),)
export SNMP_ATMFORUM_MIB=1
else
export SNMP_ATMFORUM_MIB=0
endif


ifneq ($(strip $(BUILD_SNMP_CHINA_TELECOM_CPE_MIB)),)
export BUILD_SNMP_CHINA_TELECOM_CPE_MIB=y
endif

ifneq ($(strip $(BUILD_CT_1_39_OPEN)),)
export BUILD_CT_1_39_OPEN=y
endif

ifneq ($(strip $(BUILD_SNMP_CHINA_TELECOM_CPE_MIB_V2)),)
export BUILD_SNMP_CHINA_TELECOM_CPE_MIB_V2=y
endif

ifneq ($(strip $(BUILD_SNMP_BRCM_CPE_MIB)),)
export BUILD_SNMP_BRCM_CPE_MIB=y
endif

ifneq ($(strip $(BUILD_SNMP_UDP)),)
export BUILD_SNMP_UDP=y
endif

ifneq ($(strip $(BUILD_SNMP_EOC)),)
export BUILD_SNMP_EOC=y
endif

ifneq ($(strip $(BUILD_SNMP_AAL5)),)
export BUILD_SNMP_AAL5=y
endif

ifneq ($(strip $(BUILD_SNMP_AUTO)),)
export BUILD_SNMP_AUTO=y
endif

ifneq ($(strip $(BUILD_SNMP_DEBUG)),)
export BUILD_SNMP_DEBUG=y
endif

ifneq ($(strip $(BUILD_SNMP_TRANSPORT_DEBUG)),)
export BUILD_SNMP_TRANSPORT_DEBUG=y
endif

ifneq ($(strip $(BUILD_SNMP_LAYER_DEBUG)),)
export BUILD_SNMP_LAYER_DEBUG=y
endif

endif

ifneq ($(strip $(BUILD_4_LEVEL_QOS)),)
export BUILD_4_LEVEL_QOS=y
endif

# ARCADYAN
#DECT & FXS verification tools 
ifneq ($(strip $(BUILD_VODSL)),)
dspgctl: sanity_check
	$(MAKE) -C $(BUILD_DIR)/userspace/private/apps/dspgctl
else
dspgctl: sanity_check
	@echo "skipping vodsl for dspgctl (not configured)"
endif
# ARCADYAN

# vodsl (voice daemon)
# mwang: for now, I still need to leave vodsl at this top level because
# it depends on so many of the voice defines in this top level Makefile.
ifneq ($(strip $(BUILD_VODSL)),)
vodsl: sanity_check
	$(MAKE) -C $(BUILD_DIR)/userspace/private/apps/vodsl $(BUILD_VODSL)
else
vodsl: sanity_check
	@echo "skipping vodsl (not configured)"
endif

#dectd
ifneq ($(strip $(BUILD_DECT)),)
dectd: sanity_check
	$(MAKE) -C $(BUILD_DIR)/userspace/private/apps/dectd dynamic
else
dectd: sanity_check
	@echo "Skipping dectd application (not configured)"
endif

# Leave it for the future when soap server is decoupled from cfm
ifneq ($(strip $(BUILD_SOAP)),)
ifeq ($(strip $(BUILD_SOAP_VER)),2)
soapserver:
	$(MAKE) -C $(BROADCOM_DIR)/SoapToolkit/SoapServer $(BUILD_SOAP)
else
soap:
	$(MAKE) -C $(BROADCOM_DIR)/soap $(BUILD_SOAP)
endif
else
soap:
endif



ifneq ($(strip $(BUILD_DIAGAPP)),)
diagapp:
	$(MAKE) -C $(BROADCOM_DIR)/diagapp $(BUILD_DIAGAPP)
else
diagapp:
endif



ifneq ($(strip $(BUILD_IPPD)),)
ippd:
	$(MAKE) -C $(BROADCOM_DIR)/ippd $(BUILD_IPPD)
else
ippd:
endif


ifneq ($(strip $(BUILD_PORT_MIRRORING)),)
export BUILD_PORT_MIRRORING=1
else
export BUILD_PORT_MIRRORING=0
endif

ifeq ($(BRCM_USE_SUDO_IFNOT_ROOT),y)
BRCM_BUILD_USR=$(shell whoami)
BRCM_BUILD_USR1=$(shell sudo touch foo;ls -l foo | awk '{print $$3}';sudo rm -rf foo)
else
BRCM_BUILD_USR=root
endif

hosttools:
ifndef NO_HOSTTOOLS_PARALLEL
	$(MAKE) -j $(ACTUAL_MAX_JOBS) -C $(HOSTTOOLS_DIR)
else
	$(MAKE) -C $(HOSTTOOLS_DIR)
endif

hosttools_nandcfe:
	$(MAKE) -C $(HOSTTOOLS_DIR) mkjffs2 build_imageutil build_cmplzma build_secbtutils


############################################################################
#
# IKOS defines
#
############################################################################

CMS_VERSION_FILE=$(BUILD_DIR)/userspace/public/include/version.h

ifeq ($(strip $(BRCM_IKOS)),y)
FS_COMPRESSION=-noD -noI -no-fragments
else
FS_COMPRESSION=
endif

export BRCM_IKOS FS_COMPRESSION



# IKOS Emulator build that does not include the CFE boot loader.
# Edit targets/ikos/ikos and change the chip and board id to desired values.
# Then build: make PROFILE=ikos ikos
ikos:
	@echo -e '#define SOFTWARE_VERSION ""\n#define RELEASE_VERSION ""\n#define PSI_VERSION ""\n' > $(CMS_VERSION_FILE)
	@-mv -f $(FSSRC_DIR)/etc/profile $(FSSRC_DIR)/etc/profile.dontuse >& /dev/null
	@-mv -f $(FSSRC_DIR)/etc/init.d $(FSSRC_DIR)/etc/init.dontuse >& /dev/null
	@-mv -f $(FSSRC_DIR)/etc/inittab $(FSSRC_DIR)/etc/inittab.dontuse >& /dev/null
	@sed -e 's/^::respawn.*sh.*/::respawn:-\/bin\/sh/' $(FSSRC_DIR)/etc/inittab.dontuse > $(FSSRC_DIR)/etc/inittab
	@if [ ! -a $(CFE_FILE) ] ; then echo "no cfe" > $(CFE_FILE); echo "no cfe" > $(CFE_FILE).del; fi
	@-rm $(HOSTTOOLS_DIR)/bcmImageBuilder >& /dev/null
	$(MAKE) PROFILE=$(PROFILE)
	@-rm $(HOSTTOOLS_DIR)/bcmImageBuilder >& /dev/null
	@mv -f $(FSSRC_DIR)/etc/profile.dontuse $(FSSRC_DIR)/etc/profile
	@-mv -f $(FSSRC_DIR)/etc/init.dontuse $(FSSRC_DIR)/etc/init.d >& /dev/null
	@-mv -f $(FSSRC_DIR)/etc/inittab.dontuse $(FSSRC_DIR)/etc/inittab >& /dev/null
	@cd $(PROFILE_DIR); \
	$(OBJCOPY) --output-target=srec vmlinux vmlinux.srec; \
	xxd $(FS_KERNEL_IMAGE_NAME) | grep "^00000..:" | xxd -r > bcmtag.bin; \
	$(OBJCOPY) --output-target=srec --input-target=binary --change-addresses=0xb8010000 bcmtag.bin bcmtag.srec; \
	$(OBJCOPY) --output-target=srec --input-target=binary --change-addresses=0xb8010100 rootfs.img rootfs.srec; \
	rm bcmtag.bin; \
	grep -v "^S7" vmlinux.srec > bcm9$(BRCM_CHIP)_$(PROFILE).srec; \
	grep "^S3" bcmtag.srec >> bcm9$(BRCM_CHIP)_$(PROFILE).srec; \
	grep -v "^S0" rootfs.srec >> bcm9$(BRCM_CHIP)_$(PROFILE).srec
	@if [ ! -a $(CFE_FILE).del ] ; then rm -f $(CFE_FILE) $(CFE_FILE).del; fi
	@echo -e "\nAn image without CFE for the IKOS emulator has been built.  It is named"
	@echo -e "targets/$(PROFILE)/bcm9$(BRCM_CHIP)_$(PROFILE).srec\n"

# IKOS Emulator build that includes the CFE boot loader.
# Both Linux and CFE boot loader toolchains need to be installed.
# Edit targets/ikos/ikos and change the chip and board id to desired values.
# Then build: make PROFILE=ikos ikoscfe
ikoscfe:
	@echo -e '#define SOFTWARE_VERSION ""\n#define RELEASE_VERSION ""\n#define PSI_VERSION ""\n' > $(CMS_VERSION_FILE)
	@-mv -f $(FSSRC_DIR)/etc/profile $(FSSRC_DIR)/etc/profile.dontuse >& /dev/null
	$(MAKE) PROFILE=$(PROFILE)
	@mv -f $(FSSRC_DIR)/etc/profile.dontuse $(FSSRC_DIR)/etc/profile
	$(MAKE) -C $(BL_BUILD_DIR) clean
	$(MAKE) -C $(BL_BUILD_DIR)
	$(MAKE) -C $(BL_BUILD_DIR) ikos_finish
	cd $(PROFILE_DIR); \
	echo -n "** no kernel  **" > kernelfile; \
	$(HOSTTOOLS_DIR)/bcmImageBuilder $(BRCM_ENDIAN_FLAGS) --output $(CFE_FS_KERNEL_IMAGE_NAME) --chip $(BRCM_CHIP) --board $(BRCM_BOARD_ID) --blocksize $(BRCM_FLASHBLK_SIZE) --cfefile $(BL_BUILD_DIR)/cfe$(BRCM_CHIP).bin --rootfsfile rootfs.img --kernelfile kernelfile --include-cfe; \
	$(HOSTTOOLS_DIR)/createimg.pl --set boardid=$(BRCM_BOARD_ID) voiceboardid=$(BRCM_VOICE_BOARD_ID) numbermac=$(BRCM_NUM_MAC_ADDRESSES) macaddr=$(BRCM_BASE_MAC_ADDRESS) tp=$(BRCM_MAIN_TP_NUM) psisize=$(BRCM_PSI_SIZE) --inputfile=$(CFE_FS_KERNEL_IMAGE_NAME) --outputfile=$(FLASH_IMAGE_NAME) --nvramfile $(HOSTTOOLS_DIR)/nvram.h --nvramdefsfile $(HOSTTOOLS_DIR)/nvram_defaults.h --config=$(HOSTTOOLS_DIR)/local_install/conf/$(TOOLCHAIN_PREFIX).conf;\
	$(HOSTTOOLS_DIR)/addvtoken --endian $(ARCH_ENDIAN) $(FLASH_IMAGE_NAME) $(FLASH_IMAGE_NAME).w; \
	$(OBJCOPY) --output-target=srec --input-target=binary --change-addresses=0xb8000000 $(FLASH_IMAGE_NAME).w $(FLASH_IMAGE_NAME).srec; \
	$(OBJCOPY) --output-target=srec vmlinux vmlinux.srec; \
	@rm kernelfile; \
	grep -v "^S7" vmlinux.srec > bcm9$(BRCM_CHIP)_$(PROFILE).srec; \
	grep "^S3" $(BL_BUILD_DIR)/cferam$(BRCM_CHIP).srec >> bcm9$(BRCM_CHIP)_$(PROFILE).srec; \
	grep -v "^S0" $(FLASH_IMAGE_NAME).srec >> bcm9$(BRCM_CHIP)_$(PROFILE).srec; \
	grep -v "^S7" vmlinux.srec > bcm9$(BRCM_CHIP)_$(PROFILE).utram.srec; \
	grep -v "^S0" $(BL_BUILD_DIR)/cferam$(BRCM_CHIP).srec >> bcm9$(BRCM_CHIP)_$(PROFILE).utram.srec;
	@echo -e "\nAn image with CFE for the IKOS emulator has been built.  It is named"
	@echo -e "targets/$(PROFILE)/bcm9$(BRCM_CHIP)_$(PROFILE).srec"
	@echo -e "\nBefore testing with the IKOS emulator, this build can be unit tested"
	@echo -e "with an existing chip and board as follows."
	@echo -e "1. Flash targets/$(PROFILE)/$(FLASH_IMAGE_NAME).w onto an existing board."
	@echo -e "2. Start the EPI EDB debugger.  At the edbice prompt, enter:"
	@echo -e "   edbice> fr m targets/$(PROFILE)/bcm9$(BRCM_CHIP)_$(PROFILE).utram.srec"
	@echo -e "   edbice> r"
	@echo -e "3. Program execution will start at 0xb8000000 (or 0xbfc00000) and,"
	@echo -e "   if successful, will enter the Linux shell.\n"


############################################################################
#
# Generate the credits
#
############################################################################
gen_credits:
	cd $(RELEASE_DIR); \
	if [ -e gen_credits.pl ]; then \
	  perl gen_credits.pl; \
	fi

############################################################################
#
# PinMuxCheck
#
############################################################################
pinmuxcheck:
ifeq ($(wildcard $(HOSTTOOLS_DIR)/PinMuxCheck/Makefile),)
	@echo "No PinMuxCheck needed"
else
	cd $(HOSTTOOLS_DIR); $(MAKE) build_pinmuxcheck;
endif


############################################################################
#
# This is where we build the image
#
############################################################################

execstack_exec=$(shell which execstack)
buildimage: kernelbuild libcreduction gen_credits
ifneq ($(strip $(BRCM_PERMIT_STDCPP)),)
	@if ls $(PROFILE_DIR)/fs.install/lib/libstdc* 2> /dev/null ; then  \
		echo -e "libstdc++ must be replaced with STLPORT";         \
		echo -e "override with BRCM_PERMIT_STDCPP=1 if ok";         \
		false;                                                     \
	fi
endif
ifeq ($(BUILD_DISABLE_EXEC_STACK),y)
ifneq ($(execstack_exec),)
	@echo no need to build execstack $(execstack_exec)
else
	make -C $(HOSTTOOLS_DIR) build_execstack;
endif
endif
	cd $(TARGETS_DIR); ./buildFS;
ifeq ($(strip $(BRCM_RAMDISK_BOOT_EN)),y)
	cd $(TARGETS_DIR); $(HOSTTOOLS_DIR)/fakeroot/fakeroot ./buildFS_RD
endif
	cd $(TARGETS_DIR); $(HOSTTOOLS_DIR)/fakeroot/fakeroot ./buildFS2

	@mkdir -p $(IMAGES_DIR)

ifeq ($(strip $(BRCM_KERNEL_ROOTFS)),all)
# NAND UBI and JFFS2
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_16KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_16KB) --rootfs jffs2_rootfs16kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_16)_jffs2 --fsonly $(FLASH_NAND_FS_IMAGE_NAME_16)_jffs2
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_16KB) --ubifs --bootfs bootfs16kb.img --rootfs ubi_rootfs16kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_16)_ubi --fsonly $(FLASH_NAND_FS_IMAGE_NAME_16)_ubi
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_128KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_128KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB)  --rootfs jffs2_rootfs128kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_128)_jffs2 --fsonly $(FLASH_NAND_FS_IMAGE_NAME_128)_jffs2
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_128KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB)  --ubifs --bootfs bootfs128kb.img --rootfs ubi_rootfs128kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_128)_ubi --fsonly $(FLASH_NAND_FS_IMAGE_NAME_128)_ubi
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_256KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_256KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_256KB)  --rootfs jffs2_rootfs256kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_256)_jffs2 --fsonly $(FLASH_NAND_FS_IMAGE_NAME_256)_jffs2
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_256KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_256KB)  --ubifs --bootfs bootfs256kb.img --rootfs ubi_rootfs256kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_256)_ubi --fsonly $(FLASH_NAND_FS_IMAGE_NAME_256)_ubi
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_512KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_512KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_512KB)  --rootfs jffs2_rootfs512kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_512)_jffs2 --fsonly $(FLASH_NAND_FS_IMAGE_NAME_512)_jffs2
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_512KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_512KB)   --ubifs --bootfs bootfs512kb.img --rootfs ubi_rootfs512kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_512)_ubi --fsonly $(FLASH_NAND_FS_IMAGE_NAME_512)_ubi
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_1024KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_1024KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_1024KB)  --rootfs jffs2_rootfs1024kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_1024)_jffs2 --fsonly $(FLASH_NAND_FS_IMAGE_NAME_1024)_jffs2
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_1024KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_1024KB)   --ubifs --bootfs bootfs1024kb.img --rootfs ubi_rootfs1024kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_1024)_ubi --fsonly $(FLASH_NAND_FS_IMAGE_NAME_1024)_ubi
endif

ifeq ($(strip $(BUILD_SECURE_BOOT)),y)
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_16KB)),y)
ifeq ($(strip $(BRCM_CHIP)),63268)
        # NOTE: 63268 small page nand bootsize is 128KB on purpose (ie $(FLASH_NAND_BLOCK_128KB)).  do not change
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB) --rootfs jffs2_rootfs16kb_secureboot.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_16)_jffs2_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_16)_jffs2_secureboot
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB) --ubifs --bootfs bootfs16kb_secureboot.img --rootfs ubi_rootfs16kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_16)_ubi_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_16)_ubi_secureboot
else
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_16KB) --rootfs jffs2_rootfs16kb_secureboot.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_16)_jffs2_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_16)_jffs2_secureboot
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_16KB) --ubifs --bootfs bootfs16kb_secureboot.img --rootfs ubi_rootfs16kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_16)_ubi_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_16)_ubi_secureboot
endif
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_128KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_128KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB) --rootfs jffs2_rootfs128kb_secureboot.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_128)_jffs2_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_128)_jffs2_secureboot
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_128KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB) --ubifs --bootfs bootfs128kb_secureboot.img --rootfs ubi_rootfs128kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_128)_ubi_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_128)_ubi_secureboot
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_256KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_256KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_256KB) --rootfs jffs2_rootfs256kb_secureboot.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_256)_jffs2_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_256)_jffs2_secureboot
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_256KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_256KB) --ubifs --bootfs bootfs256kb_secureboot.img --rootfs ubi_rootfs256kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_256)_ubi_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_256)_ubi_secureboot
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_512KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_512KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_512KB) --rootfs jffs2_rootfs512kb_secureboot.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_512)_jffs2_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_512)_jffs2_secureboot
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_512KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_512KB) --ubifs --bootfs bootfs512kb_secureboot.img --rootfs ubi_rootfs512kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_512)_ubi_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_512)_ubi_secureboot
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_1024KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_1024KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_1024KB) --rootfs jffs2_rootfs1024kb_secureboot.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_1024)_jffs2_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_1024)_jffs2_secureboot
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_1024KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_1024KB) --ubifs --bootfs bootfs1024kb_secureboot.img --rootfs ubi_rootfs1024kb.img --image $(FLASH_NAND_CFEROM_FS_IMAGE_NAME_1024)_ubi_secureboot --mirroredcfe --fsonly $(FLASH_NAND_FS_IMAGE_NAME_1024)_ubi_secureboot
endif
endif
    ifeq ($(strip $(SKIP_TIMESTAMP_IMAGE)),)
# copy images to images directory and add a timestamp
	find $(PROFILE_DIR) -name *_nand_cferom_*.w  -printf "%f\n" | while read name; do cp $(PROFILE_DIR)/$$name $(IMAGES_DIR)/$${name/.w/_$(BRCM_RELEASETAG)-$(shell date '+%y%m%d_%H%M').w}; done
    endif

endif

ifneq ($(findstring _$(strip $(BRCM_KERNEL_ROOTFS))_,_all_ _squashfs_),)
# NOR SQUASH
	cd $(PROFILE_DIR); \
	cp $(KERNEL_DIR)/vmlinux . ; \
	$(STRIP) --remove-section=.note --remove-section=.comment vmlinux; \
	$(OBJCOPY) -O binary vmlinux vmlinux.bin; \
	$(HOSTTOOLS_DIR)/cmplzma -k -2 -$(KERNEL_COMPRESSION) vmlinux vmlinux.bin vmlinux.lz; \
	$(HOSTTOOLS_DIR)/bcmImageBuilder $(BRCM_ENDIAN_FLAGS) --output $(FS_KERNEL_IMAGE_NAME) --chip $(or $(TAG_OVERRIDE),$(BRCM_CHIP)) --board $(BRCM_BOARD_ID) --blocksize $(BRCM_FLASHBLK_SIZE) --image-version $(IMAGE_VERSION) --cfefile $(CFE_FILE) --rootfsfile rootfs.img --kernelfile vmlinux.lz; \
	$(HOSTTOOLS_DIR)/bcmImageBuilder $(BRCM_ENDIAN_FLAGS) --output $(CFE_FS_KERNEL_IMAGE_NAME) --chip $(or $(TAG_OVERRIDE),$(BRCM_CHIP)) --board $(BRCM_BOARD_ID) --blocksize $(BRCM_FLASHBLK_SIZE) --image-version $(IMAGE_VERSION) --cfefile $(CFE_FILE) --rootfsfile rootfs.img --kernelfile vmlinux.lz --include-cfe; \
	$(HOSTTOOLS_DIR)/createimg.pl --set  boardid=$(BRCM_BOARD_ID) voiceboardid=$(BRCM_VOICE_BOARD_ID) numbermac=$(BRCM_NUM_MAC_ADDRESSES) macaddr=$(BRCM_BASE_MAC_ADDRESS) tp=$(BRCM_MAIN_TP_NUM) psisize=$(BRCM_PSI_SIZE) logsize=$(BRCM_LOG_SECTION_SIZE) auxfsprcnt=$(BRCM_AUXFS_PERCENT) gponsn=$(BRCM_GPON_SERIAL_NUMBER) gponpw=$(BRCM_GPON_PASSWORD) --inputfile=$(CFE_FS_KERNEL_IMAGE_NAME) --outputfile=$(FLASH_IMAGE_NAME) --nvramfile $(HOSTTOOLS_DIR)/nvram.h --nvramdefsfile $(HOSTTOOLS_DIR)/nvram_defaults.h --config=$(HOSTTOOLS_DIR)/local_install/conf/$(TOOLCHAIN_PREFIX).conf; \
	$(HOSTTOOLS_DIR)/addvtoken --endian $(ARCH_ENDIAN) --chip $(or $(TAG_OVERRIDE),$(BRCM_CHIP)) --flashtype NOR $(FLASH_IMAGE_NAME) $(FLASH_IMAGE_NAME).w


    ifeq ($(strip $(BUILD_SECURE_BOOT)),y)
    ifneq ($(findstring _$(strip $(BRCM_CHIP))_,_63268_63381_63138_63148_),)
    ifeq ($(strip $(BRCM_CHIP)),63268)
	cat $(PROFILE_DIR)/vmlinux_secureboot.lz $(PROFILE_DIR)/vmlinux_secureboot.sig > $(PROFILE_DIR)/vmlinux_secureboot.lz.sig;
    else
	cat $(PROFILE_DIR)/vmlinux.lz $(PROFILE_DIR)/vmlinux.sig > $(PROFILE_DIR)/vmlinux_secureboot.lz.sig;
    endif
	cd $(PROFILE_DIR); \
	$(HOSTTOOLS_DIR)/SecureBootUtils/makeSecureBootCfe spi $(BRCM_CHIP) $(PROFILE_DIR) $(SECURE_BOOT_NOR_BOOT_SIZE); \
	$(HOSTTOOLS_DIR)/bcmImageBuilder $(BRCM_ENDIAN_FLAGS) --output $(FS_KERNEL_IMAGE_NAME)_secureboot --chip $(or $(TAG_OVERRIDE),$(BRCM_CHIP)) --board $(BRCM_BOARD_ID) --blocksize $(SECURE_BOOT_NOR_BOOT_SIZE) --image-version $(IMAGE_VERSION) --cfefile ../cfe/cfe$(BRCM_CHIP)bi_nor.bin --rootfsfile rootfs.img --kernelfile vmlinux_secureboot.lz.sig; \
	$(HOSTTOOLS_DIR)/bcmImageBuilder $(BRCM_ENDIAN_FLAGS) --output $(CFE_FS_KERNEL_IMAGE_NAME)_secureboot --chip $(or $(TAG_OVERRIDE),$(BRCM_CHIP)) --board $(BRCM_BOARD_ID) --blocksize $(SECURE_BOOT_NOR_BOOT_SIZE) --image-version $(IMAGE_VERSION) --cfefile ../cfe/cfe$(BRCM_CHIP)bi_nor.bin --rootfsfile rootfs.img --kernelfile vmlinux_secureboot.lz.sig --include-cfe; \
	rm -f ../cfe/cfe$(BRCM_CHIP)bi_nor.bin vmlinux_secureboot.lz.sig; \
	$(HOSTTOOLS_DIR)/createimg.pl --set boardid=$(BRCM_BOARD_ID) voiceboardid=$(BRCM_VOICE_BOARD_ID) numbermac=$(BRCM_NUM_MAC_ADDRESSES) macaddr=$(BRCM_BASE_MAC_ADDRESS) tp=$(BRCM_MAIN_TP_NUM) psisize=$(BRCM_PSI_SIZE) logsize=$(BRCM_LOG_SECTION_SIZE) auxfsprcnt=$(BRCM_AUXFS_PERCENT) gponsn=$(BRCM_GPON_SERIAL_NUMBER) gponpw=$(BRCM_GPON_PASSWORD) --inputfile=$(CFE_FS_KERNEL_IMAGE_NAME)_secureboot --outputfile=$(FLASH_IMAGE_NAME)_secureboot --nvramfile $(HOSTTOOLS_DIR)/nvram.h --nvramdefsfile $(HOSTTOOLS_DIR)/nvram_defaults.h --config=$(HOSTTOOLS_DIR)/local_install/conf/$(TOOLCHAIN_PREFIX).conf; \
	$(HOSTTOOLS_DIR)/addvtoken --endian $(ARCH_ENDIAN) --chip $(or $(TAG_OVERRIDE),$(BRCM_CHIP)) --flashtype NOR --btrm 1 $(FLASH_IMAGE_NAME)_secureboot $(FLASH_IMAGE_NAME)_secureboot.w
    endif
    endif


    ifeq ($(strip $(SKIP_TIMESTAMP_IMAGE)),)
# copy images to images directory and add a timestamp
	    @cp $(PROFILE_DIR)/$(FLASH_IMAGE_NAME).w $(IMAGES_DIR)/$(FLASH_IMAGE_NAME)_$(BRCM_RELEASETAG)-$(shell date '+%y%m%d_%H%M').w
	    @cp $(PROFILE_DIR)/$(CFE_FS_KERNEL_IMAGE_NAME) $(IMAGES_DIR)/$(CFE_FS_KERNEL_IMAGE_NAME)_$(BRCM_RELEASETAG)-$(shell date '+%y%m%d_%H%M')
    endif

	@echo
	    @echo -e "Done! Image $(PROFILE) has been built in $(PROFILE_DIR)."
endif


nandcfeimage: hosttools_nandcfe
	rm -rf  $(TARGET_FS)/
	mkdir -p $(TARGET_FS)
	cp $(PROFILE_DIR)/../cfe/cfe$(BRCM_CHIP)ram.bin $(TARGET_FS)/cferam.000
	echo -e "/cferam.000" > $(HOSTTOOLS_DIR)/nocomprlist
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_16KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_16KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs16kb.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_128KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_128KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs128kb.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_256KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_256KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs256kb.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_512KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_512KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs512kb.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_1024KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_1024KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs1024kb.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif

ifeq ($(strip $(BUILD_SECURE_BOOT)),y)
	$(HOSTTOOLS_DIR)/SecureBootUtils/makeEncryptedCfeRam $(BRCM_CHIP) $(PROFILE_DIR)
	mv $(PROFILE_DIR)/../cfe/cfeenc$(BRCM_CHIP)ram.bin $(TARGET_FS)/secram.000
	echo -e "/secram.000" >> $(HOSTTOOLS_DIR)/nocomprlist
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_16KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_16KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs16kb_secureboot.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_128KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_128KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs128kb_secureboot.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_256KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_256KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs256kb_secureboot.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_512KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_512KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs512kb_secureboot.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_1024KB)),y)
	$(HOSTTOOLS_DIR)/mkfs.jffs2 -v $(BRCM_ENDIAN_FLAGS) -p -n -e $(FLASH_NAND_BLOCK_1024KB) -r $(TARGET_FS) -o $(PROFILE_DIR)/rootfs1024kb_secureboot.img -N $(HOSTTOOLS_DIR)/nocomprlist
endif
endif
	rm $(HOSTTOOLS_DIR)/nocomprlist

ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_16KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_16KB) --rootfs rootfs16kb.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly.16
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_128KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_128KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB)  --rootfs rootfs128kb.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly.128
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_256KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_256KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_256KB) --rootfs rootfs256kb.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly.256
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_512KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_512KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_512KB)  --rootfs rootfs512kb.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly.512
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_1024KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_1024KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_1024KB)  --rootfs rootfs1024kb.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly.1024
endif

ifeq ($(strip $(BUILD_SECURE_BOOT)),y)
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_16KB)),y)
ifeq ($(strip $(BRCM_CHIP)),63268)
	# NOTE: 63268 small page nand bootsize is 128K on purpose (ie $(FLASH_NAND_BLOCK_128KB)). Do not change.
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB) --rootfs rootfs16kb_secureboot.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly_secureboot.16 --mirroredcfe 
else
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_16KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_16KB) --rootfs rootfs16kb_secureboot.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly_secureboot.16 --mirroredcfe 
endif
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_128KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_128KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_128KB) --rootfs rootfs128kb_secureboot.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly_secureboot.128 --mirroredcfe
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_256KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_256KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_256KB) --rootfs rootfs256kb_secureboot.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly_secureboot.256 --mirroredcfe
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_512KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_512KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_512KB) --rootfs rootfs512kb_secureboot.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly_secureboot.512 --mirroredcfe
endif
ifeq ($(strip $(BUILD_NAND_IMG_BLKSIZE_1024KB)),y)
	$(HOSTTOOLS_DIR)/scripts/bcmImageMaker --cferom $(CFE_ROM_FILE) --blocksize $(FLASH_NAND_BLOCK_1024KB) --bootofs $(FLASH_BOOT_OFS) --bootsize $(FLASH_NAND_BLOCK_1024KB) --rootfs rootfs1024kb_secureboot.img --image $(FLASH_BASE_IMAGE_NAME)_nand_cfeonly_secureboot.1024 --mirroredcfe
endif
endif

	rm -f  $(TARGET_FS)/cferam.000
	rm -f  $(TARGET_FS)/secram.000



###########################################
#
# System code clean-up
#
###########################################

#
# mwang: since SUBDIRS are no longer defined, the next two targets are not useful anymore.
# how were they used anyways?
#
#subdirs: $(patsubst %, _dir_%, $(SUBDIRS))

#$(patsubst %, _dir_%, $(SUBDIRS)) :
#	$(MAKE) -C $(patsubst _dir_%, %, $@) $(TGT)

clean: bcmdrivers_clean data-model_clean userspace_clean  \
       kernel_clean hosttools_clean voice_clean xchange_clean wlan_clean nvram_3k_kernelclean target_clean rdplink_clean
	rm -f $(HOSTTOOLS_DIR)/scripts/lxdialog/*.o
	rm -f .tmpconfig*
	rm -f $(LAST_PROFILE_COOKIE)
	rm -f $(HOST_PERLARCH_COOKIE)

cleanall: clean_local_tools clean

clean_local_tools:
	rm -rf $(HOSTTOOLS_DIR)/local_install

fssrc_clean:
	rm -fr $(FSSRC_DIR)/bin
	rm -fr $(FSSRC_DIR)/sbin
	rm -fr $(FSSRC_DIR)/lib
	rm -fr $(FSSRC_DIR)/upnp
	rm -fr $(FSSRC_DIR)/docs
	rm -fr $(FSSRC_DIR)/webs
	rm -fr $(FSSRC_DIR)/usr
	rm -fr $(FSSRC_DIR)/linuxrc
	rm -fr $(FSSRC_DIR)/images
	rm -fr $(FSSRC_DIR)/etc/wlan
	rm -fr $(FSSRC_DIR)/etc/certs

kernel_clean: sanity_check
	-$(MAKE) -C $(KERNEL_DIR) mrproper
	rm -f $(KERNEL_DIR)/arch/mips/defconfig
	rm -f $(KERNEL_DIR)/arch/arm/defconfig
	rm -f $(HOSTTOOLS_DIR)/lzma/decompress/*.o
	rm -rf $(XCHANGE_DIR)/dslx/lib/LinuxKernel
	rm -rf $(XCHANGE_DIR)/dslx/obj/LinuxKernel
	rm -f $(KERNEL_INCLUDE_LINK)
	rm -f $(KERNEL_MIPS_INCLUDE_LINK)
	rm -f $(KERNEL_ARM_INCLUDE_LINK)

bcmdrivers_clean:
	-$(MAKE) -C bcmdrivers clean

userspace_clean: sanity_check fssrc_clean 
	-$(MAKE) -C userspace clean

data-model_clean:
	-$(MAKE) -C data-model clean

unittests_clean:
	-$(MAKE) -C unittests clean

target_clean: sanity_check
	rm -f $(PROFILE_DIR)/*.img
	rm -f $(PROFILE_DIR)/vmlinux*
	rm -f $(PROFILE_DIR)/*.w
	rm -f $(PROFILE_DIR)/*.srec
	rm -f $(PROFILE_DIR)/ramdisk
	rm -f $(PROFILE_DIR)/$(FS_KERNEL_IMAGE_NAME)*
	rm -f $(PROFILE_DIR)/$(CFE_FS_KERNEL_IMAGE_NAME)*
	rm -f $(PROFILE_DIR)/$(FLASH_IMAGE_NAME)*
	rm -fr $(PROFILE_DIR)/modules
	rm -fr $(PROFILE_DIR)/op
	rm -fr $(INSTALL_DIR)
	rm -fr $(BCM_FSBUILD_DIR)
	find targets -name vmlinux -print -exec rm -f "{}" ";"
	rm -fr $(TARGET_FS)
ifeq ($(strip $(BRCM_KERNEL_ROOTFS)),all)
	rm -fr $(TARGET_BOOTFS)
endif

hosttools_clean:
	-$(MAKE) -C $(HOSTTOOLS_DIR) clean

xchange_clean:
	rm -rf $(XCHANGE_DIR)/dslx/lib/LinuxUser
	rm -rf $(XCHANGE_DIR)/dslx/obj/LinuxUser	

# In the data release tarball, vodsl is not present,
# so don't go in there unconditionally.

ifneq ($(strip $(BUILD_VODSL)),)
vodsl_clean: sanity_check
	-$(MAKE) -C $(BUILD_DIR)/userspace/private/apps/vodsl clean
	find userspace/private/apps/vodsl -name '*.o'|xargs rm -f
else
vodsl_clean:
	@echo "skipping vodsl (not configured)"
endif

ifneq ($(strip $(BUILD_DECT)),)
dectd_clean: sanity_check fssrc_clean 
	-$(MAKE) -C $(BUILD_DIR)/userspace/private/apps/dectd clean
	find bcmdrivers/opensource/char/dectshim -name '*.ko'|xargs rm -f
	find bcmdrivers/opensource/char/dectshim -name '*.o'|xargs rm -f
	find bcmdrivers/opensource/char/dectshim -name '*.a'|xargs rm -f
	find bcmdrivers/opensource/char/dectshim -name '*.lib'|xargs rm -f
else
dectd_clean:
	@echo "Skipping dectd application (not configured)"
endif

voice_clean: sanity_check vodsl_clean data-model_clean dectd_clean
ifneq ($(strip $(BUILD_VODSL)),)
	find bcmdrivers/broadcom/char/endpoint -name '*.ko'|xargs rm -f
	find bcmdrivers/broadcom/char/endpoint -name '*.o'|xargs rm -f
	find bcmdrivers/broadcom/char/endpoint -name '*.a'|xargs rm -f
	find bcmdrivers/broadcom/char/endpoint -name '*.lib'|xargs rm -f
	find bcmdrivers/broadcom/char/endpoint -name '.tmp_versions'|xargs rm -rf
	find bcmdrivers/broadcom/char/dspapp -name '*.o'|xargs rm -f
	find bcmdrivers/broadcom/char/dspapp -name '*.ko'|xargs rm -f
	find kernel/linux/eptlib -name '*.lib'|xargs rm -f
	find userspace/private -name '*voice.o'|xargs rm -f
endif
	rm -rf $(XCHANGE_DIR)/dslx/lib/LinuxKernel
	rm -rf $(XCHANGE_DIR)/dslx/obj/LinuxKernel
	rm -rf $(XCHANGE_DIR)/dslx/lib/LinuxUser
	rm -rf $(XCHANGE_DIR)/dslx/obj/LinuxUser
	rm -f $(KERNEL_DIR)/eptlib/*

slic_clean: sanity_check voice_clean
ifneq ($(strip $(BUILD_VODSL)),)
	find userspace/private/libs/cms_core/linux -name 'rut_voice.o' -exec rm -f "{}" ";"
	find userspace/private/libs/cms_core/linux -name 'rut_voice.d' -exec rm -f "{}" ";"
	find userspace/private/libs/cms_dal -name 'dal_voice.o' -exec rm -f "{}" ";"
	find userspace/private/libs/cms_dal -name 'dal_voice.d' -exec rm -f "{}" ";"
endif
	-$(MAKE) -C data-model clean

wlan_clean:
	-$(MAKE) -C $(BUILD_DIR)/userspace/private/apps/wlan clean 
	-$(MAKE) -C $(BUILD_DIR)/userspace/private/libs/wlcsm clean 
	-$(MAKE) -C $(BUILD_DIR)/userspace/private/apps/httpd clean 
	cd bcmdrivers/broadcom/net/wl;\
	files="\
		`find . -name 'build' | sed 's/tools\///' `\
		`find . -type l`\
		`find . -name '*.o'` \
		`find . -name '*.ko'`\
		`find . -name '*.oo'`\
		`find . -name '*.bin' ! -iname 'rtecdc.bin'`\
		`find . -name 'dummy.c'`\
		`find . -name 'epivers'`\
		`find . -name 'rver'`\
	";\
	for x in $$files; do\
		rm -rf $$x;\
	done;\
	cd $(BUILD_DIR)/targets;\
	rm -rf $(PROFILE)/fs.install/etc/wlan

###########################################
#
# Temporary kernel patching mechanism
#
###########################################

.PHONY: genpatch patch

genpatch:
	@hostTools/kup_tmp/genpatch

patch:
#	@hostTools/kup_tmp/patch

###########################################
#
# Get modules version
#
###########################################
.PHONY: version_info

version_info: sanity_check
ifeq ($(wildcard $(KERNEL_DIR)/.config),)
	$(call pre_kernelbuild)
endif
	@echo "$(MAKECMDGOALS):";\
	cd $(KERNEL_DIR); $(MAKE) --silent version_info;

###########################################
#
# System-wide exported variables
# (in alphabetical order)
#
###########################################


export \
ACTUAL_MAX_JOBS            \
BRCMAPPS                   \
BRCM_BOARD                 \
BRCM_DRIVER_PCI            \
BRCM_EXTRAVERSION          \
BRCM_KERNEL_NETQOS         \
BRCM_KERNEL_ROOTFS         \
BRCM_KERNEL_AUXFS_JFFS2    \
BRCM_LDX_APP               \
BRCM_MIPS_ONLY_BUILD       \
BRCM_CPU_FREQ_PWRSAVE      \
BRCM_CPU_FREQ_TARGET_LOAD  \
BRCM_PSI_VERSION           \
BRCM_PTHREADS              \
BRCM_RAMDISK_BOOT_EN       \
BRCM_RAMDISK_SIZE          \
BRCM_NFS_MOUNT_EN          \
BRCM_RELEASE               \
BRCM_RELEASETAG            \
BRCM_SNMP                  \
BRCM_VERSION               \
BUILD_CMFCTL               \
BUILD_CMFVIZ               \
BUILD_CMFD                 \
BUILD_XDSLCTL              \
BUILD_XTMCTL               \
BUILD_VLANCTL              \
BUILD_BRCM_VLAN            \
BUILD_BRCTL                \
BUILD_BUSYBOX              \
BUILD_BUSYBOX_BRCM_LITE    \
BUILD_BUSYBOX_BRCM_FULL    \
BUILD_CERT                 \
BUILD_DDNSD                \
BUILD_DEBUG_TOOLS          \
BUILD_DIAGAPP              \
BUILD_DIR                  \
BUILD_DNSPROBE             \
BUILD_DPROXY               \
BUILD_DPROXYWITHPROBE      \
BUILD_DYNAHELPER           \
BUILD_DNSSPOOF             \
BUILD_EBTABLES             \
BUILD_EPITTCP              \
BUILD_ETHWAN               \
BUILD_FTPD                 \
BUILD_FTPD_STORAGE         \
BUILD_MCAST_PROXY          \
BUILD_WLHSPOT              \
BUILD_IPPD                 \
BUILD_IPROUTE2             \
BUILD_IPSEC_TOOLS          \
BUILD_L2TPAC               \
BUILD_ACCEL_PPTP           \
BUILD_IPTABLES             \
BUILD_WPS_BTN              \
BUILD_LLTD                 \
BUILD_WSC                  \
BUILD_BCMCRYPTO \
BUILD_BCMSHARED \
BUILD_MKSQUASHFS           \
BUILD_NAS                  \
BUILD_NVRAM                \
BUILD_PORT_MIRRORING			 \
BUILD_PPPD                 \
PPP_AUTODISCONN			   \
BUILD_SES                  \
BUILD_SIPROXD              \
BUILD_SLACTEST             \
BUILD_SNMP                 \
BUILD_SNTP                 \
BUILD_SOAP                 \
BUILD_SOAP_VER             \
BUILD_SSHD                 \
BUILD_SSHD_MIPS_GENKEY     \
BUILD_TOD                  \
BUILD_TR64                 \
BUILD_TR64_DEVICECONFIG    \
BUILD_TR64_DEVICEINFO      \
BUILD_TR64_LANCONFIGSECURITY \
BUILD_TR64_LANETHINTERFACECONFIG \
BUILD_TR64_LANHOSTS        \
BUILD_TR64_LANHOSTCONFIGMGMT \
BUILD_TR64_LANUSBINTERFACECONFIG \
BUILD_TR64_LAYER3          \
BUILD_TR64_MANAGEMENTSERVER  \
BUILD_TR64_TIME            \
BUILD_TR64_USERINTERFACE   \
BUILD_TR64_QUEUEMANAGEMENT \
BUILD_TR64_LAYER2BRIDGE   \
BUILD_TR64_WANCABLELINKCONFIG \
BUILD_TR64_WANCOMMONINTERFACE \
BUILD_TR64_WANDSLINTERFACE \
BUILD_TR64_WANDSLLINKCONFIG \
BUILD_TR64_WANDSLCONNECTIONMGMT \
BUILD_TR64_WANDSLDIAGNOSTICS \
BUILD_TR64_WANETHERNETCONFIG \
BUILD_TR64_WANETHERNETLINKCONFIG \
BUILD_TR64_WANIPCONNECTION \
BUILD_TR64_WANPOTSLINKCONFIG \
BUILD_TR64_WANPPPCONNECTION \
BUILD_TR64_WLANCONFIG      \
BUILD_TR69C                \
BUILD_TR69_QUEUED_TRANSFERS \
BUILD_TR69C_SSL            \
BUILD_TR69_XBRCM           \
BUILD_TR69_UPLOAD          \
BUILD_TR69C_VENDOR_RPC     \
BUILD_OMCI                 \
BUILD_UDHCP                \
BUILD_UDHCP_RELAY          \
BUILD_UPNP                 \
BUILD_VCONFIG              \
BUILD_SUPERDMZ             \
BUILD_WLCTL                \
BUILD_DHDCTL               \
BUILD_ZEBRA                \
BUILD_LIBUSB               \
BUILD_WANVLANMUX           \
HOSTTOOLS_DIR              \
INC_KERNEL_BASE            \
INSTALL_DIR                \
PROFILE_DIR                \
WEB_POPUP                  \
BUILD_VIRT_SRVR            \
BUILD_PORT_TRIG            \
BUILD_TR69C_BCM_SSL        \
BUILD_IPV6                 \
BUILD_BOARD_LOG_SECTION    \
BRCM_LOG_SECTION_SIZE      \
BRCM_FLASHBLK_SIZE         \
BRCM_AUXFS_PERCENT         \
BRCM_BACKUP_PSI            \
LINUX_KERNEL_USBMASS       \
BUILD_IPSEC                \
BUILD_MoCACTL              \
BUILD_MoCACTL2             \
BUILD_6802_MOCA            \
BRCM_MOCA_AVS          \
BUILD_GPON                 \
BUILD_GPONCTL              \
BUILD_PMON                 \
BUILD_BUZZZ                \
BUILD_BOUNCE               \
BUILD_HELLO                \
BUILD_SPUCTL               \
BUILD_PWRCTL               \
BUILD_RNGD                 \
RELEASE_BUILD              \
NO_PRINTK_AND_BUG          \
FLASH_NAND_BLOCK_16KB      \
FLASH_NAND_BLOCK_128KB     \
FLASH_NAND_BLOCK_256KB     \
FLASH_NAND_BLOCK_512KB     \
FLASH_NAND_BLOCK_1024KB     \
FLASH_NAND_BLOCK_2056KB     \
BRCM_SCHED_RT_RUNTIME       \
BRCM_CONFIG_HIGH_RES_TIMERS \
BRCM_SWITCH_SCHED_SP        \
BRCM_SWITCH_SCHED_WRR       \
BUILD_SWMDK                 \
BUILD_IQCTL                 \
BUILD_BPMCTL                \
BUILD_EPONCTL               \
BUILD_ETHTOOL               \
BUILD_TMS                   \
IMAGE_VERSION               \
TOOLCHAIN_VER               \
TOOLCHAIN_PREFIX            \
PROFILE_KERNEL_VER          \
KERNEL_LINKS_DIR            \
LINUX_VER_STR               \
KERNEL_DIR                  \
FORCE                       \
BUILD_VLAN_AGGR             \
BUILD_DPI                   \
BRCM_KERNEL_DEBUG           \
BUILD_BRCM_FTTDP            \
BUILD_BRCM_XDSL_DISTPOINT   \
BRCM_1905_FM                \
BUILD_BRCM_CMS              \
BUILD_WEB_SOCKETS           \
BUILD_WEB_SOCKETS_TEST      \
BRCM_1905_TOPOLOGY_WEB_PAGE \
BUILD_NAND_KERNEL_LZMA      \
BUILD_NAND_KERNEL_LZ4       \
BUILD_DISABLE_EXEC_STACK    \
NO_MINIFY                   \
BCM_SPEEDYGET

