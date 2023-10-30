# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>

void permisiuniUser(struct stat stat)
{
    if(stat.st_mode & S_IRUSR)
        printf("R");
    else
        printf("-");
    if(stat.st_mode & S_IWUSR)
            printf("W");
    else
        printf("-");
    if(stat.st_mode & S_IXUSR)
        printf("X");
    else
       printf("-");
    printf("\n");
}

void permisiuniGrup(struct stat stat)
{
    if(stat.st_mode & S_IRGRP)
        printf("R");
    else
        printf("-");
    if(stat.st_mode & S_IWGRP)
        printf("W");
    else
        printf("-");
    if(stat.st_mode & S_IXGRP)
        printf("X");
    else
        printf("-");
    printf("\n");
}

void permisiuniAltii(struct stat stat)
{
    if(stat.st_mode & S_IROTH)
        printf("R");
    else
        printf("-");
    if(stat.st_mode & S_IWOTH)
        printf("W");
    else
        printf("-");
    if(stat.st_mode & S_IXOTH)
        printf("X");
    else
        printf("-");
    printf("\n");
}

int main(int argc, char**argv)
{   
    struct stat s;
    if(argc != 2)
    {
        printf("Usage ./program %s\n", argv[1]);
        exit(-1);
    }
    stat(argv[1],&s);
    if(S_ISREG(s.st_mode))
    {
        permisiuniUser(s);
        permisiuniGrup(s);
        permisiuniAltii(s);

    }
    return 0;
}