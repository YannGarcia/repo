
#security framework for embedded application

This is a basic lightweight security framework to be used in embedded application running on hardware such as raspberry or BeagleBone.

##Features
The security framework provides the following features
* Level of the log message
* Log, trace warning and error methods
* Date/Time stamped output
* File output
* A security factory provides a unique access to the 'named' security instances

##Documentation
In a terminal, execute the command make gendoc to generate the documentation

##Usage
The security framework is quite simple to use and very intuitive.

### Create an instance of the security
There are two way to create an instance of the security:
TODO

### Set log level
TODO

### Set Date/Time format
TODO

### Code sample
TODO

##Building with security
When building your project, you need to add the framework security directory in your build path:

```
-I <path-to-security-dir>
```

You also need to link against libsecurity.so:

```
-L <path-to-security-dir> -lsecurity
```

##Make options
The make options are:
  - help         : This menu.
  - clean        : Erase objects, binaries and temporary files.
  - debug        : Build a debug version.
  - compile      : Build the application in release mode.
  - recompile    : Rebuild the application in release mode.
  - rel          : Clean and rebuild the application in release mode.
  - test         : Build the test application.
  - test_d       : Build the test application in debug mod.
  - run          : Build and execute the test application.
  - run_d        : Build and execute the test application in debug mode.
  - run_v        : Build and execute the test application in debug mode, using Valgrind.
  - dist         : Build a compressed archive of the applicatione.
  - install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)xxx.
  - install-devel: Same as 'install' plus include files.
  - depend       : Build the file dependencies for $(MAKE).
  - gendoc       : Build the Doxygen documantation.

