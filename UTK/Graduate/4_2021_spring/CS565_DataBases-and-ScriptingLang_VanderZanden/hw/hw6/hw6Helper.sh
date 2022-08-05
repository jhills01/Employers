#!/bin/bash

s="solutions/"

# create student database 
mongo < mongo_setup.js > /dev/null
if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
then 
    echo "ERROR: command \"mongo < mongo_setup.js > /dev/null\"" 
	mongo < mongo_setup.js
    exit 1 
fi

#add in my custom students for further testing
mongo < TOM_mongo_setup.js > /dev/null
if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
then 
    echo "ERROR: command \"mongo < TOM_mongo_setup.js > /dev/null\"" 
	mongo < TOM_mongo_setup.js
    exit 1 
fi


# test and perform homework problem
if [ $# -eq 0 ]; 
then
	printf "\nuhhh, you need to specific a HW number (i.e. 1.js, etc)\n"

else
    printf "\nmongo --quiet < $s$1.js\n\n"
    
	# "--quiet" supresses Mongo preface info
	mongo --quiet < $s$1.js
	
fi

