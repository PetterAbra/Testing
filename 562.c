#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* usleep */
#include <pthread.h>
#include <semaphore.h>
#define SHARED 1        /* process-sharing if !=0, thread-sharing if =0*/
#define BUF_SIZE 20
#define MAX_MOD 100000
#define NUM_ITER 100


void *Producer(void *); /* Producer thread */
void *Consumer(void *); /* Consumer thread */

sem_t empty;            /* empty: How many empty buffer slots */
sem_t full;             /* full: How many full buffer slots */
pthread_mutex_t mutex;  /* mutex: Mutex lock */
int g_data[BUF_SIZE];     /* shared finite buffer  */
int N;

int main(int args, char* argv[]) {

  // Gets the number that is sent as an argument.
  N = atoi(argv[1]);
  //Creat an array of id's
  int id[N];
  // Creat an array of producers and consumers
  pthread_t pid[N], cid[N];
  sem_init(&empty, SHARED, BUF_SIZE);
  sem_init(&full, SHARED, 0);
  pthread_mutex_init(&mutex,0);

  printf("main started\n");
  int i;
  // Stors the id values on dynamicstore and gives every slot an id.
  for(i = 0; i<N; i++){
    id[i]=*(int*)malloc(sizeof(int));
    id[i]=i;
  }

  // Creat N consumers and N producers
  for(i=0;i<N;i++){
    pthread_create(&pid[i], NULL, Producer, (void*)&id[i] );
    pthread_create(&cid[i], NULL, Consumer, (void*)&id[i] );
  }

  for(i =0; i<N; i++){
    pthread_join(pid[i], NULL);
    pthread_join(cid[i], NULL);
  }
  printf("main done\n");
  return 0;
}


void *Producer(void *arg) {
  // Stors the id from its parametes
  int *id = (int*)arg;
  int semvalem,i=0,j;/* semvalem: semaphore value of empty semaphore */
  while(i < NUM_ITER) {
    usleep(rand()%MAX_MOD);    /* pretend to generate an item by a random wait*/
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    sem_getvalue(&empty, &semvalem);
    g_data[BUF_SIZE-(semvalem-1)]=1; /* put item in buffer */
    /* the following two lines just prints a bar showing current buffer fill */
    j=BUF_SIZE; printf("(Producer %d, semaphore empty is %d) \t",*id,semvalem);
    while(j > semvalem) { j--; printf("="); } printf("\n");
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    i++;
  }

  return 0;
}

void *Consumer(void *arg){
  int semvalfu,i=0,j;
  // Stors the id from its parameter
  int *id = (int*)arg;
  while(i<NUM_ITER){
    usleep(rand()%MAX_MOD);
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    sem_getvalue(&full,&semvalfu);
    g_data[semvalfu]=0;
    j=0; printf("(Consumer %d, semaphore full is %d)\t", *id, semvalfu);
    while(j < semvalfu) { j++; printf("="); } printf("\n");
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    i++;
  }

  return 0;

}

