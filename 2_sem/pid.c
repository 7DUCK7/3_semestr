#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    long int current_process_id = (long int)getpid();
    long int parent_process_id = (long int)getppid();

    printf("current process id = %ld\n", current_process_id);
    printf("parent process id = %ld\n", parent_process_id);




    return 0;
}
