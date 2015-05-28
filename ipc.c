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

// global var for quiting the loop
int loop;
int msgid;
//Signalhandler um Server richtig zu beenden...
void sig_handler(int signum)
{
  if(signum == 2)
  {
    int rc;
    //quit the message queue
    rc=msgctl(msgid,IPC_RMID,NULL);
    if (rc < 0) {
      perror("Fehler bei msgctl()!\n");
      exit(EXIT_FAILURE);
    }
    printf("message queue %d is removed\n",msgid);
  }
}

//Server code

int main()
{
  int pid;
  int rc;
  loop = 1;

  struct msg_buf {
    long mtype;
    char mtext[MAXSIZE]; //Max length of message text defined in share.h
  } msg;

  signal(SIGINT, sig_handler);
  msgid = msgget(INKEY, IPC_CREAT | 00400 | 00200);
  if(msgid < 0)
  {
    perror("Fehler bei msgget()!\n!");
    exit(EXIT_FAILURE);
  }
  printf("Messagequeue created ID: %d\n", msgid);
  while(1)
  {
  // read the message from queue
  rc = msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0);
  pid = fork();
  if(pid < 0)
  {
    perror("Fehler bei fork()");
    return EXIT_FAILURE;
  }


  else if(pid == 0) // Kind Prozess
  {
    printf("I am a child... \n");
    if (rc < 0)
    {
      perror("receive failed!\n");
      return 1;
    }
    else
    {
      // Kind Code
      printf("received msg: %s\n", msg.mtext);
      char cp[256];
      sprintf(cp, "cp %s /tmp", msg.mtext);
      printf("executing system() with %s\n", cp);
      exit(EXIT_SUCCESS);
    }
  }
  else //Vater Prozess
  {
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
}
