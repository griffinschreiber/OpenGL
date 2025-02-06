#!/bin/sh

echo "Please build cglm and put the .a file in the lib directory or this script will not work."

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
