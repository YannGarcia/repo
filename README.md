
# repo #

Welcome to mase repository for all my developments

## 1. Introduction
This development repository contains all my staff regarding embedded development for different kind of hardware

    1. Hardware Linux based:
        - Raspberry PI
        - BeagleBone
        - NewThing C.H.I.P
    2. BareMetal Hardware:
        - MBED, based on LPC1768FBD100
        - TI LanchPad TM4C1xxx
        - TI LaunchPad MSP432P01
        - Microchip MCUs
	
## 2. Setup development environment
According to your OS of your board (Raspberry PI, BeagleBone, CHIP...):

    1. Create a link from the correct bash profile located into repo/bash_profiles/devenv.bash.<os|board> to ~/devenv.bash (e.g. ln -s ~/repo/bash_profiles/devenv.bash.macosx ~/devenv.bash)
    2. Update your own ~/.bash_profile to execute the development environment file
    
```bash
        if [ -f ~/devenv.bash ]
        then
            . ~/devenv.bash
        fi
```	

## 3. Hardware testing ##
The hardware testing is a special hardware used to validate embedded development.
This hardware provides test capabilities for both Digital and analog testing. It provides also diffrerent kind of sensors..

### 1. Digital testing ###
Digital testing includes:
    * LED switch on/off
    * Interrupt checking, debouncing...
    * external interrupt
    * Transistor command

### 2. Analog testing ###
    * ADC testing capability
    * DAC testing capability

### Sensors cqpqbilities ###
    * Gyroscope, Accelerometer
    * Hall effect
    * 

### Schematic ###

### PCB and realisation ###

END
