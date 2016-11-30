
#############################################################################
#
#       Makefile for IFSTTAR WiFiMapper project
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc
include $(PATH_IFSTTAR_PROJECTS)/Makefile.ifsttar

#############################################################################
# List of projects

projects=wifimapping gpsmon iperfmon guimon tempmon rtcmon aggreg tsharkmon
exec=wifimapping

clean:
	for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile clean; \
		cd -; \
	done

compile_d:
	for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile compile_d; \
		cd -; \
	done

compile:
	for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile compile; \
		cd -; \
	done

recompile:
	for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile recompile; \
		cd -; \
	done

install:
	project=$(projects) overlay
	for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile install; \
		cd -; \
	done

install-devel:
	project=$(projects) overlay
	for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile install-devel; \
		cd -; \
	done

run: compile
	cd $(exec)/objs && $(MAKE) -f Makefile run > out.txt 2>&1 3>&1; \
	stty sane; \
	cd -; \
	sleep 3; \
	ipcs -mq; \
	ps aux | egrep 'iperfmon|wifimapping|guimon|gpsmon|tshark|accmon|tempmon|rtcmon|aggreg' | grep -v egrep;

run1: compile
	cd $(exec)/objs && $(MAKE) -f Makefile run1 > out.txt 2>&1 3>&1; \
	stty sane; \
	cd -; \
	sleep 3; \
	ipcs -mq; \
	ps aux | egrep 'iperfmon|wifimapping|guimon|gpsmon|tshark|accmon|tempmon|rtcmon|aggreg' | grep -v egrep;

help:
	@clear
	@echo
        @echo "make options are:"
	@echo
	@echo "   help         : This menu."
	@echo "   clean        : Erase objects, binaries and temporary files."
	@echo "   compile      : Build the full application in release mode."
	@echo "   recompile    : Rebuild the full application in release mode."
	@echo "   compile_d    : Build the full application in debug mode."
	@echo "   run          : Execute the main application."
	@echo "   run1         : Execute the main application with autoMode set to false."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)/xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo
	@echo "*** $@ done ***"
