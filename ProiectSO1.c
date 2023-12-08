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
    parse(argv[1],argv[2],c);
    return 0;
}