#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* usleep */
#include <pthread.h>
#include <semaphore.h>
#define SHARED 0        /* process-sharing if !=0, thread-sharing if =0*/
#define BUF_SIZE 20
#define MAX_MOD 100000
#define NUM_ITER 100

void *Producer(void *); /* Producer thread */
void *Consumer(void *); /* Consumer thread */

sem_t empty;            /* empty: How many empty buffer slots */
sem_t full;             /* full: How many full buffer slots */
pthread_mutex_t mutex;  /* mutex: Mutex lock */
int g_data[BUF_SIZE];     /* shared finite buffer  */

int main (int argc, char *argv[]) {
  int N = atoi(argv[1]);
  printf("%d", N);
  pthread_t pid[N], cid[N];
  sem_init(&empty, SHARED, BUF_SIZE);
  sem_init(&full, SHARED, 0);
  pthread_mutex_init(&mutex,0);
  int id[N*2];
  //  pthread_t *pid1 = &pid[0], *cid1 = &cid[0];
  printf("main started\n");
  int i;
  for(i = 0; i<N; i++)
  {
   
    id[i*2] = i*2;
    pthread_create(&pid[i], NULL, Producer, (void*) &id[i*2]);
    id[(i*2)+1]=(i*2)+1;
    pthread_create(&cid[i], NULL, Consumer, (void*) &id[(i*2)+1]);
  }
  for(i = 0; i<N; i++)
  {
    pthread_join(pid[i], NULL);
    pthread_join(cid[i], NULL);
  }
   
  printf("main done\n");

  return 0;
}


void *Producer(void *arg) {
  int semvalem,i=0,j;          /* semvalem: semaphore value of empty semaphore */
  int *x = (int*) arg;
  int navn = *x;
  while(i < NUM_ITER) {
    //    usleep(rand()%MAX_MOD);    /* pretend to generate an item by a random wait*/
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    sem_getvalue(&empty, &semvalem);
    g_data[BUF_SIZE-(semvalem-1)]=1; /* put item in buffer */
    /* the following two lines just prints a bar showing current buffer fill */
    j=BUF_SIZE; printf("(Producer %d, semaphore empty is %d) \t", navn, semvalem);
    while(j > semvalem) { j--; printf("="); } printf("\n");
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    i++;
  }

  return 0;
}


void *Consumer(void *arg) {
  int semvalfu,i=0,j;       /* semvalfu: semaphore value of full semaphore */
  int *x = (int*) arg;
  int navn = *x;
  while(i < NUM_ITER) {
    //    usleep(rand()%MAX_MOD); /* pretend to generate an item by a random wait*/
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    sem_getvalue(&full, &semvalfu);
    g_data[semvalfu]=0;       /* remove item from buffer */
    /* the following two lines just prints a bar showing current buffer fill */
    j=0; printf("(Consumer %d, semaphore full is %d)  \t", navn, semvalfu);
    while(j < semvalfu) { j++; printf("="); } printf("\n");
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    i++;
  }

  return 0;

}

