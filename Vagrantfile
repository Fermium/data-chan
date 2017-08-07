# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.define 'arch' do |arch|
    # Every Vagrant development environment requires a box. You can search for
    # boxes at https://atlas.hashicorp.com/search.
    arch.vm.box = 'terrywang/archlinux'

    # Create a public network, which generally matched to bridged network.
    # Bridged networks make the machine appear as another physical device on
    # your network.
    arch.vm.network 'private_network', type: 'dhcp'

    # Provider-specific configuration so you can fine-tune various
    # backing providers for Vagrant. These expose provider-specific options.
    arch.vm.provider 'virtualbox' do |vb|
      # Display the VirtualBox GUI when booting the machine
      vb.gui = false

      vb.name = 'data-chan-arch'
      # Customize the amount of memory on the VM:
      vb.memory = '1024'

      # Avoid ubuntu network problems at boot
      vb.customize ['modifyvm', :id, '--cableconnected1', 'on']

      # Limit CPU usage
      vb.customize ['modifyvm', :id, '--cpuexecutioncap', '65']
    end

    ###############################################################
    arch.vm.provision 'shell', privileged: false, inline: <<-SHELL
      sudo pacman --noconfirm -Syu
      sudo pacman --noconfirm -S avr-gcc avr-libc avr-binutils libusb make doxygen git autogen autoconf clang avrdude python-pip
      sudo pip install mkdocs
      # link volume to home user folder
      ln -s /vagrant /home/vagrant/data-chan
      
      bash /vagrant/scripts/print-versions.sh
    SHELL
  end

  config.vm.define 'ubuntu' do |ubuntu|
    # Every Vagrant development environment requires a box. You can search for
    # boxes at https://atlas.hashicorp.com/search.
    ubuntu.vm.box = 'bento/ubuntu-16.04'

    # Create a public network, which generally matched to bridged network.
    # Bridged networks make the machine appear as another physical device on
    # your network.
    ubuntu.vm.network 'private_network', type: 'dhcp'

    # Provider-specific configuration so you can fine-tune various
    # backing providers for Vagrant. These expose provider-specific options.
    ubuntu.vm.provider 'virtualbox' do |vb|
      # Display the VirtualBox GUI when booting the machine
      vb.gui = false

      vb.name = 'data-chan-ubuntu'
      # Customize the amount of memory on the VM:
      vb.memory = '1024'

      # Limit CPU usage
      vb.customize ['modifyvm', :id, '--cpuexecutioncap', '65']
    end

    ## Enable USB Controller on VirtualBox
    #ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['modifyvm', :id, '--usb', 'on']
    #  vb.customize ['modifyvm', :id, '--usbehci', 'on']
    #end

    ## Implement determined configuration attributes
    #ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'datachan tester',
    #                '--product', 'datachan tester']
    #end

    #ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'USBasp',
    #                '--product', 'USBasp']
    #end

    ###############################################################
    ubuntu.vm.provision 'shell', privileged: false, inline: <<-SHELL
       printf "\n\nInstalling software\n"
       sudo apt-get update && sudo apt-get upgrade -y
       sudo apt-get -y install byacc flex doxygen libpcre3 libpcre3-dev git openssl pkg-config libssl-dev wget libusb-1.0-0-dev zlib1g-dev unzip python python-dev openssh-client tar gcc g++ gcc-avr avr-libc avrdude binutils-avr make autogen autoconf curl build-essential clang

       printf "\n\nInstalling pip and mkdocs\n"
       sudo bash  /vagrant/scripts/install-mkdocs.sh

       # link volume to home user folder
       ln -s /vagrant data-chan

       bash /vagrant/scripts/print-versions.sh

       printf "\n\n\n\nThe box is ready. Now simply run \"vagrant ssh\" to connect! \n"

     SHELL
  end
  config.vm.define 'windows_10_64' do |windows_10_64|
    # Every Vagrant development environment requires a box. You can search for
    # boxes at https://atlas.hashicorp.com/search.
    ubuntu.vm.box = 'bento/ubuntu-16.04'
    config.vm.box_url = 's3://example.com/secret.box'


    # Create a public network, which generally matched to bridged network.
    # Bridged networks make the machine appear as another physical device on
    # your network.
    ubuntu.vm.network 'private_network', type: 'dhcp'

    # Provider-specific configuration so you can fine-tune various
    # backing providers for Vagrant. These expose provider-specific options.
    ubuntu.vm.provider 'virtualbox' do |vb|
      # Display the VirtualBox GUI when booting the machine
      vb.gui = false

      vb.name = 'data-chan-win10-64'
      # Customize the amount of memory on the VM:
      vb.memory = '2048'
      vb.cpus = 2

      # Limit CPU usage
      vb.customize ['modifyvm', :id, '--cpuexecutioncap', '65']
    end

    ## Enable USB Controller on VirtualBox
    #ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['modifyvm', :id, '--usb', 'on']
    #  vb.customize ['modifyvm', :id, '--usbehci', 'on']
    #end

    ## Implement determined configuration attributes
    #ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'datachan tester',
    #                '--product', 'datachan tester']
    #end

    #ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'USBasp',
    #                '--product', 'USBasp']
    #end

    ###############################################################
    windows_10_64.vm.provision 'shell', privileged: false, inline: <<-SHELL
       echo "aaaa"

     SHELL
  end
end
