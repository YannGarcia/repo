
########################################################################
#
#       Makefile for Beaglebone Black communication library
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

CURL_INC=-I/usr/include
CURL_LD=-L/usr/lib -lcurl

BEAGLEBONEUTILS_INC=-I$(PATH_DEV_CROSS_PROJECTS)/beagleboneUtils/include
BEAGLEBONEUTILS_LD=-L$(HOME_LIB) -lbeagleboneUtils

WIRINGXX_LD=-L$(HOME_LIB) -lwiringBbb

CC_INS=$(WIRINGXX_INC) $(PTHREAD_INC) $(BEAGLEBONEUTILS_INC) $(CURL_INC) -I$(HOME_INC)
LD_INS=$(WIRINGXX_LD) $(PTHREAD_LD) $(BEAGLEBONEUTILS_LD) $(CURL_LD)

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
LIBRARY=libbeagleboneComm

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
	$(SOCKET_OBJ) \
	$(CHANNELMGR_OBJ) \
	$(RAWCHANNEL_OBJ) \
	$(SERIALCHANNEL_OBJ) \
	$(UDPCHANNEL_OBJ) \
	$(TCPCHANNEL_OBJ) \
	$(IPV4ADDRESS_OBJ) \
	$(SOCKETADDRESS_OBJ) \
	$(SHAREDMEMORY_OBJ) \
	$(MESSAGEQUEUE_OBJ) \
	$(NTPCLIENT_OBJ) \
	$(HTTPCLIENT_OBJ)

TEST_OBJ=testlib.o
TEST_OBJ_SERIAL_THREAD=serialThread.o
TEST_OBJ_NETWORK_THREAD=networkThread.o
TEST_OBJ_TCP_NETWORK_THREAD=networkTcpThread.o

INIT_LIB_OBJ=beagleboneCommInit.o
# Network
SOCKETADDRESS_OBJ=socketAddress.o
IPV4ADDRESS_OBJ=ipv4Address.o
SOCKET_OBJ=socket.o
CHANNELMGR_OBJ=channelManager.o
RAWCHANNEL_OBJ=rawChannel.o
SERIALCHANNEL_OBJ=serialChannel.o
UDPCHANNEL_OBJ=udpChannel.o
TCPCHANNEL_OBJ=tcpChannel.o
NTPCLIENT_OBJ=ntpClient.o
HTTPCLIENT_OBJ=httpClient.o
# IPC
SHAREDMEMORY_OBJ=sharedMemory.o
MESSAGEQUEUE_OBJ=messageQueue.o

# Header files
BEAGLEBONECOMMINIT_INC=beagleboneCommInit.h
SOCKETADDRESS_INC=socketAddress.h
IPV4ADDRESS_INC=ipv4Address.h
ABSTRACTHTTPDATA_INC=abstractHttpData.h
ABSTRACTCHANNEL_INC=abstractChannel.h
CHANNELMGR_INC=channelManager.h
SOCKET_INC=socket.h
RAWCHANNEL_INC=rawChannel.h
SERIALCHANNEL_INC=serialChannel.h
UDPCHANNEL_INC=udpChannel.h
TCPCHANNEL_INC=tcpChannel.h
SHAREDMEMORY_INC=sharedMemory.h
MESSAGEQUEUE_INC=messageQueue.h
BEAGLEBONECOMMEX_INC=beagleboneCommEx.h
NTPCLIENT_INC=ntpClient.h
HTTPCLIENT_INC=httpClient.h
HTTPTEXT_INC= httpText.h

INC_MODULES=BEAGLEBONECOMMINIT_INC SOCKETADDRESS_INC IPV4ADDRESS_INC ABSTRACTHTTPDATA_INC ABSTRACTCHANNEL_INC CHANNELMGR_INC SHAREDMEMORY_INC MESSAGEQUEUE_INC BEAGLEBONECOMMEX_INC RAWCHANNEL_INC SERIALCHANNEL_INC UDPCHANNEL_INC TCPCHANNEL_INC NTPCLIENT_INC HTTPCLIENT_INC HTTPTEXT_INC

