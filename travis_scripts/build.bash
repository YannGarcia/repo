#!/bin/bash
#set -e # Exit with non 0 if any command fails
set -vx

OLD_PWD=`pwd`
# Use ${PATH_DEV}/g++/projectsxs/embedded/Makefile at the end of the full review
cd ${PATH_DEV}/g++/projects/embedded
if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then # Debug mode
    cmake -DCMAKE_BUILD_TYPE=Debug .
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
then # Using LCov
    cmake -DCMAKE_BUILD_TYPE=Coverage .
elif [ ${TRAVIS_CONTEXT} == "WithValgrind" ]
then # Using Valgrind
    cmake -DCMAKE_BUILD_TYPE=Valgrind .
else # Release mode
    cmake -DCMAKE_BUILD_TYPE=Release .
fi
make
cd ${OLD_PWD}

exit 0
