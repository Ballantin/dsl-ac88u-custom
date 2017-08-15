###########################################################################
#
# xChgRules.mk
#
# This file is the common file which is included for building projects
# within the xchg_common VOB.
#
# The general intent of the xchg_common VOB is to deliver source code, and
# not libraries/header files. The high amount of compile time
# configurability also makes a source delivery the most practical.
#
# Because of this, what is provided here is a makefile infrastructure.
#
# Please see the list of variables below. The only variable that MUST
# be set is XCHG_ROOT. Everything else has a default.
#
###########################################################################

ifneq ($(XCHG_ENV_INCLUDED),1)
$(error xChgEnv.mk needs to be included BEFORE xChgRules.mk)
endif

#--------------------------------------------------------------------------
# PATH is manipulated in the Rules makefile NOT the ENV makefile.
# This is because ENV type makefiles can be included in other make environments
# and we don't want to change their path. Only when we get around to building
# xchg_common targets do we want to take over the path.
#
# Add the xChg tools bin directory into the path.
#
# For Windows host platforms, both 'PATH' and 'Path' must be set in order to
# satisfy windows and make. Make built for win32 understands mix mode paths
# using ';' as the path seperator. Windows also understands mixed mode paths
# with ';' seperators. Make uses the 'PATH' variable but windows uses 'Path'.
# Both are set identically.
#
# The inherited PATH will be in windows, non mixed format, because make (win32 build)
# converts it when launched. This is best converted to mixed mode format by
# substituting '\' with \/'. This may not be strictly necessary, but it eases
# interworking issues between cygwin apps and windows.
#
# For Linux host platforms, a ':' path separator needs to be used.
#
# We only need to set and export the paths once, this is more efficient than
# calling cygpath to do the conversion for every module that includes this
# makefile.
#

ifeq ($(findstring win32,$(XCHG_HOST_TYPE)),win32)
XCHG_PATH_SEPARATOR := ;
else
XCHG_PATH_SEPARATOR := :
endif

ifeq ($(XCHG_PATH_ADDED),)
	export XCHG_PATH_ADDED := TRUE
	export PATH := $(XCHG_TOOLS_BIN_DIR)$(XCHG_PATH_SEPARATOR)$(subst \,/,$(PATH))
	export Path := $(PATH)
endif

 

#--------------------------------------------------------------------------
# SHELL is manipulated in the Rules makefile NOT the ENV makefile.
# This is because ENV type makefiles can be included in other make environments
# and we don't want to change their SHELL. Only when we get around to building
# xchg_common targets do we want to take over the SHELL
#
# Setup the SHELL so that we're using a known one
#

ifeq ($(findstring win32,$(XCHG_HOST_TYPE)),win32)
# Only need to set the shell for Win32.
export SHELL := $(XCHG_TOOLS_BIN_DIR)/sh.exe
endif


#--------------------------------------------------------------------------
#
# You can run arbitrary commands from within the context that is setup by
# the makefile by using:
#
#  make exec="command to execute"
#
# Note that we're also specifying the default target here.
#
# You can also specify arbitrary targets by using the target=foo
# This is useful when trying to build preprocessor (%.pp) and such.
#

ifeq ($(exec),)
ifeq ($(target),)
all: target
else
MAKECMDGOALS += $(target)
export target
all: $(target)
endif
else
MAKECMDGOALS += exec
export exec
all: exec
endif
.PHONY: all target exec

#--------------------------------------------------------------------------
#
# Figure out what files need to be built. Note that even though all of the
# components of XCHG_LIB_NAME haven't been defined yet, this is fine because
# of the deferred way that make evaluates things.
#
# The "EXTRA" versions are files that need to be compiled, but NOT added to
# a library. Typically this is used when mixing libraries and executables in
# the same makefile.
#
# We use the $(addsuffix $(XCHG_OBJ_EXT),$(basename ...)) rather than
# .c=$(XCHG_OBJ_EXT) so that we can get .c and .cpp files.
#

XCHG_OBJ_FILES = $(addprefix $(XCHG_OBJ_DIR)/, $(addsuffix $(XCHG_OBJ_EXT),$(basename $(XCHG_SRC_FILES))))
XCHG_EXTRA_OBJ_FILES = $(addprefix $(XCHG_OBJ_DIR)/, $(addsuffix $(XCHG_OBJ_EXT),$(basename $(XCHG_EXTRA_SRC_FILES))))

