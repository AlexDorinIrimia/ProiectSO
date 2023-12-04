#!/bin/bash

if test "$#" -ne 2
then
echo "Use .script.sh "
exit 1
fi

if test -f "$2"
then
contor=0
while read linie
    do
        rezultat=`echo $linie | grep -E '[A-Z].*\.$' | grep -v ',si'`
        if test -n "$rezultat"
        then
        contor=`expr $contor + 1`
        fi
done < "$1"
echo "Numar de propozitii corecte: $contor" > "$2"
fi