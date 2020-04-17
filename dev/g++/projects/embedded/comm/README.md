
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

##How to build the library
All CMake files are located in objs folder.
To build this library and its test application, to test it and to install it, please follow the steps below:
```bash
$ cd objs
$ cmake .
$ make
$ ../bin/testlib
$ make prefix=$HOME install
```

To cleanup everything, please follow the steps below:
```bash
$ cd objs
$ make distclean
```
