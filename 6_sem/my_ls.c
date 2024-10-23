#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_PATH_SIZE 1024

void ls_dir(char *path);
void get_rights(mode_t mode, char *buf);
char * get_user_name(uid_t user_id);
char * get_group_name(gid_t group_id);
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
    struct stat statbuf;
    char path_buf[MAX_PATH_SIZE] = "";
    while((e = readdir(dp)) != NULL)
    {
        sprintf(path_buf, "%s/%s", path, e->d_name);
        if(stat(path_buf, &statbuf) < 0)
            perror("stat failed");    
        char buf[10]; 
        get_rights(statbuf.st_mode, buf);
        
        printf("\t%ld\t%s\t%ld\t%s\t%s\t%ld\t%s\t%s", (long int)(e->d_ino), buf, statbuf.st_nlink, get_user_name(statbuf.st_uid),
                get_group_name(statbuf.st_gid), statbuf.st_size, e->d_name, ctime(&statbuf.st_mtim.tv_sec));
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
                strcat(strcat(path_copy, e->d_name), "/");
                printf("%s:\n", path_copy);
                ls_dir(path_copy);
            }
        }
    }
    

    closedir(dp);
    return;
}
void get_rights(mode_t mode, char *buf)
{
    char perm_str[] = "-rwxrwxrwx";
    for(int i = 0; i < 9; i++)
    {
        buf[i+1] = (mode & (1 << (8 - i))) ? perm_str[i+1] : '-';
    }
    if(S_ISREG(mode) == 1)
        buf[0] = '-';

    if(S_ISDIR(mode) == 1)
        buf[0] = 'd';

    if(S_ISFIFO(mode) == 1)
        buf[0] = 'p';

    if(S_ISLNK(mode) == 1)
        buf[0] = 'l';

    if(S_ISCHR(mode) == 1)
        buf[0] = 'c';
    
    if(S_ISBLK(mode) == 1)
        buf[0] = 'b';

    if(S_ISSOCK(mode) == 1)
        buf[0] = 's';

    buf[10] = 0;
    return;
}

char * get_user_name(uid_t user_id)
{
    struct passwd *passwd_buf;
    passwd_buf = getpwuid(user_id);
    return passwd_buf->pw_name;
}

char * get_group_name(gid_t group_id)
{
    struct group *passwd_buf;
    passwd_buf = getgrgid(group_id);
    return passwd_buf->gr_name;
}

void print_modification_data(time_t time)
{
    


    return;
}

