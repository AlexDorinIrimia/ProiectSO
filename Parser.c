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

#define PIX_OFF_BIT_OFF 10L

int bmpORother(char *file)
{
    char *ext = strstr(file,".bmp");
    if(ext)
        return 1;
    else
        return 0;
}

int childProcess(char *arg1,char*arg2,int(*func)(char *arg1,char* arg2))
{
    int stare;
    int pid =  fork();
    if(pid == -1)
    {
        return -1;
    }
    if(pid == 0)
    {
        exit(func(arg1,arg2));
    }
    else
    {
        wait(&stare);
        printf("S-au scris %d linii!\n",WEXITSTATUS(stare));
        return pid;
    }
}

void grayscale(char *file)
{
    int descriptor = open(file, O_RDWR);
    unsigned long y = lseek(descriptor, 0, SEEK_END);
    int fileLen = y;
    unsigned int pixelDataOffset = 0;
	lseek(descriptor, PIX_OFF_BIT_OFF, SEEK_SET);
	read(descriptor, &pixelDataOffset, sizeof(unsigned int));
    lseek(descriptor, pixelDataOffset, SEEK_SET);
    unsigned int pix_len = fileLen - pixelDataOffset;
    unsigned char* fileMem = malloc(pix_len);
    read(descriptor, fileMem, pix_len);
	for (int i = 0; i < pix_len; i += 3) {
		int gray = 0.299 * fileMem[i] + 0.587 * fileMem[i + 1] + 0.114 * fileMem[i + 2];
		fileMem[i]   = (unsigned char) gray;
		fileMem[i+1] = (unsigned char) gray;
		fileMem[i+2] = (unsigned char) gray;
	}
    lseek(descriptor, pixelDataOffset, SEEK_SET);
	write(descriptor, fileMem, pix_len);

	close(descriptor);
}

int correctSentenceNumber(char *file,char c)
{
    pid_t pid;
    int fd[2];

    pipe(fd);
    pid = fork();

    if(pid==0)
    {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp("cat","cat", file, (char*) NULL);
        fprintf(stderr, "Failed to execute 'cat'\n");
        exit(1);
    }
    else
    { 
        pid=fork();

        if(pid==0)
        {   
            dup2(fd[0],0);
            close(fd[1]);
            close(fd[0]);
            execlp("bash", "bash","script.sh",&c,(char*) NULL);
            fprintf(stderr, "Failed to execute bash script\n");
            exit(1);
        }
        else
        {
            int status;
            close(fd[0]);
            close(fd[1]);
            waitpid(pid,&status,0);
        }
    }
    return 0;
}

int parse(char *Dir,char *dir,char c)
{  
    char *newpath;
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
                    {   
                        int n = childProcess(newpath,dir,bmpFileStat);
                        grayscale(newpath);
                        exit(n);
                    }
                    else
                    {
                        int n = childProcess(newpath,dir,fileStat);
                        correctSentenceNumber(newpath,c);
                        exit(n);
                    }
                }
                else
                if(S_ISLNK(st.st_mode))
                {
                    int n = childProcess(newpath,dir,symbloicLinkStat);
                    exit(n);
                }
                else    
                if(S_ISDIR(st.st_mode))
                {
                    int n = childProcess(newpath,dir,dirStat);
                    parse(newpath,dir,c);
                    exit(n);
                }
            }
            else
            {
               int PID = wait(&stare);
               if(WEXITSTATUS(stare) >= 0)
               printf("S-a incheiat procesul cu PID %d si codul %d!\n",PID,EXIT_SUCCESS);
               else
               printf("S-a incheiat procesul cu PID %d si codul %d!\n",PID,EXIT_FAILURE);
            }
        }
    }
    closedir(director);
    return 0;
}