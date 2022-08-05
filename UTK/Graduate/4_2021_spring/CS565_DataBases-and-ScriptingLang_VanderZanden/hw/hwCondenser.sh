#!/bin/bash

#SCRIPT INSTRUCTIONS
# 1) Adjust "Variable Parameters" to whatever you need
# 2) Then simply run the script in ...../cs140_Emrich directory 

#misc variables
fileCnt=0

#Variable variables
hwNum=6

#derived variables--------------------------------------------------------------
hwDir="hw$hwNum/solutions/"

printf "HOMEWORK $hwNum==============================================================================================================\n\n" >> allHW.sql
cd $hwDir
# Loop will handle each file individually as they are read in
for f in *.js
# for f in *.sql
do
    # get file name (everything B4 1st '_') and extension separately
    printf "HW$hwNum - $f: ----------------------------------------------------------------------------------------------------------------------\n" >> ../../HW6.sql
	cat $f >> ../../HW6.sql
	printf "\n\n" >> ../../HW6.sql

done

