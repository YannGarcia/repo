#!/bin/bash
set -e # Exit with non 0 if any command fails
#set -vx

OLD_PWD=`pwd`
# Use ${PATH_DEV}/g++/projectsxs/embedded/Makefile at the end of the full review
cd ${PATH_DEV}/g++/projects/embedded
make clean
if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
    make debug
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
then
    make coverage
elif [ ${TRAVIS_CONTEXT} == "WithValgrind" ]
then
    make compile
else
    make compile
fi
make gendoc
cd ${OLD_PWD}

exit 0
