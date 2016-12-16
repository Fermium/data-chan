#!/bin/bash

printf "\n\n\n\nInstalling node:\n"

export NODE_VER=6
curl -o- https://deb.nodesource.com/setup_6.x -o nodesource_setup.sh | bash
apt-get install -y nodejs
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.32.1/install.sh | bash
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm
nvm install $NODE_VER && nvm alias default $NODE_VER nvm use default
