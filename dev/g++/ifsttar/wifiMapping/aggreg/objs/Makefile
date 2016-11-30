
#############################################################################
#
#       Makefile for IFSTTAR WiFi Mapping prototype / Data aggregation processing
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc
include $(PATH_IFSTTAR_PROJECTS)/Makefile.ifsttar

#############################################################################
# Libraries and options.

CC_INS=$(PTHREAD_INC) $(BEAGLEBONEKML_INC) $(BEAGLEBONEGEO_INC)
LD_INS=$(WIRINGXX_LD) $(PTHREAD_LD) $(BEAGLEBONEKML_LD) $(BEAGLEBONEGEO_LD)

# Compiler flags
CC_DEBUG_OPTIONS=$(GLOBAL_DEF_GCC_DEBUG) $(GLOBAL_INC_CPP) $(CC_INS) $(WIFIMAPPING_COMMON_INS) 
CC_RELEASE_OPTIONS=$(GLOBAL_DEF_GCC) $(GLOBAL_INC_CPP) $(CC_INS) $(WIFIMAPPING_COMMON_INS)

# Linker flags
LD_DEBUG_OPTIONS=$(GLOBAL_DEF_STD_GLD) $(LD_INS)
LD_RELEASE_OPTIONS=$(GLOBAL_DEF_STD_GLD) $(LD_INS)

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
TARGET=aggreg
BIN=$(BINDIRS)$(TARGET)

MODULE_BIN=$(BIN)

# Object app
OBJS=\
	$(MAIN_OBJ) \
	$(MAIN_APP_OBJ) \
	$(MQMGR_OBJ) \
	$(STORAGEMANAGER_OBJ) \
	$(FILESTORAGE_OBJ) \
	$(TXTSTORAGE_OBJ) \
	$(KMLSTORAGE_OBJ) \
	$(DATUM_OBJ)

MAIN_OBJ=aggreg.o
MAIN_APP_OBJ=mainApp.o
MQMGR_OBJ=mqMgr.o
STORAGEMANAGER_OBJ=storageManager.o
FILESTORAGE_OBJ=fileStorage.o
TXTSTORAGE_OBJ=txtStorage.o
KMLSTORAGE_OBJ=kmlStorage.o
DATUM_OBJ=datum.o

#############################################################################
# Real compilation sections.

all: directories $(TARGET) links
	@echo "*** $@ done ***"

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $(BIN) $(LD_OPTION) $(BEAGLEBONEKML_LD) $(BEAGLEBONECOMM_LD) $(BEAGLEBONEUTILS_LD)
	@echo "*** $@ done ***"

$(MAIN_OBJ): $(SRCDIRS)$(MAIN_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MAIN_OBJ)

$(MAIN_APP_OBJ): $(SRCDIRS)$(MAIN_APP_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MAIN_APP_OBJ)

$(MQMGR_OBJ): $(PATH_WIFIMAPPING_COMMON_SRC)/$(MQMGR_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MQMGR_OBJ)

$(STORAGEMANAGER_OBJ): $(SRCDIRS)$(STORAGEMANAGER_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(STORAGEMANAGER_OBJ)

$(FILESTORAGE_OBJ): $(SRCDIRS)$(FILESTORAGE_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(FILESTORAGE_OBJ)

$(TXTSTORAGE_OBJ): $(SRCDIRS)$(TXTSTORAGE_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TXTSTORAGE_OBJ)

$(KMLSTORAGE_OBJ): $(SRCDIRS)$(KMLSTORAGE_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(KMLSTORAGE_OBJ)

$(DATUM_OBJ): $(SRCDIRS)$(DATUM_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(DATUM_OBJ)

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		all

compile_d :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_DEBUG_OPTIONS)" \
		LD_OPTION="$(LD_DEBUG_OPTIONS)" \
		all

recompile :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_RELEASE_OPTIONS)" \
		LD_OPTION="$(GLOBAL_DEF_STD_GLD) $(LD_INS)" \
		all

run : compile
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(BINDIRS)$(TARGET)

run_d : compile_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) gdb $(BINDIRS)$(TARGET)

links:
ifeq (,$(wildcard $(HOME_BIN)/$(TARGET)))
	ln -s $(PWD)/$(BINDIRS)$(TARGET) $(HOME_BIN)
endif
	@echo "*** $@ done ***"

clean: cleanlinks
	@$(RM) -f $(BINS)$(TARGET) $(LIBDIRS)*.o ./../$(TARGET).tar.gz $(TARGET) *.lst *.o *.d *~  $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) -f $(HOME_BIN)/$(TARGET)
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