#############################################################################
# Real compilation sections.

all: directories $(MODULE_LIBS) links
	@echo "*** $@ done ***"

all_d: directories $(MODULE_LIBS) links
	@echo "*** $@ done ***"

$(LIB): $(LIB_OBJS)
	$(LD) $(LD_OPTION) -o $(LIB) $(LIB_OBJS)
	@echo "*** $@ done ***"

$(TEST_TARGET): $(TEST_OBJ) $(TEST_OBJ_SERIAL_THREAD) $(TEST_OBJ_NETWORK_THREAD) $(TEST_OBJ_TCP_NETWORK_THREAD)
	$(LD) $(TEST_OBJ) $(TEST_OBJ_SERIAL_THREAD) $(TEST_OBJ_NETWORK_THREAD) $(TEST_OBJ_TCP_NETWORK_THREAD) -o $(BIN) -L$(HOME_LIB) -lbeagleboneComm $(LD_OPTION)
	@echo "*** $@ done ***"

$(TEST_OBJ): $(TESTDIRS)$(TEST_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ)

$(TEST_OBJ_SERIAL_THREAD): $(TESTDIRS)$(TEST_OBJ_SERIAL_THREAD:.o=.cpp)
	$(CCP) $(CC_OPTION) -I. -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ_SERIAL_THREAD)

$(TEST_OBJ_NETWORK_THREAD): $(TESTDIRS)$(TEST_OBJ_NETWORK_THREAD:.o=.cpp)
	$(CCP) $(CC_OPTION) -I. -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ_NETWORK_THREAD)

$(TEST_OBJ_TCP_NETWORK_THREAD): $(TESTDIRS)$(TEST_OBJ_TCP_NETWORK_THREAD:.o=.cpp)
	$(CCP) $(CC_OPTION) -I. -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TEST_OBJ_TCP_NETWORK_THREAD)

