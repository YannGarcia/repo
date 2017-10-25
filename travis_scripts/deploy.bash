#!/bin/bash
set -e # Exit with non 0 if any command fails
#set -vx

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
elif [ ${TRAVIS_CONTEXT} == "LinuxHW" ]
then
    if [ "${SCP_REMOTE_ADDRESS}" != "" && "${SCP_REMOTE_USER}" != "" ]
    then
        OLD_PWD=`pwd`
        cd ${PATH_DEV}/g++/projects/embedded/
        PROJECTS='logger helper converter ipc'
        for i in ${PROJECTS}
        do
            scp ./$i/lib/*.so pi@${SCP_REMOTE_ADDRESS}:/home/${SCP_REMOTE_USER}/lib
            scp ./$i/test/testcpp pi@${SCP_REMOTE_ADDRESS}:/home/${SCP_REMOTE_USER}/bin
        done
        cd -
    else
        echo "deploy.bash: Wrong parameters for Linux hardware SCP command"
    fi
fi

exit 0
