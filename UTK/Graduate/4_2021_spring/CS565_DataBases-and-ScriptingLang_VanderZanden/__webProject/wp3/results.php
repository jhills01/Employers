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
    if (setAndNotEmptyStr($_GET["major"], "major")) {$major = $_GET["major"];}
    if (setAndNotEmptyStr($_GET["outcome"], "outcome")) {$outcome = $_GET["outcome"];}
    if (setAndNotEmptyStr($_GET["sectionId"], "sectionId")) {$sectionId = $_GET["sectionId"];}
        
    // construct query
    // NOTE: "bind_param" was not workng b/c I forgot to put extra space at the end of each string... 
    //       ... for each concatenated line
    $sql = "SELECT pl.description, ocr.numberOfStudents " .
            "FROM OutcomeResults ocr " . 
            "NATURAL JOIN PerformanceLevels pl " . 
            "WHERE ocr.outcomeId = ? " .
                "AND ocr.major = ? " .
                "AND ocr.sectionId = ? " .
            "ORDER BY ocr.performanceLevel ASC";

    // construct prepared statement 
    $stmt = $mysqli->prepare($sql);
    if ($stmt === False) print("prepare() failed: " . htmlspecialchars($stmt->error) . "\n");

    $errChk = $stmt->bind_param("isi", $outcome, $major, $sectionId);
    if ($errChk === False) print("bind_param() failed: " . htmlspecialchars($stmt->error) . "\n");

    $errChk = $stmt->execute();
    if ($errChk === False) print("execute() failed: " . htmlspecialchars($stmt->error) . "\n");

    // $result = $stmt->get_result(); //doesn't work b/c "mysqlnd driver" is not installed

    $errChk = $stmt->bind_result($description, $numberOfStudents);
    if ($errChk === False) print("bind_result() failed: " . htmlspecialchars($stmt->error) . "\n");

    // fetch values
    $results = array();
    while ($stmt->fetch()) 
    {
        $tempArr = array("description" => $description, "numberOfStudents" => $numberOfStudents);

        array_push($results, $tempArr);
    }

    $stmt->close();

    // print_r($results);
    // print("\n--------------------------------------------------------------------------------\n");

    echo json_encode($results, JSON_PRETTY_PRINT);
    print("\n");
?>