#include <fcntl.h>          
#include <sys/stat.h>        
#include <semaphore.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define BUF_SIZE 4096

int main()
{
    int fd;
    
    sem_t *full;
    sem_t *empty;
    FILE* input;

    if((input = fopen("input.txt", "r")) == NULL)
        perror("fopen(input.txt) failed");

    if((full = sem_open("full", O_CREAT, 0777, 0)) < 0)
        perror("sem_open(full) failed");

    if((empty = sem_open("empty", O_CREAT, 0777, 1)) < 0)
        perror("sem_open(empty) failed");

    if((fd = shm_open("pccat", O_CREAT | O_RDWR, 0777)) < 0)
        perror("shm_open(pccat) failed");

    if(ftruncate(fd, BUF_SIZE + sizeof(long int)) < 0)
        perror("setting buf_size failed");

    char* buf = (char*) mmap(NULL, BUF_SIZE + sizeof(long int), PROT_WRITE, MAP_SHARED, fd, 0);
    
    *((long int*)(buf + BUF_SIZE)) = 1;
    while(*((long int*)(buf + BUF_SIZE)) > 0)
    {
        if((sem_wait(empty)) < 0)
            perror("sem_wait(empty) failed");
        for(int i = 0; i < BUF_SIZE; i++)
            buf[i] = 0;
        
        *((long int*)(buf + BUF_SIZE)) = (fread(buf, 1, BUF_SIZE, input));

        if((sem_post(full)) < 0)
            perror("sem_post(full) failed");
    }

    return 0;
}