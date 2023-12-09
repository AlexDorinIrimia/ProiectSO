# include <stdio.h>
# include <stdlib.h>
# include "Parser.h"

int main(int argc, char**argv)
{
    if(argc != 4)
    {
        printf("Usage ./program <%s> <%s> <%s>\n", argv[1], argv[2],argv[3]);
        exit(-1);
    }
    char c = *argv[3];
    int n = 0;
    n = parse(argv[1],argv[2],c);
    printf("Au fost identificate in total %d propozitii corecte care contin caracterul %c.\n",n,c);
    return 0;
}