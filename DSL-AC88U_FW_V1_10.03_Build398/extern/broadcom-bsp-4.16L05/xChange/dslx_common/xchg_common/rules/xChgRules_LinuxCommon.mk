###########################################################################
#
# bosRules_LinuxCommon.mk
#
# This file contains specific build rules for XCHG for both
# Linux kernel space and Linux user space.
#
###########################################################################


#
# It is assumed that the compiler is able to generate the dependency files
# during the compile process, e.g. by using the -MD compiler option with gcc.
# If this is not true, then we will need to update the compile/dependency
# rules.
#
ifeq ($(XCHG_LINUX_ARCH),MIPS)
ifeq ($(XCHG_C_COMPILE),)
XCHG_C_COMPILE  := $(XCHG_COMPILER_BIN_PATH)/mips-linux-uclibc-gcc
endif
ifeq ($(XCHG_CPP_COMPILE),)
XCHG_CPP_COMPILE  := $(XCHG_COMPILER_BIN_PATH)/mips-linux-uclibc-gcc
endif
ifeq ($(XCHG_AR),)
XCHG_AR := $(XCHG_COMPILER_BIN_PATH)/mips-linux-uclibc-ar
endif
ifeq ($(XCHG_ASSEMBLE),)
XCHG_ASSEMBLE := $(XCHG_COMPILER_BIN_PATH)/mips-linux-uclibc-gcc -x assembler-with-cpp
endif
endif

ifeq ($(XCHG_LINUX_ARCH),ARM)
ifeq ($(XCHG_C_COMPILE),)
XCHG_C_COMPILE  := arm-uclibc-gcc
endif
ifeq ($(XCHG_AR),)
XCHG_AR := arm-uclibc-ar
endif
endif

ifeq ($(XCHG_LINUX_ARCH),NATIVE)
ifeq ($(XCHG_C_COMPILE),)
XCHG_C_COMPILE  := gcc
endif
ifeq ($(XCHG_AR),)
XCHG_AR := ar
endif
endif

XCHG_ARCHIVE      := $(XCHG_AR)
XCHG_ARCHIVE_OPTS := -rcs$(space)

ifeq ($(XCHG_COMPILER_INC_PATH),)
ifeq ($(XCHG_OS),LinuxKernel)
export XCHG_COMPILER_INC_PATH = $(XCHG_LINUX_ROOT)/include $(XCHG_LINUX_ROOT)/include/asm
endif
endif

#--------------------------------------------------------------------------
# C compiler options, defines, and search paths.
#


INCLUDE_PATH     = $(XCHG_C_LOCAL_INCLUDE)                              \
						 $(XCHG_C_GLOBAL_INCLUDE)                             \
						 $(XCHG_INC_PATH)                                     \
						 $(XCHG_COMPILER_INC_PATH)


#
# XCHG_C_INCLUDE inserts -I in front of each include path, which is suitable
# for both the compiler and the preprocessor.
#
XCHG_C_INCLUDE     = $(addprefix -I ,$(strip $(INCLUDE_PATH)))


#
# XCHG_C_DEFS inserts -D in front of each define, which is suitable
# for both the compiler and the preprocessor.
#
XCHG_C_DEFS = $(addprefix -D,$(strip $(XCHG_C_GLOBAL_DEFS) $(XCHG_C_LOCAL_DEFS)))


#
# Set XCHG_ADD_SYMBOLS := 1 in the project makefile to add symbols to object
# files for _ALL_ modules. To enable symbols for a specific module only (or to
# override the project default behaviour), add the following to the module 
# makefile:
#
#     $(XCHG_LIB_BASE): XCHG_C_SYM_OPT := $(XCHG_C_SYM_OPT_ENABLE)
#
XCHG_C_SYM_OPT_ENABLE   := -g
XCHG_C_SYM_OPT_DISABLE  :=
ifeq ($(XCHG_ADD_SYMBOLS),1)
XCHG_C_SYM_OPT := $(XCHG_C_SYM_OPT_ENABLE)
else
XCHG_C_SYM_OPT := $(XCHG_C_SYM_OPT_DISABLE)
endif



#
# Set XCHG_OPTIMIZE := 1 in the project makefile to enable optimizations
# for _ALL_ modules. To enable optimizations for a specific module only (or to
# override the project default behaviour), add the following to the module 
# makefile:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT := $(XCHG_C_OPTIMIZE_OPT_ENABLE)
#
XCHG_C_OPTIMIZE_OPT_ENABLE  := -Os -fomit-frame-pointer
XCHG_C_OPTIMIZE_OPT_DISABLE :=

ifeq ($(XCHG_OPTIMIZE),1)
XCHG_C_OPTIMIZE_OPT := $(XCHG_C_OPTIMIZE_OPT_ENABLE)
else
XCHG_C_OPTIMIZE_OPT := $(XCHG_C_OPTIMIZE_OPT_DISABLE)
endif


