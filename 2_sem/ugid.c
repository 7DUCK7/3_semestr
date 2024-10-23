#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>

int main()
{
    long int current_uid = (long int)getuid();
    char *uid_name = getpwuid(current_uid)->pw_name;
    long int current_gid = (long int)getgid();
    char *gid_name = getgrgid(current_gid)->gr_name;
    gid_t list_of_groups[32] = {};   
    printf("current_uid = %ld(%s)\n", current_uid, uid_name);
    printf("current_gid = %ld(%s)\n", current_gid, gid_name);
    int number_of_groups = getgroups(32, list_of_groups);

    printf("list of groups:\n");
    for(int i = 0; i < number_of_groups; i++)
    {
        gid_name = getgrgid(list_of_groups[i])->gr_name;
        printf("%ld(%s)\n", (long int)list_of_groups[i], gid_name);
    }




    return 0;
}
