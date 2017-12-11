#!/bin/bash
set -e
set -vx

if [ -f ./Dockerfile.$1 ]
then
#    docker run -it --name my$1 --rm --volume /f:/media/sf_F_DRIVE $1 /bin/bash
    if [ "$2" == "rm" ]
    then
        docker run -it --name my$1 --rm $1 /bin/bash
    else
        docker run -it --name my$1 $1 /bin/bash
	docker stop $(docker ps -a -q)
    fi
fi

exit 0
