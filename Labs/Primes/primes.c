#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

int getNumberToCalculatePrimes(char *number)
{
    char *p;
    int receivedNumber;

    errno = 0;
    long convertedNumber = strtol(number, &p, 10);

    // Check for errors: e.g., the string does not represent an integer or the integer is larger than int
    if (errno != 0 || *p != '\0' || convertedNumber > INT_MAX || convertedNumber < INT_MIN)
    {
        printf("Error: Invalid number\n");
        exit(-1);
    }

    return convertedNumber;
}

int main(int argc, char *argv[])
{

    int receivedNumber = getNumberToCalculatePrimes(argv[1]);

    printf("The received number is %d\n", receivedNumber);

    //   int i = fork();

    //   if (i == 0) {
    //     printf("Soy el proceso hijo y mi pid es: %d\n", getpid());
    //     sleep(2);
    //     printf("Proceso hijo termina (%d)\n", getpid());
    //     exit(17);
    //   } else {
    //     printf("Soy el proceso padre y mi pid es: %d\n", getpid());

    //     int wstatus;
    //     int ret = wait(&wstatus);
    //     if (WIFEXITED(wstatus)) {
    //       printf("PID %d terminócon %d\n", ret, WEXITSTATUS(wstatus));
    //     } else if (WIFSIGNALED(wstatus)) {
    //     	printf("PID %d fue terminado con %d\n", ret, WTERMSIG(wstatus));
    //     }

    //     printf("Proceso padre termina (%d)\n", getpid());
    //   }
}