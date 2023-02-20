pushd $PROJECT_HOME
	rm -rf ./linux 2>/dev/null
	rm -rf ./linux-analysis 2>/dev/null

	git clone --branch v4.15 https://github.com/torvalds/linux linux
	cp -rf linux linux-analysis
	echo "Patch Linux-analysis for corruption information"
	pushd $PROJECT_HOME/linux-analysis
  		patch -p1 < $PROJECT_HOME/setup/linux/patch/report.diff
		patch -p1 < $PROJECT_HOME/setup/linux/patch/slub.diff
		patch -p1 < $PROJECT_HOME/setup/linux/patch/uaccess.diff
	popd
popd
