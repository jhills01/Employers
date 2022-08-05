#!/bin/bash

mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -DELETED_4_SECURITY -e "source /home/jhills/gradSchool/_classes/4_2021_spring/CS565_DataBases-and-ScriptingLang_VanderZanden/hw/hw1/Hotel.sql;"

# printf "\nShow all rows of Hotel\n"
# mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -DELETED_4_SECURITY -e "SELECT * FROM Hotel;"

cnt=1
max=9
dir="source /home/jhills/gradSchool/_classes/4_2021_spring/CS565_DataBases-and-ScriptingLang_VanderZanden/hw"
hw="/hw1/solutions"
chk="/subChk"
fullDir=$dir$hw #$chk

if [ $# -eq 0 ]; 
then
    # mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -DELETED_4_SECURITY -e "SELECT * FROM Hotel;"
    # printf "\n"
    
    while [ $cnt -le $max ]
    do    
        printf "\n$fullDir/$cnt.sql\n"
        
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -DELETED_4_SECURITY -e "$fullDir/$cnt.sql;"       

        ((cnt++))
    done

else
    printf "\n$fullDir/$1.sql\n"
    
    mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -DELETED_4_SECURITY -e "$fullDir/$1.sql;"       

fi
printf "\nEND\n"