
########################################################################
#
#       Makefile for Utilities and Helpers library
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc

#############################################################################
# Override CC, CCP and LD in case of cross compiler

ifneq ($(PI_CROSS_COMPILER_PATH), )
CC=$(PI_CC)
CCP=$(PI_CCP)
LD=$(PI_LD)

GLOBAL_DEF_STD_GLD=$(PI_PI_GLOBAL_DEF_STD_GLD)

GLOBAL_DEF_GCC_DEBUG=$(PI_GLOBAL_DEF_GCC_DEBUG)
GLOBAL_DEF_GCC=$(PI_GLOBAL_DEF_GCC)

PTHREAD_INC=$(PI_PTHREAD_INC)
PTHREAD_LD=$(PI_PTHREAD_LD)
endif

#############################################################################
# Shared library version

DYN_VERS_MAJ=0
DYN_VERS_MIN=1

VERSION=$(DYN_VERS_MAJ).$(DYN_VERS_MIN)

#############################################################################
# Libraries and options.

#CC_INS=$(WIRINGPI_INC) $(PTHREAD_INC)
#LD_INS=$(WIRINGPI_LD) $(PTHREAD_LD)

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
LIBRARY=libbeagleboneUtils

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
	$(CONVERTER_OBJ) \
	$(GETOPT_OBJ) \
	$(KEYBOARD_OBJ) \
	$(DUMP_OBJ) \
	$(DATETIME_OBJ) \
	$(ARCHIVE_OBJ) \
	$(DEBUG_OBJ)

# Objects
TEST_OBJ=testlib.o
CONVERTER_OBJ=converter.o 
GETOPT_OBJ=getOpt.o 
KEYBOARD_OBJ=keyboard.o
DUMP_OBJ=dump.o
ARCHIVE_OBJ=archive.o
DATETIME_OBJ=dateTime.o
DEBUG_OBJ=debug.o

# Headears
CONVERTER_INC=converter.h
GETOPT_INC=getOpt.h
KEYBOARD_INC=keyboard.h
DUMP_INC=dump.h
DATETIME_INC=dateTime.h
ARCHIVE_INC=archive.h
DEBUG_INC=debug.h
RUNNABLE_INC=runnable.h

INC_MODULES=CONVERTER_INC GETOPT_INC KEYBOARD_INC DUMP_INC DATETIME_INC ARCHIVE_INC DEBUG_INC RUNNABLE_INC

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
	$(LD) $(LD_OPTION) -o $(LIB) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(TEST_TARGET): $(TEST_OBJ)
	$(LD) $(TEST_OBJ) -o $(BIN) -L$(HOME_LIB) -lbeagleboneUtils $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(CONVERTER_OBJ): $(SRCDIRS)$(CONVERTER_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(CONVERTER_OBJ)

$(GETOPT_OBJ): $(SRCDIRS)$(GETOPT_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(GETOPT_OBJ)

$(KEYBOARD_OBJ): $(SRCDIRS)$(KEYBOARD_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(KEYBOARD_OBJ)

$(ARCHIVE_OBJ): $(SRCDIRS)$(ARCHIVE_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(ARCHIVE_OBJ)

$(DATETIME_OBJ): $(SRCDIRS)$(DATETIME_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(DATETIME_OBJ)

$(DEBUG_OBJ): $(SRCDIRS)$(DEBUG_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(DEBUG_OBJ)

$(DUMP_OBJ): $(SRCDIRS)$(DUMP_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(DUMP_OBJ)

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all
ifneq ($(PI_CROSS_COMPILER_PATH), )
	$(shell $(SCP) $(LIB) $(PI_FTP_REMOTE_USERID)@$(PI_FTP_REMOTE_HOST):$(PI_FTP_REMOTE_BACKUP_DIR)/$(LIBRARY).so)
endif

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
		CC_OPTION="$(GLOBAL_DEF_GCC) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)
ifneq ($(PI_CROSS_COMPILER_PATH), )
	$(shell $(SCP) $(BIN) $(PI_FTP_REMOTE_USERID)@$(PI_FTP_REMOTE_HOST):$(PI_FTP_REMOTE_BACKUP_DIR)/$(TEST_TARGET))
endif

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)

run : test
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(TESTDIRS)$(TEST_TARGET)

run_d : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) gdb $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) -f $(BIN) $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGET_D) ./../$(LIBRARY).tar.gz $(TARGET) *.lst *.o *.d *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) -f $(HOME_INC)/$(CONVERTER_INC) $(HOME_INC)/$(GETOPT_INC) $(HOME_INC)/$(KEYBOARD_INC) $(HOME_INC)/$(DEBUG_INC) $(HOME_INC)/$(DUMP_INC) $(HOME_INC)/$(RUNNABLE_INC) $(HOME_INC)/$(ARCHIVE_INC) $(HOME_INC)/$(DATETIME_INC) $(HOME_LIB)/$(LIBRARY).so $(LIBDIRS)*.o
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY).so))
	ln -s $(PWD)/$(LIBDIRS)$(TARGET) $(HOME_LIB)/$(LIBRARY).so
endif
ifeq (,$(wildcard $(HOME_INC)/$(CONVERTER_INC)))
	ln -s $(PWD)/$(INCDIRS)$(CONVERTER_INC) $(HOME_INC)/$(CONVERTER_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GETOPT_INC)))
	ln -s $(PWD)/$(INCDIRS)$(GETOPT_INC) $(HOME_INC)/$(GETOPT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(KEYBOARD_INC)))
	ln -s $(PWD)/$(INCDIRS)$(KEYBOARD_INC) $(HOME_INC)/$(KEYBOARD_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(ARCHIVE_INC)))
	ln -s $(PWD)/$(INCDIRS)$(ARCHIVE_INC) $(HOME_INC)/$(ARCHIVE_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DATETIME_INC)))
	ln -s $(PWD)/$(INCDIRS)$(DATETIME_INC) $(HOME_INC)/$(DATETIME_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DEBUG_INC)))
	ln -s $(PWD)/$(INCDIRS)$(DEBUG_INC) $(HOME_INC)/$(DEBUG_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DUMP_INC)))
	ln -s $(PWD)/$(INCDIRS)$(DUMP_INC) $(HOME_INC)/$(DUMP_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(RUNNABLE_INC)))
	ln -s $(PWD)/$(INCDIRS)$(RUNNABLE_INC) $(HOME_INC)/$(RUNNABLE_INC)
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
	@$(CP) $(INCDIRS)$(CONVERTER_INC) $(PATH_INSTALL_INC)/$(CONVERTER_INC)
	@$(CP) $(INCDIRS)$(GETOPT_INC) $(PATH_INSTALL_INC)/$(GETOPT_INC)
	@$(CP) $(INCDIRS)$(KEYBOARD_INC) $(PATH_INSTALL_INC)/$(KEYBOARD_INC)
	@$(CP) $(INCDIRS)$(ARCHIVE_INC) $(PATH_INSTALL_INC)/$(ARCHIVE_INC)
	@$(CP) $(INCDIRS)$(DATETIME_INC) $(PATH_INSTALL_INC)/$(DATETIME_INC)
	@$(CP) $(INCDIRS)$(DEBUG_INC) $(PATH_INSTALL_INC)/$(DEBUG_INC)
	@$(CP) $(INCDIRS)$(DUMP_INC) $(PATH_INSTALL_INC)/$(DUMP_INC)
	@$(CP) $(INCDIRS)$(RUNNABLE_INC) $(PATH_INSTALL_INC)/$(RUNNABLE_INC)
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





