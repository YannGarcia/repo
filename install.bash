 #!/bin/bash
set -evx

# Create bin/lib/include directories
mkdir -p ${HOME_LIB} ${HOME_INC} ${HOME_BIN}
OLD_PWD=`pwd`
# Install gcc-4.9
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc-4.9 g++-4.9 gdb doxygen -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 50 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9
gcc -v
g++ -v
# Install cpptest-1.1.2
mkdir -p ${HOME_FRAMEWORKS}
cd ${HOME_FRAMEWORKS}
wget https://downloads.sourceforge.net/project/cpptest/cpptest/cpptest-1.1.2/cpptest-1.1.2.tar.gz
tar xvzf cpptest-1.1.2.tar.gz
cd ./cpptest-1.1.2
find . -name "*.h*" -type f -exec sed -i 's/auto_ptr/unique_ptr/g' {} \;
find .  -name "*.c*" -type f -exec sed -i 's/auto_ptr/unique_ptr/g' {} \;
cd ${HOME_FRAMEWORKS}/cpptest-1.1.2
./configure CXXFLAGS="-g -O2 -std=c++11"
make
sudo make install 
cd ${OLD_PWD}

exit 0
