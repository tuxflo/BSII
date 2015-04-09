#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/sem.h>


/*try to solve the readers/wrtiters problem (simple solution)*/


void p(int semid)
{
  int tmp;
  struct sembuf semaphore = { 0, -1, 0};
  tmp = semop(semid,&semaphore,1);
  if(tmp == -1)
  {
    perror("Fehler bei semop()\n");
    exit(EXIT_FAILURE);
  }
  printf("Wert des Semaphores %d\n", semctl(semid, 0, GETVAL, 0));
}

void v(int semid)
{
  int tmp;
  struct sembuf semaphore = { 0, 1, 0};
  tmp = semop(semid, &semaphore, 1);
  if(tmp == -1)
  {
    perror("Fehler bei semop()\n");
    exit(EXIT_FAILURE);
  }
}

int create_sem(int value)
{
  int semid;
  int tmp;
  semid = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600);
  if(semid < 0)
  {
    perror("Fehler beim Anlegen des Semaphors!");
    exit(EXIT_FAILURE);
  }
  printf("Semaphore angelegt! ID:%d \n", semid);
  //Semaphore auf value setzen
  tmp = semctl(semid, 0, SETVAL, value);
  if(tmp == -1)
  {
    perror("Fehler beim Schliessen des Semaphores");
    exit(EXIT_FAILURE);
  }
  return semid;
}
int remove_sem(int semid)
{
  printf("Semaphore beenden...\n");
  tmp = semctl(semid, 0, IPC_RMID, 0);
  if(tmp < 0)
  {
    perror("Fehler beim Löschen des Semaphores!\n");
    exit(EXIT_FAILURE);
  }
}

int reader_code()
{
}

int writer_code()
{
}
/*ugly global semaphore to avoid shared memory segment*/

int rc = create_sem(0);

int main(int argc, char** argv)
{
  int wri, mutex;
 //semaphoren für  writer  und mutex geschlossen initialisieren
  wri = create_sem(1);
  mutex = create_sem(1);
  //einzelnen Sohn erzeugen
  pid = fork();
  switch(pid)
  {
    case -1:
      perror("Fehler bei fork()");
      return EXIT_FAILURE;

    case 0: // reader Prozess
      printf("Ich bin der Leser!\n");
      reader_code();
      exit(EXIT_SUCCESS);
  }
  writer_code();
  pid = wait(NULL);
  if(pid == -1)
  {
    perror("Fehler bei wait()");
    exit(EXIT_FAILURE);
  }
  printf("Ich bin der Vater!\n");
  printf("Wert des Semaphores %d\n", semctl(semid, 0, GETVAL, 0));
  return 0;
}

