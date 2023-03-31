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

    if (errno != 0 || *p != '\0' || convertedNumber > INT_MAX || convertedNumber < INT_MIN)
    {
        printf("Error: Invalid number\n");
        exit(-1);
    }

    return convertedNumber;
}

void calculatePrimes(int parentToChildPipe)
{
    int primeNumber = 0;
	if (read(parentToChildPipe, &primeNumber, sizeof(int)) > 0) {
		printf("primo %d\n", primeNumber);
	} else {
		return;
	}

    int childToChildPipe[2];
    if (pipe(childToChildPipe) < 0)
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

    if (forkResult != 0) 
    {
        close(childToChildPipe[0]);

        int candidate = 0;
        while (read(parentToChildPipe, &candidate, sizeof(int)) > 0)
        {
            if (candidate % primeNumber != 0) 
            {
				write(childToChildPipe[1], &candidate, sizeof(int));
			}
        }

        close(parentToChildPipe);
        close(childToChildPipe[1]);

        wait(NULL);
    }
    else
    {
        close(parentToChildPipe);
        close(childToChildPipe[1]);
        calculatePrimes(childToChildPipe[0]);
    }
}

int main(int argc, char *argv[])
{
    int receivedNumber = getNumberToCalculatePrimes(argv[1]);

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

    if (forkResult != 0) 
    {
        close(parentToChildPipe[0]);

        for (int candidate = 2; candidate < receivedNumber; candidate++)
        {
            write(parentToChildPipe[1], &candidate, sizeof(int));
        }

        close(parentToChildPipe[1]);
        wait(NULL);
    } 
    else 
    {
        close(parentToChildPipe[1]);
        calculatePrimes(parentToChildPipe[0]);
        close(parentToChildPipe[0]);

        exit(0);
    }
    
    exit(0);
}