#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <pthread.h> /* pthread_t pthread_create pthread_join */

/*Svar paa lab oppgave 4.5.3. Threadsene teller begge to paa samme variabel og
dette gjoer at man kun oppdaterer variabelen fra 0 -> 30 en gang i motsetning
til oppgave 4.5.2 hvor man telte fra 0 og til 30 to ganger. Dette er grunnet
join delen av oppgaven som bidrar til at traadene jobber sammen og bruker samme
variabel naar de oeker g_ant med 1*/

int g_ant = 0; /* global declaration */
void *writeloop(void *arg) {
  long i = 0;
  while (g_ant < 30) {
    if (++i % 1000000 == 0)
      printf("%s: %d\n", (char*) arg, ++g_ant);
  }
  exit(0);
}

int main(void)
{
  pthread_t tid;
  pthread_create(&tid, NULL, writeloop, "2nd thread");
  writeloop("1st thread");
  pthread_join(tid, NULL);
  return 0;
}
