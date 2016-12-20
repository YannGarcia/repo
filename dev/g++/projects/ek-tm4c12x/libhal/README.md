# libhal project

## Introduction
The name libhal stands for library for Hardware Abstract Layer. The idea is to prpvide the same interface for all my electronic hardware (Linux base hardware or Baremetal hardware)


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
    * Interrupt checking, debouncing...
    * external interrupt
    * Transistor command

### Analog testing
    * ADC testing capabilityIt provides the following features
    * DAC testing capability

### Sensors capabilities
    * Gyroscope, Accelerometer
    * Hall effect
    * 

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

See http://www.ti.com/tool/BOOSTXL-EDUMKII for BoosterPacks compatibiliy.

TODO Continue

That's all Folks
