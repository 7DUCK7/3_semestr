#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void my_parser(char *buf, char**** table_of_programs, int* number_of_programs);
int get_number_of_programs(char *buf);
int get_argc(char *buf);

int main()
{
    int number_of_programs = 0;
    char*** program_table = NULL;
    char test_str[] = "echo hello world | wc | hi";
    printf("str %d is alright and number of programs = %d\n", __LINE__, get_number_of_programs(test_str));        

    printf("str %d is alright\n", __LINE__);

    my_parser(test_str, &program_table, &number_of_programs);

    printf("str %d is alright\n", __LINE__);

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

    return 0;
}

void my_parser(char *buf, char**** table_of_programs, int* number_of_programs)
{
    int n_of_programs = get_number_of_programs(buf);
    printf("str %d is alright and number of programs = %d\n", __LINE__, n_of_programs);
    char*** program_table = (char***)calloc(n_of_programs, sizeof(char*));
    //int* program_argc_table = (int*)calloc(n_of_progrmas, sizeof(int));
    char* current_ptr_position = buf;

    for(int i = 0; i < n_of_programs; i++)
    {
        int counter = 0;
        int j = 0;
        if(current_ptr_position == NULL)
            perror("current_ptr_postion became a NULL");
        printf("str %d is alright\n", __LINE__);
        int current_argc = get_argc(current_ptr_position);
        program_table[i] = (char**)calloc(current_argc + 1, sizeof(char*));
        printf("str %d is alright\n", __LINE__);
        while(1) 
        {
            current_ptr_position = strtok(buf, " ");
                    printf("str %d is alright\n", __LINE__);
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

