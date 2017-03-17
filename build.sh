#! /bin/bash

set -ev

mkdir -p build
cd build
cmake -G Xcode ..
