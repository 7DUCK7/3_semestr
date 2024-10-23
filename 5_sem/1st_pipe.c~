#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 30

int main()
{
    int fd[2];
    char *message = "Hello, world!";
    pid_t p_pid;
    pid_t c_pid;

    if(pipe(fd) < 0)
        perror("pipe failed");
    pid_t p = fork();

    if(p == 0)
    {
        p_pid = getppid();
        c_pid = getpid();
        sleep(2);
        printf("This child process has pid %ld, with parent's pid %ld\n", (long int)c_pid, (long int) p_pid);
        int s = write(fd[1], message, 13);
    }
    else
    {
        char receiving_buf[BUF_SIZE];
        int s = read(fd[0], receiving_buf, strlen(message) + 1);
        printf("received message (receiving process pid = %d): %s\n", (int)(getpid()), receiving_buf);
    }

    return 0;
}
