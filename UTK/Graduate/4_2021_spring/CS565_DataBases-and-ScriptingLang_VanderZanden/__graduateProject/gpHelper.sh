#!/bin/bash
# cd /home/jhills/gradSchool/_classes/4_2021_spring/CS565_DataBases-and-ScriptingLang_VanderZanden

esd=1 # This variable stands for "extra sample data" and is additional inserts from me to test queries further
dir="/home/jhills/gradSchool/_classes/4_2021_spring/CS565_DataBases-and-ScriptingLang_VanderZanden/__graduateProject/sql"
chk="/subChk"

exeDir="$dir" #$chk

# CREATE DATABASE & INSERT SAMPLE DATA----------------------------------------------------------------------------
printf "\nsource $exeDir/university.sql;\n\n"
mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "source $exeDir/university.sql;"
if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
    then 
        echo "ERROR: university.sql\n" 
        exit 1 
fi

# TESTING for university.sql
if [[ $1 = "test" ]]; # if $2 argument is unset, then don't use it
    then
        printf "\nTESTING...\n\n"
        # mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "show tables;"   
        printf "apartmentInspections=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe apartmentInspections;" 
        printf "apartmentPlaces=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe apartmentPlaces;" 
        printf "hallPlaces=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe hallPlaces;" 
        printf "halls=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe halls;" 
        printf "Invoice=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe Invoice;" 
        printf "nextOfKin=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe nextOfKin;" 
        printf "Lease=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe Lease;"
        printf "Student=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe Student;" 
        printf "Adviser=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe Adviser;" 
        printf "residencePlaces=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe residencePlaces;" 
        printf "residenceStaff=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe residenceStaff;" 
        printf "apartmentBuildings=================================================================\n"
        mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "describe apartmentBuildings;" 
fi

# EXECUTE QUERIES------------------------------------------------------------------------------------------------
printf "\n\nsource $exeDir/queries.sql;\n\n"
mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "source $exeDir/queries.sql;"
if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
    then 
        echo "ERROR: sampleData.sql\n" 
        exit 2 
fi

printf "\nEND\n"