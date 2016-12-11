
# repo #

Welcome to mase repository for all my developments

## Introduction ##
This development repository contains all my staff regarding embedded development for different kind of hardware
    * Hardware Linux based:
        Raspberry PI
	BeagleBone
	NewThing C.H.I.P
    * BareMetal Hardware:
        MBED, based on LPC1768FBD100
	TI LanchPad TM4C1xxx
	TI LaunchPad MSP432P01
	Microchip MCUs
	
## Setup development environment ##
According to your OS of your board (Raspberry PI, BeagleBone, CHIP...):
    1. Create a link from the correct bash profile located into repo/bash_profiles/devenv.bash.<os|board> to ~/devenv.bash
        e.g. ln -s ~/repo/bash_profiles/devenv.bash.macosx ~/devenv.bash
    2. Update your own ~/.bash_profile to execute the development environment file
        if [ -f ~/devenv.bash ]
        then
            . ~/devenv.bash
        fi

## Hardware testing ##
The hardware testing is a special hardware used to validate embedded development.
This hardware provides test capabilities for both Digital and analog testing. It provides also diffrerent kind of sensors..

### Digital testing ###
Digital testing includes:
* LED switch on/off
* Interrupt checking, debouncing...
* external interrupt
* Transistor command

### Analog testing ###
* ADC testing capability
* DAC testing capability

### Sensors cqpqbilities ###
* Gyroscope, Accelerometer
* Hall effect
* 

### Schematic ###

### PCB and realisation ###

