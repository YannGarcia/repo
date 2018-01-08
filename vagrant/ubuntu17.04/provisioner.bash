#!/bin/bash
# Prepare environment for the build
#set -e # Exit with non 0 if any command fails
#set -vx

export USERNAME=$1
export PASSWORD=$2

# Update system
sudo apt-get update
sudo apt-get dist-upgrade -y
sudo DEBIAN_FRONTEND=noninteractive apt-get install emacs git-core subversion lsof ntp gdb make cmake flex bison autoconf doxygen graphviz libtool libncurses5-dev expect libssl-dev libxml2-dev xutils-dev tcpdump libpcap-dev libwireshark-dev valgrind wget tree unzip sshpass texlive-font-utils -y
sudo DEBIAN_FRONTEND=noninteractive apt-get install --reinstall g++ -y
gcc --version
g++ --version
sudo apt-get update
sudo DEBIAN_FRONTEND=noninteractive apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf -y
arm-linux-gnueabihf-gcc --version
arm-linux-gnueabihf-g++ --version
sudo apt-get autoremove
sudo apt-get clean

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

OLD_PWD=`pwd`
# Create directories
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

git clone https://github.com/YannGarcia/repo.git repo
git clone https://github.com/YannGarcia/TriesAndDelete.git TriesAndDelete

# Create links
ln -sf ${HOME}/repo/bash_profiles/devenv.bash.ubuntu ${HOME}/devenv.bash 
ln -sf ${HOME}/repo/emacs/.emacs ${HOME}/.emacs
ln -sf ${HOME}/repo/emacs/emacs_ttcn ${HOME}/emacs_ttcn
sudo cp -Rp ${HOME}/repo/emacs/emacs_ttcn/* /usr/local/share/emacs/site-lisp 

echo "" >> ${HOME}/.bashrc
echo ". ./devenv.bash" >> ${HOME}/.bashrc
echo "" >> ${HOME}/.bashrc

# Install all frameworks
cd ${HOME_FRAMEWORKS}

# Install Raspberry PI WringPI library
#git clone git://git.drogon.net/wiringPi wiringpi
#cd ${HOME_FRAMEWORKS}/wiringpi/wiringPi
#make CC=arm-linux-gnueabihf-gcc
#ln -sf ${HOME_FRAMEWORKS}/wiringpi/wiringPi/libwiringPi.so.`cat ../VERSION` ${HOME_LIB}/libwiringPi.so
#for i in `ls *.h`
#do
#    ln -sf $i ${HOME_INC}/`basename $i`
#done
#cd -

# Install ARM cross compile for bare metal hardware
#cd ${HOME_FRAMEWORKS}
#wget https://launchpad.net/ubuntu/+archive/primary/+files/gcc-arm-none-eabi_5.4.1+svn241155.orig.tar.xz
#tar xvJf ./gcc-arm-none-eabi_5.4.1+svn241155.orig.tar.xz ./gcc-arm-none-eabi_5.4.1+svn241155.orig
#rm ./gcc-arm-none-eabi_5.4.1+svn241155.orig.tar.xz
#cd /gcc-arm-none-eabi_5.4.1+svn241155.orig
#./configure
#./gcc-arm-none-eabi_5.4.1+svn241155.orig/bin/arm-none-eabi-gcc --version
#./gcc-arm-none-eabi_5.4.1+svn241155.orig/bin/arm-none-eabi-g++ --version

# Install asn1c
cd ${HOME_FRAMEWORKS}
git clone https://github.com/vlm/asn1c.git asn1c
cd ${HOME_FRAMEWORKS}/asn1c
test -f configure || autoreconf -iv
./configure
make && sudo make install
cd -

# Install GoogleTest
cd ${HOME_FRAMEWORKS}
git clone https://github.com/google/googletest.git googletest
cd ${HOME_FRAMEWORKS}/googletest/googletest
cmake .
make
if [ -f ./libgtest.a ]
then
    sudo cp -rp ./include/* /usr/local/include
    sudo cp ./*.a /usr/local/lib
fi
cd -

# Install Crypto++ library
cd ${HOME_FRAMEWORKS}
git clone https://github.com/weidai11/cryptopp.git cryptopp
cd ${HOME_FRAMEWORKS}/cryptopp
CXXFLAGS="-DNDEBUG -g2 -O3 -std=c++11" make
if [ -f cryptest.exe ]
then
    sudo make install PREFIX=/usr/local
fi

# install latest LCOV
mkdir -p ${HOME_FRAMEWORKS}/lcov
cd ${HOME_FRAMEWORKS}/lcov
wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.13.orig.tar.gz
tar xf lcov_1.13.orig.tar.gz
sudo make -C lcov-1.13/ install
cd -

# install lcov to coveralls conversion
sudo gem install coveralls-lcov

lcov --version
coveralls-lcov -h
valgrind --version

# Checkout the project
if [ ${USERNAME} != '' ] && [ ${PASSWORD} != '' ]
then
    cd ${HOME}/tmp
    svn co --username $USERNAME --password $PASSWORD --non-interactive https://oldforge.etsi.org/svn/ITS/branches/STF525
fi

cd ${HOME}/TriesAndDelete/scripts
. ${HOME}/devenv.bash
./build_titan.bash
. ${HOME}/devenv.bash
./update_its_project.bash
ln -sf ~/TriesAndDelete/etsi_its/lib/libItsAsn.so ~/lib/libItsAsn.so
cd ${HOME}/TriesAndDelete/etsi_its/src/TestCodec/objs
. ${HOME}/devenv.bash
../bin/testcodec_generate_makefile.bash
../bin/run_mtc.bash &
../bin/run_ptcs.bash

cd ${OLD_PWD}

unset USERNAME
unset PASSWORD

sudo init 6

exit 0
