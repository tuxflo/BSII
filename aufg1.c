#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  int pid;

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
      printf("Ich bin das Kind Nummer %i \n", i+1); exit(0); }
    else
    {
      printf("Ich bin dein Vater!\n");
      wait();
    }
  }
  return 0;
}
