 #!/bin/bash
set -evx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
make test
make run
cd ${PATH_DEV}/g++/projects/embedded/converter/objs
make test
make run
cd ${PATH_DEV}/g++/projects/embedded/helper/objs
make test
make run
cd ${OLD_PWD}

exit 0
