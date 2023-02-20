#include "header.h"
#include <fcntl.h>

int main()
{
	kmsg_fd = open("/dev/kmsg", O_RDWR);
	write(kmsg_fd, "\n\n", 2);
	write(kmsg_fd, "[-] Debugging Start\n", strlen("[-] Debugging Start\n"));
	close(kmsg_fd);
memset(r, -1, sizeof(r));
syscall(__NR_mmap, 0x20000000, 0x7000, 3, 0x32, -1, 0);
memcpy((void*)0x20000000, "/dev/sg#", 9);
	kmsg_fd = open("/dev/kmsg", O_RDWR);
	write(kmsg_fd, "[-] r[0] = syz_open_dev(0x20000000, 0, 0);\n", strlen("[-] r[0] = syz_open_dev(0x20000000, 0, 0);\n"));
	close(kmsg_fd);
	r[0] = syz_open_dev(0x20000000, 0, 0);
memcpy((void*)0x20001f8a,
	"\xf9\xda\x28\x06\x6d\xb5\xd5\xf8\xaf\xd8\xcf\xc6\x33\xff\x20\x6e\xef"
	"\x50\xe6\x78\x98\xb4\x2e\x5a\x8c\xb1\x6d\x6b\x4d\x5a\x92\x1f\x0c\x00"
	"\x00\x00\x01\x00\x00\x00\x01\x5b\xd8\x9b\x97\x9c\xd9\xf6\x84\x2a\xca"
	"\x1c\xe0\xf7\xcf\xf7\xad\x5f\xa9\xf5\x29\xea\xe6\xb5\x4d\xf3\x45\x95"
	"\x37\xfd\x96\x21\x6c\xa5\x83\x33\xe2\x14\x7c\x5a\xc7\x78\xab\x1f\x68"
	"\x0a\x27\x61\xea\x3b\xb2\x0a\xbe\x0f\xbe\xf2\x63\xbb\x5d\x16\x85\x42"
	"\x62\x4f\x89\x82\x83\x36\x9d\xdf\x1a\xc2\xb9\xe6\x50\x63\x7e\x7f",
	118);
	kmsg_fd = open("/dev/kmsg", O_RDWR);
	write(kmsg_fd, "[-] syscall(__NR_ioctl, r[0], 0xc0481273, 0x20001f8a);\n", strlen("[-] syscall(__NR_ioctl, r[0], 0xc0481273, 0x20001f8a);\n"));
	close(kmsg_fd);
	syscall(__NR_ioctl, r[0], 0xc0481273, 0x20001f8a);
	
	kmsg_fd = open("/dev/kmsg", O_RDWR);
	write(kmsg_fd, "[-] close(4);\n", strlen("[-] close(4);\n"));
	close(4);
	write(kmsg_fd, "[-] close(5);\n", strlen("[-] close(5);\n"));
	close(5);
	close(kmsg_fd);
	kmsg_fd = open("/dev/kmsg", O_RDWR);
	write(kmsg_fd, "[-] Debugging End\n", strlen("[-] Debugging End\n"));
}
	close(kmsg_fd);
