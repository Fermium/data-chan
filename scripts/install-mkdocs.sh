#!/bin/bash

printf "\n\n\n\nInstalling mkdocs and pip:\n"

curl -o- https://bootstrap.pypa.io/get-pip.py | python
pip install mkdocs
