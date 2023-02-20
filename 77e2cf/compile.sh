echo "[*] Case 77e2cfee3bc0fdd3bcaf05ea83a9c26a59ddbf6c"

pushd $PROJECT_HOME/linux-analysis
	cat $PROJECT_HOME/77ec2f/config >> .config
	make olddefconfig
	make -j `nproc`
popd
