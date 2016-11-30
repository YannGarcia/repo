
#############################################################################
#
#       Makefile for IFSTTAR WiFi Mapping prototype / Temperature sensor processing
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc
include $(PATH_IFSTTAR_PROJECTS)/Makefile.ifsttar

#############################################################################
# Libraries and options.

CC_INS=-D$(HARDWARE_CONFIG) $(PTHREAD_INC)
LD_INS=$(WIRINGXX_LD) $(PTHREAD_LD)

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(GLOBAL_INC_CPP) $(CC_INS) $(WIFIMAPPING_COMMON_INS) $(PTHREAD_INC) 
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(GLOBAL_INC_CPP) $(CC_INS) $(WIFIMAPPING_COMMON_INS) $(PTHREAD_INC)

# Linker flags
LD_DEBUG_OPTIONS=$(GLOBAL_DEF_STD_GLD) $(LD_INS)
LD_RELEASE_OPTIONS=$(GLOBAL_DEF_STD_GLD) $(LD_INS)

# Define Include paths + definition flags
#DEFS_CC=-I$(GLOBAL_INC_CPP_PATH) -I$(INCDIRS) -I $(GLOBAL_INC_BOOST_PATH) $(DEFS_CC_RELEASE)
#DEFS_BOOST_LD=$(GLOBAL_DEF_BOOST_GLD)
DEFS_CC=
DEFS_BOOST_LD=

#############################################################################
# Module variables.

# Paths
BINDIR=../bin
BINDIRS=../bin/
OBJSDIR=../objs
OBJSDIRS=../objs/
SRCDIRS=../src/
INCDIR=../include
DOCDIR=../doc

# Targets
TARGET=tempmon
BIN=$(BINDIRS)$(TARGET)

MODULE_BIN=$(BIN)

# Object app
OBJS=\
	$(MAIN_OBJ) \
	$(MAIN_APP_OBJ) \
	$(SMMGR_OBJ)

MAIN_OBJ=tempmon.o
MAIN_APP_OBJ=mainApp.o
SMMGR_OBJ=smMgr.o

#############################################################################
# Real compilation sections.

all: directories $(TARGET) links
	@echo "*** $@ done ***"

$(TARGET): $(OBJS) # TODO Remove dependancies to WiringPI
	$(LD) $(OBJS) -o $(BIN) $(LD_OPTION) $(BEAGLEBONEHW_LD) $(BEAGLEBONECOMM_LD) $(BEAGLEBONEUTILS_LD)
	@echo "*** $@ done ***"

$(MAIN_OBJ): $(SRCDIRS)$(MAIN_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MAIN_OBJ)

$(MAIN_APP_OBJ): $(SRCDIRS)$(MAIN_APP_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MAIN_APP_OBJ)

$(TEMPIMPL_OBJ): $(SRCDIRS)$(TEMPIMPL_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEMPIMPL_OBJ)

$(SMMGR_OBJ): $(PATH_WIFIMAPPING_COMMON_SRC)/$(SMMGR_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(SMMGR_OBJ)

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all

compile_d :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_DEBUG_OPTIONS)" \
		LD_OPTION="$(LD_DEBUG_OPTIONS)" \
		all

recompile :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all

run : compile
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(BINDIRS)$(TARGET)
	@echo "*** $@ done ***"

run_d : compile_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) gdb $(BINDIRS)$(TARGET)

links:
ifeq (,$(wildcard $(HOME_BIN)/$(TARGET)))
	ln -s $(PWD)/$(BINDIRS)$(TARGET) $(HOME_BIN)
endif
	@echo "*** $@ done ***"

strip : compile
	@$(STRIP) $(BINDIRS)$(TARGET)
	@echo "*** $@ done ***"

clean:: cleanlinks
	@$(RM) -f $(BINS)$(TARGET) $(LIBDIRS)*.o ./../$(TARGET).tar.gz $(TARGET) *.lst *.o *.d *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) -f $(HOME_BIN)/$(TARGET)
	@echo "*** $@ done ***"

directories :
	@if [ ! -d $(LIBDIR) ] ; then mkdir -p $(LIBDIR) ; fi ;
	@echo "*** $@ done ***"

dist: clean
	@$(RM) ../../$(LIBRARY).tar.gz
	@cd ../.. ; \
	$(TAR) - $(LIBRARY) | $(GZIP) > $(LIBRARY).tar.gz ; \
	cd -
	@echo "*** $@ done ***"

install: strip cleanlinks
	@$(CP) $(BINDIRS)$(TARGET) $(PATH_INSTALL_BIN)
	@echo "*** $@ done ***"

depend :
	makedepend -m $(CC_OPTIONS) $(SRCDIR)/*.cpp -f$(MAKEFILE)
	@echo "*** $@ done ***"

GenDoc: $(DOCCONF)
	$(O2) $(DOCCONF)

help:
	@clear
	@echo
	@echo "make options are:"
	@echo
	@echo "   help      : This menu."
	@echo "   clean     : Erase objects, binaries and temporary files."
	@echo "   compile_d : Build a debug version."
	@echo "   compile   : Build the application in release mode."
	@echo "   recompile : Rebuild the application in release mode."
	@echo "   run       : Execute the application."
	@echo "   run_d     : Execute the application in debug mode using GDB."
	@echo "   strip     : Strip the binary file."
	@echo "   dist      : Build a compressed archive of the application."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)/xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo "   depend    : Build the file dependencies for $(MAKE)."
	@echo "   GenDoc    : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE





