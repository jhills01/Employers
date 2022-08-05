<?php
    //IMPORTANT: COMMENT OUT THIS INCLUDE B4 SUBMITTING
    include "include/passwordHider.php";

    function setAndNotEmptyStr($postItem, $postItemName)
    {
        // if ( isset($postItem) && !empty($postItem) ) {return True;} // empty() should not be used according to
                                                                     // ...many online references. 
        if ( isset($postItem) && ($postItem !== "") ) {return True;} // use "!==" instead of "!=" b/c != is true
                                                                   // ...for numeric 0. We want empty string.
        else {die("ERROR: \"$postItemName\" in _POST[] is not set or is empty string.\n");}
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
    if (setAndNotEmptyStr($_POST["email"], "email")) {$email = $_POST["email"];}
    if (setAndNotEmptyStr($_POST["password"], "password")) {$password = $_POST["password"];}
        
    // construct query
    // NOTE: "bind_param" was not workng b/c I forgot to put extra space at the end of each string... 
    //       ... for each concatenated line
    $sql = "SELECT DISTINCT s.instructorId, s.sectionId, s.courseId, com.major, s.semester, s.year " .
    "FROM Instructors i " .
    "NATURAL JOIN Sections s " .
    "NATURAL JOIN CourseOutcomeMapping com " .
    "WHERE i.email = ? " .
        "AND i.password = PASSWORD( ? ) " .
    "ORDER BY s.year DESC, s.semester ASC";

    // construct prepared statement 
    $stmt = $mysqli->prepare($sql);
    // if ($stmt === False) print("prepare() failed: " . htmlspecialchars($stmt->error) . "\n");
    if ($stmt === False) print("prepare() failed: \n");
    
    $errChk = $stmt->bind_param("ss", $email, $password);
    if ($errChk === False) print("bind_param() failed: " . htmlspecialchars($stmt->error) . "\n");
    
    $errChk = $stmt->execute();
    if ($errChk === False) print("execute() failed: " . htmlspecialchars($stmt->error) . "\n");

    // $result = $stmt->get_result(); //doesn't work b/c "mysqlnd driver" is not installed
    $errChk = $stmt->bind_result($instructorId, $sectionId, $courseId, $major, $semester, $year);
    if ($errChk === False) print("bind_result() failed: " . htmlspecialchars($stmt->error) . "\n");

    // fetch values
    $results = array();
    while ($stmt->fetch()) 
    {
        $tempArr = array("instructorId" => $instructorId, "sectionId" => $sectionId, "courseId" => $courseId, "major" => $major, "semester" => $semester, "year" => $year);

        array_push($results, $tempArr);
    }

    $stmt->close();

    //establish session and store results for later use
    //NOTE: storing $results in SESSION reorders items. Looks like it sorts them somehow.
    session_start();
    $_SESSION["login"] = $results;
    echo json_encode($_SESSION["login"]); 

    // echo json_encode($results, JSON_PRETTY_PRINT);
    // echo json_encode($results);
    // print("\n");
?>