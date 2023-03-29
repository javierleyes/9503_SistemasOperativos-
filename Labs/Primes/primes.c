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
    int numberCandidates = receivedNumber - 1;

    printf("The received number is %d\n", receivedNumber);

    printf("Generating an array with number to check...\n");
   
    printf("Done\n");

    // Parent pipe.
    int parentToChildPipe[2];

    if (pipe(parentToChildPipe) < 0)
    {
        perror("Pipe error");
        exit(-1);
    }

    int forkResult = fork();
    if (forkResult == -1)
    {
        perror("Fork error");
        exit(-1);
    }

    // Parent.
    if (forkResult != 0) 
    {
        close(parentToChildPipe[0]);

        int primesNumberCandidates[numberCandidates];

        int firstPrimesNumber = 2;
        for (int i = 0; i < numberCandidates; i++)
        {
            primesNumberCandidates[i] = firstPrimesNumber++;
        }

        write(parentToChildPipe[1], &primesNumberCandidates, sizeof(primesNumberCandidates));

        close(parentToChildPipe[1]);

        wait(NULL);
    } 
    else 
    {
        close(parentToChildPipe[1]);

        int primesNumber[numberCandidates];
        read(parentToChildPipe[0], &primesNumber, sizeof(primesNumber));

        int primeNumber = primesNumber[0];

        printf("Primo %d\n", primeNumber);
        int* nextfilter = malloc(1 * sizeof(int));
        int nextFilterIndex = 0;

        for (int i = 1; i < numberCandidates; i++)
        {
            int moduloDivision = primesNumber[i] % primeNumber;

            if (moduloDivision != 0)
            {
                if (nextFilterIndex != 0)
                {
                    nextfilter = realloc(nextfilter, 1 * sizeof(int));
                }

                printf("Send %d\n", primesNumber[i]);
                nextfilter[nextFilterIndex] = primesNumber[i];
                nextFilterIndex++;
            }
        }

        // printf("Next filter:\n");
        // for (int i = 0; i < nextFilterIndex; i++)
        // {
        //     printf("%d, \n", nextfilter[i]);
        // }

        free(nextfilter);
        
        close(parentToChildPipe[0]);

        exit(0);
    }
    
    
    printf("\nEnd\n");

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