#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void showPipe(int pipe[], int quantityElements, char numberPipe[])
{
    printf("  - %s pipe me devuelve: [", numberPipe);

    for (int i = 0; i < quantityElements; i++)
    {
        if ((i - (quantityElements - 1)) == 0)
        {
            printf("%d]\n", pipe[i]);
        }
        else
        {
            printf("%d, ", pipe[i]);
        }
    }
}

void showPid()
{
    printf("  - getpid me devuelve: %d\n", getpid());
    printf("  - getppid me devuelve: %d\n", getpid());
}

void showFork(int forkResult)
{
    printf("Donde fork me devuelve %d:\n", forkResult);
}

int main(int argc, char *argv[])
{
    srand(time(0));

    int firstPipe[] = {3, 4};
    int secondPipe[] = {6, 7};

    printf("Hola, soy PID %d:\n", getpid());

    int quantityElements = 2;
    showPipe(firstPipe, quantityElements, "primer");
    showPipe(secondPipe, quantityElements, "segundo");

    printf("\n");

    int parentToChildPipe[2];
    int parentoToChildResultPipe = pipe(parentToChildPipe);
    if (parentoToChildResultPipe < 0)
    {
        perror("Error en pipe");
        exit(-1);
    }

    int childToParentPipe[2];
    int childToParentResultPipe = pipe(childToParentPipe);
    if (childToParentResultPipe < 0)
    {
        perror("Error en pipe");
        exit(-1);
    }

    int forkResult = fork();

    if (forkResult == 0)
    {
        showFork(forkResult);
        showPid();

        int receivedValue = 0;
        read(parentToChildPipe[0], &receivedValue, sizeof(receivedValue));
        printf("  - recibo valor %d vía fd=%d\n", receivedValue, parentToChildPipe[0]);

        printf("  - reenvío valor en fd=%d y termino\n\n", childToParentPipe[1]);
        write(childToParentPipe[1], &receivedValue, sizeof(receivedValue));
    }
    else
    {
        showFork(forkResult);

        showPid();

        int randomValue = rand();
        printf("  - random me devuelve: %d\n", randomValue);

        printf("  - envío valor %d a través de fd=%d\n\n", randomValue, parentToChildPipe[1]);
        write(parentToChildPipe[1], &randomValue, sizeof(randomValue));

        int receivedValue = 0;
        read(childToParentPipe[0], &receivedValue, sizeof(receivedValue));
        printf("Hola, de nuevo PID %d:\n", getpid());
        printf("  - recibí valor %d vía fd=%d\n", receivedValue, childToParentPipe[1]);

    }

    close(parentToChildPipe[0]);
    close(parentToChildPipe[1]);
    close(childToParentPipe[0]);
    close(childToParentPipe[1]);
}