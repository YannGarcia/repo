
# repo #

Welcome to my working repository for all my electronics projects & developments
### Build status - Mac Os X, Linux, Cygwin gcc-6
[![Build Status](https://travis-ci.org/YannGarcia/repo.svg?branch=master)](https://travis-ci.org/YannGarcia/repo)
[![Coverage Status](https://coveralls.io/repos/YannGarcia/repo/badge.png?branch=master)](https://coveralls.io/r/YannGarcia/repo?branch=master)


## Introduction
This repository contains all my staff regarding embedded development for different kind of hardware

    1. OS development environment to co compile or cross-compile projects (see bash_profiles README), based on standard Linux disto
	
    2. Hardware Linux based:
        - Raspberry PI
        - BeagleBone
        - NewThing C.H.I.P
	
    3. BareMetal Hardware:
        - MBED, based on LPC1768FBD100
        - TI LanchPad TM4C1xxx
        - TI LaunchPad MSP432P01
        - Microchip MCUs
	
## Setup development environment
According to your OS (Linux based or MAC OS X) for cross-compiling or the board you are working on (Raspberry PI, BeagleBone, CHIP...), follow the stps below:

    1. Update your own ~/.bash_profile to execute the development environment file  
    
    ```bash
    if [ -f ~/devenv.bash ]
    then
			. ~/devenv.bash
    fi
    ```
    
    2. Create a link from the correct bash profile located into repo/bash_profiles/devenv.bash.<os|board> to ~/devenv.bash (e.g. ln -s ~/repo/bash_profiles/devenv.bash.macosx ~/devenv.bash)
    NOTE if you want to use my .emacs setting file, back up yur own .emacs file first, then create asoft link on my .emacs file:
    
      ```bash
      mv .emacs ~/tmp/
	    ln -sf ~/repo/emacs/.emacs ~/.emacs
      ```
    
    3. In your home directory, create the folders bin, lib, include, tmp
    
    ```bash
    mkdir -p bin lib include tmp
    ```
    
    4. Restart your current session
    
    5. Install the following frameworks into your frameworks directory ${HOME_FRAMEWORKS}
      - google test
      - Linaro cross compiler
      - Texas Instrument TIVA and MSP driver libraries
    NOTE Take a look to the ~/repo/travis_scripts/install.bash srcript for more details
    
    6. What to do now:
    	- Native build on standard Linux:
        1. Unset the env. variable CROSS_COMPILER_PATH
				
		      ```bash
		      unset CROSS_COMPILER_PATH
		      ```
					
        2. Change floder to ~/repo/dev/g++/projects/embedded
        3. Execute make test
				
		      ```bash
		      cd ~/repo/dev/g++/projects/embedded
          make test
		      ```
					
        4. Now you can build your own project
    	- Cross build on Linux:
        1. Unset the env. variable CROSS_COMPILER_PATH
				
		      ```bash
		      unset CROSS_COMPILER_PATH
		      ```
					
        2. Change floder to ~/repo/dev/g++/projects/embedded
        3. Execute make test
		      
					```bash
		      cd ~/repo/dev/g++/projects/embedded
          make test
		      ```
					
        4. Now you can build your own project
    	- Native build on Linux Hardware (RaspeberryPI, C.H.I.P. BBB...):
        1. Change floder to ~/repo/dev/g++/projects/embedded
        2. Execute make test
				
		      ```bash
		      cd ~/repo/dev/g++/projects/embedded
          make test
		      ```
					
        3. Now you can build your own project
    	- Cross build on standard Linux:
        1. TODO
        x. Now you can build your own project
        
If you do not want to follow these steps, update the devenv.bash accordingly by modifying variables HOME_BIN, HOME_INC, HOME_LIB...

## Tools ##
Here is a list of the tools I use to develop my projects:

    1. Electronic schematics & PCB design 
        - Eagle
        - DesignSpark PCB (Windows only) (https://www.rs-online.com/designspark/pcb-software)
        - Fritzing (http://fritzing.org/home/)

    2. Digital analyzer
        - Saleae Logic (https://www.saleae.com/?gclid=Cj0KEQiAhNnCBRCqkP6bvOjz_IwBEiQAMn_TMQpUtSRAZjHYZiuPgMPIOkSjVyFC7Ki9zCnTHwnSNB4aAnKW8P8HAQ)

    3. Compilers
        - GCC for C/C++ develpment for Hardware Linux based
        - GNU ARM Embedded Toolchain for Baremetal hardware (https://launchpad.net/gcc-arm-embedded)
	- XCode (MAC OS X)

    4. Simulation
        - LTspice XII
        - Matlab R2015a

    5. Texas Instrument specific
        - Code Composer Studio (CCS)

    7. Serial communication
				- GNU screen (Linux, MAC OS X)
				- PuTTY (Windows)

    6. Code documentation
    	- Doxygen

    7. C++ test unit framework
        - cpptest: http://cpptest.sourceforge.net/
	           CppTest is released under the GNU Lesser General Public License
	        NOTE For MAC OS X, I had to use the following configure settings
    
	       ```bash
	       ./configure CC=gcc-6 CXX=g++-6 AR=/usr/bin/ar
	       ```
         
	       For raspberry pi, I had to use the following configure settings
         
	       ```bash
	       ./configure CXXFLAGS="-g -O2 -std=c++11"
	       ```
         
	       With gcc-6, I had also to replace std::auto_ptr by std::unique_ptr (in <memory>) because of std::auto_ptr was deprecated
	       
    8. C++ instrumentation framework & profiling
        - Valgrind (http://valgrind.org)
	       
    9. Continuous build integration with TRAVIS CI
        - See .travis.yml and travis_scripts/trigger_build.bash
        - See .coveralls.yml for code coverage information
	       
That's all Folks
