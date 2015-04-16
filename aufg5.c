#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/sem.h>
#define NUM_CHILDREN 100


/*try to solve the readers/wrtiters problem (simple solution)*/

int rc;

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
  //printf("Wert des Semaphores %d\n", semctl(semid, 0, GETVAL, 0));
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
  int tmp;
  printf("Semaphore beenden...\n");
  tmp = semctl(semid, 0, IPC_RMID, 0);
  if(tmp < 0)
  {
    perror("Fehler beim Löschen des Semaphores!\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}

int reader_code(int mutex, int wri)
{
  int tmp;
  p(mutex);
  int rc_val = semctl(rc, 0, GETVAL, 0);
  /*printf("rc_val Wert: %d\n", rc_val);*/
  rc_val++;
  tmp = semctl(rc, 0, SETVAL, rc_val);
  if(tmp == -1)
  {
    perror("Fehler beim setzen des Semaphoreswertes");
    exit(EXIT_FAILURE);
  }
  if(semctl(rc, 0, GETVAL, 0) == 1)
    p(wri);

  v(mutex);
  p(mutex);

  rc_val = semctl(rc, 0, GETVAL, 0);
  /*printf("rc_val Wert: %d\n", rc_val);*/
  rc_val = rc_val - 1;
  tmp = semctl(rc, 0, SETVAL, rc_val);
  if(tmp == -1)
  {
    perror("Fehler beim setzen des Semaphoreswertes");
    exit(EXIT_FAILURE);
  }
  if(semctl(rc, 0, GETVAL, 0) == 0)
    v(wri);
  v(mutex);
  printf("Fertig mit Lesen!\n");
  return 0;
}

int writer_code(int mutex, int wri)
{
  p(wri);
  sleep(2);
  v(wri);
  printf("Fertig mit Schreiben!\n");
  return 0;
}
/*ugly global semaphore to avoid shared memory segment, using it as counting variable*/



int main(int argc, char** argv)
{
  rc = create_sem(0);
  int wri, mutex;
  int pid, wpid;
 //semaphoren für  writer  und mutex geschlossen initialisieren
  wri = create_sem(1);
  mutex = create_sem(1);
  for(int i=0; i<NUM_CHILDREN; i++)
  {
    pid = fork();
    if(pid < 0)
    {
      perror("Fehler bei fork()");
      return EXIT_FAILURE;
    }
    else if(pid == 0) // Kind Prozess
    {
      if(i < 95) //gerade Anzahl, erzeuge Leser
      {
        /*printf("Ich bin der Leser Nummer %i \n", i+1);*/
        reader_code(mutex, wri);
      }
      else
      {
        /*printf("Ich bin der Schreiber Nummer %i \n", i+1);*/
        writer_code(mutex, wri);
      }
        exit(EXIT_SUCCESS);
    }
  }
  //auf alle Kinder warten...
  while((wpid = wait(NULL)) > 0);
  if(pid == -1)
  {
    perror("Fehler bei wait()");
    exit(EXIT_FAILURE);
  }
  printf("Ich bin der Vater und beende die Semaphoren...\n");
  remove_sem(mutex);
  remove_sem(wri);
  remove_sem(rc);
  //printf("Wert des Semaphores %d\n", semctl(semid, 0, GETVAL, 0));
  return 0;
}

