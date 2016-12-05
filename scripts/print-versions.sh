#!/bin/bash
export NVM_DIR="/root/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm

printf "\n\n\n\nPrinting software versions:\n"

echo "Linux version: $(uname -a)"
echo "gcc version: $(gcc -v)"
echo "g++ version: $(g++ -v)"
echo "Swig version: $(swig -version)"
echo "Node Package Manager: $(npm --version)"
echo "Node: $(node --version)"
echo "Node Version Manager: $(nvm --version)"
echo "Node V8: $(node -e "console.log(process.versions.v8)")"
