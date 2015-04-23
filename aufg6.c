#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

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

void create_shm(int key)
{
  int shm;
  shm = shmget(key, 4096, IPC_CREAT | 0600);
  if(shm < 0)
  {
    perror("Fehler bei shmget!\n");
    exit(EXIT_FAILURE);
  }
}

void delete_shm(int key)
{
  int tmp;
  tmp = shmctl(key, IPC_RMID, NULL);
  if(tmp < 0)
  {
    perror("Fehler beim Löschen des SHM!\n");
    exit(EXIT_FAILURE);
  }
}

void write_time(int key)
{
  int tmp = shmat();
    if(tmp ==(void*) == -1)
    {
      perror("Fehler bei shmat()!\n");
      exit(EXIT_FAILURE);
    }
  while(true)
  {

    time_t t = time();
  }
}
int main()
{
  int key = get_key();
  create_shm(key);
  delete_shm(key);
  return 0;
}
