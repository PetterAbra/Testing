#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* fork */
#include <sys/wait.h> /* waitpid */
#include <sys/types.h> /* pid_t */

/* Svar på Lab oppgave 4.5.2: både child og parent har samme globale variable
men denne er forskjellig fra child til parent, så child teller g_ant fra 0->30
og parent teller g_ant fra 0 -> 30. tallet øker hver gang i blir hundre tusen,
så g_ant økes hver hundretusende runde i while loopen. Dette gjør at når 
datamaskinen switcher mellom trådene så vil ikke den første tråden være ferdig
enda med å telle. Det vil finnes en g_ant for hver tråd, så g_ant teller to 
ganger til 30, en for hver tråd.*/


int g_ant = 0; /* global declaration */
void writeloop(char *text) {
  long i = 0;
  while (g_ant < 30) {
    /* print and increment g_ant only each 100000 iteration */

    if (++i % 100000 == 0) /* % is the same as modulo */
      printf("%s: %d\n", text, ++g_ant);
  }
}
/* Note: the following code does not do proper error checking
of return values from functions, so this is not robust code,
but coded this way to be easy to read */
int main(void)
{
  pid_t pid;
  pid = fork();
  if (pid == 0) { /* child */
    writeloop("Child");
    exit(0);
  }
  writeloop("Parent"); /* parent */
  waitpid(pid, NULL, 0);
  return 0;
}

