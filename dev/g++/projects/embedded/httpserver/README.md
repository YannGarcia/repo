
#lightweight HTTP server based built upon libmicrohttpd

This is a basic HTTP server based built upon [libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) to be used in embedded application running on hardware such as raspberry or BeagleBone.

##Features
The HTTP server library provides the following features
* IPv4 & IPv6 address support
* TLS support
* Implementation is HTTP 1.1 compliant
* Multithreaded

##Documentation
In a terminal, execute the command make gendoc to generate the documentation

##Usage
The HTTP server library is quite simple to use and very intuitive.

### Create an instance of the HTTP server library
TODO

##Building with HTTP server library
When building your project, you need to add the HTTP server directory in your build path:

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
