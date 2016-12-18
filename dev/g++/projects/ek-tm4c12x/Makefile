
#############################################################################
#
#       Makefile for Libraries project
#
#       ident  "$Id$"
#
#############################################################################

include $(PATH_DEV)/Makefile.inc

#############################################################################
# List of projects

projects=hello libhal

clean:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile clean; \
		cd -; \
	done

compile_d:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile compile_d; \
		cd -; \
	done

compile:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile compile; \
		cd -; \
	done

recompile:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile recompile; \
		cd -; \
	done

gendoc:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile gendoc; \
		cd -; \
	done

install:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile install; \
		cd -; \
	done

install-devel:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile install-devel; \
		cd -; \
	done

test:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile test; \
		cd -; \
	done

help:
	@clear
	@echo
        @echo "make options are:"
	@echo
	@echo "   help         : This menu."
	@echo "   clean        : Erase objects, binaries and temporary files."
	@echo "   compile      : Build the full application in release mode."
	@echo "   compile_d    : Build the full application in debug mode."
	@echo "   gendoc       : Build Doxygen documentation."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)/xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo "   test         : ."
