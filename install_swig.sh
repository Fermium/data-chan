#!/bin/bash
echo fetching swig
git clone https://github.com/arfoll/swig.git
cd swig
echo Configuring swig...
./autogen.sh
./configure
echo building swig...
make
echo installing swig...
sudo make install
echo done.
cd ../ && rm -rf swig
