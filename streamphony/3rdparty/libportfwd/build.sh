#!/bin/bash
cd $1
mkdir build
cd build
cmake ..
make
cp libportfwd.a $2
cd $1
rm -rf build
rm -rf portfwd-demo
