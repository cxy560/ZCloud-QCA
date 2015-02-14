# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
# $ATH_LICENSE_TARGET_MK$
#
# Set TARGET in order to restrict the built to a single target type.
# Example: make TARGET=QCA4004
# If you don't specify a target, actions are taken for ALL_TARGETS
ALL_TARGETS = QCA4004

ifndef TARGET # {
TARGET := QCA4004
endif # }

ifndef FLAVOR # {
FLAVOR := SP143
endif # }

ifndef PDIR # {
TARGET_TOP := 1
endif # }

TOP_DIR := $(KF_SCS_DIR)
SDK_DIR := $(KF_SCS_DIR)/sdk

# Custerom Specific Defines
sinclude $(TOP_DIR)/make.users

OUTPUTDIR := .output
ODIR := $(OUTPUTDIR)/$(TARGET)/$(FLAVOR)
OBJODIR := $(ODIR)/obj
LIBODIR := $(ODIR)/lib
IMAGEODIR := $(ODIR)/image
BINODIR := $(ODIR)/bin
EXTRACT_DIR := $(ODIR)/.extract
UP_EXTRACT_DIR := ../../../..

#############################################################
# Required variables for each makefile
# Discard this section from all parent makefiles
# Expected variables (with automatic defaults):
#   CSRCS (all "C" files in the dir)
#   SUBDIRS (all subdirs with a Makefile)
#   GEN_LIBS - list of libs to be generated ()
#   GEN_IMAGES - list of object file images to be generated ()
#   GEN_BINS - list of binaries to be generated ()
#   COMPONENTS_xxx - a list of libs/objs in the form
#     subdir/lib to be extracted and rolled up into
#     a generated lib/image xxx.a ()
#
ifdef TARGET_USER # {
# TARGET_USER }
else # { TARGET_USER
ifdef TARGET_TOP # {

TARGET_TOP := 1

# Map target source name to directory
dir_sdk_shell=demos/sdk_shell

QCA4004_DEMO_RAM_IMAGES = \
	sdk_shell.out

QCA4004_DEMO_SUBDIRS = \
	$(dir_sdk_shell)

SUBDIRS := $(QCA4004_DEMO_SUBDIRS) $(QCA4004_USER_SUBDIRS)
GEN_IMAGES := $(QCA4004_DEMO_RAM_IMAGES) $(QCA4004_USER_RAM_IMAGES)
GEN_BINS := $(QCA4004_DEMO_RAM_BINS) $(QCA4004_USER_RAM_BINS)


# =====QCA4004 sdk_shell.out
COMPONENTS_sdk_shell =                    \
    $(dir_sdk_shell)/libsdk_shell.a\
    $(dir_sdk_shell)/aes/libsdk_aes.a\
    $(dir_sdk_shell)/ZC/libsdk_crc.a\
    $(dir_sdk_shell)/ZC/src/tropicssl/libsdk_ssl.a\
    $(dir_sdk_shell)/ZC/src/zc/libsdk_zc.a\

# Extra dependencies that are not handled by general rules
DEPENDS_sdk_shell =                   \
	./rom.addrs.ld		\
	$(dir_sdk_shell)/hostless.target.ld \
	./lib/libsdk.a

# sdk_flash link options
LINKFLAGS_sdk_shell =                 \
	$(TARGET_LDFLAGS)                   \
	-L$(SDK_DIR)                         \
	-L$(SDK_DIR)/image                   \
	-T$(dir_sdk_shell)/hostless.target.ld     \
	-u main_entry                        \
	-Wl,-static                         \
	-Wl,--start-group                   \
	-lgcc                               \
	./lib/libsdk.a			\
	./lib/libsniffer.a  \
	$(DEP_LIBS_sdk_shell)             \
	$(DEP_OBJS_sdk_shell)             \
	-Wl,--end-group


endif # TARGET_TOP }
endif # TARGET_USER }

#############################################################
# Configuration i.e. compile options etc.
# Generally values applying to a tree are captured in the
#   makefile at its root level - these are then overridden
#   for a subtree within the makefile rooted therein
#

