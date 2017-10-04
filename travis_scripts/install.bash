#!/bin/bash
# Prepare environment for the build
#set -evx

OLD_PWD=`pwd`
# Create bin/lib/include directories
mkdir -p ${HOME_LIB}
if [ ! -d ${HOME_LIB} ]
then
    exit -1
fi
mkdir -p ${HOME_INC}
if [ ! -d ${HOME_INC} ]
then
    exit -1
fi
mkdir -p ${HOME_BIN}
if [ ! -d ${HOME_BIN} ]
then
    exit -1
fi
# Install gcc-6
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc-6 g++-6 gdb doxygen libncurses5-dev expect libssl-dev libxml2-dev xutils-dev tcpdump libpcap-dev libwireshark-dev tree lcov texlive-font-utils -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 50 --slave /usr/bin/g++ g++ /usr/bin/g++-6
gcc -v
g++ -v
# Install frameworks
mkdir -p ${HOME_FRAMEWORKS}
cd ${HOME_FRAMEWORKS}
# Install ARM Compile
wget https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
bzip2 -d ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
tar xvf ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar
# Install cpptest-1.1.2
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

g++ --version
gcov --version
lcov --version

exit 0
