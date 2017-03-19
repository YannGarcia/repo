 #!/bin/bash
set -evx

OLD_PWD=`pwd`
cd ${PATH_DEV}/g++/project/embedded/logger/objs
make run
cd ${OLD_PWD}

exit 0