XCHG_EXTRA_DEP_FILES = $(XCHG_EXTRA_OBJ_FILES:$(XCHG_OBJ_EXT)=.d)
XCHG_DEP_FILES = $(XCHG_OBJ_FILES:$(XCHG_OBJ_EXT)=.d) $(XCHG_EXTRA_DEP_FILES)


#--------------------------------------------------------------------------
#
# Get the OS specific stuff
#

ifeq ($(XCHG_OS),LinuxKernel)
include $(XCHG_ROOT)/crt/rules/crtEnv.mk
endif

ifeq ($(XCHG_OS_RULES_FILE),)
XCHG_OS_RULES_FILE := $(XCHG_RULES_DIR)/xChgRules_$(XCHG_ENV).mk
endif
include $(XCHG_OS_RULES_FILE)



#--------------------------------------------------------------------------
#
# This has been maintained for backwards compatibility, and provides the
# ability to build module libraries recursively. In this case, the default target
# will be 'XCHG_LIB_BASE'.
#
# In order operate in this mode, XCHG_RECURSIVE_BUILD=1 must be set (either
# globally in the project makefile, or in individual module makefiles).
#
# Since XCHG_LIB_NAME can't be evaluated until the OS specific rules have
# been included, we need to put the targets that use XCHG_LIB_NAME after
# the include.
#

ifneq ($(XCHG_LIB_BASE),)
ifeq ($(XCHG_RECURSIVE_BUILD),1)

XCHG_LIB_NAME  := $(call XCHG_MAKE_LIB_NAME,$(XCHG_LIB_BASE))

lib: $(XCHG_LIB_NAME)
target: lib

$(XCHG_LIB_NAME) : $(XCHG_OBJ_FILES)
	$(ECHO) 'Creating Archive $@'
	@$(MKDIR) -p $(@D)
	$(RM) -f $@
	$(XCHG_ARCHIVE) $(XCHG_ARCHIVE_OPTS)$@ $^
	$(ECHO)
 
clean_lib : clean_lib_$(XCHG_LIB_BASE)
clean_lib_$(XCHG_LIB_BASE) :
	$(ECHO) 'Removing $(XCHG_LIB_BASE) library'
	$(RM) -f $(XCHG_LIB_NAME)
 
.PHONY: clean_lib_$(XCHG_LIB_BASE)

endif
endif


#--------------------------------------------------------------------------
# This defined sequence of variables and archive rule provides a "template"
# that is used to generate the library for each module using the non-recursive
# build model.
#
# It is expected that each module makefile will define XCHG_SRC_FILES_xxx,
# where 'xxx' is the module's target name. Based upon this, we can determine
# the set of dependency and object files that should be generated.
#
# In order to use this defined template, the project makefile should include
# a line similar to the following:
#
#  $(foreach target,$(libTargets),$(eval $(call XCHG_ARCHIVE_TEMPLATE,$(target))))
#
# where 'libTargets' is a list of all the library targets to build.
#
# The argument to the eval function (module target) is expanded, then the
# results of that expansion are parsed as makefile syntax. This results in a
# set of rules that define how to build each individual module library.
#
# Also included in this template are a set of target-specific variables that
# define the compiler include and defines that are _local_ to the library.
# When you define a target-specific variable, that variable value is also in
# effect for all prerequisites of this target. In our case, the target is the
# the module library, and therefore the prerequisite object files also
# inherit the module specific compiler options. To use this feature, the
# module makefile should set the following variables - XCHG_C_LOCAL_INCLUDE_xxx
# and XCHG_C_LOCAL_DEFS_xxx, again, where 'xxx' is the module's target name.
#
define XCHG_ARCHIVE_TEMPLATE
XCHG_OBJ_FILES_$(1) := $(addprefix $(XCHG_OBJ_DIR)/, $(addsuffix $(XCHG_OBJ_EXT),$(basename $(XCHG_SRC_FILES_$(1)))))
XCHG_DEP_FILES_$(1) := $$(XCHG_OBJ_FILES_$(1):$(XCHG_OBJ_EXT)=.d)
XCHG_PREPROCESSED_FILES_$(1) := $(addsuffix .pp,$(basename $(XCHG_SRC_FILES_$(1))))
XCHG_LIB_NAME_$(1) := $(call XCHG_MAKE_LIB_NAME,$(1))

