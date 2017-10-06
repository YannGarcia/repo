#!/bin/bash
set -evx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded
make deploy
tree ${HOME}/docs

make clean

cd ${OLD_PWD}
exit 0
