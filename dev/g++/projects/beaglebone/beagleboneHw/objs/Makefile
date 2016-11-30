
########################################################################
#
#       Makefile for hardware library
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

CC_INS=$(WIRINGXX_INC) $(PTHREAD_INC) $(BEAGLEBONEUTILS_INC)
LD_INS=$(WIRINGXX_LD) $(PTHREAD_LD) $(BEAGLEBONEUTILS_LD)

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
LIBRARY=libbeagleboneHw

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
LIB_OBJS= \
	$(INIT_LIB_OBJ) \
	$(MCP320x_SPI_OBJ) \
	$(MCP4xxxx_SPI_OBJ) \
	$(MCP251x_SPI_OBJ) \
	$(RTCC_I2C_OBJ) \
	$(DS1307_I2C_OBJ) \
	$(MCP7941x_I2C_OBJ) \
	$(MCP23x17_I2C_OBJ) \
	$(MCP24LC_I2C_OBJ) \
	$(MCP980_I2C_OBJ) \
	$(DIGITAL_IN) \
	$(DIGITAL_OUT)

TEST_OBJ=testlib.o

INIT_LIB_OBJ=beagleboneHwInit.o
# SPI/I2C devices
MCP320x_SPI_OBJ=mcp320x_spi.o 
MCP4xxxx_SPI_OBJ=mcp4xxxx_spi.o 
MCP251x_SPI_OBJ=mcp251x_spi.o
RTCC_I2C_OBJ=rtcc_i2c.o
DS1307_I2C_OBJ=ds1307_i2c.o
MCP23x17_I2C_OBJ=mcp23x17_i2c.o 
MCP24LC_I2C_OBJ=mcp24lc_i2c.o
MCP980_I2C_OBJ=mcp980x_i2c.o
MCP7941x_I2C_OBJ=mcp7941x_i2c.o

#  Header files
BEAGLEBONEHWINIT_INC=beagleboneHwInit.h
BEAGLEBONEHWEX_INC=beagleboneHwEx.h
DIGITAL_IN_INC=digitalIn.h
DIGITAL_OUT_INC=digitalOut.h
DIGITAL_INOUT_INC=digitalInOut.h
RTCC_I2C_INC=rtcc_i2c.h
DS1307_I2C_INC=ds1307_i2c.h
MCP7941x_I2C_INC=mcp7941x_i2c.h
MCP980_I2C_INC=mcp980x_i2c.h
MCP320x_SPI_INC=mcp320x_spi.h 
MCP251x_SPI_INC=mcp251x_spi.h 

INC_MODULES=BEAGLEBONEHWINIT_INC BEAGLEBONEHWEX_INC DIGITAL_IN_INC DIGITAL_OUT_INC DIGITAL_INOUT_INC MCP980_I2C_INC MCP320x_SPI_INC MCP251x_SPI_INC RTCC_I2C_INC DS1307_I2C_INC MCP7941x_I2C_INC 

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
	$(LD) $(TEST_OBJ) -o $(BIN) -L$(HOME_LIB) -lbeagleboneHw $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(MCP320x_SPI_OBJ): $(SRCDIRS)$(MCP320x_SPI_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MCP320x_SPI_OBJ)

$(INIT_LIB_OBJ): $(SRCDIRS)$(INIT_LIB_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(INIT_LIB_OBJ)

$(MCP4xxxx_SPI_OBJ): $(SRCDIRS)$(MCP4xxxx_SPI_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MCP4xxxx_SPI_OBJ)

$(MCP251x_SPI_OBJ): $(SRCDIRS)$(MCP251x_SPI_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MCP251x_SPI_OBJ)

$(MCP23x17_I2C_OBJ): $(SRCDIRS)$(MCP23x17_I2C_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MCP23x17_I2C_OBJ)

$(DS1307_I2C_OBJ): $(SRCDIRS)$(DS1307_I2C_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(DS1307_I2C_OBJ)

$(RTCC_I2C_OBJ): $(SRCDIRS)$(RTCC_I2C_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(RTCC_I2C_OBJ)

$(MCP24LC_I2C_OBJ): $(SRCDIRS)$(MCP24LC_I2C_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MCP24LC_I2C_OBJ)

$(MCP980_I2C_OBJ): $(SRCDIRS)$(MCP980_I2C_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MCP980_I2C_OBJ)

$(MCP7941x_I2C_OBJ): $(SRCDIRS)$(MCP7941x_I2C_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MCP7941x_I2C_OBJ)

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all
	@echo "*** $@ done ***"

recompile :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all
	@echo "*** $@ done ***"

rel :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(LD_RELEASE_OPTIONS)" \
		all
	@echo "*** $@ done ***"

debug :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_DEBUG_OPTIONS)" \
		LD_OPTION="$(LD_DEBUG_OPTIONS)" \
		all_d
	@echo "*** $@ done ***"

test : compile
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)
	@echo "*** $@ done ***"

test_d : debug
	${MAKE} -f Makefile \
		CC_OPTION="$(GLOBAL_DEF_GCC_DEBUG) $(CC_INS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		directories $(TEST_TARGET)
	@echo "*** $@ done ***"

run : test
	@echo "Running $(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(TESTDIRS)$(TEST_TARGET)"
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(TESTDIRS)$(TEST_TARGET)
	@echo "*** $@ done ***"

run_d : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(GDB) $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) -f $(BIN) $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGET_D) $(LIBDIRS)*.o ./../$(LIBRARY).tar.gz $(TARGET) *.lst *.o *.d *~ $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) -f $(HOME_LIB)/$(LIBRARY).so $(HOME_INC)/$(DIGITAL_IN_INC) $(HOME_INC)/$(DIGITAL_OUT_INC) $(HOME_INC)/$(DIGITAL_INOUT_INC) $(HOME_INC)/$(MCP980_I2C_INC) $(HOME_INC)/$(BEAGLEBONEHWINIT_INC) $(HOME_INC)/$(BEAGLEBONEHWEX_INC) $(HOME_INC)/$(MCP320x_SPI_INC) $(HOME_INC)/$(MCP980_I2C_INC) $(HOME_INC)/$(RTCC_I2C_INC) $(HOME_INC)/$(DS1307_I2C_INC) $(HOME_INC)/$(MCP7941x_I2C_INC)
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY).so))
		ln -s $(PWD)/$(LIBDIRS)$(TARGET) $(HOME_LIB)/$(LIBRARY).so
