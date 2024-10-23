#include <stdio.h>
#include <string.h>

//void print_in_a_row(int a, int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("\n");
        return 0;
    }

    if(strcmp(argv[1], "-n") != 0)
    {
        if(argc >= 2)
        {    
            for(int i = 1; i < argc; i++)
            {
                printf("%s", argv[i]);

                if(i + 1 < argc)
                {
                    printf(" ");
                }
        
            }
            printf("\n");
        }
        else
        {
            printf("\n");
        }
    }
    else
    {
        for(int i = 2; i < argc; i++)
        {

            printf("%s", argv[i]);

            if(i + 1 < argc)
            {
                printf(" ");
            }

        }
    }

    return 0;
}
/*
void print_in_a_row(int a, int argc, char *argv[])
{
    for(int i = a; i < argc; i++)
    {
        printf("%s", argv);

    }
    return 0;
}
*/
