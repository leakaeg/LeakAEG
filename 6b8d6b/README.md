# Case 6b8d6b1847122db76e4ebd32b9d580684bac133c

## Instruction

### Build kernel

```
$ ./compile.sh
```

### Extract corruption information

```
$ cd $PROJECT_HOME
$ python $PROJECT_HOME/script/analysis.py --poc $PROJECT_HOME/6b8d6b/poc.c
```

Analysis Result: 

`$ cat $PROJECT_HOME/tmp/CI_0.log`

```
CRASH TYPE : DF
CRASH ZONE : kmalloc-512
ALLOC SYSCALL : syscall(__NR_ioctl, r[0], 0xc0481273, 0x20001f8a);
FREE SYSCALL : syscall(__NR_ioctl, r[0], 0xc0481273, 0x20001f8a);
DOUBLE FREE SYSCALL : syscall(__NR_ioctl, r[0], 0xc0481273, 0x20001f8a);
SPRAY : 0
```

### Comparison Anaylsis 

```
$ cd $PROJECT_HOME
$ python $PROJECT_HOME/script/comparison.py --ci $PROJECT_HOME/tmp/CI_X.log
(CI_X.log is the output file of corruption analysis)
```
