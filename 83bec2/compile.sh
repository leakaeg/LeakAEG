echo "[*] Case 83bec290888c08680fb630ec3a2bc87d0ad4b73f"

pushd $PROJECT_HOME/linux-analysis
	cat $PROJECT_HOME/83bec2/config >> .config
	make olddefconfig
	make -j `nproc`
popd
