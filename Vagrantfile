# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|

  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://atlas.hashicorp.com/search.
   config.vm.box = "bento/ubuntu-16.04"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
   config.vm.network "public_network"

  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
   config.vm.synced_folder "./", "/vagrant/data-chan"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
   config.vm.provider "virtualbox" do |vb|
     # Display the VirtualBox GUI when booting the machine
     vb.gui = false
  
     vb.name = "data-chan"
     # Customize the amount of memory on the VM:
     vb.memory = "1024"

     # Avoid ubuntu network problems at boot
     vb.customize ["modifyvm", :id, "--cableconnected1", "on"]
     
     # Limit CPU usage
     vb.customize ["modifyvm", :id, "--cpuexecutioncap", "65"]
   end
  
  ###############################################################
   config.vm.provision "shell", inline: <<-SHELL
     echo "Installing software"
     sudo apt-get update
     sudo apt-get -y  install byacc flex libpcre3 libpcre3-dev git openssl pkg-config libssl-dev wget libusb-1.0-0-dev zlib1g-dev unzip python python-dev openssh-client unzip tar gcc g++ gcc-avr avr-libc binutils-avr make autogen autoconf curl build-essential ruby
     
     
     echo "Installing Swig"
     git clone https://github.com/arfoll/swig.git
     cd swig
     ./autogen.sh
     ./configure
     make
     sudo make install
     cd ../ && rm -rf swig
     swig -version
     
     
     echo "Installing NodeJS"
     export NODE_VER=6
     curl -sL https://deb.nodesource.com/setup_6.x -o nodesource_setup.sh
     sudo bash nodesource_setup.sh
     sudo apt-get install -y nodejs
     curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.32.1/install.sh | bash
     export NVM_DIR="/root/.nvm"
     [ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm
     nvm install $NODE_VER && nvm alias default $NODE_VER nvm use default
     
     
     echo "Node Package Manager: $(npm --version)"
     echo "Node: $(node --version)"
     echo "Node Version Manager: $(nvm --version)"
     echo Node V8: $(node -e "console.log(process.versions.v8)")
     
     echo "The box is ready. Have fun!"
   SHELL
   
   
   
end
