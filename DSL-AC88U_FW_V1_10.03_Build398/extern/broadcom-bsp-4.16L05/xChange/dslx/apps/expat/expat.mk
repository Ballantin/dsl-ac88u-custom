###########################################################################
#
# This makefile is used to build the Expat library.
#
###########################################################################

#
# This should match the target name.
#
EXPAT_LIBDIR = $(CXC_CABLEX_DIR)/lib/$(CXC_OS_TYPE)
XCHG_OBJ_DIR = $(CXC_CABLEX_DIR)/obj/$(CXC_OS_TYPE)
EXPAT_ROOT = $(CXC_EXPAT_ROOT)

XCHG_C_LOCAL_COMPILER_OPTS += -Wno-error

include $(EXPAT_ROOT)/expat.mk
