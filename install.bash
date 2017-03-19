 #!/bin/bash
set -evx

OLD_PWD=`pwd`
cd ${HOME_FRAMEWORKS}/cpptest-1.1.2
./configure CXXFLAGS="-g -O2 -std=c++11"
make
sudo make install 
cd ${OLD_PWD}

exit 0
