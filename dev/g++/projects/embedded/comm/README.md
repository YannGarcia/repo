
#Network communication framework for embedded application

This is a basic Network Communication library to be used in embedded application running on hardware such as raspberry or BeagleBone.

##Features
The Network Communication library provides the following features
* IPv4 & IPv6 address support
* UDP client/server support on IPv4/IPv6
* TCP client/server support on IPv4/IPv6
* A factory provides a unique access to the 'named' logger instances

##Documentation
In a terminal, execute the command make gendoc to generate the documentation

##Usage
The Network Communication library is quite simple to use and very intuitive.

### Create an instance of the Network Communication library
TODO

##Building with Network Communication library
When building your project, you need to add the Network Communication directory in your build path:

```
-I <path-to-helper-dir>
```

You also need to link against libhelper.so:

```
-L <path-to-helperer-dir> -lhelper
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
  - test_d       : Build the test application in debug mode.
  - run          : Build and execute the test application.
  - run_d        : Build and execute the test application in debug mode.
  - run_v        : Build and execute the test application in debug mode, using Valgrind.
  - dist         : Build a compressed archive of the application.
  - install      : Install binaries, libraries and configuration files into final directory $(PATH_INSTALL)xxx.
  - install-devel: Same as 'install' plus include files.
  - depend       : Build the file dependencies for $(MAKE).
  - gendoc       : Build the Doxygen documantation.
