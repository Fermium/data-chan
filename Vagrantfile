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
    # ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['modifyvm', :id, '--usb', 'on']
    #  vb.customize ['modifyvm', :id, '--usbehci', 'on']
    # end

    ## Implement determined configuration attributes
    # ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'datachan tester',
    #                '--product', 'datachan tester']
    # end

    # ubuntu.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'USBasp',
    #                '--product', 'USBasp']
    # end

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
  config.vm.define 'windows' do |windows|
    # Every Vagrant development environment requires a box. You can search for
    # boxes at https://atlas.hashicorp.com/search.
    windows.vm.box = 'eval-win2012r2-enterprise-ssh'
    windows.vm.box_url = 's3://fermiumlabs-vagrant-boxes/virtualbox/eval-win2012r2-standard-ssh-nocm-1.0.4.box'
    # windows.vm.network 'private_network', type: 'dhcp'

    # Port forward WinRM and RDP
    windows.vm.network :forwarded_port, guest: 3389, host: 3389, id: 'rdp', auto_correct: true
    windows.vm.communicator = 'winrm'
    windows.vm.guest = :windows

    windows.vm.network :forwarded_port, guest: 5985, host: 55_985, id: 'winrm', auto_correct: true
    # Port forward SSH
    windows.vm.network :forwarded_port, guest: 22, host: 2222, id: 'ssh', auto_correct: true

    windows.windows.halt_timeout = 20
    # username/password for accessing the image
    # windows.winrm.username = "vagrant"
    # windows.winrm.password = "vagrant"
    windows.vm.provider :virtualbox do |v, _override|
      v.gui = true
      v.customize ['modifyvm', :id, '--memory', 1536]
      v.customize ['modifyvm', :id, '--cpus', 1]
      v.customize ['modifyvm', :id, '--vram', '256']
      v.customize ['setextradata', 'global', 'GUI/MaxGuestResolution', 'any']
      v.customize ['setextradata', :id, 'CustomVideoMode1', '1024x768x32']
    end

    ## Enable USB Controller on VirtualBox
    # windows.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['modifyvm', :id, '--usb', 'on']
    #  vb.customize ['modifyvm', :id, '--usbehci', 'on']
    # end

    ## Implement determined configuration attributes
    # windows.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'datachan tester',
    #                '--product', 'datachan tester']
    # end

    # windows.vm.provider 'virtualbox' do |vb|
    #  vb.customize ['usbfilter', 'add', '0',
    #                '--target', :id,
    #                '--name', 'USBasp',
    #                '--product', 'USBasp']
    # end
    ###############################################################
    windows.vm.provision 'shell', privileged: true, path: "scripts/vagrant_provision_win.ps1"
  end
end
