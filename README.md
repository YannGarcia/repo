# repo
Base repository for all my developments

1. Introduction
This development repository contains all my staff regarding embedded development for different kind of hardware
    Hardware Linux based:
        Raspberry PI
	BeagleBone
	NewThing C.H.I.P
    BareMetal Hardware:
        MBED, based on LPC1768FBD100
	TI LanchPad TM4C1xxx
	TI LaunchPad MSP432P01
	Microchip MCUs
	
2. Setup development environment
According to your OS of your board (Raspberry PI, BeagleBone, CHIP...):
    1. Create a link from the correct bash profile located into repo/bash_profiles/devenv.bash.<os|board> to ~/devenv.bash
        e.g. ln -s ~/repo/bash_profiles/devenv.bash.macosx ~/devenv.bash
    2. Update your own ~/.bash_profile to execute the development environment file
        if [ -f ~/devenv.bash ]
        then
            . ~/devenv.bash
        fi


