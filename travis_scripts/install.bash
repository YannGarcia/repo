#!/bin/bash
# Prepare environment for the build
#set -e # Exit with non 0 if any command fails
#set -vx

export PATH_DEV=${HOME}/dev
export HOME_FRAMEWORKS=${HOME}/frameworks
export HOME_LIB=${HOME}/lib
export HOME_BIN=${HOME}/bin
export HOME_ETC=${HOME}/etc
export HOME_INC=${HOME}/include
export HOME_TMP=${HOME}/tmp
export HOME_DOCS=${HOME}/docs
export PATH=${HOME_BIN}:${PATH}
export LD_LIBRARY_PATH=${HOME_LIB}:/usr/local/lib:${LD_LIBRARY_PATH}
echo ${TRAVIS_CONTEXT}
echo ${TRAVIS_JOB_ID}
echo ${COVERALLS_TOKEN}

# Setup the date format for the test units
sudo timedatectl set-timezone UTC
date --rfc-2822

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
mkdir -p ${HOME_DOCS}
if [ ! -d ${HOME_DOCS} ]
then
    exit -1
fi
mkdir -p ${HOME_FRAMEWORKS}
if [ ! -d ${HOME_FRAMEWORKS} ]
then
    exit -1
fi

# Install gcc-9
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc- g++-9 gdb make cmake doxygen graphviz libncurses5-dev expect libssl-dev libxml2-dev xutils-dev tcpdump libpcap-dev libwireshark-dev valgrind wget tree unzip sshpass texlive-font-utils -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 50 --slave /usr/bin/g++ g++ /usr/bin/g++-9

# Install frameworks
cd ${HOME_FRAMEWORKS}

# Install GoogleTest
git clone https://github.com/google/googletest.git googletest
cd ${HOME_FRAMEWORKS}/googletest/ci
ADD_GCC='s/^make.*/& CC=gcc/'
sed --in-place "${ADD_GCC}" ./travis.sh
SAMPLES_OFF='s/=ON/=OFF/g'
sed --in-place "${SAMPLES_OFF}" ./travis.sh
#cat ./travis.sh
cd ${HOME_FRAMEWORKS}/googletest
./ci/travis.sh
if [ -d ./build ]
then
    cd ./build
    sudo make install
fi

# Install Crypto++ library
cd ${HOME_FRAMEWORKS}
git clone https://github.com/weidai11/cryptopp.git cryptopp
cd ${HOME_FRAMEWORKS}/cryptopp
CXXFLAGS="-DNDEBUG -g2 -O3 -std=c++11" make
if [ -f cryptest.exe ]
then
    sudo make install PREFIX=/usr/local
fi

# Install libmicrohttpd library
git config --global http.sslVerify false
cd ${HOME_FRAMEWORKS}
git clone https://git.gnunet.org/libmicrohttpd.git libmicrohttpd
cd ${HOME_FRAMEWORKS}/libmicrohttpd
autoreconf -fi
./configure --enable-https
make
if [ -f ./src/microhttpd/.libs/libmicrohttpd.so ]
then
    sudo make install PREFIX=/usr/local
fi