$(1) : XCHG_C_LOCAL_INCLUDE := $$(XCHG_C_LOCAL_INCLUDE_$(1))
$(1) : XCHG_C_LOCAL_DEFS := $$(XCHG_C_LOCAL_DEFS_$(1))
$$(XCHG_DEP_FILES_$(1)) : XCHG_C_LOCAL_INCLUDE := $$(XCHG_C_LOCAL_INCLUDE_$(1))
$$(XCHG_DEP_FILES_$(1)) : XCHG_C_LOCAL_DEFS := $$(XCHG_C_LOCAL_DEFS_$(1))
$$(XCHG_PREPROCESSED_FILES_$(1)) : XCHG_C_LOCAL_INCLUDE := $$(XCHG_C_LOCAL_INCLUDE_$(1))
$$(XCHG_PREPROCESSED_FILES_$(1)) : XCHG_C_LOCAL_DEFS := $$(XCHG_C_LOCAL_DEFS_$(1))

$(1) : $$(XCHG_LIB_NAME_$(1))
$$(XCHG_LIB_NAME_$(1)) : $$(XCHG_OBJ_FILES_$(1))
	$(ECHO) 'Creating Archive $$@'
	$(MKDIR) -p $$(@D)
	$(RM) -f $$@
	$(XCHG_ARCHIVE) $(XCHG_ARCHIVE_OPTS)$$@ $$^
	$(ECHO)
endef



#--------------------------------------------------------------------------
#
# Include dependencies. Note that make will generate the dependency files
# if they're missing.
#
# The '-' at the beginning tells make not to complain about the missing
# dependency file. Since there are rules to make dependencies, make will
# automatically try to build the dependencies before building any targets.
#

ifneq ($(XCHG_DEP_FILES),)
ifeq ($(strip $(filter clean% exec print-%, $(MAKECMDGOALS))),)
-include $(XCHG_DEP_FILES)
endif
endif

#--------------------------------------------------------------------------
#
# XCHG_C_xxx_INCLUDE is designed to pass include file search paths to the
#                    compiler and pre-processor.
#
# The LOCAL version is not exported and is intended only for the source
# files compiled directly from Makefile.
#
# The GLOBAL version is exported and is intended for the current Makefile
# and any sub-Makefiles.
#
# XCHG_INC_PATH has been replaced by XCHG_C_GLOBAL_INCLUDE
#

unexport XCHG_C_LOCAL_INCLUDE    # Don't pass this down to sub-Makefiles
export   XCHG_C_GLOBAL_INCLUDE   # Pass this one down to sub-Makefiles

export   XCHG_INC_PATH           # deprecated - Use XCHG_C_GLOBAL_INCLUDE

#--------------------------------------------------------------------------
#
# XCHG_C_xxx_DEFS is designed to pass pre-processor definitions to the
#                 compiler and the pre-processor.
#
# See above for GLOBAL versus LOCAL.
#

unexport XCHG_C_LOCAL_DEFS       # Don't pass this down to sub-Makefiles
export   XCHG_C_GLOBAL_DEFS      # Pass this one down to sub-Makefiles
export   XCHG_ASM_GLOBAL_DEFS

XCHG_C_GLOBAL_DEFS += XCHG_ARCH_$(XCHG_ARCH)=1

#--------------------------------------------------------------------------
#
# XCHG_C_xxx_COMPILER_OPTS is designed to pass additional compiler options
#                          into the C compiler.
#
# See above for GLOBAL versus LOCAL.
#

unexport XCHG_C_LOCAL_COMPILER_OPTS
export   XCHG_C_GLOBAL_COMPILER_OPTS

#--------------------------------------------------------------------------
#
# The FORCE target can be used for things that you want to have rebuild
# every single time (like *.pp files)
#

FORCE:


#--------------------------------------------------------------------------
#
# Print out a bunch of useful information
#
# Note that print-include is provided within the OS specific rules file.
#

.PHONY: print-info print-cfg print-include print-path

print-info: print-cfg print-include print-path

