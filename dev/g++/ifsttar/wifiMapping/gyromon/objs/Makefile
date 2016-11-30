
#############################################################################
#
#       Makefile for IFSTTAR WiFi Mapping prototype / Gyroscope sensor processing
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc
include $(PATH_IFSTTAR_PROJECTS)/Makefile.ifsttar

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

#WIRINGPI_PATH=${HOME}/frameworks/wiringPi/wiringPi
#WIRINGPI_INC=-I$(WIRINGPI_PATH)
#WIRINGPI_LD=-L$(WIRINGPI_PATH) -lwiringPi
endif

#############################################################################
# Libraries and options.

CC_INS=$(PTHREAD_INC)
LD_INS=$(WIRINGPI_LD) $(PTHREAD_LD)

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(GLOBAL_INC_CPP) $(CC_INS) $(WIFIMAPPING_COMMON_INS) $(PTHREAD_INC) 
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(GLOBAL_INC_CPP) $(CC_INS) $(WIFIMAPPING_COMMON_INS) $(PTHREAD_INC)

# Linker flags
LD_DEBUG_OPTIONS=-Wall -o$(TARGET)
LD_RELEASE_OPTIONS=-Wall -o$(TARGET)

# Define Include paths + definition flags
#DEFS_CC=-I$(GLOBAL_INC_CPP_PATH) -I$(INCDIRS) -I $(GLOBAL_INC_BOOST_PATH) $(DEFS_CC_RELEASE)
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
DOCDIR=../doc

# Targets
TARGET=wifimapping
BIN=$(BINDIRS)$(TARGET)

MODULE_BIN=$(BIN)

# Object app
OBJS=\
	$(MAIN_OBJ) \
	$(MAIN_APP_OBJ) \
	$(SMMGR_OBJ)

MAIN_OBJ=wifimapping.o
MAIN_APP_OBJ=mainApp.o
SMMGR_OBJ=smMgr.o

#############################################################################
# Real compilation sections.

all: directories $(TARGET)
	@echo "*** $@ done ***"

$(TARGET): $(OBJS) # TODO Remove dependancies to WiringPI
	$(LD) $(LD_OPTION) -L$(HOME_LIB) -lraspberryComm -lraspberryUtils -o $(BIN) $(OBJS)
	@echo "*** $@ done ***"

$(MAIN_OBJ): $(SRCDIRS)$(MAIN_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MAIN_OBJ)

$(MAIN_APP_OBJ): $(SRCDIRS)$(MAIN_APP_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MAIN_APP_OBJ)

$(SMMGR_OBJ): $(PATH_WIFIMAPPING_COMMON_SRC)/$(SMMGR_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(SMMGR_OBJ)

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		all

recompile :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		all

run : compile
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(BINDIRS)$(TARGET)

clean:
	@$(RM) -f $(BINS)$(TARGET) $(LIBDIRS)*.o ./../$(TARGET).tar.gz $(TARGET) *.lst *.o *.d *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~
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

install: strip cleanlinks
	@$(CP) $(BINDIRS)$(TARGET) $(PATH_INSTALL_BIN)
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
	@echo "   run       : Execute the application."
	@echo "   dist      : Build a compressed archive of the application."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)/xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo "   depend    : Build the file dependencies for $(MAKE)."
	@echo "   GenDoc    : Build the Doxygen documantation."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

#############################################################################
# Dependencies sections.

# DO NOT DELETE





