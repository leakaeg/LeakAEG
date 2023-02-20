# Case 83bec290888c08680fb630ec3a2bc87d0ad4b73f

## Instruction

### Build kernel

```
$ ./compile.sh
```

### Extract corruption information

```
$ cd $PROJECT_HOME
$ python $PROJECT_HOME/script/analysis.py --poc $PROJECT_HOME/83bec2/poc.c
```

Analysis Result: 

`$ cat $PROJECT_HOME/tmp/CI_0.log`

```
CRASH TYPE : WP
CRASH ZONE : kmalloc-4096
ALLOC SYSCALL : syscall(__NR_ioctl, r[0], 0xc004743e, 0x20000100);
FREE SYSCALL : syscall(__NR_ioctl, r[0], 0x4004743c, 0x20d1df52);
CRASH SYSCALL : close(4);
AVAILABILITY : 2184
SPRAY : 0
```

### Comparison Anaylsis 

```
$ cd $PROJECT_HOME
$ python $PROJECT_HOME/script/comparison.py --ci $PROJECT_HOME/tmp/CI_X.log
(CI_X.log is the output file of corruption analysis)
```
