# Supported hardwares #

This document provides the list of the supported hardwares, cards and extensions, document references...
In addition, it provides the value to use for the env. variable SELECTED_HW in case of cross-build.


## Baremetal hardwares
This clause provides the list of 'Baremetal' hardwares

### Texas Instrument Launchpads hardware

#### The MSP432 family

In bash script, update SELECTED_HW as decribed below:

  ```bash
  SELECTED_HW=ti_msp_exp432p401r
  ```
  
TI documentation  : http://dev.ti.com/tirex/#/Device/MSP432P401R/?link=Device%20Documentation%2FMSP432P401R
TI Driver library : http://www.ti.com/tool/MSPDRIVERLIB

#### The TIVA family

TODO

### MDED LPC1768 family

TODO

## Linux hardwares

### Raspberry Pi family

In bash script, update SELECTED_HW as decribed below:

  ```bash
  SELECTED_HW=raspberry_pi
  ```
  
RaspberryPI documentation : https://www.raspberrypi.org/
WiringPI library          : http://wiringpi.com/download-and-install/

### BeagleBone Black family

TODO

### BC.H.I.P family

TODO

That's all folks
