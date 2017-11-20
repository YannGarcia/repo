
########################################################################
#
#       Makefile for TIVA-C LaunchPad HAL (Hardware Abstract Level) support support library
#       The same libraries exists for Raspeberry PI and MBed LPC1768 and BeagleBoneBlack
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc
include ../Makefile.inc

#############################################################################
# Override CC, CCP and LD in case of cross compiler

ifneq ($(MBED_CROSS_COMPILER_PATH), )
CC=$(MBED_CC)
CCP=$(MBED_CCP)
LD=$(MBED_LD)

GLOBAL_DEF_STD_GLD=$(MBED_GLOBAL_DEF_STD_GLD)

GLOBAL_DEF_GCC_DEBUG=$(MBED_GLOBAL_DEF_GCC_DEBUG)
GLOBAL_DEF_GCC=$(MBED_GLOBAL_DEF_GCC)

endif

#############################################################################
# Shared library version

ifneq (,$(wildcard $(SHARED_LIBRARY_VERSIONING)))
DYN_VERS_MAJ=0
DYN_VERS_MIN=1
VERSION=.$(DYN_VERS_MAJ).$(DYN_VERS_MIN)
endif

#############################################################################
# Libraries and options.

CC_INS=
LD_INS=

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH)
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH)

# Linker flags
LD_DEBUG_OPTIONS=-T$(SCRIPTDIRS)$(LIBRARY).ld $(GLOBAL_DEF_STD_GLD_DEBUG) $(LD_INS)
LD_RELEASE_OPTIONS=-T$(SCRIPTDIRS)$(LIBRARY).ld $(GLOBAL_DEF_STD_GLD) $(LD_INS)

#############################################################################
# Module variables.

# Paths
LIBDIR=../lib
LIBDIRS=../lib/
OBJSDIR=../objs
OBJSDIRS=../objs/
SRCDIRS=../src/
SCRIPTDIRS=../ld/
INCDIR=../include
INCDIRS=../include/
TESTDIRS = ../test/
DOCDIR=../docs
DOCDIRS=../docs/
DOCCONF=$(DOCDIR)/o2.cfg

# Needed to build application.
LIBRARY=libhal

# Targets
TARGET=$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)$(VERSION)
TARGET_D=$(LIBRARY)_d$(SHARED_LIBRARY_EXT_NAME)$(VERSION)
LIB=$(LIBDIRS)$(TARGET)
LIB_D=$(LIBDIRS)$(TARGET_D)
TEST_TARGET=testlib
BIN=$(TESTDIRS)$(TEST_TARGET)

MODULE_LIBS=$(LIB) 
MODULE_LIBS_D=$(LIB) 
MODULE_TEST=$(BIN)

# Object app
LIB_OBJS= \
	$(STARTUP_GCC_OBJ) \
	$(LIBHAL_OBJ) \
	$(LIBHAL_SHIFT_OBJ)

TEST_OBJ=testlib.o
LIBHAL_OBJ=libhal.o
LIBHAL_SHIFT_OBJ=libhal_shift.o

# Header files
LIBHAL_INC=libhal.h
LIBHAL_DIGITALOUT_INC=digital_out.hpp
LIBHAL_SHIFT_INC=libhal_shift.hpp
LIBHAL_FACTORY_INC=libhal_factory.hpp

INC_MODULES=LIBLIBHAL_INC LIBHAL_DIGITALOUT_INC LIBHAL_SHIFT_INC LIBHAL_FACTORY_INC

#############################################################################
# Real compilation sections.

all: directories $(MODULE_LIBS) links
	@echo "*** $@ done ***"

all_d: directories $(MODULE_LIBS_D) links
	@echo "*** $@ done ***"

$(LIB): $(LIB_OBJS)
	$(AR) -cr $(LIB) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(TEST_TARGET): $(TEST_OBJ)
#	$(LD) $(TEST_OBJ) -o$(BIN)$(SHARED_LIBRARY_EXT_NAME)xf -L$(HOME_LIB) -lhal $(LD_OPTION) -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys
	$(LD) $(TEST_OBJ) -o$(BIN)$(SHARED_LIBRARY_EXT_NAME)xf -L$(HOME_LIB) -lhal $(LD_OPTION)
	$(OBJCOPY) -O binary $(BIN)$(SHARED_LIBRARY_EXT_NAME)xf $(BIN).bin
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.c)
	$(CC) $(GLOBAL_DEF_GPP) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(LIBHAL_OBJ): $(SRCDIRS)$(LIBHAL_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHAL_OBJ)

$(LIBHAL_SHIFT_OBJ): $(SRCDIRS)$(LIBHAL_SHIFT_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHAL_SHIFT_OBJ)

$(STARTUP_GCC_OBJ): ../../$(STARTUP_GCC_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(STARTUP_GCC_OBJ)

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all

recompile :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all

rel :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all

debug :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_DEBUG_OPTIONS)" \
		LD_OPTION="$(LD_DEBUG_OPTIONS)" \
		all

test : compile
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC) $(CC_INS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS) $(LD_INS)" \
		directories $(TEST_TARGET)

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD_DEBUG) $(LD_INS)" \
		directories $(TEST_TARGET)

directories :
	@if [ ! -d $(BINDIR) ] ; then mkdir -p $(BINDIR) ; fi ;
	@echo "*** $@ done ***"

dist: clean
	@$(RM) ../../$(TARGET).tar.gz
	@cd ../.. ; \
	$(TAR) - $(TARGET) | $(GZIP) > $(TARGET).tar.gz ; \
	cd -
	@echo "*** $@ done ***"

clean:
	@$(RM) -f $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGET_D) $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME) $(BIN).bin $(BIN)$(SHARED_LIBRARY_EXT_NAME)xf ../../$(TARGET).tar.gz $(TARGET) *.lst *.o *.d *.bin *$(SHARED_LIBRARY_EXT_NAME)xf *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)\*~ $(BINDIRS)*~ $(DOCDIRS)*~
	@$(RM) -fr $(DOCDIRS)html $(DOCDIRS)latex $(DOCDIRS)man
	@echo "*** $@ done ***"

cleanlinks:
	@for header in $(INC_MODULES); do \
		$(RM) $(HOME_INC)/$$header; \
	done
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)))
		ln -s $(PWD)/$(LIB) $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHAL_INC) $(HOME_INC)/$(LIBHAL_INC)
endif

upload: compile
	$(LM4FLASH) -v $(BIN).bin
	@echo "*** $@ done ***"

depend :
	makedepend -m $(CC_OPTIONS) $(SRCDIR)/*.cpp -f$(MAKEFILE)
	@echo "*** $@ done ***"

gendoc: $(DOCCONF)
	@$(O2) $(DOCCONF)
	@echo "$(SUDO) mandb $(DOCDIRS)man"

help:
	@clear
	@echo
	@echo "make options are:"
	@echo
	@echo "   help       : This menu."
	@echo "   clean      : Erase objects, binaries and temporary files."
	@echo "   debug      : Build a debug version."
	@echo "   compile    : Build the application in release mode."
	@echo "   recompile  : Rebuild the application in release mode."
	@echo "   upload     : Flash application on device (using lm4flash)."
	@echo "   dist       : Build a compressed archive of the application."
	@echo "   depend     : Build the file dependencies for $(MAKE)."
	@echo "   gendoc     : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE
