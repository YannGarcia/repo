
########################################################################
#
#       Makefile for GPS library
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc

#############################################################################
# Shared library version

DYN_VERS_MAJ=0
DYN_VERS_MIN=1

VERSION=$(DYN_VERS_MAJ).$(DYN_VERS_MIN)

#############################################################################
# Libraries and options.

# beaglebone Utils library
BEAGLEBONEUTILS_INC=-I$(HOME_INC)
BEAGLEBONEUTILS_LD=-L$(HOME_LIB) -lbeagleboneUtils

# beaglebone Utils library
BEAGLEBONECOMM_INC=-I$(HOME_INC)
BEAGLEBONECOMM_LD=-L$(HOME_LIB) -lbeagleboneComm

CC_INS=$(WIRINGXX_INC) $(BEAGLEBONEUTILS_INC) $(BEAGLEBONECOMM_INC)
LD_INS=$(WIRINGXX_LD) $(PTHREAD_LD) $(BEAGLEBONEUTILS_LD) $(BEAGLEBONECOMM_LD)

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -fPIC
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -fPIC

# Linker flags
LD_DEBUG_OPTIONS=-shared -Wall -o$(TARGET_D)
LD_RELEASE_OPTIONS=-shared -Wall -o$(TARGET)

# Define Include paths + definition flags
#DEFS_CC=-I$(GLOBAL_INC_CPP_PATH) -I$(INCDIRS) -I $(GLOBAL_INC_BOOST_PATH) $(DEFS_CC_RELEASE)
#DEFS_BOOST_LD=$(GLOBAL_DEF_BOOST_GLD)
DEFS_CC=
DEFS_BOOST_LD=

#############################################################################
# Module variables.

# Paths
LIBDIR=../lib
LIBDIRS=../lib/
OBJSDIR=../objs
OBJSDIRS=../objs/
SRCDIRS=../src/
INCDIR=../include
INCDIRS=../include/
TESTDIRS = ../test/
DOCDIR=../html

# Needed to build application.
LIBRARY=libbeagleboneGps

# Targets
TARGET=$(LIBRARY).so.$(VERSION)
TARGET_D=$(LIBRARY)_d.so.$(VERSION)
LIB=$(LIBDIR)/$(TARGET)
LIB_D=$(LIBDIR)/$(TARGET_D)
TEST_TARGET=testlib
BIN=$(TESTDIRS)$(TEST_TARGET)

MODULE_LIBS=$(LIB) 
MODULE_LIBS_D=$(LIB_D) 
MODULE_TEST=$(BIN)

# Object app
OBJS=$(TEST_OBJS)
LIB_OBJS=\
	$(GPSDEVICESMGR_OBJ) \
	$(GPSNMEAANALYZER_OBJ) \
	$(GPSPROTOCOLSMGR_OBJ) \
	$(GPSDEVICES_OBJ) \
	$(GPSDEVICESUBLOX_OBJ)

TEST_OBJ=testlib.o
GPSDEVICESMGR_OBJ=gpsDevicesMgr.o
GPSNMEAANALYZER_OBJ=gpsNmeaAnalyzer.o
GPSDEVICES_OBJ=gpsDevices.o
GPSPROTOCOLSMGR_OBJ=gpsProtocolsMgr.o
GPSDEVICESUBLOX_OBJ=gpsDeviceUblox.o

# Header files
GPSDEVICESMGR_INC=gpsDevicesMgr.h
GPSPROTOCOLSMGR_INC=gpsProtocolsMgr.h
GPSPROTOCOLSANALYZER_INC=gpsProtocolsAnalyzer.h
BEAGLEBONEGPSEX_INC=beagleboneGpsEx.h
GPSDEVICES_INC=gpsDevices.h

INC_MODULES=GPSDEVICESMGR_INC GPSPROTOCOLSMGR_INC GPSPROTOCOLSANALYZER_INC GPSDEVICES_INC BEAGLEBONEGPSEX_INC

#############################################################################
# Real compilation sections.

all: directories $(MODULE_LIBS) links
	@echo "*** $@ done ***"

all_d: directories $(MODULE_LIBS_D)
	@echo "*** $@ done ***"

$(LIB): $(LIB_OBJS)
	$(LD) $(LD_OPTION) -o $(LIB) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(TEST_TARGET): $(TEST_OBJ)
	$(LD) $(TEST_OBJ) -o $(BIN) -L$(HOME_LIB) -lbeagleboneUtils -lbeagleboneGps -lbeagleboneComm $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(GPSDEVICESMGR_OBJ): $(SRCDIRS)$(GPSDEVICESMGR_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(GPSDEVICESMGR_OBJ)

$(GPSNMEAANALYZER_OBJ): $(SRCDIRS)$(GPSNMEAANALYZER_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(GPSNMEAANALYZER_OBJ)

$(GPSPROTOCOLSMGR_OBJ): $(SRCDIRS)$(GPSPROTOCOLSMGR_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(GPSPROTOCOLSMGR_OBJ)

$(GPSDEVICES_OBJ): $(SRCDIRS)$(GPSDEVICES_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(GPSDEVICES_OBJ)

$(GPSDEVICESUBLOX_OBJ): $(SRCDIRS)$(GPSDEVICESUBLOX_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(GPSDEVICESUBLOX_OBJ)

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
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="-g $(GLOBAL_DEF_GCC) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)

run : test
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(TESTDIRS)$(TEST_TARGET)

run_d : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(GDB) $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) -f $(BIN) $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGET_D) $(LIBDIRS)*.o ./../$(LIBRARY).tar.gz $(TARGET) *.lst *.o *.d *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) -f $(HOME_LIB)/$(LIBRARY).so $(HOME_INC)/$(GPSDEVICESMGR_INC) $(HOME_INC)/$(GPSDEVICES_INC) $(HOME_INC)/$(GPSPROTOCOLSMGR_INC) $(HOME_INC)/$(GPSPROTOCOLSANALYZER_INC) $(HOME_INC)/$(BEAGLEBONEGPSEX_INC)
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY).so))
	ln -s $(PWD)/$(LIBDIRS)$(TARGET) $(HOME_LIB)/$(LIBRARY).so
endif
ifeq (,$(wildcard $(HOME_INC)/$(GPSDEVICESMGR_INC)))
	ln -s $(PWD)/$(INCDIRS)$(GPSDEVICESMGR_INC) $(HOME_INC)/$(GPSDEVICESMGR_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GPSDEVICES_INC)))
	ln -s $(PWD)/$(INCDIRS)$(GPSDEVICES_INC) $(HOME_INC)/$(GPSDEVICES_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GPSPROTOCOLSMGR_INC)))
	ln -s $(PWD)/$(INCDIRS)$(GPSPROTOCOLSMGR_INC) $(HOME_INC)/$(GPSPROTOCOLSMGR_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GPSPROTOCOLSANALYZER_INC)))
	ln -s $(PWD)/$(INCDIRS)$(GPSPROTOCOLSANALYZER_INC) $(HOME_INC)/$(GPSPROTOCOLSANALYZER_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(BEAGLEBONEGPSEX_INC)))
	ln -s $(PWD)/$(INCDIRS)$(BEAGLEBONEGPSEX_INC) $(HOME_INC)/$(BEAGLEBONEGPSEX_INC)
endif
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

install: compile cleanlinks
	@$(CP) $(LIBDIRS)$(TARGET) $(PATH_INSTALL_LIB)/$(LIBRARY).so
	@echo "*** $@ done ***"

install-devel: install
	@$(CP) $(INCDIRS)$(GPSDEVICESMGR_INC) $(PATH_INSTALL_INC)/$(GPSDEVICESMGR_INC)
	@$(CP) $(INCDIRS)$(GPSDEVICES_INC) $(PATH_INSTALL_INC)/$(GPSDEVICES_INC)
	@$(CP) $(INCDIRS)$(GPSPROTOCOLSMGR_INC) $(PATH_INSTALL_INC)/$(GPSPROTOCOLSMGR_INC)
	@$(CP) $(INCDIRS)$(GPSPROTOCOLSANALYZER_INC) $(PATH_INSTALL_INC)/$(GPSPROTOCOLSANALYZER_INC)
	@$(CP) $(INCDIRS)$(BEAGLEBONEGPSEX_INC) $(PATH_INSTALL_INC)/$(BEAGLEBONEGPSEX_INC)
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
	@echo "   help         : This menu."
	@echo "   clean        : Erase objects, binaries and temporary files."
	@echo "   debug        : Build a debug version."
	@echo "   compile      : Build the application in release mode."
	@echo "   recompile    : Rebuild the application in release mode."
	@echo "   rel          : Clean and rebuild the application in release mode."
	@echo "   test         : Build the test application."
	@echo "   test_d       : Build the test application in debug mode."
	@echo "   run          : Execute the test application."
	@echo "   run_d        : Execute the test application in debug mode using GDB."
	@echo "   dist         : Build a compressed archive of the application."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)/xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo "   depend       : Build the file dependencies for $(MAKE)."
	@echo "   GenDoc       : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE





