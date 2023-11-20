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

char* userID;
char* dimensiune;
char* links;
char* modificare;
char* inaltime;
char* latime;
char *newpath;
char *path;
char *name;

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

void bmpFileStat(char *file,char *dir)
{
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
        int descriptor = open(path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(descriptor == -1)
        {
            perror("Eroare");
            exit(-1);
        }
        int descriptor2 = open(file,O_RDONLY);
        if(descriptor2 == -1)
        {
            perror("Eroare");
            exit(-1);
        }
        write(descriptor, "Nume fisier: ", 13);
        write(descriptor,file,strlen(file) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Inaltime: ", 10);
        if((lseek(descriptor2,18,SEEK_SET) == -1))
        {
            perror("Eroare");
        }
        if((read(descriptor2,aux,4) == -1))
        {
            perror("Eroare");
        }
        sprintf(inaltime,"%d",*aux);
        write(descriptor,inaltime,strlen(inaltime));
        write(descriptor,"\n",1);
        write(descriptor, "Lungime: ", 9);
        if((read(descriptor2,aux,4) == -1))
        {
            perror("Eroare");
        }
        sprintf(latime,"%d",*aux);
        write(descriptor,latime,strlen(latime));
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
        time_t timp = s.st_mtime;
        modificare = ultimaModificare(timp);
        write(descriptor, modificare, strlen(modificare) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Numar de legaturi: ", 19);
        sprintf(links,"%ld",s.st_nlink);
        write(descriptor,links,strlen(links) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Drepturi de acces user: ", 24);
        permisiuniUser(s,descriptor);
        write(descriptor, "Drepturi de acces grup: ", 24);
        permisiuniGrup(s,descriptor);
        write(descriptor, "Drepturi de acces altii: ", 25);
        permisiuniAltii(s,descriptor);
        write(descriptor,"\n",1);
        close(descriptor2);
        close(descriptor);

    }
}

void fileStat(char *file,char *dir)
{
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
        write(descriptor, "Nume fisier: ", 13);
        write(descriptor,file,strlen(file) * sizeof(char));
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
        time_t timp = s.st_mtime;
        modificare = ultimaModificare(timp);
        write(descriptor, modificare, strlen(modificare) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Numar de legaturi: ", 19);
        sprintf(links,"%ld",s.st_nlink);
        write(descriptor,links,strlen(links) * sizeof(char));
        write(descriptor,"\n",1);
        write(descriptor, "Drepturi de acces user: ", 24);
        permisiuniUser(s, descriptor);
        write(descriptor, "Drepturi de acces grup: ", 24);
        permisiuniGrup(s, descriptor);
        write(descriptor, "Drepturi de acces altii: ", 25);
        permisiuniAltii(s, descriptor);
        write(descriptor,"\n",1);
        close(descriptor);

    }
}


void dirStat(char *dir,char *dir2)
{
    path = malloc(100 * sizeof(char));
    name = malloc(100 * sizeof(char));
    userID = malloc(sizeof(unsigned));
    struct stat dirstat;
    stat(dir,&dirstat);
    name = basename(dir);
    sprintf(path,"%s/%s_statistica.txt",dir2,name);
    int descriptor = open(path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(descriptor == -1)
    {
        perror("Eroare");
        exit(-1);
    }
    write(descriptor,"Nume director: ", 14);
    write(descriptor,dir,strlen(dir) * sizeof(char));
    write(descriptor,"\n",1);
    write(descriptor, "Identificatorul utilizatorului: ", 32);
    sprintf(userID,"%d", dirstat.st_uid);
    write(descriptor, userID, strlen(userID) * sizeof(char));
    write(descriptor,"\n",1);
    write(descriptor, "Drepturi de acces user: ", 24);
    permisiuniUser(dirstat,descriptor);
    write(descriptor, "Drepturi de acces grup: ", 24);
    permisiuniGrup(dirstat,descriptor);
    write(descriptor, "Drepturi de acces altii: ", 25);
    permisiuniAltii(dirstat,descriptor);
    write(descriptor,"\n",1);
    close(descriptor);
}

void symbloicLinkStat(char *file,char *dir)
{
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
    write(descriptor,"Nume: ",6);
    write(descriptor,file,strlen(file) * sizeof(char));
    write(descriptor,"\n",1);
    write(descriptor, "Dimensiune: ", 12);
    sprintf(dimensiune,"%ld", s.st_size);
    write(descriptor, dimensiune, strlen(dimensiune) * sizeof(char));
    write(descriptor,"\n",1);
    write(descriptor,"Dimensiune target: ",19);
    sprintf(dimensiune,"%ld", st.st_size);
    write(descriptor, dimensiune, strlen(dimensiune) * sizeof(char));
    write(descriptor,"\n",1);
    write(descriptor, "Drepturi de acces user: ", 24);
    permisiuniUser(s,descriptor);
    write(descriptor, "Drepturi de acces grup: ", 24);
    permisiuniGrup(s,descriptor);
    write(descriptor, "Drepturi de acces altii: ", 25);
    permisiuniAltii(s,descriptor);
    write(descriptor,"\n",1);
    close(descriptor);
}

int bmpORother(char *file)
{
    char *ext = strstr(file,".bmp");
    if(ext)
        return 1;
    else
        return 0;
}

int parse(char *Dir,char *dir)
{
    if((newpath = malloc(150 * sizeof(char))) == NULL)
    {
        perror("Eroare!");
        return -1;
    }
    DIR *director;
    struct dirent *direct;
    struct stat st;
    int stare = 0;
    if((director = opendir(Dir)) == NULL)
    {
        perror("Eroare!");
        return -1;
    }
    while((direct = readdir(director)) != NULL)
    {   
        if((strcmp(direct->d_name,".") == 0) || (strcmp(direct->d_name,"..") == 0))
        {
            continue;
        }
        else
        {
            sprintf(newpath,"%s/%s", Dir,direct->d_name);
            stat(newpath,&st);
            int pid = fork();
            if(pid == 0)
            {
                if(S_ISREG(st.st_mode))
                {
                    if((bmpORother(newpath)))
                        bmpFileStat(newpath,dir);
                    else
                        fileStat(newpath,dir);
                }
                if(S_ISLNK(st.st_mode))
                {
                    symbloicLinkStat(newpath,dir);
                }
                if(S_ISDIR(st.st_mode))
                {
                    dirStat(newpath,dir);
                    parse(newpath,dir);
                }
                exit(0);
            }
            else 
            {
               wait(&stare);
               printf("S-a incheiat procesul cu PID %d si codul %d!\n",getpid(),stare);
            }
    
        }
    }
    closedir(director);
    return 0;
}

int main(int argc, char**argv)
{
    if(argc != 3)
    {
        printf("Usage ./program %s\n", argv[1]);
        exit(-1);
    }

    parse(argv[1],argv[2]);
    return 0;
}