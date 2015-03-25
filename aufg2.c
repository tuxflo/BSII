#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <stdlib.h>

#define NUM_CHILDS 5
#define KEY 123458L
#define NUM_FILEOPERATIONS 20000

void p(int semid)
{
  printf("P() ausgeführt, Semaphore für kritischen abschnitt gelockt\n");
  /*semop(semid, 0, 1);*/
}

void v()
{
  printf("V() ausgeführt, Semaphore nach kririschem Abnschitt freigegeben\n");
}
int main(int argc, char** argv)
{
  int pid, tmp;
  int status;
  int semid;

  //einzelnen Sohn erzeugen
  pid = fork();
  switch(pid)
  {
    case -1:
      perror("Fehler bei fork()");
      return EXIT_FAILURE;

    case 0: //Sohn Prozess
      printf("Ich bin der Sohn!\n");
      v();
      printf("Semaphore deblockiert\n");
      exit(EXIT_SUCCESS);

    default: //Vater Prozess
      semid = semget(KEY, 0, IPC_PRIVATE);
      if(semid < 0)
      {
        //Semaphore existiert noch nicht, anlegen
        umask(0);
        semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0700);
        if(semid < 0)
        {
          perror("Fehler beim Anlegen des Semaphors!");
          exit(EXIT_FAILURE);
        }
        printf("Semaphore angelegt! ID:%d \n", semid);
      }
      //Semaphore auf 1 setzen (geschlossen)
      tmp = semctl(semid, 0, SETVAL, (int) 1);
      if(tmp == -1)
      {
        perror("Fehler beim Schliessen des Semaphores");
        exit(EXIT_FAILURE);
      }
      p(semid);
      waitpid(pid, &status, 0);
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
