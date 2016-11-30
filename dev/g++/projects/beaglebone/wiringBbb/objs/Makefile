
########################################################################
#
#       Makefile for Raspberry LCD support support library
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

CC_INS=$(PTHREAD_INC) -I/usr/include/arm-linux-gnueabihf
LD_INS=$(PTHREAD_LD)

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -fPIC
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -fPIC

# Linker flags
LD_DEBUG_OPTIONS=-shared -Wall -o$(TARGET_D)
LD_RELEASE_OPTIONS=-shared -Wall -o$(TARGET)

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
LIBRARY=libwiringBbb

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
	$(WIRINGBBB_OBJ) \
	$(WIRINGBBBI2C_OBJ) \
	$(WIRINGBBBSPI_OBJ) \
	$(WIRINGSERIAL_OBJ) \
	$(WIRINGSHIFT_OBJ)

TEST_OBJ=testlib.o
WIRINGBBB_OBJ=wiringBbb.o
WIRINGBBBI2C_OBJ=wiringBbbI2C.o
WIRINGBBBSPI_OBJ=wiringBbbSPI.o
WIRINGSERIAL_OBJ=wiringSerial.o
WIRINGSHIFT_OBJ=wiringShift.o

# Header files
WIRINGBBB_INC=wiringBbb.h
WIRINGBBBI2C_INC=wiringBbbI2C.h
WIRINGBBBSPI_INC=wiringBbbSPI.h
WIRINGSERIAL_INC=wiringSerial.h
WIRINGSHIFT_INC=wiringShift.h

INC_MODULES=WIRINGBBB_INC WIRINGBBBI2C_INC WIRINGBBBSPI_INC WIRINGSERIAL_INC WIRINGSHIFT_INC

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
	$(LD) $(TEST_OBJ) -o $(BIN) $(LD_OPTION) $(WIRINGXX_LD)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(WIRINGBBB_OBJ): $(SRCDIRS)$(WIRINGBBB_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(WIRINGBBB_OBJ)

$(WIRINGBBBSPI_OBJ): $(SRCDIRS)$(WIRINGBBBSPI_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(WIRINGBBBSPI_OBJ)

$(WIRINGBBBI2C_OBJ): $(SRCDIRS)$(WIRINGBBBI2C_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(WIRINGBBBI2C_OBJ)

$(WIRINGSERIAL_OBJ): $(SRCDIRS)$(WIRINGSERIAL_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(WIRINGSERIAL_OBJ)

$(WIRINGSHIFT_OBJ): $(SRCDIRS)$(WIRINGSHIFT_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(WIRINGSHIFT_OBJ)

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
	@$(RM) -f $(HOME_LIB)/$(LIBRARY).so $(HOME_INC)/$(WIRINGBBB_INC) $(HOME_INC)/$(WIRINGBBBI2C_INC) $(HOME_INC)/$(WIRINGBBBSPI_INC) $(HOME_INC)/$(WIRINGSERIAL_INC) $(HOME_INC)/$(WIRINGSHIFT_INC)
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY).so))
		ln -s $(PWD)/$(LIBDIRS)$(TARGET) $(HOME_LIB)/$(LIBRARY).so
endif
ifeq (,$(wildcard $(HOME_INC)/$(WIRINGBBB_INC)))
		ln -s $(PWD)/$(INCDIRS)$(WIRINGBBB_INC) $(HOME_INC)/$(WIRINGBBB_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(WIRINGBBBI2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(WIRINGBBBI2C_INC) $(HOME_INC)/$(WIRINGBBBI2C_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(WIRINGBBBSPI_INC)))
		ln -s $(PWD)/$(INCDIRS)$(WIRINGBBBSPI_INC) $(HOME_INC)/$(WIRINGBBBSPI_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(WIRINGSERIAL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(WIRINGSERIAL_INC) $(HOME_INC)/$(WIRINGSERIAL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(WIRINGSHIFT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(WIRINGSHIFT_INC) $(HOME_INC)/$(WIRINGSHIFT_INC)
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
	@echo "   debug     : Build a debug version."
	@echo "   compile   : Build the application in release mode."
	@echo "   recompile : Rebuild the application in release mode."
	@echo "   rel       : Clean and rebuild the application in release mode."
	@echo "   test      : Build the test application."
	@echo "   dist      : Build a compressed archive of the application."
	@echo "   depend    : Build the file dependencies for $(MAKE)."
	@echo "   GenDoc    : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE





