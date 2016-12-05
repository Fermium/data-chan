echo updating apt
apt-get update

echo installing basic software
apt-get install -y pkg-config  wget  curl git unzip tar unzip

echo installing basic building software
apt-get install -y make autogen autoconf build-essential #general building utils
apt-get install -y gcc g++ #default compilers

echo installing avr software
apt-get install -y gcc-avr avr-libc binutils-avr avrdude #avr compiler

echo installing needed libs
apt-get install -y libusb-1.0-0-dev zlib1g-dev #libs

echo installing swig
apt-get install -y byacc flex #necessary to build swig
apt-get install -y libpcre3 libpcre3-dev #perl regex
git clone https://github.com/arfoll/swig.git
cd swig
chmod +x autogen.sh && ./autogen.sh
./configure
make && make install
cd ../ && rm -rf swig

echo installing documentation software
apt-get install -y doxygen

echo installing secondary software
apt-get install -y openssl libssl-dev openssh-client

echo installing python
apt-get install -y python python-dev #python and the library to build the binding API

echo installing node.js
export NODE_VER=6.9.1
curl -sL https://deb.nodesource.com/setup_6.x -o nodesource_setup.sh
bash nodesource_setup.sh
apt-get install -y nodejs
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.32.1/install.sh | bash
export NVM_DIR="/root/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm
nvm install $NODE_VER
nvm alias default $NODE_VER
nvm use default

