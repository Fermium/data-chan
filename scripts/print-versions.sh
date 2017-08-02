#!/bin/bash
printf "\n\n\n\nPrinting software versions:\n"

echo "Linux version: $(uname -a)"
echo "gcc version: $(gcc -v)"
echo "g++ version: $(g++ -v)"
echo "Swig version: $(swig -version)"
