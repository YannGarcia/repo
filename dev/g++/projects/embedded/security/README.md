
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
