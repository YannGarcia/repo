#!/bin/bash
# Setup environment variable and install coveralls-lcov
set -evx

export HOME=`pwd`
export PATH_DEV=${HOME}/dev
export HOME_FRAMEWORKS=${HOME}/frameworks
export HOME_LIB=${HOME}/lib
export HOME_BIN=${HOME}/bin
export HOME_ETC=${HOME}/etc
export HOME_INC=${HOME}/include
export PATH=${HOME_BIN}:${PATH}
export LD_LIBRARY_PATH=${HOME_LIB}:/usr/local/lib:${LD_LIBRARY_PATH}
ls -l ${HOME} && pwd
echo ${TRAVIS_CONTEXT}
echo ${TRAVIS_JOB_ID}
echo ${COVERALLS_TOKEN}
gem install coveralls-lcov

exit 0
