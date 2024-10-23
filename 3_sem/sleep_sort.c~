#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int s = 0;
    for(int i = 1; i < argc; i++)
    {

        pid_t p = fork();


        if(p == 0)
        {
            usleep(atoi(argv[i]) * 1000);
            printf("%d\n", atoi(argv[i]));
            exit(0);
        }   
        
    }
    
    for(int i = 1; i < argc; i++)
        wait(&s);



    return 0;
}
