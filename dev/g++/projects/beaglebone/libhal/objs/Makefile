
########################################################################
#
#       Makefile for BeagleBone Black HAL (Hardware Abstract Level) support support library
#       The same libraries exists for Raspeberry PI and MBed LPC1768
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
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS) -fPIC -I$(GLOBAL_INC_CPP_PATH)
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(CC_INS) -fPIC -I$(GLOBAL_INC_CPP_PATH)

# Linker flags
LD_DEBUG_OPTIONS=-shared -Wall $(GLOBAL_DEF_STD_GLD)
LD_RELEASE_OPTIONS=-shared -Wall $(GLOBAL_DEF_STD_GLD)

DTC_CC_OPTION=-I dts -O dtb -b 0 -@

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

# Cape Targets
DTC_VERSION=00A0
DTC_NAME=BB-LIBHAL
DTC_TARGET=$(DTC_NAME)-$(DTC_VERSION).dtbo
DTC_BIN=$(OBJSDIRS)$(DTC_TARGET)

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
LIBHAL_INC=libhal.h
LIBHALI2C_INC=libhal_i2c.h
LIBHALSPI_INC=libhal_spi.h
LIBHALSERIAL_INC=libhal_serial.h
LIBHALSHIFT_INC=libhal_shift.h
LIBHALPINAMES_INC=libhal_pin_names.h
LIBHALI2C_INCP=libhal_i2c.hpp
LIBHALSPI_INCP=libhal_spi.hpp
LIBHAL_DIGITALOUT_INC=digital_out.hpp
LIBHAL_ANALOGIN_INC=analog_in.hpp
LIBHAL_FACTORY_INC=libhal_factory.hpp

INC_MODULES=LIBLIBHAL_INC LIBHALI2C_INC LIBHALSPI_INC LIBHALSERIAL_INC LIBHALSHIFT_INC LIBHALPINAMES_INC LIBHALI2C_INCP LIBHALSPI_INCP LIBHAL_DIGITALOUT_INC LIBHAL_ANALOGIN_INC LIBHAL_FACTORY_INC

# Cape Object app
DTC_OBJS=\
	$(DTC_MAIN_OBJ)

DTC_MAIN_OBJ=$(DTC_NAME).dtbo

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
	$(LD) $(TEST_OBJ) -o $(BIN) -L$(HOME_LIB) -lhal -lhal++ $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

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

$(DTC_MAIN_OBJ): $(DTCDIRS)$(DTC_MAIN_OBJ:.dtbo=.dts)
	@$(DTC) $(DTC_CC_OPTION) -o $(DTC_BIN) $?
	@$(SUDO) $(CP) $(DTC_BIN) $(PATH_LIB_FRMW)
	@echo "*** $@ done ***"

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

dtc : $(DTC_MAIN_OBJ)
	@echo "*** $@ done ***"

install_dtc : dtc
	@$(CD) $(PATH_LIB_FRMW)
	@$(SUDO) $(SU) -c "echo $(DTC_NAME) > $(SLOTS)"
	@$(CD) -
	dmesg
	@echo "*** $@ done ***"

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
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHAL_INC) $(HOME_INC)/$(LIBHAL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALI2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALI2C_INC) $(HOME_INC)/$(LIBHALI2C_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALSPI_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALSPI_INC) $(HOME_INC)/$(LIBHALSPI_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALSERIAL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALSERIAL_INC) $(HOME_INC)/$(LIBHALSERIAL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALSHIFT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALSHIFT_INC) $(HOME_INC)/$(LIBHALSHIFT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALPINAMES_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALPINAMES_INC) $(HOME_INC)/$(LIBHALPINAMES_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(I2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(I2C_INC) $(HOME_INC)/$(I2C_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(SPI_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SPI_INC) $(HOME_INC)/$(SPI_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_DIGITALOUT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SPI_INC) $(HOME_INC)/$(LIBHAL_DIGITALOUT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_ANALOGIN_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SPI_INC) $(HOME_INC)/$(LIBHAL_ANALOGIN_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHAL_FACTORY_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SPI_INC) $(HOME_INC)/$(LIBHAL_FACTORY_INC)
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
	@echo "   dtc        : Rebuild the device Tree Overlay."
	@echo "   install_dtc: Install the device Tree Overlay."
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
