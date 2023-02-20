#include <sys/types.h>
#include <sys/socket.h>

#define spray_size 600
int spray_fds[2];
void spray_init()
{
	socketpair(AF_LOCAL, SOCK_DGRAM, 0, spray_fds);
}
void spray(int times)
{
	char buffer[spray_size];
	memset(buffer, 0, spray_size);

	for(int i = 0; i < times; i++)
	{
		send(spray_fds[0], buffer, spray_size, 0);
	}
}

