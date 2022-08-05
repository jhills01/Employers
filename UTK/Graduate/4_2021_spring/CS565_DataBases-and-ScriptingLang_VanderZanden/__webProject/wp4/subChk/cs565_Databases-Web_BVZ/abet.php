<!-- <!DOCTYPE html> -->
<html>
    <head>
        <title>UTK ABET - Results, Assessment, Narrative</title>
        <link rel="stylesheet" href="css/abet.css" />
        <script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
        <!-- needed to add "defer" so that selector could find form element 
             HOWEVER, "pageReload.js" does not need defer since I'm registering callback...
             ... which execute once DOM is completely loaded. 
        -->
        <script type="text/javascript" src="js/pageReload.js"></script> 
        <script type="text/javascript" src="js/nav_selectedDropDown.js" defer></script>
        <script type="text/javascript" src="js/nav_selectedOutcome.js" defer></script>
        <script type="text/javascript" src="js/updateResultsTotal.js" defer></script>
        <script type="text/javascript" src="js/preventEnterKeyRefresh-BtnSubmit.js" defer></script>
        <!-- <script type="text/javascript" src="js/assessmentChks.js" defer></script> -->
        <script type="text/javascript" src="js/deleteAssessment.js" defer></script>
        <script type="text/javascript" src="js/addAssessment.js" defer></script>
        <script type="text/javascript" src="js/narrativeSubmit_ajax.js" defer></script>
        <script type="text/javascript" src="js/assessmentSubmit_ajax.js" defer></script>
        <script type="text/javascript" src="js/resultsSubmit_ajax.js" defer></script>
        <script type="text/javascript" src="js/inputModified.js" defer></script>
        <script type="text/javascript" src="js/changePassword.js" defer></script>
        <script type="text/javascript" src="js/logout.js" defer></script>

    </head>

    <body>
        <!--header-------------------------------------------------------------------------------->
        <?php include "header.php";?>

        <!--nav-------------------------------------------------------------------------------->
        <?php include "nav.php";?>

        <!--results-------------------------------------------------------------------------------->
        <div class="results" id="results">
            <div id="resultsRow1">Results</div>
            <div id="resultsRow2">
                Please enter the number of students who do not expectations, meet expectations,
                and exceed expectations. You can type directly into the boxes --
                you do not need to use the arrows.
            </div>
            <div id="resultsRow3">
                <?php
                    //REMEMBER. All php code in header.php and nav.php is still in scope here!!!

                    /* NOTE: don't need to do this b/c I've already got selectedSectionId and selectedMajor from nav.php
                    $pieces = explode(' ', $selectedOption); //split string by spaces
                    $major = array_pop($pieces); // get last item out of string which should be major
                    */

                    // selectedOutcome was set in nav_selectedOutcome.js
                    $selectedOutcome = $_GET["selectedOutcome"];
                    if ($selectedOutcome === null) echo "selectedOutcome from GET is null";

                    $pieces = explode(' ', $selectedOutcome); //split string by spaces
                    $selectedOutcomeId = array_pop($pieces); // get last item out of string which should be the outcomeId used in queries

                    foreach($outcomes as $i)
                    {
                        foreach($i as $key => $val)
                        {
                            if ($key === "outcomeId" && $val == $selectedOutcomeId)
                            {
                                // next() advances the internal array pointer one place forward before returning the element value
                                $selectedOutcomeDescription = next($i);
                            }
                        }
                    }

                    echo "<b>$selectedOutcome - $selectedMajor:</b> $selectedOutcomeDescription";
                ?>
                <!-- <b>Test Outcome 2 - CS:</b> Design, implement, and evaluate a computing-based solution
                to meet a given set of computing requirements in the context of the program's discipline. -->
            </div>

            <div id="resultsRow4-1">Not Meets Expectations</div>
            <div id="resultsRow4-2">Meets Expectations</div>
            <div id="resultsRow4-3">Exceeds Expectations</div>
            <div id="resultsRow4-4">Total</div>



            <form id="resultsRow5" action="">

                <?php
                    $_POST["sectionId"] = $selectedSectionId;
                    $_POST["major"] = $selectedMajor;
                    $_POST["outcome"] = $selectedOutcomeId; //selectedOutcomeId created in preceding php island
                    $_POST["outcomeId"] = $selectedOutcomeId; //selectedOutcomeId created in preceding php island

                    include "php_modules/results.php";

                    $performanceLevels = $_SESSION["results"];
                    $total = 0;
                    $val = "";
                    
                    if ($performanceLevels[0])
                    {
                        $val = $performanceLevels[0]["numberOfStudents"];
                        $total += $val;
                        echo "<input id=\"resultsRow5-1\" type=\"number\" min=0 value=$val>";
                    }
                    else
                    {
                        echo "<input id=\"resultsRow5-1\" type=\"number\" min=0 value=0>";
                    }

                    if ($performanceLevels[1])
                    {
                        $val = $performanceLevels[1]["numberOfStudents"];
                        $total += $val;
                        echo "<input id=\"resultsRow5-2\" type=\"number\" min=0 value=$val>";
                    }
                    else
                    {
                        echo "<input id=\"resultsRow5-2\" type=\"number\" min=0 value=0>";
                    }

                    if ($performanceLevels[2])
                    {
                        $val = $performanceLevels[2]["numberOfStudents"];
                        $total += $val;
                        echo "<input id=\"resultsRow5-3\" type=\"number\" min=0 value=$val>";
                    }
                    else
                    {
                        echo "<input id=\"resultsRow5-3\" type=\"number\" min=0 value=0>";
                    }

                    echo "<input type=\"text\" id=\"resultsRow5-4\" value=$total readonly>";
                ?>
            
                <span id="resultErr" style="height: 0px"></span>
                    

                <div id="resultsRow6">
                    <button id="resultsRow6-1" type="submit">Save Results</button>
                </div>
            </form>
        </div>

        <!--assessPlan-------------------------------------------------------------------------------->
        <div class="assessPlan" id="assessPlan">
            <div id="assRow1">Assessment Plan</div>
            <div id="assRow2">
                <!-- ordered list are numbered by default -->
                <ol>
                    <li>
                        Please enter your assessment plan for each outcome. The weights are percentages from
                        0-100 and the weights should add up to 100%.
                    </li>
                    <li>
                        Always press "Save Assessment" when finished, even if you removed an assessment. The
                        trash can only removes an assessment from this screen - it does not remove it from the
                        database until you press "Save Assessments".
                    </li>
                </ol>
            </div>

            <div id="assRow3">
                <div id="assRow3-1">Weight (%)</div>
                <div id="assRow3-2">Description</div>
                <div id="assRow3-3">Remove</div>
            </div>
            <!--NOTE: I commented out FORM tag b/c I'm not sure it's needed.
                However, the good news:
                    1) Changing DIV to FORM with dame ID doesn't mess up layout!!!!!
                    2) And most likely, I can wrap FORM around anything and it will work seemlessly...
                       ...as long as I keep the same IDs for other elements

                UPDATE: 2) is NOT true if I stick FORM on outside like with id=form1 below-->
            <form id="assRow4" action="">
                <!-- input fields -->
                <?php
                    include "php_modules/assessment.php";

                    $assessments = $_SESSION["assessments"];
                    $assessmentId = 0;
                    $assessmentDescription = "";
                    $weight = 0;
                    if (!empty($assessments))
                    {
                        foreach($assessments as $a)
                        {
                            foreach($a as $key => $val)
                            {
                                if ($key === "assessmentId") {$assessmentId = $val;}
                                else if ($key === "assessmentDescription") {$assessmentDescription = $val;}
                                else if ($key === "weight") //weight is last key/val. Time to echo result.
                                {
                                    $weight = $val;
                                    echo
                                        "<div class=\"assessmentRow assInDB aID-$assessmentId\">
                                            <input class=\"assessmentRow-1\" type=\"number\" min=1 max=100 value='$weight'>
                                            <textarea class=\"assessmentRow-2\" rows=\"4\" maxlength=\"400\">$assessmentDescription</textarea>
                                            <div class=\"assessmentRow-3\">
                                                <button class=\"assessmentRow4-3_Btn\" type=\"button\">
                                                    <!-- Made this icon a class b/c there is more than one -->
                                                    <img class=\"icon_trash\" src=\"icons/trash.svg\">
                                                </button>
                                            </div>
                                        </div>"
                                    ;
                                }
                            }
                        }
                    }
                    else
                    {
                        echo
                            "<div class=\"assessmentRow assInDB aID-$assessmentId\">
                                <input class=\"assessmentRow-1\" type=\"number\" min=1 max=100>
                                <textarea class=\"assessmentRow-2\" rows=\"4\" maxlength=\"400\"></textarea>
                                <div class=\"assessmentRow-3\">
                                    <button class=\"assessmentRow4-3_Btn\" type=\"button\">
                                        <!-- Made this icon a class b/c there is more than one -->
                                        <img class=\"icon_trash\" src=\"icons/trash.svg\">
                                    </button>
                                </div>
                            </div>"
                        ;
                    }
                ?>

                <!-- buttons -->
                <div id="assRow5">
                    <button id="assRow5_btn" type="button">+ New</button>
                </div>

                <span id="assError"></span>

                <div id="assRow6">
                    <button id="assRow6_btn" type="submit">Save Assessments</button>
                </div>
            </form>

        </div>

        <!--narrativeSum-------------------------------------------------------------------------------->
        <div class="narrativeSum" id="narrativeSum">
            <div id="narraRow1">Narrative Summary</div>

            <div id="narraRow2">
                Please enter your narrative for each outcome, including the student strengths for
                the outcomes, student weaknesses for the outcomes, and suggested actions for
                improving student attainment of each outcome.
            </div>

            <!-- PROBLEM HERE!! My "action" doesn't have GET variables in it so my page state is lost -->
            <form id="narraRow3" action="use AJAX!" method="POST">
                <!-- text fields -->
                <?php
                    //NOTE: I think the answer to my problems with updating is to include query parameters
                    //      ..that I can extract from _GET to know whether to run updateNarratives or not.
                    //      Other, possibly bigger problem, is how to get currently selected section/outcome
                    //      ... info into the action url. Perhaps I can write some JS to somehow build
                    //      ... action url before it submits. However, I'm not sure if call back function
                    //      ... will finish before default behavior kicks in....
                    //      From some reference, looks like I can event.preventDefault, and then...
                    //      ... when I'm ready, $(form).submit() to actually submit it.
                    //      OR, is there a way to do this using only HTML?!?!? hmmmm....
                    //      Actually, it's might be best to just use a callback with ajax request. 
                    //      Look at login.js
                    //      YES!!! After looking at login.js, AJAX is the answer b/c I can get SQL/server
                    //      ... data without page refesh!!! And then I can do whatever with the return.
                    //      ... probably nothing!!! AWESOME!!!
                    // if ($_GET["some shit I put in form action url"]) {include "php_modules/updateNarrative.php";}

                    include "php_modules/narrative.php";

                    $narratives = $_SESSION["narratives"];
                    $num1 = 0;
                    $num2 = 2;
                    $narrativeNames = ["Strengths", "Weaknesses", "Actions"];
                    if (!empty($narratives))
                    {
                        foreach($narratives as $n)
                        {
                            foreach($n as $key => $val)
                            {
                                $name = $narrativeNames[$num1]; //use $num1 before it increments!!!
                                $lowerName = strtolower($name);
                                $_POST[$lowerName] = $val; //store for use in updateNarratives.php
                                $num1++;
                                $num2++;
                                echo
                                    "<div id=\"narraRow3-$num1\">
                                        <label for=\"narraRow3_textArea\">$name</label> <br>
                                        <textarea id=\"narraRow${num2}_textArea\" name='$lowerName' placeholder=\"None\" rows=\"4\" maxlength=\"2000\">$val</textarea>
                                    </div>"

                                    // "<div id=\"narraRow3-$num1\">
                                    //     <label for=\"narraRow3_textArea\">Strengths</label> <br>
                                    //     <textarea id=\"narraRow${num2}_textArea\" name=\"narraRow${num2}_textArea\" placeholder=\"None\" rows=\"4\" maxlength=\"2000\">$val</textarea>
                                    // </div>"
                                ;
                            }
                        }
                    }
                    else
                    {
                        for ($i = 0; $i < 3; $i++)
                        {
                            $name = $narrativeNames[$num1]; //use $num1 before it increments!!!
                            $lowerName = strtolower($name);
                            $_POST[$lowerName] = $val; //store for use in updateNarratives.php
                            $num1++;
                            $num2++;
                            echo
                                "<div id=\"narraRow3-$num1\">
                                    <label for=\"narraRow3_textArea\">$name</label> <br>
                                    <textarea id=\"narraRow${num2}_textArea\" name='$lowerName' placeholder=\"None\" rows=\"4\" maxlength=\"2000\"></textarea>
                                </div>"

                                // "<div id=\"narraRow3-$num1\">
                                //     <label for=\"narraRow3_textArea\">Strengths</label> <br>
                                //     <textarea id=\"narraRow${num2}_textArea\" name=\"narraRow${num2}_textArea\" placeholder=\"None\" rows=\"4\" maxlength=\"2000\"></textarea>
                                // </div>"
                            ;
                        }
                    }

                ?>

                <span id="narraErr" style="height: 0px"></span>

                <!-- save button -->
                <div id="narraRow3-4">
                    <!-- <button id="narraRow6_btn" type="submit" value="Save Narrative">Save Narrative</button> -->
                    <!-- <button type="submit" id="narraRow6_btn" form="narraRow3">Save Narrative</button> -->
                    <button type="submit" id="narraRow6_btn">Save Narrative</button>
                </div>

            </form>
        </div>

        <!--footer------------------------------------------------------------------------------
            NOTE: I'm probably not going to need this...but just in case....-->
        <div class="footer" id="footer">

        </div>
    </body>
</html>