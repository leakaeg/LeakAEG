echo "[*] Case 6b8d6b1847122db76e4ebd32b9d580684bac133c"

pushd $PROJECT_HOME/linux-analysis
	cat $PROJECT_HOME/6b8d6b/config >> .config
	make olddefconfig
	make -j `nproc`
popd
