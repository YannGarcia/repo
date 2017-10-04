
########################################################################
#
#       Makefile for embedded framework - helper library
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
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH)
CC_COVERAGE_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) $(GCOV_DEF_GCC) -I$(GLOBAL_INC_CPP_PATH)
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH)

# Linker flags
LD_DEBUG_OPTIONS=-shared -Wall $(GLOBAL_DEF_STD_GLD)
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
LIBRARY=libhelper

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
	$(HELPER_OBJ) \
	$(DATE_TIME_OBJ) \
	$(IBSTREAM_OBJ) \
	$(OBSTREAM_OBJ) \
	$(KEYBOARD_OBJ) \
	$(GET_OPT_OBJ)

# Objects
TEST_OBJ=testlib.o
HELPER_OBJ=helper.o
DATE_TIME_OBJ=date_time.o 
IBSTREAM_OBJ=ibstream.o
OBSTREAM_OBJ=obstream.o
GET_OPT_OBJ=get_opt.o
KEYBOARD_OBJ=keyboard.o

# Headears
HELPER_INC=helper.h
HELPER_INC_TEMP=helper.t.h
DATE_TIME_INC=date_time.h
IBSTREAM_INC=ibstream.h
OBSTREAM_INC=obstream.h
IBSTREAM_TEMP=ibstream.t.h
OBSTREAM_TEMP=obstream.t.h
GET_OPT_INC=get_opt.h
KEYBOARD_INC=keyboard.h
RUNNABLE_INC=runnable.h

INC_MODULES=$(HELPER_INC) $(HELPER_INC_TEMP) $(DATE_TIME_INC) $(IBSTREAM_INC) $(OBSTREAM_INC) $(IBSTREAM_TEMP) $(OBSTREAM_TEMP) $(GET_OPT_INC) $(KEYBOARD_INC) $(RUNNABLE_INC)

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
	$(LD) $(TEST_OBJ) -o $(BIN) -L$(HOME_LIB) -lhelper $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(HELPER_OBJ): $(SRCDIRS)$(HELPER_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(HELPER_OBJ)

$(DATE_TIME_OBJ): $(SRCDIRS)$(DATE_TIME_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(DATE_TIME_OBJ)

$(IBSTREAM_OBJ): $(SRCDIRS)$(IBSTREAM_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -I$(HOME_INC) -c $? -o $(OBJSDIRS)$(IBSTREAM_OBJ)

$(OBSTREAM_OBJ): $(SRCDIRS)$(OBSTREAM_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -I$(HOME_INC) -c $? -o $(OBJSDIRS)$(OBSTREAM_OBJ)

$(KEYBOARD_OBJ): $(SRCDIRS)$(KEYBOARD_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -I$(HOME_INC) -c $? -o $(OBJSDIRS)$(KEYBOARD_OBJ)

$(GET_OPT_OBJ): $(SRCDIRS)$(GET_OPT_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -I$(HOME_INC) -c $? -o $(OBJSDIRS)$(GET_OPT_OBJ)

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
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS) -L$(HOME_LIB) -lconverter $(CPP_TEST_LIB)" \
	 	directories $(TEST_TARGET)

test_c : coverage
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(GCOV_DEF_GCC) $(CC_INS) $(CPP_TEST_INC) -I$(HOME_INC)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(GCOV_DEF_GCC) $(LD_INS) -L$(HOME_LIB) -llogger $(CPP_TEST_LIB)" \
		directories $(TEST_TARGET)

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) $(CPP_TEST_INC) -I$(HOME_INC)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS) -L$(HOME_LIB) -lconverter $(CPP_TEST_LIB)" \
		directories $(TEST_TARGET)

run : test
	@$(TESTDIRS)$(TEST_TARGET)

run_c : test_c
	@$(LCOV) --directory $(OBJSDIR) --zerocounters
	@$(TESTDIRS)$(TEST_TARGET)
	@$(LCOV) $(LCOV_OPTIONS) --output-file $(OBJSDIR)/$(TEST_TARGET).coverage.info
	@$(LCOV) --list $(OBJSDIR)/$(TEST_TARGET).coverage.info

run_d : test_d
	@gdb $(TESTDIRS)$(TEST_TARGET) #$(GDB_OPTIONS)

run_v : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB):$(USR_LOCAL_LIB) $(MEMCHECK) $(MEMCHECK_OPTIONS) $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) -f $(BIN) $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGET_D) ./../$(LIBRARY).tar.gz $(TARGET) $(OBJSDIR)/$(TARGET) *.lst *.o *.d *.gcno *.gcda *.info *~ $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~ $(DOCDIRS)*~
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
#	for header in $(INC_MODULES); do \
#ifeq (,$(wildcard $(HOME_INC)/$$header)) \
#		$(LN) $(PWD)/$(INCDIRS)$$header $(HOME_INC)/$$header; \
#endif \
#	done
ifeq (,$(wildcard $(HOME_INC)/$(HELPER_INC)))
	ln -s $(PWD)/$(INCDIRS)$(HELPER_INC) $(HOME_INC)/$(HELPER_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(HELPER_INC_TEMP)))
	ln -s $(PWD)/$(INCDIRS)$(HELPER_INC_TEMP) $(HOME_INC)/$(HELPER_INC_TEMP)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DATE_TIME_INC)))
	ln -s $(PWD)/$(INCDIRS)$(DATE_TIME_INC) $(HOME_INC)/$(DATE_TIME_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(IBSTREAM_INC)))
	ln -s $(PWD)/$(INCDIRS)$(IBSTREAM_INC) $(HOME_INC)/$(IBSTREAM_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(OBSTREAM_INC)))
	ln -s $(PWD)/$(INCDIRS)$(OBSTREAM_INC) $(HOME_INC)/$(OBSTREAM_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(IBSTREAM_TEMP)))
	ln -s $(PWD)/$(INCDIRS)$(IBSTREAM_TEMP) $(HOME_INC)/$(IBSTREAM_TEMP)
endif
ifeq (,$(wildcard $(HOME_INC)/$(OBSTREAM_TEMP)))
	ln -s $(PWD)/$(INCDIRS)$(OBSTREAM_TEMP) $(HOME_INC)/$(OBSTREAM_TEMP)
endif
ifeq (,$(wildcard $(HOME_INC)/$(GET_OPT_INC)))
	ln -s $(PWD)/$(INCDIRS)$(GET_OPT_INC) $(HOME_INC)/$(GET_OPT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(KEYBOARD_INC)))
	ln -s $(PWD)/$(INCDIRS)$(KEYBOARD_INC) $(HOME_INC)/$(KEYBOARD_INC)
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
	@$(CP) $(LIBDIRS)$(TARGET) $(PATH_INSTALL_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)
	@echo "*** $@ done ***"

install-devel: install
	@$(CP) $(INCDIRS)$(HELPER_INC) $(PATH_INSTALL_INC)/$(HELPER_INC)
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
	@echo "   depend       : Build the file dependencies for $(MAKE)."
	@echo "   gendoc       : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE
