###########################################################################
#
# xChgEnv.mk
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
# This file should only define variables and not have any rules in it.
#
###########################################################################

XCHG_ENV_INCLUDED := 1

#
# XCHG_ROOT describes the path to get from the directory containing the
# current directory (which is usually the directory containing the makefile
# which included this one), to the xchg_common directory (VOB).
#
ifeq ($(XCHG_ROOT),)
$(error "XCHG_ROOT variable hasn't been set")
endif

#
# XCHG_OS describes the Operating system which is the source code is
# currently being compiled for. For each supported OS, there is another
# rules file in the /xchg_common/rules directory which is named using
# xChgRules_$(XCHG_OS).mk
#

XCHG_SUPPORTED_OS := eCos LinuxKernel LinuxUser None pSOS VxWorks Win32 Zsp

# UNFINISHED: GNU MIPS isn't really an operating system type.
XCHG_SUPPORTED_OS += gnu_mips


ifeq ($(filter $(XCHG_OS),$(XCHG_SUPPORTED_OS)),)
$(warning Unsupported value for XCHG_OS: '$(XCHG_OS)')
$(warning XCHG_OS must be one of the following:)
$(warning $(space))
$(foreach os,$(XCHG_SUPPORTED_OS),$(warning $(space)  $(os)))
$(warning $(space))
$(warning Please note that these are case sensitive)
$(warning $(space))
$(error Run 'make XCHG_OS=DesiredOS' to correct)
endif


#
# XCHG_ARCH describes the target architecture type which the source code is
# currently being compiled for.
#
# XCHG_ARCH may optionally be specified by the project makefile. If it is not,
# a reasonable default will be assumed. This will provide backwards compatibility
# with legacy project makefiles that do not specify the target architecture.
#
# Set the architecture based upon the deprecated variable XCHG_LINUX_ARCH. 
ifneq ($(XCHG_LINUX_ARCH),)
export XCHG_ARCH := $(XCHG_LINUX_ARCH)
endif

# If no architecture was specified, choose a reasonable default.
ifeq ($(XCHG_ARCH),)

# If the target OS is Win32, select an Intel architecture by default. 
ifeq ($(XCHG_OS),Win32)
export XCHG_ARCH := i386
else

# The ZSP operating system is a misnomer, but if it's selected, then default
# the architecture type to ZSP as well. Otherwise, the default architecture
# for all other operating systems is MIPS (for backwards compatibility).
# If ARM or some other architecture is required, then this needs to be
# explicitly specified in the project makefile.
ifeq ($(XCHG_OS),Zsp)
export XCHG_ARCH := ZSP
else
export XCHG_ARCH := MIPS
endif
endif

endif

# Verify that a valid architecture was selected.
XCHG_SUPPORTED_ARCH = MIPS ARM i386 ZSP NATIVE
ifeq ($(filter $(XCHG_ARCH),$(XCHG_SUPPORTED_ARCH)),)
$(warning Unsupported value for XCHG_ARCH: '$(XCHG_ARCH)')
$(warning XCHG_ARCH must be one of the following:)
$(warning $(space))
$(foreach arch,$(XCHG_SUPPORTED_ARCH),$(warning $(space)  $(arch)))
$(warning $(space))
$(warning Please note that these are case sensitive)
$(warning $(space))
$(error Run 'make XCHG_ARCH=DesiredArch' to correct)
endif



#
# Based upon the selected target operating system and architecture, select
# the appropriate set of makefiles to compile the source code. These makefiles
# are specific to the operating system, architecture, and toolchain.
#
# For backwards compatilibity, use the operating system name.
export XCHG_ENV := $(XCHG_OS)

# The current exception is the GNU-MIPS-ELF toolchain, which is not associated
# with any operating system.
ifeq ($(XCHG_OS),None)
ifeq ($(XCHG_ARCH),MIPS)
XCHG_ENV := gnu_mips_elf
endif
endif




