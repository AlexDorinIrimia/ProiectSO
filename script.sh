counter=0
while read line
do
    rezultat=`echo $line | grep ^[A-Z] | grep [.]$ | gerp -v ",si"`
    if test -n "$rezultat"
        counter=`expr $counter + 1`
    fi
done