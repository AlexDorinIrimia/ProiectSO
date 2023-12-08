# include <time.h>

void permisiuniUser(struct stat stat, int descriptor);
void permisiuniGrup(struct stat stat,int descriptor);
void permisiuniAltii(struct stat stat, int descriptor);

char *ultimaModificare(time_t s);

int bmpFileStat(char *file,char *dir);
int fileStat(char *file,char *dir);
int dirStat(char *dir,char *dir2);
int symbloicLinkStat(char *file,char *dir);