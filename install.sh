#!/bin/bash
# This script builds all dependencies of the ntlib number theory library.
# For Linux and Mac OS X systems.

CUR_DIR=`pwd`

# Installs the header files to the given location.
if [ $# -ge 1 ]; then
  NTLIB_INSTALL_PREFIX=${1}
else
  echo "ERROR: Specify a directory to install into."
  exit 1
fi

# Get absolute path name of installation directory.
mkdir -p "${NTLIB_INSTALL_PREFIX}/include/ntlib" 2> /dev/null
cd "${NTLIB_INSTALL_PREFIX}" > /dev/null 2>&1
if [ $? != 0 ]; then
  echo "ERROR: Directory 'S{NTLIB_INSTALL_PREFIX}' does not exist nor could be created."
  echo "Please choose another directory."
  exit 1
else
  NTLIB_INSTALL_PREFIX=`pwd -P`
fi

echo "Library will be installed in '${NTLIB_INSTALL_PREFIX}'."

# Removing old files.
echo "Removing old files."
rm -rf "${NTLIB_INSTALL_PREFIX}/include/ntlib"
if [ $? != 0 ]; then
  echo "WARNING: Could not remove old header files."
fi

# Install library.
cd ${CUR_DIR}
cp -r ./include "${NTLIB_INSTALL_PREFIX}/include/ntlib"
if [ $? != 0 ]; then
  echo "ERROR: Installation failed."
  exit 1
fi

echo "SUCCESS: ntlib was installed successfully."
echo "The ntlib include files are located in '${NTLIB_INSTALL_PREFIX}/include'."
