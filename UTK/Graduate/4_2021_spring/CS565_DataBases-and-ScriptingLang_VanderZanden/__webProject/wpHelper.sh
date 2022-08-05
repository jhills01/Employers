#!/bin/bash

cnt=1
max=6
dir="/home/jhills/gradSchool/_classes/4_2021_spring/CS565_DataBases-and-ScriptingLang_VanderZanden"
wp1="__webProject/wp1"
wp2="__webProject/wp2"
wp3="__webProject/wp3"
wp4="__webProject/wp4"
chk="/subChk"

exeDir="$dir/$wp1" #$chk

mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -pDELETED_4_SECURITY -e "source $exeDir/abet.sql;"
if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
    then 
        echo "ERROR: abet.sql\n" 
        exit 2 
fi
mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -pDELETED_4_SECURITY -e "source $exeDir/sampleData.sql;"
if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
    then 
        echo "ERROR: sampleData.sql\n" 
        exit 2 
fi

if [ $# -eq 0 ]; 
then
    while [ $cnt -le $max ]
    do    
        printf "\n$exeDir/q$cnt.sql\n"
        
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -pDELETED_4_SECURITY -e "source $exeDir/q$cnt.sql;"       

        ((cnt++))
    done

else
    printf "\n$exeDir/q$1.sql\n"
    
    mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -pDELETED_4_SECURITY -e "source $exeDir/q$1.sql;"       

fi
printf "\nEND\n"