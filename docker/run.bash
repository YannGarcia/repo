#!/bin/bash
set -e
set -vx

if [ -f ./Dockerfile.$1 ]
then
#    docker run -it --name my$1 --rm --volume /f:/media/sf_F_DRIVE $1 /bin/bash
    docker run -it -h docker-$1 --name my$1 --rm $1 /bin/bash
fi

exit 0