#
# XCHG_HOST_TYPE is the host processor and operating system combination used
# used to cross-compile for the specified target operating system. (XCHG_OS 
# specifies the target operating system).
#
# If not specified, the default host-type is i386-Win32 for backwards 
# compatibility.
#

ifeq ($(XCHG_HOST_TYPE),)
XCHG_HOST_TYPE := i386-win32
endif

XCHG_SUPPORTED_HOST_TYPES := i386-win32 i386-linux

ifeq ($(filter $(XCHG_HOST_TYPE),$(XCHG_SUPPORTED_HOST_TYPES)),)
$(warning Unsupported value for XCHG_HOST_TYPE: '$(XCHG_HOST_TYPE)')
$(warning XCHG_HOST_TYPE must be one of the following:)
$(warning $(space))
$(foreach host,$(XCHG_SUPPORTED_HOST_TYPES),$(warning $(space)  $(host)))
$(warning $(space))
$(warning Please note that these are case sensitive)
$(error $(space))
endif



#
# CYGPATH is used to translate between cygwin and windows versions of paths.
#
# We can't use XCHG_TOOLS_BIN_DIR because we need it to be absolute, and we
# need CYGPATH to make XCHG_ROOT (and hence XCHG_TOOLS_BIN_DIR) absolute.
#
# PATH_REL converts all of it's arguments to mixed mode style paths which are usable
# by unix and windows. It is designed to be used with the $(call ) function
#
# ABS_PATH is similar but return absolute paths.
#
#
ifeq ($(findstring win32,$(XCHG_HOST_TYPE)),win32)
# Win32 host.
CYGPATH := $(XCHG_ROOT)/tools/bin/$(XCHG_HOST_TYPE)/cygpath
else
# For Linux, use the cygpath emulator script
CYGPATH := $(XCHG_ROOT)/tools/bin/$(XCHG_HOST_TYPE)/cygpath.sh
endif

ifeq ($(findstring win32,$(XCHG_HOST_TYPE)),win32)
# Win32 host.
PATH_REL   = $(foreach file,$(1),$(shell  $(CYGPATH) -m    $(file)))
ABS_PATH   = $(foreach file,$(1),$(shell  $(CYGPATH) -m -a $(file)))
else
# Linux host.
PATH_REL   = $(1)
ABS_PATH   = $(1)
endif



#
# XCHG_BACK_TO_FWD_SLASH is used to convert all back-slashes in it's arguments
# to forward-slashes. i.e. Windows style paths will be converted to a mixed
# UNIX and Windows style path, e.g. c:/foo/bar. Arguments that are already in
# mixed or UNIX format will stay in those respective formats. This macro is
# is much more efficient than the xxx_PATH and xxx_ABS_PATH macros above since
# it doesn't call cygpath, it simply uses the built-in make 'subst' command.
# If you need to convert a relative path into an absolute path, you must use
# one of the xxx_ABS_PATH macros.
#
XCHG_BACK_TO_FWD_SLASH = $(foreach file,$(1),$(subst \,/,$(file)))


#
# Export XCHG_ROOT as an absolute path so that we can reference it from
# other makefiles in other directories and still have everything work out OK.
#
# We only need to convert XCHG_ROOT to an absolute path once, this is more
# efficient than calling cygpath to do the conversion for every module that
# includes this makefile.
#
ifeq ($(XCHG_ROOT_CONVERTED_TO_ABS_PATH),)
export XCHG_ROOT_CONVERTED_TO_ABS_PATH := TRUE
export XCHG_ROOT := $(shell ${CYGPATH} -m -a $(XCHG_ROOT))
endif


#
# XCHG_INC_PATH specifies a list of application directories to look in for
# the header files.
#
# Makefiles should add to XCHG_INC_PATH by using the += syntax.
#

#
# XCHG_SRC_FILES is list of the source file names.
#

