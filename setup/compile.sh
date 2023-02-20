pushd $PROJECT_HOME/linux
	echo "[*] Compile Linux"
	make defconfig
	make kvmconfig
	cat $PROJECT_HOME/setup/config >> .config
	make olddefconfig
	make -j `nproc`
popd

pushd $PROJECT_HOME/linux-analysis
	echo "[*] Compile Linux-analysis"
	make defconfig
	make kvmconfig
	cat $PROJECT_HOME/setup/config_sa >> .config
	make olddefconfig
	make -j `nproc`
popd