endif
ifeq (,$(wildcard $(HOME_INC)/$(BEAGLEBONEHWINIT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(BEAGLEBONEHWINIT_INC) $(HOME_INC)/$(BEAGLEBONEHWINIT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(BEAGLEBONEHWEX_INC)))
		ln -s $(PWD)/$(INCDIRS)$(BEAGLEBONEHWEX_INC) $(HOME_INC)/$(BEAGLEBONEHWEX_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DIGITAL_IN_INC)))
		ln -s $(PWD)/$(INCDIRS)$(DIGITAL_IN_INC) $(HOME_INC)/$(DIGITAL_IN_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DIGITAL_OUT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(DIGITAL_OUT_INC) $(HOME_INC)/$(DIGITAL_OUT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DIGITAL_INOUT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(DIGITAL_INOUT_INC) $(HOME_INC)/$(DIGITAL_INOUT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(MCP980_I2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(MCP980_I2C_INC) $(HOME_INC)/$(MCP980_I2C_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(MCP320x_SPI_INC)))
		ln -s $(PWD)/$(INCDIRS)$(MCP320x_SPI_INC) $(HOME_INC)/$(MCP320x_SPI_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(MCP251x_SPI_INC)))
		ln -s $(PWD)/$(INCDIRS)$(MCP251x_SPI_INC) $(HOME_INC)/$(MCP251x_SPI_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(RTCC_I2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(RTCC_I2C_INC) $(HOME_INC)/$(RTCC_I2C_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(DS1307_I2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(DS1307_I2C_INC) $(HOME_INC)/$(DS1307_I2C_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(MCP7941x_I2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(MCP7941x_I2C_INC) $(HOME_INC)/$(MCP7941x_I2C_INC)
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
	@$(CP) $(INCDIRS)$(BEAGLEBONEHWEX_INC) $(PATH_INSTALL_INC)/$(BEAGLEBONEHWEX_INC)
	@$(CP) $(INCDIRS)$(DIGITAL_IN_INC) $(PATH_INSTALL_INC)/$(DIGITAL_IN_INC)
	@$(CP) $(INCDIRS)$(DIGITAL_OUT_INC) $(PATH_INSTALL_INC)/$(DIGITAL_OUT_INC)
	@$(CP) $(INCDIRS)$(DIGITAL_INOUT_INC) $(PATH_INSTALL_INC)/$(DIGITAL_INOUT_INC)
	@$(CP) $(INCDIRS)$(MCP980_I2C_INC) $(PATH_INSTALL_INC)/$(MCP980_I2C_INC)
	@$(CP) $(INCDIRS)$(MCP320x_SPI_INC) $(PATH_INSTALL_INC)/$(MCP320x_SPI_INC)
	@$(CP) $(INCDIRS)$(MCP251x_SPI_INC) $(PATH_INSTALL_INC)/$(MCP251x_SPI_INC)
	@$(CP) $(INCDIRS)$(RTCC_I2C_INC) $(PATH_INSTALL_INC)/$(RTCC_I2C_INC)
	@$(CP) $(INCDIRS)$(DS1307_I2C_INC) $(PATH_INSTALL_INC)/$(DS1307_I2C_INC)
	@$(CP) $(INCDIRS)$(MCP7941x_I2C_INC) $(PATH_INSTALL_INC)/$(MCP7941x_I2C_INC)
	@echo "*** $@ done ***"

depend :
	makedepend -m $(CC_OPTIONS) $(SRCDIRS)*.cpp -f$(MAKEFILE)
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
	@echo "   run          : Run the test application."
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
