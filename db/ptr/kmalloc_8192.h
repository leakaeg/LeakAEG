#include <netinet/ip.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/tty.h>
#include <termios.h>
#include <pthread.h>
#include <syscall.h>
#include <sys/time.h>
#include <sys/resource.h>

int dp_sock;

void kmalloc_init()
{
	dp_sock = open("/dev/ptmx", O_RDWR);
}

void kmalloc_alloc()
{
	int n_hdlc = N_HDLC;
	ioctl(dp_sock, TIOCSETD, &n_hdlc);
}
