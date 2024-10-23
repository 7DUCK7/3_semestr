#include <stdio.h>

int main(int argc, char* argv[])
{
    char str[20] = "";
    scanf("%s", str);
    if(*argv[1] == 'b')
        printf("%s bebeb dobavilos' resko\n", str);
    else
        printf("no flag\n");
    return 0;
}
