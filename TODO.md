
# repo

This file contains the todo list regarding my different projects.

## Staff on libhal project
The main objective is to validate the libhal concept on my different hardwares, starting with Baremetal hardware (TI launchpad).
The next step will be to develop the same libhal concept on my Linux hardwares.

In the same time, I will develop a test card to validate this libhal concept. This hardware should provide test capabilities for both Digital and analog testing. It provides also diffrerent kind of sensors..

### Digital testing

Digital testing includes: * LED switch on/off * Interrupt checking, debouncing... * external interrupt * Transistor command

###  Analog testing

* ADC testing capability
* DAC testing capability

### Sensors capabilities

* Gyroscope, Accelerometer
* Hall effect
* ...

## Developement of common embedded peaces of code for Linux based hardware
The main objective is to basic lightweight software libraries such as logging, communication, NMEA support and so on for my different Linux based hardwares 

## GoogleTest Framework Support

* Rebuild framework with ARM compiler
* Apply it to LaunchPad TM4C

## Existing Project Review
* Makefile
  - Review help
* Doxygen
  - Review docs/o2.cfg
  - Add doxygen tags in code
  - Review file header
  - Review README & Mainpage
* Test code review
  - Enhance test units

## Planning for the furture
* Development chain tool for each hardware
* TI
  - Terminate TM4C129 libhal
  - Validate TM4C123 libahl
  - Terminate Power Supply
  - ...
* PI/CHIP/BBB
  - Validate libhal
  - Add support of the CCD
  - ...
* Microchip/dsPIC33/DSP
  - ...
* Vegetable garden
  - Automate watering
  - Electric fencing

That's all folks
