#!/bin/bash
set -evx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
   make test_d
   make run_d
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
<<<<<<< HEAD
then
=======
>>>>>>> d32de5c53cd329b45a4d1fe10667370b903ede40
   make test_c
   make run_c
   if [ -f ./logger.coverage.info ]
   then
       coveralls-lcov --verbose ./logger.coverage.info
   fi
else
   make test
   make run_v    
fi
if [ ! -d ../docs/htm ]
then
    exit -1
fi
ls -ltr
cd ${PATH_DEV}/g++/projects/embedded/converter/objs
make test
make run
if [ ! -d ../docs/htm ]
then
    exit -1
fi
cd ${PATH_DEV}/g++/projects/embedded/helper/objs
make test
make run
if [ ! -d ../docs/htm ]
then
    exit -1
fi
cd ${OLD_PWD}

exit 0
