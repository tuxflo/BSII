#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
//Signalhandler um Server richtig zu beenden...
void sig_handler(int signum)
{
	    printf("Received signal %d\n", signum);
}
//Server code

int get_key()
{
  int tmp;

  tmp = ftok("./aufg6.c", 42);
  if(tmp < 0)
  {
    perror("Fehler bei ftok()\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Shared Memory Segment angelegt! Token: %d\n", tmp);
    return tmp;
  }
}

int create_shm(int shmid)
{
  int shm;
  shm = shmget(shmid, 4096, IPC_CREAT | 0600);
  if(shm < 0)
  {
    perror("Fehler bei shmget!\n");
    exit(EXIT_FAILURE);
  }
  return shm;
}

void delete_shm(int shmid)
{
  int tmp;
  tmp = shmctl(shmid, IPC_RMID, NULL);
  if(tmp < 0)
  {
    perror("Fehler beim Löschen des SHM!\n");
    exit(EXIT_FAILURE);
  }
}

void write_time(int shmid)
{
  char *tmp;
  tmp = (char*)shmat(shmid, NULL, 0);
    if(tmp ==(void*) -1)
    {
      perror("Fehler bei shmat()!\n");
      exit(EXIT_FAILURE);
    }
  while(1)
  {

    time_t *t;
    time(t);
    char *tstring;
    tstring = ctime(t);
    strcpy(tmp, tstring);
    sleep(1);
  }
}

int main()
{
  signal(SIGINT, sig_handler);
  int token = get_key();
  int shmid = create_shm(token);
  write_time(shmid);
  delete_shm(shmid);
  return 0;
}
