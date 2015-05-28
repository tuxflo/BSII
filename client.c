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

int msgid;

//Client code
int main()
{
  int pid;
  int rc;
  struct msg_buf {
    long mtype;
    char mtext[MAXSIZE]; //Max length of message text defined in share.h
  } msg;

  msgid = msgget(INKEY, 0);
  if(id < 0)
  {
    perror("Fehler bei msgget()!\n!");
    exit(EXIT_FAILURE);
  }
  // read the message from queue
  while(loop)
  {
    rc = msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0);
    if (rc < 0)
    {
      perror("receive failed!\n");
      return 1;
    }
    else
    {
      pid = fork();
      if(pid < 0)
      {
        perror("Fehler bei fork()");
        return EXIT_FAILURE;
      }
      else if(pid == 0) // Kind Prozess
      {
        printf("I am a child... \n");
        // Kind Code
        printf("received msg: %s\n", msg.mtext);
        char cp[256];
        sprintf(cp, "cp %s /tmp", msg.mtext);
        printf("executing system() with %s\n", cp);
        exit(EXIT_SUCCESS);
      }
      else //Vater Prozess
      {
        wait(NULL);
        return 0;
      }
    }
  }
}