#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MY_BUF_SIZE 10

void stream_copy(int in_stream, int out_stream);

int main(int argc, char *argv[])
{
    if (argc == 1)
        stream_copy(0, 1);    
    else
    {
        for(int i = 1; i < argc; i++)
        {
            int input = open(argv[i], O_RDONLY);
            if(input < 0)
            {
               perror("Нет такого файла или каталога");
               return -1;
            } 
            stream_copy(input, 1);
            if(close(input) < 0)
            {
                perror("file closing failed");
                return -2;
            }
        } 
    }

    return 0;
}

void stream_copy(int in_stream, int out_stream)
{
    char buf[MY_BUF_SIZE] = "";
    size_t buf_size = MY_BUF_SIZE;
    ssize_t bytes_read = 0;
    ssize_t bytes_written = 0;
    
    while(1)
    { 
        if((bytes_read = read(in_stream, buf, buf_size)) < 0)
        {
            perror("read failed");
            break;
        }

        if(bytes_read == 0)
            break;

        while(bytes_read > 0)
        {
            if((bytes_written = (write(out_stream, buf, bytes_read))) < 0)
                perror("write failed");
            bytes_read -= bytes_written; 
        }
    }
    return;
}
