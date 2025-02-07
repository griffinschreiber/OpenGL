#!/bin/sh

cd $(dirname "$0")

echo "Installing and updating submodules..."
git submodule init
git submodule update

echo "Generating files and building..."
autoreconf --install
autoheader
aclocal
autoconf
automake --add-missing --copy

echo "Done!"
