#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/sem.h>

#define NUM_CHILDS 5
#define NUM_FILEOPERATIONS 20000

int filework(FILE *fp);

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
}

int create_sem()
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
  int pid;
  int final_number;
  FILE *fp = NULL;
  int semid = create_sem();
  for(int i=0; i<NUM_CHILDS; i++)
  {
    pid = fork();
    if(pid < 0)
    {
      perror("Fehler bei fork()");
      return EXIT_FAILURE;
    }
    else if(pid == 0) // Kind Prozess
    {
      printf("Ich bin das Kind Nummer %i \n", i+1);
      for(int c=0; c<NUM_FILEOPERATIONS; c++)
      {
        p(semid);
        filework(fp);
        v(semid);
      }
        exit(EXIT_SUCCESS);
    }
  }
  for(int i=0; i<NUM_CHILDS; i++)
  {
    pid = wait(NULL);
    if(pid == -1)
    {
      perror("Fehler bei wait()");
      exit(EXIT_FAILURE);
    }

  }
  /*waitpid(pid, &status, 0);*/
  fp = fopen("./zahl.dat", "r");
  if(fp == NULL)
  {
    perror("Fehler beim öffnen der Datei");
    return EXIT_FAILURE;
  }
  fscanf(fp, "%d", &final_number);
  printf("Wert nach Abschluss der Kindprozesse: %d\n", final_number);
  return 0;
}

//open file, read number, write incremented number, close file
int filework(FILE *fp)
{
  int tmp = 0;
  fp = fopen("./zahl.dat", "r+");
  if(fp == NULL)
  {
    perror("Fehler beim öffnen der Datei");
    return EXIT_FAILURE;
  }
  fscanf(fp, "%d", &tmp);
  //printf("eingelesene Zahl: %d\n", tmp);
  tmp++;
  rewind(fp);
  fprintf(fp, "%d", tmp);
  if(fclose(fp)!=0)
  {
    perror("Fehler bei fclose:");
    exit(EXIT_FAILURE);
  }
  return 0;
}
