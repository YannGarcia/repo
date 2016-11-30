
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

projects=libhalApp ProtoWaterElectroValve

clean:
	@for project in $(projects); do \
		cd "$$project" && $(MAKE) -f Makefile clean; \
		cd -; \
	done

all:
	@for project in $(projects); do \
		cd "$$project" && $(MAKE) -f Makefile compile_d; \
		cd -; \
	done

help:
	@clear
	@echo
        @echo "make options are:"
	@echo
	@echo "   help         : This menu."
	@echo "   clean        : Erase objects, binaries and temporary files."
	@echo "   all          : Build the full application in release mode."
