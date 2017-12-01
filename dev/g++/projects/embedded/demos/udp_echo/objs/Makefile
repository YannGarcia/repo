#############################################################################
#
#       Makefile for embedded framework demos - UDP echo application
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
# Options.

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH) -I $(HOME_INC)-I../include
CC_COVERAGE_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) $(GCOV_DEF_GCC) -I$(GLOBAL_INC_CPP_PATH)  -I $(HOME_INC)-I../include
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -I$(GLOBAL_INC_CPP_PATH)  -I $(HOME_INC)-I../include

# Linker flags
LD_DEBUG_OPTIONS=-g -Wall $(GLOBAL_DEF_STD_GLD) -L$(HOME_LIB) -llogger -lhelper -lcomm
LD_COVERAGE_OPTIONS=-g -Wall $(GCOV_DEF_GCC) $(GLOBAL_DEF_STD_GLD) -L$(HOME_LIB) -llogger -lhelper -lcomm
LD_RELEASE_OPTIONS=-Wall $(GLOBAL_DEF_STD_GLD) -L$(HOME_LIB) -llogger -lhelper -lcomm

# Define Include paths + definition flags
#DEFS_CC=-I$(GLOBAL_INC_CPP_PATH) -I$(INCDIRS) -I$(GLOBAL_INC_BOOST_PATH) $(DEFS_CC_RELEASE)
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
INCDIRS=../include/
DOCDIR=../docs
DOCDIRS=../docs
DOCCONF=$(DOCDIR)/o2.cfg

# Targets
BIN_TARGET=udp_echo
BIN=$(BINDIRS)$(BIN_TARGET)

MODULE_TEST=$(BIN)

# Object app
OBJS=$(BIN_OBJS)

# Objects
BIN_OBJ=main.o udp_echo.o

# Headears
BIN_INC=udp_echo.hh

INC_MODULES=$(BIN_INC)

#############################################################################
# Real compilation sections.

all: $(BIN_TARGET)
	@echo "*** $@ done ***"

$(BIN_TARGET): $(BIN_OBJ)
	$(CCP) $(BIN_OBJ) -o $(BIN) $(LD_OPTION)
	@echo "*** $@ done ***"

$(BIN_OBJ): $(SRCDIRS)$(BIN_OBJ:.o=.cc)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(BIN_OBJ)

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

run : compile
	@$(BINDIRS)$(BIN_TARGET)

run_c : coverage
	@$(LCOV) --directory $(OBJSDIR) --zerocounters
	@$(BINDIRS)$(BIN_TARGET) $(CPP_BIN_ARGS)
	@$(LCOV) $(LCOV_OPTIONS) --directory $(OBJSDIR) --output-file $(BIN_TARGET).coverage.info --capture
	@$(LCOV) --list $(OBJSDIR)/$(BIN_TARGET).coverage.info

run_d : debug
	@$(GDB) $(GDB_OPTIONS) $(BINDIRS)$(BIN_TARGET)

run_v : compile
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB):$(USR_LOCAL_LIB) $(MEMCHECK) $(MEMCHECK_OPTIONS) $(BINDIRS)$(BIN_TARGET)

clean:
	@$(RM) $(BIN) *.lst *.o *.d *.gcno *.gcda *.info *~ $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(BINDIRS)*.o $(BINDIRS)*~ $(DOCDIRS)*~
	@$(RM) -fr $(DOCDIRS)html $(DOCDIRS)latex $(DOCDIRS)man
	@echo "*** $@ done ***"

depend :
	makedepend -m $(CC_OPTIONS) $(SRCDIR)/*.cc -f$(MAKEFILE)
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
	@echo "   run          : Build and execute the test application."
	@echo "   run_c        : Build and execute the test application in debug+coverage mode, using gcov."
	@echo "   run_d        : Build and execute the test application in debug mode."
	@echo "   run_v        : Build and execute the test application in debug mode, using Valgrind."
	@echo "   dist         : Build a compressed archive of the application."
	@echo "   depend       : Build the file dependencies for $(MAKE)."
	@echo "   gendoc       : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE
