########################################################################
#
#       Makefile for embedded framework - GPS device/protocol library
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc

#############################################################################
# Override CC, CCP and LD in case of cross compiler

ifneq ($(CROSS_COMPILER_PATH), )
CC=$(CROSS_CC)
CCP=$(CROSS_CCP)
LD=$(CROSS_LD)

GLOBAL_DEF_STD_GLD=$(CROSS_GLOBAL_DEF_STD_GLD)

GLOBAL_DEF_GCC_DEBUG=$(CROSS_GLOBAL_DEF_GCC_DEBUG)
GLOBAL_DEF_GCC=$(CROSS_GLOBAL_DEF_GCC)

PTHREAD_INC=$(CROSS_PTHREAD_INC)
PTHREAD_LD=$(CROSS_PTHREAD_LD)
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

CC_INS=$(PTHREAD_INC)
LD_INS=$(PTHREAD_LD)

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH) -I../../uart/include
CC_COVERAGE_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(GCOV_DEF_GCC) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH) -I../../uart/include
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH) -I../../uart/include

# Linker flags
LD_DEBUG_OPTIONS=-g -shared -Wall $(GLOBAL_DEF_STD_GLD)
LD_COVERAGE_OPTIONS=-g -shared -Wall $(GCOV_DEF_GCC) $(GLOBAL_DEF_STD_GLD)
LD_RELEASE_OPTIONS=-shared -Wall $(GLOBAL_DEF_STD_GLD)

# Define Include paths + definition flags
#DEFS_CC=-I$(GLOBAL_INC_CPP_PATH) -I$(INCDIRS) -I$(GLOBAL_INC_BOOST_PATH) $(DEFS_CC_RELEASE)
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
DOCDIR=../docs
DOCDIRS=../docs
DOCCONF=$(DOCDIR)/o2.cfg

# Needed to build application.
LIBRARY=libgps

# Targets
TARGET=$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)$(VERSION)
TARGET_D=$(LIBRARY)_d$(SHARED_LIBRARY_EXT_NAME)$(VERSION)
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
	$(NMEA_2000_OBJ) \
	$(UBLOX_OBJ) \
	$(GPS_PARSER_OBJ)

# Objects
TEST_OBJ=testlib.o
NMEA_2000_OBJ=nmea2000.o 
UBLOX_OBJ=ublox.o
GPS_PARSER_OBJ=gps_parser.o

# Headears
GPS_DEVICE_FACTORY_INC=gps_device_factory.h
GPS_PARSER_FACTORY_INC=gps_parser_factory.h
GPS_DEVICE_INC=gps_device.h
GPS_PARSER_INC=gps_parser.h
DEVICES_INC=devices.h
PARSERS_INC=parsers.h
NMEA_2000_INC=nmea2000.h 
UBLOX_INC=ublox.h 

INC_MODULES=$(GPS_DEVICE_FACTORY_INC) $(GPS_PARSER_FACTORY_INC) $(GPS_DEVICE_INC) $(GPS_PARSER_INC) $(DEVICES_INC) $(PARSERS_INC) $(NMEA_2000_INC) $(UBLOX_INC)

#############################################################################
# Real compilation sections.

all: directories $(MODULE_LIBS) links
	@echo "*** $@ done ***"

all_d: directories $(MODULE_LIBS_D) links
	@echo "*** $@ done ***"

$(LIB): $(LIB_OBJS)
	$(LD) $(LD_OPTION) -o $(LIB) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(LIB_D): $(LIB_OBJS)
	$(LD) $(LD_OPTION) -o $(LIB_D) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(TEST_TARGET): $(TEST_OBJ)
	$(LD) $(TEST_OBJ) -o $(BIN) $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(NMEA_2000_OBJ): $(SRCDIRS)$(NMEA_2000_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(NMEA_2000_OBJ)

$(UBLOX_OBJ): $(SRCDIRS)$(UBLOX_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(UBLOX_OBJ)

$(GPS_PARSER_OBJ): $(SRCDIRS)$(GPS_PARSER_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(GPS_PARSER_OBJ)

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all

coverage :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_COVERAGE_OPTIONS)" \
		LD_OPTION="$(LD_COVERAGE_OPTIONS)" \
		all

recompile :
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
		CC_OPTION="$(GLOBAL_DEF_GCC) $(CC_INS) $(CPP_TEST_INC) -I$(HOME_INC)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS) -L$(HOME_LIB) -lgps -luart -lhal $(CPP_TEST_LIB)" \
		directories $(TEST_TARGET)

test_c : coverage
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(GCOV_DEF_GCC) $(CC_INS) $(CPP_TEST_INC) -I$(HOME_INC)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(GCOV_DEF_GCC) $(LD_INS) -L$(HOME_LIB) -luart -lhal -lconverter $(CPP_TEST_LIB)" \
		directories $(TEST_TARGET)

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) $(CPP_TEST_INC) -I$(HOME_INC)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS) -L$(HOME_LIB) -lgps -luart -lhal $(CPP_TEST_LIB)" \
		directories $(TEST_TARGET)

