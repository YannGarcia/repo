
#libhal framework for embedded application

This is a hardware abstraction layer wich provide a common interface far all supported cards such as Texas Instrument TIVA/MSP hardware, Arduino hardware, C.H.I.P or raspberry hardware, or BeagleBone hardware.

##Features
The libhal framework provides the following features
* Common basic functionalities (library initialisation, GPIO operations, basic analog operations, PWM...)
* I2C bus support
* SPI bus support
* UART support

##Synoptic
The libhal frame work provide a common set of include files
In a terminal, execute the command make gendoc to generate the documentation

##Usage
The libhal framework is quite simple to use and very intuitive.

### Select your hardware
All the Makefiles files use te common variable SELECTED_HWD which indicates the hardware to be used. Based on this variable, the proper libhal framework is built. It can be used for all your rojects based on the selected hardware.
