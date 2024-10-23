#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    pid_t main_process_id = getpid();
    printf("(main)self pid = %ld, parent pid = %ld\n", (long int)(getpid()), (long int)(getppid()));

    for(int i = 0; i < 5; i++)
    {
       
        if(getpid() == main_process_id)
        {   
            fork(); 
        }
        if(getpid() != main_process_id)
        {
            printf("(%d)self pid = %ld, parent pid = %ld\n", i + 1, (long int)(getpid()), (long int)(getppid()));
            exit(1);
        }
    }


    return 0;
}