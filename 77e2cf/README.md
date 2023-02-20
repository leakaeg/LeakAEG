# Case 77e2cfee3bc0fdd3bcaf05ea83a9c26a59ddbf6c

## Instruction

### Build kernel

```
$ ./compile.sh
```

### Extract corruption information

```
$ cd $PROJECT_HOME
$ python $PROJECT_HOME/script/analysis.py --poc $PROJECT_HOME/77e2cf/poc.c
```

Analysis Result: 

`$ cat $PROJECT_HOME/tmp/CI_0.log`

```
CRASH TYPE : WP
CRASH ZONE : kmalloc-192
ALLOC SYSCALL : syscall(__NR_write, r[0], 0x200064c0ul, 0x20ul);
FREE SYSCALL : syscall(__NR_write, r[0], 0x200064c0ul, 0x20ul);
CRASH SYSCALL : syscall(__NR_write, r[0], 0x200064c0ul, 0x21ul);
AVAILABILITY : 80
SPRAY : 0
```

### Comparison Anaylsis 

```
$ cd $PROJECT_HOME
$ python $PROJECT_HOME/script/comparison.py --ci $PROJECT_HOME/tmp/CI_X.log
(CI_X.log is the output file of corruption analysis)
```
### Comparison Anaylsis 

```
$ cd $PROJECT_HOME
$ python $PROJECT_HOME/script/comparison.py --ci $PROJECT_HOME/tmp/CI_X.log
(CI_X.log is the output file of corruption analysis)
```
