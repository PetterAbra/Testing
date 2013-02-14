#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void process(int number, int time) {
  printf("Prosess %d kjører\n", number);
  sleep(time);
  printf(" %d %d terminerte\n", number, time);
}

int main(void)
{
  pid_t pid[7];
  pid[1] = fork();
  if(pid[1] == 0)
    {
      process(1, 1);
      exit(0);
    }

  pid[3] = fork();
  if(pid[3]==0){
    process(3,3);
    exit(0);
  }

  waitpid(pid[1], NULL, 0);
  if(pid[2] = fork() == 0)
    {
      process(2, 2);
      exit(0);
    }
  if(pid[5] = fork() == 0)
    {
      process(5, 3);
      exit(0);
    }

  waitpid(pid[3], NULL, 0);
  waitpid(pid[2], NULL, 0);

  if(pid[4] = fork() == 0)
    {
      process(4, 2);
      exit(0);
    }

  waitpid(pid[5], NULL, 0);

  if(pid[6] = fork() == 0)
    {
      process(6, 3);
      exit(0);
    }
  waitpid(pid[4], NULL, 0); 
  waitpid(pid[6], NULL, 0);
  printf("FERDIG \n");

  return 0;

}