AR = xt-ar
CC = xt-xcc
NM = xt-nm
CPP = xt-cpp
OBJCOPY = xt-objcopy
# AR6002 linker flags
TARGET_LDFLAGS =		\
	-g			\
	-nostdlib		\
	-Wl,-EL   \
	-Wl,--gc-sections

# Use -DAR6K for the generic common architecture,
#     -DAR6002 for Mercury-specific
#     -DFPGA for FPGA systems
#PLAT_DEFS = -DAR6K -DAR6002 $(FPGA_DEF)
PLAT_DEFS += -TARG:unaligned_loads=1

RAM_BUILD_OPTS += -ffunction-sections
RAM_BUILD_OPTS += -fdata-sections

#PLAT_DEFS += -DTX_TEMPERATURE_THROTTLE
# TBDXXX: RB-2008.3 version of Tensilica tools needs -Wno-return-type in
# order to avoid bogus warnings.
PLAT_DEFS += -Wno-return-type

# Build options used for all RAM code (AR6002 REV4)
RAM_BUILD_OPTS += -mlongcalls

PLAT_DEFS += -DAR6002_REV7
#PLAT_DEFS += -DAR6002_REV71
PLAT_DEFS += -DOTA_DEMO_EN
PLAT_DEFS += -DZC_MODULE_VERSION
PLAT_DEFS += -DZC_MODULE_TYPE
#PLAT_DEFS += -DIOT_BUILD_FLAG
#PLAT_DEFS += -DAR6002_REV7 -include fwconfig_AR6006.h

RAM_BUILD_OPTS += -mlongcalls

# TBDXXX: Eventually remove -g3 from COPTS
COPTS += 			\
 	-g3			\
	-Os			\
	-Wall			\
	-Wpointer-arith		\
	-Wundef			\
	-Werror			\
	-Wl,-EL			\
	-fno-inline-functions	\
	-nostdlib		\
	$(BUILD_OPTS)

DOPTS +=			\
 	-g3			\
	-Wall			\
	-Wpointer-arith		\
	-Wundef			\
	-Werror			\
	-Wl,-EL			\
	-nostdlib		\
	$(BUILD_OPTS)

UNIVERSAL_TARGET_DEFINES =		\
	-DATHOS				\
	-DATH_TARGET

CONFIGURATION_DEFINES =


DEFINES +=				\
	$(PLAT_DEFS)			\
	$(UNIVERSAL_TARGET_DEFINES)	\
	$(CONFIGURATION_DEFINES)

DDEFINES +=				\
	$(PLAT_DEFS)			\
	$(UNIVERSAL_TARGET_DEFINES)	\
	$(CONFIGURATION_DEFINES)

default: all

#############################################################
# Recursion Magic - Don't touch this!!
#
# Each subtree potentially has an include directory
#   corresponding to the common APIs applicable to modules
#   rooted at that subtree. Accordingly, the INCLUDE PATH
#   of a module can only contain the include directories up
#   its parent path, and not its siblings
#
# Required for each makefile to inherit from the parent
#

#INCLUDES := -I$(ROMDIR)/include -I$(ROMDIR)/include/whal/internal $(INCLUDES) -I$(PDIR)include -I../$(PDIR)include -I../$(PDIR)include/$(TARGET) -I../$(PDIR)include/$(HW_HDRS)
#PDIR := ../$(PDIR)
#sinclude $(PDIR)Makefile
INCLUDES := -I$(SDK_DIR)/include -I$(TOP_DIR)/src/include -I. $(INCLUDES)

ifeq ($(THREADX_EN), 1)
INCLUDES += -DTHREADX_ENABLED -DQCA
endif

ifeq ($(SSL_EN), 1)
INCLUDES += -DSSL_EN
endif

ifeq ($(ALLJOYN_EN), 1)
#INCLUDES += -DNDEBUG -DMIN_ALLJOYN_LIB -DAJ_SOCKET_IPC
INCLUDES += -DAJ_SOCKET_IPC
endif