#
# XCHG_RULES_DIR describes the location of the directory containing the OS
# specific rules files.
#
# If the default directory is not used, XCHG_RULES_DIR should be specified as an
# absolute path.
#
ifeq ($(XCHG_RULES_DIR),)
export XCHG_RULES_DIR := $(XCHG_ROOT)/rules
else
export XCHG_RULES_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_RULES_DIR))
endif


#
#
# Get OS specific environment makefile.
#
# This must be included after the export of XCHG_RULES_DIR, and before the
# export of output directories below, since the output directories use
# XCHG_RELEASE_EXT which is defined in the OS specified environment file.
#

ifeq ($(XCHG_OS_ENV_FILE),)
XCHG_OS_ENV_FILE := $(XCHG_RULES_DIR)/xChgEnv_$(XCHG_ENV).mk
endif
include $(XCHG_OS_ENV_FILE)


#
# XCHG_INITIAL_DIR is set to the first directory that this makefile is included
# from. This is used to set default output directories, e.g. for the default
# obj, lib, exe directories.
#
# We only need to set XCHG_INITIAL_DIR once, this is more efficient than calling
# cygpath everytime a module includes this makefile.
#
ifeq ($(XCHG_INITIAL_DIR_SET),)
export XCHG_INITIAL_DIR_SET := TRUE
export XCHG_INITIAL_DIR := $(shell ${CYGPATH} -m -a $(CURDIR))
endif


#
# XCHG_OBJ_DIR describes the location where the generated object and
# dependancy files will be placed. 
#
# If the default directory is not used, XCHG_OBJ_DIR should be specified as an
# absolute path.
#
ifeq ($(XCHG_OBJ_DIR),)
export XCHG_OBJ_DIR := $(XCHG_INITIAL_DIR)/obj/$(XCHG_ENV)$(XCHG_RELEASE_EXT)
else
export XCHG_OBJ_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_OBJ_DIR))
endif

#
# XCHG_LIB_DIR describes the location where generated libraries files will
# be placed.
#
# If the default directory is not used, XCHG_LIB_DIR should be specified as an
# absolute path.
#
ifeq ($(XCHG_LIB_DIR),)
export XCHG_LIB_DIR := $(XCHG_INITIAL_DIR)/lib/$(XCHG_ENV)$(XCHG_RELEASE_EXT)
else
export XCHG_LIB_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_LIB_DIR))
endif

#
# XCHG_EXE_DIR describes the location where the generated executable files
# will be placed.
#
# If the default directory is not used, XCHG_EXE_DIR should be specified as an
# absolute path.
#
ifeq ($(XCHG_EXE_DIR),)
export XCHG_EXE_DIR := $(XCHG_INITIAL_DIR)/exe/$(XCHG_ENV)$(XCHG_RELEASE_EXT)
else
export XCHG_EXE_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_EXE_DIR))
endif

#
# XCHG_DLL_DIR describes the location where the generated DLL files
# will be placed.
#
# If the default directory is not used, XCHG_DLL_DIR should be specified as an
# absolute path.
#
ifeq ($(XCHG_DLL_DIR),)
export XCHG_DLL_DIR := $(XCHG_INITIAL_DIR)/dll/$(XCHG_ENV)$(XCHG_RELEASE_EXT)
else
export XCHG_DLL_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_DLL_DIR))
endif

#
# XCHG_TOOLS_BIN_DIR describes the location containing tools that are used by
# these rules.
#
# If the default directory is not used, XCHG_TOOLS_BIN_DIR should be specified as an
# absolute path.
#
ifeq ($(XCHG_TOOLS_BIN_DIR),)
export XCHG_TOOLS_BIN_DIR := $(XCHG_ROOT)/tools/bin/$(XCHG_HOST_TYPE)
else
export XCHG_TOOLS_BIN_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_TOOLS_BIN_DIR))
endif


#--------------------------------------------------------------------------
#
# Define space to be a single space character. Using " " gets the quotes
# as well, which we don't want.
#

