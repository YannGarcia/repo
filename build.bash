 #!/bin/bash
set -evx

OLD_PWD=`pwd`
# Use ${PATH_DEV}/g++/project/embedded/Makefile at the end of the full review
cd ${PATH_DEV}/g++/project/embedded/logger/objs
make clean
make test
make gendoc
cd ${PATH_DEV}/g++/project/embedded/helper/objs
make clean
make test
make gendoc
cd ${PATH_DEV}/g++/project/embedded/converter/objs
make clean
make test
make gendoc
cd ${OLD_PWD}

exit 0
