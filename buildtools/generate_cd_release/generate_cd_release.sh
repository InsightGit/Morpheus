#!/bin/bash

echo "We are go for launch :D"

# Setting up Butler
curl -L -o butler.zip https://broth.itch.ovh/butler/linux-amd64/LATEST/archive/default
unzip butler.zip
chmod +x butler
./butler -V
./butler login

# Creating release package directory structure
mkdir morpheus-release/
mkdir morpheus-release/docs
mkdir morpheus-release/buildtools
mkdir morpheus-release/include
mkdir morpheus-release/lib
mkdir morpheus-release/tests
mkdir morpheus-release/tests/gba-bin
mkdir morpheus-release/tests/nds-bin


# Copy files and directories to release package
cp -R include morpheus-release/include
cp -R buildtools morpheus-release/buildtools
cp -R public morpheus-release/docs
cp -R tests morpheus-release/tests
cp -R lib morpheus-release/lib
cp build/*.nds morpheus-release/tests/nds-bin
cp build/*.gba morpheus-release/tests/gba-bin
cp build/*.nds morpheus-release/tests/nds-bin


# Get rid of unneeded files and directories
rm -rf morpheus-release/buildtools/generate_breathe_rst
rm -rf morpheus-release/buildtools/generate_cd_release
rm -rf morpheus-release/buildtools/Dockerfile

# Push release package to itch.io
./butler push morpheus-release insighted/Morpheus:universal-dev

echo "Launch Successful!"