empty :=
space := $(empty) $(empty)


#
# Eliminate use of the built-in implicit rules. This is done for efficiency 
# reasons since implicit rules aren't typically used. Set 
# XCHG_ENABLE_IMPLICIT_MAKE_RULES=1 to override the default behaviour and
# enable implicit make rules.
#
ifneq ($(XCHG_ENABLE_IMPLICIT_MAKE_RULES),1)
MAKEFLAGS += -r
endif


#
# Run make with verbose=1 to get verbose output
#
ifeq ($(verbose),false)
export verbose := 0
else
export verbose := 1
endif

ifeq ($(verbose),false)
MAKEFLAGS += -s
endif


#
# Run make with debug=1 to get debug versions generated.
#

ifeq ($(XCHG_OPTIMIZE),)

ifeq ($(debug),)
export debug := 1
endif

ifeq ($(debug),1)
XCHG_OPTIMIZE    := 0
else
XCHG_OPTIMIZE    := 1
endif

endif

# XCHG_ADD_SYMBOLS := 1


#--------------------------------------------------------------------------
#
# The XCHG_MAKE_LIB_NAME macro can be used to construct the name of a library
# by providing the basename of the library (or libraries)
#
# For example:
#
#  STR_LIB_NAME = $(call XCHG_MAKE_LIB_NAME,str)
#
# will assign STR_LIB_NAME with the full version of the 'str' library.
#

XCHG_MAKE_LIB_NAME = $(foreach libBase,$(1),$(XCHG_LIB_DIR)/$(XCHG_LIB_PREFIX)$(libBase)$(XCHG_LIB_EXT))

#--------------------------------------------------------------------------
#
# The XCHG_MAKE_xxx_NAME macro can be used to construct the name of an xxx
#
#

XCHG_MAKE_EXE_EXT_NAME = $(foreach exeBase,$(1),$(XCHG_EXE_DIR)/$(exeBase)$(strip $(2)))
XCHG_MAKE_OBJ_EXT_NAME = $(foreach objBase,$(1),$(XCHG_OBJ_DIR)/$(objBase)$(strip $(2)))
XCHG_MAKE_DLL_EXT_NAME = $(foreach dllBase,$(1),$(XCHG_DLL_DIR)/$(dllBase)$(strip $(2)))

XCHG_MAKE_EXE_NAME = $(call XCHG_MAKE_EXE_EXT_NAME,$(1),$(XCHG_EXE_EXT))
XCHG_MAKE_OBJ_NAME = $(call XCHG_MAKE_OBJ_EXT_NAME,$(1),$(XCHG_OBJ_EXT))
XCHG_MAKE_DLL_NAME = $(call XCHG_MAKE_DLL_EXT_NAME,$(1),$(XCHG_DLL_EXT))


#--------------------------------------------------------------------------
#
# Setup variables for the various utilities that are required.
#

