include ../../Makeinclude

#############################################################################
# Select TIVA C board

# Part for EK-TM4C1294XL
#PART=TM4C1294NCPDT

# Part for EK-TM4C129EXL
PART=TM4C129ENCPDT

# Part of DK-TM4C123G
#PART=TM4C123GH6PMI

#############################################################################
# Import the correct makefiles

ifeq ($(PART), TM4C1294NCPDT)
include ../../Makeinclude.tm4c129x
else ifeq ($(PART), TM4C129ENCPDT)
include ../../Makeinclude.tm4c129x
else ifeq ($(PART), TM4C123GH6PMI)
include ../../Makeinclude.tm4c123x
endif
