function build
{
    echo "NOTE: specify your own LLVM_DIR and LLVM_ROOT"
    JOBS=`getconf _NPROCESSORS_ONLN`
    mkdir build
    pushd build
    cmake ../ \
        -DLLVM_CMAKE_PATH=/usr/lib/llvm-12/lib/cmake \
        -DCMAKE_BUILD_TYPE=Debug \

    make -j${JOBS}
    popd
}

build
