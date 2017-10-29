#!/bin/bash
set -e
set -vx

if [ -f ./Dockerfile.$i ]
then
    if [ "$2" == "r" ]
    then
        docker rmi $(docker images -q)
    fi
    docker build --tag '$i' --force-rm ./Dockerfile.$i:
fi

exit 0
