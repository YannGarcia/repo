
########################################################################
#
#       Makefile for TI TIVA C Series TM4C1294 HAL (Hardware Abstract Level) support support library
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc
include ../../Makefile.inc

#############################################################################
# Libraries and options.

CC_INS=
LD_INS=

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS)
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS)

# Linker flags
LD_DEBUG_OPTIONS=-T$(SCRIPTDIRS)$(TARGET).ld $(GLOBAL_DEF_STD_GLD)
LD_RELEASE_OPTIONS=-T$(SCRIPTDIRS)$(TARGET).ld $(GLOBAL_DEF_STD_GLD)

#############################################################################
# Module variables.

# Paths
OBJSDIR=../objs
OBJSDIRS=../objs/
SRCDIRS=../src/
SCRIPTDIRS=../ld/
INCDIR=../include
INCDIRS=../include/
BINDIR= ../bin
BINDIRS= ../bin/
DOCDIR=../docs
DOCDIRS=../docs/
DOCCONF=$(DOCDIR)/o2.cfg

# Needed to build application.
LIBRARY=libhal

# Targets
TARGET=$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)$(VERSION)
TARGETP=$(LIBRARY)++$(SHARED_LIBRARY_EXT_NAME)$(VERSION)
TARGET_D=$(LIBRARY)_d$(SHARED_LIBRARY_EXT_NAME)$(VERSION)
LIB=$(LIBDIRS)$(TARGET)
LIBP=$(LIBDIRS)$(TARGETP)
LIB_D=$(LIBDIRS)$(TARGET_D)
TEST_TARGET=testlib
BIN=$(TESTDIRS)$(TEST_TARGET)

MODULE_LIBS=$(LIB) $(LIBP)
MODULE_LIBS_D=$(LIB) 
MODULE_TEST=$(BIN)

# Object app
OBJS= \
	$(STARTUP_GCC_OBJ) \
	$(LIBHAL_OBJ)
	$(TEST_OBJ)

TEST_OBJ=blinky.o
LIBHAL_OBJ=libhal.c

#############################################################################
# Real compilation sections.

all: directories $(TARGET)
	@echo "*** $@ done ***"

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o$(BIN).axf $(LD_OPTION)
	$(OBJCOPY) -O binary $(BIN).axf $(BIN).bin
	@echo "*** $@ done ***"

$(TEST_OBJ): $(SRCDIRS)$(TEST_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(LIBHAL_OBJ): $(SRCDIRS)$(LIBHAL_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHAL_OBJ)

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
	@$(RM) -f $(BIN).bin $(BIN).axf ../../$(TARGET).tar.gz $(TARGET) *.lst *.o *.d *.bin *.axf *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)\*~ $(BINDIRS)*~ $(DOCDIRS)*~
	@$(RM) -fr $(DOCDIRS)html $(DOCDIRS)latex $(DOCDIRS)man
	@echo "*** $@ done ***"

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