ifeq ($(XCHG_NO_INSTRUCTION_REORDER),1)
XCHG_C_REORDER_OPT := $(XCHG_C_REORDER_OPT_DISABLE)
else
XCHG_C_REORDER_OPT := $(XCHG_C_REORDER_OPT_ENABLE)
endif

ifeq ($(XCHG_SUPPRESS_WARNINGS),1)
$(error "Need to add support for 'XCHG_SUPPRESS_WARNINGS' to Linux")
endif

ifeq ($(XCHG_NO_REDUNDANT_DECLS_WARNING),1)
$(error "Need to add support for 'XCHG_NO_REDUNDANT_DECLS_WARNING' to Linux")
endif


#
# Composite assembler options.
#
XCHG_C_COMPILER_OPTS = $(XCHG_C_COMMON_OPT) \
								$(XCHG_C_SYM_OPT) \
								$(XCHG_C_OPTIMIZE_OPT) \
								$(XCHG_C_GLOBAL_COMPILER_OPTS) \
								$(XCHG_C_LOCAL_COMPILER_OPTS) \
							  $(XCHG_C_INCLUDE) \
								$(XCHG_C_DEFS)

XCHG_CPP_COMPILER_OPTS = $(XCHG_C_COMPILER_OPTS) \
								-Wno-deprecated

XCHG_C_PREPROC_OPTS  := $(XCHG_C_INCLUDE) \
								$(XCHG_C_DEFS)


#--------------------------------------------------------------------------
# Assembly options, defines, and search paths.
#

# The assembly include path includes the C include path because assembly files
# may include C header files.
ASM_INCLUDE_PATH = $(XCHG_ASM_LOCAL_INCLUDE) $(XCHG_ASM_GLOBAL_INCLUDE) $(INCLUDE_PATH)


#
# XCHG_C_INCLUDE inserts -I in front of each include path, which is suitable
# for both the compiler and the preprocessor.
#
XCHG_ASM_INCLUDE     = $(addprefix -I ,$(strip $(ASM_INCLUDE_PATH)))


#
# XCHG_C_DEFS inserts -D in front of each define, which is suitable
# for both the compiler and the preprocessor.
#
XCHG_ASM_DEFS = $(addprefix -D ,$(strip $(XCHG_ASM_LINUX_DEFS) $(XCHG_ASM_GLOBAL_DEFS) $(XCHG_ASM_LOCAL_DEFS)))


#
# Common assembler options.
#
XCHG_ASM_COMMON_OPT := -c $(XCHG_ASM_$(XCHG_LINUX_ARCH)_OPT)

#
# Common assembler definitions.
#
XCHG_ASM_LINUX_DEFS :=


#
#
#
XCHG_ASM_SYM_OPT_ENABLE    := -g
XCHG_ASM_SYM_OPT_DISABLE   :=

ifeq ($(XCHG_ADD_SYMBOLS),1)
XCHG_ASM_SYM_OPT := $(XCHG_ASM_SYM_OPT_ENABLE)
else
XCHG_ASM_SYM_OPT := $(XCHG_ASM_SYM_OPT_DISABLE)
endif

#
# Composite assembler options.
#
XCHG_ASSEMBLE_OPTS    = $(XCHG_ASM_COMMON_OPT) \
                        $(XCHG_ASM_SYM_OPT) \
                        $(XCHG_ASM_GLOBAL_COMPILER_OPTS) \
                        $(XCHG_ASM_LOCAL_COMPILER_OPTS) \
                        $(XCHG_ASM_INCLUDE) \
                        $(XCHG_ASM_DEFS)



#--------------------------------------------------------------------------
#
# Compile C source files and dependencies.
#
# The basis for the following rule to build dependencies came from the GNU
# makefile manual.
#
# The sed script translates lines of the form:
#
#  foo.obj: foo.c foo.h
#
# into
#
#  obj/myproj/foo.obj obj/myproj/foo.d : foo.c foo.h
#
# This forces the dependancy file to get rebuilt if any of the dependant
# headers change.
#

# The .d and .o file are produced in a single step by using the -MD compiler
# option provided by the compiler. This is done for efficiency reasons. 
# Previously, the dependency file was produced by running the preprocessor on
# the C source file using the -M compiler option. And then, as a second step
# the source file would be compiled (which involves running the preprocessor again).
#
# Note that there are no commands for the dependency rule (since the dependency
# file is generated as part of the compile process). Admittedly, this probably
# isn't 'proper', but it is efficient. The one disadvantage is that if a
# dependency file is manually deleted, it will not be regenerated if the
# corresponding object file is up-to-date. But this is a small price to pay...
# Creating chained rules like the following:
#
#     $(XCHG_OBJ_DIR)/%.d: $(XCHG_OBJ_DIR)/%.o
#     $(XCHG_OBJ_DIR)/%.o: %.c
#        compile_commands
#
# solves this problem, but is much less efficient for incremental builds. Likewise,
# the following rule:
#
#     $(XCHG_OBJ_DIR)/%.d: $(XCHG_OBJ_DIR)/%.o: %.c
#        compile_commands
#
# doesn't work because if the dependency file is the target, the module-specific
# compiler options will not take effect because the archive is not dependent on
# on the dependency files. (Module-specific compiler options are set by using
# target specific make variables, where the target is the archive library).
$(XCHG_OBJ_DIR)/%.d: ;


#
# If the length of the target is greater than 72 characters, the dependency
# file generated by gcc will contain ' \' in the first line (and nothing else).
# When this dependency file is included, make will complain that "commands 
# commence before first target". This occurs because we are using a custom
# version of make that allows spaces as well tabs to prefix command line. 
# To work-around this issue, I have used the -MT and specified the dependency
# file (not including the path) as the target. I then run this through SED
# to add back the path portion.
#
$(XCHG_OBJ_DIR)/%.o: %.c
	$(MKDIR) -p $(@D)
	#$(ECHO) 'Preprocessing $(<F)'
	$(XCHG_C_COMPILE) $(XCHG_C_COMPILER_OPTS) -E $< -o $(@:.o=.pp)
	$(ECHO) 'Compiling $(<F)'
	$(XCHG_C_COMPILE) $(XCHG_C_COMPILER_OPTS) -MT '$(*F).d' $< -o $@
	$(SED) 's=$(*F)\.d=$(@:.o=.d)=g' $(@:.o=.d) > $(@:.o=.d).new || ( $(RM) -f $*.d; exit 1 )
	$(MV) -f $(@:.o=.d).new $(@:.o=.d)

$(XCHG_OBJ_DIR)/%.o: %.cpp
	$(ECHO) 'Compiling C++ file $(<F)'
	$(MKDIR) -p $(@D)
	$(XCHG_CPP_COMPILE) $(XCHG_CPP_COMPILER_OPTS) -MT '$(*F).d' $< -o $@
	$(SED) 's=$(*F)\.d=$(@:.o=.d)=g' $(@:.o=.d) > $(@:.o=.d).new || ( $(RM) -f $*.d; exit 1 )
	$(MV) -f $(@:.o=.d).new $(@:.o=.d)


#--------------------------------------------------------------------------
#
# Assemble .s source files and dependency.
#

#$(XCHG_OBJ_DIR)/%.d: $(XCHG_OBJ_DIR)/%.o
$(XCHG_OBJ_DIR)/%.o: %.s
	$(ECHO) 'Assembling $(<F)'
	$(MKDIR) -p $(@D)
	$(XCHG_ASSEMBLE) $(XCHG_ASSEMBLE_OPTS) -MT '$(*F).d' $< -o $@
	$(SED) 's=$(*F)\.d=$(@:.o=.d)=g' $(@:.o=.d) > $(@:.o=.d).new || ( $(RM) -f $*.d; exit 1 )
	$(MV) -f $(@:.o=.d).new $(@:.o=.d)


#--------------------------------------------------------------------------
#
# Generate Preprocessed files from C/C++ source (useful for debugging)
#

%.pp : %.c FORCE
	$(ECHO) 'Preprocessing $<'
	$(XCHG_C_COMPILE) $(XCHG_C_COMPILER_OPTS) -E $< -o $(XCHG_OBJ_DIR)/$@

%.pp : %.cpp FORCE
	$(ECHO) 'Preprocessing C++ $<'
	$(XCHG_C_COMPILE) $(XCHG_CPP_COMPILER_OPTS) -I h:/cablex/src/mta/libsip_mx/sources -DMXD_RELEASE -DMXD_OS_BOS -E $< -o $(XCHG_OBJ_DIR)/$@

#--------------------------------------------------------------------------
#
# Generate Assembly files from C/C++ source (useful for debugging)
#
# I added the :-g= to get rid of the debug information that was littering
# the assembler output.

%.cod : %.c FORCE
	$(ECHO) 'Producing assembly for $<'
	$(XCHG_C_COMPILE) $(XCHG_C_COMPILER_OPTS:-g=) -S $< -o $(XCHG_OBJ_DIR)/$@


#--------------------------------------------------------------------------
#
# Print out the include path in a nice readable form
#

print-info:
	 $(ECHO)
	 $(ECHO) 'XCHG_COMPILER_BIN_PATH = $(XCHG_COMPILER_BIN_PATH)'

print-include:
	 $(ECHO)
	 $(ECHO) -e 'INCLUDE_PATH (used by cpp) =\n  $(subst $(space),\n  ,$(strip $(INCLUDE_PATH)))'
	 $(ECHO)
	 $(ECHO) -e 'INCLUDE_PATH_DOS (used by ccmips) =\n  $(subst $(space),\n  ,$(subst \,\\\\,$(INCLUDE_PATH_DOS)))'
	 $(ECHO)

print-copts:
	$(ECHO)
	$(ECHO) -e 'XCHG_C_COMPILER_OPTS =\n  $(subst $(space),\n  ,$(subst -I ,-I,$(subst \,/,$(strip $(XCHG_C_COMPILER_OPTS)))))'
	$(ECHO)
