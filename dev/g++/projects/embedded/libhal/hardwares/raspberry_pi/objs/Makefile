
########################################################################
#
#       Makefile for Raspberry PI HAL (Hardware Abstract Level) support support library
#       The same libraries exists for BeagleBone Back and TIVA-C Launchpad and MBed LPC1768
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

CC_INS=$(PTHREAD_INC) $(LIBHAL_INC) 
LD_INS=$(PTHREAD_LD) $(LIBHAL_LD) 

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -I../include -I../../../include -fPIC -D$(SELECTED_HW)
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -I../include -I../../../include -fPIC -D$(SELECTED_HW)

# Linker flags
LD_DEBUG_OPTIONS=-shared -Wall $(GLOBAL_DEF_STD_GLD)
LD_RELEASE_OPTIONS=-shared -Wall $(GLOBAL_DEF_STD_GLD)

#############################################################################
# Module variables.

# Paths
LIBDIR=../lib
LIBDIRS=../lib/
OBJSDIR=../objs
OBJSDIRS=../objs/
SRCDIRS=../src/
DTCDIRS=../dtc/
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
OBJS=$(TEST_OBJS)
LIB_OBJS=\
	$(LIBHAL_OBJ) \
	$(LIBHALI2C_OBJ) \
	$(LIBHALSPI_OBJ) \
	$(LIBHALSERIAL_OBJ) \
	$(LIBHALSHIFT_OBJ) \
	$(I2C_OBJ) \
	$(SPI_OBJ)

LIBP_OBJS=\
	$(I2C_OBJ) \
	$(SPI_OBJ)

TEST_OBJ=testlib.o
LIBHAL_OBJ=libhal.o
LIBHALI2C_OBJ=libhal_i2c.o
LIBHALSPI_OBJ=libhal_spi.o
LIBHALSERIAL_OBJ=libhal_serial.o
LIBHALSHIFT_OBJ=libhal_shift.o
I2C_OBJ=i2c.o
SPI_OBJ=spi.o

# Header files
LIBHALINC=libhal.$(SELECTED_HW).h
LIBHALPINNAMESINC=libhal_pin_names.$(SELECTED_HW).h
LIBHALI2C_INCP=libhal_i2c.hpp
LIBHALSPI_INCP=libhal_spi.hpp
LIBHAL_DIGITALOUT_INC=digital_out.hpp
LIBHAL_ANALOGIN_INC=analog_in.hpp
LIBHAL_FACTORY_INC=libhal_factory.hpp

INC_MODULES=$(LIBHALINC) $(LIBHALPINNAMESINC) $(LIBHALI2C_INCP) $(LIBHALSPI_INCP) $(LIBHAL_DIGITALOUT_INC) $(LIBHAL_ANALOGIN_INC) $(LIBHAL_FACTORY_INC)

#############################################################################
# Real compilation sections.

all: directories $(MODULE_LIBS) links
	@echo "*** $@ done ***"

all_d: directories $(MODULE_LIBS_D) links
	@echo "*** $@ done ***"

$(LIB): $(LIB_OBJS)
	$(LD) $(LD_OPTION) -o $(LIB) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(LIBP): $(LIBP_OBJS)
	$(LD) $(LD_OPTION) -o $(LIBP) $(LIBP_OBJS)
	@echo "*** $@ done ***"

$(TEST_TARGET): $(TEST_OBJ)
	$(LD) $(TEST_OBJ) -o $(BIN) $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.c)
	$(CCP) $(CC_OPTION) -D$(SELECTED_HW) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(LIBHAL_OBJ): $(SRCDIRS)$(LIBHAL_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHAL_OBJ)

$(LIBHALSPI_OBJ): $(SRCDIRS)$(LIBHALSPI_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHALSPI_OBJ)

$(LIBHALI2C_OBJ): $(SRCDIRS)$(LIBHALI2C_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHALI2C_OBJ)

$(LIBHALSERIAL_OBJ): $(SRCDIRS)$(LIBHALSERIAL_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHALSERIAL_OBJ)

$(LIBHALSHIFT_OBJ): $(SRCDIRS)$(LIBHALSHIFT_OBJ:.o=.c)
	$(CC) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(LIBHALSHIFT_OBJ)

$(SPI_OBJ): $(SRCDIRS)$(SPI_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(SPI_OBJ)

$(I2C_OBJ): $(SRCDIRS)$(I2C_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(I2C_OBJ)

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
		CC_OPTION="$(GLOBAL_DEF_GCC) $(CC_INS) $(CPP_TEST_INC)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS) $(CPP_TEST_LIB)" \
		directories $(TEST_TARGET)

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) $(CPP_TEST_INC)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS) $(CPP_TEST_LIB)" \
		directories $(TEST_TARGET)

run : test
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB):$(USR_LOCAL_LIB) $(TESTDIRS)$(TEST_TARGET)

run_d : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB):$(USR_LOCAL_LIB) $(GDB) $(TESTDIRS)$(TEST_TARGET)

run_v : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB):$(USR_LOCAL_LIB) $(MEMCHECK) $(MEMCHECK_OPTIONS) $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) -f $(BIN) $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGETP) $(LIBDIRS)$(TARGET_D) $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME) $(HOME_LIB)/$(LIBRARY)++$(SHARED_LIBRARY_EXT_NAME) $(LIBDIRS)*.o ./../$(LIBRARY).tar.gz $(TARGET) *.lst *.o *.d *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~ $(DOCDIRS)*~ $(DTC_TARGET)
	@$(RM) -fr $(DOCDIRS)html $(DOCDIRS)latex $(DOCDIRS)man
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)
	@$(RM) $(HOME_LIB)/$(LIBRARY)++$(SHARED_LIBRARY_EXT_NAME)
	@for header in $(INC_MODULES); do \
		$(RM) $(HOME_INC)/$$header; \
	done
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)))
		ln -s $(PWD)/$(LIB) $(HOME_LIB)/$(LIBRARY)$(SHARED_LIBRARY_EXT_NAME)
endif
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY)++$(SHARED_LIBRARY_EXT_NAME)))
		ln -s $(PWD)/$(LIBP) $(HOME_LIB)/$(LIBRARY)++$(SHARED_LIBRARY_EXT_NAME)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALINC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALINC) $(HOME_INC)/$(LIBHALINC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALPINNAMES_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALPINNAMES_INC) $(HOME_INC)/$(LIBHALPINNAMES_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALI2C_INCP)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALI2C_INCP) $(HOME_INC)/$(LIBHALI2C_INCP)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALSPI_INCP)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALSPI_INCP) $(HOME_INC)/$(LIBHALSPI_INCP)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_DIGITALOUT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHAL_DIGITALOUT_INC) $(HOME_INC)/$(LIBHAL_DIGITALOUT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_ANALOGIN_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHAL_ANALOGIN_INC) $(HOME_INC)/$(LIBHAL_ANALOGIN_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_FACTORY_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHAL_FACTORY_INC) $(HOME_INC)/$(LIBHAL_FACTORY_INC)
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

pushO2:
	@if [ -d $(HOME_DOCS) ] ; then mkdir -p $(HOME_DOCS)/$(LIBRARY) && mv ./$(OBJDIR)/html $(HOME_DOCS)/$(LIBRARY) ; fi ;
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
	@echo "   rel        : Clean and rebuild the application in release mode."
	@echo "   test       : Build the test application."
	@echo "   test_d     : Build the test application in debug mode."
	@echo "   run        : Build and execute the test application."
	@echo "   run_d      : Build and execute the test application in debug mode."
	@echo "   run_v      : Build and execute the test application in debug mode, using Valgrind."
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
