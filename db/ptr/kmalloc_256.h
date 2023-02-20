#include <sound/asound.h>
#include <sound/asequencer.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int dp_sock;

void kmalloc_init()
{
	dp_sock = open("/dev/snd/seq", O_RDWR);
}

void kmalloc_alloc()
{
	int client_id;
	struct snd_seq_port_info info = {0};

	ioctl(dp_sock, SNDRV_SEQ_IOCTL_CLIENT_ID, &client_id);

	info.addr.client=client_id;
	info.addr.port=255;

	ioctl(dp_sock, SNDRV_SEQ_IOCTL_CREATE_QUEUE, &info);
}
