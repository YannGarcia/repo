# TIVA-C LaunchPad HAL (Hardware Abstract Level)

1. To select the TI board to be used to generate the library, open Makefile.inc file and modify PART value (see http://dev.ti.com/tirex/#/?link=Software%2FTM4C%20ARM%20Cortex-M4F%20MCU)

2. To update the version of the TivaWare C Series, open Makeinclude file and update TIVA_C_PATH

3. On Linux based OS and MAC OS X, use lm4tool to upload baremetal binary file to the board (see https://github.com/utzig/lm4tools)

4. The file Makefile provides a way to build all project in ek-tm4c12x in one command, see clause below

5. Usage example:

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
```

That's all folks
