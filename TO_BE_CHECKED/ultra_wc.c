#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <poll.h>

#define BUF_SIZE 10 
#define STORAGE_SIZE 10000

int lines_count(char *buf, int buf_size);
int words_count(char *buf, int buf_size);

int main(int argc, char* argv[])
{
    struct timespec t1 = {};
    struct timespec t2 = {};
    int fd[2], fd_err[2];
    int num_of_open_fds = 2;
    
    clock_gettime(CLOCK_MONOTONIC, &t1);

    if(pipe(fd) < 0)
        perror("pipe failed");

    if(pipe(fd_err) < 0)
        perror("pipe failed");

    struct pollfd fd_array[] = {{fd[0], POLLIN, 0}, {fd_err[0], POLLIN, 0}};

    pid_t p = fork();
        
    if(p == 0)
    {
        if(close(1) < 0)
            perror("close(1) failed");
        if(dup(fd[1]) < 0)
            perror("dup failed");
        if(close(fd[1]) < 0)
            perror("close(fd[1]) failed");

        if(close(2) < 0)
            perror("close(1) failed");
        if(dup(fd_err[1]) < 0)
            perror("dup failed");
        if(close(fd_err[1]) < 0)
            perror("close(fd[1]) failed");

        if(execvp(argv[1], argv + 1) < 0)
            perror("execvp failed");
    }

    clock_gettime(CLOCK_MONOTONIC, &t2);

    char buf_in[BUF_SIZE];
    char buf_err[BUF_SIZE];
    char input_storage[STORAGE_SIZE];
    char err_storage[STORAGE_SIZE];
    int bytes_read_in = 0, bytes_in = 0;
    int bytes_read_err = 0, bytes_err = 0;


    while(num_of_open_fds > 0)
    {
        //printf("i'm alive %d\n", __LINE__);
        int ready = poll(fd_array, num_of_open_fds, -1);
        //printf("i'm alive %d\n", __LINE__);
        if(ready < 0)
        {
            perror("poll errror");
            //printf("i'm alive %d\n", __LINE__);
            break;
        }

        //printf("i'm alive %d\n", __LINE__);

        if(fd_array[0].revents & POLLIN)
        {
            if((bytes_read_in = read(fd_array[0].fd, buf_in, BUF_SIZE)) == 0)
                //printf("gg no input\n");
            memcpy(&input_storage[bytes_in], buf_in, bytes_read_in);
            bytes_in += bytes_read_in;
            //printf("i'm alive %d\n", __LINE__);
            if(bytes_read_in < BUF_SIZE)
                break;  
        }
        else
        {
            //printf("i'm alive %d\n", __LINE__);
            num_of_open_fds--;
            close(fd[0]);
        }
        
        if(fd_array[1].revents & POLLIN)
        {
            if((bytes_read_err = read(fd_array[1].fd, buf_err, BUF_SIZE)) > 0)
                memcpy(&err_storage[bytes_err], buf_err, bytes_read_err);
            bytes_err += bytes_read_err;
            //printf("i'm alive %d\n", __LINE__);
            if(bytes_read_err < BUF_SIZE)
                break;  
        }    
        else
        {
            //printf("i'm alive %d\n", __LINE__);
            num_of_open_fds--;
            close(fd_err[0]);   
        } 
    }
    
    wait(NULL);

    input_storage[bytes_in] = 0;
    int words_in = words_count(input_storage, bytes_in); 
    int lines_in = lines_count(input_storage, bytes_in);

    err_storage[bytes_err] = 0;
    int words_err = words_count(err_storage, bytes_err); 
    int lines_err = lines_count(err_storage, bytes_err);

    printf("program lasted for %lg miliseconds\n", (double)(t2.tv_sec - t1.tv_sec) * 1000 + (double)(t2.tv_nsec - t1.tv_nsec) / 1000000);
    printf("input:\t%d\t%d\t%d\n", lines_in, words_in, bytes_in);
    printf("error:\t%d\t%d\t%d\n", lines_err, words_err, bytes_err);

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

    if (buf_size == 0)
       return 0;
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