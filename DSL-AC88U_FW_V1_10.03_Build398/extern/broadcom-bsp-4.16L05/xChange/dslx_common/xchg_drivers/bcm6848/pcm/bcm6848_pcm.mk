###########################################################################
#
# This makefile is used to build the bcm6848_pcm module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE           := bcm6848_pcm

BCM6848_PCM_ROOT := $(XDRV_DIR_$(XCHG_LIB_BASE))


#
# We use notdir and vpath so that the object files don't wind up in
# nested subdirectories
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(BCM6848_PCM_ROOT)/*.c))

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))

vpath %.c $(BCM6848_PCM_ROOT)


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) :=  $(XCHG_LINUX_ROOT)/../../bcmdrivers/opensource/char/rdpa_gpl/impl1/include \
                                          $(XCHG_LINUX_ROOT)/../../bcmdrivers/opensource/char/bdmf/impl1/framework \
                                          $(XCHG_LINUX_ROOT)/../../bcmdrivers/opensource/char/bdmf/bcm96848/system/linux/cm \
                                          $(XCHG_LINUX_ROOT)/../../bcmdrivers/opensource/char/bdmf/bcm96848/system/sim \
                                          $(XCHG_LINUX_ROOT)/../../bcmdrivers/opensource/char/bdmf/bcm96848/system/linux \
                                          $(XCHG_LINUX_ROOT)/../../bcmdrivers/opensource/char/bdmf/bcm96848/system \
                                          $(XCHG_LINUX_ROOT)/../../bcmdrivers/broadcom/char/rdpa/bcm96848/plateform/sim \
                                          $(XCHG_LINUX_ROOT)/../../bcmdrivers/opensource/include/rdp/
#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) :=


#
# To set a module specific compiler option, create a target specific make
# variable which sets the option and depends on the module target name.
# For example, to enable compiler optimizations for this module:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_ENABLE)
#
# or, to disable optimizations:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_DISABLE)