print-cfg:
	 @$(ECHO)
	 @$(ECHO) 'XCHG_OS        = $(XCHG_OS)'
	 @$(ECHO) 'XCHG_ROOT      = $(XCHG_ROOT)'
	 @$(ECHO) 'XCHG_OBJ_DIR   = $(XCHG_OBJ_DIR)'
	 @$(ECHO) 'XCHG_LIB_DIR   = $(XCHG_LIB_DIR)'
	 @$(ECHO) 'XCHG_EXE_DIR   = $(XCHG_EXE_DIR)'
	 @$(ECHO) 'XCHG_DLL_DIR   = $(XCHG_DLL_DIR)'
	 @$(ECHO) 'XCHG_RULES_DIR = $(XCHG_RULES_DIR)'
	 @$(ECHO)

print-path:
	 @$(ECHO)
	 @$(ECHO) -e 'PATH =\n  $(subst ;,\n  ,$(PATH))'
	 @$(ECHO)

print-src:
	 @$(ECHO) 'XCHG_SRC_FILES     = $(XCHG_SRC_FILES)'

print-obj:
	 @$(ECHO) 'XCHG_OBJ_FILES = $(XCHG_OBJ_FILES)'

print-dep:
	 @$(ECHO) 'XCHG_DEP_FILES = $(XCHG_DEP_FILES)'

#--------------------------------------------------------------------------
#
# You can run arbitrary commands from within the context that is setup by
# the makefile by using:
#
#  make exec="command to execute"
#

exec:
	@$(ECHO) 'About to execute '$(exec)' '
	$(exec)

clean: clean_obj clean_dep clean_lib clean_exe clean_dll

clean_dep:
clean_exe:
clean_lib:
clean_obj:

clean_all:
	@$(ECHO) 'Removing all files from $(XCHG_OBJ_DIR)'
	$(RM) -f $(XCHG_OBJ_DIR)/*.*
	-$(TEST) -d $(XCHG_OBJ_DIR) && $(RM) -R $(XCHG_OBJ_DIR)
	@$(ECHO) 'Removing all files from $(XCHG_LIB_DIR)'
	$(RM) -f $(XCHG_LIB_DIR)/*.*
	-$(TEST) -d $(XCHG_LIB_DIR) && $(RM) -R $(XCHG_LIB_DIR)
	@$(ECHO) 'Removing all files from $(XCHG_EXE_DIR)'
	$(RM) -f $(XCHG_EXE_DIR)/*
	-$(TEST) -d $(XCHG_EXE_DIR) && $(RM) -R $(XCHG_EXE_DIR)
	@$(ECHO) 'Removing all files from $(XCHG_DLL_DIR)'
	$(RM) -f $(XCHG_DLL_DIR)/*
	-$(TEST) -d $(XCHG_DLL_DIR) && $(RM) -R $(XCHG_DLL_DIR)


ifneq ($(strip $(XCHG_DEP_FILES)),)
clean_dep: xChg_clean_dep
xChg_clean_dep:
	@$(ECHO) 'Removing dependencies'
	$(RM) -f $(XCHG_DEP_FILES)
	
 
.PHONY: xChg_clean_dep
endif

ifneq ($(strip $(XCHG_OBJ_FILES)),)
clean_obj: xChg_clean_obj
xChg_clean_obj:
	@$(ECHO) 'Removing objects'
	$(RM) -f $(XCHG_OBJ_FILES)
	
 
.PHONY: xChg_clean_obj
endif

ifneq ($(XCHG_LIB_BASE),)
clean_lib: xChg_clean_lib
xChg_clean_lib:
	@$(ECHO) 'Removing libraries'
#
# In the non-recursive make model, the build environment doesn't have
# a list of the libraries that are to be made. Therefore, there is no way to
# selectively delete only these libraries. I don't want to remove *.a because
# there could be libraries from many VOBs in the same directory. 'clean' will
# will still nuke all the objs, which will cause the libraries to be recreated
# the next time the project is built, so this is not a big deal. If users
# really want to delete everything, they can use the 'clean_all' target.
#
#   $(RM) -f $(XCHG_LIB_DIR)/*.*
 
.PHONY: xChg_clean_lib
endif


.PHONY: clean clean_all clean_obj clean_dep clean_lib clean_exe clean_dll
