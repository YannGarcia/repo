
#Helper library for embedded application

This is a basic Helper library to be used in embedded application running on hardware such as raspberry or BeagleBone.

##Features
The Helper library provides the following features
* Input/Output bit stream
* Basic thread support
* Date/Time support
* Command line parser

##Documentation
In a terminal, execute the command make gendoc to generate the documentation

##Usage
The Helper library is quite simple to use and very intuitive.

### Create an instance of the Helper library
TODO

##Building with Helper library
When building your project, you need to add the Helper directory in your build path:

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
