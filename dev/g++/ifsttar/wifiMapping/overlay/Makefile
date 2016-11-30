
#############################################################################
#
#       Makefile for IFSTTAR WiFi Mapping prototype
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc

#############################################################################
# Libraries and options.

CC_INS=-O dtb -b 0 -@

#############################################################################
# Module variables.

# Paths
BINDIR=.
BINDIRS=./
SRCDIR=.
SRCDIRS=./
OBJDIR=.
OBJDIRS=./

# Targets
CAPE_VERSION=00A0
CAPE_NAME=cape-bbb-ifsttar
TARGET=$(CAPE_NAME)-$(CAPE_VERSION).dtbo
BIN=$(BINDIRS)$(TARGET)

MODULE_BIN=$(BIN)

# Object app
OBJS=\
	$(MAIN_OBJ)

MAIN_OBJ=$(CAPE_NAME).dtbo


#############################################################################
# Real compilation sections.

all: $(MAIN_OBJ)
	@echo "*** $@ done ***"

$(MAIN_OBJ): $(SRCDIRS)$(MAIN_OBJ:.dtbo=.dts)
	$(DTC) $(CC_OPTION) $? -o $(TARGET)
	$(SUDO) $(CP) $(BIN) $(PATH_LIB_FRMW)
	$(CD) $(PATH_LIB_FRMW)
	$(SUDO) $(SU) -c "echo $(CAPE_NAME) > $(SLOTS)"
	$(CD) -
	@echo "*** $@ done ***"

compile :
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_INS)" \
		all
	dmesg

recompile :
	${MAKE} -f Makefile clean
	${MAKE} -f Makefile \
		CC_OPTION="$(CC_INS)" \
		all
	dmesg

clean:
	@$(RM) -f $(BINS)$(TARGET) *~
	@echo "*** $@ done ***"

install:
	@$(CP) $(BIN) $(PATH_INSTALL_BIN)
	@$(CP) ifsttard $(PATH_INSTALL_BIN)
	@echo "*** $@ done ***"

help:
	@clear
	@echo
	@echo "make options are:"
	@echo
	@echo "   help      : This menu."
	@echo "   clean     : Erase objects, binaries and temporary files."
	@echo "   compile   : Build the Device Tree Overlay blob."
	@echo "   recompile : Rebuild the Device Tree Overlay blob."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)/xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo
	@echo "Without option it compilates in optimized mode."
	@echo
	@echo "*** $@ done ***"

