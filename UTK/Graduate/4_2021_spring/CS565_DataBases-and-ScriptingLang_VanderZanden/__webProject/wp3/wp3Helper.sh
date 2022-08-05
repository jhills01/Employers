#!/bin/bash
# cd /home/jhills/gradSchool/_classes/4_2021_spring/CS565_DataBases-and-ScriptingLang_VanderZanden

dir="/home/jhills/gradSchool/_classes/4_2021_spring/CS565_DataBases-and-ScriptingLang_VanderZanden"
wp1="__webProject/wp1"
wp2="__webProject/wp2"
wp3="__webProject/wp3"
wp4="__webProject/wp4"
chk="/subChk"

exeDir="$dir/$wp3" #$chk

testSql=false
# testSql=true
runAllPhp=false

arguments="$*" # captures all arguments from CL

# CREATE DATABASE----------------------------------------------------------------------------------------------
if [[ $testSql = true ]];
then
    # create tables
    mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "source $dir/$wp1/abet.sql;"
    if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
        then 
            echo "ERROR: abet.sql\n" 
            exit 1 
    fi

    # insert sample data
    # NOTE: Don't edit "sampleData.sql" which was given by Dr. VZ.
    mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "source $dir/$wp1/sampleData.sql;"
    if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
    then 
        echo "ERROR: sampleData.sql\n" 
        exit 2 
    fi
fi
# RUN PHP SCRIPTS----------------------------------------------------------------------------------------------
if [[ $runAllPhp = false ]];
then

    printf "=====================================================================\n"
    printf "\nphp-cgi -f $arguments\n"
    printf "=====================================================================\n"
    php-cgi -f $arguments 
    if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
    then 
        echo "ERROR: php-cgi -f $arguments\n" 
        exit 1 
    fi

    printf "\n=====================================================================\n"
    printf "END\n"

# else
    # Not sure how to go about doing this one yet

    #-------------------------------------------------------------
    # login.php email='coyote@utk.edu' password='password'

    #-------------------------------------------------------------
    # outcomes.php sectionId=1 major='CS'

    #-------------------------------------------------------------
    # results.php outcome=2 major='CS' sectionId=3

    #-------------------------------------------------------------
    # assessment.php sectionId=3 outcome=2 major='CS'

    #-------------------------------------------------------------
    # narrative.php major='CS' outcome=2 sectionId=5

    #-------------------------------------------------------------
    # updateResults.php sectionId=9 outcomeId=2 major='CS' performanceLevel=3 numberOfStudents=80

    #-------------------------------------------------------------
    # NOTE: Notice the "+" symbol representing a space in...
    #       ...in the assessmentDescription string
    # updateAssessment.php assessmentId=null sectionId=1 assessmentDescription='testing+updateAssessment.php' weight=100 outcomeId=1 major='CS'
    # updateAssessment.php assessmentId=20 sectionId=1 assessmentDescription='testing+updateAssessment.php' weight=100 outcomeId=1 major='CS'
    # updateAssessment.php assessmentId=18 sectionId=1 assessmentDescription='testing+updateAssessment.php' weight=100 outcomeId=1 major='CS'

    #-------------------------------------------------------------
    # updateNarrative.php sectionId=8 major='CS' outcomeId=2 strengths='testing+narrative.php' weaknesses='testing+narrative.php' actions='testing+narrative.php'

    #-------------------------------------------------------------
    # ./wp3Helper.sh deleteAssessment.php assessmentId=18
fi
# RUN TEST SQL SCRIPT----------------------------------------------------------------------------------------------
if [[ $testSql = true ]];
then
    mysql -h dbs2.eecs.utk.edu -p cosc465_jhills -u jhills -ptheflesh0101 -e "source $dir/$wp3/test.sql;"
    if [[ $? != 0 ]]; # $? is a error catch which captures exit status of last program.
        then 
            echo "ERROR: test.sql\n" 
            exit 3 
    fi
fi