pushd $PROJECT_HOME

mkdir $KERNEL_IMAGE 2>/dev/null
cd $KERNEL_IMAGE/
wget https://raw.githubusercontent.com/google/syzkaller/master/tools/create-image.sh -O create-image.sh
chmod +x create-image.sh
./create-image.sh

popd
