#!/bin/bash -e

GCC_VER=gcc-7.5.0
GCC_DIR=$GCC_HOME

if [ ! -d "$GCC_DIR" ]; then
  pushd $TOOLS_HOME
  rm -f $GCC_VER.tar.xz
  wget https://ftp.gnu.org/gnu/gcc/$GCC_VER/$GCC_VER.tar.xz
  tar xf $GCC_VER.tar.xz
  rm -f $GCC_VER.tar.xz
  cd $GCC_DIR > /dev/null
  echo "[*] Patch GCC for corruption information"
  patch -p1 < $PROJECT_HOME/setup/GCC/patch/xmalloc.diff
  patch -p1 < $PROJECT_HOME/setup/GCC/patch/vec.diff
  patch -p1 < $PROJECT_HOME/setup/GCC/patch/asan.diff
  popd > /dev/null
fi

pushd ./ > /dev/null
  cd $GCC_DIR > /dev/null
  if [ ! -d "build" ]; then
          mkdir build
  fi
  if [ ! -d "install" ]; then
          mkdir install
  fi
  ./contrib/download_prerequisites
  cd build
  ../configure --enable-languages=c,c++ --disable-bootstrap \
               --enable-checking=no --with-gnu-as \
               --with-gnu-ld --with-ld=/usr/bin/ld.bfd \
               --disable-multilib --enable-plugin --prefix=$GCC_DIR/install/
  make -j`nproc`
  make install
popd > /dev/null
