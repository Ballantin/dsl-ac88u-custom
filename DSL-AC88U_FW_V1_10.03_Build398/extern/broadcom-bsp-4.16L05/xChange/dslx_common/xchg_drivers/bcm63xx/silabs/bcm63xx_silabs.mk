###########################################################################
#
# This makefile is used to build the PROSlics module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE           := bcm63xx_silabs

BCM63XX_SILABS_ROOT := $(XDRV_DIR_$(XCHG_LIB_BASE))


#
# We use notdir and vpath so that the object files don't wind up in
# nested subdirectories
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/*.c))                                      \
                                   $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/common/*.c))                               \
                                   $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/*.c))                          \
				   $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_mlt_api/*.c))                      \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si321x/*.c))      \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3217x/*.c))      \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3226/*.c))      \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3226x/*.c))      \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_mlt_api/si3217x_mlt/*.c))           \
						       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_mlt_api/si3226x_mlt/*.c))          \
						       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/common/*.c)) \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si321x/*.c)) \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3217x/*.c)) \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226/*.c)) \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226x/*.c)) \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226x_isi/*.c)) \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si3050/*.c))                               \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si3215/*.c))                               \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si3216/*.c))                               \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si32178/*.c))                              \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si3226/*.c))                                \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si32176/*.c))                               \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si32261/*.c))                               \
				                       $(notdir $(wildcard $(BCM63XX_SILABS_ROOT)/si32267/*.c))                               \
						       
XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))

vpath %.c $(BCM63XX_SILABS_ROOT)                                       \
          $(BCM63XX_SILABS_ROOT)/common                                \
          $(BCM63XX_SILABS_ROOT)/proslic_api                           \
          $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si321x/      \
          $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3217x/      \
          $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3226/      \
          $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3226x/      \
          $(BCM63XX_SILABS_ROOT)/proslic_mlt_api                        \
          $(BCM63XX_SILABS_ROOT)/proslic_mlt_api/si3217x_mlt/           \
          $(BCM63XX_SILABS_ROOT)/proslic_mlt_api/si3226x_mlt/           \
          $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/common/ \
          $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si321x/ \
          $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3217x/ \
          $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226/ \
          $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226x/ \
          $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226x_isi/ \
          $(BCM63XX_SILABS_ROOT)/si3050                                \
          $(BCM63XX_SILABS_ROOT)/si3215                                \
          $(BCM63XX_SILABS_ROOT)/si3216                                \
          $(BCM63XX_SILABS_ROOT)/si32178                               \
          $(BCM63XX_SILABS_ROOT)/si32176                               \
          $(BCM63XX_SILABS_ROOT)/si3226                                \
          $(BCM63XX_SILABS_ROOT)/si32261                               \
          $(BCM63XX_SILABS_ROOT)/si32267
#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) := XDRV_SILABS_TPD_SUPPORT=0 

#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(VRG_ROOT)/inc        \
                                         $(VRG_ROOT)/casCtl/inc \
                                         $(VRG_ROOT)/boardHal/inc

XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(BCM63XX_SILABS_ROOT)                                       \
                                         $(BCM63XX_SILABS_ROOT)/common                                \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api                           \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si321x/      \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3217x/      \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3226/      \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/slic_drivers/si3226x/      \
                                         $(BCM63XX_SILABS_ROOT)/proslic_mlt_api                        \
                                         $(BCM63XX_SILABS_ROOT)/proslic_mlt_api/si3217x_mlt/          \
                                         $(BCM63XX_SILABS_ROOT)/proslic_mlt_api/si3226x_mlt/          \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/common/ \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si321x/ \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3217x/ \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226/ \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226x/ \
                                         $(BCM63XX_SILABS_ROOT)/proslic_api/interface_drivers/si3226x_isi/ \
                                         $(BCM63XX_SILABS_ROOT)/si3050                                \
                                         $(BCM63XX_SILABS_ROOT)/si3215                                \
                                         $(BCM63XX_SILABS_ROOT)/si3216                                \
                                         $(BCM63XX_SILABS_ROOT)/si32178                               \
                                         $(BCM63XX_SILABS_ROOT)/si32176                               \
                                         $(BCM63XX_SILABS_ROOT)/si3226                                 \
                                         $(BCM63XX_SILABS_ROOT)/si32261                                \
                                         $(BCM63XX_SILABS_ROOT)/si32267

XCHG_C_LOCAL_COMPILER_OPTS := -Wno-error

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
