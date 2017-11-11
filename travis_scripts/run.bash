#!/bin/bash
# Execute all test library applications. This script does not check results.
#set -e # Exit with non 0 if any command fails
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
elif [ ${TRAVIS_CONTEXT} == "LinuxHW" ]
then
    if [ "${SCP_REMOTE_ADDRESS}" != "" ] && [ "${SCP_REMOTE_USER}" != "" ]
    then
        OLD_PWD=`pwd`
        cd ${PATH_DEV}/g++/projects/embedded/
        PROJECTS='logger helper converter ipc'
        for i in ${PROJECTS}
        do
            sshpass -p raspberry scp ./$i/lib/*.so pi@${SCP_REMOTE_ADDRESS}:/home/${SCP_REMOTE_USER}/lib
            sshpass -p raspberry scp ./$i/test/testcpp pi@${SCP_REMOTE_ADDRESS}:/home/${SCP_REMOTE_USER}/bin
        done
        cd -
    else
        echo "deploy.bash: Wrong parameters for Linux hardware SCP command"
    fi
fi

cd ${OLD_PWD}

exit 0
