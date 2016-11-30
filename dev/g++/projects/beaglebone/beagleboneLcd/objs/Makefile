
########################################################################
#
#       Makefile for LCD support support library
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

BEAGLEBONEUTILS_INC=-I$(PATH_DEV_CROSS_PROJECTS)/beagleboneUtils/include
BEAGLEBONEUTILS_LD=-L$(HOME_LIB) -lbeagleboneUtils
BEAGLEBONEHW_INC=-I$(PATH_DEV_CROSS_PROJECTS)/beagleboneHw/include
BEAGLEBONEHW_LD=-L$(HOME_LIB) -lbeagleboneHw

CC_INS=$(WIRINGXX_INC) $(PTHREAD_INC) $(BEAGLEBONEUTILS_INC) $(BEAGLEBONEHW_INC)
LD_INS=$(WIRINGXX_LD) $(PTHREAD_LD) $(BEAGLEBONEUTILS_LD) $(BEAGLEBONEHW_LD)

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
LIBRARY=libbeagleboneLcd

# Targets
TARGET=$(LIBRARY).so.$(VERSION)
TARGET_D=$(LIBRARY)_d.so.$(VERSION)
LIB=$(LIBDIR)/$(TARGET)
LIB_D=$(LIBDIR)/$(TARGET_D)
TEST_TARGET=testlib
BIN=$(TESTDIRS)$(TEST_TARGET)

MODULE_LIBS=$(LIB) 
MODULE_LIBS_D=$(LIB) 
MODULE_TEST=$(BIN)

# Object app
OBJS=$(TEST_OBJS)
LIB_OBJS=\
	$(LCDMANAGER_OBJ) \
	$(LCDHD44780U_OBJ)

TEST_OBJ=testlib.o
LCDMANAGER_OBJ=lcdManager.o 
LCDHD44780U_OBJ=lcdHD44780U.o 

# Header files
ABSTRACTLCD_INC=abstractLcd.h
LCDMGR_INC=lcdManager.h

INC_MODULES=ABSTRACTLCD_INC LCDMGR_INC

#############################################################################
# Real compilation sections.

all: directories $(MODULE_LIBS) links
	@echo "*** $@ done ***"

all_d: directories $(MODULE_LIBS_D) links
	@echo "*** $@ done ***"

$(LIB): $(LIB_OBJS)
	$(LD) $(LD_OPTION) -o $(LIB) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(TEST_TARGET): $(TEST_OBJ)
	$(LD) $(TEST_OBJ) -o $(BIN) -L$(HOME_LIB) -lbeagleboneLcd $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(LCDMANAGER_OBJ): $(SRCDIRS)$(LCDMANAGER_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LCDMANAGER_OBJ)

$(LCDHD44780U_OBJ): $(SRCDIRS)$(LCDHD44780U_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LCDHD44780U_OBJ)

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
		all_d

test : compile
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)

run : test
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(TESTDIRS)$(TEST_TARGET)

run_d : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(GDB) $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) -f $(BIN) $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGET_D) $(HOME_LIB)/$(LIBRARY).so $(LIBDIRS)*.o ./../$(LIBRARY).tar.gz $(TARGET) *.lst *.o *.d *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) -f $(HOME_LIB)/$(LIBRARY).so $(HOME_INC)/$(ABSTRACTLCD_INC) $(HOME_INC)/$(LCDMGR_INC)
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY).so))
		ln -s $(PWD)/$(LIBDIRS)$(TARGET) $(HOME_LIB)/$(LIBRARY).so
endif
ifeq (,$(wildcard $(HOME_INC)/$(ABSTRACTLCD_INC)))
		ln -s $(PWD)/$(INCDIRS)$(ABSTRACTLCD_INC) $(HOME_INC)/$(ABSTRACTLCD_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LCDMGR_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LCDMGR_INC) $(HOME_INC)/$(LCDMGR_INC)
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
	@$(CP) $(INCDIRS)$(ABSTRACTLCD_INC) $(PATH_INSTALL_INC)/$(ABSTRACTLCD_INC)
	@$(CP) $(INCDIRS)$(LCDMGR_INC) $(PATH_INSTALL_INC)/$(LCDMGR_INC)
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





