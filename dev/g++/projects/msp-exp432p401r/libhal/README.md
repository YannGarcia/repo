v# libhal project

## Introduction
The name libhal stands for library for Hardware Abstract Layer. The idea is to provide the same interface for all my electronic hardwares (Linux base hardware or Baremetal hardware)
NOTE  The test application (test/testlib.c) contains non optimised code (no read/wirte functions...). The purpose is to provide step by step tests

## How to build the project
Pleas foLlow the steps below:

```bash
        # Change to libhal directory    
        cd ~/repo/dev/g++/projects/ek-tm4c12x/libhal/objs
        # Display help 
        make help
        # Clean up
        make clean
        # Build the library
        make compile
        # Build the demo application
        make test
        # Upload to the baremetal
        make upload
        # Generate documentation
        make gendoc
```

## Developing your hardware testing
The hardware testing is a special hardware used to validate embedded development.
This hardware provides test capabilities for both Digital and analog testing. It provides also diffrerent kind of sensors..

### Digital testing
Digital testing includes:
    * LED switch on/off
    * Switch checking, debouncing...
    * Transistor command

### Analog testing
    * ADC testing capabilityIt provides the following features
    * DAC testing capability

### ICs communication
    * SPI
    * I2C
    * UART

### Sensors capabilities
    * Gyroscope, Accelerometer
    * Hall effect
    * Ambiant Light sensor
    * Temperature sensor

### Timers capabilities
TODO

### Interupt capabilities
    * External interrupts
    * Timers interupts
    * ADC interupts
TODO

### Schematic
TODO

### PCB and realisation
TODO

## Using the TI Educational BoosterPack MKII
It provides the followings features (http://www.ti.com/tool/BOOSTXL-EDUMKII):

    * TI OPT3001 Light Sensor
    * TI TMP006 Temperature Sensor
    * Servo Motor Connector
    * 3-Axis Accelerometer
    * User Push Buttons
    * RGB Multi-color LED
    * Buzzer
    * 40-pin Stackable BoosterPack Connector
    * Color TFT LCD Display
    * Microphone
    * 2-Axis Joystick with Pushbutton

See BoosterPack Checker (https://dev.ti.com/bpchecker/#/) to get BoosterPacks pin mapping compatibiliy.

# Using GNU screen
    * screen /dev/tty.usbmodem0F8002B1 115200 8N1

TODO Continue

That's all Folks
