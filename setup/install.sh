#!/bin/bash -e
. $(dirname $BASH_SOURCE)/common.sh

if [ ! -d $PROJECT_HOME/tools ]
then
        mkdir $PROJECT_HOME/tools
fi

for _SUBDIR in linux image GCC ; do
  SUBDIR=$(basename $_SUBDIR)
  [ ! -f $SETUP_SCRIPT_HOME/$SUBDIR/envsetup.sh ] || . $SETUP_SCRIPT_HOME/$SUBDIR/envsetup.sh
  [ ! -f $SETUP_SCRIPT_HOME/$SUBDIR/install.sh ] || $SETUP_SCRIPT_HOME/$SUBDIR/install.sh
done
