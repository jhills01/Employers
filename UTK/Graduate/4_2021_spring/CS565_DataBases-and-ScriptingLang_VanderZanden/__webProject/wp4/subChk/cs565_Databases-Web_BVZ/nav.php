<div class="nav" id="nav">
    
    <div id="nav_Section">Section:</div>
    
    <select id="nav_DropDown">
        <?php 
            session_start();

            $login = $_SESSION["login"];

            $sectionId_0 = $login[0]["sectionId"]; 
            $major_0 = $login[0]["major"];

            foreach($login as $i)
            {
                $sectionId = $i["sectionId"]; 
                $major = $i["major"];

                $courseId = $i["courseId"];
                $semester = $i["semester"];
                $year = $i["year"];

                $option = "$courseId $semester $year $major";

                //NOTE: $option has been put in '' for value assignment. 
                //      Otherwise, value only gets assigned first item before 1st space
                echo "<option value='$option'>$option</option>";

                $options[$option] = array("sectionId" => $sectionId, "major" => $major, "courseId" => $courseId, "semester" => $semester, "year" => $year);
            }
        ?>
    </select>

    <div id="nav_Outcomes">
        <?php // display the outcomes for selected section-------------------------------------------------------
            // selectedDropDown in GET is set in nav_selectedDropDown.js
            $selectedOption = $_GET["selectedDropDown"];
            if ($selectedOption === null)
            {
                $selectedSectionId = $sectionId_0;
                $selectedMajor = $major_0;
            }
            else
            {
                $selectedSectionId = $options[$selectedOption]["sectionId"];
                $selectedMajor = $options[$selectedOption]["major"];
            }

            // set POST manually with PHP variables (JS variables require GET/POST/AJAX)
            // these POST values will be used in outcomes.php
            $_POST["sectionId"] = $selectedSectionId;
            $_POST["major"] = $selectedMajor;

            include "php_modules/outcomes.php";

            $outcomes = $_SESSION["outcomes"];

            foreach($outcomes as $i)
            {
                foreach($i as $key => $val)
                {
                    if ($key === "outcomeId") echo "<a href=\"#\">Outcome $val</a>";
                }
            }
        ?>
    </div>

</div>
