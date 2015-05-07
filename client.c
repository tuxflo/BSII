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
    char mtext[128]; //Max length of message text = 128
  } msg;

  signal(SIGINT, sig_handler);
  msgid = msgget(IPC_PRIVATE, IPC_CREAT | 00400 | 00200);
  while(loop)
  {
    if(msgid < 0)
    {
      perror("Fehler bei msgget()!\n!");
      exit(EXIT_FAILURE);
    }
    printf("Messagequeue created ID: %d\n", msgid);
    // read the message from queue
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
        printf("Ich bin das Kind \n");
        // Kind Code
        printf("received msg: %s\n", msg.mtext);
        char cp[256];
        sprintf(cp, "cp %s /tmp", msg.mtext);
        printf("executing system() with %s\n", cp);
        exit(EXIT_SUCCESS);
      }
    }
  }
}
