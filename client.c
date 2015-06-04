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
#include <string.h>

#include "share.h"

int msgid;

//Client code
int main()
{
  int rc;
  struct msg_buf {
    long mtype;
    char mtext[MAXSIZE]; //Max length of message text defined in share.h
  } msg;
  char str[MAXSIZE];
  msgid = msgget(INKEY, 0);
  if(msgid < 0)
  {
    perror("Fehler bei msgget()!\n!");
    exit(EXIT_FAILURE);
  }
  msg.mtype = 1;
  while(fgets(str, MAXSIZE, stdin))
  {
    strtok(str, "\n");  //remove \n from string
    sprintf(msg.mtext, "%s", str);
    rc = msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0);
    if(rc < 0)
    {
      perror("Fehler bei msgsnd()!\n");
      exit(EXIT_FAILURE);
    }
  }

  
}
