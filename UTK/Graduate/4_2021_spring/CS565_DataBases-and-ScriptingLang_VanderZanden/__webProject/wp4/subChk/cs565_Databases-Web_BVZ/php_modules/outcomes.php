<?php
    //IMPORTANT: COMMENT OUT THIS INCLUDE B4 SUBMITTING
    include "include/passwordHider.php";

    function setAndNotEmptyStr($postItem, $postItemName)
    {
        // if ( isset($postItem) && !empty($postItem) ) {return True;} // empty() should not be used according to
                                                                     // ...many online references. 
        if ( isset($postItem) && ($postItem !== "") ) {return True;} // use "!==" instead of "!=" b/c != is true
                                                                   // ...for numeric 0. We want empty string.
        else {die("ERROR: \"$postItemName\" in _post[] is not set or is empty string.\n");}
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

    // post parameters from the query string
    if (setAndNotEmptyStr($_POST["sectionId"], "sectionId")) {$sectionId = $_POST["sectionId"];}
    if (setAndNotEmptyStr($_POST["major"], "major")) {$major = $_POST["major"];}
    // if (setAndNotEmptyStr($_POST["requestId"], "requestId")) {$requestId = $_POST["requestId"];}
        
    // construct query
    // NOTE: "bind_param" was not workng b/c I forgot to put extra space at the end of each string... 
    //       ... for each concatenated line
    $sql = "SELECT o.outcomeId, o.outcomeDescription " .
            "FROM Outcomes o " . 
            "NATURAL JOIN CourseOutcomeMapping com " . 
            "NATURAL JOIN Sections s " .
            "WHERE s.sectionId = ? " .
                "AND o.major = ? " .
            "ORDER BY o.outcomeId ASC";

    // construct prepared statement 
    $stmt = $mysqli->prepare($sql);
    // if ($stmt === False) print("prepare() failed: " . htmlspecialchars($stmt->error) . "\n");
    if ($stmt === False) print("prepare() failed: \n");

    $errChk = $stmt->bind_param("is", $sectionId, $major);
    if ($errChk === False) print("bind_param() failed: " . htmlspecialchars($stmt->error) . "\n");

    $errChk = $stmt->execute();
    if ($errChk === False) print("execute() failed: " . htmlspecialchars($stmt->error) . "\n");

    // $result = $stmt->get_result(); //doesn't work b/c "mysqlnd driver" is not installed
    
    $errChk = $stmt->bind_result($outcomeId, $outcomeDescription);
    if ($errChk === False) print("bind_result() failed: " . htmlspecialchars($stmt->error) . "\n");

    // fetch values
    $results = array();
    while ($stmt->fetch()) 
    {
        $tempArr = array("outcomeId" => $outcomeId, "outcomeDescription" => $outcomeDescription, "requestId" => $requestId);

        array_push($results, $tempArr);
    }

    $stmt->close();

    session_start();
    // $_SESSION["outcomes"]["$sectionId $major"] = $results;
    $_SESSION["outcomes"] = $results;
    // echo json_encode($_SESSION["outcomes"]); 


    // // print_r($results);
    // // print("\n--------------------------------------------------------------------------------\n");

    // echo json_encode($results, JSON_PRETTY_PRINT);
    // print("\n");
?>