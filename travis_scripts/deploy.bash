#!/bin/bash
set -e # Exit with non 0 if any command fails
set -vx

if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
    OLD_PWD=`pwd`
    cd ${PATH_DEV}/g++/projects/embedded
    make pushO2
    
#    cd ${HOME_TMP}
#    mkdir docs
#    cd ./docs
#    git clone https://github.com/YannGarcia/repo.git .
#    git config user.name "YannGarcia"
#    git config user.email "garcia.yann@gmail.com"
#    git checkout origin/gh-pages -b gh-pages
#    mv ${HOME}/docs/* .
#    git add .
#    git commit -m "New generated HTML doc"
#    git push origin gh-pages
#    cd ${HOME_TMP}
#    rm -fr ./docs

    cd ${OLD_PWD}
fi

exit 0
