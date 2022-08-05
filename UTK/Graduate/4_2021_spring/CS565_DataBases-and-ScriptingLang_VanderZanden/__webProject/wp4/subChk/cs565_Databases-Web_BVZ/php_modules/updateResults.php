<?php
    //IMPORTANT: COMMENT OUT THIS INCLUDE B4 SUBMITTING
    include "include/passwordHider.php";

    function setAndNotEmptyStr($getItem, $getItemName)
    {
        // if ( isset($getItem) && !empty($getItem) ) {return True;} // empty() should not be used according to
                                                                     // ...many online references. 
        if ( isset($getItem) && ($getItem !== "") ) {return True;} // use "!==" instead of "!=" b/c != is true
                                                                   // ...for numeric 0. We want empty string.
        else {die("ERROR: \"$getItemName\" in _GET[] is not set or is empty string.\n");}
    }

    // Connect 
    //IMPORTANT: IN SUBMISSION COMMENTS, give them "$pwHidden" as password... 
    //           ...so they know what to search/replace with. 
    $mysqli = new mysqli("dbs2.eecs.utk.edu", "jhills", $pwHidden, "cosc465_jhills"); 
    if ($mysqli->connect_errno) 
    {
        die("Failed to connect to MySQL: ($mysqli->connect_errno) 
            $mysqli->connect_error");
    }

    // Get parameters from the query string
    // NOTE: $_GET will need to be changed to $_POST for wp4
    if (setAndNotEmptyStr($_POST["sectionId"], "sectionId")) {$sectionId = $_POST["sectionId"];}
    if (setAndNotEmptyStr($_POST["outcomeId"], "outcomeId")) {$outcomeId = $_POST["outcomeId"];}
    if (setAndNotEmptyStr($_POST["major"], "major")) {$major = $_POST["major"];} 
    if (setAndNotEmptyStr($_POST["performanceLevel"], "performanceLevel")) {$performanceLevel = $_POST["performanceLevel"];}
    if (setAndNotEmptyStr($_POST["numberOfStudents"], "numberOfStudents")) {$numberOfStudents = $_POST["numberOfStudents"];}
      
    // construct query
    // NOTE: "bind_param" was not workng b/c I forgot to put extra space at the end of each string... 
    //       ... for each concatenated line
    $sql =  "INSERT INTO `OutcomeResults` (`sectionId`, `outcomeId`, `major`, `performanceLevel`, `numberOfStudents`) " . 
            "VALUES (?, ?, ?, ?, ?) " .
                "ON DUPLICATE KEY UPDATE `numberOfStudents` = ?";
    
    // construct prepared statement 
    $stmt = $mysqli->prepare($sql);
    // if ($stmt === False) print("prepare() failed: " . htmlspecialchars($stmt->error) . "\n");
    if ($stmt === False) print("prepare() failed: \n");
    
    // NOTE: $numberOfStudents is included twice at the end for the last 2 "?" in $sql
    $errChk = $stmt->bind_param("iisiii", $sectionId, $outcomeId, $major, $performanceLevel, $numberOfStudents, $numberOfStudents);
    if ($errChk === False) print("bind_param() failed: " . htmlspecialchars($stmt->error) . "\n");
    
    $errChk = $stmt->execute();
    if ($errChk === False) print("execute() failed: " . htmlspecialchars($stmt->error) . "\n");

?>