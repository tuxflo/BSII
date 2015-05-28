#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>

#include "share.h"

//Client code


int main()
{
  struct msg_buf {
    long mtype;
    char mtext[MAXSIZE]; //Max length of message text defined in share.h
  } msg;
  int msgid, rc;

  msgid = msgget(INKEY, 00400 | 00200);
  if(msgid < 0)
  {
    perror("Fehler bei msgget()!\n!");
    exit(EXIT_FAILURE);
  }
  printf("Messagequeue created ID: %d\n", msgid);
    // message to send
    msg.mtype = 1; // set the type of message
    sprintf (msg.mtext, "%s", "my_awesome_file"); /* setting the right time format by means of ctime() */

    // send the message to queue
    rc = msgsnd(msgid, &msg, sizeof(msg.mtext), 0); // the last param can be: 0, IPC_NOWAIT, MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR
    if (rc < 0) 
    {
      perror("msgsnd failed!\n");
      return 1;
    }
}
