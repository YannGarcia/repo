 #!/bin/bash
set -evx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
make test
make run
if [ ! -d ../docs/htm ]
then
    exit -1
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
