#!/bin/bash
set -e # Exit with non 0 if any command fails
#set -vx

if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
    OLD_PWD=`pwd`
    cd ${PATH_DEV}/g++/projects/embedded
    make pushO2
    cd ${OLD_PWD}
fi

exit 0
