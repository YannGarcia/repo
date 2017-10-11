#!/bin/bash
set -e # Exit with non 0 if any command fails
set -vx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded
make pushO2
tree ${HOME}/docs
make clean


#cd ${HOME}/docs
#git config user.name "YannGarcia"
#git config user.email "garcia.yann@gmail.com"
#git config user.name YannGarcia
#git add .
#git commit -a -m""
#git push


cd ${OLD_PWD}
exit 0
