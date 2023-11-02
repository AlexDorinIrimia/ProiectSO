# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <time.h> 

char* userID;
char* dimensiune;
char* links;

void permisiuniUser(struct stat stat)
{
    int descriptor = open("Info.txt",O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(stat.st_mode & S_IRUSR)
        write(descriptor,"R",1);
    else
        write(descriptor,"-",1);
    if(stat.st_mode & S_IWUSR)
        write(descriptor,"W",1);
    else
        write(descriptor,"-",1);
    if(stat.st_mode & S_IXUSR)
        write(descriptor,"X",1);
    else
       write(descriptor,"-",1);
    write(descriptor,"\n",1);
}

void permisiuniGrup(struct stat stat)
{
    int descriptor = open("Info.txt",O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   if(stat.st_mode & S_IRGRP)
        write(descriptor,"R",1);
    else
        write(descriptor,"-",1);
    if(stat.st_mode & S_IWGRP)
        write(descriptor,"W",1);
    else
        write(descriptor,"-",1);
    if(stat.st_mode & S_IXGRP)
        write(descriptor,"X",1);
    else
       write(descriptor,"-",1);
    write(descriptor,"\n",1);
}

void permisiuniAltii(struct stat stat)
{
    int descriptor = open("Info.txt",O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(stat.st_mode & S_IROTH)
        write(descriptor,"R",1);
    else
        write(descriptor,"-",1);
    if(stat.st_mode & S_IWOTH)
        write(descriptor,"W",1);
    else
        write(descriptor,"-",1);
    if(stat.st_mode & S_IXOTH)
        write(descriptor,"X",1);
    else
       write(descriptor,"-",1);
    write(descriptor,"\n",1);
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
        userID = malloc(sizeof(unsigned));
        dimensiune = malloc(sizeof(unsigned));
        links = malloc(sizeof(int));
        int descriptor = open("Info.txt", O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        write(descriptor, "Nume fisier: ", 13);
        write(descriptor,argv[1],strlen(argv[1]) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Inaltime: ", 10);
        write(descriptor,"\n",1);
        write(descriptor, "Lungime: ", 9);
        write(descriptor,"\n",1);
        write(descriptor, "Dimensiune: ", 12);
        sprintf(dimensiune,"%ld", s.st_size);
        write(descriptor, dimensiune, strlen(dimensiune) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Identificatorul utilizatorului: ", 32);
        sprintf(userID,"%d", s.st_uid);
        write(descriptor, userID, strlen(userID) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Timpul ultimei modificari: ", 27);
        write(descriptor,"\n",1);
        write(descriptor, "Numar de legaturi: ", 19);
        sprintf(links,"%ld",s.st_nlink);
        write(descriptor,links,strlen(links) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Drepturi de acces user: ", 24);
        permisiuniUser(s);
        write(descriptor, "Drepturi de acces grup: ", 24);
        permisiuniGrup(s);
        write(descriptor, "Drepturi de acces altii: ", 25);
        permisiuniAltii(s);
        close(descriptor);
    }
    return 0;
}