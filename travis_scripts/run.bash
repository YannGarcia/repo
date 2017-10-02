#!/bin/bash
# Execute all test library applications. This script does not check results.
set -evx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
   make test_d
   make run
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
then
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
