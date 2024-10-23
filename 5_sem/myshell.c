#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
void my_parser(char *buf, char**** table_of_programs, int* number_of_programs);
int get_number_of_programs(char *buf);
int get_argc(char *buf);


int main()
{
    printf("To exit myshell write leave\n");
    while(1) 
    {
        printf("$ ");
        size_t len = 1;
        int bytes_read;

        int number_of_programs = 0;
        char*** program_table = NULL;


        char *buf = (char*)malloc(len); 
        if((bytes_read = getline(&buf, &len, stdin)) < 0)
            perror("getline failed");

        my_parser(buf, &program_table, &number_of_programs);       
/*
        for(int i = 0; i < number_of_programs; i++)
        {
            int j = 0;
            while((program_table[i])[j] != NULL)
            {
                printf("%s ", (program_table[i])[j]);
                j++;
            }
            printf("\n");
        }
*/
        int s;

        switch(number_of_programs){
            case 1:
                pid_t p = fork();
                if(p == 0)
                {
                    if(execvp((program_table[0])[0], &((program_table[0])[0])) < 0)
                        perror("single program execvp failed");
                }
                wait(&s);
                break;
            default:
                if(number_of_programs <= 0)
                {
                    printf("no arguments were read\n");
                    return -1;
                }
                pid_t writer = fork(); 
                for(int i = 0; i < number_of_programs - 1; i++)
                {
                    if(writer != 0)
                        int fd[2]; 
                    if(writer == 0)
                    {
                        if(pipe(fd) < 0)
                            perror("pipe failed");
                        if(close(1) < 0)
                            perror("close(1) failed");
                        if(dup(fd[1]) < 0)
                            perror("dup failed");
                        if(close(fd[1]) < 0)
                            perror("close(fd[1]) failed");
                        if(execvp((program_table[i])[0], &((program_table[i])[0])) < 0)
                            perror("multiple execvp failed");
                    }
                    
                    pid_t reader = fork();

                    if(reader == 0)
                    {
                        if(close(0) < 0)
                            perror("close(0) failed");
                        if(dup(fd[0]) < 0)
                            perror("dup failed");
                        if(close(fd[0]) < 0)
                            perror("close(fd[0]) failed");
                        writer = getpid();
                    }
                }
                if(writer == 0)
                {
                    if(execvp((program_table[number_of_programs - 1])[0], &((program_table[number_of_programs - 1])[0])) < 0)
                        perror("multiple execvp failed");
                }

                for(int i = 0; i < number_of_programs; i++)
                    wait(&s); 
                break;

        }
        
        free(buf);
    }

    return 0;
}
void my_parser(char *buf, char**** table_of_programs, int* number_of_programs)
{
    int n_of_programs = get_number_of_programs(buf);
    char*** program_table = (char***)calloc(n_of_programs, sizeof(char*));
    //int* program_argc_table = (int*)calloc(n_of_progrmas, sizeof(int));
    char* current_ptr_position = buf;

    for(int i = 0; i < n_of_programs; i++)
    {
        int j = 0;
        if(current_ptr_position == NULL)
            perror("current_ptr_postion became a NULL");
        int current_argc = get_argc(current_ptr_position);
        program_table[i] = (char**)calloc(current_argc + 1, sizeof(char*));
        while(1) 
        {
            current_ptr_position = strtok(buf, " \n");
            if(current_ptr_position == NULL)
                break;
            if(strcmp(current_ptr_position, "|") == 0)
                break;

            if(program_table == NULL)
                perror("program_table = NULL");
            if(program_table[i] == NULL)
                perror("program_table = NULL");
            (program_table[i])[j] = current_ptr_position;
            j++;
            buf = NULL;
        }
        current_ptr_position += 2;
    }
    *number_of_programs = n_of_programs;
    *table_of_programs = program_table;
    return;
}

int get_number_of_programs(char *buf)
{
    int counter = 0;
    int i = 0;
    while(buf[i] != 0)
    {
        if(buf[i] == '|')
        {
            i++;
            counter++;
        }
        else
            i++;
    }
    return counter + 1;
}

int get_argc(char *buf)
{
    int counter = 0;
    int i = 0;
    while(buf[i] != '|')
    {
        if(buf[i] == 0)
        {
            counter++;
            break;
        }

        if(buf[i] == ' ')
        {
            counter++;
            while(buf[i] == ' ')
                i++;
        }
        else
            i++;
    }
    return counter;
}

