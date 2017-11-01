#!/bin/bash
set -e
set -vx

if [ -f ./Dockerfile.$1 ]
then
    docker run -it --name my$1 --rm --volume /f:/media/sf_F_DRIVE $1 /bin/bash
fi

exit 0
