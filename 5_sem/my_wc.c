#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#define BUF_SIZE 10 
#define STORAGE_SIZE 10000

int lines_count(char *buf, int buf_size);
int words_count(char *buf, int buf_size);

int main(int argc, char* argv[])
{
    struct timespec t1 = {};
    struct timespec t2 = {};
    int fd[2];

    clock_gettime(CLOCK_MONOTONIC, &t1);

    if(pipe(fd) < 0)
        perror("pipe failed");

    pid_t p = fork();
        
    if(p == 0)
    {
        if(close(1) < 0)
            perror("close(1) failed");
        if(dup(fd[1]) < 0)
            perror("dup failed");
        if(close(fd[1]) < 0)
            perror("close(fd[1]) failed");
        if(execvp(argv[1], argv) < 0)
            perror("execvp failed");
    }

    int s;
    wait(&s);

    clock_gettime(CLOCK_MONOTONIC, &t2);

    char buf[BUF_SIZE];
    char input_storage[STORAGE_SIZE];
    int bytes_read = 0, bytes = 0;


    while(1)
    {
        if((bytes_read = read(fd[0], buf, BUF_SIZE)) < 0)
        {
            perror("read failed");
            break;
        }
        //printf("I'm in cycle while(1); bytes_read = %d\n", bytes_read);
        memcpy(&input_storage[bytes], buf, bytes_read);
        bytes += bytes_read;
        if(bytes_read < BUF_SIZE)
            break;
    }

    //printf("I'm still alive\n");

    input_storage[bytes] = 0;
    int words = words_count(input_storage, bytes); 
    int lines = lines_count(input_storage, bytes);


    printf("program lasted for %lg miliseconds\n", (double)(t2.tv_sec - t1.tv_sec) * 1000 + (double)(t2.tv_nsec - t1.tv_nsec) / 1000000);
    printf("\t%d\t%d\t%d\n", lines, words, bytes);


    return 0;
}

int words_count(char *buf, int buf_size)
{
    int counter = 0;
    for(int i = 0; i < buf_size; i++)
    {        
        if(buf[i] == ' ' || buf[i] == '\n')
        {
            counter++;
            while(buf[i] == ' ' || buf[i] == '\n')
                i++;
        }
    }
    if (buf[buf_size - 1] == '\n')
       return counter; 
    return counter + 1;
}

int lines_count(char *buf, int buf_size)
{
    int counter = 0;
    for(int i = 0; i < buf_size; i++)
        if(buf[i] == '\n')
            counter++;
    return counter;
}


