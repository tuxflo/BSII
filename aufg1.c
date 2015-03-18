#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  pid_t pid;

  pid = fork();
  if(pid < 0)
  {
    printf("Fehler bei fork()");
    exit -1;
  }

  return 0;
}
