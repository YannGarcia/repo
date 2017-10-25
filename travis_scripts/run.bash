#!/bin/bash
# Execute all test library applications. This script does not check results.
set -e # Exit with non 0 if any command fails
set -vx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/

if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
    make test_d
    make run # Compiled in debug mode but running without gdb
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
then
    make test_c
    make run_c
    make push2coveralls
elif [ ${TRAVIS_CONTEXT} == "WithValgrind" ]
then
    make test
    make run_v
fi

cd ${OLD_PWD}

exit 0
