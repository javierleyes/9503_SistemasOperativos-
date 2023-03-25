#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

// run test.
// ./test-fork -c pingpong -v /home/javi/FISOP/9503_SistemasOperativos-/Labs/PingPong/

void showPipe(int pipe[], char numberPipe[])
{
    printf("- %s pipe me devuelve: [%d, %d]\n", numberPipe, pipe[0], pipe[1]);
}

void showPids()
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

    int parentToChildPipe[2];
    int childToParentPipe[2];

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

    printf("Hola, soy PID %d:\n", getpid());
    showPipe(parentToChildPipe, "primer");
    showPipe(childToParentPipe, "segundo");
    printf("\n");

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
        showPids();

        int receivedValueFromParent = 0;
        read(parentToChildPipe[0], &receivedValueFromParent, sizeof(receivedValueFromParent));
        printf("  - recibo valor %d vía fd=%d\n", receivedValueFromParent, parentToChildPipe[0]);

        write(childToParentPipe[1], &receivedValueFromParent, sizeof(receivedValueFromParent));
        printf("  - reenvío valor en fd=%d y termino\n\n", childToParentPipe[1]);

        close(parentToChildPipe[0]);
        close(childToParentPipe[1]);

        exit(0);
    }
    else
    {
        close(parentToChildPipe[0]);
        close(childToParentPipe[1]);

        showFork(forkResult);
        showPids();

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

        wait(NULL);
    }
}
