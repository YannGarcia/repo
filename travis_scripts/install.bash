#!/bin/bash
# Prepare environment for the build
set -e # Exit with non 0 if any command fails
#set -vx

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
mkdir -p ${HOME_TMP}
if [ ! -d ${HOME_TMP} ]
then
    exit -1
fi
mkdir -p ${HOME_FRAMEWORKS}
if [ ! -d ${HOME_FRAMEWORKS} ]
then
    exit -1
fi
# Install gcc-6
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc-6 g++-6 gdb doxygen graphviz libncurses5-dev expect libssl-dev libxml2-dev xutils-dev tcpdump libpcap-dev libwireshark-dev tree texlive-font-utils -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 50 --slave /usr/bin/g++ g++ /usr/bin/g++-6
# Install frameworks
cd ${HOME_FRAMEWORKS}
# Install ARM Compile
wget https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
bzip2 -d ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
tar xvf ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar
rm ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
# install latest LCOV
wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.11.orig.tar.gz
tar xf lcov_1.13.orig.tar.gz
sudo make -C lcov-1.11/ install
# install lcov to coveralls conversion
gem install coveralls-lcov
# Install GoogleTest
git clone https://github.com/google/googletest.git googletest
cd ${HOME_FRAMEWORKS}/googletest
ADD_GCC='/make/make CC=gcc'
sed --in-place "${ADD_GCC}" ./travis.sh
SAMPLES_OFF='/ON/OFF/g'
sed --in-place "${SAMPLES_OFF}" ./travis.sh
cat ./travis.sh
./travis.sh
if [ -d ./build ]
then
    cd ./build
    sudo make install
fi
cd ${OLD_PWD}

g++ --version
gcov --version
lcov --version
coveralls-lcov -h

exit 0
