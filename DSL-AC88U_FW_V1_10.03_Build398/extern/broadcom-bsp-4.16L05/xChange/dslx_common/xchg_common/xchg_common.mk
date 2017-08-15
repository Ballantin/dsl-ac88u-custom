###########################################################################
#
# /xchg_common/xchg_common.mk
#
# This makefile is used to build all of the components contained in
# the xchg_common VOB.
#
# It is intended to be invoked (not included) from a project makefile, but can 
# be invoked directly from the command line if the appropriate environment 
# variables are set.
#
###########################################################################



# ---------------------------------------------------------------------------
# Setup generic build environment.
#
include $(XCHG_ROOT)/rules/xChgEnv.mk
include $(XCHG_RULES_DIR)/xChgCheckVars.mk


#
# Setup xchg_common build environment.
#
include $(XCHG_ROOT)/xchg_common_env.mk



# ---------------------------------------------------------------------------
# Determine the set of modules to make.
#
# If no target is explicitly specified, the default behaviour is to build all 
# targets specified by $(XCHG_MAKE_TARGETS).
# 
XCHG_VALID_TARGETS=$(XCHG_ALL_TARGETS) $(addprefix clean_,$(XCHG_ALL_TARGETS)) $(addprefix audit_,$(XCHG_ALL_TARGETS))

ifeq ($(strip $(filter $(MAKECMDGOALS),${XCHG_VALID_TARGETS})),)
# Either no target was specified, or a target that does not match a module name
# was specified (e.g. clean).
libTargets := $(XCHG_MAKE_TARGETS)
else
# Build a specific module (e.g. bos), or clean a specific module (using the clean_xxx target).
libTargets := $(subst clean_,,$(MAKECMDGOALS))
libTargets := $(subst audit_,,$(MAKECMDGOALS))
endif



# ---------------------------------------------------------------------------
# These will be added to by the module makefiles. Initialize these here 
# using immediate evaluation to guarantee that when things get added
# to these variables that it will be done using immediate evaluation. This is
# more efficient than using deferred variable evaluation.
#
XCHG_SRC_FILES                   :=
XCHG_C_IMPORTED_GLOBAL_DEFS      := $(XCHG_C_GLOBAL_DEFS)
XCHG_C_GLOBAL_DEFS               := $(XCHG_C_IMPORTED_GLOBAL_DEFS)
XCHG_C_IMPORTED_GLOBAL_INCLUDE   := $(XCHG_C_GLOBAL_INCLUDE)
XCHG_C_GLOBAL_INCLUDE            := $(XCHG_C_IMPORTED_GLOBAL_INCLUDE)


#
# Append the public include paths and defines for the specified xchg_common 
# targets to the global compiler options.
#
XCHG_C_GLOBAL_DEFS               += $(XCHG_COMPOSITE_PUBLIC_DEFS)
XCHG_C_GLOBAL_INCLUDE            += $(XCHG_COMPOSITE_PUBLIC_INCLUDE)



# ---------------------------------------------------------------------------
# Include all the module makefiles.
#

# This phony target is necessary because some of the module makefiles may have
# targets in them, and we don't want one of them to become the default 
# make target. Also, xChgRules.mk needs to be included after the module 
# makefiles. The real 'all' target is specified in xChgRules.mk.
.PHONY: all
all:

XCHG_MODULE_MAKEFILES := $(foreach target,$(libTargets),$(XCHG_DIR_$(target))/$(target).mk)
include $(XCHG_MODULE_MAKEFILES)


include $(XCHG_ROOT)/rules/xChgRules.mk


# ---------------------------------------------------------------------------
# Default target - build all specified libraries.
#
.PHONY: target $(XCHG_MAKE_TARGETS)
target: $(XCHG_MAKE_TARGETS)

target_with_audit: $(addprefix audit_,$(XCHG_MAKE_TARGETS))


#
# Target to clean a specific module, e.g. clean_bos.
#
.PHONY: $(addprefix clean_,$(XCHG_ALL_TARGETS))
$(addprefix clean_,$(XCHG_ALL_TARGETS)): clean_obj clean_dep clean_xchg_common_libs

.PHONY: clean_xchg_common_libs
clean_lib: clean_xchg_common_libs
clean_xchg_common_libs:
	$(RM) -f $(call XCHG_MAKE_LIB_NAME,$(libTargets))



# ---------------------------------------------------------------------------
# Generate the make syntax that defines the rules that are used to build the
# module libraries.
# 

# Error check.
$(foreach target,$(libTargets),$(if $(XCHG_SRC_FILES_$(target)),,$(error "XCHG_SRC_FILES_$(target) is empty.")))

# Generate library targets.
$(foreach target,$(libTargets),$(eval $(call XCHG_ARCHIVE_TEMPLATE,$(target))))
