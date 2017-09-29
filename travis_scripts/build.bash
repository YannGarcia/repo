 #!/bin/bash
set -evx

OLD_PWD=`pwd`
# Use ${PATH_DEV}/g++/projectsxs/embedded/Makefile at the end of the full review
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
make clean
if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
    make debug
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
     make coverage
else
    make compile
fi
make gendoc
cd ${PATH_DEV}/g++/projects/embedded/converter/objs
make clean
if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
    make debug
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
    make coverage
else
    make compile
fi
make gendoc
cd ${PATH_DEV}/g++/projects/embedded/helper/objs
make clean
if [ ${TRAVIS_CONTEXT} == "NoCoveralls" ]
then
    make debug
elif [ ${TRAVIS_CONTEXT} == "WithCoveralls" ]
    make coverage
else
    make compile
fi
make gendoc
cd ${OLD_PWD}

exit 0
