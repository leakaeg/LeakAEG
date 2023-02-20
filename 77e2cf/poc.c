#define _GNU_SOURCE

#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

uint64_t r[1] = {0xffffffffffffffff};

int main(void)
{
  syscall(__NR_mmap, 0x1ffff000ul, 0x1000ul, 0ul, 0x32ul, -1, 0ul);
  syscall(__NR_mmap, 0x20000000ul, 0x1000000ul, 7ul, 0x32ul, -1, 0ul);
  syscall(__NR_mmap, 0x21000000ul, 0x1000ul, 0ul, 0x32ul, -1, 0ul);
  intptr_t res = 0;
  memcpy((void*)0x20000200, "/dev/infiniband/rdma_cm\000", 24);
  res = syscall(__NR_openat, 0xffffffffffffff9cul, 0x20000200ul, 2ul, 0ul);
  if (res != -1)
    r[0] = res;
  memcpy((void*)0x200064c0,
         "\x00\x00\x00\x00\x18\x00\x00\xfa\x00\x00\x00\x00\x00\x00\x00\x00",
         16);
  *(uint64_t*)0x200064d0 = -1;
  memcpy((void*)0x200064d8, "\x11\x01\x00\x00\x00\x00\x04\x00", 8);
  syscall(__NR_write, r[0], 0x200064c0ul, 0x20ul);

  syscall(__NR_write, r[0], 0x200064c0ul, 0x21ul);

  getchar();
  return 0;
}
