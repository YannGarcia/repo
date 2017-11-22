#############################################################################
#
#       Makefile for libhal project
#
#       ident  "$Id$"
#
#############################################################################

LIBHALINC=libhal.h
LIBHALI2C_INC=libhal_i2c.h
LIBHALSPI_INC=libhal_spi.h
LIBHALSERIAL_INC=libhal_serial.h
LIBHALSHIFT_INC=libhal_shift.h
LIBHALPINNAMES_INC=libhal_pin_names.h
INC_MODULES=$(LIBHALINC) $(LIBHALI2C_INC) $(LIBHALSPI_INC) $(LIBHALSERIAL_INC) $(LIBHALSHIFT_INC) $(LIBHALPINNAMES_INC)

INCDIRS=../include/

clean: cleanlinks
ifneq ($(SELECTED_HW),)
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile clean; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

debug:
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile debug; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

compile: links
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile compile; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

recompile: clean
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile recompile; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

gendoc:
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile gendoc; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

install:
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile install; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

install-devel:
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile install-devel; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

test: links
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile test; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

test_d:
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile test_d; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

test_v:
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile test_v; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

run:
ifneq ($(SELECTED_HW), )
	cd ../hardwares/$(SELECTED_HW)/objs && $(MAKE) -f Makefile run; \
	cd -
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

links:
ifneq ($(SELECTED_HW),)
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALINC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALINC) $(HOME_INC)/$(LIBHALINC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALI2C_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALI2C_INC) $(HOME_INC)/$(LIBHALI2C_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALSPI_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALSPI_INC) $(HOME_INC)/$(LIBHALSPI_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALSERIAL_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALSERIAL_INC) $(HOME_INC)/$(LIBHALSERIAL_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALSHIFT_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALSHIFT_INC) $(HOME_INC)/$(LIBHALSHIFT_INC)
endif
ifeq (,$(wildcard $(HOME_INC)/$(LIBHALPINNAMES_INC)))
		ln -s $(PWD)/$(INCDIRS)$(LIBHALPINNAMES_INC) $(HOME_INC)/$(LIBHALPINNAMES_INC)
endif
else
	@echo "The env. variable SELECTED_HW is net set"
endif
	@echo "*** $@ done ***"

cleanlinks:
	@for header in $(INC_MODULES); do \
		$(RM) $(HOME_INC)/$$header; \
	done
	@echo "*** $@ done ***"

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
	@echo "   test         : Build and execute the test application."
	@echo "--------------------------------------------------------------------------------------------------------------"
	@echo "   NOTE         : The env. variable SELECTED_HW shall be set"
	@echo "                : Possible values are:"
	@echo "                :   raspberry_pi, for Raspberry PI family"
	@echo "                :   chip for C.H.I.P hardware"
	@echo "                :   bbb for Beagle Bone Black family"
	@echo "                :   ti_msp_exp432p401r for TI MSP432P family"
	@echo "*** $@ done ***"

#############################################################################
# DO NOT DELETE
