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
    printf("  - getppid me devuelve: %d\n", getppid());
}

void showFork(int forkResult)
{
    printf("Donde fork me devuelve %d:\n", forkResult);
}

int main(int argc, char *argv[])
{
    srand(time(0));

    int parentToChildPipe[] = {3, 4};
    int childToParentPipe[] = {6, 7};

    printf("Hola, soy PID %d:\n", getpid());

    int quantityElements = 2;
    showPipe(parentToChildPipe, quantityElements, "primer");
    showPipe(childToParentPipe, quantityElements, "segundo");
    printf("\n");

    if (pipe(parentToChildPipe) < 0)
    {
        perror("Error en pipe parent to child");
        exit(-1);
    }

    if (pipe(childToParentPipe) < 0)
    {
        perror("Error en pipe child to parent");
        exit(-1);
    }

    int forkResult = fork();
    if (forkResult == -1)
    {
        perror("Fork error");
        exit(-1);
    }
    
    if (forkResult == 0)
    {
        close(parentToChildPipe[1]);
        close(childToParentPipe[0]);

        showFork(forkResult);
        showPid();

        int receivedValueFromParent = 0;
        read(parentToChildPipe[0], &receivedValueFromParent, sizeof(receivedValueFromParent));
        printf("  - recibo valor %d vía fd=%d\n", receivedValueFromParent, parentToChildPipe[0]);

        printf("  - reenvío valor en fd=%d y termino\n\n", childToParentPipe[1]);
        write(childToParentPipe[1], &receivedValueFromParent, sizeof(receivedValueFromParent));

        close(parentToChildPipe[0]);
        close(childToParentPipe[1]);
    }
    else
    {
        close(parentToChildPipe[0]);
        close(childToParentPipe[1]);

        showFork(forkResult);
        showPid();

        int randomValueToSend = rand();
        printf("  - random me devuelve: %d\n", randomValueToSend);

        printf("  - envío valor %d a través de fd=%d\n\n", randomValueToSend, parentToChildPipe[1]);
        write(parentToChildPipe[1], &randomValueToSend, sizeof(randomValueToSend));

        int receivedValueFromChild = 0;
        read(childToParentPipe[0], &receivedValueFromChild, sizeof(receivedValueFromChild));
        printf("Hola, de nuevo PID %d:\n", getpid());
        printf("  - recibí valor %d vía fd=%d\n", receivedValueFromChild, childToParentPipe[0]);

        close(parentToChildPipe[1]);
        close(childToParentPipe[0]);
    }
}
