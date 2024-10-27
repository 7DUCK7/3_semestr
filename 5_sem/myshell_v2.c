#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void free_memory(int number_of_programs, char**** table_of_programs);
void pipe_management(int number_of_programs, char*** program_table);
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
        if(strncmp(buf, "leave", 5) == 0)
            return 0;

        my_parser(buf, &program_table, &number_of_programs); 

        pipe_management(number_of_programs, program_table);

        free_memory(number_of_programs, &program_table);
    }

    return 0;
}

void free_memory(int number_of_programs, char**** table_of_programs)
{
    for(int i = 0; i < number_of_programs; i++)
    {
        free((*table_of_programs)[i]);
        (*table_of_programs)[i] = NULL;
    }
    free(*table_of_programs);
    *table_of_programs = NULL;
    return;
}

void pipe_management(int number_of_programs, char*** program_table)
{
    int * fd = calloc(number_of_programs * 2, sizeof(int)); //как бы длинный pipe
    fd[0] = 0;
    fd[number_of_programs * 2 - 1] = 1;
    int pipe_fd[2];
    for(int i = 0; i < number_of_programs; i++)
    {
        if(i < number_of_programs - 1)
        {
            if(pipe(pipe_fd) < 0)
                perror("pipe failed");
            fd[2 * i + 1] = pipe_fd[1];
            fd[2 * i + 2] = pipe_fd[0];
        }
        pid_t p = fork();
        if(p == 0)
        {
            dup2(fd[2 * i], 0);
            dup2(fd[2 * i + 1], 1);       

            if(execvp(program_table[i][0], program_table[i]) < 0)
                perror("couldn't exec program");
        }
        if(i > 0)
            close(fd[2 * i]);
        if(i < number_of_programs - 1)
            close(fd[2 * i + 1]);
  
    }

    for(int i = 0; i < number_of_programs; i++)
        wait(NULL); 
        
    free(fd);
}

void my_parser(char *buf, char**** table_of_programs, int* number_of_programs)
{
    int n_of_programs = get_number_of_programs(buf);
    char*** program_table = (char***)calloc(n_of_programs, sizeof(char*));
    char *current_ptr_position;


    //разделение на программы
    current_ptr_position = strtok(buf, "|\n");
    while(*current_ptr_position == ' ')
            current_ptr_position++;
    program_table[0] = (char**)calloc(get_argc(current_ptr_position) + 1, sizeof(char*));
    program_table[0][0] = current_ptr_position;
    for(int j = 1; j < n_of_programs; j++)
    {
        if((current_ptr_position = strtok(NULL, "|")) == NULL)
            perror("found wrong number of programs");
        while(*current_ptr_position == ' ')
            current_ptr_position++;
        program_table[j] = (char**)calloc(get_argc(current_ptr_position) + 1, sizeof(char*));
        program_table[j][0] = current_ptr_position;
    }

    //разделение строк программ на аргументы командной строки 
    for(int i = 0; i < n_of_programs; i++)
    {
        int j = 0;  // по сути argc для данной программы
        current_ptr_position = strtok(program_table[i][0], " \n");
        j++;

        while(1)
        {
            if((current_ptr_position = strtok(NULL, " \n")) == NULL)
                break;
            program_table[i][j] = current_ptr_position;
            //printf("потоковая печать argv: program_table[%d][%d] == %s\n",i, j, program_table[i][j]);
            j++;
        }
        program_table[i][j] = current_ptr_position;
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

    while(buf[i] != 0)
    {
        while(buf[i] == ' ')
            i++;
        if(buf[i] != ' ' && buf[i] != 0)
        {
            counter++;
            while(buf[i] != ' ' && buf[i] != 0)
                i++;
        }
    }
    
    return counter;
}
