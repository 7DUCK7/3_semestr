#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>

int main(int argc, char* argv[])
{
    long int current_uid = 0; 
    char *uid_name = NULL;
    long int current_gid = 0;
    char *gid_name = NULL;
    gid_t list_of_groups[32] = {};   
    int number_of_groups = 32;
    struct passwd *current_passwd = NULL;
    if(argc == 1)
    {
        current_uid = (long int)getuid();
        uid_name = getpwuid(current_uid)->pw_name;
        current_gid = (long int)getgid();
        gid_name = getgrgid(current_gid)->gr_name;
       
        number_of_groups = getgroups(32, list_of_groups);

    }
    else
    {
        uid_name = argv[1];
        if((current_passwd = getpwnam(argv[1])) == NULL)
        {
            printf("Couldn't find this user\n");
            return 0;
        }
    
        current_uid = current_passwd->pw_uid;
        current_gid = current_passwd->pw_gid;
        gid_name = getgrgid(current_gid)->gr_name;
        getgrouplist(uid_name, current_gid, list_of_groups, &number_of_groups);
    }
    
    printf("uid=%ld(%s) ", current_uid, uid_name);
    printf("gid=%ld(%s) ", current_gid, gid_name);
    printf("группы=");
    for(int i = number_of_groups - 1; i >= 0; i--)
    {
        gid_name = getgrgid(list_of_groups[i])->gr_name;
        printf("%ld(%s)", (long int)list_of_groups[i], gid_name);
        if(i != 0)
            printf(",");
    }
    printf("\n");

    return 0;
}