run : test
	@$(TESTDIRS)$(TEST_TARGET)

run_c : test_c
	@$(LCOV) --directory $(OBJSDIR) --zerocounters
	@$(TESTDIRS)$(TEST_TARGET) $(CPP_TEST_ARGS)
	@$(LCOV) $(LCOV_OPTIONS) --directory $(OBJSDIR) --output-file $(OBJSDIR)/$(TEST_TARGET).coverage.info
	@$(LCOV) --list $(OBJSDIR)/$(TEST_TARGET).coverage.info

run_d : test_d
	@gdb $(TESTDIRS)$(TEST_TARGET)

run_v : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB):$(USR_LOCAL_LIB) $(MEMCHECK) $(MEMCHECK_OPTIONS) $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) $(BIN) $(LIBDIRS)$(TARGET) $(LIB) $(LIB_D) ../../$(LIBRARY).tar.gz $(TARGET) $(TARGET) $(OBJSDIR)/$(TARGET) $(OBJSDIR)/$(CPP_TEST_RESULT_FILE) *.lst *.o *.d *.gcno *.gcda *.info *~ $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~ $(DOCDIRS)*~
	@$(RM) -fr $(DOCDIRS)html $(DOCDIRS)latex $(DOCDIRS)man
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)
	@for header in $(INC_MODULES); do \
		$(RM) $(HOME_INC)/$$header; \
	done
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)))
	$(LN) $(PWD)/$(LIBDIRS)$(TARGET) $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)
endif
#	@for header in $(INC_MODULES); do \
#		echo "### $(HOME_INC)/$$header"
#		if test -s $(HOME_INC)/$$header; then \
#			echo "$(LN) $(PWD)/$(INCDIRS)$$header $(HOME_INC)/$$header" \
#		fi \
#	done
ifeq (,$(wildcard $(HOME_INC)/$(GPS_PARSER_FACTORY_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(GPS_PARSER_FACTORY_INC) $(HOME_INC)/$(GPS_PARSER_FACTORY_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GPS_DEVICE_FACTORY_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(GPS_DEVICE_FACTORY_INC) $(HOME_INC)/$(GPS_DEVICE_FACTORY_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GPS_DEVICE_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(GPS_DEVICE_INC) $(HOME_INC)/$(GPS_DEVICE_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GPS_PARSER_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(GPS_PARSER_INC) $(HOME_INC)/$(GPS_PARSER_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DEVICES_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(DEVICES_INC) $(HOME_INC)/$(DEVICES_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(PARSERS_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(PARSERS_INC) $(HOME_INC)/$(PARSERS_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(NMEA_2000_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(NMEA_2000_INC) $(HOME_INC)/$(NMEA_2000_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(UBLOX_INC)))
	$(LN) $(PWD)/$(INCDIRS)$(UBLOX_INC) $(HOME_INC)/$(UBLOX_INC)
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
	@$(CP) $(LIBDIRS)$(TARGET) $(PATH_INSTALL_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)
	@echo "*** $@ done ***"

install-devel: install
	@$(CP) $(INCDIRS)$(GPS_DEVICE_INC) $(PATH_INSTALL_INC)/$(GPS_DEVICE_INC)
	@echo "*** $@ done ***"

push2coveralls:
	@if [ ! -f $(OBJSDIR)/$(TEST_TARTGET).coverage.info ] ; then coveralls-lcov --verbose ./logger.coverage.info ; fi ;
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
	@echo "   help         : This menu."
	@echo "   clean        : Erase objects, binaries and temporary files."
	@echo "   debug        : Build a debug version."
	@echo "   coverage     : Build a debug+coverage version."
	@echo "   compile      : Build the application in release mode."
	@echo "   recompile    : Rebuild the application in release mode."
	@echo "   rel          : Clean and rebuild the application in release mode."
	@echo "   test         : Build the test application."
	@echo "   test_c       : Build the test application in debug+coverage mode."
	@echo "   test_d       : Build the test application in debug mode."
	@echo "   run          : Build and execute the test application."
	@echo "   run_c        : Build and execute the test application in debug+coverage mode, using gcov."
	@echo "   run_d        : Build and execute the test application in debug mode."
	@echo "   run_v        : Build and execute the test application in debug mode, using Valgrind."
	@echo "   dist         : Build a compressed archive of the application."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)/xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo "*** $@ done ***"

	@echo "   depend       : Build the file dependencies for $(MAKE)."
	@echo "   gendoc       : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE





