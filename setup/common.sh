#!/bin/bash -e

printenv() {
echo -----------------------------------------------------------------------
echo "Setting environment for $1"
echo
  for ENV in $2; do
    echo "$ENV = ${!ENV} " | column -t
  done
echo -----------------------------------------------------------------------
}

DIR=$(dirname $BASH_SOURCE)
export SETUP_SCRIPT_HOME=$(realpath $DIR)
export PROJECT_HOME=$(realpath $DIR/../)
export TOOLS_HOME=$(realpath $PROJECT_HOME/tools)
export PATCH_HOME=$PROJECT_HOME/patch

# tools
export SYZKALLER_HOME=$TOOLS_HOME/syzkaller
export GO_HOME=$TOOLS_HOME/GO-1.14.2
export GCC_HOME=$TOOLS_HOME/gcc-7.5.0
export LLVM_HOME=$TOOLS_HOME/llvm-10.0.0
#export LLVM_ANDROID=$TOOLS_HOME/llvm-android
#export GDB_HOME=$TOOLS_HOME/gdb-8.1