if [ "${TRAVIS_CONTEXT}" == "LinuxHW" ]
then
    # Install ARM cross compiler for Linux hardware
    #sudo apt-add-repository "deb http://fr.archive.ubuntu.com/ubuntu/ wily main"
    #sudo apt-get update
    #sudo apt-get install gcc-5-arm-linux-gnueabihf g++-5-arm-linux-gnueabihf -y
    sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf -y
    #sudo apt-add-repository  --remove "deb http://fr.archive.ubuntu.com/ubuntu/ wily main"
    #    ls -l /usr/bin | grep gnueab
    #sudo rm -f /usr/bin/arm-linux-gnueabihf-gcc /usr/bin/arm-linux-gnueabihf-g++
    #sudo ln -s /usr/bin/arm-linux-gnueabihf-gcc-5 /usr/bin/arm-linux-gnueabihf-gcc
    #sudo ln -s /usr/bin/arm-linux-gnueabihf-g++-5 /usr/bin/arm-linux-gnueabihf-g++
    
    arm-linux-gnueabihf-gcc --version
    arm-linux-gnueabihf-g++ --version

    # Install Raspberry PI WringPI library
    git clone git://git.drogon.net/wiringPi wiringpi
    cd ${HOME_FRAMEWORKS}/wiringpi/wiringPi
    make CC=arm-linux-gnueabihf-gcc
    ln -sf ${HOME_FRAMEWORKS}/wiringpi/wiringPi/libwiringPi.so.`cat ../VERSION` ${HOME_LIB}/libwiringPi.so
    for i in `ls *.h`
    do
        ln -sf $i ${HOME_INC}/`basename $i`
    done
    cd -
    
    export SELECTED_HW="raspberry_pi"
    CROSS_COMPILER_VERSION=4.8.4
    export CROSS_COMPILER_PATH=/usr
    export CROSS_COMPILER_BIN=${CROSS_COMPILER_PATH}/bin
    export CROSS_COMPILER_INC_PATH=${CROSS_COMPILER_PATH}/arm-linux-gnueabihf/include
    ls -l ${CROSS_COMPILER_INC_PATH}/c++
    export CROSS_COMPILER_SYS_INC_PATH=${CROSS_COMPILER_INC_PATH}/c++/${CROSS_COMPILER_VERSION}
    export CROSS_COMPILER_SYS_LIB_PATH=${CROSS_COMPILER_PATH}/arm-linux-gnueabi/libhf
    export CROSS_COMPILER_ARM_GCC_PATH=${CROSS_COMPILER_SYS_LIB_PATH}
    export CROSS_COMPILER_ARM_GCC_LIB_PATH=${CROSS_COMPILER_SYS_LIB_PATH}

    export KERNEL_SRC=~/Kernel/linux/
    export CCPREFIX=arm-linux-gnueabihf-
    export KBUILD_OUTPUT=~/tmp

    export PATH=${CROSS_COMPILER_BIN}:${PATH}

elif [ "${TRAVIS_CONTEXT}" == "BaremetalHW" ]
then
    # Install ARM cross compile for bare metal hardware
    wget https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
    bzip2 -d ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
    tar xvf ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar
    #    rm ./gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar

    ./gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-gcc --version
    ./gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-g++ --version

    export SELECTED_HW="ti_msp_exp432p401r"
    MBED_CROSS_COMPILER_VERSION=5.4.1
    export MBED_CROSS_COMPILER_PATH=${HOME_FRAMEWORKS}/gcc-arm-none-eabi-5_4-2016q2
    export MBED_CROSS_COMPILER_BIN=${MBED_CROSS_COMPILER_PATH}/bin
    export MBED_CROSS_COMPILER_SYS_INC_PATH=${MBED_CROSS_COMPILER_PATH}/arm-none-eabi/include/c++/${MBED_CROSS_COMPILER_VERSION}
    export MBED_CROSS_COMPILER_SYS_LIB_PATH=${MBED_CROSS_COMPILER_PATH}/arm-none-eabi/lib
    export MBED_CROSS_COMPILER_BIN=${MBED_CROSS_COMPILER_PATH}/bin

    # Download the correct driver library
    if [ "${SELECTED_HW}" == "ti_msp_exp432p401r" ]
    then
        # See http://www.ti.com/tool/MSPDRIVERLIB
        cd ${HOME_FRAMEWORKS}
        wget http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP432_Driver_Library/latest/exports/msp432_driverlib_3_21_00_05.zip
        unzip msp432_driverlib_3_21_00_05.zip
        cd -
    else
        # TODO download for TIVA
        echo "Error: worng hardware selector: ${SELECTED_HW}"
        exit -1
    fi
else # Linux amd64
    # install latest LCOV
    mkdir -p ${HOME_FRAMEWORKS}/lcov
    cd ${HOME_FRAMEWORKS}/lcov
    wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.13.orig.tar.gz
    tar xf lcov_1.13.orig.tar.gz
    sudo make -C lcov-1.13/ install
    
    # install lcov to coveralls conversion
    gem install coveralls-lcov
    
    g++ --version
    #gcov --version
    lcov --version
    coveralls-lcov -h
    valgrind --version
fi

cd ${OLD_PWD}

#exit 0
return 0
