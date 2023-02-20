#!/bin/bash -e

# Environment setup scripts for all subprojects

pushd "$(dirname $BASH_SOURCE)" > /dev/null

# Setup common env variables
. $(pwd)/common.sh

#echo "Select the kernel version
export KERNEL_DIR=$PROJECT_HOME/linux
export KERNEL_BUILD=$PROJECT_HOME/build
export KERNEL_IMAGE=$PROJECT_HOME/image
export ENV_SETUP=SETUP

for SUBDIR in $(ls -d */); do
  [ ! -f $SUBDIR/envsetup.sh ] || . $SUBDIR/envsetup.sh
done

# TODO: Another way?
printenv 'all subprojects' \
        'PROJECT_HOME SETUP_SCRIPT_HOME KERNEL_DIR KERNEL_IMAGE GCC_HOME PATH'

popd > /dev/null
