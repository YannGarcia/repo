
#Type converter library for embedded application

This is a basic lightweight type converter library to be used in embedded application running on hardware such as raspberry or BeagleBone.

##Features
The type converter library provides the following features
* Level of the log message
* Log, trace warning and error methods
* Date/Time stamped output
* File output
* A logger factory provides a unique access to the 'named' logger instances

##Documentation
In a terminal, execute the command make gendoc to generate the documentation

##Usage
The type converter library is quite simple to use and very intuitive.

### Create an instance of the logger
There are two way to create an instance of the logger:
* Using the logger_factory to create an instance of the logger

```
std::string s("MyLogger");
<<<<<<< HEAD
converter::get_instance().add_logger(s, "/tmp/Mylogger.log", logger_levels_t::trace);
=======
logger_factory::get_instance().add_logger(s, "/tmp/Mylogger.log", logger_levels_t::trace);
>>>>>>> ab78568f8b7b855924e67049893a4f5d6ed38e7f
```

* Create an instance of the logger

```
logger l();
```

### Set log level
TODO

### Set Date/Time format
TODO

### Code sample
Here is a sample code indicating how to use the type converter library:

```
<<<<<<< HEAD
converter::get_instance().bin_to_bcd(0x0A);
...
std::string frombytes("Hello");
std::vector<uint8_t> to_bytes(converter::get_instance().string_to_bytes(frombytes));
=======
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
>>>>>>> ab78568f8b7b855924e67049893a4f5d6ed38e7f
```

##Building with logger
When building your project, you need to add the framework logger directory in your build path:

```
<<<<<<< HEAD
-I <path-to-converter-dir>
=======
-I <path-to-Logger-dir>
>>>>>>> ab78568f8b7b855924e67049893a4f5d6ed38e7f
```

You also need to link against liblogger.so:

```
<<<<<<< HEAD
-L <path-to-converter-dir> -llogger
=======
-L <path-to-SimpleLogger-dir> -llogger
>>>>>>> ab78568f8b7b855924e67049893a4f5d6ed38e7f
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

