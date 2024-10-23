#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_PATH_SIZE 1024
#define NUMBER_OF_SUPPORTED_OPTIONS 5 

void ls_dir(char *path);
char** get_args_and_opts(int argc, char *argv[], char* array_of_options);

int main(int argc, char* argv[])
{
    char array_of_options[NUMBER_OF_SUPPORTED_OPTIONS + 1];
    for(int i = 0; i < NUMBER_OF_SUPPORTED_OPTIONS; i++)
        array_of_options[i] = '_';
    if(argc <= 1)
    {
        printf("no arguments has been written\n");
        return -1;
    }

    char **array_of_arguments = get_args_and_opts(argc, argv, array_of_options);
    printf("array of options: %s\n", array_of_options);

    int i = 0;
    while(array_of_arguments[i] != 0)
    {
        printf("%s ", array_of_arguments[i]);
        i++;
    }
    printf("\n");
    
    //ls_dir(argv[1]);


     
    return 0;
}

char** get_args_and_opts(int argc, char *argv[], char* array_of_options)
{
    char *optstring = "liRad";
    struct option longopts[NUMBER_OF_SUPPORTED_OPTIONS + 1] = {   
                                                                    {"long",        0,  0,  'l'}, 
                                                                    {"inode",       0,  0,  'i'}, 
                                                                    {"recursive",   0,  0,  'R'}, 
                                                                    {"all",         0,  0,  'a'},
                                                                    {"directory",   0,  0,  'd'},   
                                                                    {0,             0,  0,  0}
                                                                }; 
    
    int longindex;
    char c;
    while((c = getopt_long(argc, argv, optstring, longopts, &longindex)) >= 0)
    {
        array_of_options[longindex] = c;
    }
    return &(argv[optind]); 


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
    }
    printf("\n");
    closedir(dp);

    dp = opendir(path);
    while((e = readdir(dp)) != NULL)
    {
        char path_copy[MAX_PATH_SIZE];
        if(e->d_type == 4)
        {
            if(strcmp(e->d_name, "." ) != 0 && strcmp(e->d_name, "..") != 0)
            {
                strcpy(path_copy, path);
                strcat(strcat(path_copy, "/"), e->d_name);
                printf("%s:\n", path_copy);
                ls_dir(path_copy);
            }
        }
    }
    

    closedir(dp);
    return;
}


