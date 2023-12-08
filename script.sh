#!/bin/bash

if test "$#" -ne 1
then
echo "Use .script.sh "
exit 1
fi

contor=0
while read -r linie || test -n "$linie"
    do
        rezultat=`echo $linie | grep ^[A-Z] | grep $1 | grep [.]$ | grep -v ",si"`
        if test -n "$rezultat"
        then
            contor=`expr $contor + 1`
        fi
done
echo $contor