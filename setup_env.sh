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
./autogen.sh && ./configure
make && make install
cd ../ && rm -rf swig

echo installing documentation software
apt-get install doxygen

echo installing secondary software
apt-get install -y openssl libssl-dev openssh-client

echo installing python
apt-get install -y python python-dev #python and the library to build the binding API

