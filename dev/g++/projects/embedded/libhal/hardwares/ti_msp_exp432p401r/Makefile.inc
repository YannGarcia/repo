include ../Makeinclude

#############################################################################
# Select MSP board

# Part for MSP4322P401R
PART=MSP432P401R

#############################################################################
# Import the correct makefiles

ifeq ($(PART), MSP432)
include ../Makeinclude.msp432p401r
#else ifeq ($(PART), MSP???)
#include ../Makeinclude.msp???
endif
