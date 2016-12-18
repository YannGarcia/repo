
# repo #

Welcome to my working repository for all my electronics projects & developments

## Introduction
This repository contains all my staff regarding embedded development for different kind of hardware

    1. Hardware Linux based:
        - Raspberry PI
        - BeagleBone
        - NewThing C.H.I.P
	
    2. BareMetal Hardware:
        - MBED, based on LPC1768FBD100
        - TI LanchPad TM4C1xxx
        - TI LaunchPad MSP432P01
        - Microchip MCUs
	
## Setup development environment
According to your OS (Linux based or MAC OX X) for cross-compiling or the board you are working on (Raspberry PI, BeagleBone, CHIP...), follow the stps below:

    1. In your home directory, create the folders bin, lib, include, tmp: ```bash mkdir -p bin lib include tmp```
    
    2. Create a link from the correct bash profile located into repo/bash_profiles/devenv.bash.<os|board> to ~/devenv.bash (e.g. ln -s ~/repo/bash_profiles/devenv.bash.macosx ~/devenv.bash)
    
    3. Update your own ~/.bash_profile to execute the development environment file
    
```bash
        if [ -f ~/devenv.bash ]
        then
            . ~/devenv.bash
        fi
```	

If you do not want to follow these steps, update the devenv.bash accordingly by modifying variables HOME_BIN, HOME_INC, HOME_LIB...

## Tools ##
Here is a list of the tools I use to develop my projects:

    1. Electronic schematics & PCB design 
        - Eagle
        - DesignSpark PCB (Windows only) (https://www.rs-online.com/designspark/pcb-software)
        - Fritzing (http://fritzing.org/home/)

    2. Digital analyzer
        - Saleae Logic (https://www.saleae.com/?gclid=Cj0KEQiAhNnCBRCqkP6bvOjz_IwBEiQAMn_TMQpUtSRAZjHYZiuPgMPIOkSjVyFC7Ki9zCnTHwnSNB4aAnKW8P8HAQ)

    3. Compiler
        - GCC for C/C++ develpment for Hardware Linux based
        - GNU ARM Embedded Toolchain for Baremetal hardware (https://launchpad.net/gcc-arm-embedded)

    4. Simulation
        - LTspice XII
        - Matlab R2015a

    5. Texas Instrument specific
        - Code Composer Studio (CCS)


That's all Folks
