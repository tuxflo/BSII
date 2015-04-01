#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <stdlib.h>

void p(int semid)
{
  int tmp;
  struct sembuf semaphore = { 0, -1, SEM_UNDO};
  tmp = semop(semid,&semaphore,1);
  if(tmp == -1)
  {
    perror("Fehler bei semop()\n");
    exit(EXIT_FAILURE);
  }
  printf("Warte bis der Sohn mich deblockiert...\n");
}

void v(int semid)
{
  int tmp;
  struct sembuf semaphore = { 0, 1, SEM_UNDO};
  tmp = semop(semid, &semaphore, 1);
  if(tmp == -1)
  {
    perror("Fehler bei semop()\n");
    exit(EXIT_FAILURE);
  }
  printf("V() ausgeführt, Semaphore nach kririschem Abnschitt freigegeben\n");
}

int create_sem()
{
  int semid;
  int tmp;
  semid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0755);
  if(semid < 0)
  {
    perror("Fehler beim Anlegen des Semaphors!");
    exit(EXIT_FAILURE);
  }
  printf("Semaphore angelegt! ID:%d \n", semid);
  //Semaphore auf 1 setzen (geschlossen)
  tmp = semctl(semid, 0, SETVAL, 1);
  if(tmp == -1)
  {
    perror("Fehler beim Schliessen des Semaphores");
    exit(EXIT_FAILURE);
  }
  return semid;
}

int main(int argc, char** argv)
{
  int pid, tmp;
  int status;
  int semid = create_sem();
  //einzelnen Sohn erzeugen
  pid = fork();
  switch(pid)
  {
    case -1:
      perror("Fehler bei fork()");
      return EXIT_FAILURE;

    case 0: //Sohn Prozess
      printf("Ich bin der Sohn!\n");
      v(semid);
      printf("Semaphore deblockiert\n");
      exit(EXIT_SUCCESS);

    default: //Vater Prozess
      printf("Ich bin der Vater!\n");
      p(semid);
      tmp= waitpid(pid, &status, 0);
      if(tmp != -1)
        return EXIT_FAILURE;
      printf("Rückgabewert des Sohnes: %d\n", status);
      printf("Semaphore beenden...\n");
      tmp = semctl(semid, 0, IPC_RMID, 0);
      if(tmp < 0)
      {
        perror("Fehler beim Löschen des Semaphores!\n");
        exit(EXIT_FAILURE);
      }
      return 0;
  }
}
