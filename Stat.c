# define _XOPEN_SOURCE
# define _GNU_SOURCE
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <time.h>
# include <dirent.h>
# include <libgen.h>
# include <sys/wait.h>
# include "Stat.h" 

char* userID;
char* dimensiune;
char* links;
char* modificare;
char* inaltime;
char* latime;
char *newpath;
char *path;
char *name;
int nr_linii,coddescriere,codinfo;

void permisiuniUser(struct stat stat, int descriptor)
{
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

void permisiuniGrup(struct stat stat,int descriptor)
{
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

void permisiuniAltii(struct stat stat, int descriptor)
{
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

char *ultimaModificare(time_t s)
{
    char *rezultat = malloc(9 * sizeof(char));
    char *aux = malloc(9 * sizeof(char));
    char *aux2 = malloc(9 * sizeof(char));
    struct tm timp;
    memset(&timp,0,sizeof(struct tm));
    sprintf(aux,"%ld",s);
    strptime(aux,"%s",&timp);
    strftime(aux2,sizeof(aux2),"%d.",&timp);
    strcat(rezultat,aux2);
    strftime(aux2,sizeof(rezultat),"%m.%Y",&timp);
    strcat(rezultat,aux2);
    return rezultat;
}

int bmpFileStat(char *file,char *dir)
{
    nr_linii = 0;
    path = malloc(100 * sizeof(char));
    name = malloc(100 * sizeof(char));
    struct stat s;
    stat(file,&s);
    if(S_ISREG(s.st_mode))
    {
        userID = malloc(sizeof(unsigned));
        dimensiune = malloc(sizeof(unsigned));
        links = malloc(sizeof(int));
        modificare = malloc(50 * sizeof(char));
        inaltime = malloc(sizeof(int));
        latime = malloc(sizeof(int));
        int *aux = malloc(sizeof(int));
        name = basename(file);
        sprintf(path,"%s/%s_statistica.txt",dir,name);
        int descriptor = open(path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR |S_IXUSR| S_IRGRP |S_IWGRP| S_IROTH);
        if(descriptor == -1)
        {
            perror("Eroare!");
            exit(-1);
        }
        int descriptor2 = open(file,O_RDONLY);
        if(descriptor2 == -1)
        {
            perror("Eroare");
            exit(-1);
        }
        coddescriere = write(descriptor, "Nume fisier: ", 13);
        if(coddescriere != -1)
        nr_linii+=1;
        codinfo = write(descriptor,file,strlen(file) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Inaltime: ", 10);
        if(coddescriere != -1)
        nr_linii++;
        if((lseek(descriptor2,18,SEEK_SET) == -1))
        {
            perror("Eroare");
        }
        if((read(descriptor2,aux,4) == -1))
        {
            perror("Eroare");
        }
        sprintf(inaltime,"%d",*aux);
        codinfo = write(descriptor,inaltime,strlen(inaltime));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Lungime: ", 9);
        if(coddescriere != -1)
        nr_linii++;
        if((read(descriptor2,aux,4) == -1))
        {
            perror("Eroare");
        }
        sprintf(latime,"%d",*aux);
        codinfo = write(descriptor,latime,strlen(latime));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Dimensiune: ", 12);
        if(coddescriere != -1)
        nr_linii++;
        sprintf(dimensiune,"%ld", s.st_size);
        codinfo = write(descriptor, dimensiune, strlen(dimensiune) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Identificatorul utilizatorului: ", 32);
        if(coddescriere != -1)
        nr_linii++;
        sprintf(userID,"%d", s.st_uid);
        codinfo = write(descriptor, userID, strlen(userID) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Timpul ultimei modificari: ", 27);
        if(coddescriere != -1)
        nr_linii++;
        time_t timp = s.st_mtime;
        modificare = ultimaModificare(timp);
        codinfo = write(descriptor, modificare, strlen(modificare) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Numar de legaturi: ", 19);
        if(coddescriere != -1)
        nr_linii++;
        sprintf(links,"%ld",s.st_nlink);
        codinfo = write(descriptor,links,strlen(links) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Drepturi de acces user: ", 24);
        if(coddescriere != -1)
        nr_linii++;
        permisiuniUser(s,descriptor);
        coddescriere = write(descriptor, "Drepturi de acces grup: ", 24);
        if(coddescriere != -1)
        nr_linii++;
        permisiuniGrup(s,descriptor);
        coddescriere = write(descriptor, "Drepturi de acces altii: ", 25);
        if(coddescriere != -1)
        nr_linii++;
        permisiuniAltii(s,descriptor);
        write(descriptor,"\n",1);
        close(descriptor2);
        close(descriptor);
    }
    return nr_linii;
}

int fileStat(char *file,char *dir)
{
    nr_linii = 0;
    path = malloc(100 * sizeof(char));
    name = malloc(100 * sizeof(char));
    struct stat s;
    stat(file,&s);
    if(S_ISREG(s.st_mode))
    {
        userID = malloc(sizeof(unsigned));
        dimensiune = malloc(sizeof(unsigned));
        links = malloc(sizeof(int));
        modificare = malloc(50 * sizeof(char));
        name = basename(file);
        sprintf(path,"%s/%s_statistica.txt",dir,name);
        int descriptor = open(path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(descriptor == -1)
        {
            perror("Eroare");
            exit(-1);
        }
        coddescriere = write(descriptor, "Nume fisier: ", 13);
        if(coddescriere != -1)
        nr_linii++;
        write(descriptor,file,strlen(file) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Dimensiune: ", 12);
        if(coddescriere != -1)
        nr_linii++;
        sprintf(dimensiune,"%ld", s.st_size);
        write(descriptor, dimensiune, strlen(dimensiune) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Identificatorul utilizatorului: ", 32);
        if(coddescriere != -1)
        nr_linii++;
        sprintf(userID,"%d", s.st_uid);
        write(descriptor, userID, strlen(userID) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Timpul ultimei modificari: ", 27);
        if(coddescriere != -1)
        nr_linii++;
        time_t timp = s.st_mtime;
        modificare = ultimaModificare(timp);
        write(descriptor, modificare, strlen(modificare) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Numar de legaturi: ", 19);
        if(coddescriere != -1)
        nr_linii++;
        sprintf(links,"%ld",s.st_nlink);
        write(descriptor,links,strlen(links) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Drepturi de acces user: ", 24);
        if(coddescriere != -1)
        nr_linii++;
        permisiuniUser(s, descriptor);
        coddescriere = write(descriptor, "Drepturi de acces grup: ", 24);
        if(coddescriere != -1)
        nr_linii++;
        permisiuniGrup(s, descriptor);
        coddescriere = write(descriptor, "Drepturi de acces altii: ", 25);
        if(coddescriere != -1)
        nr_linii++;
        permisiuniAltii(s, descriptor);
        write(descriptor,"\n",1);
        close(descriptor);
    }
    return nr_linii;
}


int dirStat(char *dir,char *dir2)
{
    nr_linii = 0;
    path = malloc(100 * sizeof(char));
    name = malloc(100 * sizeof(char));
    userID = malloc(sizeof(unsigned));
    struct stat dirstat;
    stat(dir,&dirstat);
    if(S_ISDIR(dirstat.st_mode))
    {
        name = basename(dir);
        sprintf(path,"%s/%s_statistica.txt",dir2,name);
        int descriptor = open(path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(descriptor == -1)
        {
            perror("Eroare");
            exit(-1);
        }
        coddescriere = write(descriptor,"Nume director: ", 14);
        if(coddescriere != -1)
            nr_linii++;
        write(descriptor,dir,strlen(dir) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
            nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Identificatorul utilizatorului: ", 32);
        if(coddescriere != -1)
            nr_linii++;
        sprintf(userID,"%d", dirstat.st_uid);
        write(descriptor, userID, strlen(userID) * sizeof(char));
        if(codinfo != -1 && coddescriere == -1)
            nr_linii++;
        write(descriptor,"\n",1);
        coddescriere = write(descriptor, "Drepturi de acces user: ", 24);
        if(coddescriere != -1)
            nr_linii++;
        permisiuniUser(dirstat,descriptor);
        coddescriere = write(descriptor, "Drepturi de acces grup: ", 24);
        if(coddescriere != -1)
            nr_linii++;
        permisiuniGrup(dirstat,descriptor);
        coddescriere = write(descriptor, "Drepturi de acces altii: ", 25);
        if(coddescriere != -1)
            nr_linii++;
        permisiuniAltii(dirstat,descriptor);
        write(descriptor,"\n",1);
        close(descriptor);
    }
        return nr_linii;
}

int symbloicLinkStat(char *file,char *dir)
{
    nr_linii = 0;
    path = malloc(100 * sizeof(char));
    name = malloc(100 * sizeof(char));
    dimensiune = malloc(sizeof(unsigned));
    struct stat s;
    struct stat st;
    stat(file,&st);
    lstat(file,&s);
    name = basename(file);
    sprintf(path,"%s/%s_statistica.txt",dir,name);
    int descriptor = open(path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(descriptor == -1)
    {
        perror("Eroare");
        exit(-1);
    }
    coddescriere = write(descriptor,"Nume: ",6);
    if(coddescriere != -1)
        nr_linii++;
    write(descriptor,file,strlen(file) * sizeof(char));
    if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
    write(descriptor,"\n",1);
    coddescriere = write(descriptor, "Dimensiune: ", 12);
    if(coddescriere != -1)
        nr_linii++;
    sprintf(dimensiune,"%ld", s.st_size);
    write(descriptor, dimensiune, strlen(dimensiune) * sizeof(char));
    if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
    write(descriptor,"\n",1);
    coddescriere = write(descriptor,"Dimensiune target: ",19);
    if(coddescriere != -1)
        nr_linii++;
    sprintf(dimensiune,"%ld", st.st_size);
    write(descriptor, dimensiune, strlen(dimensiune) * sizeof(char));
    if(codinfo != -1 && coddescriere == -1)
        nr_linii++;
    write(descriptor,"\n",1);
    coddescriere = write(descriptor, "Drepturi de acces user: ", 24);
    if(coddescriere != -1)
        nr_linii++;
    permisiuniUser(s,descriptor);
    coddescriere = write(descriptor, "Drepturi de acces grup: ", 24);
    if(coddescriere != -1)
        nr_linii++;
    permisiuniGrup(s,descriptor);
    coddescriere = write(descriptor, "Drepturi de acces altii: ", 25);
    if(coddescriere != -1)
        nr_linii++;
    permisiuniAltii(s,descriptor);
    write(descriptor,"\n",1);
    close(descriptor);
    return nr_linii;
}