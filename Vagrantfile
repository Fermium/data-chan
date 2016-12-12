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
  
  # Enable USB Controller on VirtualBox
  config.vm.provider "virtualbox" do |vb|
    vb.customize ["modifyvm", :id, "--usb", "on"]
    vb.customize ["modifyvm", :id, "--usbehci", "on"]
  end

  # Implement determined configuration attributes
  config.vm.provider "virtualbox" do |vb|
    vb.customize ["usbfilter", "add", "0",
        "--target", :id,
        "--name", "datachan tester",
        "--product", "datachan tester"]
  end
  
  config.vm.provider "virtualbox" do |vb|
    vb.customize ["usbfilter", "add", "0",
        "--target", :id,
        "--name", "USBasp",
        "--product", "USBasp"]
  end

  ###############################################################
   config.vm.provision "shell", privileged: false, inline: <<-SHELL
     printf "\n\nInstalling software\n"
     sudo apt-get update && sudo apt-get upgrade -y
     sudo apt-get -y install byacc flex doxygen libpcre3 libpcre3-dev git openssl pkg-config libssl-dev wget libusb-1.0-0-dev zlib1g-dev unzip python python-dev openssh-client tar gcc g++ gcc-avr avr-libc avrdude binutils-avr make autogen autoconf curl build-essential clang
     
     
     printf "\n\nInstalling Swig\n"
     sudo bash /vagrant/scripts/install-swig.sh
     
     
     printf "\n\nInstalling NodeJS\n"
     sudo bash /vagrant/scripts/install-node.sh

     
     printf "\n\nInstalling pip and mkdocs\n"
     sudo bash  /vagrant/scripts/install-mkdocs.sh
        
     # link volume to home user folder
     ln -s /vagrant data-chan
     
     bash /vagrant/scripts/print-versions.sh
     
     printf "\n\n\n\nThe box is ready. Now simply run \"vagrant ssh\" to connect! \n"
     
   SHELL
   
   
   
end