#
# General utility info
# --------------------
# The absolute-path for utilities should be specified (not just the name of the
# utility). This guarantees that the correct tool is used by make. Relying on
# the path doesn't always work; this is especially true when using a native
# win32 make.exe. For native win32 make, if only the name of the utility is 
# specified, make will look in the following directories, in the order shown:
#
#     1) The directory from which the application loaded.
#     2) The current directory.
#     3) The 32-bit Windows system directory.
#     4) The 16-bit Windows system directory.
#     5) The Windows directory.
#     6) The directories that are listed in the PATH environment variable.
#
# Note that the PATH is the _last_ place where make looks for the utility. If
# multiple build environments are used, each using the same set of tools (but
# potentially different versions), it is fairly easy for the wrong set of tools
# to be used since make will look in the directory where the application (make)
# was loaded first.
#
# For cygwin based make.exe's (and presumably linux based make.exe's), make
# will not use the search order above - it finds the utilitiy in the path.
#
# So, using an absolute path also provide consistency between native win32
# and cygwin based versions of make.exe.
#
# Ideally, there should be two sets of utilities in the xchg_common bin
# folder, one for win32 and one for linux, since these are the two environments
# that we currently build under. The same version of tools should be provided
# for win32 and linux. Currently, for linux, we rely on the built-in tools,
# which are assumed to be compatible with the version of cygwin win32 tools 
# that are distributed with xchg_common.
#
#
# Echo
# ----
#
# There is an issue with using double-quotes(") in echo statements with the
# native win32 make.exe that I don't completely understand...basically, you
# end-up with unsolicted back-slashes at the end of the echo'ed line.
#
# e.g. $(ECHO) "Hello, world!", would result in:
#
# Hello,world!\
#
# Using double-quotes in echo states is useful because it preserves white-space
# and also allows special characters to be used, e.g. '|' which would otherwise
# be interpreted as a pipe by the shell. However, using double-quotes causes
# make to invoke echo.exe via a shell; whereas, if the quotes are omitted,
# echo.exe is invoked directly. The work-around is to use single quotes (')
# instead of double quotes.
#
# This cygwin version of make.exe appears to invoke echo.exe directly even if
# double-quotes are present. This is why is problem appeared after switching 
# to a native win32 make.exe.
#
ifeq ($(findstring win32,$(XCHG_HOST_TYPE)),win32)
CAT      := $(XCHG_TOOLS_BIN_DIR)/cat
CP       := $(XCHG_TOOLS_BIN_DIR)/cp
CPP      := $(XCHG_TOOLS_BIN_DIR)/cpp
ECHO     := $(XCHG_TOOLS_BIN_DIR)/echo
GREP     := $(XCHG_TOOLS_BIN_DIR)/grep
MKDIR    := $(XCHG_TOOLS_BIN_DIR)/mkdir
MV       := $(XCHG_TOOLS_BIN_DIR)/mv
RM       := $(XCHG_TOOLS_BIN_DIR)/rm
RMDIR    := $(XCHG_TOOLS_BIN_DIR)/rmdir
SED      := $(XCHG_TOOLS_BIN_DIR)/sed
TEST     := $(XCHG_TOOLS_BIN_DIR)/test
TR       := $(XCHG_TOOLS_BIN_DIR)/tr
else
CAT      := cat
CP       := cp
CPP      := cpp
ECHO     := echo
GREP     := grep
MKDIR    := mkdir
MV       := mv
RM       := rm
RMDIR    := rmdir
SED      := sed
TEST     := test
TR       := tr
endif



#--------------------------------------------------------------------------
#
# Setup variables for TCL
#

#
# If the default directory is not used, XCHG_TCL_HOME_DIR should be specified as
# an absolute path.
#
ifeq ($(XCHG_TCL_HOME_DIR),)
export XCHG_TCL_HOME_DIR := $(XCHG_ROOT)/../zOEMTools_TCL
else
export XCHG_TCL_HOME_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_TCL_HOME_DIR))
endif

ifeq ($(XCHG_TCL_INCLUDE),)
export XCHG_TCL_INCLUDE := $(XCHG_TCL_HOME_DIR)/include
endif

ifeq ($(XCHG_TCL_LIB),)
export XCHG_TCL_LIB := $(XCHG_TCL_HOME_DIR)/lib/tcl83.lib
endif

#--------------------------------------------------------------------------
#
# Setup variables for SWIG
#

#
# If the default directory is not used, XCHG_SWIG_HOME_DIR should be specified as
# an absolute path.
#
ifeq ($(XCHG_SWIG_HOME_DIR),)
export XCHG_SWIG_HOME_DIR := $(XCHG_ROOT)/../zOEMTools_misc/swig
else
export XCHG_SWIG_HOME_DIR := $(call XCHG_BACK_TO_FWD_SLASH,$(XCHG_SWIG_HOME_DIR))
endif

ifeq ($(SWIG),)
SWIG     := $(XCHG_SWIG_HOME_DIR)/swig
endif
