#!/bin/bash -e
PROJECT_DIR=$(realpath $(dirname $BASH_SOURCE)/../../)
. $PROJECT_DIR/setup/common.sh

export GCCROOT=$GCC_HOME
export PATH=$GCCROOT/install/bin/:$PATH

printenv 'GCC' 'GCCROOT'
