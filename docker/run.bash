#!/bin/bash
set -e
set -vx

if [ -f ./Dockerfile.$i ]
then
    docker run -i -t -v /f:/media/sf_F_DRIVE docker run -it $i /bin/bash
fi

exit 0
