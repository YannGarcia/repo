#!/bin/bash
set -e
#set -vx

if [ "$1" == "" ]
then
    exit -1
fi

BUILD_FOLDER=./build.$1
DOCKER_FILE=./Dockerfile.$1
if [ -f ${DOCKER_FILE} ]
then
    if [ "$2" == "r" ]
    then
        docker rmi -f $(docker images -q)
    fi
    if [ -d ${BUILD_FOLDER} ]
    then
        rm -fr ${BUILD_FOLDER}
    fi
    mkdir ${BUILD_FOLDER}
    cp ${DOCKER_FILE} ${BUILD_FOLDER}/Dockerfile
    cd ${BUILD_FOLDER}
    docker build --tag $1 --force-rm --build-arg USERNAME=${USERNAME} --build-arg PASSWORD=${PASSWORD} .
    cd ..
    docker image ls -a
    rm -fr ${BUILD_FOLDER}
    docker inspect $1:latest
else
    exit -2
fi

exit 0
