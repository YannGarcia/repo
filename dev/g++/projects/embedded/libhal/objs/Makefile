#############################################################################
#
#       Makefile for libhal project
#
#       ident  "$Id$"
#
#############################################################################

clean:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile clean; \
	cd -
endif

debug:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile debug; \
	cd -
endif

compile:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile compile; \
	cd -
endif

recompile:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile recompile; \
	cd -
endif

gendoc:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile gendoc; \
	cd -
endif

install:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile install; \
	cd -
endif

install-devel:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile install-devel; \
	cd -
endif

test:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile test; \
	cd -
endif

test_d:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile test_d; \
	cd -
endif

test_v:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile test_v; \
	cd -
endif

run:
ifeq ($(SELECTED_HW), raspberry_pi)
	cd ../hardware/$(SELECTED_HW)/objs && $(MAKE) -f Makefile run; \
	cd -
endif

help:
	@clear
	@echo
	@echo "make options are:"
	@echo
	@echo "   help         : This menu."
	@echo "   clean        : Erase objects, binaries and temporary files."
	@echo "   compile      : Build the full application in release mode."
	@echo "   debug    : Build the full application in debug mode."
	@echo "   gendoc       : Build Doxygen documentation."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo "   test         : ."
	@echo "*** $@ done ***"

#############################################################################
# DO NOT DELETE
