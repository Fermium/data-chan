echo "updating apt"
apt-get update

echo "Installing basic software"
apt-get install -y pkg-config  wget  curl git unzip tar unzip

echo "Installing basic building software"
apt-get install -y make autogen autoconf build-essential #general building utils
apt-get install -y gcc g++ #default compilers

echo "Installing avr software"
apt-get install -y gcc-avr avr-libc binutils-avr avrdude #avr compiler

echo "Installing needed libs"
apt-get install -y libusb-1.0-0-dev zlib1g-dev #libs

echo "Installing swig"
apt-get install -y byacc flex #necessary to build swig
apt-get install -y libpcre3 libpcre3-dev #perl regex
git clone https://github.com/arfoll/swig.git
cd swig
chmod +x autogen.sh && ./autogen.sh
./configure
make && make install
cd ../ && rm -rf swig

echo "Installing documentation software"
apt-get install -y doxygen

echo "Installing secondary software"
apt-get install -y openssl libssl-dev openssh-client

echo "Installing python"
apt-get install -y python python-dev #python and the library to build the binding API
