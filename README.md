
# repo #

Welcome to my working repository for all my electronics projects & developments

### Build status - Mac Os X, Linux, Cygwin
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
        - TI LanchPad TM4C1xxx (TIVA family)
        - TI LaunchPad MSP432P0r (MSP432 family)
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
    
    5. Install and build the following frameworks into your frameworks directory ${HOME_FRAMEWORKS}
      - google test (git clone https://github.com/google/googletest.git googletest)
        NOTE For native Linux Hardware, in travis.sh script, you shoud change this: -Dgmock_build_tests=ON to -Dgmock_build_tests=NO
      - Linaro cross compiler (e.g. https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2)
      - Texas Instrument TIVA and MSP driver libraries
    NOTE Take a look to the ~/repo/travis_scripts/install.bash srcript for more details
    
    6. What to do now:
    
      1. Native build on standard Linux:
      
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
				
			2. Cross build on Linux:
      
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
        
      3. Native build on Linux Hardware (RaspeberryPI, C.H.I.P. BBB...):
      
          1. Change floder to ~/repo/dev/g++/projects/embedded
        
          2. Execute make test
				
		      ```bash
          cd ~/repo/dev/g++/projects/embedded
          make test
          ```
					
          3. Now you can build your own project
          
      4. Cross build on standard Linux:
      
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

    7. C++ instrumentation framework & profiling
        - Valgrind (http://valgrind.org)
	       
    8. Continuous build integration with TRAVIS CI
        - See .travis.yml and travis_scripts/trigger_build.bash
        - See .coveralls.yml for code coverage information

## Folders organisation ##

Please switch to RAW view first ;-)
You will find below is my current organisation of my repo folder:

.
├── bash_profiles
├── dev
│   ├── eea
│   │   └── arduino
│   │       ├── blink
│   │       │   └── objs
│   │       └── WIKSketch
│   │           └── objs
│   └── g++
│       ├── ifsttar
│       │   └── wifiMapping
│       │       ├── accmon
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── aggreg
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── common
│       │       │   ├── include
│       │       │   └── src
│       │       ├── gpsmon
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── guimon
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── gyromon
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── iperfmon
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── overlay
│       │       ├── rtcmon
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── tempmon
│       │       │   ├── bin
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       ├── tsharkmon
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   └── src
│       │       └── wifimapping
│       │           ├── bash
│       │           ├── bin
│       │           ├── etc
│       │           ├── include
│       │           ├── objs
│       │           └── src
│       ├── projects
│       │   ├── beaglebone
│       │   │   ├── beagleboneComm
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── beagleboneGeo
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── beagleboneGps
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── beagleboneHw
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── beagleboneKml
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── beagleboneLcd
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── beagleboneThingSpeak
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── beagleboneUtils
│       │   │   │   ├── include
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── libhal
│       │   │   │   ├── designspark
│       │   │   │   ├── docs
│       │   │   │   ├── dtc
│       │   │   │   ├── include
│       │   │   │   ├── lib
│       │   │   │   ├── objs
│       │   │   │   ├── saleae logic
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   └── wiringBbb
│       │   │       ├── include
│       │   │       ├── objs
│       │   │       ├── src
│       │   │       └── test
│       │   ├── ek-tm4c12x
│       │   │   ├── dds
│       │   │   │   ├── include
│       │   │   │   ├── ld
│       │   │   │   ├── objs
│       │   │   │   └── src
│       │   │   └── libhal
│       │   │       ├── ccs
│       │   │       │   ├── Debug
│       │   │       │   └── targetConfigs
│       │   │       ├── docs
│       │   │       ├── include
│       │   │       ├── ld
│       │   │       ├── lib
│       │   │       ├── objs
│       │   │       ├── saleae logic
│       │   │       ├── src
│       │   │       └── test
│       │   ├── embedded
│       │   │   ├── comm
│       │   │   │   ├── docs
│       │   │   │   ├── include
│       │   │   │   ├── lib
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── converter
│       │   │   │   ├── docs
│       │   │   │   ├── include
│       │   │   │   ├── lib
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── gps
│       │   │   │   ├── include
│       │   │   │   ├── lib
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── helper
│       │   │   │   ├── docs
│       │   │   │   ├── include
│       │   │   │   ├── lib
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── ipc
│       │   │   │   ├── docs
│       │   │   │   ├── include
│       │   │   │   ├── lib
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   ├── libhal
│       │   │   │   ├── docs
│       │   │   │   ├── hardwares
│       │   │   │   │   ├── raspberry_pi
│       │   │   │   │   │   ├── bin
│       │   │   │   │   │   ├── include
│       │   │   │   │   │   ├── lib
│       │   │   │   │   │   ├── objs
│       │   │   │   │   │   ├── src
│       │   │   │   │   │   └── test
│       │   │   │   │   └── ti_msp_exp432p401r
│       │   │   │   │       ├── bin
│       │   │   │   │       ├── include
│       │   │   │   │       ├── lib
│       │   │   │   │       ├── objs
│       │   │   │   │       ├── src
│       │   │   │   │       └── test
│       │   │   │   ├── include
│       │   │   │   └── objs
│       │   │   ├── logger
│       │   │   │   ├── docs
│       │   │   │   ├── include
│       │   │   │   ├── lib
│       │   │   │   ├── objs
│       │   │   │   ├── src
│       │   │   │   └── test
│       │   │   └── uart
│       │   │       ├── include
│       │   │       ├── lib
│       │   │       ├── objs
│       │   │       ├── src
│       │   │       └── test
│       │   ├── mbed
│       │   │   └── ProtoWaterElectroValve
│       │   │       ├── DebugLibrary
│       │   │       ├── EthernetInterface
│       │   │       │   ├── lwip
│       │   │       │   │   ├── api
│       │   │       │   │   ├── core
│       │   │       │   │   │   ├── ipv4
│       │   │       │   │   │   └── snmp
│       │   │       │   │   ├── include
│       │   │       │   │   │   ├── ipv4
│       │   │       │   │   │   │   └── lwip
│       │   │       │   │   │   ├── lwip
│       │   │       │   │   │   └── netif
│       │   │       │   │   └── netif
│       │   │       │   │       └── ppp
│       │   │       │   ├── lwip-eth
│       │   │       │   │   └── arch
│       │   │       │   │       └── TARGET_NXP
│       │   │       │   ├── lwip-sys
│       │   │       │   │   └── arch
│       │   │       │   └── Socket
│       │   │       ├── GPS
│       │   │       ├── Hardware
│       │   │       ├── Helpers
│       │   │       ├── HTTPClient
│       │   │       │   └── data
│       │   │       ├── mbed
│       │   │       │   └── TARGET_LPC1768
│       │   │       │       ├── TARGET_NXP
│       │   │       │       │   └── TARGET_LPC176X
│       │   │       │       │       └── TARGET_MBED_LPC1768
│       │   │       │       └── TOOLCHAIN_GCC_ARM
│       │   │       ├── mbed-rtos
│       │   │       │   ├── rtos
│       │   │       │   └── rtx
│       │   │       │       └── TARGET_M3
│       │   │       │           └── TOOLCHAIN_GCC
│       │   │       └── Network
│       │   ├── msp-exp432p401r
│       │   │   └── libhal
│       │   │       ├── ccs
│       │   │       │   └── targetConfigs
│       │   │       ├── docs
│       │   │       ├── include
│       │   │       ├── ld
│       │   │       ├── lib
│       │   │       ├── objs
│       │   │       ├── saleae logic
│       │   │       ├── src
│       │   │       └── test
│       │   ├── raspberry
│       │   │   └── libhal
│       │   │       ├── include
│       │   │       ├── lib
│       │   │       ├── objs
│       │   │       ├── src
│       │   │       └── test
│       │   └── ttcn3
│       │       ├── internal_tci
│       │       │   ├── include
│       │       │   ├── objs
│       │       │   ├── src
│       │       │   └── test
│       │       ├── internal_tri
│       │       │   ├── include
│       │       │   ├── lib
│       │       │   ├── objs
│       │       │   ├── src
│       │       │   └── test
│       │       └── internal_tritci
│       │           ├── include
│       │           └── src
│       └── test
│           └── raspstill
│               ├── EGL
│               ├── GLES
│               ├── interface
│               │   ├── khronos
│               │   │   ├── common
│               │   │   │   ├── abstract
│               │   │   │   ├── direct
│               │   │   │   ├── linux
│               │   │   │   ├── openwfc
│               │   │   │   ├── vcos
│               │   │   │   └── vcos_vchiq
│               │   │   ├── egl
│               │   │   ├── ext
│               │   │   ├── glxx
│               │   │   ├── include
│               │   │   │   ├── EGL
│               │   │   │   ├── GLES
│               │   │   │   ├── GLES2
│               │   │   │   ├── KHR
│               │   │   │   ├── VG
│               │   │   │   └── WF
│               │   │   ├── vg
│               │   │   └── wf
│               │   ├── mmal
│               │   │   ├── core
│               │   │   ├── util
│               │   │   └── vc
│               │   ├── peer
│               │   ├── usbdk
│               │   │   ├── test
│               │   │   └── vmcs_rpc_client
│               │   ├── vchi
│               │   │   ├── common
│               │   │   ├── connections
│               │   │   └── message_drivers
│               │   ├── vchiq_arm
│               │   ├── vcos
│               │   │   ├── generic
│               │   │   ├── glibc
│               │   │   └── pthreads
│               │   ├── vctypes
│               │   └── vmcs_host
│               │       ├── khronos
│               │       │   └── IL
│               │       └── linux
│               │           └── vcfiled
│               │               └── etc
│               │                   └── init.d
│               ├── KHR
│               └── vcinclude
├── docs
├── emacs
│   └── emacs_ttcn
└── travis_scripts


That's all Folks