$(IPV4ADDRESS_OBJ): $(SRCDIRS)$(IPV4ADDRESS_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(IPV4ADDRESS_OBJ)

$(SOCKETADDRESS_OBJ): $(SRCDIRS)$(SOCKETADDRESS_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(SOCKETADDRESS_OBJ)

$(SOCKET_OBJ): $(SRCDIRS)$(SOCKET_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(SOCKET_OBJ)

$(CHANNELMGR_OBJ): $(SRCDIRS)$(CHANNELMGR_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(CHANNELMGR_OBJ)

$(RAWCHANNEL_OBJ): $(SRCDIRS)$(RAWCHANNEL_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(RAWCHANNEL_OBJ)

$(SERIALCHANNEL_OBJ): $(SRCDIRS)$(SERIALCHANNEL_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(SERIALCHANNEL_OBJ)

$(UDPCHANNEL_OBJ): $(SRCDIRS)$(UDPCHANNEL_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(UDPCHANNEL_OBJ)

$(TCPCHANNEL_OBJ): $(SRCDIRS)$(TCPCHANNEL_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(TCPCHANNEL_OBJ)

$(SHAREDMEMORY_OBJ): $(SRCDIRS)$(SHAREDMEMORY_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(SHAREDMEMORY_OBJ)

$(MESSAGEQUEUE_OBJ): $(SRCDIRS)$(MESSAGEQUEUE_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(MESSAGEQUEUE_OBJ)

$(NTPCLIENT_OBJ): $(SRCDIRS)$(NTPCLIENT_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(NTPCLIENT_OBJ)

$(HTTPCLIENT_OBJ): $(SRCDIRS)$(HTTPCLIENT_OBJ:.o=.cpp)
	$(CCP) $(CC_OPTION) -I$(INCDIR) -c $? -o $(OBJSDIRS)$(HTTPCLIENT_OBJ)

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
	@echo "Running $(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(TESTDIRS)$(TEST_TARGET)"
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(TESTDIRS)$(TEST_TARGET)

run_d : test_d
	@$(SUDO) LD_LIBRARY_PATH=$(HOME_LIB) $(GDB) $(TESTDIRS)$(TEST_TARGET)

clean: cleanlinks
	@$(RM) -f $(BIN) $(LIBDIRS)$(TARGET) $(LIBDIRS)$(TARGET_D) $(LIBDIRS)*.o ./../$(LIBRARY).tar.gz $(TARGET) *.lst *.o *.d *~ $(SRCDIRS)*.o $(SRCDIRS)*~ $(SRCDIRS)*.err $(INCDIR)/*~ $(TESTDIRS)*.o $(TESTDIRS)*~
	@echo "*** $@ done ***"

cleanlinks:
	@$(RM) -f $(HOME_LIB)/$(LIBRARY).so $(HOME_INC)/$(SOCKETADDRESS_INC) $(HOME_INC)/$(SOCKET_INC) $(HOME_INC)/$(IPV4ADDRESS_INC) $(HOME_INC)/$(ABSTRACTHTTPDATA_INC) $(HOME_INC)/$(ABSTRACTCHANNEL_INC) $(HOME_INC)/$(CHANNELMGR_INC) $(HOME_INC)/$(RAWCHANNEL_INC) $(HOME_INC)/$(SERIALCHANNEL_INC) $(HOME_INC)/$(UDPCHANNEL_INC) $(HOME_INC)/$(TCPCHANNEL_INC) $(HOME_INC)/$(SHAREDMEMORY_INC) $(HOME_INC)/$(BEAGLEBONECOMMINIT_INC) $(HOME_INC)/$(BEAGLEBONECOMMEX_INC) $(HOME_INC)/$(NTPCLIENT_INC) $(HOME_INC)/$(HTTPCLIENT_INC) $(HOME_INC)/$(HTTPTEXT_INC)
	@echo "*** $@ done ***"

links:
ifeq (,$(wildcard $(HOME_LIB)/$(LIBRARY).so))
		ln -s $(PWD)/$(LIBDIRS)$(TARGET) $(HOME_LIB)/$(LIBRARY).so
endif
ifeq (,$(wildcard $(HOME_INC)/$(SOCKETADDRESS_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SOCKETADDRESS_INC) $(HOME_INC)/$(SOCKETADDRESS_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(SOCKET_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SOCKET_INC) $(HOME_INC)/$(SOCKET_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(IPV4ADDRESS_INC)))
		ln -s $(PWD)/$(INCDIRS)$(IPV4ADDRESS_INC) $(HOME_INC)/$(IPV4ADDRESS_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(ABSTRACTHTTPDATA_INC)))
		ln -s $(PWD)/$(INCDIRS)$(ABSTRACTHTTPDATA_INC) $(HOME_INC)/$(ABSTRACTHTTPDATA_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(ABSTRACTCHANNEL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(ABSTRACTCHANNEL_INC) $(HOME_INC)/$(ABSTRACTCHANNEL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(CHANNELMGR_INC)))
		ln -s $(PWD)/$(INCDIRS)$(CHANNELMGR_INC) $(HOME_INC)/$(CHANNELMGR_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(RAWCHANNEL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(RAWCHANNEL_INC) $(HOME_INC)/$(RAWCHANNEL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(SERIALCHANNEL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SERIALCHANNEL_INC) $(HOME_INC)/$(SERIALCHANNEL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(UDPCHANNEL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(UDPCHANNEL_INC) $(HOME_INC)/$(UDPCHANNEL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(TCPCHANNEL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(TCPCHANNEL_INC) $(HOME_INC)/$(TCPCHANNEL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(SHAREDMEMORY_INC)))
		ln -s $(PWD)/$(INCDIRS)$(SHAREDMEMORY_INC) $(HOME_INC)/$(SHAREDMEMORY_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(MESSAGEQUEUE_INC)))
		ln -s $(PWD)/$(INCDIRS)$(MESSAGEQUEUE_INC) $(HOME_INC)/$(MESSAGEQUEUE_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(BEAGLEBONECOMMINIT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(BEAGLEBONECOMMINIT_INC) $(HOME_INC)/$(BEAGLEBONECOMMINIT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(BEAGLEBONECOMMEX_INC)))
		ln -s $(PWD)/$(INCDIRS)$(BEAGLEBONECOMMEX_INC) $(HOME_INC)/$(BEAGLEBONECOMMEX_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(NTPCLIENT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(NTPCLIENT_INC) $(HOME_INC)/$(NTPCLIENT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(HTTPCLIENT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(HTTPCLIENT_INC) $(HOME_INC)/$(HTTPCLIENT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(HTTPTEXT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(HTTPTEXT_INC) $(HOME_INC)/$(HTTPTEXT_INC)
endif

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
	@$(CP) $(INCDIRS)$(SOCKETADDRESS_INC) $(PATH_INSTALL_INC)/$(SOCKETADDRESS_INC)
	@$(CP) $(INCDIRS)$(SOCKET_INC) $(PATH_INSTALL_INC)/$(SOCKET_INC)
	@$(CP) $(INCDIRS)$(IPV4ADDRESS_INC) $(PATH_INSTALL_INC)/$(IPV4ADDRESS_INC)
	@$(CP) $(INCDIRS)$(ABSTRACTHTTPDATA_INC) $(PATH_INSTALL_INC)/$(ABSTRACTHTTPDATA_INC)
	@$(CP) $(INCDIRS)$(ABSTRACTCHANNEL_INC) $(PATH_INSTALL_INC)/$(ABSTRACTCHANNEL_INC)
	@$(CP) $(INCDIRS)$(CHANNELMGR_INC) $(PATH_INSTALL_INC)/$(CHANNELMGR_INC)
	@$(CP) $(INCDIRS)$(RAWCHANNEL_INC) $(PATH_INSTALL_INC)/$(RAWCHANNEL_INC)
	@$(CP) $(INCDIRS)$(SERIALCHANNEL_INC) $(PATH_INSTALL_INC)/$(SERIALCHANNEL_INC)
	@$(CP) $(INCDIRS)$(UDPCHANNEL_INC) $(PATH_INSTALL_INC)/$(UDPCHANNEL_INC)
	@$(CP) $(INCDIRS)$(TCPCHANNEL_INC) $(PATH_INSTALL_INC)/$(TCPCHANNEL_INC)
	@$(CP) $(INCDIRS)$(SHAREDMEMORY_INC) $(PATH_INSTALL_INC)/$(SHAREDMEMORY_INC)
	@$(CP) $(INCDIRS)$(MESSAGEQUEUE_INC) $(PATH_INSTALL_INC)/$(MESSAGEQUEUE_INC)
	@$(CP) $(INCDIRS)$(BEAGLEBONECOMMINIT_INC) $(PATH_INSTALL_INC)/$(BEAGLEBONECOMMINIT_INC)
	@$(CP) $(INCDIRS)$(BEAGLEBONECOMMEX_INC) $(PATH_INSTALL_INC)/$(BEAGLEBONECOMMEX_INC)
	@$(CP) $(INCDIRS)$(NTPCLIENT_INC) $(PATH_INSTALL_INC)/$(NTPCLIENT_INC)
	@$(CP) $(INCDIRS)$(HTTPCLIENT_INC) $(PATH_INSTALL_INC)/$(HTTPCLIENT_INC)
	@$(CP) $(INCDIRS)$(HTTPTEXT_INC) $(PATH_INSTALL_INC)/$(HTTPTEXT_INC)
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





