#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void ls_dir(char *path);

int main(int argc, char* argv[])
{

    if(argc != 2)
    {
        printf("no arguments or more than 1 argument written (requiers 1 argument\n)");
        return -1;
    }
    ls_dir(argv[1]);

     
    return 0;
}

void ls_dir(char *path)
{
    DIR *dp = opendir(path);
    if(dp == NULL)
    {
        return;
    }
    struct dirent *e;

    while((e = readdir(dp)) != NULL)
    {
        printf("i_node number = %ld\tfile_type = %d\tfile_name = %s\n", (long int)(e->d_ino), e->d_type, e->d_name);
        char *next_path = strcat(strcat(path, "/"), e->d_name);
        if(strcmp(e->d_name, "." ) != 0 && strcmp(e->d_name, "..") != 0)
        {
            ls_dir(next_path);
        }
    }


    closedir(dp);
    return;
}
