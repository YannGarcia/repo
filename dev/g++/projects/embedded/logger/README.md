
#logger framework for embedded application

This is a basic lightweight logger framework to be used in embedded application running on hardware such as raspberry or BeagleBone.

##Features
The logger framework provides the following features
* Level of the log message
* Log, trace warning and error methods
* Date/Time stamped output
* File output
* A logger factory provides a unique access to the 'named' logger instances

##Documentation
In a terminal, execute the command make gendoc to generate the documentation

##Usage
The logger framework is quite simple to use and very intuitive.

### Create an instance of the logger
There are two way to create an instance of the logger:
* Using the logger_factory to create an instance of the logger

```
std::string s("MyLogger");
logger_factory::get_instance().add_logger(s, "/tmp/Mylogger.log", logger_levels_t::trace);
```

* Create an instance of the logger

```
logger::logger l("");
```

### Set log level
TODO

### Set Date/Time format
TODO

### Code sample
Here is a sample code indicating how to use the logger framework:

```
std::string s("MyLogger");
logger_factory::get_instance().add_logger(s, "/tmp/Mylogger.log", logger_levels_t::trace);
...
logger_factory::get_instance().get_logger(s).trace("Test logger trace");
logger_factory::get_instance().get_logger(s).debug("Test logger debug");
logger_factory::get_instance().get_logger(s).info("Test logger info");
logger_factory::get_instance().get_logger(s).warning("Test logger warning");
...
logger_factory::get_instance().get_logger(s).error(std::string("Test logger error")));logger_factory::get_instance().get_logger(s).error("This a multiple variable error: %d - %s", 666, "The devil is in the details");
...
logger_factory::get_instance().remove_logger(s);
```

##Building with logger
When building your project, you need to add the framework logger directory in your build path:

```
-I <path-to-logger-dir>
```

You also need to link against liblogger.so:

```
-L <path-to-logger-dir> -llogger
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
