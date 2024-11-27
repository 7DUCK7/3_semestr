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
    FILE* output;

    if((output = fopen("output.txt", "w")) == NULL)
        perror("fopen(output.txt) failed");

    if((full = sem_open("full", O_CREAT, 0777, 0)) < 0)
        perror("sem_open failed");

    if((empty = sem_open("empty", O_CREAT, 0777, 1)) < 0)
        perror("sem_open failed");

    if((fd = shm_open("pccat", O_CREAT | O_RDWR, 0777)) < 0)
        perror("shm_open(pccat) failed");

    if(ftruncate(fd, BUF_SIZE + sizeof(long int)) < 0)
        perror("setting buf_size failed");

    char* buf = (char*) mmap(NULL, BUF_SIZE + sizeof(long int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    *((long int*)(buf + BUF_SIZE)) = 1;
    while(*((long int*)(buf + BUF_SIZE)) > 0)
    {  
        if((sem_wait(full)) < 0)
            perror("sem_wait failed");

        fwrite(buf, 1, *((long int*)(buf + BUF_SIZE)), output);

        if((sem_post(empty)) < 0)
            perror("sem_post failed");
    }

    if((sem_unlink("full")) < 0)
        perror("sem_unlink(full) failed");
    if((sem_unlink("empty")) < 0)
        perror("sem_unlink(empty) failed");
    if((munmap(buf, BUF_SIZE + sizeof(long int))) < 0)
        perror("munmap(buf) failed");
    if((shm_unlink("pccat")) < 0)
        perror("shm_unlink(pcat) failed");
    if((close(fd)) < 0)
        perror("close(fd) failed");
    return 0;
}