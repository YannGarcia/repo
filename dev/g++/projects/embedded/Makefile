
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

projects=logger converter helper ipc #comm uart gps

clean:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile clean; \
		cd -; \
	done

debug:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile debug; \
		cd -; \
	done

coverage:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile coverage; \
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

test_c:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile test_c; \
		cd -; \
	done

test_v:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile test_v; \
		cd -; \
	done

run:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile run; \
		cd -; \
	done

run_c:
	@for project in $(projects); do \
		cd "$$project"/objs && $(MAKE) -f Makefile run_c; \
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
	@echo "   debug    : Build the full application in debug mode."
	@echo "   gendoc       : Build Doxygen documentation."
	@echo "   install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)xxx."
	@echo "   install-devel: Same as 'install' plus include files."
	@echo "   test         : ."
	@echo "*** $@ done ***"

#############################################################################
# DO NOT DELETE
