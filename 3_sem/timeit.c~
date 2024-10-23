#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[])
{
    struct timespec t1 = {};
    struct timespec t2 = {};
    
    clock_gettime(CLOCK_MONOTONIC, &t1);

    pid_t p = fork();
        
    if(p == 0)
    {
        execvp(argv[1], argv + 1);
    }

    int s;
    wait(&s);

    clock_gettime(CLOCK_MONOTONIC, &t2);
    printf("program lasted for %lg miliseconds\n", (double)(t2.tv_sec - t1.tv_sec) * 1000 + (double)(t2.tv_nsec - t1.tv_nsec) / 1000000);


    return 0;
}
