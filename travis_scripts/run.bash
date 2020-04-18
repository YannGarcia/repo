#!/bin/bash
# Execute all test library applications. This script does not check results.
#set -e # Exit with non 0 if any command fails
#set -vx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/

if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then # Compiled in debug mode but running without gdb
    make run_logger
    make run_converter
    #make run_helper
    make run_ipc
    #make run_comm
    make run_security
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
then
    make run_logger_c
    make run_converter_c
    #make run_helper_c
    make run_ipc_c
    #make run_comm_c
    make run_security_c
    make push2coveralls
elif [ ${TRAVIS_CONTEXT} == "WithValgrind" ]
then
    make run_logger_v
    #make run_converter_v
    make run_helper_v
    make run_ipc_v
    #make run_comm_v
    make run_security_v
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
