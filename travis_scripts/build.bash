 #!/bin/bash
set -evx

OLD_PWD=`pwd`
# Use ${PATH_DEV}/g++/projectsxs/embedded/Makefile at the end of the full review
cd ${PATH_DEV}/g++/projects/embedded/logger/objs
make clean
make compile_c
make gendoc
cd ${PATH_DEV}/g++/projects/embedded/converter/objs
make clean
make compile
make gendoc
cd ${PATH_DEV}/g++/projects/embedded/helper/objs
make clean
make compile
make gendoc
cd ${OLD_PWD}

exit 0
