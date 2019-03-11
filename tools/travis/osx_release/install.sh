#!/bin/bash

# Install Qt
brew install qt
brew linkapps qt
brew link --force qt

# Install dylibbundler
brew install dylibbundler

# Set Environment -> Extract Qt dirs from actual version
QMAKE_VERSION=`qmake -v`
QT_LIB_DIR=${QMAKE_VERSION##*in }
QT_BIN_DIR=${QT_LIB_DIR%/*}/bin

MNE_LIB_DIR=$(pwd)/lib

PATH=$QT_BIN_DIR:$PATH
export PATH

DYLD_LIBRARY_PATH=$MNE_LIB_DIR:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH
DYLD_FALLBACK_LIBRARY_PATH=$MNE_LIB_DIR
export DYLD_FALLBACK_LIBRARY_PATH

which qmake
