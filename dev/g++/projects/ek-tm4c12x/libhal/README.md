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

## Hardware testing
The hardware testing is a special hardware used to validate embedded development.
This hardware provides test capabilities for both Digital and analog testing. It provides also diffrerent kind of sensors..

### Digital testing
Digital testing includes:
    * LED switch on/off
    * Interrupt checking, debouncing...
    * external interrupt
    * Transistor command

### Analog testing
    * ADC testing capability
    * DAC testing capability

### Sensors capabilities
    * Gyroscope, Accelerometer
    * Hall effect
    * 

### Schematic
TODO

### PCB and realisation
TODO


That's all Folks
