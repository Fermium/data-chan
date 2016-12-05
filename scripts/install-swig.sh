#!/bin/bash

printf "\n\n\n\nInstalling swig:\n"

apt-get install -y byacc flex #necessary to build swig
apt-get install -y libpcre3 libpcre3-dev #perl regex
git clone https://github.com/arfoll/swig.git
cd swig
chmod +x autogen.sh && ./autogen.sh
./configure
make && make install
cd ../ && rm -rf swig
