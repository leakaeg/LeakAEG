#include <sys/types.h>
#include <sys/socket.h>

#define leak_size 1680
int leak_fds[2];
char leak_buf[leak_size];

void hexdump_memory(unsigned char *buf, size_t byte_count) {
  unsigned long byte_offset_start = 0;
  if (byte_count % 16)
    errx(1, "hexdump_memory called with non-full line");
  for (unsigned long byte_offset = byte_offset_start; byte_offset < byte_offset_start + byte_count;
          byte_offset += 16) {
    char line[1000];
    char *linep = line;
    linep += sprintf(linep, "%08lx  ", byte_offset);
    for (int i=0; i<16; i++) {
      linep += sprintf(linep, "%02hhx ", (unsigned char)buf[byte_offset + i]);
    }
    linep += sprintf(linep, " |");
    for (int i=0; i<16; i++) {
      char c = buf[byte_offset + i];
      if (isalnum(c) || ispunct(c) || c == ' ') {
        *(linep++) = c;
      } else {
        *(linep++) = '.';
      }
    }
    linep += sprintf(linep, "|");
    puts(line);
  }
}

void leak_init()
{
	socketpair(AF_LOCAL, SOCK_DGRAM, 0, leak_fds);
}
void leak_alloc()
{
	char buffer[leak_size];
	memset(buffer, 0, leak_size);

	send(leak_fds[0], buffer, leak_size, 0);
}

void leak()
{
	memset(leak_buf, 0, leak_size);

	recv(leak_fds[1], leak_buf, leak_size, 0);
}

void leak_dump()
{
	hexdump_memory(leak_buf, leak_size);
}
