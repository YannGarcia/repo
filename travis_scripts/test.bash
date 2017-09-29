 #!/bin/bash
set -evx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
make test_c
make run_c
if [ ! -d ../docs/htm ]
then
    exit -1
fi
if [ -f ./logger.coverage.info ]
then
    echo "coveralls --verbose --no-gcov --gcov ./logger.coverage.info"
fi
cd ${PATH_DEV}/g++/projects/embedded/converter/objs
make test
make run
if [ ! -d ../docs/htm ]
then
    exit -1
fi
cd ${PATH_DEV}/g++/projects/embedded/helper/objs
make test
make run
if [ ! -d ../docs/htm ]
then
    exit -1
fi
cd ${OLD_PWD}

exit 0
