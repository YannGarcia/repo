 #!/bin/bash
set -evx

OLD_PWD=`pwd`
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc-4.9 -y
mkdir -p ${HOME_FRAMEWORKS}
cd ${HOME_FRAMEWORKS}
wget https://downloads.sourceforge.net/project/cpptest/cpptest/cpptest-1.1.2/cpptest-1.1.2.tar.gz
tar xvzf cpptest-1.1.2.tar.gz
ls -ltr ${HOME_FRAMEWORKS}
cd ./cpptest-1.1.2
find . -name "*.h*" -type f -exec sed -i 's/auto_ptr/unique_ptr/g' {} \;
find .  -name "*.c*" -type f -exec sed -i 's/auto_ptr/unique_ptr/g' {} \;
cd ${HOME_FRAMEWORKS}/cpptest-1.1.2
./configure CXXFLAGS="-g -O2 -std=c++11"
make
sudo make install 
cd ${OLD_PWD}

exit 0
