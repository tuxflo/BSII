#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int filework(FILE *fp);

int main(int argc, char** argv)
{
  int pid;
  FILE *fp;
  for(int i=0; i<5; i++)
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
      filework(fp);
      exit(0);
    }
    else
    {
      printf("Ich bin dein Vater!\n");
      wait();
    }
  }
  return 0;
}

int filework(FILE *fp)
{
  int tmp = 0;
  fp = fopen("./zahl.dat", "r+");
  if(fp == NULL)
  {
    perror("Fehler beim öffnen der Datei");
    return EXIT_FAILURE;
  }
  while((tmp = fgetc(fp))!=EOF)
  {
    printf("%c", tmp);
  }
  fclose(fp);
  return 0;
}
