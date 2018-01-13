include $(PATH_DEV)/Makeinclude

# Define OS specific varaibles
UNAME := $(shell uname -n)
ifeq ($(UNAME), imac.lan)
include $(PATH_DEV)/Makeinclude.macosx
else ifeq ($(UNAME), mbp-de-garcia.home)
include $(PATH_DEV)/Makeinclude.macosx
else ifeq ($(UNAME), debian32)
include $(PATH_DEV)/Makeinclude.debian
else ifeq ($(UNAME), raspberrypi)
include $(PATH_DEV)/Makeinclude.raspberry
else ifeq ($(UNAME), beaglebone)
include $(PATH_DEV)/Makeinclude.bbb
else ifeq ($(UNAME), yann-FSCOM)
include $(PATH_DEV)/Makeinclude.cygwin
else
include $(PATH_DEV)/Makeinclude.ubuntu
endif

#############################################################################
# C/C++ Project path
#############################################################################
PATH_DEV_CPP=${PATH_DEV}/g++/projects

#############################################################################
# IFSTTAR C/C++ Project path
#############################################################################
PATH_IFSTTAR_PROJECTS=${PATH_DEV_CPP}/../ifsttar

#############################################################################
# Default installtion paths path
#############################################################################
PATH_INSTALL=/usr/local/
PATH_INSTALL_INC=${PATH_INSTALL}/include
PATH_INSTALL_BIN=${PATH_INSTALL}/bin
PATH_INSTALL_LIB=${PATH_INSTALL}/lib
PATH_INSTALL_ETC=${PATH_INSTALL}/etc
PATH_INSTALL_DOC=${PATH_INSTALL}/doc
PATH_INSTALL_SHARE=${PATH_INSTALL}/share
PATH_INSTALL_DEBIAN=${PATH_INSTALL}/debian

#############################################################################
# Dotnet Project paths
#############################################################################

#############################################################################
# EEA Project paths
#############################################################################

#############################################################################
# IOBoard4PIC application.
#############################################################################
PATH_IOBoard4PIC=${PATH_DEV_EEA_PROJECTS}/IOBoard4PIC

#############################################################################
# TestUnit application.
#############################################################################
PATH_TestUnits=${PATH_DEV_EEA_PROJECTS}/TestUnits

#############################################################################
# Proto Electrovalve application.
#############################################################################
PATH_ProtoElectricWaterValve=${PATH_DEV_EEA_PROJECTS}/ProtoElectricWaterValve

