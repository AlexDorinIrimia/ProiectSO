# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

int main(int argc, char**argv)
{
    if(argc != 2)
    {
        printf("Eroare!");
        exit(-1);
    }
    if(open())
    return 0;
}