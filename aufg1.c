#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM_CHILDS 5
#define NUM_FILEOPERATIONS 20000

int filework(FILE *fp);

int main(int argc, char** argv)
{
  int pid;
  int status;
  int final_number;
  FILE *fp = NULL;
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
        filework(fp);
      }
        exit(EXIT_SUCCESS);
    }
    else
    {
      wait(&status);
      printf("Rückgabewert der Kinder: %d\n", status);
      fp = fopen("./zahl.dat", "r");
      if(fp == NULL)
      {
        perror("Fehler beim öffnen der Datei");
        return EXIT_FAILURE;
      }
      fscanf(fp, "%d", &final_number);
      printf("Wert nach Abschluss der Kindprozesse: %d\n", final_number);
    }
  }
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
