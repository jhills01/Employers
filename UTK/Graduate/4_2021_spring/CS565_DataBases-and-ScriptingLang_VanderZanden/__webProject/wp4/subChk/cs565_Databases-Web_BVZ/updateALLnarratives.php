<?php
    include "include/passwordHider.php";

    function setAndNotEmptyStr($getItem, $getItemName)
    {
        // if ( isset($getItem) && !empty($getItem) ) {return True;} // empty() should not be used according to
                                                                     // ...many online references. 
        if ( isset($getItem) && ($getItem !== "") ) {return True;} // use "!==" instead of "!=" b/c != is true
                                                                   // ...for numeric 0. We want empty string.
        else {die("ERROR: \"$getItemName\" in _GET[] is not set or is empty string.\n");}
    }

    
?>