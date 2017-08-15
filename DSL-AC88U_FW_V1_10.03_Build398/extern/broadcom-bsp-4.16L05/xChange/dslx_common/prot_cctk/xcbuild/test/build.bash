#!/bin/bash

SOURCE_PROJ_DIR=`cd ../../..;pwd`

BUILD_HOST_TYPE=i386-linux


if [ ! -n "${XCHG_COMPILER_BIN_PATH}" ] ; then
   echo "Error: XCHG_COMPILER_BIN_PATH not defined yet"
   exit 1
fi
if [ ! -n "${XCHG_LINUX_ROOT}" ] ; then
   echo "Error: XCHG_LINUX_ROOT not defined yet"
   exit 1
fi
if [ ! -n "${TARGET_OS_TYPE}" ] ; then
   echo "Error: TARGET_OS_TYPE not defined yet"
   exit 1
fi
if [ ! -n "${TARGET_LINUX_ARCH}" ] ; then
   echo "Error: TARGET_LINUX_ARCH not defined yet"
   exit 1
fi

# COMPILER_ROOT_DIR=${TOOLCHAIN}
Usage()
{
   echo ""
   echo "Usage: build <target>"
   exit 1
}

if [ ! -n "$1" ] ; then
   Usage
fi


echo "********************************************"
echo "** Build environment Initialized.."
echo "** Target OS = ${TARGET_OS_TYPE}"
echo "********************************************"
echo ""

export TARGET_OS_TYPE
export TARGET_LINUX_ARCH
export SOURCE_PROJ_DIR
export BUILD_HOST_TYPE

export XCHG_LINUX_ROOT
export XCHG_COMPILER_BIN_PATH

export PATH=$PATH:/$XCHG_COMPILER_BIN_PATH

echo ${XCHG_COMPILER_BIN_PATH}

echo ${PATH}

../../../xchg_common/tools/bin/i386-linux/make verbose=0 -f makefile $1

exit
