#!/bin/bash
# This script builds all dependencies of the ntlib number theory library.
# For Linux and Mac OS X systems.

CUR_DIR=`pwd`

# Installs into the home directory unless another location prefix is specified.
NTLIB_INSTALL_PREFIX=${HOME}
if [ $# -ge 1 ]; then
  NTLIB_INSTALL_PREFIX=${1}
fi

# Get absolute path name of installation directory.
mkdir -p "${NTLIB_INSTALL_PREFIX}" 2> /dev/null
cd "${NTLIB_INSTALL_PREFIX}" > /dev/null 2>&1
if [ $? != 0 ]; then
  echo "ERROR: Directory 'S{NTLIB_INSTALL_PREFIX}' does not exist nor could be created."
  echo "Please choose another directory."
  exit 1
else
  NTLIB_INSTALL_PREFIX=`pwd -P`
fi

echo "Library will be installed in '${NTLIB_INSTALL_PREFIX}'."

# Get the directory name the script is executed in.
cd "${CUR_DIR}"
OLD_DIR="$( cd "$( dirname "$0" )" && pwd )"
cd "${OLD_DIR}"
OLD_DIR=`pwd`

# Change into the build directory.
cd build
if [ $? != 0 ]; then
  exit 1
fi

# Clean up build directory.
./clean.sh
if [ $? != 0 ]; then
  exit 1
fi

# Run cmake.
cmake -DCMAKE_INSTALL_PREFIX="${NTLIB_INSTALL_PREFIX}" ..
if [ $? != 0 ]; then
  echo "ERROR: cmake build failed."
  exit 1
fi

# Run make.
make ntlib
if [ $? != 0 ]; then
  echo "ERROR: Build failed."
  exit 1
fi

# Removing old files.
echo "Removing old files."
echo "rm -rf '${NTLIB_INSTALL_PREFIX}/include/ntlib/*'"
rm -rf "${NTLIB_INSTALL_PREFIX}/include/ntlib/*"
if [ $? != 0 ]; then
  echo "WARNING: Could not remove old header files."
fi
echo "rm -f '${NTLIB_INSTALL_PREFIX}/lib/libntlib*'"
rm -f "${NTLIB_INSTALL_PREFIX}/lib/libntlib"
if [ $? != 0 ]; then
  echo "WARNING: Could not remove old library files."
fi

# Install library.
make install
if [ $? != 0 ]; then
  echo "ERROR: Installation failed."
  exit 1
fi

cd ..

if [ "`pwd`" != "${OLD_DIR}" ]; then
  echo "ERROR: We are not in the original directory ${OLD_DIR} now."
  exit 1
fi

echo "SUCCESS: ntlib was installed successfully."
echo "The ntlib include files are located in '${NTLIB_INSTALL_PREFIX}/include'."
echo "The library files are located in '${NTLIB_INSTALL_PREFIX}/lib'."
