#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t sem;
int g_ant = 0;

void *writeloop(void *arg){
  while(1){
    sem_wait(&sem);

    if(g_ant < 10) {
      g_ant++;
      usleep(rand()%10);
      printf("%d\n", g_ant);
      sem_post(&sem);
    }
    else
      exit(0);
  }
}

int main(void)
{
  pthread_t tid;
  sem_init(&sem, 0, 0);
  sem_post(&sem);
  pthread_create(&tid, NULL, writeloop, NULL);
  writeloop(NULL);
  pthread_join(tid, NULL);
  return 0;
}