sinclude $(TOP_DIR)/make.rules
# END Recursion Magic
#############################################################

ifdef TARGET_TOP # {
# Things under TARGET_TOP are only available to the top-level Target Makefile.
# These rules are not seen by Makefiles in subdirectories.

# This "all" rule is used only when make is invoked
# in this top-level Target directory for AR6002.
all: $(MKDBG) ram $(OBJS) $(OLIBS) $(OIMAGES) $(OBINS) $(SPECIAL_MKTARGS)

SUPPORT_INIT_TEXT=0
SUPPORT_MAKESEG=1

ifeq ($(SUPPORT_MAKESEG), 1) # {
$(BINODIR)/%.bin: $(IMAGEODIR)/%.out
	@mkdir -p $(BINODIR)
ifeq ($(SUPPORT_INIT_TEXT), 1) # {
	sdk/support/makeseg.sh -ex_init_text -o $@ -new -begin $< -done
else # } {
	sdk/support/makeseg.sh -o $@ -new -begin $< -done
endif # }
endif # }

FORCE:

ram: FORCE
	@set -e;$(foreach D, $(SUBDIRS), $(MAKE) TARGET=$(TARGET) BUILD_OPTS="$(RAM_BUILD_OPTS)" -C $(D) _all;)

%.bin: $(BINODIR)/%.bin

else # TARGET_TOP } {
# Things here are only available to makefiles in subdirectories.
# These rules are not seen by the top-level Target Makefile.


# This rule is used when the "all" rule is invoked in subdirectories.
# Subdirectories do not know which build options to use because,
# for instance, they are unaware of whether they will end up in
# RAM or in ROM.
all: $(MKDBG)
	@echo error: Builds must be initiated from a higher directory \(not from `pwd`\)
	@exit 1

# This rule is the typical rule used by subordinate directories.
# The default/all rule prints a warning message, so this rule
# is used by "make rom" and "make ram" to recurse.
_all:	$(MKDBG) allsubdirs $(OBJS) $(OLIBS) $(OIMAGES) $(OBINS) $(SPECIAL_MKTARGS)

allsubdirs:
	@set -e; $(foreach d, $(SUBDIRS), $(MAKE) -C $(d) _all;)

endif # TARGET_TOP }

# Aid for debugging Makefile issues.
mkdbg:
	@echo pwd=`pwd`
	@echo PDIR is $(PDIR)
	@echo TARGET is $(TARGET)
	@echo AR6002_REV is $(AR6002_REV)
	@echo SUBDIRS is $(SUBDIRS)
	@echo FLAVOR is $(FLAVOR)
	@echo OUTPUTDIR is $(OUTPUTDIR)
	@echo ODIR is $(ODIR)
	@echo OBJODIR is $(OBJODIR)
	@echo OBJS is $(OBJS)
	@echo OLIBS is $(OLIBS)
	@echo OIMAGES is $(OIMAGES)
	@echo OBINS is $(OBINS)
	@echo SPECIAL_MKTARGS is $(SPECIAL_MKTARGS)
	@echo ROMDIR is $(ROMDIR)
	@echo LIBODIR is $(LIBODIR)
	@echo IMAGEODIR is $(IMAGEODIR)
	@echo BINODIR is $(BINODIR)
	@echo EXTRACT_DIR is $(EXTRACT_DIR)
	@echo UP_EXTRACT_DIR is $(UP_EXTRACT_DIR)
	@echo CSRCS is $(CSRCS)
	@echo GEN_LIBS is $(GEN_LIBS)
	@echo GEN_IMAGES is $(GEN_IMAGES)
	@echo GEN_BINS is $(GEN_BINS)
	@echo TARGET_LDFLAGS is $(TARGET_LDFLAGS)
	@echo CC is $(CC) at `which $(CC)`
	@echo COPTS is $(COPTS)
	@echo DEFINES is $(DEFINES)
	@echo HW_HDRS is $(HW_HDRS)
